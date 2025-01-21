#include "Isle.h"

Isle::Isle(std::string name) : name(name)
{
}


const std::string& Isle::getName() const
{
    return this->name;
}
Item Isle::getItem()
{
    return item;
}

void Isle::setItem(Item item)
{
    this->item = item;
}

int Isle::getShaperCount()
{
    return shaperCount;
}

bool Isle::increaseShaperCount()
{
    bool isFull = false;

    // TODO: Increase shaperCount if necessary
    // return isFull, True if capacity is exceded, false otherwise

    shaperCount++;
    if (shaperCount > capacity)
    {
        isFull = true;
    }

    return isFull;
}

bool Isle::decreaseShaperCount()
{
    bool isEmpty = true;

    // TODO: Decrease shaperCount if necessary
    // return isEmpty, True if shaper count less and equal to 0, false otherwise

    shaperCount--;
    if (shaperCount > 0)
    {
        isEmpty = false;
    }

    return isEmpty;
}

bool Isle::operator==(const Isle &other) const
{
    // TODO: Compare by name, return true if same

    if (this->name == other.name)
    {
        return true;
    }
    return false;
}

bool Isle::operator<(const Isle &other) const
{
    // TODO: Compare by name

    if (this->name < other.name)
    {
        return true;
    }

    return false;
}
int Isle::getCapacity()
{
    return capacity;
}

bool Isle::operator>(const Isle &other) const
{
    // TODO: Compare by name

    if (this->name > other.name)
    {
        return true;
    }

    return false;
}
/*
void Isle::addShaper(RealmShaper *shaper) {
    if (getShaperCount() < getCapacity()) {
        shaperList.push_back(shaper);
        increaseShaperCount();
    }
}

void Isle::removeShaper(RealmShaper *shaper) {
    for (int i = 0; i < shaperList.size(); i++) {
        if (shaperList[i] == shaper) {
            shaperList.erase(shaperList.begin() + i);
            decreaseShaperCount();
            break;
        }
    }
}

bool Isle::hasShaper(RealmShaper *shaper) {
    for (auto & i : shaperList) {
        if (i == shaper) {
            return true;
        }
    }
    return false;
}*/


// Implementation of readFromFile
std::vector<Isle *> Isle::readFromFile(const std::string &filename)
{
    std::vector<Isle *> isles;
    // TODO: Read isles from the file,
    // add them to vector
    // return the vector
    // Input format: isleName

    std::ifstream isle_file(filename);

    if (!isle_file.is_open())
    {
        std::cerr << "File could not be opened!" << std::endl;
        return isles;
    }

    std::string temp_line;
    while (std::getline(isle_file, temp_line))
    {
        Isle *isle = new Isle(temp_line);
        isles.push_back(isle);
    }

    isle_file.close();

    return isles;
}

std::ostream &operator<<(std::ostream &os, const Isle &p)
{
    // Prints to terminal with color
    // This function might cause some problems in terminals that are not Linux based
    // If so, comment out the colors while testing on local machine
    // But open them back up while submitting or using TurBo

    std::string einsteiniumColor = "\033[38;2;6;134;151m";
    std::string goldiumColor = "\033[38;2;255;198;5m";
    std::string amazoniteColor = "\033[38;2;169;254;255m";
    std::string resetColorTag = "\033[0m";

    if (p.item == EINSTEINIUM)
        return (os << einsteiniumColor << p.name << resetColorTag);
    if (p.item == GOLDIUM)
        return (os << goldiumColor << p.name << resetColorTag);
    if (p.item == AMAZONITE)
        return (os << amazoniteColor << p.name << resetColorTag);
    return (os << p.name);
}