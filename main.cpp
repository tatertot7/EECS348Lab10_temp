#include <iostream> 
#include <fstream>   // for file stream operations
#include <string>    // for using the string class
#include "calculator.hpp"  //include custom header file for calculator functions

using namespace std;  //use the standard namespace to avoid prefixing std::

int main(int argc, char *argv[]) {
    // check if the user provided exactly one command-line argument (the file name)
    if (argc != 2) {
        cout << "Usage: " << argv[0] << " <filename>" << endl;  // Print usage message
        return 1;  //return error code
    }

    ifstream infile(argv[1]);  // open the input file using the provided filename
    if (!infile) {  // check if the file failed to open
        cout << "Failed to open file: " << argv[1] << endl;  // print error message
        return 1;  // return error code
    }

    string line;  // variable to store each line from the file
    const string base = "-123.456";  // the constant base number to add to each valid input
    int line_number = 1;  // line counter for output labeling

    // read the file line by line
    while (getline(infile, line)) {
        // output the current line number and the string read
        cout << "Line " << line_number << ": \"" << line << "\"" << endl;

        // check if the string is a valid double
        if (is_valid_double(line)) {
            // if valid, add it to the base string and output the result
            string result = add_strings(line, base);
            cout << "  Valid double. Result of addition with " << base << " is: " << result << endl;
        } else {
            // if invalid, print a message
            cout << "  Invalid double." << endl;
        }

        line_number++;  // increment line counter
    }

    return 0;  //exit successfully
}
