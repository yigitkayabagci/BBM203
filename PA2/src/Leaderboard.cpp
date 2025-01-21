#include "Leaderboard.h"

#include <fstream>
#include <iostream>
#include <sstream>

// Read the stored leaderboard status from the given file such that the "head_leaderboard_entry" member
// variable will point to the highest all-times score, and all other scores will be reachable from it
// via the "next_leaderboard_entry" member variable pointer.
void Leaderboard::read_from_file(const string &filename) {
    // TODO: Your code here
    ifstream infile(filename);
    string temp_line;
    if (!infile) {
        cout << "There is a problem in input file" << endl;
        return;
    }
    while (getline(infile, temp_line)) {
        istringstream iss(temp_line);
        string p_name;
        unsigned long score;
        time_t time;
        iss >> score >> time >> p_name;
        LeaderboardEntry *leader = new LeaderboardEntry(score, time, p_name);
        insert(leader);
    }
    infile.close();
}


// Write the latest leaderboard status to the given file in the format specified in the PA instructions
void Leaderboard::write_to_file(const string &filename) {
    // TODO: Your code here
    ofstream outfile(filename);
    if (!outfile) {
        cout << "There is a problem in output file" << endl;
        return;
    }
    LeaderboardEntry *temp = head_leaderboard_entry;
    while (temp != nullptr) {
        outfile << temp->score << " " << temp->last_played << " " << temp->player_name << endl;
        temp = temp->next;
    }
    outfile.close();
}

// Print the current leaderboard status to the standard output in the format specified in the PA instructions
void Leaderboard::print_leaderboard() {
    // TODO: Your code here
    cout << "Leaderboard" << endl;
    cout << "-----------"<< endl;
    int order  = 1;
    LeaderboardEntry *curr = head_leaderboard_entry;
    while (curr != nullptr) {
        tm *time_info = localtime(&curr->last_played);
        char time_buffer[20];
        strftime(time_buffer, sizeof(time_buffer), "%H:%M:%S/%d.%m.%Y", time_info);
        cout << order << ". " << curr->player_name << " " << curr->score << " " << time_buffer << endl;
        curr = curr->next;
        order++;
    }
}

//  Insert a new LeaderboardEntry instance into the leaderboard, such that the order of the high-scores
//  is maintained, and the leaderboard size does not exceed 10 entries at any given time (only the
//  top 10 all-time high-scores should be kept in descending order by the score).
void Leaderboard::insert(LeaderboardEntry *new_entry) {
    // TODO: Your code here
    if (head_leaderboard_entry == nullptr) {
        head_leaderboard_entry = new_entry;
    }
    else {
        LeaderboardEntry *current = head_leaderboard_entry;
        LeaderboardEntry *prev = nullptr;

        while (current != nullptr && current->score >= new_entry->score) {
            prev = current;
            current = current->next;
        }
        if (prev == nullptr) {
            new_entry->next = head_leaderboard_entry;
            head_leaderboard_entry = new_entry;
        } else {
            prev->next = new_entry;
            new_entry->next = current;
        }
    }

    int count = 1;
    LeaderboardEntry *tail = head_leaderboard_entry;
    while (tail != nullptr && count < MAX_LEADERBOARD_SIZE) {
        tail = tail->next;
        count++;
    }
    if (tail != nullptr) {
        LeaderboardEntry *next = tail->next;
        tail->next = nullptr;

        while (next != nullptr) {
            LeaderboardEntry *dummy = next;
            next = next->next;
            delete dummy;
        }
    }
}

// Free dynamically allocated memory used for storing leaderboard entries
Leaderboard::~Leaderboard() {
    // TODO: Your code here
    LeaderboardEntry *curr = head_leaderboard_entry;
    while (curr != nullptr) {
        LeaderboardEntry *dummy = curr;
        curr = curr->next;
        delete dummy;
    }
    head_leaderboard_entry = nullptr;
}
