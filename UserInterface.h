//
// Created by kianr on 4/21/2021.
//

/**
 * This class is the main class that the program runs on. It will show the User Interface
 * for the user to act on and once done, it will call the other classes to perform jobs
 */

#ifndef SEARCH_ENGINE_USERINTERFACE_H
#define SEARCH_ENGINE_USERINTERFACE_H

#include "AVLTree.h"
#include "Words.h"
#include "Hash.h"
#include "Documents.h"
#include <iostream>
#include <string>

#include "Parsing.h"

using namespace std;

class UserInterface {
private:
    string test;

public:
    void run(Hash<string, Documents>& authorTree,AVLTree<Words>& wordTree);

    void clearIndex(Hash<string, Documents>& authorTree,AVLTree<Words>& wordTree);
    void printStatistics(Parsing& parse, AVLTree<Words>& wordTree, Hash<string, Documents>& authorHash);
    void booleanQuery(AVLTree<Words>& wordTree, Hash<string, Documents>& authorHash, map<string, int>& searchResults);


    void stringLowerCase(string &temp);

    void compareNot(map<string, int>& searchResults, map<string, int>& found);
    void commpareAuthors(map<string, int>& searchResults, map<string, int>& found);

    void authorProcess(Parsing& parse, Hash<string, Documents>& authorHash, string& searchWord, map<string, int>& results);
    void notProcess(Parsing& parse, AVLTree<Words>& wordTree, string& searchWord, map<string, int>& results);
    void output(map<string, int>& results);

    void get50(AVLTree<Words>& wordTree);

};


#endif //SEARCH_ENGINE_USERINTERFACE_H
