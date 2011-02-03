#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <math.h>
#include <unistd.h> // for
using namespace std;

// define our program name
#define PROGRAM_NAME "shuffle"

// define our parameter checking macro
#define PARAMETER_CHECK(param, paramLen, actualLen) (strncmp(argv[i], param, min(actualLen, paramLen)) == 0) && (actualLen == paramLen)

// function declarations
void ShowHelp(void);

int main(int argc, char* argv[]) {

    // our configuration variables
    bool showHelp = false;

    string inFile;
    bool isFile = false;

    bool useSeed = false;
    int seed = 0;

    for(int i = 1; i < argc; i++) {
        int parameterLength = (int)strlen(argv[i]);
        if(PARAMETER_CHECK("-h", 2, parameterLength) || PARAMETER_CHECK("--help", 5, parameterLength)) {
            showHelp = true;
        }
    }

    if(showHelp) ShowHelp();

    // do some parsing (all of these parameters require 1 string---the are booleans)
    for(int i = 1; i < argc; i++) {

        string parameter = argv[i];
        int parameterLength = (int)strlen(argv[i]);

        if(PARAMETER_CHECK("-s", 2, parameterLength)) {
            useSeed = true;
            seed = atoi(argv[i+1]);
            i++;
            showHelp = true;
        }
        else if((parameterLength > 0) && (parameter[0] != '-')) {
            isFile = true;
            inFile = parameter;
        }
        else {
            cout << "ERROR: Unrecognized parameter: " << argv[i] << endl;
            showHelp = true;
        }
    }

    //###############################################
    // Main processing.
    //###############################################
    string line;
    long totalLines = 0;

    vector<string> linesVector;
    linesVector.reserve(1E6);   // allocate 1 mill lines of input.
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

    // 1. Read and store all the lines of the file.
    while (getline(*in,line)) {
        // skip if just whitespace.
        if(line.empty()) continue;
        // increment the count of lines processed
        totalLines++;
        // add the line to the vector and map
        linesVector.push_back(line);
    }

    // 2. Shuffle the input.
    // if no seed supplied, use time and the current priocess id
    if (!useSeed)
        srand((unsigned)time(0)+(unsigned)getpid());
    else
        srand(seed);

    // now shuffle the input:
    random_shuffle (linesVector.begin(), linesVector.end());

    // 3. Write the shuffled data to stdout
    for(vector<string>::const_iterator iter = linesVector.begin(); iter != linesVector.end(); ++iter) {
        printf("%s\n", (*iter).c_str());
        //cout << *iter << endl;
    }
    // exit nicely
    return 0;
}


// show the help
void ShowHelp(void) {
    cout << "===============================================================================" << endl;
    cout << PROGRAM_NAME << ": randomize (shuffle, unsort) input." << endl << endl;
    cout << "Aaron Quinlan, Ph.D." << endl;
    cout << "Marth Laboratory" << endl;
    cout << "Dept. of Biology, Boston College" << endl;
    cout << "===============================================================================" << endl << endl;
    cout << "Description: " << PROGRAM_NAME << " randomizes a file or stdin" << endl << endl;
    cout << "Usage: " << PROGRAM_NAME << " <file (opt.)> [-s <random seed>]" << endl << endl;
    cout << "Default: " << "By default, " << PROGRAM_NAME << " will use it's own seed to randomize." << endl << endl;

    cout << "Options:" << endl;
    cout << "  -s                       Custom seed for randomizing the output." << endl;
    cout << "\nHelp:" << endl;
    cout << "  -h                       Shows this help text" << endl;

    // end the program here
    exit(1);
}
