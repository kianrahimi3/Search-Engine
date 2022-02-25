//
// Created by kianr on 4/14/2021.
//

#include "Parsing.h"
#include <utility>
#include <algorithm>

/**
 *
 * @param filename
 * @return
 *
 * This will return the whole text into one large string. This will help for parsing
 */
//need to return as big string bc JSON parse only uses c_str
string Parsing::fileToString(const char *filename) {
    ifstream myFile(filename);

    string getEverything;
    string temp;
    while (getline(myFile, temp)) {
        getEverything += temp;
    }

    myFile.close();

    return getEverything;
}

//for demo output
void Parsing::parseSearch(AVLTree<Words>& wordTree, string &wordSearch, Hash<string, Documents>& authorHash) {



    cout << "The word being searched for is: " << endl << "        " << wordSearch << endl;
    cout << "Below are the ID's that have this word: " << endl;

    //looking = where we will be inserting the found occurences
    Words looking;
    if (wordTree.find(wordSearch)) {
        looking = wordTree.get(wordSearch);
        map<string, int>::iterator it;
        for (it = looking.getMap().begin(); it != looking.getMap().end(); it++) {

            //output ID and no occurences
            cout << "        " << it->first << endl;
        }
    }


    Documents documents;
    if(authorHash.find(wordSearch)) {
        documents = authorHash.get(wordSearch);

        cout << endl << "Authors" << endl;


        cout << documents.getVec().size() << endl;
        for(int i = 0; i < documents.getVec().size(); i++)
            if(i-1 < 0 || documents.getVec()[i-1] != documents.getVec()[i])
                cout << documents.getVec()[i] << endl;
    }




}
/**
 *
 * @param temp
 *
 * Acts as toLower()
 */
//lower case string
void Parsing::stringLowerCase(string &temp) {
    for(int i = 0; i < temp.length(); i++)
        temp[i] = tolower(temp[i]);

}

/**
 *
 * @param temp
 *
 * removes ending punctuations
 */
//remove punctuation.... etc .,'!?
void Parsing::removeEndingPunctuation(string &temp) {
    for(int i = temp.length() -1; i > 0; i--) {
        //remove if not a digit or letter
        if(!isalpha(temp[i]) && !isdigit(temp[i]))
            temp.erase(i, 1);
    }
}

/**
 *
 * @param temp
 *
 * removes beginning punctuations
 */
void Parsing::removeBeginningPunctuation(string &temp) {
    while(ispunct(temp[0])) {
        if(temp.length() == 0)
            break;
        temp.erase(0,1);
    }
}

/**
 *
 * @param stopWords
 *
 * This takes account of all the stop words (words that should not be taken account of)
 */
set<string> Parsing::createStopWordSet() {
    ifstream myfile("../StopWords.txt");
    set<string> stopWords;

    string buffer;
    while(myfile >> buffer) {
        stopWords.insert(buffer);
        //cout << buffer << endl;
    }
    return stopWords;
}

/**
 *
 * @return
 *
 * return privates
 */
int Parsing::getNumArticles() {
    return numArticles;
}
int Parsing::getWordCount() {
    return wordCount;
}
int Parsing::getAverage() {
    return average;
}

/**
 *
 * @param wordTree
 * @param pathtoDir
 * @param authorHash
 *
 * This function will read through the JSON files and update the word and author trees.
 * This is essential for anything else
 */
