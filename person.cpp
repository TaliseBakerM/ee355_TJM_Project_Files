#include "person.h"
#include <iostream>
using namespace std;

// Constructor to init. all desired parameters
Person::Person(string f_name, string l_name, string b_date, string email_addr, string phone_num) {
    // Store names
    this->f_name = f_name;
    this->l_name = l_name;
   
    // Dynamically allocate objects
    phone = new Phone("Home", phone_num);
    email = new Email("Work", email_addr);
    birthday = new Date(b_date);
}

// Destructor
Person::~Person() {
    delete phone;
    delete email;
    delete birthday;
}

// Prints formatted person info
void Person::print_person() {
    cout << l_name << ", " << f_name << endl << endl;
    cout << birthday->print_date() << endl << endl;
    cout << phone->get_contact() << endl << endl;
    cout << email->get_contact() << endl;
}

// Testing!
// Uncomment and run: 
// g++ contact.cpp date.cpp person.cpp -o test
// ./test  
/*
int main() {
    // Testing date
    Date d1("01/13/1961");
    cout << d1.print_date() << endl;
    
    // Testing date with diff. formatting
    Date d2("1/3/1961");
    cout << d2.print_date() << endl;
    
    Email e1("Work", "julia@wh.gov");
    e1.print();

    Phone p1("Home", "310-192-2847");
    p1.print();

    Person person1("Julia Scarlett Elizabeth", "Louis-Dreyfus",
                   "01/13/1961", "veep@wh.gov", "310-192-2847");
    cout << endl;
    
    person1.print_person();

    return 0;
}
*/