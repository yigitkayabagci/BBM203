#include "Player.h"

#include <iostream>
#include <ostream>

// Constructor to initialize the player's spacecraft, position, and ammo
Player::Player(const vector<vector<bool>> &shape, int row, int col, const string &player_name, int max_ammo, int lives)
        : spacecraft_shape(shape), position_row(row), position_col(col), player_name(player_name), max_ammo(max_ammo),
          current_ammo(max_ammo), lives(lives) {
    // TODO: Your code here, if you want to do further initializations

}

// Move player left within the grid boundaries
void Player::move_left() {
    // TODO: Your code here
    if (position_col > 0) {
        position_col--;
    }
}

// Move player right within the grid boundaries
void Player::move_right(int grid_width) {
    // TODO: Your code here
    int spacecraft_width = spacecraft_shape[0].size();

    if (position_col + spacecraft_width < grid_width) {
        position_col++;
    }
}

// Move player up within the grid boundaries
void Player::move_up() {
    // TODO: Your code here
    if (position_row > 0) {
        position_row-=1;
    }
}

// Move player down within the grid boundaries
void Player::move_down(int grid_height) {
    // TODO: Your code here
    if (position_row + spacecraft_shape.size() < grid_height) {
        position_row++;
    }
}