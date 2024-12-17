#include "Map.h"
#include <algorithm>
#include <iostream>
#include <queue>

using namespace std;

void postOrderDeletion(MapNode* node) {
    if (node == nullptr) return;
    postOrderDeletion(node->left);
    postOrderDeletion(node->right);
    delete node;
}

Map::Map()
{
    this->root = nullptr;
    this->count_rebalancing = 0;
    this->isRebalanced = false;
    this->isInit = false;
}
Map::~Map()
{
    // TODO: Free any dynamically allocated memory if necessary
    std::cerr << "bitti"<< std::endl;
    postOrderDeletion(root);
}

void Map::check_amazonite() {
    count_rebalancing++;
    if (count_rebalancing%3 == 0) {
        populateWithItems();
        dropItemBFS();
    }
    isRebalanced = false;
}

void Map::initializeMap(std::vector<Isle *> isles)
{
    // TODO: Insert innitial isles to the tree
    // Then populate with Goldium and Einstainium items

    isInit = true;
    for (Isle* isle : isles) {
        insert(isle);
    }
    isInit = false;

    populateWithItems();
}

MapNode *Map::rotateRight(MapNode *current)
{
    // TODO: Perform right rotation according to AVL
    // return necessary new root
    // Use std::cerr << "[Right Rotation] " << "Called on invalid node!" << std::endl;
    if (current == nullptr || current->left == nullptr) {
        std::cerr << "[Right Rotation] " << "Called on invalid node!" << std::endl;
        return nullptr;
    }

    MapNode* left = current->left; // left child of current
    MapNode* left_right = left->right; // right child of the left child of current

    // rotation
    left->right = current;
    current->left = left_right;

    // update heights
    current->height = max(height(current->left), height(current->right)) + 1;
    left->height = max(height(left->left), height(left->right)) + 1;

    return left;
}

MapNode *Map::rotateLeft(MapNode *current)
{
    // TODO: Perform left rotation according to AVL
    // return necessary new root
    // Use std::cerr << "[Left Rotation] " << "Called on invalid node!" << std::endl;
    if (current == nullptr  || current->right == nullptr) {
        std::cerr << "[Left Rotation] " << "Called on invalid node!" << std::endl;
        return nullptr;
    }

    MapNode* right = current->right;  // right child of current
    MapNode* right_left = right->left; // left child of the right child of current

    // rotation
    right->left = current;
    current->right = right_left;

    // update heights
    current->height = max(height(current->left), height(current->right)) + 1;
    right->height = max(height(right->left), height(right->right)) + 1;

    return right;
}

int Map::calculateMinMapDepthAccess(int playerDepth, int totalShaperTreeHeight, int totalMapDepth)
{
    return (int)totalMapDepth * ((double)playerDepth / totalShaperTreeHeight);
}

int Map::height(MapNode *node)
{
    // TODO: Return height of the node
    if (node)return node->height;
    return 0;
}

MapNode *Map::insert(MapNode *node, Isle *isle)
{
    MapNode *newNode = nullptr;

    if (node==nullptr) {
        newNode = new MapNode(isle);
        return newNode;
    }

    // TODO: Recursively insert isle to the tree
    // returns inserted node

    if (isle->operator<(*node->isle))
        node->left = insert(node->left, isle);
    else if (isle->operator>(*node->isle))
        node->right = insert(node->right, isle);

    // update height
    node->height = 1 + max(height(node->left), height(node->right));

    int balance = getBalanceFactor(node);

    if (balance > 1) { // (Left-heavy)
        if (isle->operator<(*node->left->isle)) { //(Left-Left)
            if (!isInit) isRebalanced = true;
            return rotateRight(node);
        } else if (isle->operator>(*node->left->isle)) { // (Left-Right)
            node->left = rotateLeft(node->left);
            if (!isInit) isRebalanced = true;
            return rotateRight(node);
        }
    } else if (balance < -1) { // (Right-heavy)
        if (isle->operator>(*node->right->isle)) { // (Right-Right)
            if (!isInit) isRebalanced = true;
            return rotateLeft(node);
        } else if (isle->operator<(*node->right->isle)) { // (Right-Left)
            node->right = rotateRight(node->right);
            if (!isInit) isRebalanced = true;
            return rotateLeft(node);
        }
    }


    return node;
}

void Map::insert(Isle *isle)
{
    root = insert((root), isle);

    if (isRebalanced) {
        check_amazonite();
    }
    // you might need to insert some checks / functions here depending on your implementation
}

MapNode* findMax(MapNode* node) {
    while (node && node->right != nullptr) {
        node = node->right;
    }
    return node;
}

