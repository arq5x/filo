/*****************************************************************************
  stats.cpp

  (c) 2009, 2010, 2011 - Aaron Quinlan
  Center for Public Health Genomics
  University of Virginia
  aaronquinlan@gmail.com

  Licenced under the MIT license.
******************************************************************************/
#include <iostream>
#include <fstream>
#include <iomanip>
#include <vector>
#include <map>
#include <algorithm>
#include <math.h>
#include <exception>
#include <stdexcept> // out_of_range exception
#include <cstring>
#include <climits>

#include "version.h"
#include "lineFileUtilities.h"
#include "tabFile.h"

using namespace std;

// define our program name
#define PROGRAM_NAME "stats"

// define our parameter checking macro
#define PARAMETER_CHECK(param, paramLen, actualLen) (strncmp(argv[i], param, min(actualLen, paramLen)) == 0) && (actualLen == paramLen)
#define LOOKS_LIKE_A_PARAM(string) (strlen(string)>0 && string[0]=='-')

// function declarations
void ShowHelp(void);
inline void addValue (const vector<string> &fromList, double &to, int index, int lineNum);
bool isLineNumeric(string);

int main(int argc, char* argv[]) {

    // our configuration variables
    bool showHelp = false;

    bool isFile = false;
    string inFile = "stdin";

    int column = 0;

    bool doAll      = true;
    bool doMean     = false;
    bool doGeoMean  = false;
    bool doMode     = false;
    bool doMedian   = false;
    bool doMin      = false;
    bool doMax      = false;
    bool doVariance = false;
    bool doStdDev   = false;

    //###############################################
    // Parse command line.
    //###############################################

    for(int i = 1; i < argc; i++) {
        int parameterLength = (int)strlen(argv[i]);
        if(PARAMETER_CHECK("-h", 2, parameterLength) || PARAMETER_CHECK("--help", 5, parameterLength)) {
            showHelp = true;
        }
    }
    if(showHelp) ShowHelp();

    // do some parsing (all of these parameters require 1 string---the are booleans)
    for(int i = 1; i < argc; i++) {

        int parameterLength = (int)strlen(argv[i]);
        string parameter = argv[i];

        if(PARAMETER_CHECK("-all", 4, parameterLength)) {
            doAll = true;
        }
        else if(PARAMETER_CHECK("-c", 2, parameterLength)) {
            if ((i+1) >= argc || LOOKS_LIKE_A_PARAM(argv[i+1])) {
                cerr << endl << "*****ERROR: -c parameter requires a value." << endl << endl;
                ShowHelp();
                break;
            }
            else {
                column     = atoi(argv[i + 1]) - 1;  // user is requesting 1-based columns
                i++;
            }
        }
        else if(PARAMETER_CHECK("-mu", 3, parameterLength)) {
            doMean = true;
            doAll = false;
        }
        else if(PARAMETER_CHECK("-gm", 3, parameterLength)) {
            doGeoMean = true;
            doAll = false;
        }
        else if(PARAMETER_CHECK("-med", 4, parameterLength)) {
            doMedian = true;
            doAll = false;
        }
        else if(PARAMETER_CHECK("-mod", 4, parameterLength)) {
            doMode = true;
            doAll = false;
        }
        else if(PARAMETER_CHECK("-var", 4, parameterLength)) {
            doVariance = true;
            doAll = false;
        }
        else if(PARAMETER_CHECK("-std", 4, parameterLength)) {
            doStdDev = true;
            doAll = false;
        }
        else if(PARAMETER_CHECK("-min", 4, parameterLength)) {
            doMin = true;
            doAll = false;
        }
        else if(PARAMETER_CHECK("-max", 4, parameterLength)) {
            doMax = true;
            doAll = false;
        }
        else if((parameterLength > 0) && (parameter[0] != '-')) {
            isFile = true;
            inFile = parameter;
        }
        else {
            cout << "ERROR: Unrecognized parameter: " << argv[i] << endl;
            showHelp = true;
            exit(1);
        }
    }

    //###############################################
    // Main processing.
    //###############################################
    string line;
    double sumOfLines = 0.0;
    double natLogSumOfLines = 0.0;
    double mean = 0.0;
    double geoMean = 0.0;
    double median = 0.0;
    double mode = 0.0;
    double min, max;
    double variance;
    double stddev;
    long totalLines = 0;
    bool zeroFound = false;

    // vector of strings holding the tokenized current line
    vector<string>  inFields;
    inFields.reserve(20);
    double value;
    vector<double> linesVector;
    map<double,int> linesMap;

    // 0. Are we dealing with a stream or a proper file? Default to a stream.
    istream *in = &cin;
    if (isFile) {
        ifstream *inF = new ifstream(inFile.c_str(), ios::in);
        // ensure that the file can be opened
        if ( !inF ) {
            cerr << "Error: The requested input file (" << inFile << ") could not be opened.  Exiting!" << endl;
            exit (1);
        }
        else {
            in = inF;
        }
    }

    // check the status of the current line
    TabLineStatus tabLineStatus;

    // open a new tab file, loop through it line by line
    // and summarize the data for a given group when the group
    // fields change
    int lineNum = 0;
    TabFile *_tab = new TabFile(inFile);
    _tab->Open();
    while ((tabLineStatus = _tab->GetNextTabLine(inFields, lineNum)) != TAB_INVALID) {
        if (tabLineStatus == TAB_VALID) {
            // grab the current value from the requested column
            addValue(inFields, value, column, lineNum);
            //value = atof(inFields[column].c_str());
            // increment the count of lines processed
            totalLines++;
            // add to the total sum of the lines
            sumOfLines += value;
            // add to the sum of natural logs.  used for geometric mean
            natLogSumOfLines += log(value);
            // flag if we found a zero.  This prevents a geometric mean from being calculated.
            if (value == 0) zeroFound = true;
            // add the line to the vector and map
            linesVector.push_back(value);
            linesMap[value]++;
        }
        inFields.clear();
    }
    _tab->Close();
    

    // Report the results.
    cout << "Total lines:\t\t"  << setprecision (15) << totalLines << endl;
    cout << "Sum of lines:\t\t" << setprecision (15) << sumOfLines << endl;

    if (totalLines < 1) {
        return 0;
    }
    if (doMean || doAll) {
        // compute the mean
        mean = sumOfLines / totalLines;
        cout << "Ari. Mean:\t\t" << setprecision (15) << mean << endl;
    }
    if (doGeoMean || doAll) {
        // compute the mean
        if (!zeroFound) {
            geoMean = exp(natLogSumOfLines / totalLines);
            cout << "Geo. Mean:\t\t" << setprecision (15) << geoMean << endl;
        }
        else {
            cout << "Geo. Mean:\t\t" << "undef (zero found in data)"<< endl;
        }
    }
    if (doMedian || doAll) {
        //sort the vector of values in ascending order.  We will then pick the "middle element"
        sort(linesVector.begin(), linesVector.end());

        if ((totalLines % 2) > 0) {
            long mid;
            mid = totalLines / 2;
            median = linesVector[mid];
        }
        else {
            long midLow;
            long midHigh;

            midLow = (totalLines / 2) - 1;
            midHigh = (totalLines / 2);

            median = (linesVector[midLow] + linesVector[midHigh]) / 2.0;
        }
        cout << "Median:\t\t\t" << setprecision (15) << median << endl;
    }
    if (doMode || doAll) {
        // get the mode
        int count = 0;
        int minCount = INT_MAX;
        double antiMode = 0.0;
        for(map<double,int>::const_iterator iter = linesMap.begin(); iter != linesMap.end(); ++iter) {
            if (iter->second > count) {
                mode = iter->first;
                count = iter->second;
            }
            if (iter->second < minCount) {
                antiMode = iter->first;
                minCount = iter->second;
            }
        }
        cout << "Mode:\t\t\t" << setprecision (15) << mode << " (N=" << count << ")" << endl;
        cout << "Anti-Mode:\t\t" << setprecision (15) << antiMode << " (N=" << minCount << ")" << endl;
    }
    if (doMin || doAll) {
        // get the min value
        min = *min_element(linesVector.begin(), linesVector.end());
        cout << "Minimum:\t\t" << setprecision (15) << min << endl;
    }
    if (doMax || doAll) {
        // get the min value
        max = *max_element(linesVector.begin(), linesVector.end());
        cout << "Maximum:\t\t" << setprecision (15) << max << endl;
    }
    if (doVariance || doStdDev || doAll) {
        double totalVariance = 0.0;
        for(vector<double>::const_iterator iter = linesVector.begin(); iter != linesVector.end(); ++iter) {
            totalVariance += pow((*iter - mean),2);
        }
        variance = totalVariance / totalLines;
        cout << "Variance:\t\t" << setprecision (15) << variance << endl;
    }
    if (doStdDev || doAll) {
        stddev = sqrt(variance);
        cout << "StdDev:\t\t\t" << setprecision (15) << stddev << endl;
    }

    return 0;
}


