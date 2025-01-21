#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include "CelestialObject.h"


// Constructor to initialize CelestialObject with essential properties
CelestialObject::CelestialObject(const vector<vector<bool>> &shape, ObjectType type, int start_row,
                                 int time_of_appearance)
        : shape(shape), object_type(type), starting_row(start_row), time_of_appearance(time_of_appearance) {
    // TODO: Your code here
    left_rotation = nullptr;
    right_rotation = nullptr;
}

// Copy constructor for CelestialObject
CelestialObject::CelestialObject(const CelestialObject *other)
        : shape(other->shape),  // Copy the 2D vector shape
          object_type(other->object_type),  // Copy the object type
          starting_row(other->starting_row),  // Copy the starting row
          time_of_appearance(other->time_of_appearance)  // Copy the time of appearance
{
    // TODO: Your code here
    left_rotation = nullptr;
    right_rotation = nullptr;
    set_up_rotations();
}

// Function to delete rotations of a given celestial object. It should free the dynamically allocated
// memory for each rotation.
void CelestialObject::delete_rotations(CelestialObject *target) {
    // TODO: Your code here
    if (target == nullptr) {
        return;
    }

    CelestialObject *next_node = target->right_rotation;

    while (next_node && next_node != target) {
        CelestialObject *temp_node = next_node->right_rotation;
        delete next_node;
        next_node = temp_node;
    }

    target->right_rotation = nullptr;
    target->left_rotation = nullptr;
}

vector<vector<bool>> CelestialObject::rotate_right(const vector<vector<bool>> &shape) {
    vector<vector<bool>> rota_shape(shape[0].size(), vector<bool>(shape.size(), false));

    for (int i = 0; i < shape.size(); i++) {
        for (int j = 0; j < shape[0].size(); j++) {
            rota_shape[j][shape.size() - 1 - i] = shape[i][j];
        }
    }
    return rota_shape;
}
vector<vector<bool>> CelestialObject::rotate_left(const vector<vector<bool>> &shape) {
    vector<vector<bool>> rota_shape(shape[0].size(), vector<bool>(shape.size(), false));

    for (int i = 0; i < shape.size(); i++) {
        for (int j = 0; j < shape[0].size(); j++) {
            rota_shape[shape[0].size() - 1 - j][i] = shape[i][j];
        }
    }
    return rota_shape;
}

void CelestialObject::set_up_rotations() {
    int unique_rotations = count_unique_rotations(shape);

    if (unique_rotations == 1) {
        this->right_rotation = this;
        this->left_rotation = this;
        this ->next_celestial_object = this ->next_celestial_object;
    }
    else if (unique_rotations == 2) {
        vector<vector<bool>> right_shape = rotate_right(shape);
        CelestialObject* r_rota = new CelestialObject(right_shape, object_type, starting_row, time_of_appearance);

        this->right_rotation = r_rota;
        r_rota->left_rotation = this;
        r_rota->right_rotation = this;
        this->left_rotation = r_rota;

        this->next_celestial_object = this ->next_celestial_object;
        r_rota->next_celestial_object = this->next_celestial_object;

    }
    else if (unique_rotations == 4) {
        vector<vector<bool>> first_shape = rotate_right(shape);
        CelestialObject* f_Rota = new CelestialObject(first_shape, object_type, starting_row, time_of_appearance);

        vector<vector<bool>> s_Rota = rotate_right(first_shape);
        CelestialObject* second_rot = new CelestialObject(s_Rota, object_type, starting_row, time_of_appearance);

        vector<vector<bool>> third_shape = rotate_right(s_Rota);
        CelestialObject* third_rot = new CelestialObject(third_shape, object_type, starting_row, time_of_appearance);

        this->right_rotation = f_Rota;
        f_Rota->left_rotation = this;
        f_Rota->right_rotation = second_rot;
        second_rot->left_rotation = f_Rota;
        second_rot->right_rotation = third_rot;
        third_rot->left_rotation = second_rot;
        third_rot->right_rotation = this;
        this->left_rotation = third_rot;


        this ->next_celestial_object = this ->next_celestial_object;
        f_Rota->next_celestial_object = this->next_celestial_object;
        second_rot->next_celestial_object = this->next_celestial_object;
        third_rot->next_celestial_object = this->next_celestial_object;


    }
}


int CelestialObject::count_unique_rotations(const vector<vector<bool>> &shape) {
    vector<vector<bool>> rota_shape = shape;
    int temp_count = 1;

    for (int i = 0; i < 3; i++) {
        rota_shape = rotate_right(rota_shape);
        if (rota_shape == shape) {
            break;
        }
        temp_count++;
    }
    return temp_count;
}