MapNode *Map::remove(MapNode *node, Isle *isle)
{
    // TODO: Recursively delete isle from the tree
    // Will be called if there is overcrowding
    // returns node neye bakıcaz bak log gösterem
    // Use std::cout << "[Remove] " << "Tree is Empty" << std::endl;

    if (node == nullptr) {
        std::cout << "[Remove] Tree is Empty" << std::endl;
        return nullptr;
    }

    // if the node to delete is found
    if (node->isle->getName() == isle->getName()) {
        // Node has no children (leaf node)
        if (node->left == nullptr && node->right == nullptr) {
            return nullptr;
        }
        // Node has one child
        if (node->left == nullptr) { //only right child
            MapNode* temp = node->right;
            return temp;
        }
        if (node->right == nullptr) { //only left child
            MapNode* temp = node->left;
            return temp;
        }
        // Node has two children
        // Find inorder successor (biggest in the left subtree according to the assignment) and replace with deleted node
        MapNode* temp = findMax(node->left);
        MapNode *oldNode = node;
        if (temp==node->left) {
            node = temp;
            return node;
        }
        node = temp;
        node->left = remove(oldNode->left, temp->isle);
        node->right = oldNode->right;
        return node;
    }
    // Recur down the tree
    if (isle->operator<(*node->isle)) {
        node->left = remove(node->left, isle);
    } else {
        node->right = remove(node->right, isle);
    }

    // Balance the tree after deletion using rotations
    int balance = getBalanceFactor(node);

    if (balance > 1) { // Left-heavy
        if (getBalanceFactor(node->left) >= 0) {
            isRebalanced = true;
            return rotateRight(node); // Left-Left
        } else {
            isRebalanced = true;
            node->left = rotateLeft(node->left); // Left-Right
            return rotateRight(node);
        }
    } else if (balance < -1) { // Right-heavy
        if (getBalanceFactor(node->right) <= 0) {
            isRebalanced = true;
            return rotateLeft(node); // Right-Right
        } else {
            isRebalanced = true;
            node->right = rotateRight(node->right); // Right-Left
            return rotateLeft(node);
        }
    }


    return node;
}

void Map::remove(Isle *isle)
{
    root = remove((root), isle);

    if (isRebalanced) {
        check_amazonite();
    }

    // you might need to insert some checks / functions here depending on your implementation
}

void Map::preOrderItemDrop(MapNode *current, int &count)
{
    // TODO: Drop EINSTEINIUM according to rules
    // Use std::cout << "[Item Drop] " << "EINSTEINIUM dropped on Isle: " << current->isle->getName() << std::endl;

    if (current == nullptr) return;
    count++;
    if (count % 5 == 0 && current->isle->getItem() != AMAZONITE) {
        std::cout << "[Item Drop] " << "EINSTEINIUM dropped on Isle: " << current->isle->getName() << std::endl;
        current->isle->setItem(EINSTEINIUM);
    }
    preOrderItemDrop(current->left, count);
    preOrderItemDrop(current->right, count);
}

// to Display the values by Post Order Method .. left - right - node
void Map::postOrderItemDrop(MapNode *current, int &count)
{
    // TODO: Drop GOLDIUM according to rules
    // Use  std::cout << "[Item Drop] " << "GOLDIUM dropped on Isle: " << current->isle->getName() << std::endl;

    if (current == nullptr) return;
    postOrderItemDrop(current->left, count);
    postOrderItemDrop(current->right, count);
    count++;
    if (count % 3 == 0 && current->isle->getItem() != AMAZONITE) {
        std::cout << "[Item Drop] " << "GOLDIUM dropped on Isle: " << current->isle->getName() << std::endl;
        current->isle->setItem(GOLDIUM);
    }
}

MapNode *Map::findFirstEmptyIsle(MapNode *node)
{
    // Return nullptr if the input node is null
    if (node == nullptr) return nullptr;

    // level-order traversal
    std::queue<MapNode *> queue;
    queue.push(node);

    while (!queue.empty()) {
        MapNode *current = queue.front();
        queue.pop();

        if (current->isle->getItem() == EMPTY) {
            return current;
        }

        if (current->left != nullptr) {
            queue.push(current->left);
        }
        if (current->right != nullptr) {
            queue.push(current->right);
        }
    }

    return nullptr;
}


void Map::dropItemBFS()
{
    // TODO: Drop AMAZONITE according to rules
    // Use std::cout << "[BFS Drop] " << "AMAZONITE dropped on Isle: " << targetNode->isle->getName() << std::endl;
    // Use std::cout << "[BFS Drop] " << "No eligible Isle found for AMAZONITE drop." << std::endl;

    MapNode *targetNode = findFirstEmptyIsle(root);

    if (targetNode == nullptr) {
        std::cout << "[BFS Drop] " << "No eligible Isle found for AMAZONITE drop." << std::endl;
    }
    else if (targetNode->isle->getItem() == EMPTY) {
        targetNode->isle->setItem(AMAZONITE);
        std::cout << "[BFS Drop] " << "AMAZONITE dropped on Isle: " << targetNode->isle->getName() << std::endl;
    }
}

void Map::displayMap()
{
    std::cout << "[World Map]" << std::endl;
    display(root, 0, 0);
}

