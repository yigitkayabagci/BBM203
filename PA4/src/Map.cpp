#include "Map.h"

#include <algorithm>
using namespace std;

Map::Map()
{
    this->root = nullptr;
}
Map::~Map()
{
    // TODO: Free any dynamically allocated memory if necessaryi
    Clear(root);
    root = nullptr;
}

void Map::Clear(MapNode *&node)
{
    if (node == nullptr)
        return;

    Clear(node->left);
    Clear(node->right);
    delete node;
}


void Map::initializeMap(std::vector<Isle *> isles)
{
    // TODO: Insert innitial isles to the tree

    for (auto *isle : isles)
    {
        insert(isle);
        balanced_Count = 0;
    }

    // Then populate with Goldium and Einstainium items
    populateWithItems();

}

MapNode *Map::rotateRight(MapNode *current)
{
    // TODO: Perform right rotation according to AVL
    // return necessary new root
    // Use std::cerr << "[Right Rotation] " << "Called on invalid node!" << std::endl;

    MapNode *new_root = current->left;
    MapNode * temp_node = new_root->right;
    new_root->right = current;
    current->left = temp_node;

    current->height = max(height(current->left), height(current->right)) + 1;
    new_root->height = max(height(new_root->left), height(new_root->right)) + 1;

    return new_root;
}

MapNode *Map::rotateLeft(MapNode *current)
{
    // TODO: Perform left rotation according to AVL
    // return necessary new root
    // Use std::cerr << "[Left Rotation] " << "Called on invalid node!" << std::endl;

    MapNode *new_root = current->right;
    MapNode * temp_node = new_root->left;

    new_root->left = current;
    current->right = temp_node;

    current->height = max(height(current->left), height(current->right)) + 1;
    new_root->height = max(height(new_root->left), height(new_root->right)) + 1;

    return new_root;
}

MapNode *Map::balance(MapNode *current)
{
    if (!current)
        return nullptr;

    int balanceFactor = find_balance_factor(current);

    if (balanceFactor > 1 && find_balance_factor(current->left) >= 0) {
        balanced_Count++;
        return rotateRight(current);
    }

    if (balanceFactor > 1 && find_balance_factor(current->left) < 0) {
        balanced_Count+=1;
        current->left = rotateLeft(current->left);
        return rotateRight(current);
    }

    if (balanceFactor < -1 && find_balance_factor(current->right) <= 0) {
        balanced_Count = balanced_Count + 1;
        return rotateLeft(current);
    }

    if (balanceFactor < -1 && find_balance_factor(current->right) > 0) {
        balanced_Count+=1;
        current->right = rotateRight(current->right);
        return rotateLeft(current);
    }

    return current;
}


int Map::find_balance_factor(MapNode *current)
{
    if (current == nullptr)
    {
        return 0;
    }
    else {
        return height(current->left) - height(current->right);
    }
}

int Map::calculateMinMapDepthAccess(int playerDepth, int totalShaperTreeHeight, int totalMapDepth)
{
    return (int)totalMapDepth * ((double)playerDepth / totalShaperTreeHeight);
}

int Map::height(MapNode *node)
{
    // TODO: Return height of the node

    if (node == nullptr)
    {
        return 0;
    }

    queue <MapNode *> map_queue;
    map_queue.push(node);
    int height = 0;

    while (!map_queue.empty())
    {
        int size = map_queue.size();
        for (int ind = 0; ind < size; ind++)
        {
            MapNode *current = map_queue.front();
            map_queue.pop();

            if (current->left != nullptr)
            {
                map_queue.push(current->left);
            }
            if (current->right != nullptr)
            {
                map_queue.push(current->right);
            }
        }
        height++;
    }

    return height;
}

MapNode *Map::insert(MapNode *node, Isle *isle)
{
    MapNode *newNode = nullptr;
    // TODO: Recursively insert isle to the tree
    // returns inserted node
    if (!node) {
        newNode = new MapNode(isle);
        return newNode;
    }
    if (isle->getName() < node->isle->getName()) {
        node->left = insert(node->left, isle);
    } else if (isle->getName() > node->isle->getName()) {
        node->right = insert(node->right, isle);
    } else {
        return node;
    }
    node->height = 1 + max(height(node->left), height(node->right));
    newNode = balance(node);
    return newNode;
}

