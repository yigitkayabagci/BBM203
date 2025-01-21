#include "RealmShaper.h"
#include <fstream>
#include <sstream>
#include <iostream>
using namespace std;

#define NECESSARY_ENERGY 2000 // Necessary energy to craft an Isle

RealmShaper::RealmShaper(std::string name, int honour) : name(name)
{
    this->honour = honour;
}

RealmShaper::~RealmShaper()
{
}

const std::string &RealmShaper::getName() const
{
    return this->name;
}

int RealmShaper::getEnergyLevel()
{
    return this->collectedEnergyPoints;
}

int RealmShaper::getHonour()
{
    return this->honour;
}

void RealmShaper::gainHonour()
{
    this->honour += 150;
}

void RealmShaper::loseHonour()
{
    this->honour -= 150;
}

void RealmShaper::collectItem(Item item)
{
    this->collectedEnergyPoints += item;
}

void RealmShaper::loseEnergy()
{
    this->collectedEnergyPoints -= NECESSARY_ENERGY;
}

bool RealmShaper::hasEnoughEnergy()
{
    return this->collectedEnergyPoints >= NECESSARY_ENERGY;
}

std::vector<RealmShaper *> RealmShaper::readFromFile(const std::string &filename)
{
    std::vector<RealmShaper *> players;

    // TODO: Read RealmShapers from the file,
    // add them to vector
    // return the vector
    // Input format: playerName[tab]honourPoints

    ifstream sharp_player_infile = ifstream(filename);

    if (!sharp_player_infile.is_open())
    {
        cerr << "File could not be opened" << endl;
        return players;
    }

    string temp_line;

    while (getline (sharp_player_infile, temp_line))
    {
        size_t tab_index = temp_line.find(' ');
        if (tab_index == string::npos)
        {
            cerr << "Tab not found in line: " << temp_line << endl;
            continue;
        }
        string player_name = temp_line.substr(0, tab_index);
        string player_honour = temp_line.substr(tab_index + 1);
        int player_honour_int = stoi(player_honour);

        auto *new_player = new RealmShaper(player_name, player_honour_int);
        players.push_back(new_player);
    }

    sharp_player_infile.close();
    return players;
}

bool RealmShaper::operator==(const RealmShaper &other) const
{
    // TODO: Compare by name, return true if same

    if (this->name == other.name)
    {
        return true;
    }
    return false;
}

std::ostream &operator<<(std::ostream &os, const RealmShaper &p)
{
    // Outs the player name
    return (os << p.name); // << std::endl);
}