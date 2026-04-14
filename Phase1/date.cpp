#include "date.h"
#include <sstream> // Needed for stringstream
using namespace std;

// Default constructor
Date::Date() {
    month = 1;
    day = 1;
    year = 1900;
}

// Constructor that parses "MM/DD/YYYY" or "M/D/YYYY"
Date::Date(string date_str) {
    month = 1;
    day = 1;
    year = 1900;

    // Using stringstream to read form string (like cin)
    char slash1, slash2;
    stringstream ss(date_str);
    ss >> month >> slash1 >> day >> slash2 >> year;
}

// Converts stored data into desired string format
string Date::print_date() const {
    string month_names[12] = {
        "January", "February", "March", "April", "May", "June", "July",
        "August", "September", "October", "November", "December"
    };

    string result = "";
    
    // Convert month number to month name
    if (month >= 1 && month <= 12) {
        result += month_names[month-1];
    } else {
        result += "Unknown";
    }
    result += " ";

    // Convert day to string 
    // Note: Got rid of leading zeros. I assumed this was fine since, as far as I could tell,
    // the project instructions did not specify if "January 03, 1961" or "January 3, 1961"
    // was preferred. 
    int d = day;
    
    if (day >= 10) {
        result += char('0' + d/10); // Tens place
    } 
    result += char('0' + d%10); // Ones place
    result += ", ";

    // Convert year to string
    int y = year;
    string year_str = "";

    if (y == 0) {
        year_str = "0";
    } else {
        while (y > 0) {
            year_str = char('0' + (y%10)) + year_str;
            y /= 10;
        }
    }
    result += year_str;
    return result;
}