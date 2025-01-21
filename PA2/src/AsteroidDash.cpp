#include "AsteroidDash.h"



// Constructor to initialize AsteroidDash with the given parameters
AsteroidDash::AsteroidDash(const string &space_grid_file_name,
                           const string &celestial_objects_file_name,
                           const string &leaderboard_file_name,
                           const string &player_file_name,
                           const string &player_name)

        : leaderboard_file_name(leaderboard_file_name), leaderboard(Leaderboard()) {

    read_player(player_file_name, player_name);  // Initialize player using the player.dat file
    read_space_grid(space_grid_file_name);  // Initialize the grid after the player is loaded
    read_celestial_objects(celestial_objects_file_name);  // Load celestial objects
    leaderboard.read_from_file(leaderboard_file_name);
}

// Function to read the space grid from a file
void AsteroidDash::read_space_grid(const string &input_file) {
    // TODO: Your code here
    ifstream infile(input_file);
    if (!infile) {
        cout << "There is a problem in input file in space grid" << endl;
        exit(1);
    }
    int rows = 1;
    int cols = 0;
    string temp;

    if (getline (infile, temp)) {
        istringstream iss(temp);
        int grid;
        while(iss >> grid) {
            cols++;
        }
    }
    while (getline(infile, temp)) {
        rows++;
    }

    infile.clear();
    infile.seekg(0, ios::beg);

    space_grid.resize(rows, vector<int>(cols));

    for (int row_ind = 0; row_ind < rows; ++row_ind) {
        string basic_string;
        if (getline(infile, basic_string)) {
            istringstream iss(basic_string);
            for (int col_ind = 0; col_ind < cols; ++col_ind) {
                int grid = 0;
                if (iss >> grid) {
                    space_grid[row_ind][col_ind] = grid;
                }
            }
        }
    }

    infile.close();

}

// Function to read the player from a file
void AsteroidDash::read_player(const string &player_file_name, const string &player_name) {
    // TODO: Your code here
    int start_row = 0;
    int start_col = 0;
    vector<vector<bool>> spacecraft;

    ifstream infile(player_file_name);
    if (!infile) {
        cout << "There is a problem in input file in read player" << endl;
        exit(1);
    }
    string temp;
    if (getline (infile, temp)) {
        istringstream iss(temp);
        iss >> start_row >> start_col;
    }
    while (getline(infile, temp)) {
        vector<bool> row;
        istringstream iss(temp);
        int grid;
        while(iss >> grid) {
            row.push_back(static_cast<bool>(grid));;
        }
        spacecraft.push_back(row);
    }

    player = new Player(spacecraft, start_row, start_col, player_name);

    infile.close();

}

