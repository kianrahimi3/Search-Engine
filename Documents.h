//
// Created by kianr on 4/13/2021.
//

/**
 * This class is used to hold all of the Document information...such as the id, title, and text
 * The private vector is used to store the authors
 * So far I have only used this for the authors...I used the id and text in other methods
 */

#ifndef SEARCH_ENGINE_DOCUMENTS_H
#define SEARCH_ENGINE_DOCUMENTS_H

#include <iostream>
#include <string>
#include <vector>
#include <set>

using namespace std;

class Documents {
private:
    string id;
    string title;
    string docText;

    vector<string> vec;
public:
    Documents();


    void setDocID(string &temp);
    string getDocID();

    void addToVec(string &temp);
    vector<string> getVec();

    void setDocText(string &temp);
    string getDocText();
};


#endif //SEARCH_ENGINE_DOCUMENTS_H
