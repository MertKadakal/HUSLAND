#include "RealmShapers.h"
#include <cmath>
#include <algorithm>
#include <queue>
#include <string>
using namespace std;

ShaperTree::ShaperTree()
{
}

ShaperTree::~ShaperTree()
{
    // TODO: Free any dynamically allocated memory if necessary
    for (int i = 0; i < realmShapers.size(); i++) {
        delete realmShapers.at(i);
    }
}

void ShaperTree::initializeTree(std::vector<RealmShaper *> shapers)
{
    // TODO: Insert innitial shapers to the tree
    for (int i = 0; i < shapers.size(); i++) {
        insert(shapers.at(i));
    }

}

int ShaperTree::getSize()
{
    // TODO: Return number of shapers in the tree
    return realmShapers.size();
}

std::vector<RealmShaper *> ShaperTree::getTree()
{
    return realmShapers;
}

bool ShaperTree::isValidIndex(int index)
{
    bool isValid = false;

    // TODO: Check if the index is valin in the tree
    if (index >= 0 && index < getSize()) {
        isValid = true;
    }
    return isValid;

}

void ShaperTree::insert(RealmShaper *shaper)
{
    // TODO: Insert shaper to the tree
    realmShapers.push_back(shaper);

}

int ShaperTree::remove(RealmShaper *shaper)
{
    // TODO: Remove the player from tree if it exists
    // Make sure tree protects its form (complate binary tree) after deletion of a node
    // return index if found and removed
    // else
    int ind = findIndex(shaper);
    if (isValidIndex(ind)) {
        realmShapers.erase(realmShapers.begin() + ind);
        if (shaper->lastIsle) {
            shaper->lastIsle->decreaseShaperCount();
        }
        delete shaper;
        return ind;
    }
    return -1;

}

int ShaperTree::findIndex(RealmShaper *shaper)
{
    // return index in the tree if found
    // else
    for (int i = 0; i < getSize(); i++) {
        if (realmShapers.at(i) == shaper) {
            return i;
        }
    }
    return -1;

}

int ShaperTree::getDepth(RealmShaper *shaper)
{
    // return depth of the node in the tree if found
    // else
    if (isValidIndex(findIndex(shaper))) {
        return std::floor(std::log2(findIndex(shaper) + 1));
    }
    return -1;

}

int ShaperTree::getDepth()
{
    // return total|max depth|height of the tree
    return std::floor(std::log2(getSize() + 1));

}

RealmShaper ShaperTree::duel(RealmShaper *challenger, bool result)
{
    // TODO: Implement duel logic, return the victor
    // Use   std::cout << "[Duel] " << victorName << " won the duel" << std::endl;
    // Use   std::cout << "[Honour] " << "New honour points: ";
    // Use   std::cout << challengerName << "-" << challengerHonour << " ";
    // Use   std::cout << opponentName << "-" << opponentHonour << std::endl;
    // Use   std::cout << "[Duel] " << loserName << " lost all honour, delete" << std::endl;

    RealmShaper* winner;
    RealmShaper* loser;
    RealmShaper* parent = getParent(challenger);

    if (result) {
        winner = challenger;
        loser = parent;
        replace(challenger, parent);
    } else {
        winner = parent;
        loser = challenger;
    }

    winner->gainHonour();
    loser->loseHonour();

    if (result) {
        std::cout << "[Duel] " << winner->getName() << " won the duel" << std::endl;
        std::cout << "[Honour] " << "New honour points: ";
        std::cout << challenger->getName() << "-" << challenger->getHonour() << " ";
        std::cout << parent->getName() << "-" << parent->getHonour() << std::endl;
    }
    else {
        std::cout << "[Duel] " << challenger->getName() << " lost the duel" << std::endl;
        std::cout << "[Honour] " << "New honour points: ";
        std::cout << challenger->getName() << " - " << challenger->getHonour() << " ";
        std::cout << parent->getName() << " - " << parent->getHonour() << std::endl;
    }


    if (challenger->getHonour() <= 0) {
        std::cout << "[Duel] " << challenger->getName() << " lost all honour, delete" << std::endl;
        remove(challenger);
    }
    if (parent->getHonour() <= 0) {
        std::cout << "[Duel] " << parent->getName() << " lost all honour, delete" << std::endl;
        remove(parent);
    }

    return *winner;
}


