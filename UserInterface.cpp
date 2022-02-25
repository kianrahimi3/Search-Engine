//
// Created by kianr on 4/21/2021.
//

#include <algorithm>
#include "UserInterface.h"

/**
 *
 * @param authorTree
 * @param wordTree
 *
 * This function will be the User Interface for the user.
 * It will ask what choice the user wants to do, and call the function that they want.
 * After performing the function, it will ask again until they exit.
 */
void UserInterface::run(Hash<string, Documents>& authorTree, AVLTree<Words>& wordTree) {
    Parsing parse;
    string testPath;
    //char* path = "../../ds_01";
    //  string word = "virus";

    map<string, int> results;

    //  parse.readJson(wordTree, path, word, authorTree);

    bool shouldIrun = true;

    while(shouldIrun) {


        cout << "What would you like to do?" << endl
             << "1. Clear the Index" << endl
             << "2. Parse the corpus and populate the Index OR open a persistence file" << endl
             << "3. Boolean Query" << endl
             << "4. Print Statistics" << endl
             << "0. Exit" << endl;

        int choice;
        cin >> choice;

        switch (choice) {
            case 1: {
                cout << "Clearing the Index" << endl;
                clearIndex(authorTree, wordTree);
                break;
            }

            case 2: {
                cout << "Parsing the corpus" << endl;
                cout << "Which would you like to do?" << endl;
                cout << "1. Parse and Populate the Index" << endl;
                cout << "2. Open a persistence file" << endl;

                int option = 0;
                cin >> option;

                switch (option) {
                    case 1: {
                        cout << "Provide the path to the directory" << endl;

                        cin.ignore();
                        getline(cin, testPath);

                        cout << "Path: " << testPath << endl;
                        parse.readJson(wordTree, testPath, authorTree);

                        break;
                    }

                    case 2: {
                        cout << "Not Done Yet" << endl;

                        break;
                    }
                }

                break;
            }

            case 3: {
                cout << "Performing Boolean Query" << endl;
                booleanQuery(wordTree, authorTree, results);
                break;
            }

            case 4: {
                cout << "Printing Statistics" << endl;
                printStatistics(parse, wordTree, authorTree);
                break;
            }
            case 0: {
                shouldIrun = false;
                break;
            }

            default:
                cout << "Wrong Input. Try again" << endl;
                break;
        }
        cout << endl;
    }
}

/**
 *
 * @param authorTree
 * @param wordTree
 *
 * This function is called to clear the Index clean
 */
void UserInterface::clearIndex(Hash<string, Documents> &authorTree, AVLTree<Words> &wordTree) {

    wordTree.makeEmpty();
    authorTree.clear();

    cout << "The AVL Tree and Hash Table have been cleared" << endl;
}

/**
 *
 * @param parse
 * @param wordTree
 * @param authorHash
 *
 * This function prints all the statistics. These statistics are:
 *          1. # Articles
 *          2. Average # of words
 *          3. # Unique Words
 *          4. # Unique Authors
 *          5. Top 50 Most Frequent
 */
void UserInterface::printStatistics(Parsing& parse, AVLTree<Words>& wordTree, Hash<string, Documents>& authorHash) {
    cout << "Articles: " << parse.getNumArticles() << endl
         << "Average # of words: " << parse.getAverage() << endl
         << "# Unique Words: " << wordTree.getCount() << endl
         << "# Unique Authors: " << authorHash.getCapacity() << endl
         << "Top 50 Most Frequent Words: " << endl;

    get50(wordTree);

}

/**
 *
 * @param wordTree
 * @param authorHash
 * @param results
 *
 * This function is called to do the boolean search query.
 * If AND is found, it will take account of the next words and find all the documents that have these words
 * If OR is found, it will take account of the next words and find all the documents that have this or that
 * IF Author, it will search the Hash Table for the author
 * Else...Just look for the word
 *
 * If Not or Author is found within the search, I will need to go through the process and do comparisons to make sure the proper data is being pulled
 * A map will be returned of the values searched for
 */
