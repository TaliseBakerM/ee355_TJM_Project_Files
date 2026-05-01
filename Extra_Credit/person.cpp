#include "person.h"
#include <fstream>

// Phase 2 Part 5
#include <algorithm>
#include <cctype>

Person::Person(){
    // I'm already done! 
    roommate_profile = NULL; // Phase 3
    set_person();
}


Person::~Person(){
    delete birthdate;
    // Completed.
    delete phone;
    delete email;
    delete roommate_profile; // Phase 3
}


Person::Person(string f_name, string l_name, string b_date, string email, string phone){
    // Completed.
    birthdate = NULL;
    this->phone = NULL;
    this->email = NULL;
    roommate_profile = NULL; // Phase 3

    this->f_name = f_name;
    this->l_name = l_name;
    birthdate = new Date(b_date);
    
    // phone and email strings are in full version
    this->email = new Email("Work", email);
    this->phone = new Phone("Home", phone);
    this->id = codeName(f_name, l_name);
}


Person::Person(string filename){
    roommate_profile = NULL; // Phase 3
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

    roommate_profile = NULL; // Phase 3

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
    for (int i = 0; i < (int)myfriends.size(); i++) {
        Person* f = myfriends[i];
        cout << f->id << " (" << f->f_name << " " << f->l_name << ")" << endl;
    }

    // Phase 3: print roommate profile if present
    if (roommate_profile != NULL) {
        roommate_profile->print();
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
void Person::print_friends() const
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

// ─────────────────────────────────────────────────────────
// Phase 3: Roommate Profile Methods
// ─────────────────────────────────────────────────────────

void Person::set_roommate_profile() {
    if (roommate_profile != NULL) {
        delete roommate_profile;
    }
    roommate_profile = new RoommateProfile();

    string input;

    cout << "\n  -- Setting Roommate Profile for " << f_name << " " << l_name << " --\n";

    // Attributes
    cout << "  College (e.g. USC): ";
    getline(cin, input);
    roommate_profile->add_attribute("college", input);

    cout << "  Major: ";
    getline(cin, input);
    roommate_profile->add_attribute("major", input);

    cout << "  Year (Freshman/Sophomore/Junior/Senior): ";
    getline(cin, input);
    roommate_profile->add_attribute("year", input);

    cout << "  State (e.g. CA): ";
    getline(cin, input);
    roommate_profile->add_attribute("state", input);

    cout << "  Zip Code: ";
    getline(cin, input);
    roommate_profile->add_attribute("zip", input);

    cout << "  Sleep schedule (early bird / night owl): ";
    getline(cin, input);
    roommate_profile->add_attribute("sleep", input);

    cout << "  Cleanliness (1-10): ";
    getline(cin, input);
    roommate_profile->add_attribute("cleanliness", input);

    // Preferences
    cout << "  Enter preferences one per line (blank line to stop):\n";
    cout << "  Options: quiet hours, no smoking, pets ok, early bird, night owl, guests ok, no guests\n";
    while (true) {
        cout << "  > ";
        getline(cin, input);
        if (input.empty()) break;
        roommate_profile->add_preference(input);
    }

    // Weights
    cout << "  Rate importance of quiet (1-5): ";
    getline(cin, input);
    roommate_profile->set_weight("quiet", atoi(input.c_str()));

    cout << "  Rate importance of cleanliness (1-5): ";
    getline(cin, input);
    roommate_profile->set_weight("cleanliness", atoi(input.c_str()));

    cout << "  Rate importance of social atmosphere (1-5): ";
    getline(cin, input);
    roommate_profile->set_weight("social", atoi(input.c_str()));

    cout << "  Rate importance of studious environment (1-5): ";
    getline(cin, input);
    roommate_profile->set_weight("studious", atoi(input.c_str()));

    cout << "  Rate importance of no pets (1-5): ";
    getline(cin, input);
    roommate_profile->set_weight("pets", atoi(input.c_str()));

    cout << "  Rate importance of no smoking (1-5): ";
    getline(cin, input);
    roommate_profile->set_weight("smoking", atoi(input.c_str()));

    // Hobbies
    cout << "  Enter hobbies one per line (blank line to stop):\n";
    while (true) {
        cout << "  > ";
        getline(cin, input);
        if (input.empty()) break;
        roommate_profile->add_hobby(input);
    }

    // Dealbreakers
    cout << "  Enter dealbreakers one per line (blank line to stop):\n";
    cout << "  (e.g. smoker, loud music after midnight, no guests)\n";
    while (true) {
        cout << "  > ";
        getline(cin, input);
        if (input.empty()) break;
        roommate_profile->add_dealbreaker(input);
    }

    roommate_profile->log_activity("Profile created interactively");
    cout << "  Roommate profile saved!\n";
}

void Person::set_roommate_profile(string filename) {
    if (roommate_profile != NULL) {
        delete roommate_profile;
    }
    roommate_profile = new RoommateProfile(filename);
}

void Person::print_roommate_profile() const {
    if (roommate_profile == NULL) {
        cout << "  No roommate profile set.\n";
        return;
    }
    roommate_profile->print();
}

bool Person::has_roommate_profile() const {
    return roommate_profile != NULL;
}

int Person::roommate_compatibility(const Person& other) const {
    if (roommate_profile == NULL || other.roommate_profile == NULL) {
        return -1; // Can't score without both profiles
    }
    return roommate_profile->compatibility_score(*other.roommate_profile);
}