// Function to read celestial objects from a file
void AsteroidDash::read_celestial_objects(const string &input_file) {
    // TODO: Your code here
    ifstream infile(input_file);
    if (!infile) {
        cout << "There is a problem in input file in read celestial" << endl;
        return;
    }
    string temp_line;
    vector<vector<bool>> shape;
    ObjectType type = ASTEROID;
    int tick = 0;
    int start_row = 0;

    while(getline(infile, temp_line)) {
        if (temp_line.empty()) {
            //shape.begin() != shape.end()
            if(!shape.empty()) {
                CelestialObject *celestial_object = new CelestialObject(shape, type, start_row, tick);
                if (celestial_objects_list_head == nullptr) {
                    celestial_objects_list_head = celestial_object;
                }
                else {
                    CelestialObject *dummy_node = celestial_objects_list_head;
                    while (dummy_node->next_celestial_object != nullptr) {
                        dummy_node = dummy_node->next_celestial_object;
                    }
                    dummy_node -> next_celestial_object = celestial_object;
                }
                shape.clear();
                type = ASTEROID;
            }
            continue;
        }
        if (!temp_line.empty()) {
            switch (temp_line[0]) {
                case '[': {
                    while (temp_line[temp_line.size()-1] != ']') {
                        vector<bool> temp_row;
                        for (auto it : temp_line) {
                            if (it == '1') temp_row.push_back(true);
                            else if (it == '0') temp_row.push_back(false);
                        }
                        shape.push_back(temp_row);
                        getline(infile, temp_line);
                    }

                    // last line
                    vector<bool> temp_row;
                    for (auto it : temp_line) {
                        if (it == '1') temp_row.push_back(true);
                        else if (it == '0') temp_row.push_back(false);
                    }
                    shape.push_back(temp_row);
                    break;
                }
                case '{': {
                    while (temp_line.back() != '}') {
                        vector<bool> temp_row;
                        for (auto it : temp_line) {
                            if (it == '1') temp_row.push_back(true);
                            else if (it == '0') temp_row.push_back(false);
                        }
                        shape.push_back(temp_row);
                        getline(infile, temp_line);
                    }

                    // last line
                    vector<bool> temp_row;
                    for (auto it : temp_line) {
                        if (it == '1') temp_row.push_back(true);
                        else if (it == '0') temp_row.push_back(false);
                    }
                    shape.push_back(temp_row);
                    break;
                }

                case 's': {
                    start_row = stoi(temp_line.substr(2));
                    break;
                }
                case 'e': {
                    string type_info = temp_line.substr(2);
                    if (type_info == "life"){type = LIFE_UP;}
                    if (type_info == "ammo"){type = AMMO;}
                    break;
                }
                case 't': {
                    tick = stoi(temp_line.substr(2));
                    break;
                }
                default:
                    break;
            }
        }
    }
    //last object
    //shape.begin() != shape.end()
    if(!shape.empty()) {
        CelestialObject *celestial_object = new CelestialObject(shape, type, start_row, tick);
        if (celestial_objects_list_head == nullptr) {
            celestial_objects_list_head = celestial_object;
        }
        else {
            CelestialObject *dummy_node = celestial_objects_list_head;
            while (dummy_node->next_celestial_object != nullptr) {
                dummy_node = dummy_node->next_celestial_object;
            }
            dummy_node -> next_celestial_object = celestial_object;
        }

    }

    CelestialObject *current = celestial_objects_list_head;
    while (current != nullptr) {
        current->set_up_rotations();
        current = current->next_celestial_object;
    }

    infile.close();

}


