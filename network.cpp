#include "network.h"
#include <limits>
#include "misc.h"
#include <fstream>

Network::Network(){
    head = NULL;
    tail = NULL;
    count = 0;
}


Network::Network(string fileName){
    // TODO: complete this method!
    // Implement it in one single line!
    // You may need to implement the load method before this!
    cout << "Loading network database" << endl;
    loadDB(fileName);
    cout << "Network loaded from " << fileName << " with " << count << " people" << endl;
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




void Network::loadDB(string filename){
    // TODO: Complete this method
    ifstream infile(filename);
    if (!infile) {
        cout << filename << " does not exist." << endl;
        return;
    }
    string f_name, l_name, b_date, email_line, phone_line;
    while (getline(infile, f_name)) {
        if (f_name.empty() || f_name == "--------------------") {
            continue;
        }
        else {
            getline(infile, l_name);
            getline(infile, b_date);
            getline(infile, email_line);
            getline(infile, phone_line);
        }

        // Parse email_line to get email type and email address
        int pos = email_line.find(')');
        string email = email_line.substr(pos + 1);
        string email_type = email_line.substr(0, pos + 1);
        // Parse phone_line to get phone type and phone number
        pos = phone_line.find(')');
        string phone = phone_line.substr(pos + 1);
        string phone_type = phone_line.substr(0, pos + 1);
        Person* newEntry = new Person(f_name, l_name, b_date, email, phone);
        push_front(newEntry);
    }
}

void Network::saveDB(string filename){
    // TODO: Complete this method
    ofstream outfile(filename);
    if (!outfile) {
        cout << "Error opening file for writing." << endl;
        return;
    }
    Person* current = head;
    while (current != NULL) {
        outfile << current->l_name << ", " << current->f_name << endl << endl;
        outfile << current->birthday->print_date() << endl << endl;
        outfile << current->phone->get_contact() << endl << endl;
        outfile << current->email->get_contact() << endl;
        current = current->next;
    }
}


void Network::printDB(){
    // Leave me alone! I know how to print! 
    // Note: Notice that we don't need to update this even after adding to Personattributes
    // This is a feature of OOP, classes are supposed to take care of themselves!
    cout << "Number of people: " << count << endl;
    cout << "------------------------------" << endl;
    Person* ptr = head;
    while(ptr != NULL){
        ptr->print_person();
        cout << "------------------------------" << endl;
        ptr = ptr->next;
    }
}

void Network::push_front(Person* newEntry){
    newEntry->prev = NULL;
    newEntry->next = head;

    if (head != NULL)
        head->prev = newEntry;
    else
        tail = newEntry;
    
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
    return NULL;
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
        cout << "\nSelect an option ... ";
        
        if (cin >> opt) {
            cin.clear();
            cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        } else {
            cin.clear();
            cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            cout << "Wrong option! " << endl;
            return;
        }
        
        // You may need these variables! Add more if you want!
        string fname, lname, fileName, bdate;
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
            cout << "Network saved in " << fileName << endl;
        }
        else if (opt==2){
            // TODO: Complete me!
            cout << "Loading network database \n";
            // TODO: print all the files in this same directory that have "networkDB.txt" format
            // print format: one filename one line.
            // This step just shows all the available .txt file to load.
            system("dir /b *.txt"); // For Windows
            // system("ls *.txt"); // For Linux/Mac
            cout << "Enter the name of the load file: ";
            cin >> fileName; 
            // If file with name FILENAME does not exist: 
            ifstream checkFile(fileName);
            if (!checkFile) {
                cout << "File " << fileName << " does not exist!" << endl;
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
            cin >> fname;
            cout << "Last name: ";
            cin >> lname;
            if (search(fname, lname) != NULL) {
                cout << "Person already exists! \n";
            }
            else {
                cout << "Adding a new person \n";
                Person* newPerson = new Person(fname, lname, "", "", ""); // Create a new Person with just the name, you can update the other info later
                push_front(newPerson); // Add to the front of the list
                cout << "Enter the birthdate (MM/DD/YYYY): ";
                cin >> bdate;
                newPerson->birthday = new Date(bdate);
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
            cin >> fname;
            cout << "Last name: ";
            cin >> lname;
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
            cin >> lname;
            Person* current = head;
            while (current != NULL) {
                if (current->l_name == lname) {
                    current->print_person();
                    cout << "------------------------------" << endl;
                }
                current = current->next;
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
    cout << "Starting main" << endl;
    // Testing loadDB
    Network network("networkDB.txt"); // Already loadDB in the constructor
    cout << "Network created" << endl;
    network.printDB();
    cout << "Printed DB" << endl;
    // Testing saveDB
    network.saveDB("networkDB_saved.txt");
    cout << "Saved DB" << endl;
    return 0;
}