RealmShaper *ShaperTree::getParent(RealmShaper *shaper)
{
    RealmShaper *parent = nullptr;

    int index = findIndex(shaper);
    if (index==-1) return nullptr;

    int parentIndex = (index - 1) / 2; // formula for getting parent's index
    if (parentIndex >= 0) {
        parent = realmShapers.at(parentIndex);
        return parent;
    }
    return nullptr;
}

void ShaperTree::replace(RealmShaper *player_low, RealmShaper *player_high)
{
    int index_low = -1, index_high = -1;

    for (int i = 0; i < getTree().size(); ++i) {
        if (realmShapers.at(i) == player_low) {
            index_low = i;
        } else if (realmShapers.at(i) == player_high) {
            index_high = i;
        }

        if (index_low != -1 && index_high != -1) {
            break;
        }
    }

    if (index_low == -1 || index_high == -1) {
    }
    else {
        RealmShaper *temp = realmShapers.at(index_low);
        realmShapers[index_low] = realmShapers.at(index_high);
        realmShapers[index_high] = temp;
    }

}

RealmShaper *ShaperTree::findPlayer(RealmShaper shaper)
{
    RealmShaper *foundShaper = nullptr;

    // TODO: Search shaper by object
    // Return the shaper if found
    // Return nullptr if shaper not found

    for (int i = 0; i < getTree().size(); ++i) {
        if (*getTree()[i] == (shaper)) {
            foundShaper = realmShapers.at(i);
        }
    }

    return foundShaper;

}

// Find shaper by name
RealmShaper *ShaperTree::findPlayer(std::string name)
{
    RealmShaper *foundShaper = nullptr;

    // TODO: Search shaper by name
    // Return the shaper if found
    // Return nullptr if shaper not found

    for (const auto& shaper : realmShapers) {
        if (shaper->getName() == name) {
            foundShaper = shaper;
        }
    }
    return foundShaper;

}

std::vector<std::string> ShaperTree::inOrderTraversal(int index)
{
    std::vector<std::string> result = {};
    // TODO: Implement inOrderTraversal in tree
    // Add all to a string vector
    // Return the vector

    // Define and implement as many helper functions as necessary for recursive implementation

    // Note: Since SheperTree is not an binary search tree,
    // in-order traversal will not give rankings in correct order
    // for correct order you need to implement level-order traversal
    // still you are to implement this function as well

    inOrderTraversalRec(index, result);

    return result;
}

void ShaperTree::inOrderTraversalRec(int index, std::vector<std::string>& result) {
    if (index >= getSize() || realmShapers.at(index) == nullptr) {
        return;
    }

    inOrderTraversalRec(2 * index + 1, result);
    result.push_back(realmShapers.at(index)->getName());
    inOrderTraversalRec(2 * index + 2, result);
}

std::vector<std::string> ShaperTree::preOrderTraversal(int index)
{
    std::vector<std::string> result = {};
    // TODO: Implement preOrderTraversal in tree
    // Add all to a string vector
    // Return the vector

    // Define and implement as many helper functions as necessary for recursive implementation
    preOrderTraversalRec(index, result);
    return result;
}

void ShaperTree::preOrderTraversalRec(int index, std::vector<std::string>& result) {
    if (index >= getSize() || realmShapers.at(index) == nullptr) {
        return;
    }

    result.push_back(realmShapers.at(index)->getName());
    preOrderTraversalRec(2 * index + 1, result);
    preOrderTraversalRec(2 * index + 2, result);
}

std::vector<std::string> ShaperTree::postOrderTraversal(int index)
{
    std::vector<std::string> result = {};
    // TODO: Implement postOrderTraversal in tree
    // Add all to a string vector
    // Return the vector

    // Define and implement as many helper functions as necessary for recursive implementation
    postOrderTraversalRec(index, result);
    return result;
}