void Parsing::readJson(AVLTree<Words> &wordTree, string& pathtoDir, Hash<string, Documents>& authorHash) {

    struct dirent *entry = nullptr;
    DIR* dp = nullptr;

    string path(pathtoDir);
    dp = opendir(path.c_str());
    if (!dp)
        cout << "ERROR" << endl;

    set<string> stopWords = createStopWordSet();

    if(dp != nullptr) {
        while ((entry = readdir(dp))) {
            string file = path + "/" + entry->d_name;
            string fileString = fileToString(file.c_str()); //get file

            Document json;
            json.Parse(fileString.c_str());

            string paper_id;
            string authors;
            string doc_text;
            string title;


            if (json.IsObject()) {   //not sure why i need this
                numArticles++;

                if (json.HasMember("paper_id")) {
                    paper_id = json["paper_id"].GetString();
                    // cout << paper_id << endl;
                }

                if (json.HasMember("body_text")) {
                    for (int i = 0; i < json["body_text"].Size(); i++) {
                        //  doc_text += doc["body_text"][i]["text"].GetString();
                        //  cout << doc_text << endl;

                        doc_text = json["body_text"][i]["text"].GetString();
                        //get each line of the document body text
                        stringstream stream;
                        stream << doc_text;

                        //get every word
                        string buffer;
                        while (getline(stream, buffer, ' ')) {

                            removeBeginningPunctuation(buffer);
                            removeEndingPunctuation(buffer);
                            stringLowerCase(buffer);

                            if(buffer.length() == 0)
                                continue;
                            if(stopWords.count(buffer))
                                continue;
                            if( !isalpha(buffer[0]) && !isdigit(buffer[0]) )
                                continue;

                            /// Porter2Stemmer
                            stem(buffer);   // i'm guessing it will just find the stem automatically?

                            wordCount++;

                            //insert the word and add the id to word map
                            if ( !(wordTree.find(buffer)) ) {
                                wordTree.insert(buffer);
                                wordTree.get(buffer).addID(paper_id);
                                wordTree.get(buffer).getMap()[paper_id]++;

                                //else if word exists already, add
                            } else {
                                wordTree.get(buffer).getMap()[paper_id]++;
                            }
                        }
                    }
                }

                if(json["metadata"].HasMember("title")) {

                    //get each line of the document body text
                    stringstream stream;
                    title = json["metadata"]["title"].GetString();
                    stream << title;

                    //get every word
                    string buffer;
                    while (getline(stream, buffer, ' ')) {

                        removeBeginningPunctuation(buffer);
                        removeEndingPunctuation(buffer);
                        stringLowerCase(buffer);

                        if(buffer.length() == 0)
                            continue;
                        if(stopWords.count(buffer))
                            continue;
                        if( !isalpha(buffer[0]) && !isdigit(buffer[0]) )
                            continue;

                        /// Porter2Stemmer
                        stem(buffer);   // i'm guessing it will just find the stem automatically?

                        wordCount++;

                        //if word exists already, add
                        if ( !(wordTree.find(buffer)) ) {
                            wordTree.insert(buffer);
                            wordTree.get(buffer).addID(paper_id);
                            wordTree.get(buffer).getMap()[paper_id]++;

                            //else insert the word and add the id to word map
                        } else {
                            wordTree.get(buffer).getMap()[paper_id]++;
                        }
                    }
                }

                if(json.HasMember("abstract")) {
                    //  doc_text += doc["body_text"][i]["text"].GetString();
                    //  cout << doc_text << endl;

                    for(int i = 0; i < json["abstract"].Size(); i++) {
                        //get each line of the document body text
                        stringstream stream;
                        title = json["abstract"][i]["text"].GetString();
                        stream << title;

                        //get every word
                        string buffer;
                        while (getline(stream, buffer, ' ')) {

                            removeBeginningPunctuation(buffer);
                            removeEndingPunctuation(buffer);
                            stringLowerCase(buffer);

                            if (buffer.length() == 0)
                                continue;
                            if (stopWords.count(buffer))
                                continue;
                            if (!isalpha(buffer[0]) && !isdigit(buffer[0]))
                                continue;


                            /// Porter2Stemmer
                            stem(buffer);   // i'm guessing it will just find the stem automatically?



                            //if word exists already, add
                            if (!(wordTree.find(buffer))) {
                                wordTree.insert(buffer);
                                wordTree.get(buffer).addID(paper_id);
                                wordTree.get(buffer).getMap()[paper_id]++;

                                //else insert the word and add the id to word map
                            } else {
                                wordTree.get(buffer).getMap()[paper_id]++;
                            }
                        }
                    }
                }



                if(json["metadata"].HasMember("authors")) {
                    for(int i = 0; i < json["metadata"]["authors"].Size(); i++) {
                        authors = json["metadata"]["authors"][i]["last"].GetString();
                        stringLowerCase(authors);

                        //    cout << authors << endl;

                        if(authorHash.find(authors))
                            authorHash.get(authors).addToVec(paper_id);
                        else {
                            Documents documents;
                            documents.addToVec(paper_id);
                            authorHash.insert(authors, documents);
                        }

                    }
                }
            }
        }
    }
    //   cout << "Paper ID: " << vec[1].getDocID() << endl;
    //   cout << "Title: " << vec[1].getTitle() << endl;
    //   cout << "Doc Text: " << endl << vec[1].getDocText() << endl;

    //  stringLowerCase(wordSearch);
    //  parseSearch(wordTree, wordSearch, authorHash);


    closedir(dp);

    average = wordCount / numArticles;
}

/**
 *
 * @param wordTree
 * @param wordSearch
 * @param found
 *
 * This function is used to look for the word that is requested to be searched for.
 * It will go through the word tree and check if the word exists in there. If so, put it in the found map for later use
 */
void Parsing::parseSearch(AVLTree<Words>& wordTree, string &wordSearch, map<string, int>& found) {
    stringLowerCase(wordSearch);
    stem(wordSearch);

    if(wordTree.find(wordSearch)) {
        Words temp = wordTree.get(wordSearch);

        map<string, int>::iterator iter;
        for(iter = temp.getMap().begin(); iter != temp.getMap().end(); iter++) {
            pair<string, int> temp(iter->first, iter->second);
            found.insert(pair<string, int>(iter->first, iter->second));
            //   cout << it->first << endl;
        }

    }


}

/**
 *
 * @param authorHash
 * @param search
 * @param found
 *
 * This function is used to look for the word that is requested to be searched for.
 * It will go through the author tree and check if the word exists in there. If so, put it in the found map for later use
 */
void Parsing::searchForAuthors(Hash<string, Documents> &authorHash, string &search, map<string, int> &found) {
    stringLowerCase(search);

    if(authorHash.find(search)) {
        Documents document = authorHash.get(search);
        for(int i = 0; i < document.getVec().size(); i++) {
            pair<string, int> temp(document.getVec()[i], i);
            found.insert(temp);
        }
    }
}