void UserInterface::booleanQuery(AVLTree<Words>& wordTree, Hash<string, Documents>& authorHash, map<string, int>& results) {
    cout << "Enter your Boolean Search" << endl;

    string searchPhrase;
    cin.ignore();
    getline(cin, searchPhrase);
    stringLowerCase(searchPhrase);

    cout << "Search = " << searchPhrase << endl;
    string end = " \0";
    searchPhrase = searchPhrase + end;

    stringstream stream;
    stream << searchPhrase;

    string searchWord;
    while(getline(stream, searchPhrase, ' ')) {
        if(searchPhrase == "or") {
            getline(stream, searchWord, ' ');

            while(searchWord != "not" && searchWord != "author") {
                if(searchWord == end)
                    break;

                Parsing parse;
                parse.parseSearch(wordTree, searchWord, results);

                //start the loop over again
                getline(stream, searchWord, ' ');
            }

            if(searchWord == "author") {
                //find all of the words that exist after AUTHOR and then put in map...
                getline(stream, searchWord, ' ');
                Parsing parse;
                authorProcess(parse, authorHash, searchWord, results);
            }
            else if(searchWord == "not") {
                //find all of the words that exist after NOT and then put in map...
                //then compare this map with the results map and take out those that exist
                //in map found here
                getline(stream, searchWord, ' ');
                Parsing parse;
                notProcess(parse, wordTree, searchWord, results);
            }
            break;
        }
        else if(searchPhrase == "author") {
            getline(stream, searchWord, ' ');
            Parsing parse;
            map<string, int> temp;
            parse.searchForAuthors(authorHash, searchWord, temp);

            if(results.empty())
                results = temp;
            else
                commpareAuthors(results, temp);
            break;
        }

        else if(searchPhrase == "and") {
            getline(stream, searchWord, ' ');

            //Do what i did similarly in OR
            while(searchWord != "not" && searchWord != "author") {
                if(searchWord == end)
                    break;

                Parsing parse;
                map<string, int> temp;
                parse.parseSearch(wordTree, searchWord, temp);

                if(temp.size() == 0){
                    results.clear();
                    break;
                }

                if(results.empty())
                    results = temp;
                else {
                    map<string, int>::iterator iter;
                    map<string, int> replaceMap;

                    for(iter = results.begin(); iter != results.end(); iter++) {
                        //https://thispointer.com/how-check-if-a-given-key-exists-in-a-map-c/
                        if(temp.count(iter->first) > 0) {
                            pair<string, int> tempo(iter->first, iter->second + temp[iter->first]);
                            replaceMap.insert(tempo);
                        }
                    }
                    results = replaceMap;
                }
                getline(stream, searchWord, ' ');
            }

            if(searchWord == "author") {
                getline(stream, searchWord, ' ');
                Parsing parse;
                authorProcess(parse, authorHash, searchWord, results);
            }
            else if(searchWord == "not") {
                getline(stream, searchWord, ' ');
                Parsing parse;
                notProcess(parse, wordTree, searchWord, results);
            }
            break;
        }

        else {
            //getline(stream, searchWord, ' ' );
            Parsing parse;
            map<string, int> temp;
            parse.parseSearch(wordTree, searchPhrase, temp);

            searchWord = searchPhrase;
            while(searchWord != "not" && searchWord != "author") {
                if(searchWord == end)
                    break;
                results = temp;

                getline(stream, searchWord, ' ');
            }
            if(searchWord == "not") {
                getline(stream, searchWord, ' ');
                notProcess(parse, wordTree, searchWord, results);
            }
            else if(searchWord == "author") {
                getline(stream, searchWord, ' ');
                authorProcess(parse, authorHash, searchWord, results);
            }
        }
        break;
    }

    if(results.size() > 0) {
        output(results);
    }
    else
        cout << "None " << endl;

    results.clear();

}


/**
 *
 * @param results
 * @param found
 *
 * When NOT is found, this function will be called.
 * This will compare the results map to the found map, and delete anything that was found
 *
 * In theory, this is saying:
 *          "If the word exists in the document, remove the document"
 */
