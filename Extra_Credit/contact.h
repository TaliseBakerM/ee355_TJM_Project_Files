#ifndef CONTACT_H
#define CONTACT_H

#include <iostream>
#include <string>
// You may need to add other libraries here!
// For now, not adding any more libraries
using namespace std;


class Contact {
    // TODO: private or protected? Look at your children!
    // Protected because subclasses need to be able to access this information in get_contact
protected:
	string type;
public:
	virtual void print() = 0;
    virtual string get_contact(string style="full") = 0;
	virtual void set_contact() = 0;
};


class Email: public Contact{
private:
    string email_addr;
public:
    Email(string type, string email_addr);
    // TODO: Complete me!
    void set_contact();
    // Default argument here rather than in the other file
    string get_contact(string style = "full");
    void print();
};


class Phone: public Contact{
private:
    // modify dataType! Can int store 10 digit phone-number? 
    // int would not be able to store a 10 digit phone number since it maxes out at 32 bits which would be 2^32 - 1 = about 2,150,000,000 (2,147,483,647)
    // a number like 310-999-9999 would not fit so use a long instead (basically cannot use an int)
	long phone_num; 
public:
    Phone(string type, string phone_number);
    // Complete me!
    void set_contact();
    string get_contact(string style = "full");
    void print();

};

#endif