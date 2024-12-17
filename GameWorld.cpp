#include "GameWorld.h"

#include <complex>
#include <iostream>
#include <fstream>
#include <deque>
using namespace std;

GameWorld::GameWorld() : mapTree(), shaperTree() {}

void GameWorld::initializeGame(std::vector<Isle *> places, std::vector<RealmShaper *> players)
{
    shaperTree.initializeTree(players);
    mapTree.initializeMap(places);
}

Map& GameWorld::getMapTree()
{
    return mapTree;
}
ShaperTree& GameWorld::getShaperTree()
{
    return shaperTree;
}

bool GameWorld::hasAccess(RealmShaper *realmShaper, Isle *isle)
{
    bool hasAccess = false;

    // TODO: Check if the realmShaper has access to explore the isle
    // Get necessary depth values
    // Use mapTree.calculateMinMapDepthAccess
    // Use // std::cout << "[Access Control] " << "RealmShaper not found!" << std::endl;

    if (shaperTree.findIndex(realmShaper)==-1) {
        std::cout << "[Access Control] " << "RealmShaper not found!" << std::endl;
    }
    else {
        int depth = shaperTree.getDepth(realmShaper);
        int totalShaperTreeHeight = shaperTree.getDepth();
        int totalMapTreeHeight = mapTree.getDepth();

        if (mapTree.calculateMinMapDepthAccess(depth, totalShaperTreeHeight, totalMapTreeHeight) <= mapTree.getIsleDepth(isle)) {
            hasAccess = true;
        }
    }

    return hasAccess;
}

void GameWorld::exploreArea(RealmShaper *realmShaper, Isle *isle)
{
    // TODO:
    // Check if realmShaper has access
    // Use // std::cout << "[Explore Area] " << realmShaper->getName() << " does not have access to explore area " << *isle << std::endl;
    // If realmShaper has access
    // Visit isle, 
    // collect item, 
    // check overcrowding for Isle, 
    // delete Isle if necessary

    // Use // std::cout << "[Explore Area] " << realmShaper->getName() << " visited " << isle->getName() << std::endl;
    // Use // std::cout << "[Energy] " << realmShaper->getName() << "'s new energy level is " << realmShaper->getEnergyLevel() << std::endl;
    // Use // std::cout << "[Owercrowding] " << isle->getName() << " self-destructed, it will be removed from the map" << std::endl;

    // You will need to implement a mechanism to keep track of how many realm shapers are at an Isle at the same time
    // There are more than one ways to do this, so it has been left completely to you
    // Use shaperCount, but that alone will not be enough,
    // you will likely need to add attributes that are not currently defined
    // to RealmShaper or Isle or other classes depending on your implementation

    if (!hasAccess(realmShaper, isle)) {
        std::cout << "[Explore Area] " << realmShaper->getName() << " does not have access to explore area " << *isle << "." << std::endl;
    } else if (mapTree.findIsle(*isle) == nullptr) { //craft
        craft(realmShaper, isle->getName());
    } else {
        if (realmShaper->lastIsle)realmShaper->lastIsle->decreaseShaperCount();
        realmShaper->lastIsle = isle;
        realmShaper->collectItem(isle->getItem());
        std::cout << "[Explore Area] " << realmShaper->getName() << " visited " << isle->getName() << std::endl;
        std::cout << "[Energy] " << realmShaper->getName() << "'s new energy level is " << realmShaper->getEnergyLevel() << std::endl;
        if (isle->increaseShaperCount()) {
            std::cout << "[Owercrowding] " << isle->getName() << "self-destructed, it will be removed from the map" << std::endl;
            MapNode *map_node = mapTree.findNode(*isle);
            for (int i=0;i<shaperTree.getSize();i++) {
                shaperTree.deleteLastIsle(i,isle);
            }
            mapTree.remove(isle);
            delete map_node;
        }
    }
}

void GameWorld::craft(RealmShaper *shaper, const std::string &isleName){
    // TODO: Check energy and craft new isle if possible
    // Use std::cout << "[Energy] " << shaperName << " has enough energy points: " << shaperEnergyLevel << std::endl;
    // Use std::cout << "[Craft] " << shaperName << " crafted new Isle " << isleName << std::endl;
    // Use std::cout << "[Energy] " << shaperName << " does not have enough energy points: " << shaperEnergyLevel << std::endl;

    if (shaper->hasEnoughEnergy()) {
        std::cout << "[Energy] " << shaper->getName() << " has enough energy points: " << shaper->getEnergyLevel() << std::endl;
        mapTree.insert(new Isle(isleName));
        shaper->loseEnergy();
        std::cout << "[Craft] " << shaper->getName() << " crafted new Isle " << isleName << std::endl;
    } else {
        std::cout << "[Energy] " << shaper->getName() << " does not have enough energy points: " << shaper->getEnergyLevel() << std::endl;
    }
}

void GameWorld::displayGameState()
{
    mapTree.displayMap();
    shaperTree.displayTree();
}

// TODO: Implement functions to read and parse Access and Duel logs

void GameWorld::processGameEvents(const std::string &accessLogs, const std::string &duelLogs) {
    // TODO:
    // Read logs
    // For every 5 access, 1 duel happens
    // If there are still duel logs left after every access happens duels happens one after other

    // This function should call exploreArea and craft functions

    // Use displayGameState();

    int access_count = 0;
    std::ifstream accessFile(accessLogs);
    std::ifstream duelFile(duelLogs);
    std::string line;
    std::deque<std::pair<std::string, bool>> duelQueue;

    getline(accessFile, line);
    getline(duelFile, line);

    while (getline(duelFile, line)) {
        std::stringstream ss(line);
        std::string playerName;
        bool result;

        ss >> playerName >> result;
        duelQueue.push_back({playerName, result});
    }

    while (getline(accessFile, line)) {
        std::stringstream ss(line);
        std::string playerName, placeName;

        ss >> playerName >> placeName;
        access_count++;

        RealmShaper* explorer_shaper = shaperTree.findPlayer(playerName);
        Isle* explored_isle = mapTree.findIsle(placeName);
        if (explored_isle==nullptr) {
            craft(explorer_shaper, placeName);
        }
        else {
            exploreArea(explorer_shaper, explored_isle);
        }

        if (access_count % 5 == 0 && !duelQueue.empty()) {
            auto duel = duelQueue.front();
            duelQueue.pop_front();
            shaperTree.duel(shaperTree.findPlayer(duel.first), duel.second);
        }
    }

    while (!duelQueue.empty()) {
        auto duel = duelQueue.front();
        duelQueue.pop_front();
        if (shaperTree.findPlayer(duel.first)!=nullptr) {
            shaperTree.duel(shaperTree.findPlayer(duel.first), duel.second);
        }
    }

    accessFile.close();
    duelFile.close();
}

void GameWorld::saveGameState(const std::string &currentIsles, const std::string &currentWorld, const std::string &currentShapers, const std::string &currentPlayerTree)
{
    mapTree.writeIslesToFile(currentIsles);
    mapTree.writeToFile(currentWorld);
    shaperTree.writeToFile(currentPlayerTree);
    shaperTree.writeShapersToFile(currentShapers);
}