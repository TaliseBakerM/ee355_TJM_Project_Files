#include "roommate.h"
#include <fstream>
#include <sstream>
#include <algorithm>
#include <cmath>

// Constructors

RoommateProfile::RoommateProfile() {
    // Default weights, user can override
    pref_weights["quiet"]       = 3;
    pref_weights["cleanliness"] = 3;
    pref_weights["social"]      = 3;
    pref_weights["studious"]    = 3;
    pref_weights["pets"]        = 2;
    pref_weights["smoking"]     = 5; // smoking is a strong dealbreaker by default cause we hate smoking
}

// Build profile and loads saved data from file 
RoommateProfile::RoommateProfile(string filename) : RoommateProfile() {
    load(filename);
}

/*
  File format:
  
  [ATTRIBUTES]
  college=USC
  major=Electrical Engineering
  year=Junior
  state=CA
  zip=90007
  sleep=night owl
  cleanliness=7
  [PREFERENCES]
  quiet hours
  no smoking
  [WEIGHTS]
  quiet=5
  cleanliness=4
  social=2
  studious=3
  pets=1
  smoking=5
  [HOBBIES]
  guitar
  hiking
  [DEALBREAKERS]
  smoker
  loud music after midnight
  [LOG]
  Viewed profile of julialouisdreyfus
  [END]
*/

void RoommateProfile::load(string filename) {
    ifstream fin(filename.c_str());
    if (!fin) return;

    string line;
    string section = "";

    // Read file line by line
    while (getline(fin, line)) {
        if (line.empty()) continue;

        // Section header detection (since they begin w/ '[')
        if (line[0] == '[') {
            section = line;
            continue;
        }

        if (section == "[ATTRIBUTES]") {
            size_t eq = line.find('=');
            if (eq != string::npos) {
                string key = line.substr(0, eq);
                string val = line.substr(eq + 1);
                attributes[key] = val;
            }
        }
        else if (section == "[PREFERENCES]") {
            preferences.push_back(line);
        }
        else if (section == "[WEIGHTS]") {
            size_t eq = line.find('=');
            if (eq != string::npos) {
                string key = line.substr(0, eq);
                int val = atoi(line.substr(eq + 1).c_str());
                pref_weights[key] = val;
            }
        }
        else if (section == "[HOBBIES]") {
            add_hobby(line); // sorted insertion
        }
        else if (section == "[DEALBREAKERS]") {
            dealbreakers.insert(line);
        }
        else if (section == "[LOG]") {
            log_activity(line);
        }
    }
    fin.close();
}

// Save current profile back to file using same section format
void RoommateProfile::save(ofstream& out) const {
    out << "[ATTRIBUTES]\n";
    for (map<string,string>::const_iterator it = attributes.begin(); it != attributes.end(); ++it) {
        out << it->first << "=" << it->second << "\n";
    }

    out << "[PREFERENCES]\n";
    for (size_t i = 0; i < preferences.size(); i++) {
        out << preferences[i] << "\n";
    }

    out << "[WEIGHTS]\n";
    // Use a sorted map to sort by weights
    map<string,int> sorted_weights(pref_weights.begin(), pref_weights.end());
    for (map<string,int>::const_iterator it = sorted_weights.begin(); it != sorted_weights.end(); ++it) {
        out << it->first << "=" << it->second << "\n";
    }

    out << "[HOBBIES]\n";
    for (list<string>::const_iterator it = hobbies.begin(); it != hobbies.end(); ++it) {
        out << *it << "\n";
    }

    out << "[DEALBREAKERS]\n";
    for (set<string>::const_iterator it = dealbreakers.begin(); it != dealbreakers.end(); ++it) {
        out << *it << "\n";
    }

    out << "[LOG]\n";
    for (size_t i = 0; i < activity_log.size(); i++) {
        out << activity_log[i] << "\n";
    }

    out << "[END]\n";
}

