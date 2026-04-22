#ifndef PERSON_H
#define PERSON_H

#include <iostream>
#include <string>
#include <vector>
#include <fstream>

#include "date.h"
#include "contact.h"
#include "misc.h"

using namespace std;

class Person {
    friend class Network;

private:
    string f_name;
    string l_name;
    Date* birthdate = NULL;
    Phone* phone = NULL;
    Email* email = NULL;

    Person* next;
    Person* prev;

    vector<Person*> myfriends; // Phase 2 Part 2

    string id;

public:
    Person();
    ~Person();
    Person(string f_name, string l_name, string b_date, string email, string phone);
    Person(string filename);

    void print_person();
    void set_person();
    void set_person(string filename);

    bool operator==(const Person& rhs);
    bool operator!=(const Person& rhs);

    void makeFriend(Person* newFriend); // Phase 2 Part 2

    void print_friends() const; // Phase 2 Part 5
};

#endif