
#include "AVLTree.h"
#include "Documents.h"
#include <string>
#include "Hash.h"
#include "Words.h"
#include "UserInterface.h"

using namespace rapidjson;
using namespace std;

int main() {
    AVLTree<Words> wordTree;
    Hash<string, Documents> authorTree;

    UserInterface userInterface;
    userInterface.run(authorTree, wordTree);



    return 0;
}
