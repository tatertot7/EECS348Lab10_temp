#include "calculator.hpp" 
#include <cctype>          //for isdigit
#include <algorithm>       //for reverse
#include <iostream>

using namespace std;  //no "std::"

// function to check whether a string is a valid double format
bool is_valid_double(const string &s) {
    int i = 0, n = s.length();  //initialize index and length of string

    //skip optional '+' or '-' sign
    if (i < n && (s[i] == '+' || s[i] == '-')) i++;

    bool digits_before = false;  //flag to check if there are digits before the decimal point

    // consume digits before decimal point
    while (i < n && isdigit(s[i])) {
        i++;
        digits_before = true;  //mark that digits were found
    }

    //if there's a '.', process the decimal part
    if (i < n && s[i] == '.') {
        i++;  //skip the dot
        // must have at least one digit after the dot
        if (i == n || !isdigit(s[i])) return false;
        // consume digits after decimal point
        while (i < n && isdigit(s[i])) i++;
    }

    //return true if we've consumed the entire string and had digits before decimal
    return i == n && digits_before;
}

// pads a string with zeros on the left to make it a certain length
string pad_left(const string &s, int len) {
    string res = s;
    while ((int)res.length() < len) res = '0' + res;  // add leading zeros
    return res;
}

// pads a string with zeros on the right to make it a certain length
string pad_right(const string &s, int len) {
    string res = s;
    while ((int)res.length() < len) res += '0';  // add trailing zeros
    return res;
}

//compare two numbers' integer and fractional parts without considering signs
int compare_parts(string a_int, string a_frac, string b_int, string b_frac) {
    if (a_int.length() != b_int.length())
        return a_int.length() > b_int.length() ? 1 : -1;  //compare integer length first
    if (a_int != b_int)
        return a_int > b_int ? 1 : -1;  //compare integer values
    if (a_frac != b_frac)
        return a_frac > b_frac ? 1 : -1;  //compare fractional parts
    return 0;  //they're equal
}

// subtract b from a where both are strings of digits and a >= b
string subtract_integer_strings(const string &a, const string &b) {
    string res;
    int borrow = 0, i = a.length() - 1, j = b.length() - 1;

    while (i >= 0) {
        int x = a[i--] - '0';
        int y = (j >= 0) ? b[j--] - '0' : 0;

        int diff = x - y - borrow;
        if (diff < 0) {
            diff += 10;
            borrow = 1;
        } else
            borrow = 0;

        res.push_back(diff + '0');  // append digit result
    }

    // remove leading zeros
    while (res.size() > 1 && res.back() == '0')
        res.pop_back();

    reverse(res.begin(), res.end());  //reverse to correct order
    return res;
}

// add two unsigned integer strings
string add_integer_strings(const string &a, const string &b) {
    string res;
    int carry = 0, i = a.size() - 1, j = b.size() - 1;

    while (i >= 0 || j >= 0 || carry > 0) {
        int digit_a = (i >= 0) ? a[i--] - '0' : 0;
        int digit_b = (j >= 0) ? b[j--] - '0' : 0;
        int sum = digit_a + digit_b + carry;
        res.push_back((sum % 10) + '0');
        carry = sum / 10;
    }

    reverse(res.begin(), res.end());  //reverse result
    return res;
}

//add two signed decimal strings without converting to double
string add_strings(const string &a, const string &b) {
    // copy input strings
    string x = a, y = b;
    bool neg_x = false, neg_y = false;

    // remove optional '+' signs
    if (x[0] == '+') x = x.substr(1);
    if (y[0] == '+') y = y.substr(1);

    // check and record if input is negative
    if (x[0] == '-') {
        neg_x = true;
        x = x.substr(1);
    }
    if (y[0] == '-') {
        neg_y = true;
        y = y.substr(1);
    }

    //split into integer and fractional parts
    size_t x_dot = x.find('.');
    size_t y_dot = y.find('.');
    string x_int = (x_dot == string::npos) ? x : x.substr(0, x_dot);
    string x_frac = (x_dot == string::npos) ? "" : x.substr(x_dot + 1);
    string y_int = (y_dot == string::npos) ? y : y.substr(0, y_dot);
    string y_frac = (y_dot == string::npos) ? "" : y.substr(y_dot + 1);

    //pad fractions and integers for alignment
    int max_frac_len = max(x_frac.size(), y_frac.size());
    x_frac = pad_right(x_frac, max_frac_len);
    y_frac = pad_right(y_frac, max_frac_len);

    int max_int_len = max(x_int.size(), y_int.size());
    x_int = pad_left(x_int, max_int_len);
    y_int = pad_left(y_int, max_int_len);

    string int_res, frac_res;
    string result;
    bool result_negative = false;

    //same sign, direct addition
    if (neg_x == neg_y) {
        frac_res = add_integer_strings(x_frac, y_frac);
        int carry = 0;
        if (frac_res.length() > max_frac_len) {
            carry = 1;
            frac_res = frac_res.substr(1);  //remove overflow digit
        }
        int_res = add_integer_strings(x_int, y_int);
        if (carry) int_res = add_integer_strings(int_res, "1");

        result_negative = neg_x;  //result sign matches input
    } else {
        //opposite signs → subtraction
        int cmp = compare_parts(x_int, x_frac, y_int, y_frac);
        string a_int, a_frac, b_int, b_frac;

        if (cmp == 0) {
            return "0";  //they're equal
        } else if (cmp > 0) {
            a_int = x_int; a_frac = x_frac;
            b_int = y_int; b_frac = y_frac;
            result_negative = neg_x;
        } else {
            a_int = y_int; a_frac = y_frac;
            b_int = x_int; b_frac = x_frac;
            result_negative = neg_y;
        }

        if (a_frac < b_frac) {
            a_frac = add_integer_strings(a_frac, string(max_frac_len, '0'));
            a_int = subtract_integer_strings(a_int, "1");
        }

        string raw_frac = subtract_integer_strings(a_frac, b_frac);
        string raw_int = subtract_integer_strings(a_int, b_int);

        frac_res = pad_left(raw_frac, max_frac_len);
        int_res = raw_int;
    }

    //remove leading and trailing zeros
    while (int_res.size() > 1 && int_res[0] == '0') int_res.erase(0, 1);
    while (frac_res.size() > 0 && frac_res.back() == '0') frac_res.pop_back();

    result = int_res;
    if (!frac_res.empty()) result += "." + frac_res;
    if (result_negative && result != "0") result = "-" + result;

    return result;
}

//optional parser for debugging (not used for logic)
double parse_number(const string &expression) {
    return stod(expression);  // convert string to double (used for testing only)
}
