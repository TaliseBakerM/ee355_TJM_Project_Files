#include "network.h"

#include "misc.h"
#include <fstream>
#include <limits>
#include <vector>
#include <algorithm>
#include <utility>
#include <sstream>

Network::Network(){
    head = NULL;
    tail = NULL;
    count = 0;
}


Network::Network(string fileName){
    head = NULL;
    tail = NULL;
    count = 0;
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
        current = current->next;
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
    head = NULL;
    tail = NULL;
    count = 0;
 
    vector<pair<Person*, vector<string> > > friend_lists;
 
    string line;
    while (getline(infile, line)) {
        if (line.empty() || line == "--------------------") continue;
 
        // line is f_name at this point
        string f_name = line;
        string l_name, b_date, email_line, phone_line;
        getline(infile, l_name);
        getline(infile, b_date);
        getline(infile, email_line);
        getline(infile, phone_line);
 
        int pos = email_line.find(')');
        string email = email_line.substr(pos + 2);
        string email_type = email_line.substr(1, pos - 1);
        pos = phone_line.find(')');
        string phone = phone_line.substr(pos + 2);
        string phone_type = phone_line.substr(1, pos - 1);
 
        Person* newEntry = new Person(f_name, l_name, b_date, email, phone);
        push_front(newEntry);
 
        // Read friend IDs until "--------------------"
        vector<string> ids;
        string friends_header;
        getline(infile, friends_header); 
        if (friends_header == "Friends:") {
            string fid;
            while (getline(infile, fid) && fid != "--------------------") {
                if (!fid.empty()) ids.push_back(fid);
            }
        }
        friend_lists.push_back(make_pair(newEntry, ids));
    }
 
    // all people are now in the list, so link friends by ID
    for (int i = 0; i < (int)friend_lists.size(); i++) {
        Person* person = friend_lists[i].first;
        vector<string>& ids = friend_lists[i].second;
        for (int j = 0; j < (int)ids.size(); j++) {
            Person* friendPtr = searchbyID(ids[j]);
            if (friendPtr != NULL) {
                person->myfriends.push_back(friendPtr);
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
        outfile << current->f_name << "\n";
        outfile << current->l_name << "\n";
        outfile << current->birthdate->to_string() << "\n";
 
        string email_full = current->email->get_contact("full");
        size_t lp = email_full.find('(');
        size_t rp = email_full.find(')');
        outfile << "(" << email_full.substr(lp + 1, rp - lp - 1) << ") "
                << current->email->get_contact("short") << "\n";
 
        string phone_full = current->phone->get_contact("full");
        lp = phone_full.find('(');
        rp = phone_full.find(')');
        outfile << "(" << phone_full.substr(lp + 1, rp - lp - 1) << ") "
                << current->phone->get_contact("short") << "\n";
 
        outfile << "Friends:\n";
        for (int i = 0; i < (int)current->myfriends.size(); i++) {
            outfile << current->myfriends[i]->id << "\n";
        }
        outfile << "--------------------\n";
        current = current->next;
    }
}


void Network::printDB(){
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
    newEntry->prev = NULL;
    newEntry->next = head;

    if (head != NULL) {
        head->prev = newEntry;
    }
    else {
        tail = newEntry;
    }
    
    head = newEntry;
    count++;
}


void Network::push_back(Person* newEntry){
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
            return true;
        }
        current = current->next;
    }
    return false;
}

// Phase 3 Roommate features

/*
  loadRoommateProfiles: looks for files named "<person_id>_roommate.txt"
  in the given directory for each person in the network, and loads them.
*/
void Network::loadRoommateProfiles(string profile_dir) {
    Person* current = head;
    int loaded = 0;
    while (current != NULL) {
        string filename = profile_dir + "/" + current->id + "_roommate.txt";
        ifstream test(filename.c_str());
        if (test.good()) {
            test.close();
            current->set_roommate_profile(filename);
            loaded++;
        }
        current = current->next;
    }
    cout << "Loaded " << loaded << " roommate profile(s).\n";
}

/*
  exportCSV: writes a CSV file of all roommate profiles for the Python ML pipeline.
  People without profiles are skipped.
*/
void Network::exportCSV(string filename) const {
    ofstream out(filename.c_str());
    if (!out) {
        cout << "Error opening " << filename << " for writing.\n";
        return;
    }
    out << RoommateProfile::csv_header() << "\n";
    Person* current = head;
    int exported = 0;
    while (current != NULL) {
        if (current->has_roommate_profile()) {
            out << current->roommate_profile->to_csv_row(current->id) << "\n";
            exported++;
        }
        current = current->next;
    }
    out.close();
    cout << "Exported " << exported << " profile(s) to " << filename << "\n";
}