void UserInterface::compareNot(map<string, int> &results, map<string, int> &found) {
    map<string, int>::iterator iter;
    for(iter = found.begin(); iter != found.end(); iter++) {
        //https://thispointer.com/how-check-if-a-given-key-exists-in-a-map-c/
        if(results.count(iter->first) > 0)
            results.erase(iter->first);
    }

}

/**
 *
 * @param results
 * @param found
 *
 * When AUTHOR is found, this function will be called.
 * This will compare the results map to the found map, and enter the found values into the results map
 *
 * In theory, this is saying:
 *          "If the author exists, add the document to the results"
 */
void UserInterface::commpareAuthors(map<string, int> &results, map<string, int> &found) {
    map<string, int> newMap;

    map<string, int>::iterator iter;
    if(found.size() > 0) {
        for (iter = results.begin(); iter != results.end(); iter++)
            //https://thispointer.com/how-check-if-a-given-key-exists-in-a-map-c/
            if (found.count(iter->first) > 0) {
                pair<string, int> temp(iter->first, iter->second);
                newMap.insert(temp);
            }
    }

    results.clear();
    results = newMap;
}


/**
 *
 * @param parse
 * @param authorHash
 * @param searchWord
 * @param results
 *
 * This is used for when the search word is equal to AUTHOR.
 */
void UserInterface::authorProcess(Parsing& parse, Hash<string, Documents> &authorHash, string &searchWord, map<string, int>& results) {
    map<string, int> temp;
    parse.searchForAuthors(authorHash, searchWord, temp);

    commpareAuthors(results, temp);
}

/**
 *
 * @param parse
 * @param wordTree
 * @param searchWord
 * @param results
 *
 * This is used for when the search word is equal to NOT.
 */
void UserInterface::notProcess(Parsing& parse, AVLTree<Words> &wordTree, string &searchWord, map<string, int> &results) {
    map<string, int> temp;
    parse.parseSearch(wordTree, searchWord, temp);

    compareNot(results, temp);
}

/**
 *
 * @param results
 *
 * This function will output a vector for relevancy ranking.
 * The reason the vector is a pair is because if it were a map, those with the same frequency will be taken out,
 * since maps only hold distinct keys
 */
void UserInterface::output(map<string, int> &results) {
    vector<pair<int, string>> relevancyRankingVec;

    map<string, int>::iterator iter;
    for(iter = results.begin(); iter != results.end(); iter++) {
        pair<int, string> temp(iter->second, iter->first);
        relevancyRankingVec.push_back(temp);
    }

    sort(relevancyRankingVec.rbegin(), relevancyRankingVec.rend());

    cout << "Articles == " << results.size() << endl;
    if(results.size() >= 15)
        for (int i = 0; i < 15; i++) {
            cout << "     " << relevancyRankingVec[i].second << endl;
        }
    else
        for (int i = 0; i < relevancyRankingVec.size(); i++) {
            cout << "     " << relevancyRankingVec[i].second << endl;
        }

}

/**
 *
 * @param temp
 *
 * Acts as toLower()
 */
void UserInterface::stringLowerCase(string &temp) {
    for(int i = 0; i < temp.length(); i++)
        temp[i] = tolower(temp[i]);
}

/**
 *
 * @param wordTree
 * @param results
 *
 * Print 50 most common words
 */
void UserInterface::get50(AVLTree<Words> &wordTree) {
    if(wordTree.getCount() > 0) {
        vector<Words> wordVector = wordTree.getVec();
        vector<pair<int, string>> vecPair;
        for (int i = 0; i < wordVector.size(); i++) {
            pair<int, string> tempo(wordVector[i].getCount(), wordVector[i].getWord());
            vecPair.push_back(tempo);
        }
        sort(vecPair.rbegin(), vecPair.rend());

        for (int i = 0; i < 50; i++) {
            cout << "   " << i + 1 << ". " << vecPair[i].second << endl;
        }
        vecPair.clear();
        wordTree.vec.clear();
    }
    else
        cout << "None" << endl;
}