// function to show a help menu
void ShowHelp(void) {

    cerr << endl << "Program: " << PROGRAM_NAME << " (v" << VERSION << ")" << endl;

    cerr << "Author:  Aaron Quinlan (aaronquinlan@gmail.com)" << endl;

    cerr << "Summary: computes statistics on a _single column_ " << endl;
    cerr << "\tfrom a file or stream." << endl << endl;

    cerr << "Usage:\t" << PROGRAM_NAME << " [OPTIONS] [FILE]" << endl;
    cerr << "\t"      << PROGRAM_NAME << " [OPTIONS] < [FILE]" << endl;
    cerr << "\t"      << "cat [FILE] | " << PROGRAM_NAME << " [OPTIONS]" << endl << endl;

    cout << "Options:" << endl;
    cout << "  -all                     Return all metrics below (default)." << endl;
    cout << "  -mu  <arithmetic mean>   Return the arithmetic mean (opt.)" << endl;
    cout << "  -med <median>            Return the median (opt.)" << endl;
    cout << "  -mod <mode>              Return the mode (opt.)" << endl;
    cout << "  -min <min>               Return the minimum value (opt.)" << endl;
    cout << "  -max <min>               Return the maximum value (opt.)" << endl;
    cout << "  -var <variance>          Return the variance (opt.)" << endl;
    cout << "  -std <std. dev.>         Return the standard deviation (opt.)" << endl << endl;

    cout << "Help:" << endl;
    cout << "  -h                       Shows this help text" << endl;

    // end the program here
    exit(1);
}


inline void addValue (const vector<string> &fromList, double &to, int index, int lineNum) {
    try {
        to = atof(fromList.at(index).c_str());
    }
    catch(std::out_of_range& e) {
        cerr << endl << "*****" << endl << "*****ERROR: requested column exceeds the number of columns in file at line "
            << lineNum << ". Exiting." << endl << "*****" << endl;
        exit(1);
    }
}

// test to see if a string is numeric
inline bool isLineNumeric(string l) {
    // check to make sure there are no alphanumeric characters in the data
    for(size_t i = 0; i < strlen(l.c_str()); i++) {
        if (isalpha(l[i])) {
            return false;
        }
    }
    return true;
}