void Map::insert(Isle *isle)
{
    root = insert(root, isle);

    if (balanced_Count >= 3) {

        populateWithItems();
        dropItemBFS();

        balanced_Count = 0;
    }
}
MapNode *Map::remove(MapNode *node, Isle *isle)
{
    if (!node)
    {
        std::cout << "[Remove] Tree is Empty" << std::endl;
        return nullptr;
    }

    if (isle->getName() < node->isle->getName())
    {
        node->left = remove(node->left, isle);
    }
    else if (isle->getName() > node->isle->getName())
    {
        node->right = remove(node->right, isle);
    }
    else
    {

        if (!node->left || !node->right)
        {
            if (node->left)
            {
                MapNode *temp = node->left;
             return temp;
            }
            else if (node->right){
                MapNode *temp = node->right;
                return temp;
            }
            else{return nullptr;}
        }

        MapNode *find_leftmax = node->left;
        while (find_leftmax->right)find_leftmax = find_leftmax->right;

        node->isle = find_leftmax->isle;
        node->left = remove(node->left, find_leftmax->isle);
    }

    node->height = 1 + max(height(node->left), height(node->right));
    node = balance(node);

    return node;
}



void Map::remove(Isle *isle)
{
    root = remove(root, isle);

    if (balanced_Count >= 3) {

        populateWithItems();
        dropItemBFS();

        balanced_Count = 0;
    }
}
void Map::preOrderItemDrop(MapNode *current, int &count)
{
    // TODO: Drop EINSTEINIUM according to rules
    // Use std::cout << "[Item Drop] " << "EINSTEINIUM dropped on Isle: " << current->isle->getName() << std::endl;

    if (current == nullptr)
    {
        return;
    }

    count+= 1;
    if (count % 5== 0)
    {
        cout << "[Item Drop] " << "EINSTEINIUM dropped on Isle: " << current->isle->getName() << endl;
        current->isle->setItem(EINSTEINIUM);
    }

    //visit left child
    preOrderItemDrop(current->left, count);

    //visit right child
    preOrderItemDrop(current->right, count);

}

// to Display the values by Post Order Method .. left - right - node
void Map::postOrderItemDrop(MapNode *current, int &count)
{
    // TODO: Drop GOLDIUM according to rules
    // Use  std::cout << "[Item Drop] " << "GOLDIUM dropped on Isle: " << current->isle->getName() << std::endl;

    if (current == nullptr)
    {
        return;
    }

    postOrderItemDrop(current->left, count);
    postOrderItemDrop(current->right, count);

    count = count + 1;
    if (count % 3 == 0)
    {
        cout << "[Item Drop] " << "GOLDIUM dropped on Isle: " << current->isle->getName() << endl;
        current->isle->setItem(GOLDIUM);
    }
}

MapNode *Map::findFirstEmptyIsle(MapNode *node)
{
    // TODO: Find first Isle with no item

    if (node == nullptr)
    {
        return nullptr;
    }

    queue <MapNode *> map_node_queue;
    map_node_queue.push(node);

    while (!map_node_queue.empty())
    {
        MapNode* curr_map_node = map_node_queue.front();
        map_node_queue.pop();

        if (curr_map_node->isle != nullptr && curr_map_node->isle->getItem() == EMPTY) {
            return curr_map_node;
        }


        if (curr_map_node->left != nullptr)map_node_queue.push(curr_map_node->left);
        if (curr_map_node->right != nullptr){map_node_queue.push(curr_map_node->right);}
    }
    return nullptr;
}

