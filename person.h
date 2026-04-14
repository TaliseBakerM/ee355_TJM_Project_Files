#ifndef PERSON_H
#define PERSON_H

#include <string>
#include <vector>

#include "contact.h"
#include "date.h"

using namespace std;

class Person {

    friend class Network;

private:
    // Attributes
    string f_name;
    string l_name;
    Phone* phone;
    Email* email;
    Date* birthday;

    Person* next;
    Person* prev;

    // Adding vector of myfriends to Person class
    vector<Person*> myfriends;

public:
    // Person constructor and destructor
    Person(string f_name, string l_name, string b_date, string email, string phone);
    ~Person();

    // Print function
    void print_person();

    // Friend function
    void makeFriend(Person* newFriend);
};

#endif