#include "contact.h"
// TODO: Add needed libraries! 

Email::Email(string type, string email_addr){
    // Done, sets type and email_addr
    this->type = type;
    this->email_addr = email_addr;
}


void Email::set_contact(){
    // Do not change the prompts!
    cout << "Enter the type of email address: ";
    // some code here
    cin >> type;
    cout << "Enter email address: ";
    // some code here
    cin >> email_addr;
}


string Email::get_contact(string style){
    // Note: We have default argument in declaration and not in definition!
    if (style=="full")
	    return "Email (" + type + "): " + email_addr;
    else 
        return email_addr;
}


void Email::print(){
    // Note: get_contact is called with default argument
	cout << get_contact() << endl;
}


Phone::Phone(string type, string num){
    // It is possible that num includes "-" or not, manage it!
    // Done: Complete this method!
    // Note: We don't want to use C++11! stol is not valid!
    this->type = type;

    // remove any - if there are any
    string digits = "";
    int length = (int)num.size();
    // loop through and check every value to see if it is a dash and add it if it is not a dash
    for (int i = 0; i < length; i++) {
        if (num[i] != '-') {
            digits += num[i];
        }
    }

    // convert it to the same type as phone number, which here is a long
    phone_num = 0;
    length = (int)digits.size();
    for (int i = 0; i < length; i++) {
        // for each digit, convert to an int using ascii conversion like we learned in ee 109
        // ex. '3' - '0' turns '3' into the integer 3 so that can be added on
        // multiplying phone_num by 10 each time moves each digit over by 1 to add the next digit
        phone_num = phone_num * 10 + (digits[i] - '0');
    }

}


void Phone::set_contact(){
    // Done: Complete this method
    // Use the same prompts as given!
	cout <<"Enter the type of phone number: ";
    cin >> type;
    string num;
	cout << "Enter the phone number: ";
    cin >> num;

    // follow the exact same converting from a string to an long procedure from the previous function (just copy pasted here)
    // remove any - if there are any
    string digits = "";
    int length = (int)num.size();
    // loop through and check every value to see if it is a dash and add it if it is not a dash
    for (int i = 0; i < length; i++) {
        if (num[i] != '-') {
            digits += num[i];
        }
    }

    // convert it to the same type as phone number, which here is a long
    phone_num = 0;
    length = (int)digits.size();
    for (int i = 0; i < length; i++) {
        // for each digit, convert to an int using ascii conversion like we learned in ee 109
        // ex. '3' - '0' turns '3' into the integer 3 so that can be added on
        // multiplying phone_num by 10 each time moves each digit over by 1 to add the next digit
        phone_num = phone_num * 10 + (digits[i] - '0');
    }
}


string Phone::get_contact(string style){
    // Done: Complete this method, get hint from Email 
    
    // Add back in any - to the number
    // start by recovering the first 3 digits using integer division to get rid of the bottom 7 digits
    long area_code = phone_num / 10000000;
    // then recover the middle 3 digits, again with integer division to get rid of the bottom 4 and a mod to get rid of the top 3
    long middle_digits = (phone_num / 10000) % 1000;
    // recover the final 4 digits by using a mod to get rid of the first 6 digits
    long last_four = phone_num % 10000;

    // now put them together into a string phone number
    string number = "";
    long parts[3] = {area_code, middle_digits, last_four};
    // also need to handle any 0 at the begginning of any of the sections
    int expected_size[3] = {3, 3, 4};

    // add each part to the string making sure that each one is the correct length
    for (int i = 0; i < 3; i++) {
        string segment = "";
        // add each number in each part to the temporary string variable
        long value = parts[i];
        for (int j = 0; j < expected_size[i]; j++) {
            // use the same ascii math from ee 109 to add either a 0 (if the current digit is a 0) or a value if the current digit has a non-zero value
            // basically, it starts with checking the right most digit of value, and adds '0' + right-most digit to the current segment, adding segment to the right accordingly
            segment = (char)('0' + value % 10) + segment;
            // decrement the value by getting rid of the right most digit with integer division
            value /= 10;
        }
        // if this is not the very first loop, add a -
        if (i > 0) {
            number += "-";
        }
        // now add on the segment we just made
        number += segment;
    }

    // from Email:
    // Note: We have default argument in declaration and not in definition!
    if (style=="full")
	    return "Phone (" + type + "): " + number;
    else 
        return number;

}


void Phone::print(){
    // Note: get_contact is called with default argument
	cout << get_contact() << endl;
}



// Code I added to test the functions
// Uncomment code to test
//int main(){
    // test the email
    //Email e1("Work", "random@usc.edu");
    //e1.print();

    // test with dashes
   // Phone p1("Home", "310-192-2011");
   // p1.print();
 
    // test without dashes
  //  Phone p2("Office", "3101922847");   
   // p2.print();
 
    // get_contact() style test
 //   cout << p1.get_contact("full") << endl;
 //   cout << p1.get_contact("a different type of contact") << endl;
 
 //   return 0;
//}