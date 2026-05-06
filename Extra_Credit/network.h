#ifndef NETWORK_H
#define NETWORK_H

#include "person.h"
#include "contact.h"
#include "date.h"
#include "roommate.h"
#include <stdlib.h>
#include <vector>
#include <utility>  

class Network{

    private:
        Person* head;
        Person* tail;
        int count; 
        Person* search(Person* searchEntry);
        Person* search(string fname, string lname);
        Person* searchbyID(string id);

    public:
        Network();
        Network(string fileName);
        ~Network();
        void loadDB(string filename);
        void saveDB(string filename);
        void printDB();
        void push_front(Person* newEntry);
        void push_back(Person* newEntry);
        bool remove(string fname, string lname);
        void showMenu();

        // Phase 3: roommate features
        void loadRoommateProfiles(string profile_dir);
        void exportCSV(string filename) const;
        void showTopMatches(string fname, string lname, int top_n = 3) const;
};

#endif
