#include "GameWorld.h"

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

    int isle_depth = mapTree.getIsleDepth(isle);

    int shaper_depth = mapTree.calculateMinMapDepthAccess(
        shaperTree.getDepth(realmShaper),
        shaperTree.getDepth(),
        mapTree.getDepth()
    );


    if (isle_depth >= shaper_depth)
    {
        hasAccess = true;
    }

    return hasAccess;
}

void GameWorld::exploreArea(RealmShaper *realmShaper, Isle *isle)
{
    // TODO:
    // Check if realmShaper has access
    // Use // std::cout << "[Explore Area] " << realmShaper->getName() << " does not have access to explore area " << *isle << std::endl;

    if (!hasAccess(realmShaper, isle))
    {
        std::cout << "[Explore Area] " << realmShaper->getName() << " does not have access to explore area " << isle->getName() << "." << std::endl;
        return;
    }


    // Use // std::cout << "[Explore Area] " << realmShaper->getName() << " visited " << isle->getName() << std::endl;
    // Use // std::cout << "[Energy] " << realmShaper->getName() << "'s new energy level is " << realmShaper->getEnergyLevel() << std::endl;
    // Use // std::cout << "[Owercrowding] " << isle->getName() << " self-destructed, it will be removed from the map" << std::endl;

    // If realmShaper has access
    if (hasAccess(realmShaper, isle))
    {
        // if realmShaper is already at an Isle, remove it from the Isle
        if(realmShaper->isles != nullptr)
        {
            realmShaper->isles->decreaseShaperCount();
        }

        // Visit isle,
        realmShaper->isles = isle;
        std::cout << "[Explore Area] " << realmShaper->getName() << " visited " << isle->getName() << std::endl;
        isle->increaseShaperCount();

        //collect item
        realmShaper->collectItem(isle->getItem());
        std::cout << "[Energy] " << realmShaper->getName() << "'s new energy level is " << realmShaper->getEnergyLevel() << std::endl;

        // check overcrowding for Isle
        if (isle->getShaperCount() > isle->getCapacity())
        {
            // delete Isle if necessary
            std::cout << "[Owercrowding] " << isle->getName() << "self-destructed, it will be removed from the map" << std::endl;
            mapTree.remove(isle);
        }
    }


    // You will need to implement a mechanism to keep track of how many realm shapers are at an Isle at the same time
    // There are more than one ways to do this, so it has been left completely to you
    // Use shaperCount, but that alone will not be enough,
    // you will likely need to add attributes that are not currently defined
    // to RealmShaper or Isle or other classes depending on your implementation
}

void GameWorld::craft(RealmShaper *shaper, const std::string &isleName){
    // TODO: Check energy and craft new isle if possible
    // Use std::cout << "[Energy] " << shaperName << " has enough energy points: " << shaperEnergyLevel << std::endl;
    // Use std::cout << "[Craft] " << shaperName << " crafted new Isle " << isleName << std::endl;
    // Use std::cout << "[Energy] " << shaperName << " does not have enough energy points: " << shaperEnergyLevel << std::endl;

    if (shaper->hasEnoughEnergy())
    {
        std::cout << "[Energy] " << shaper->getName() << " has enough energy points: " << shaper->getEnergyLevel() << std::endl;
        Isle *isle = new Isle(isleName);
        mapTree.insert(isle);
        shaper->loseEnergy();
        std::cout << "[Craft] " << shaper->getName() << " crafted new Isle " << isleName << std::endl;
    }
    else
    {
        std::cout << "[Energy] " << shaper->getName() << " does not have enough energy points: " << shaper->getEnergyLevel() << std::endl;
    }
}


void GameWorld::displayGameState()
{
    //mapTree.displayMap();
    //shaperTree.displayTree();
}

// TODO: Implement functions to read and parse Access and Duel logs

void GameWorld::processGameEvents(const std::string &accessLogs, const std::string &duelLogs)
{
    // Open access and duel log files
    std::ifstream input_accessf(accessLogs);
    std::ifstream input_duel(duelLogs);

    if (!input_accessf.is_open())
    {
        std::cerr << "Error: Access log file cannot be opened!" << std::endl;
        return;
    }
    if (!input_duel.is_open())
    {
        std::cerr << "Error: Duel log file cannot be opened!" << std::endl;
        return;
    }

    // Parse access logs, skipping lines that start with '#'
    std::vector<std::pair<std::string, std::string>> access_temp_vector;
    std::string access_line;
    while (std::getline(input_accessf, access_line))
    {
        if (access_line.empty() || access_line[0] == '#')
        {
            continue;
        }

        std::istringstream iss(access_line);
        std::string shaper_name, isle_name;
        iss >> shaper_name >> isle_name;

        if (!shaper_name.empty() && !isle_name.empty())
        {
            access_temp_vector.emplace_back(shaper_name, isle_name);
        }
    }
    input_accessf.close();

    // Parse duel logs, skipping lines that start with '#'
    std::vector<std::pair<std::string, bool>> duel_temp_vector;
    std::string duel_line;
    while (std::getline(input_duel, duel_line))
    {
        if (duel_line.empty() || duel_line[0] == '#')
        {
            continue;
        }

        std::istringstream iss(duel_line);
        std::string shaper_name;
        bool duel_result;
        iss >> shaper_name >> duel_result;

        if (!shaper_name.empty())
        {
            duel_temp_vector.emplace_back(shaper_name, duel_result);
        }
    }
    input_duel.close();

    // Initialize variables for tracking duel logs
    int duel_count = 0;

    // Process access logs and execute actions
    for (int ind = 0; ind < access_temp_vector.size(); ind++)
    {
        const auto &access_pair = access_temp_vector[ind];
        const std::string &frst_name = access_pair.first;
        const std::string &second_nme = access_pair.second;

        RealmShaper *shaper = shaperTree.findPlayer(frst_name);
        Isle *isle = mapTree.findIsle(second_nme);

        if (shaper == nullptr)
        {
            std::cout << "[Access Control] RealmShaper not found: " << frst_name << std::endl;
            continue;
        }

        if (isle != nullptr)
        {
            // RealmShaper explores an existing isle
            exploreArea(shaper, isle);
        }
        else
        {
            // RealmShaper crafts a new isle
            craft(shaper, second_nme);
        }

        // Trigger duels after every 5 access events, if duel logs are available
        if ((ind + 1) % 5 == 0 && duel_count < duel_temp_vector.size())
        {
            const auto &duel_pair = duel_temp_vector[duel_count];
            const std::string &challenger_name = duel_pair.first;
            bool result = duel_pair.second;

            RealmShaper *challenger = shaperTree.findPlayer(challenger_name);
            if (challenger != nullptr)
            {
                //make duel
                shaperTree.duel(challenger, result);
            }

            duel_count++;
        }
    }

    // Process any remaining duel logs
    while (duel_count < duel_temp_vector.size())
    {
        const auto &duel_pair = duel_temp_vector[duel_count];
        const std::string &first_challenger_name = duel_pair.first;
        bool duel_res = duel_pair.second;

        RealmShaper *challenger = shaperTree.findPlayer(first_challenger_name);
        if (challenger != nullptr)
        {
            //makeduel
            shaperTree.duel(challenger, duel_res);
        }

        duel_count++;
    }
}


void GameWorld::saveGameState(const std::string &currentIsles, const std::string &currentWorld, const std::string &currentShapers, const std::string &currentPlayerTree)
{
    mapTree.writeIslesToFile(currentIsles);
    mapTree.writeToFile(currentWorld);
    shaperTree.writeToFile(currentPlayerTree);
    shaperTree.writeShapersToFile(currentShapers);
}