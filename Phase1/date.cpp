#include "date.h"

#include <sstream>
#include <cstdlib>

using namespace std;

Date::Date() : month(1), day(1), year(1970) {}

Date::Date(string date_str) : month(1), day(1), year(1970) {
    char slash1 = '/', slash2 = '/';
    stringstream ss(date_str);
    if (!(ss >> month >> slash1 >> day >> slash2 >> year)) {
        month = 1;
        day = 1;
        year = 1970;
    }
}

string Date::month_to_name() const {
    static const string months[] = {
        "", "January", "February", "March", "April", "May", "June",
        "July", "August", "September", "October", "November", "December"
    };

    if (month >= 1 && month <= 12) {
        return months[month];
    }
    return "Invalid";
}

void Date::print_date(string style) const {
    if (style == "Month D, YYYY") {
        cout << month_to_name() << " " << day << ", " << year << endl;
    } else {
        cout << month << "/" << day << "/" << year << endl;
    }
}

bool Date::operator==(const Date& rhs) const {
    return month == rhs.month && day == rhs.day && year == rhs.year;
}