int Map::getDepth(MapNode *node)
{
    // TODO: Return node depth if found, else
    return getIsleDepth(node->isle);
}

// Function to calculate the depth of a specific node in the AVL tree
int Map::getIsleDepth(Isle *isle)
{
    // TODO: Return node depth by isle if found, else
    MapNode* current = root;
    int depth = 0;

    while (current != nullptr) {
        if (isle == current->isle) {
            return depth;
        }
        if (*isle < *current->isle) {
            current = current->left;
        } else {
            current = current->right;
        }
        depth++;
    }

    return -1;
}

int Map::getDepth()
{
    // TODO: Return max|total depth of tree
    return getMaxDepth(root);
}

void Map::populateWithItems()
{
    // TODO: Distribute fist GOLDIUM than EINSTEINIUM


    int count1=0;
    postOrderItemDrop(root, count1);
    int count=0;
    preOrderItemDrop(root, count);
}

Isle *Map::findIsle(Isle isle)
{
    // TODO: Find isle by value
    MapNode* current = root;

    while (current != nullptr) {
        if (isle.operator==(*current->isle)) {
            return current->isle;
        }
        if (isle.operator<(*current->isle)) {
            current = current->left;
        } else {
            current = current->right;
        }
    }

    return nullptr;
}

Isle *Map::findIsle(std::string name)
{
    // TODO: Find isle by name
    MapNode* current = root;
    Isle isle(name);

    while (current != nullptr) {
        if (current->isle->getName() == name) {
            return current->isle;
        }
        if (isle.operator<(*current->isle)) {
            current = current->left;
        } else {
            current = current->right;
        }
    }

    return nullptr;
}

MapNode *Map::findNode(Isle isle)
{
    // TODO: Find node by value
    MapNode* current = root;

    while (current != nullptr) {
        if (isle.operator==(*current->isle)) {
            return current;
        }
        if (isle.operator<(*current->isle)) {
            current = current->left;
        } else {
            current = current->right;
        }
    }

    return nullptr;
}

MapNode *Map::findNode(std::string name)
{
    // TODO: Find node by name
    MapNode* current = root;

    Isle isle(name);

    while (current != nullptr) {
        if (isle.operator==(*current->isle)) {
            return current;
        }
        if (isle.operator<(*current->isle)) {
            current = current->left;
        } else {
            current = current->right;
        }
    }

    return nullptr;
}

void Map::display(MapNode *current, int depth, int state)
{
    // SOURCE:

    if (current->left)
        display(current->left, depth + 1, 1);

    for (int i = 0; i < depth; i++)
        printf("     ");

    if (state == 1) // left
        printf("   ┌───");
    else if (state == 2) // right
        printf("   └───");

    std::cout << "[" << *current->isle << "] - (" << current->height << ")\n"
              << std::endl;

    if (current->right)
        display(current->right, depth + 1, 2);
}

bool isOnlyNullWords(const string& str) {
    stringstream ss(str);
    string word;

    while (ss >> word) {
        if (word != "NULL") {
            return false;
        }
    }

    return true;
}

void Map::writeToFile(const std::string &filename)
{
    // TODO: Write the tree to filename output level by level
    std::ofstream outFile(filename);

    queue<MapNode*> currentLevel;
    queue<MapNode*> nextLevel;

    currentLevel.push(root);

    while (!currentLevel.empty()) {
        string levelOutput = "";

        while (!currentLevel.empty()) {
            MapNode* current = currentLevel.front();
            currentLevel.pop();

            if (current) {
                levelOutput += current->isle->getName() + " ";
                nextLevel.push(current->left);
                nextLevel.push(current->right);
            } else {
                levelOutput += "NULL ";
            }
        }

        if (isOnlyNullWords(levelOutput)) break;

        outFile << levelOutput << endl;

        swap(currentLevel, nextLevel);
    }

    outFile.close();
}

void inorderTra(MapNode *current, std::ofstream &file) {
    if (current==nullptr) return;
    inorderTra(current->left, file);
    file << current->isle->getName() << std::endl;
    inorderTra(current->right, file);
}

void Map::writeIslesToFile(const std::string &filename)
{
    // TODO: Write Isles to output file in alphabetical order
    std::ofstream outFile(filename);

    inorderTra(root, outFile);

    outFile.close();
    std::cout << "[Output] " << "Isles have been written to " << filename << " in in alphabetical order." << std::endl;
}

// Yükseklik farkı (balance factor) hesaplama
int Map::getBalanceFactor(MapNode* node) {
    if (node == nullptr)
        return 0;
    return height(node->left) - height(node->right);
}

int Map::getMaxDepth(MapNode *node)
{
    if (node == nullptr)
        return 0;

    if (node->left==nullptr && node->right==nullptr)return getDepth(node);

    int leftDepth = getMaxDepth(node->left);
    int rightDepth = getMaxDepth(node->right);

    return std::max(leftDepth, rightDepth);
}