// Print
void RoommateProfile::print() const {
    cout << "  -- Roommate Profile --\n";

    // Attributes (sorted by key)
    cout << "  Attributes:\n";
    for (map<string,string>::const_iterator it = attributes.begin(); it != attributes.end(); ++it) {
        cout << "    " << it->first << ": " << it->second << "\n";
    }

    // Preferences (sorted by order entered in)
    cout << "  Preferences:\n";
    for (size_t i = 0; i < preferences.size(); i++) {
        cout << "    " << (i+1) << ". " << preferences[i] << "\n";
    }

    // Weights (unordered, printed via sorted copy)
    cout << "  Preference Weights:\n";
    map<string,int> sorted_w(pref_weights.begin(), pref_weights.end());
    for (map<string,int>::const_iterator it = sorted_w.begin(); it != sorted_w.end(); ++it) {
        cout << "    " << it->first << " -> " << it->second << "/5\n";
    }

    // Hobbies (sorted)
    cout << "  Hobbies: ";
    for (list<string>::const_iterator it = hobbies.begin(); it != hobbies.end(); ++it) {
        cout << *it;
        list<string>::const_iterator next_it = it;
        ++next_it;
        if (next_it != hobbies.end()) cout << ", ";
    }
    cout << "\n";

    // Dealbreakers (unique, sorted)
    cout << "  Dealbreakers: ";
    for (set<string>::const_iterator it = dealbreakers.begin(); it != dealbreakers.end(); ++it) {
        cout << *it;
        set<string>::const_iterator next_it = it;
        ++next_it;
        if (next_it != dealbreakers.end()) cout << ", ";
    }
    cout << "\n";

    // Activity log (deque, recent first)
    cout << "  Recent Activity:\n";
    for (size_t i = 0; i < activity_log.size(); i++) {
        cout << "    [" << i+1 << "] " << activity_log[i] << "\n";
    }
}

// Add/update attribute
void RoommateProfile::add_attribute(string key, string value) {
    attributes[key] = value;
}

// Add preference to list
void RoommateProfile::add_preference(string pref) {
    preferences.push_back(pref);
}

// Add log entry to front of activity history
// If log too large, drop oldest entry
void RoommateProfile::log_activity(string entry) {
    activity_log.push_front(entry); // newest at front
    if ((int)activity_log.size() > LOG_MAX) {
        activity_log.pop_back(); // drop oldest
    }
}

// Add hobby into list (keeping sorted)
void RoommateProfile::add_hobby(string hobby) {
    // Sorted insertion into list
    list<string>::iterator it = hobbies.begin();
    while (it != hobbies.end() && *it < hobby) ++it;
    hobbies.insert(it, hobby);
}

// Add dealbreaker to set
void RoommateProfile::add_dealbreaker(string item) {
    dealbreakers.insert(item);
}

// Set importance/weight for one preference category
void RoommateProfile::set_weight(string key, int weight) {
    pref_weights[key] = weight;
}

// Compatibility Scoring

/*
  Scoring breakdown (total 100 pts):
    - Attribute matches (college, state, zip, sleep, cleanliness): up to 40 pts
    - Shared hobbies: up to 20 pts
    - Dealbreaker conflicts (penalizes): up to -30 pts
    - Preference overlap (weighted): up to 30 pts
    - Shared preferences: up to 10 pts
*/

