Partners:
Talise Baker-Matsuoka, bakermat@usc.edu, 7755662598
Jessica Li, jmli@usc.edu, 4875063007
May Syi Ang, maysyian@usc.edu, 6524773417
---



Testing Part 1 - Contact class
- Uncomment the code at the bottom and then run: 
g++ contact.cpp -o contact.out
./contact.out

Prints: 
Email (Work) random@usc.edu
Phone (Home) 310-192-2011
Phone (Office) 310-192-2847
Phone (Home) 310-192-2011
310-192-2011

Testing Part 2 - Date, Person class
Note: As far as I could tell, the project manual did not describe a preference between outputting a date like "January 03, 1961" and 
"January 3, 1961" so I assumed I could choose which to display. I decided to display the second version since I think that's more commonly used. 

- Uncomment the code at the bottom of person.cpp and run:
g++ contact.cpp date.cpp person.cpp -o test 
./ test

Prints: 
January 13, 1961
January 3, 1961
Email (Work) julia@wh.gov
Phone (Home) 310-192-2847

Louis-Dreyfus, Julia Scarlett Elizabeth

January 13, 1961

Phone (Home) 310-192-2847

Email (Work) veep@wh.gov

Testing Part 3 - Network class
- Uncomment the code at the bottom of network.cpp and run:
g++ network.cpp contact.cpp date.cpp misc.cpp person.cpp -o test
./test

Prints:
Starting main
Loading network database
Network loaded from networkDB.txt with 3 people
Network created
Number of people: 3
------------------------------
Burbank, Truman

January 17, 1962

Phone (Home): 213-517-9989

Email (Work): document@info.edu
------------------------------
Van Nostrand, Martin

July 24, 1949

Phone (Home): 224-432-5809

Email (Work): myemail@yahoo.com
------------------------------
Louis-Dreyfus, Julia Scarlett Elizabeth

January 13, 1961

Phone (Home): 310-291-7482

Email (Work): something@email.com
------------------------------
Printed DB
Saved DB

The showMenu() function works as expected after the above test lines print.

Note: Part 3 can be run with c++11 and:
Network::Network(string fileName) : Network(){
    // TODO: complete this method!
    // Implement it in one single line!
    // You may need to implement the load method before this!
    cout << "Loading network database\n";
    loadDB(fileName);
    cout << "Network loaded from " << fileName << " with " << count << " people\n";
}
but the code file for Part 1 said to not use c++11 so we used a different method instead to implement it in one line.