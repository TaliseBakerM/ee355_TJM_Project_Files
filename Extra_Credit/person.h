#ifndef PERSON_H
#define PERSON_H

#include <iostream>
#include <string>
#include <vector>
#include <fstream>

#include "date.h"
#include "contact.h"
#include "misc.h"
#include "roommate.h"   // Phase 3

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

    // Phase 3: optional roommate profile (NULL if not set)
    RoommateProfile* roommate_profile;

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

    // Phase 3: roommate profile methods
    void set_roommate_profile();                    // interactive prompt
    void set_roommate_profile(string filename);     // load from file
    void print_roommate_profile() const;
    bool has_roommate_profile() const;
    int roommate_compatibility(const Person& other) const; // 0–100
};

#endif