/*
  showTopMatches: finds the top_n most compatible people with the given person,
  using the C++ compatibility scorer. Stores results in a vector<pair<int,Person*>>
  so we can sort by score.
*/
void Network::showTopMatches(string fname, string lname, int top_n) const {
    Person* target = NULL;
    Person* current = head;
    while (current != NULL) {
        if (current->f_name == fname && current->l_name == lname) {
            target = current;
            break;
        }
        current = current->next;
    }

    if (target == NULL) {
        cout << "Person not found.\n";
        return;
    }
    if (!target->has_roommate_profile()) {
        cout << target->f_name << " " << target->l_name
             << " does not have a roommate profile.\n";
        return;
    }

    // vector of (score, Person*) pairs for sorting
    vector< pair<int, Person*> > scores;

    current = head;
    while (current != NULL) {
        if (current != target && current->has_roommate_profile()) {
            int score = target->roommate_compatibility(*current);
            scores.push_back(make_pair(score, current));
        }
        current = current->next;
    }

    // Sort descending by score
    sort(scores.begin(), scores.end(),
         [](const pair<int,Person*>& a, const pair<int,Person*>& b) {
             return a.first > b.first;
         });

    cout << "\n  Top Roommate Matches for " << fname << " " << lname << ":\n";
    cout << "  ================================================\n";

    int shown = 0;
    for (size_t i = 0; i < scores.size() && shown < top_n; i++) {
        cout << "  #" << (shown+1) << "  Compatibility: " << scores[i].first << "/100\n";
        scores[i].second->print_person();
        cout << "  ------------------------------------------------\n";
        shown++;
    }

    if (shown == 0) {
        cout << "  No other people with roommate profiles found.\n";
    }
}

// showMenu updated with phase 3 options

