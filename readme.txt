----------------------------------------- GROUP --------------------------------------------------
Talise Baker-Matsuoka, bakermat@usc.edu, 7755662598
Jessica Li, jmli@usc.edu, 4875063007
May Syi Ang, maysyian@usc.edu, 6524773417
---------------------------------------------------------------------------------------------------

--------------------------------------------- PHASE 1 ---------------------------------------------

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

---------------------------------------------------------------------------------------------------

Testing Part 2 - Date, Person class
Note: As far as I could tell, the project manual did not describe a preference between outputting a date like "January 03, 1961" and 
"January 3, 1961" so I assumed I could choose which to display. I decided to display the second version since I think that's more commonly used. 

- Uncomment the code at the bottom of person.cpp and run:
g++ -std=c++11 contact.cpp date.cpp person.cpp -o test 
./ test

Prints: 
---- Testing Date ----
January 13, 1961
January 7, 1961

---- Testing Email & Phone ----
Email (Work): veep@wh.gov
Phone (Home): 310-192-2847

---- Testing Person ----
Louis-Dreyfus, Julia Scarlett Elizabeth
January 13, 1961
Phone (Home): 310-192-2847
Email (Work): veep@wh.gov

---------------------------------------------------------------------------------------------------

Testing Part 3 - Network class
- Uncomment the code at the bottom of network.cpp and run:
g++ -std=c++11 network.cpp contact.cpp date.cpp misc.cpp person.cpp -o test
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

---------------------------------------------------------------------------------------------------

--------------------------------------------- PHASE 2 ---------------------------------------------

Testing Part 1 - Adding a new option to the Main Menu
- Uncomment the code at the bottom of network.cpp and run:
g++ -std=c++11 network.cpp contact.cpp date.cpp misc.cpp person.cpp -o test
./test

Test using showMenu. When promped, enter 6 and then enter:
    Truman 
    Burbank
    Martin
    Van Nostrand
It should output their information


---------------------------------------------------------------------------------------------------

Testing Part 2 - Make Friends
- Uncomment the code at the bottom of network.cpp and run:
g++ -std=c++11 network.cpp contact.cpp date.cpp misc.cpp person.cpp -o test
./test

Testing was done using showMenu. More specifically, when prompted by the menu, select 6 and enter:
    Truman 
    Burbank
    Martin
    Van Nostrand

Result should say "They are now friends!"

---------------------------------------------------------------------------------------------------

Testing Part 3/4 - Person ID and Print Person
After running the above code for part 1/2, go back to the showMenu option. 
Select the option 5. Then type the last name 'Burbank' and it will show the newly added friend when printing his information.

Print people with last name 
Last name: Burbank
Burbank, Truman
January 17, 1962
Phone (Home): 213-517-9989
Email (Work): document@info.edu
martinvannostrand (Martin Van Nostrand)

---------------------------------------------------------------------------------------------------

Testing Part 5 - Sort Friends
- Uncomment the code at the bottom of person.cpp and run:
g++ -std=c++11 contact.cpp date.cpp misc.cpp person.cpp -o test
./test

Testing was done using the instruction manual's example friend names in order to obtain the same results and compare. Result:
Tommy, Trojan
--------------------------------
Amy, Chen

Karan, Kapoor

Katy, Bruin

Kurt, Cobain

---------------------------------------------------------------------------------------------------

