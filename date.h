#ifndef DATE_H
#define DATE_H

#include <string>
using namespace std;

// Date class stores month, day, and year
class Date {
private:
	int month;
   int day;
   int year;
    
public:
   // Constructor (no input) and constructor that takes string (like "01/13/1961")
	Date();
   Date(string date_str);

   // Returns formatted date (like "January 13, 1961")
   string print_date() const;
};

#endif