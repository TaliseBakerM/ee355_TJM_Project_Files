#include "network.h"

#include "misc.h"
#include <fstream>
#include <limits>

Network::Network(){
    head = NULL;
    tail = NULL;
    count = 0;
}


Network::Network(string fileName){
    // TODO: complete this method!
    // Implement it in one single line!
    // You may need to implement the load method before this!
    cout << "Loading network database\n";
    loadDB(fileName);
    cout << "Network loaded from " << fileName << " with " << count << " people\n";
}

Network::~Network(){ 
    Person* current = head;
    while (current != NULL) {
        Person* temp = current;
        current = current->next;
        delete temp;
    }
}

Person* Network::search(Person* searchEntry){
    // Searches the Network for searchEntry
    // if found, returns a pointer to it, else returns NULL
    // TODO: Complete this method
    Person* current = head;
    while (current != NULL) {
        if (searchEntry == current) {
            return current;
        }
        current = current->next;
    }
    return NULL;
}


Person* Network::search(string fname, string lname){
    // New == for Person, only based on fname and lname
    // if found, returns a pointer to it, else returns NULL
    // TODO: Complete this method
    // Note: two ways to implement this, 1st making a new Person with fname and lname and and using search(Person*), 2nd using fname and lname directly. 
    Person* current = head;
    while (current != NULL) {
        if (current->f_name == fname && current->l_name == lname) {
            return current;
        }
        current = current->next;
    }
    return NULL;
}


Person* Network::searchbyID(string id) {
    Person* current = head;
    while (current != NULL) {
        if (current->id == id) {
            return current;
        }
    }
    return NULL;
}


void Network::loadDB(string filename){
    // TODO: Complete this method
    ifstream infile(filename.c_str());
    if (!infile) {
        cout << filename << " does not exist.\n";
        return;
    }
    head=NULL;
    tail=NULL;
    count=0;
    string f_name, l_name, b_date, email_line, phone_line, friends_line;
    while (getline(infile, f_name)) {
        // cout << "Read first name: " << f_name << "\n";
        if (f_name.empty() || f_name == "--------------------") {
            // cout << "Skipping empty line or delimiter\n";
            continue;
        }
        else {
            getline(infile, l_name);
            // cout << "Read last name: " << l_name << "\n";
            getline(infile, b_date);
            // cout << "Read birthdate: " << b_date << "\n";
            getline(infile, email_line);
            //cout << "Read email line: " << email_line << "\n";
            getline(infile, phone_line);
            // cout << "Read phone line: " << phone_line << "\n";
        }

        // Parse email_line to get email type and email address
        int pos = email_line.find(')');
        string email = email_line.substr(pos + 2); // +2 to skip the whitespace after )
        string email_type = email_line.substr(1, pos - 1);
        pos = phone_line.find(')');
        string phone = phone_line.substr(pos + 2);
        string phone_type = phone_line.substr(1, pos - 1);
        Person* newEntry = new Person(f_name, l_name, b_date, email, phone);
        push_front(newEntry);

        // Read friends line and add friends (NEED TO EDIT)
        getline(infile, friends_line);
        if (friends_line == "Friends: ") {
            while (getline(infile, friends_line) && !friends_line.empty()) {
                Person* friendPtr = searchbyID(friends_line);
                if (friendPtr != NULL) {
                    newEntry->myfriends.push_back(friendPtr);
                }
            }
        }
    }
}

void Network::saveDB(string filename){
    // TODO: Complete this method
    ofstream outfile(filename.c_str());
    if (!outfile) {
        cout << "Error opening file for writing.\n";
        return;
    }
    Person* current = head;
    while (current != NULL) {
        outfile << current->l_name << ", " << current->f_name << "\n" << "\n";
        outfile << current->birthdate->to_string() << "\n" << "\n"; // Changed print_date to to_string during debugging
        outfile << current->phone->get_contact() << "\n" << "\n";
        outfile << current->email->get_contact() << "\n" << "\n";

        outfile << "Friends: " << "\n" << "\n";
        for (int i=0; i<current->myfriends.size(); i++) {
            outfile << current->myfriends[i]->id << "\n";
        }
        outfile << "\n";
        current = current->next;
    }
}