// Print the entire space grid
void AsteroidDash::print_space_grid() const {
    // TODO: Your code here
    for (int row = 0; row < space_grid.size(); row++ ) {
        for (int col = 0; col < space_grid[row].size(); col++) {
            if (space_grid[row][col] == 1) {
                cout << occupiedCellChar;
            }
            else {
                cout << unoccupiedCellChar;
            }
        }
        cout << endl;
    }
}
// Function to update the space grid with player, celestial objects, and any other changes
// It is called in every game tick before moving on to the next tick.
void AsteroidDash::update_space_grid() {

    // Clear the space grid
    for (int row = 0; row < space_grid.size(); row++) {
        for (int col = 0; col < space_grid[row].size(); col++) {
            space_grid[row][col] = 0;
        }
    }

    // Player
    const auto player_rsize = player->spacecraft_shape.size();
    const auto player_csize = player->spacecraft_shape[0].size();
    for (int row = 0; row < player_rsize; row++) {
        for (int col = 0; col < player_csize; col++) {
            if (player->spacecraft_shape[row][col]) {
                space_grid[player->position_row + row][player->position_col + col] = 1;
            }
        }
    }

    // Update Active Objects
    active_objects.clear();
    CelestialObject* current = celestial_objects_list_head;
    while (current != nullptr) {
        int current_col = space_grid[0].size() - 1 - (game_time - current->time_of_appearance);

        if (game_time >= current->time_of_appearance && current_col + current->shape[0].size() > 0) {
            active_objects.push_back(current);
        }
        current = current->next_celestial_object;
    }

     // Check for collisions between player and active objects
    for (int i = 0; i < active_objects.size();) {
        CelestialObject* active_celestial = active_objects[i];
        int current_col = space_grid[0].size() - 1 - (game_time - active_celestial->time_of_appearance);
        bool collision_detected = false;

        for (int row = 0; row < active_celestial->shape.size(); row++) {
            for (int col = 0; col < active_celestial->shape[row].size(); col++) {
                if (active_celestial->shape[row][col]) { // Objede aktif bir hücre
                    int grid_row = active_celestial->starting_row + row;
                    int grid_col = current_col + col;

                    if (grid_row >= player->position_row &&
                        grid_row < player->position_row + player_rsize &&
                        grid_col >= player->position_col &&
                        grid_col < player->position_col + player_csize) {

                        int player_row = grid_row - player->position_row;
                        int player_col = grid_col - player->position_col;

                        if (player->spacecraft_shape[player_row][player_col]) {
                            collision_detected = true;
                            break;
                        }
                    }
                }
            }
            if (collision_detected) break;
        }

        if (collision_detected) {

            if (active_celestial->object_type == ASTEROID) {
                active_celestial->object_type = DEFAULT;
                // ASTEROID: Can azalt
                if (player->lives > 1) {
                    player->lives -= 1;
                } else {
                    game_over = true;
                    player->lives = 0;
                }
            } else if (active_celestial->object_type == LIFE_UP) {
                active_celestial->object_type = DEFAULT;
                if (player->lives < 100) {
                    player->lives += 1;
                }
            } else if (active_celestial->object_type == AMMO) {
                active_celestial->object_type = DEFAULT;
                player->current_ammo = player->max_ammo;
            }
            active_objects.erase(active_objects.begin() + i);
        } else {
            ++i;
        }
    }



    for (CelestialObject* active_celestial : active_objects) {
        int current_col = space_grid[0].size() - 1 - (game_time - active_celestial->time_of_appearance);
        for (int row = 0; row < active_celestial->shape.size(); row++) {
            for (int col = 0; col < active_celestial->shape[row].size(); col++) {
                if (active_celestial->shape[row][col]) {
                    int grid_row = active_celestial->starting_row + row;
                    int grid_col = current_col + col;
                    if (grid_row >= 0 && grid_row < space_grid.size() && grid_col >= 0 && grid_col < space_grid[0].size()) {
                        space_grid[grid_row][grid_col] = 1;
                    }
                }
            }
        }
    }

    for (auto& projectile : allpro_arr) {
        int projectile_row = projectile[0];
        int projectile_col = projectile[1];
        if (projectile_row >= 0 && projectile_row < space_grid.size() && projectile_col >= 0 && projectile_col < space_grid[0].size()) {
            space_grid[projectile_row][projectile_col] = 1;
        }
    }
    check_collisions();
    move_projectiles();
}
// Corresponds to the SHOOT command.
// It should shoot if the player has enough ammo.
// It should decrease the player's ammo
void AsteroidDash::shoot() {
    // TODO: Your code here
    if (player->current_ammo > 0) {
        player->current_ammo--;
        int p_new_row = player->position_row + player->spacecraft_shape.size() / 2 ;
        int p_new_col = player->position_col + player->spacecraft_shape[0].size();
        allpro_arr.push_back({p_new_row, p_new_col});
    }
}
void AsteroidDash::check_collisions() {

    for (int i = 0; i < allpro_arr.size();) {
        int projectile_row = allpro_arr[i][0];
        int projectile_col = allpro_arr[i][1];
        bool collision_detected = false;


        for (CelestialObject* obj : active_objects) {
            if (obj->object_type != ASTEROID) {
                continue;
            }

            int current_col = space_grid[0].size() - 1 - (game_time - obj->time_of_appearance);
            int start_row = obj->starting_row;
            int end_row = start_row + obj->shape.size() - 1;
            int start_col = current_col;
            int end_col = current_col + obj->shape[0].size() - 1;
            if (projectile_row >= start_row && projectile_row <= end_row &&
                projectile_col >= start_col && projectile_col <= end_col) {

                int gridrow = projectile_row - start_row;
                int gridcol = projectile_col - start_col;

                if (gridrow >= 0 && gridrow < obj->shape.size() &&
                    gridcol >= 0 && gridcol < obj->shape[gridrow].size() &&
                    obj->shape[gridrow][gridcol] == true) {

                    projectile_collision(projectile_row, projectile_col, obj);
                    allpro_arr.erase(allpro_arr.begin() + i);
                    collision_detected = true;

                    break;
                }
            }
        }

        if (!collision_detected) {
            ++i;
        }
    }
}


