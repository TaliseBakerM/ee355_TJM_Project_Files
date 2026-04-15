#ifndef PERSON_H
#define PERSON_H

#include <iostream>
#include <string>
#include "date.h"
#include "contact.h"

using namespace std;

class Person {
private:
    string f_name;
    string l_name;
    Date* birthdate = nullptr;
    Phone* phone = nullptr;
    Email* email = nullptr;

public:
    Person();
    ~Person();
    Person(string f_name, string l_name, string b_date, string email, string phone);
    Person(string filename);

    void set_person();
    void set_person(string filename);

    bool operator==(const Person& rhs);
    bool operator!=(const Person& rhs);

    void print_person();
};

#endif
