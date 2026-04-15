#ifndef PERSON_H
#define PERSON_H

#include <iostream>
#include <string>
#include <vector>

#include "date.h"
#include "contact.h"

using namespace std;

class Person {
    friend class Network;

private:
    string f_name;
    string l_name;
    Date* birthdate = nullptr;
    Phone* phone = nullptr;
    Email* email = nullptr;

    vector<Person*> myfriends; // Phase 2 Part 2

    // For linked list
    Person* next;
    Person* prev;
    
    friend class Network;

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
    void makeFriend(Person* newFriend); // Phase 2 Part 2

    // Phase 2 Part 2 Testing
    void printFriends() const;
    int getFriendCount() const;
};

#endif