void AsteroidDash::move_projectiles() {

        int i = 0;
        while (i < allpro_arr.size()) {
            int projectile_col = allpro_arr[i][1];

            projectile_col++;

            if (projectile_col >= space_grid[0].size()) {
                allpro_arr.erase(allpro_arr.begin() + i);
            }
            else{
                    allpro_arr[i][1] = projectile_col;
                    ++i;
                }

        }

}
void AsteroidDash::projectile_collision(int projectile_row, int projectile_col, CelestialObject* celobj) {
    if (celobj->object_type != ASTEROID) {
        return;
    }

    int current_col = space_grid[0].size() - 1 - (game_time - celobj->time_of_appearance);
    int gridrow = projectile_row - celobj->starting_row;
    int gridcol = projectile_col - current_col;

    if (gridrow >= 0 && gridrow < celobj->shape.size() &&
        gridcol >= 0 && gridcol < celobj->shape[gridrow].size() &&
        celobj->shape[gridrow][gridcol] == true) {

        celobj->shape[gridrow][gridcol] = false; // Hücreyi yok et

        celobj->delete_rotations(celobj);
        celobj->set_up_rotations();


        int c_row = celobj->starting_row + celobj->shape.size() / 2;

        CelestialObject* speci_rot = nullptr;

        if (projectile_row < c_row) {
            speci_rot = celobj->right_rotation;
        } else if (projectile_row > c_row) {
            speci_rot = celobj->left_rotation;
        }

        if (speci_rot) {
            for (size_t i = 0; i < active_objects.size(); ++i) {
                if (active_objects[i] == celobj) {
                    active_objects[i] = speci_rot;
                    break;
                }
            }
        }

        if (isDestroyed(celobj)) {
            current_score += 100;
        }
    } else {
    }

    current_score += 10;
}



CelestialObject* AsteroidDash::find_celestial_object_at(int row, int col) {
    CelestialObject* curr_node = celestial_objects_list_head;

    while (curr_node != nullptr) {
        const auto celestial_rows_size = curr_node->shape.size();
        const auto celestial_cols_size = curr_node->shape[0].size();
        int current_col = space_grid[0].size() - 1 - (game_time - curr_node->time_of_appearance);

        for (int r = 0; r < celestial_rows_size; r++) {
            for (int c = 0; c < celestial_cols_size; c++) {
                if (curr_node->shape[r][c] == true) {
                    int object_row = curr_node->starting_row + r;
                    int object_col = current_col + c;

                    if (object_row == row && object_col == col) {
                        return curr_node;

                    }
                }
            }
        }

        curr_node = curr_node->next_celestial_object;
    }

    return nullptr;
}


bool AsteroidDash::isDestroyed(CelestialObject *target) {
    for (int i = 0; i < target->shape.size(); i++) {
        for (int j = 0; j < target->shape[0].size(); j++) {
            if (target->shape[i][j]) {
                return false;
            }
        }
    }
    return true;
}

// Destructor. Remove dynamically allocated member variables here.
AsteroidDash::~AsteroidDash() {
    // TODO: Your code here

    CelestialObject* curr_node = celestial_objects_list_head;
    while (curr_node != nullptr) {
        CelestialObject* next_node = curr_node->next_celestial_object;
        CelestialObject::delete_rotations(curr_node);
        delete curr_node;
        curr_node = next_node;
    }
    delete player;
}
