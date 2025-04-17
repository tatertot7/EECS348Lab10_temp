#ifndef __CALCULATOR_HPP
#define __CALCULATOR_HPP
#include <string>
using namespace std;

// validates if a string is a properly formatted double number
bool is_valid_double(const string &expression);

// adds two double numbers represented as strings
string add_strings(const string &a, const string &b);

// helper to parse a number (already validated string), converts to actual double (used if needed)
double parse_number(const string &expression);

#endif // __CALCULATOR_HPP
