#ifndef CELESTIALOBJECT_H
#define CELESTIALOBJECT_H

#include <vector>
#include <bits/basic_string.h>
#include <bits/char_traits.h>
#include <bits/stringfwd.h>
#include <vector>
#include <iostream>
#include <string>
#include <sstream> // to_string() için
using namespace std;

// Enum to represent the type of the object (asteroid, life-up, or ammo)
enum ObjectType {
    ASTEROID = 0,
    LIFE_UP = 1,
    AMMO = 2,
    DEFAULT = 3
};

class CelestialObject {
public:

    // Constructor to initialize CelestialObject with essential properties
    CelestialObject(const vector<vector<bool>> &shape, ObjectType type, int start_row, int time_of_appearance);

    // Copy constructor for CelestialObject
    CelestialObject(const CelestialObject *other);

    // Shape of the object represented as a 2D boolean vector
    vector<vector<bool>> shape;

    // Pointer to the object's clockwise neighbor (its right rotation)
    CelestialObject *right_rotation = nullptr;

    // Pointer to the object's clockwise neighbor (its left rotation)
    CelestialObject *left_rotation = nullptr;

    // Pointer to the next celestial object in the list
    CelestialObject *next_celestial_object = nullptr;

    // Type of the object (asteroid, life-up, or ammo)
    ObjectType object_type;

    // Row in the space grid where the object's top will appear
    int starting_row;

    // The step in the game after which the object will appear on the grid
    int time_of_appearance;

    // Function to delete rotations of a given celestial object. It should free the dynamically allocated
    // space for each rotation.
    static void delete_rotations(CelestialObject *target);

    vector<vector<bool>> rotate_left(const vector<vector<bool>> & shape);

    void hit_and_rotate(int hit_row);

    std::string to_string() const;

    vector<vector<bool>> rotate_right(const vector<vector<bool>> &shape);

    int count_unique_rotations(const vector<vector<bool>> & shape);

    void set_up_rotations();
};

#endif // CELESTIALOBJECT_H