int RoommateProfile::compatibility_score(const RoommateProfile& other) const {
    double score = 0.0;

    // 1. Attribute matching (40 pts)
    // Each attribute contributes a diff. # of pts
    struct AttrWeight { string key; double pts; };
    AttrWeight attr_weights[] = {
        {"college",     12.0},
        {"state",        6.0},
        {"zip",          5.0},
        {"sleep",        9.0},
        {"cleanliness",  8.0}
    };
    int n_attrs = 5;
    for (int i = 0; i < n_attrs; i++) {
        map<string,string>::const_iterator a = attributes.find(attr_weights[i].key);
        map<string,string>::const_iterator b = other.attributes.find(attr_weights[i].key);
        
        // Only compare if both profiles have that field
        if (a != attributes.end() && b != other.attributes.end()) {
            if (a->second == b->second) {
                score += attr_weights[i].pts; // Exact match = full pts
            } else if (attr_weights[i].key == "cleanliness") {
                // Partial credit for close cleanliness scores
                int diff = abs(atoi(a->second.c_str()) - atoi(b->second.c_str()));
                if (diff <= 2) score += attr_weights[i].pts * (1.0 - diff / 4.0);
            }
        }
    }

    // 2. Shared hobbies (20 pts)
    int shared_hobbies = 0;
    int total_hobbies = 0;
    for (list<string>::const_iterator it = hobbies.begin(); it != hobbies.end(); ++it) {
        total_hobbies++;
        
        // Check if other has this hobby (list linear search)
        for (list<string>::const_iterator jt = other.hobbies.begin(); jt != other.hobbies.end(); ++jt) {
            if (*it == *jt) { shared_hobbies++; break; }
        }
    }
    if (total_hobbies > 0) {
        score += 20.0 * ((double)shared_hobbies / total_hobbies);
    }

    // 3. Dealbreaker conflict penalty (-30 pts max)
    // If any of my dealbreakers appear in their preferences, penalize
    for (set<string>::const_iterator it = dealbreakers.begin(); it != dealbreakers.end(); ++it) {
        for (size_t j = 0; j < other.preferences.size(); j++) {
            if (other.preferences[j] == *it) {
                score -= 15.0;
            }
        }
    }
    // Check reciprocal (their dealbreakers vs my preferences)
    for (set<string>::const_iterator it = other.dealbreakers.begin(); it != other.dealbreakers.end(); ++it) {
        for (size_t j = 0; j < preferences.size(); j++) {
            if (preferences[j] == *it) {
                score -= 15.0;
            }
        }
    }

    // 4. Weighted preference overlap (30 pts)
    // For each shared preference key, add weighted score
    double weight_score = 0.0;
    double max_weight_score = 0.0;
    
    string weight_keys[] = {"quiet", "cleanliness", "social", "studious", "pets", "smoking"};
    int n_keys = 6;
    
    for (int i = 0; i < n_keys; i++) {
        unordered_map<string,int>::const_iterator wa = pref_weights.find(weight_keys[i]);
        unordered_map<string,int>::const_iterator wb = other.pref_weights.find(weight_keys[i]);
        if (wa != pref_weights.end() && wb != other.pref_weights.end()) {
            int w_a = wa->second;
            int w_b = wb->second;
            int diff = abs(w_a - w_b);
            double max_val = 5.0;
            max_weight_score += max_val;
            weight_score += max_val - diff;
        }
    }
    if (max_weight_score > 0) {
        score += 30.0 * (weight_score / max_weight_score);
    }

    // Set boundary [0, 100]
    if (score < 0.0) score = 0.0;
    if (score > 100.0) score = 100.0;

    return (int)score;
}

bool RoommateProfile::operator>(const RoommateProfile& other) const {
    // Meaningless without a reference — used externally with a ref profile
    // Returns true if this profile has more filled-in data than other
    return (attributes.size() + hobbies.size() + preferences.size()) >
           (other.attributes.size() + other.hobbies.size() + other.preferences.size());
}

// CSV export for Python ML pipeline
string RoommateProfile::csv_header() {
    return "person_id,college,major,year,state,zip,sleep,cleanliness,"
           "w_quiet,w_cleanliness,w_social,w_studious,w_pets,w_smoking,"
           "n_hobbies,n_preferences,n_dealbreakers,"
           "pref_quiet_hours,pref_no_smoking,pref_pets_ok,pref_early_bird,pref_night_owl";
}

string RoommateProfile::to_csv_row(string person_id) const {
    auto get_attr = [&](string key) -> string {
        map<string,string>::const_iterator it = attributes.find(key);
        
        return (it != attributes.end()) ? it->second : "";
    };
    auto get_weight = [&](string key) -> string {
        unordered_map<string,int>::const_iterator it = pref_weights.find(key);
        
        if (it != pref_weights.end()) {
            stringstream ss;
            ss << it->second;
            return ss.str();
        }
        return "3";
    };
    auto has_pref = [&](string pref) -> string {
        for (size_t i = 0; i < preferences.size(); i++) {
            if (preferences[i] == pref) return "1";
        }
        return "0";
    };

    stringstream row;
    row << person_id << ","
        << get_attr("college") << ","
        << get_attr("major") << ","
        << get_attr("year") << ","
        << get_attr("state") << ","
        << get_attr("zip") << ","
        << get_attr("sleep") << ","
        << get_attr("cleanliness") << ","
        << get_weight("quiet") << ","
        << get_weight("cleanliness") << ","
        << get_weight("social") << ","
        << get_weight("studious") << ","
        << get_weight("pets") << ","
        << get_weight("smoking") << ","
        << hobbies.size() << ","
        << preferences.size() << ","
        << dealbreakers.size() << ","
        << has_pref("quiet hours") << ","
        << has_pref("no smoking") << ","
        << has_pref("pets ok") << ","
        << has_pref("early bird") << ","
        << has_pref("night owl");
    return row.str();
}