void Network::showMenu(){
    int opt;
    while(1){
        cout << "\033[2J\033[1;1H";
        printMe("banner");

        cout << "Select from below: \n";
        cout << "1. Save network database \n";
        cout << "2. Load network database \n";
        cout << "3. Add a new person \n";
        cout << "4. Remove a person \n";
        cout << "5. Print people with last name  \n";
        cout << "6. Connect  \n";
        cout << "7. Wise Search \n";
        cout << "8. Roommate Finder \n";
        cout << "9. Export Profiles to CSV (for ML) \n";
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
        
        string fname, lname, fileName, bdate, fname1, lname1, fname2, lname2;
        cout << "\033[2J\033[1;1H";

        if (opt==1){
            cout << "Saving network database \n";
            cout << "Enter the name of the save file: ";
            cin >> fileName;
            saveDB(fileName);
            cout << "Network saved in " << fileName << "\n";
        }
        else if (opt==2){
            cout << "Loading network database \n";
            system("ls *.txt");
            cout << "Enter the name of the load file: ";
            cin >> fileName; 
            ifstream checkFile(fileName.c_str());
            if (!checkFile) {
                cout << "File " << fileName << " does not exist!\n";
            }
            else {
                loadDB(fileName);
                cout << "Network loaded from " << fileName << " with " << count << " people \n";
            }
        }
        else if (opt == 3){
            cout << "First name: ";
            cin >> ws;
            getline(cin, fname);
            cout << "Last name: ";
            cin >> ws;
            getline(cin, lname);
            if (search(fname, lname) != NULL) {
                cout << "Person already exists! \n";
            }
            else {
                cout << "Adding a new person \n";
                Person* newPerson = new Person(fname, lname, "", "", "");
                push_front(newPerson);
                cout << "Enter the birthdate (MM/DD/YYYY): ";
                cin >> bdate;
                newPerson->birthdate = new Date(bdate);
                newPerson->email->set_contact();
                newPerson->phone->set_contact();

                // Phase 3: offer to add roommate profile
                cout << "Would you like to add a roommate profile? (y/n): ";
                string ans;
                getline(cin, ans);
                if (ans == "y" || ans == "Y") {
                    newPerson->set_roommate_profile();
                }
            }
        }
        else if (opt == 4){
            cout << "Removing a person \n";
            cout << "First name: ";
            getline(cin, fname);
            cout << "Last name: ";
            getline(cin, lname);
            if (remove(fname, lname)) {
                cout << "Remove Successful! \n";
            }
            else {
                cout << "Person not found! \n";
            }
        }
        else if (opt==5){
            cout << "Print people with last name \n";
            cout << "Last name: ";
            getline(cin, lname);
            int cnt = 0;
            Person* current = head;
            while (current != NULL) {
                if (current->l_name == lname) {
                    current->print_person();
                    cout << "------------------------------\n";
                    cnt++;
                }
                current = current->next;
            }
            if (cnt == 0) {
                cout << "Person not found! \n";
            }
        }
        else if (opt==6){
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
                    person1->print_person();
                    cout << endl;
                    person2->print_person();
                    cout << "They are now friends!\n";
                }
            }
        }
        else if (opt == 7) {
            cout << "Wise Search \n";
            cout << "\nSearch By: \n";
            string input;
            getline(cin, input);
            
            bool found = false;
            Person* current = head;
            while (current != NULL) {
                if (current->f_name == input) {found = true;}
                else if (current->l_name == input) {found = true;}
                else if (current->phone->get_contact("short") == input) {found = true;}
                else if (current->email->get_contact("short") == input) {found = true;}
                else if (current->birthdate->to_string() == input) {found = true;}
                else {
                    Date inputDate(input);
                    if (*current->birthdate == inputDate) {found = true;}
                    else {
                        string phone_digits = "";
                        for (int i = 0; i < (int)current->phone->get_contact("short").size(); i++) {
                            if (current->phone->get_contact("short")[i] != '-') {
                                phone_digits += current->phone->get_contact("short")[i];
                            }
                        }
                        if (phone_digits == input) {found = true;}
                    }
                }
                if (found) {
                    current->print_person();
                    break;
                }
                current = current->next;
            }
            if (!found) {
                cout << "Person not found\n";
            }
        }

        // Phase 3 additions
        else if (opt == 8) {
            cout << "  ===== Roommate Finder =====\n";
            cout << "  1. Set/update my roommate profile\n";
            cout << "  2. View my roommate profile\n";
            cout << "  3. Find my top matches\n";
            cout << "  4. Load all profiles from directory\n";
            cout << "  Choose: ";

            int sub;
            if (cin >> sub) {
                cin.clear();
                cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            } else {
                cin.clear();
                cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                sub = 0;
            }

            if (sub == 1) {
                cout << "  First name: ";
                getline(cin, fname);
                cout << "  Last name: ";
                getline(cin, lname);
                Person* p = search(fname, lname);
                if (p == NULL) {
                    cout << "  Person not found.\n";
                } else {
                    p->set_roommate_profile();
                }
            }
            else if (sub == 2) {
                cout << "  First name: ";
                getline(cin, fname);
                cout << "  Last name: ";
                getline(cin, lname);
                Person* p = search(fname, lname);
                if (p == NULL) {
                    cout << "  Person not found.\n";
                } else {
                    p->print_roommate_profile();
                }
            }
            else if (sub == 3) {
                cout << "  First name: ";
                getline(cin, fname);
                cout << "  Last name: ";
                getline(cin, lname);
                cout << "  How many top matches to show? (default 3): ";
                string n_str;
                getline(cin, n_str);
                int top_n = n_str.empty() ? 3 : atoi(n_str.c_str());
                showTopMatches(fname, lname, top_n);
            }
            else if (sub == 4) {
                cout << "  Profile directory (e.g. ./profiles): ";
                getline(cin, fileName);
                loadRoommateProfiles(fileName);
            }
            else {
                cout << "  Invalid option.\n";
            }
        }

        else if (opt == 9) {
            cout << "  Export roommate profiles to CSV for ML pipeline\n";
            cout << "  Output filename (e.g. profiles.csv): ";
            getline(cin, fileName);
            exportCSV(fileName);
            cout << "  Now run: python3 recommend.py " << fileName << "\n";
        }

        else {
            cout << "Nothing matched!\n";
        }

        cin.clear();
        cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        cout << "\n\nPress Enter key to go back to main menu ... ";
        string temp;
        std::getline(std::cin, temp);
        cout << "\033[2J\033[1;1H";
    }
}

// Main to run code

int main() {
    cout << "Starting main\n";
    
    Network network("networkDB.txt");
    cout << "Network created\n";

    // Load roommate profiles from ./profiles directory if it exists
    network.loadRoommateProfiles("profiles");

    network.printDB();
    cout << "Printed DB\n";

    network.saveDB("networkDB_saved.txt");
    cout << "Saved DB\n";

    network.showMenu();

    return 0;
}
