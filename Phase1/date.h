#ifndef DATE_H
#define DATE_H

#include <iostream>
#include <string>

using namespace std;

class Date {
private:
    int month;
    int day;
    int year;

    string month_to_name() const;

public:
    Date();
    Date(string date_str);

    void print_date(string style = "Month D, YYYY") const;
    bool operator==(const Date& rhs) const;
};

#endif
