#include "RealmShapers.h"
#include <cmath>
#include <algorithm>
#include <complex>
#include <queue>
using namespace std;

ShaperTree::ShaperTree()
{
}

ShaperTree::~ShaperTree()
{

}

void ShaperTree::initializeTree(std::vector<RealmShaper *> shapers)
{
    // TODO: Insert innitial shapers to the tree

    if (!realmShapers.empty()) {
        realmShapers.clear();
    }

    for (auto* shaper : shapers)
    {
        insert(shaper);
    }
}

int ShaperTree::getSize()
{
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

    if (index >= 0 && index < realmShapers.size())
    {
        isValid = true;
    }
    else {
        isValid = false;
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

    int aim_ind = findIndex(shaper);

    if (aim_ind == -1) {
        return -1;
    }

    realmShapers.erase(realmShapers.begin() + aim_ind);

    //left index 2*i+1
    //right index 2*i+2

    return aim_ind;
}

int ShaperTree::findIndex(RealmShaper *shaper)
{
    // return index in the tree if found
    // else

    for (int ind = 0; ind < realmShapers.size(); ind++)
    {
        if (realmShapers[ind] == shaper)
        {
            return ind;
        }
    }

    return -1;
}

int ShaperTree::getDepth(RealmShaper *shaper)
{
    // return depth of the node in the tree if found
    // else

    // finding depth iteratively

    int sharper_ind = findIndex(shaper);

    if (sharper_ind == -1)
    {
        return -1;
    }

    if (realmShapers.empty())
    {
        return 0;
    }


    queue<pair<int, int>> depth_pair_q;
    depth_pair_q.push({0, 0});

    while (!depth_pair_q.empty())
    {
        pair <int, int> temp = depth_pair_q.front();
        int ind = temp.first;
        int depth = temp.second;
        depth_pair_q.pop();

        if (realmShapers[ind]->getName() == shaper->getName())return depth;


        int left_Child = 2 * ind + 1;
        int right_Child = 2 * ind + 2;

        if (isValidIndex(left_Child))
        {
            depth_pair_q.push({left_Child, depth + 1});
        }

        if (isValidIndex(right_Child))
        {
            depth_pair_q.push({right_Child, depth + 1});
        }
    }
    return -1;
}

int ShaperTree::getDepth()
{
    // return total|max depth|height of the tree

    if (realmShapers.empty())
    {
        return 0;
    }

    int dep_Count = 0;

    queue<int> depth_queue;
    depth_queue.push(0);

    while (!depth_queue.empty())
    {
        auto size = depth_queue.size();
        dep_Count = dep_Count + 1;

        for (int i = 0; i < size; i++)
        {
            int cur_node = depth_queue.front();
            depth_queue.pop();
            int l_node = 2 * cur_node + 1;
            int r_node = 2 * cur_node + 2;

            if (isValidIndex(l_node))
            {
                depth_queue.push(l_node);
            }

            if (isValidIndex(r_node))
            {
                depth_queue.push(r_node);
            }
        }
    }
    return dep_Count;
}

RealmShaper ShaperTree::duel(RealmShaper *challenger, bool result)
{
    RealmShaper *parent = getParent(challenger);

    if (!parent) {
        std::cerr << "[Duel Error] Parent not found for " << challenger->getName() << std::endl;
        return *challenger;
    }

    RealmShaper *victor = nullptr;
    RealmShaper *loser = nullptr;

    if (result)
    {
        challenger->gainHonour();
        parent->loseHonour();
        victor = challenger;
        loser = parent;

        replace(parent, challenger);
    }
    else
    {
        challenger->loseHonour();
        parent->gainHonour();
        victor = parent;
        loser = challenger;
    }
    // ı will cout by looking the challanger

    if (result)
    {
        //[Honour] New honour points: Yaren-1950 Ahmet-1250
        std::cout << "[Duel] " << challenger->getName() << " won the duel" << std::endl;
        std::cout << "[Honour] New honour points: ";
        std::cout << challenger->getName() << "-" << challenger->getHonour() << " ";
        std::cout << parent->getName() << "-" << parent->getHonour() << std::endl;
    }
    else
    {
        //[Honour] New honour points: Mehmet - 850 Asli - 1550
        std::cout << "[Duel] " << challenger->getName() << " lost the duel" << std::endl;
        std::cout << "[Honour] New honour points: ";
        std::cout << challenger->getName() << " - " << challenger->getHonour() << " ";
        std::cout << parent->getName() << " - " << parent->getHonour() << std::endl;
    }




    //std::cout << "[Duel] " << victor->getName() << " won the duel" << std::endl;
    //std::cout << "[Honour] New honour points: ";
    //std::cout << challenger->getName() << "- " << challenger->getHonour() << " ";
    //std::cout << parent->getName() << "- " << parent->getHonour() << std::endl;

    if (loser->getHonour() <= 0)
    {
        std::cout << "[Duel] " << loser->getName() << " lost all honour, delete" << std::endl;
        remove(loser);
    }

    return *victor;
}



RealmShaper *ShaperTree::getParent(RealmShaper *shaper)
{
    RealmShaper *parent = nullptr;

    // TODO: return parent of the shaper

    int aimed_index = findIndex(shaper);

    if (isValidIndex((aimed_index - 1) / 2))
    {
        int parent_index = (aimed_index - 1) / 2;
        parent = realmShapers[parent_index];
    }

    return parent;
}

void ShaperTree::replace(RealmShaper *player_low, RealmShaper *player_high)
{
    // TODO: Change player_low and player_high's positions on the tree

    int temp_index = findIndex(player_low);
    int temp_index2 = findIndex(player_high);

    if (temp_index == -1 || temp_index2 == -1)
    {
        return;
    }

    realmShapers[temp_index] = player_high;
    realmShapers[temp_index2] = player_low;
}

RealmShaper *ShaperTree::findPlayer(RealmShaper shaper)
{
    RealmShaper *foundShaper = nullptr;

    // TODO: Search shaper by object
    // Return the shaper if found
    // Return nullptr if shaper not found

    for (auto* player : realmShapers)
    {
        if (*player == shaper)
        {
            foundShaper = player;
            break;
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

    for (auto* player : realmShapers)
    {
        if (player->getName() == name)
        {
            foundShaper = player;
            break;
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

    Recursive_inorder(index, result);
    return result;
}

void ShaperTree:: Recursive_inorder(int ind, vector<string> &inordertree) {
        if (isValidIndex(ind)) {
            Recursive_inorder(2* ind + 1, inordertree);
            inordertree.push_back(realmShapers[ind]->getName());
            Recursive_inorder(2* ind + 2, inordertree);
        }
}



std::vector<std::string> ShaperTree::preOrderTraversal(int index)
{
    std::vector<std::string> result = {};
    // TODO: Implement preOrderTraversal in tree
    // Add all to a string vector
    // Return the vector

    // Define and implement as many helper functions as necessary for recursive implementation

    recursive_preorder(index, result);

    return result;
}

void ShaperTree::recursive_preorder(int index, vector<string> &preordertree) {
    if (isValidIndex(index)) {
        preordertree.push_back(realmShapers[index]->getName());
        recursive_preorder(2* index + 1, preordertree);
        recursive_preorder(2* index + 2, preordertree);
    }
}

std::vector<std::string> ShaperTree::postOrderTraversal(int index)
{
    std::vector<std::string> result = {};
    // TODO: Implement postOrderTraversal in tree
    // Add all to a string vector
    // Return the vector

    // Define and implement as many helper functions as necessary for recursive implementation

    recursive_postorder(index , result);

    return result;
}

void ShaperTree::recursive_postorder(int index, vector<string> &postordertree) {
    if (isValidIndex(index)) {
        recursive_postorder(2* index + 1, postordertree);
        recursive_postorder(2* index + 2, postordertree);
        postordertree.push_back(realmShapers[index]->getName());
    }
}

void ShaperTree::preOrderTraversal(int index, std::ofstream &outFile)
{
    // TODO: Implement preOrderTraversal in tree
    // write nodes to output file

    // Define and implement as many helper functions as necessary for recursive implementation

    vector <string> preorder_tree = preOrderTraversal(index);

    for (auto &player : preorder_tree)
    {
        outFile << player << endl;
    }

}

void ShaperTree::breadthFirstTraversal(std::ofstream &outFile)
{
    // TODO: Implement level-order traversal
    // write nodes to output file

    // Define and implement as many helper functions as necessary

    for (int i = 0; i < realmShapers.size(); i++)
    {
        outFile << realmShapers[i]->getName() << endl;
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
    int left = 2 * index + 1;  // TODO: Calculate left index
    int right = 2 *index + 2; // TODO: Calculate right index

    if (isValidIndex(left) || isValidIndex(right))
    {
        printTree(left, level + 1, prefix + (level > 0 ? "   │   " : "")); // ╎
        printTree(right, level + 1, prefix + (level > 0 ? "   │   " : ""));
    }
}

void ShaperTree::writeShapersToFile(const std::string &filename)
{
    // TODO: Write the shapers to filename output level by level
    // Use std::cout << "[Output] " << "Shapers have been written to " << filename << " according to rankings." << std::endl;

    std::ofstream outFile(filename); // Dosya açılır

    if (!outFile)
    {
        std::cerr << "Error" << std::endl;
        return;
    }

    breadthFirstTraversal(outFile);

    outFile.close();

    cout << "[Output] " << "Shapers have been written to " << filename << " according to rankings." << endl;
}

void ShaperTree::writeToFile(const std::string &filename)
{
    // TODO: Write the tree to filename output pre-order
    // Use std::cout << "[Output] " << "Tree have been written to " << filename << " in pre-order." << std::endl;

    ofstream outFile(filename);
    if (!outFile)
    {
        cerr << "Error" << endl;
        return;
    }

    preOrderTraversal(0, outFile);

    outFile.close();

    cout << "[Output] " << "Tree have been written to " << filename << " in pre-order." << endl;
}
