#include <stdio.h>
#include "property_group.hpp"
#include "street.hpp"
#include "railroad.hpp"
#include "utility.hpp"

// -------------------------------------------------------------------------
//                          CLASS BOARD:
// -------------------------------------------------------------------------
//  The Monopoly game-board typically consists of 40 spaces containing:
//     -28 properties: 22 streets (grouped into eight color groups), 4 railroads, and 2 utilities.
//     - 3 chance spaces, 3 Community Chest spaces.
//     - A Luxury Tax space, an Income Tax space.
//     - 4 corner squares: GO, Just Visiting, Free Parking, and Go to Jail.

//  Each board has:
//    - A size: 11x11 in standard monopoly.
//    - A tile_width and a tile_height (in units of blank spaces).
//    - A vector of (pointers to) properties, from which the names and costs must be extracted to display on board.
//    - A vector of strings with the properties' first line, one for the second lines and
//                      and one containing the purchase_cost.

// The only public method is a function to print the board's status.

#ifndef board_hpp
#define board_hpp

class board
{
private:
    int size;
    int tile_width;
    int tile_height;
    std::vector<std::string> first_line;
    std::vector<std::string> second_line;
    std::vector<std::string> purchase_costs;

    // A few private functions to help constructing the board:
    
    // Print as a line of all '-' characters. Used to separate tiles in the first and last row.
    void print_line();

    // Prints tile_width '-' characters in the first and last column, and white space in between.
    void print_spaced_line();
    
    // Prints the first row of tiles in the board.
    void first_row(std::vector<std::string> first_line, std::vector<std::string> second_line,
                   std::vector<std::string> purchase_costs, std::vector<std::string> player_pos);
    // Prints last row of tiles in the board:
    void last_row(std::vector<std::string> first_line, std::vector<std::string> second_line,
                  std::vector<std::string> purchase_costs, std::vector<std::string> player_pos);
    
    // Print all the middle rows:
    void middle_rows(std::vector<std::string> first_line, std::vector<std::string> second_line,
                     std::vector<std::string> purchase_costs, std::vector<std::string> player_pos, std::vector<std::shared_ptr<player>> & players);

public:
    static std::string current_player;
    static int jackpot_money;
    
    // Extract first line, second line and purchase costs from the properties.
    board(int size_, int width, int height, std::vector<std::shared_ptr<property>> & props);
    
    // Display the board's current status.
    void print_board(std::vector<std::string> player_positions, std::vector<std::shared_ptr<player>> & players);
    
};

#endif /* board_hpp */
