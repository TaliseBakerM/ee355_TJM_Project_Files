#ifndef PERSON_H
#define PERSON_H

#include <string>
#include "contact.h"
#include "date.h"
using namespace std;

class Person {
private:
    // Attributes
    string f_name;
    string l_name;
    Phone* phone;
    Email* email;
    Date* birthday;

public:
    // Person constructor and destructor
    Person(string f_name, string l_name, string b_date, string email, string phone);
    ~Person();

    // Print function
    void print_person();
};

#endif