void Network::printDB(){
    // Leave me alone! I know how to print! 
    // Note: Notice that we don't need to update this even after adding to Personattributes
    // This is a feature of OOP, classes are supposed to take care of themselves!
    cout << "Number of people: " << count << "\n";
    cout << "------------------------------\n";
    Person* ptr = head;
    while(ptr != NULL){
        ptr->print_person();
        cout << "------------------------------\n";
        ptr = ptr->next;
    }
}

void Network::push_front(Person* newEntry){
    // cout << "push_front START, newEntry=" << newEntry << " head=" << head << "\n";
    newEntry->prev = NULL;
    // cout << "push_front successfully set newEntry->prev = NULL\n";
    newEntry->next = head;
    // cout << "push_front successfully set newEntry->next = head\n";

    if (head != NULL) {
        head->prev = newEntry;
        // cout << "push_front head is not NULL, head=" << head << "\n";
    }
    else {
        tail = newEntry;
        // cout << "push_front head is NULL, set tail = newEntry\n";
    }
    
    head = newEntry;
    count++;
}


void Network::push_back(Person* newEntry){
    // Adds a new Person (newEntry) to the back of LL
    // TODO: Complete this method
    newEntry->prev = tail;
    newEntry->next = NULL;

    if (tail != NULL) {
        tail->next = newEntry;
    }
    else {
        head = newEntry;
    }
    tail = newEntry;
    count++;
}


bool Network::remove(string fname, string lname){
    // TODO: Complete this method
    Person* current = head;
    while (current != NULL) {
        if (current->f_name == fname && current->l_name == lname) {
            if (current->prev != NULL) {
                current->prev->next = current->next;
            }
            else {
                head = current->next;
            }
            if (current->next != NULL) {
                current->next->prev = current->prev;
            }
            else {
                tail = current->prev;
            }
            delete current;
            count--;
            return true; // Remove successful
        }
        current = current->next;
    }
    return false;
}