void Map::dropItemBFS()
{
    // TODO: Drop AMAZONITE according to rules
    // Use std::cout << "[BFS Drop] " << "AMAZONITE dropped on Isle: " << targetNode->isle->getName() << std::endl;
    // Use std::cout << "[BFS Drop] " << "No eligible Isle found for AMAZONITE drop." << std::endl;

    MapNode* targetNode = findFirstEmptyIsle(root);
    if (targetNode != nullptr && targetNode->isle->getItem() == EMPTY)
    {
        cout << "[BFS Drop] " << "AMAZONITE dropped on Isle: " << targetNode->isle->getName() << endl;
        targetNode->isle->setItem(AMAZONITE);
    }
    else
    {
        cout << "[BFS Drop] " << "No eligible Isle found for AMAZONITE drop." << endl;
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
    if (node == nullptr){return -1;}
    if (!root) return -1;

    int depth_count = 0;
    MapNode *curr_node = root;

    while (curr_node != nullptr) {
        if (node == curr_node) {
            return depth_count ;
        } else if (node->isle->getName() < curr_node->isle->getName()) {
            curr_node = curr_node->left;
        } else {
            curr_node = curr_node->right;
        }
        depth_count++;
    }
    return -1;
}

// Function to calculate the depth of a specific node in the AVL tree
int Map::getIsleDepth(Isle *isle)
{
    // TODO: Return node depth by isle if found, else

    if (isle == nullptr)
    {
        return -1;
    }
    if (!root)
    {
        return -1;
    }
    int dep = 0;
    MapNode *current_node = root;
    while (current_node != nullptr) {
        if (isle == current_node->isle) {
            return dep;
        }
        else if (isle->getName() < current_node->isle->getName()) {
            current_node = current_node->left;
        }
        else {
            current_node = current_node->right;
        }
        dep++;
    }

    return -1;
}

int Map::getDepth()
{
    // TODO: Return max|total depth of tree

    if (root == nullptr)return 0;
    int max_depth = height(root);

    return max_depth;
}

void Map::populateWithItems()
{
    //Distribute fist GOLDIUM than EINSTEINIUM
    int count1 = 0;
    int count2 = 0;
    postOrderItemDrop(root, count1);
    preOrderItemDrop(root, count2);
}

Isle* Map::findIsle(Isle isle) {
    MapNode* current_Temp = root;

    while (current_Temp != nullptr) {
        if (*current_Temp->isle == isle) {
            return current_Temp->isle;
        }
        if (isle < *current_Temp->isle) {
            current_Temp = current_Temp->left;
        } else {
            current_Temp = current_Temp->right;
        }
    }

    return nullptr; // Ada bulunamadı
}

Isle *Map::findIsle(std::string name)
{
    // TODO: Find isle by name

    MapNode *current_tmp = root;

    while (current_tmp != nullptr)
    {
        if (current_tmp->isle->getName() == name)
        {
            return current_tmp->isle;
        }
        if (name < current_tmp->isle->getName())
        {
            current_tmp = current_tmp->left;
        }
        else
        {
            current_tmp = current_tmp->right;
        }
    }
    return nullptr;
}

MapNode* Map::findNode(Isle isle) {
    MapNode* current = root;

    while (current != nullptr) {
        if (current->isle == &isle) {
            return current;
        }
        if (isle < *current->isle) {
            current = current->left;
        } else {
            current = current->right;
        }
    }

    return nullptr;
}

MapNode* Map::findNode(std::string name) {
    MapNode* current = root;

    while (current != nullptr) {
        if (current->isle->getName() == name) {
            return current;
        }
        if (name < current->isle->getName()) {
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

void Map::writeToFile(const std::string &filename)
{
    std::ofstream output_file(filename);
    if (!output_file.is_open())
    {
        std::cerr << "Error: Unable to open file: " << filename << std::endl;
        return;
    }

    if (!root)
    {
        return;
    }

    std::queue<MapNode*> write_mapnode_queue;
    write_mapnode_queue.push(root);

    while (!write_mapnode_queue.empty())
    {
        int level_size = (int)write_mapnode_queue.size();
        bool level_has_non_null = false;
        std::vector<MapNode*> current_nodes;
        current_nodes.reserve(level_size);

        for (int i = 0; i < level_size; i++)
        {
            MapNode* node = write_mapnode_queue.front();
            write_mapnode_queue.pop();
            current_nodes.push_back(node);

            if (node != nullptr)
            {
                level_has_non_null = true;
            }
        }

        if (!level_has_non_null)
        {
            break;
        }

        std::ostringstream oss;


        for (auto node : current_nodes)
        {
            if (node)
                oss << node->isle->getName() << " ";
            else
                oss << "NULL ";
        }

        std::string level_str = oss.str();

        output_file << level_str << "\n";

        if (!level_has_non_null)
        {
            break;
        }

        for (auto node : current_nodes)
        {
            if (node)
            {
                if(node->left)
                {
                    write_mapnode_queue.push(node->left);
                }
                else
                {
                    write_mapnode_queue.push(nullptr);
                }
                if (node->right) {
                    write_mapnode_queue.push(node->right);
                }
                else
                {
                    write_mapnode_queue.push(nullptr);
                }
            }
            else
            {
                write_mapnode_queue.push(nullptr);
                write_mapnode_queue.push(nullptr);
            }
        }
    }

    output_file.close();
    cout << "[Output] " << "Map has been written to " << filename << endl;
}

void Map::CollectAllIsles(MapNode* current, std::vector<Isle*>& isle_vector)
{
    if (current == nullptr)
    {
        return;
    }

    CollectAllIsles(current->left, isle_vector);
    isle_vector.push_back(current->isle);
    CollectAllIsles(current->right, isle_vector);
}

void Map::writeIslesToFile(const std::string &filename)
{
    // TODO: Write Isles to output file in alphabetical order
    // Use std::cout << "[Output] " << "Isles have been written to " << filename << " in in alphabetical order." << std::endl;


    if(root == nullptr)
    {
        return;
    }

    vector<Isle*> temp_isle_vector;

    CollectAllIsles(root, temp_isle_vector);

    sort(temp_isle_vector.begin(), temp_isle_vector.end(),
              [](Isle* a, Isle* b) {
                  return a->getName() < b->getName();
              });


    ofstream isle_output_file(filename);
    if (!isle_output_file.is_open())
    {
        cerr << "can not opened!" << endl;
        return;
    }

    for (const auto& isle : temp_isle_vector) {
        isle_output_file << isle->getName() << std::endl;
    }

    isle_output_file.close();
    cout << "[Output] " << "Isles have been written to " << filename << " in in alphabetical order." << endl;
}