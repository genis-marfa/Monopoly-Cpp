#include <stdio.h>
#include <vector>
#include <string>
#include <iostream>

#ifndef player_hpp
#define player_hpp

// Forward declare property class so player class can use pointers to this object.
class property;

// -------------------------------------------------------------------------
//                          CLASS PLAYER:
// -------------------------------------------------------------------------

// This class defines an object for the different players in the game.
// Each player HAS:
//      - A certain amount of money at each turn (current balance).
//      - A certain board position at each turn (current position).
//      - A name (player name) whose first two letters (player abreviation)
//           will be used to display the player's position in the board.
//      - A certain amount of properties (vector of properties).
//      - A certain number of get out of jail cards.
//      - A certain number of railroads / utilities owned, which will determine
//             the rent price.

// Each player CAN:
//      - Roll a dice.
//      - Be placed somewhere in the board.
//      - Be sent to / released from jail.
//      - Pay / Recieve a certain amount of money.
//      - Go bankrupt.

class player
{
private:
    // Number from 0-39 with the current position within the board:
    int current_pos{0};
    
    std::string player_name{""};
    std::string player_abreviation{""}; // Take first two characters from player name;
    
    // Current Balance:
    int player_balance{1500};
    
    // Additional features to play the game:
    int get_out_of_jail_cards{0};
    
    // These will determine the rent price:
    int railroads_owned{0};
    int utilities_owned{0};
    
    // Each player has 3 attempts while in jail to roll doubles to get freed.
    int turns_passed_in_jail{0};
    
public:
    // Static vector of strings holding players in jail, shared between all player types.
    // Made public for access in board class.
    static std::vector<std::string> players_in_jail;
    
    // Additional data made private for manipulation outside the class:
    std::vector<std::shared_ptr<property>> properties_owned; // Vector of properties owned.
    bool bankrupt{false};                                    // Bankrupcy status.
    bool in_jail{false};                                     // Jail status.
    
    player() {};
    
    // Parameterised constructor:  Automatically computes
    //    the player abreviation (first two chars in player_name).
    player(std::string play_name)
    {
        player_name = play_name;
        player_abreviation = play_name.erase(2,play_name.size())+" ";
    }
    
    //       POSITION MANIPULATOR FUNCTIONS:
    //     ----------------------------------
    // Function to change a player's position from outside the class.
    //      Called after each turn the player rolls the dice.
    void update_postion (int new_pos) {current_pos = new_pos;}
    
    // Set's the current player's position to some other position in the board.
    void place (std::vector<std::string> & player_pos, int pos);
    
    //        MONEY MANIPULATOR FUNCTIONS:
    //     ----------------------------------
    // Pay a certain amount of money.
    void pay(size_t amount)
    {
            std::cout << player_name << " paid $" << amount << "." << std::endl;
            player_balance -= amount;
    }
    
    // Recieve a certain amount of money:
    void receive(size_t amount)
    {
        std::cout << player_name << " received $" << amount << "." << std::endl;
        player_balance += amount;
    }
    
    //      RailRoad / Utility MANIPULATOR FUNCTIONS:
    //     ------------------------------------
    // Increment the counter for railroads/utilities held by the player.
    // These functions will get called whenever a player acquires a rr/utility.
    void add_new_railroad()
    {
        railroads_owned += 1;
        std::cout << "You now own " << railroads_owned << " railroad(s)." << std::endl;
    }
    void add_new_utility()
    {
        utilities_owned += 1;
        std::cout << "You now own " << utilities_owned << " utility / utilities." << std::endl;
    }
    
    //         JAIL FUNCTIONS:
    //      --------------------
    // If a player draws a get_out_of_jail card from chance/community chest, their counter increases.
    void add_get_out_of_jail_card()
    {
        get_out_of_jail_cards += 1;
        std::cout << player_name << ", you now have " << get_out_of_jail_cards << " get out of jail cards." << std::endl;
    }
    
    // If a player uses up a get_out_of_jail_card, their counter decreases.
    void loose_get_out_of_jail_card()
    {
        get_out_of_jail_cards -= 1;
        std::cout << player_name << ", you now have " << get_out_of_jail_cards << " get out of jail cards." << std::endl;
    }
    
    // Sends the player to jail. Modifies static variable "players_in_jail" to display on board.
    void send_to_jail();
    
    // Free's the player from jail. Place the player on "Just visiting" (tile #10).
    void free_from_jail(std::vector<std::string> & player_pos);
    
    // In jail mechanic: This method will be triggered at the start of every turn while the player is in jail.
    void in_jail_mechnic(std::vector<std::shared_ptr<player>> & players, std::vector<std::string> & player_pos);
    
    // DICE ROLL FUNCTION:
    // If a player rolls doubles, roll dice again. If player rolls doubles three times, he gets sent to jail.
    int dice_roll();
    
    // BANKRUPCY:
    // Declare bankrupcy and:
    //  - Deletes player abbreviation (token) from board.
    //  - Deletes player from vector of players.
    //  - Deletes the pointer to this player.
    //  - Checks if the game is finished. (Only one player left).
    void go_bankrupt(std::vector<std::shared_ptr<player>> & players, std::vector<std::string> & player_pos);
    
    // Return private data for out-of-class display purposes:
    int current_position()  {return current_pos;}                     // Position within the board.
    int current_balance()   {return player_balance;}                  // Current balance.
    int railroads_owned_by_player()   {return railroads_owned;}       // Number of rr owned.
    int utilities_owned_by_player()   {return utilities_owned;}       // Number of utilities owned.
    size_t num_props_owned () {return properties_owned.size();}       // Number of (overall) properties owned.
    std::string plyr_name() {return player_name;}                     // Player name string.
    std::string plyr_abrev() {return player_abreviation;}             // Player abbreviatin string.
    int num_get_out_of_jail_cards() {return get_out_of_jail_cards;}   // Number of get of out jail cards.
    
};

#endif /* player_hpp */
