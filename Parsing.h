//
// Created by kianr on 4/14/2021.
//

/**
 * This class is used to parse through the JSON files
 *
 * Private functions are:
 *      # Articles
 *      Word count
 *      Average
 */

#ifndef SEARCH_ENGINE_PARSING_H
#define SEARCH_ENGINE_PARSING_H

#include <iostream>
#include "AVLTree.h"
#include "Documents.h"
#include "Words.h"
#include "rapidjson/document.h"
#include "porter2_stemmer.h"

#include <fstream>
#include <string>
#include <dirent.h>
#include <sstream>
#include <set>

#include "Documents.h"

#include "Hash.h"

using namespace rapidjson;
using namespace std;
using namespace Porter2Stemmer;


class Parsing {
private:
    int numArticles = 0;
    int wordCount = 0;
    int average = 0;

public:

    string fileToString(const char* filename);
    void parseSearch(AVLTree<Words>& wordIndex, string& wordSearch, Hash<string, Documents>& authorHash);

    void stringLowerCase(string& temp);
    void removeEndingPunctuation(string& temp);
    void removeBeginningPunctuation(string& temp);

    set<string> createStopWordSet();

    int getNumArticles();
    int getWordCount();
    int getAverage();

    void readJson(AVLTree<Words> &wordTree, string& pathtoDir, Hash<string, Documents>& authorHash);
    void parseSearch(AVLTree<Words>& wordTree, string& search, map<string, int>& found);
    void searchForAuthors(Hash<string, Documents>& authorHash, string& search, map<string, int>& found);
};



#endif //SEARCH_ENGINE_PARSING_H