void ShaperTree::postOrderTraversalRec(int index, std::vector<std::string>& result) {
    if (index >= getSize() || realmShapers.at(index) == nullptr) {
        return;
    }

    postOrderTraversalRec(2 * index + 1, result);
    postOrderTraversalRec(2 * index + 2, result);
    result.push_back(realmShapers.at(index)->getName());
}

void ShaperTree::preOrderTraversal(int index, std::ofstream &outFile)
{
    // TODO: Implement preOrderTraversal in tree
    // write nodes to output file

    // Define and implement as many helper functions as necessary for recursive implementation

    if (index >= getSize() || realmShapers.at(index) == nullptr) {
        return;
    }

    outFile << getTree()[index]->getName() << std::endl;
    preOrderTraversal(2 * index + 1, outFile);
    preOrderTraversal(2 * index + 2, outFile);
}

void ShaperTree::breadthFirstTraversal(std::ofstream &outFile)
{
    // TODO: Implement level-order traversal
    // write nodes to output file

    // Define and implement as many helper functions as necessary

    if (getSize() == 0 || realmShapers.empty() || realmShapers.at(0) == nullptr) {
        return;
    }

    std::queue<int> q;

    q.push(0);

    while (!q.empty()) {
        int currentIndex = q.front();
        q.pop();

        outFile << getTree()[currentIndex]->getName() << std::endl;

        if (2 * currentIndex + 1 < getSize() && realmShapers.at(2 * currentIndex + 1) != nullptr) {
            q.push(2 * currentIndex + 1);
        }

        if (2 * currentIndex + 2 < getSize() && realmShapers.at(2 * currentIndex + 2) != nullptr) {
            q.push(2 * currentIndex + 2);
        }
    }
}

void ShaperTree::displayTree()
{
    std::cout << "[Shaper Tree]" << std::endl;
    printTree(0, 0, "");
}

// Helper function to print tree with indentation
void ShaperTree::printTree(int index, int level, const std::string &prefix)
{
    if (!isValidIndex(index))
        return;

    std::cout << prefix << (level > 0 ? "   └---- " : "") << *realmShapers[index] << std::endl;
    int left = index*2 + 1;  // TODO: Calculate left index
    int right = index*2 + 2; // TODO: Calculate right index

    if (isValidIndex(left) || isValidIndex(right))
    {
        printTree(left, level + 1, prefix + (level > 0 ? "   │   " : "")); // ╎
        printTree(right, level + 1, prefix + (level > 0 ? "   │   " : ""));
    }
}

void ShaperTree::deleteLastIsle(int index,Isle *isle) {
    if (realmShapers.at(index)->lastIsle == isle) {
        realmShapers.at(index)->lastIsle->decreaseShaperCount();
        realmShapers.at(index)->lastIsle = nullptr;
    }
}

void ShaperTree::writeShapersToFile(const std::string &filename)
{
    // TODO: Write the shapers to filename output level by level
    // Use std::cout << "[Output] " << "Shapers have been written to " << filename << " according to rankings." << std::endl;

    if (getSize() == 0 || getTree().empty() || getTree()[0] == nullptr) {
        return;
    }
    std::ofstream outFile(filename);
    std::queue<int> q;

    q.push(0);

    while (!q.empty()) {
        int currentIndex = q.front();
        q.pop();

        outFile << getTree()[currentIndex]->getName() << std::endl;

        if (2 * currentIndex + 1 < getSize() && realmShapers.at(2 * currentIndex + 1) != nullptr) {
            q.push(2 * currentIndex + 1);
        }

        if (2 * currentIndex + 2 < getSize() && realmShapers.at(2 * currentIndex + 2) != nullptr) {
            q.push(2 * currentIndex + 2);
        }
    }
    outFile.close();
    std::cout << "[Output] " << "Shapers have been written to " << filename << " according to rankings." << std::endl;
}

void ShaperTree::writeToFile(const std::string &filename)
{
    // TODO: Write the tree to filename output pre-order
    // Use std::cout << "[Output] " << "Tree have been written to " << filename << " in pre-order." << std::endl;

    std::ofstream outFile(filename);

    outFile << realmShapers.at(0)->getName() << std::endl;

    preOrderTraversal(1, outFile);

    preOrderTraversal(2, outFile);
    outFile.close();
}
