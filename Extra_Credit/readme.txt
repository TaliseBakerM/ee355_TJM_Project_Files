----------------------------------------- GROUP --------------------------------------------------
Talise Baker-Matsuoka, bakermat@usc.edu, 7755662598
Jessica Li, jmli@usc.edu, 4875063007
May Syi Ang, maysyian@usc.edu, 6524773417
---------------------------------------------------------------------------------------------------

------------------------------------------- EXTRA CREDIT -------------------------------------------

Video of how it works: https://drive.google.com/file/d/16q3BFcPhwGTZZPdZtrUD3kOkxqtVwaYw/view

Compile with:
g++ -std=c++11 network.cpp roommate.cpp contact.cpp date.cpp misc.cpp person.cpp -o test.out
./test.out

Testing:
1. Enter 8, then 4. When prompted, enter ./profiles
2. Enter 8, then 1. When prompted, enter the following:
   first name=Julia Scarlett Elizabeth
   last name=Louis-Dreyfus
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
3. Enter 8, then 2. When prompted, enter the following:
   first name=Julia Scarlett Elizabeth
   last name=Louis-Dreyfus
4. Enter 8, then 3. When prompted, enter the following:
   first name=Julia Scarlett Elizabeth
   last name=Louis-Dreyfus
   profiles=3
 
Before running python script, must use 9 to export the data to a csv file (profiles.csv)
Compile python portion with:
pip3 install scikit-learn pandas numpy scipy
python3 recommend.py profiles.csv

^ This code will output recommendations.csv

Resources used for recommend.py:
sklearn code:
- https://scikit-learn.org/stable/modules/preprocessing.html
- https://www.geeksforgeeks.org/machine-learning/data-pre-processing-wit-sklearn-using-standard-and-minmax-scaler/ 
- https://scikit-learn.org/stable/modules/generated/sklearn.preprocessing.LabelEncoder.html
Lines 68-71 approximately;
- the label encoder is used to help convert words like school being usc to numbers for processing later
Lines 75-107 approximately:
- MinMax scaler applies a scaling to the data so it is automatically between [0, 1] to convert the number values into a number between 0 and 1 so that it can 
be processed later on. 


cosine similarity code with sklearn:
- https://scikit-learn.org/stable/modules/generated/sklearn.metrics.pairwise.cosine_similarity.html
Lines 115-121 approximately:
- This function helps to convert each person's data for all the people into a matrix representing the similarity between each person. A score of 1 would mean that the two people
match in their profiles, while a score of 0 represents that these two people share nothing in their profiles. 


A super helpful source which was the inspiration for some of the processing
- https://fritz.ai/recommender-systems-with-python/
- this source helped with the content_based() function and the top_k_content() functions and generally explains how the process can help make recommendations


truncatedSVD methods:
- https://towardsdatascience.com/a-complete-guide-to-recommender-system-tutorial-with-sklearn-surprise-keras-recommender-5e52e8ceace1/ 
Lines 163-178 approximately:
- these lines factor the matrix using SVD, which we learned about in EE 510 and EE 141
- it helps further process the data
- https://scikit-learn.org/stable/modules/generated/sklearn.decomposition.TruncatedSVD.html
- this helps explain why truncated SVD is better than a regular SVD which has a U, sigma, and V matrix
- we only compute the top k singular values (found in the sigma matrix), which reduces how much processing it has to do expecially with a lot of people


More info on recommendation systems: + recall and precision
- https://medium.com/@m_n_malaeb/recall-and-precision-at-k-for-recommender-systems-618483226c54 
Lines 206-230 approximately:
- this resource helped us evaluate the precision and recall of the recommendation system we built and was used to guide the code



Help for roommate.cpp (in addition to lecture notes and resources recommended in lecture)
https://en.cppreference.com/cpp/container
- in addition to the lecture notes this helped us pick what data structures (like what STLs) to use

Help with maps
- https://en.cppreference.com/cpp/container/map
- good for storing key-value pairs


Help with unordered maps
- https://www.geeksforgeeks.org/cpp/unordered_map-in-cpp-stl/


Vector + list + sets
- from lecture notes (for vector and list)
- https://medium.com/@megha_bh/stl-containers-vector-set-and-map-7a90e0b1e19f
- above links as well
- essentially storing items in multiple different ways


Deque
- lecture notes
- same as above, practicing STLs by storing data different ways based on what we learned in lecture