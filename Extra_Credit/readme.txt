----------------------------------------- GROUP --------------------------------------------------
Talise Baker-Matsuoka, bakermat@usc.edu, 7755662598
Jessica Li, jmli@usc.edu, 4875063007
May Syi Ang, maysyian@usc.edu, 6524773417
---------------------------------------------------------------------------------------------------

------------------------------------------- EXTRA CREDIT -------------------------------------------

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
 
Before funning python script, must use 8 then 4 to export the data to a csv file (profiles.csv)
Compile python portion with:
pip3 install scikit-learn pandas numpy scipy
python3 recommend.py profiles.csv

^ This code will output recommendations.csv

Resources used for recommend.py:
sklearn code:
- https://scikit-learn.org/stable/modules/preprocessing.html
- https://www.geeksforgeeks.org/machine-learning/data-pre-processing-wit-sklearn-using-standard-and-minmax-scaler/ 
- https://scikit-learn.org/stable/modules/generated/sklearn.preprocessing.LabelEncoder.html

cosine similarity code with sklearn:
- https://scikit-learn.org/stable/modules/generated/sklearn.metrics.pairwise.cosine_similarity.html
A super helpful source which was the inspiration
- https://fritz.ai/recommender-systems-with-python/

truncatedSVD methods:
- https://towardsdatascience.com/a-complete-guide-to-recommender-system-tutorial-with-sklearn-surprise-keras-recommender-5e52e8ceace1/ 
- https://scikit-learn.org/stable/modules/generated/sklearn.decomposition.TruncatedSVD.html

more info on recommendation systems:
- https://medium.com/@m_n_malaeb/recall-and-precision-at-k-for-recommender-systems-618483226c54 