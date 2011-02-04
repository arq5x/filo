#include <iostream>
#include <fstream>
#include <iomanip>
#include <vector>
#include <map>
#include <algorithm>
#include <math.h>

using namespace std;

// define our program name
#define PROGRAM_NAME "stats"

// define our parameter checking macro
#define PARAMETER_CHECK(param, paramLen, actualLen) (strncmp(argv[i], param, min(actualLen, paramLen)) == 0) && (actualLen == paramLen)

// function declarations
void ShowHelp(void);
bool isLineNumeric(string);

int main(int argc, char* argv[]) {

	// our configuration variables
	bool showHelp = false;
	
	bool isFile = false;
	string inFile;
	
	bool doAll = true;
	bool doMean = true;
	bool doGeoMean = false;
	bool doMode = false;
	bool doMedian = false;
	bool doMin = false;
	bool doMax = false;
	bool doVariance = false;
	bool doStdDev = false;
	
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

		if(PARAMETER_CHECK("-a", 2, parameterLength)) {
			doAll = true;
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
	double dLine;
	double sumOfLines = 0.0;
	double natLogSumOfLines = 0.0;
	double mean = 0.0;
	double geoMean = 0.0;
	double median = 0.0;
	double mode;
	double min, max;
	double variance;
	double stddev;
	long totalLines = 0;
	bool zeroFound = false;
	
	vector<double> linesVector;
	map<double,int> linesMap;
	
	if (isFile) {
		ifstream inF(inFile.c_str(), ios::in);
		
		// ensure that the file can be opened
		if ( !inF ) {
			cerr << "Error: The requested input file (" << inFile << ") could not be opened.  Exiting!" << endl;
			exit (1);
		}
		while (getline(inF,line)) {
			
			// increment the count of lines processed
			totalLines++;	
	
			if(line.empty()) continue;

			// TO DO: make sure the current line is numeric.  If not, write an error message and quit.
			dLine = atof(line.c_str());
			
			// increment the count of lines processed
			totalLines++;

			// add to the total sum of the lines
			sumOfLines += dLine; 
			
			// add to the sum of natural logs.  used for geometric mean
			natLogSumOfLines += log(dLine);

			// flag if we found a zero.  This prevents a geometric mean from being calculated.
			if (dLine == 0) zeroFound = true;
			
			// add the line to the vector and map
			linesVector.push_back(dLine);
			linesMap[dLine]++;
		}
		cout << "stats from: " << inFile << endl << endl;
	}
	else {
		while (getline(cin,line)) {
		
			// increment the count of lines processed
			totalLines++;
			
			if(line.empty()) continue;

			
			// TO DO: make sure the current line is numeric.  If not, write an error message and quit.
			dLine = atof(line.c_str());

			// add to the total sum of the lines
			sumOfLines += dLine; 
			
			// add to the sum of natural logs.  used for geometric mean
			natLogSumOfLines += log(dLine);

			// flag if we found a zero.  This prevents a geometric mean from being calculated.
			if (dLine == 0) zeroFound = true;
			
			// add the line to the vector and map
			linesVector.push_back(dLine);
			linesMap[dLine]++;
		}
		cout << "stats from: stdin" << endl << endl;
	}
	
	//###############################################
	// Report the results.
	//###############################################
	cout << "Total lines:\t\t" << setprecision (15) << totalLines << endl;
	cout << "Sum of lines:\t\t" << setprecision (15) << sumOfLines << endl; 
	
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
		double antiMode;
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
		float freq = count / totalLines;
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

	cout << "===============================================================================" << endl;
	cout << PROGRAM_NAME << ": compute descriptive statistics on data in a file or stdin." << endl << endl;
	cout << "Aaron Quinlan, Ph.D." << endl;
	cout << "Marth Laboratory" << endl;
	cout << "Dept. of Biology, Boston College" << endl; 
	cout << "===============================================================================" << endl << endl;

	cout << "Usage: " << PROGRAM_NAME << " <file (opt.)> [-a, -mu, -med, -mod, -min, -max, -var, -std]" << endl << endl;
	cout << "Default: " << "By default, " << PROGRAM_NAME << " will report all of the metrics below." << endl << endl;
	
	cout << "Options:" << endl;
	cout << "  -a                       Return all metrics below (default)." << endl;
	cout << "  -mu  <arithmetic mean>   Return the arithmetic mean (opt.)" << endl;
	cout << "  -med <median>            Return the median (opt.)" << endl;
	cout << "  -mod <mode>              Return the mode (opt.)" << endl;
	cout << "  -min <min>               Return the minimum value (opt.)" << endl;
	cout << "  -max <min>               Return the maximum value (opt.)" << endl;	
	cout << "  -var <variance>          Return the variance (opt.)" << endl;	
	cout << "  -std <std. dev.>         Return the standard deviation (opt.)" << endl;		
	cout << "\nHelp:" << endl;
	cout << "  -h                       Shows this help text" << endl;

	// end the program here
	exit(1);
}

// test to see if a string is numeric
inline bool isLineNumeric(string l) {
	// check to make sure there are no alphanumeric characters in the data
	for(int i = 0; i < strlen(l.c_str()); i++) {
		if (isalpha(l[i])) {
			return false;
		}
	}
	return true;
}