void Network::showMenu(){
    // TODO: Complete this method!
    // All the prompts are given to you, 
    // You should add code before, between and after prompts!

    int opt;
    while(1){
        cout << "\033[2J\033[1;1H";
        printMe("banner"); // from misc library

        cout << "Select from below: \n";
        cout << "1. Save network database \n";
        cout << "2. Load network database \n";
        cout << "3. Add a new person \n";
        cout << "4. Remove a person \n";
        cout << "5. Print people with last name  \n";
        cout << "6. Connect  \n";
        cout << "\nSelect an option ... ";
        
        if (cin >> opt) {
            cin.clear();
            cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        } else {
            cin.clear();
            cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            cout << "Wrong option! \n";
            return;
        }
        
        // You may need these variables! Add more if you want!
        string fname, lname, fileName, bdate, fname1, lname1, fname2, lname2;
        cout << "\033[2J\033[1;1H";

        if (opt==1){
            // TODO: Complete me!
            cout << "Saving network database \n";
            cout << "Enter the name of the save file: ";
            // Save the network database into the file with the given name,
            // with each person saved in the format the save as printing out the person info,
            // and people are delimited similar to "networkDB.txt" format
            cin >> fileName;
            saveDB(fileName);
            cout << "Network saved in " << fileName << "\n";
        }
        else if (opt==2){
            // TODO: Complete me!
            cout << "Loading network database \n";
            // TODO: print all the files in this same directory that have "networkDB.txt" format
            // print format: one filename one line.
            // This step just shows all the available .txt file to load.
            //system("dir /b *.txt"); // For Windows
            system("ls *.txt"); // For Linux/Mac
            cout << "Enter the name of the load file: ";
            cin >> fileName; 
            // If file with name FILENAME does not exist: 
            ifstream checkFile(fileName.c_str());
            if (!checkFile) {
                cout << "File " << fileName << " does not exist!\n";
            }
            else {
            // If file is loaded successfully, also print the count of people in it: 
            loadDB(fileName);
            cout << "Network loaded from " << fileName << " with " << count << " people \n";
            }
        }
        else if (opt == 3){
            // TODO: Complete me!
            // TODO: use push_front, and not push_back 
            // Add a new Person ONLY if it does not exists!
            cout << "First name: ";
            cin >> ws;
            getline(cin, fname); // Need to add this because otherwise, the last name will automatically be detected as what is after the whitespace.
            cout << "Last name: ";
            cin >> ws;
            getline(cin, lname);
            if (search(fname, lname) != NULL) {
                cout << "Person already exists! \n";
            }
            else {
                cout << "Adding a new person \n";
                Person* newPerson = new Person(fname, lname, "", "", ""); // Create a new Person with just the name, you can update the other info later
                push_front(newPerson); // Add to the front of the list
                cout << "Enter the birthdate (MM/DD/YYYY): ";
                cin >> bdate;
                newPerson->birthdate = new Date(bdate);
                newPerson->email->set_contact();
                newPerson->phone->set_contact();
            }
        }
        else if (opt == 4){
            // TODO: Complete me!
            // if found, cout << "Remove Successful! \n";
            // if not found: cout << "Person not found! \n";
            cout << "Removing a person \n";
            cout << "First name: ";
            getline(cin, fname);
            //cin >> fname;
            cout << "Last name: ";
            getline(cin, lname); // handles lastnames with more than one word
            //cin >> lname;
            if (remove(fname, lname)) {
                cout << "Remove Successful! \n";
            }
            else {
                cout << "Person not found! \n";
            }
        }
        else if (opt==5){
            // TODO: Complete me!
            // print the people with the given last name
            // if not found: cout << "Person not found! \n";
            cout << "Print people with last name \n";
            cout << "Last name: ";
            getline(cin, lname); // handles lastnames with more than one word
            // cin >> lname;
            int count = 0;
            Person* current = head;
            while (current != NULL) {
                if (current->l_name == lname) {
                    current->print_person();
                    cout << "------------------------------\n";
                    count++;
                }
                current = current->next;
            }
            if (count == 0) {
                cout << "Person not found! \n";
            }
        }
        else if (opt==6){
            // Phase 2 Part 1: Add new option in Main Menu
            // Note: Replaced count w/ person1 and person2 pointers for cleanliness
            cout << "Make friends: " << endl;

            cout << "Person 1" << endl;
            cout << "First name: ";
            getline(cin, fname1);

            cout << "Last name: ";
            getline(cin, lname1);

            Person* person1 = search(fname1, lname1);
            if (person1 == NULL) {
                cout << "Person not found! \n";
            }
            else {
                cout << "Person 2" << endl;
                cout << "First name: ";
                getline(cin, fname2);

                cout << "Last name: ";
                getline(cin, lname2);

                Person* person2 = search(fname2, lname2);
                if (person2 == NULL) {
                    cout << "Person not found! \n";
                }
                else {
                    person1->makeFriend(person2);
                    person2->makeFriend(person1);

                    cout << "They are now friends!\n"; // Checking
                }
            }
        }
        else
            cout << "Nothing matched!\n";

        cin.clear();
        cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        cout << "\n\nPress Enter key to go back to main menu ... ";
        string temp;
        std::getline (std::cin, temp);
        cout << "\033[2J\033[1;1H";
    }
}

// Testing!
// Uncomment and run: 
// g++ network.cpp contact.cpp date.cpp misc.cpp person.cpp -o test
// ./test  

int main() {
    cout << "Starting main\n";
    
    // Testing loadDB
    Network network("networkDB.txt"); // Already loadDB in the constructor
    cout << "Network created\n";
    network.printDB();
    cout << "Printed DB\n";
    
    // Testing saveDB
    network.saveDB("networkDB_saved.txt");
    cout << "Saved DB\n";

    // Testing Phase 2 Part 2 w/ showMenu
    // Select (6) when prompted by Menu and enter: Truman, Burbank, Martin, Van Nostrand
    // Result should say "They are now friends!"
     
    //cout << "Testing Friend Connection\n";
    network.showMenu();

    return 0;
}
