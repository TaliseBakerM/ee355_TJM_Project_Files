#ifndef ROOMMATE_H
#define ROOMMATE_H

#include <iostream>
#include <string>
#include <vector>    // ordered preferences
#include <deque>     // activity log (sliding window)
#include <list>      // hobbies (sorted insertion)
#include <set>       // dealbreakers (unique, auto-sorted)
#include <map>       // key-value profile attributes
#include <unordered_map> // preference weights (O(1) lookup)

using namespace std;

class RoommateProfile {
public:
    // --- STL containers, each chosen for a reason ---

    // map: ordered key-value store for structured attributes
    // (college, major, year, state, zip, sleep schedule, cleanliness level)
    map<string, string> attributes;

    // vector: ordered list of lifestyle preferences (order matters — user ranks them)
    vector<string> preferences;

    // deque: activity/interaction log — new entries pushed to front,
    // old ones popped from back to keep a fixed-size recent history
    deque<string> activity_log;
    static const int LOG_MAX = 5;

    // list: hobbies — supports efficient sorted insertion anywhere in sequence
    list<string> hobbies;

    // set: dealbreakers — unique values, auto-sorted, fast membership test
    set<string> dealbreakers;

    // unordered_map: preference weights — O(1) average lookup by key
    // keys: "quiet", "cleanliness", "social", "studious", "pets", "smoking"
    unordered_map<string, int> pref_weights;

    // --- Constructors ---
    RoommateProfile();
    RoommateProfile(string filename);

    // --- I/O ---
    void load(string filename);
    void save(ofstream& out) const;
    void print() const;

    // --- Mutators ---
    void add_attribute(string key, string value);
    void add_preference(string pref);
    void log_activity(string entry);
    void add_hobby(string hobby);       // inserts in sorted order into list
    void add_dealbreaker(string item);
    void set_weight(string key, int weight);

    // --- Compatibility scoring ---
    // Returns a score 0–100 comparing this profile to another
    int compatibility_score(const RoommateProfile& other) const;

    // Operator overload: > means "this profile is more compatible with ref than other is"
    bool operator>(const RoommateProfile& other) const;

    // --- CSV export for Python ML pipeline ---
    string to_csv_row(string person_id) const;
    static string csv_header();
};

#endif
