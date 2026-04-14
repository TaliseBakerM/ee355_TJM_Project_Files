#ifndef PERSON_H
#define PERSON_H

#include "date.h"
#include "contact.h"
#include "fstream"

#include <string>
#include <vector>

using namespace std;

class Person {
    friend class Network;

private:
    // Attributes
    string f_name;
    string l_name;
    Date *birthday;
    Email *email;
    Phone *phone;

    Person* next;
    Person* prev;

    // Adding vector of myfriends to Person class
    vector<Person*> myfriends;

public:
    // Person constructor and destructor
    Person();
    ~Person();
    Person(string filename);
    Person(string f_name, string l_name, string b_date, string email, string phone);

    void print_person();
	void set_person();
	void set_person(string filename);
    bool operator==(const Person& rhs);
    bool operator!=(const Person& rhs);

    void makeFriend(Person* newFriend); // Friend function
};

#endif