#include "GameController.h"

// Simply instantiates the game
GameController::GameController(
        const string &space_grid_file_name,
        const string &celestial_objects_file_name,
        const string &leaderboard_file_name,
        const string &player_file_name,
        const string &player_name

) {
    game = new AsteroidDash(space_grid_file_name, celestial_objects_file_name, leaderboard_file_name, player_file_name,
                            player_name);
    // TODO: Your code here, if you want to perform extra initializations
}

// Reads commands from the given input file, executes each command in a game tick
void GameController::play(const string &commands_file) {
    // TODO: Your code here
    ifstream infile(commands_file);
    string temp_line;
    while (getline(infile, temp_line)) {
        cout << temp_line << endl;
        if (temp_line == "MOVE_LEFT") {
            game->player->move_left();
        } else if (temp_line == "MOVE_RIGHT") {
            game->player->move_right(game->space_grid[0].size());
        } else if (temp_line == "MOVE_UP") {
            game->player->move_up();
        } else if (temp_line == "MOVE_DOWN") {
            game->player->move_down(game->space_grid.size());
        } else if (temp_line == "SHOOT") {
            game->shoot();
        }
        else if (temp_line == "PRINT_GRID") {
            game->print_space_grid();
        }
        else if (temp_line == "NOP") {;
            game->print_space_grid();
            game->game_time++;
        }
        else {
            cerr << "Unknown command: " << temp_line << endl;
        }
        //game->update_space_grid();

    }
}

// Destructor to delete dynamically allocated member variables here
GameController::~GameController() {
    // TODO: Your code here
    delete game;
}
