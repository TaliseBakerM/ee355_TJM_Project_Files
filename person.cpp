#include "person.h"
#include <fstream>

// Phase 2 Part 5
#include <algorithm>
#include <cctype>

Person::Person(){
    // I'm already done! 
    set_person();
}


Person::~Person(){
    delete birthdate;
    // Completed.
    delete phone;
    delete email;
}


Person::Person(string f_name, string l_name, string b_date, string email, string phone){
    // Completed.
    birthdate = NULL;
    this->phone = NULL;
    this->email = NULL;

    this->f_name = f_name;
    this->l_name = l_name;
    birthdate = new Date(b_date);
    
    // phone and email strings are in full version
    this->email = new Email("Work", email);
    this->phone = new Phone("Home", phone);
    this->id = codeName(f_name, l_name);
}


Person::Person(string filename){
    set_person(filename);
}


void Person::set_person(){
    // prompts for the information of the user from the terminal
    // first/last name can have spaces!
    // date format must be "M/D/YYYY"
    // We are sure user enters info in correct format.
    // Completed.
    
    string temp;
    string type;

    cout << "First Name: ";
    // pay attention to how we read first name, as it can have spaces!
    std::getline(std::cin,f_name);

	cout << "Last Name: ";
    std::getline(std::cin,l_name);

    cout << "Birthdate (M/D/YYYY): ";
    std::getline(std::cin,temp);
    // pay attention to how we passed argument to the constructor of a new object created dynamically using new command
    birthdate = new Date(temp); 

    cout << "Type of email address: ";
    std::getline(std::cin, type);
    
    cout << "Email address: ";
    std::getline(std::cin, temp);
    email = new Email(type, temp);

    cout << "Type of phone number: ";
    std::getline(std::cin, type);
    
    cout << "Phone number: ";
    std::getline(std::cin, temp);
    phone = new Phone(type, temp);
}


void Person::set_person(string filename){
    // reads a Person from a file
    // Look at person_template files as examples.     
    // Phone number in files can have '-' or not.
    // TODO: Completed.

    std::ifstream fin(filename.c_str());

    string line;
    string type;
    string value;
    size_t left;
    size_t right;

    std::getline(fin, f_name);
    std::getline(fin, l_name);

    std::getline(fin, line);
    birthdate = new Date(line);

    std::getline(fin, line);
    left = line.find('(');
    right = line.find(')');
    type = line.substr(left + 1, right - left - 1);
    value = line.substr(right + 2);
    phone = new Phone(type, value);

    std::getline(fin, line);
    left = line.find('(');
    right = line.find(')');
    type = line.substr(left + 1, right - left - 1);
    value = line.substr(right + 2);
    email = new Email(type, value);

    fin.close();
}


bool Person::operator==(const Person& rhs){
    // Completed.
    // Note: you should check first name, last name and birthday between two persons
    // refer to bool Date::operator==(const Date& rhs)
    return f_name == rhs.f_name && l_name == rhs.l_name && (*birthdate == *rhs.birthdate);
}

bool Person::operator!=(const Person& rhs){ 
    // Completed.
    return !(*this == rhs);
}


void Person::print_person(){
    // Already implemented for you! Do not change!
	cout << l_name <<", " << f_name << endl;
	birthdate->print_date("Month D, YYYY");
    phone->print();
    email->print();

    // Phase 2 Part 4 Addition
    for (int i = 0; i < myfriends.size(); i++) {
        Person* f = myfriends[i];
        cout << f->id << " (" << f->f_name << " " << f->l_name << ")" << endl;
    }
}

// Phase 2: Part 2 Implementation
// Adds a Person pointer to friend list
void Person::makeFriend(Person* newFriend) {
    if (newFriend == NULL) { // Check if pointer is null (invalid person)
        return;
    }

    if (newFriend == this) { // Person cannot be friends with themselves
        return;
    }

    // Check if friend is already in the list
    for (size_t i = 0; i < myfriends.size(); i++) {
        if (myfriends[i] == newFriend) { // Friend already exists so we don't need to add again
            return;
        }
    }

    // Otherwise, add new friend to the vector
    myfriends.push_back(newFriend);
}

// Phase 2 Part 5: Sorting Friends
void Person::printFriends() const
{
    vector<Person*> sortedFriends = myfriends;

    sort(sortedFriends.begin(), sortedFriends.end(),
        [](const Person* a, const Person* b)
        {
            string ac = a->id;
            string bc = b->id;

            // Compare first letter
            char a1 = (ac.size() > 0) ? tolower(ac[0]) : '\0';
            char b1 = (bc.size() > 0) ? tolower(bc[0]) : '\0';
            if (a1 != b1) return a1 < b1;

            // Compare second letter
            char a2 = (ac.size() > 1) ? tolower(ac[1]) : '\0';
            char b2 = (bc.size() > 1) ? tolower(bc[1]) : '\0';
            if (a2 != b2) return a2 < b2;

            // If both letters match, any order is allowed
            return ac < bc;
        });

    cout << "--------------------------------" << endl << endl;

    for (size_t i = 0; i < sortedFriends.size(); i++)
    {
        cout << sortedFriends[i]->f_name << ", "
             << sortedFriends[i]->l_name << endl << endl;
    }
}

// Testing!
// Uncomment and run: 
// g++ contact.cpp date.cpp person.cpp -o test
// ./test  
/*
int main() {

    cout << "---- Testing Date ----\n";
    Date d1("01/13/1961");
    d1.print_date("Month D, YYYY");

    Date d2("1/7/1961");
    d2.print_date("Month D, YYYY");

    cout << "\n---- Testing Email & Phone ----\n";
    Email e1("Work", "veep@wh.gov");
    e1.print();

    Phone p1("Home", "310-192-2847");
    p1.print();

    cout << "\n---- Testing Person ----\n";
    Person person1("Julia Scarlett Elizabeth", "Louis-Dreyfus",
                   "01/13/1961", "veep@wh.gov", "310-192-2847");

    person1.print_person();

    return 0;
}
*/

// Testing for Phase 2 Part 5
// Run: 
// g++ -std=c++11 person.cpp misc.cpp -o test 
// ./test

#include <iostream>
using namespace std;

int main()
{
    // Create the main person using the 5-argument constructor
    Person tommy("Tommy", "Trojan", "01/13/1961", "tommy@usc.edu", "310-555-0100");

    // Create friends
    Person karan("Karan", "Kapoor", "01/01/2000", "karan@usc.edu", "310-555-0101");
    Person katy("Katy", "Bruin", "01/01/2000", "katy@usc.edu", "310-555-0102");
    Person amy("Amy", "Chen", "01/01/2000", "amy@usc.edu", "310-555-0103");
    Person kurt("Kurt", "Cobain", "01/01/2000", "kurt@usc.edu", "310-555-0104");

    // Add friends 
    tommy.makeFriend(&karan);
    tommy.makeFriend(&katy);
    tommy.makeFriend(&amy);
    tommy.makeFriend(&kurt);

    cout << "Tommy, Trojan" << endl;

    // Call print_friends function
    tommy.print_friends();

    return 0;
}