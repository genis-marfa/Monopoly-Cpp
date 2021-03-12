#include "board.hpp"

// Initialise static member data:
std::string board::current_player = "";
int board::jackpot_money = 100;

// We must first compute the total number of characters that will go into each line of the board:
// Each line will contain:
//      size*(tile_width)  characters for each tile.
//      + size             charcaters for each tile edge.
//      + 1                extra character for the first/last edge.
// In total, each line of the board will be size*(tile_width+1)+1 characters long.
// For example, in a standard monopoly board of size 11, with a tile_width = 12,
//      each line is 11*(13)+1 = 144 characters long.

void board::print_line()
{
    for (int i{}; i<size*(tile_width+1)+1; i++) std::cout << '-';
    std::cout << '\n';
}

void board::print_spaced_line()
{
    for (int i{}; i<= tile_width+1; i++) std::cout << '-';
    for (int i{}; i<(size-2)*tile_width+(size-3); i++) std::cout << " ";
    for (int i{}; i<= tile_width+1; i++) std::cout << "-";
    std::cout << '\n';
}

// To print rows, must take in a vector of strings with the position of the player abreviations (player_pos).
// For example:
//    If all the players are at the go tile, the vector will look like:
//          {"P1 P2 P3 P4", (+39 empty strings) }.
//    If P1 then rolls a 2, the the vector will look like:
//          {"P2 P3 P4", "", "P1", (+37 empty stings)}

void board::first_row(std::vector<std::string> first_line, std::vector<std::string> second_line,
               std::vector<std::string> purchase_costs, std::vector<std::string> player_pos)
{
    print_line(); // Start by printing out a line of constant '-'.
    
    // Start constructing the tile. Iterate Over tile height:
    for (int i{}; i<=tile_height; i++) {
        std::cout << '|'; // Put a delimiter at the start of the tile (edge)
        for (int j{}; j < size; j++) {
            // i counts the height within the tile (runs from 0 -> tile_height).
            // j counts the tile number within the row. (runs from 0 -> (size-1)).
            
            // Structure the tile as follows:
            //  - The 0th line contains the first string of the property's name.
            //  - The 1st line contains the second string of the property's name.
            //  - The 3rd line will display the players standing on that tile.
            //  - The last line contains the property's cost:
            
            // The first row in the board starts with runs from tile 20-30. (i.e. from free parking to go to jail).
            if (i==0) std::cout << first_line[j+20];
            else if (i==1) std::cout << second_line[j+20];
            else if (i==3) std::cout << player_pos[j+20];
            else if (i==tile_height) std::cout << purchase_costs[j+20];
            else {
                for (int i{}; i< tile_width; i++) std::cout << " ";
            }
             // Close the tile with an delimiter (edge).
            std::cout << '|';
        }
        std::cout << '\n';
    }
    print_line(); // Close the tile by printing out another line of constant '-'.
}

// In a similar fashion, print the last row:
void board::last_row(std::vector<std::string> first_line, std::vector<std::string> second_line, std::vector<std::string> purchase_costs, std::vector<std::string> player_pos)
{
    print_line();
    for (int i{}; i<=tile_height; i++) {
        std::cout << "|";
        for (int j{}; j < size; j++) {
            // Runs from tile 10 (Just Visiting) to 0 (Go tile) in decreasing order.
            if      (i==0) std::cout << first_line[(size-1)-j];
            else if (i==1) std::cout << second_line[(size-1)-j];
            else if (i==3) std::cout << player_pos[(size-1)-j];
            else if (i==tile_height) std::cout << purchase_costs[(size-1)-j];
            else {
                for (int i{}; i< tile_width; i++) std::cout << " ";
            }
            std::cout << "|";
        }
        std::cout << '\n';
    }
    print_line();
}

void board::middle_rows(std::vector<std::string> first_line, std::vector<std::string> second_line,
                           std::vector<std::string> purchase_costs, std::vector<std::string> player_pos, std::vector<std::shared_ptr<player>> & players)
{
    for (int j{1}; j<size-1; j++) {
        for (int i{}; i<=tile_height; i++) {
            // First column:
            // Tiles run from 19 (New york avenue) to 11 (St. Charles place) in decreasing order.
            
            // Now contruct tile:
            std::cout << "|";
            if (i==0) std::cout << first_line[2*(size-1)-j];
            else if (i==1) std::cout << second_line[2*(size-1)-j];
            else if (i==3) std::cout << player_pos[2*(size-1)-j];
            else if (i==tile_height) std::cout << purchase_costs[2*(size-1)-j];
                
            else {
                for (int i{}; i< tile_width; i++) std::cout << " ";
            }
            std::cout << "|";
                
            // Generate blank space at the center of the board:
            // Add additional information at the center of the board:
            //      - A Welcome to Monopoly statement.
            //      - A reminder of whose turn it is.
            //      - The players who are currently in jail.
            //      - Free parking jackpot amount.
            //      - Player balances.
            
            // Store strings with the above information in a vector. Make use of static data.
            
            // Recover jail data from static vector:
            std::string jail_slot_one = player::players_in_jail[0];
            std::string jail_slot_two = player::players_in_jail[1];
            std::string jail_slot_three = player::players_in_jail[2];
            std::string jail_slot_four = player::players_in_jail[3];
            std::string jail_slots = jail_slot_one + jail_slot_two + jail_slot_three + jail_slot_four;
            
            std::vector<std::string> middle_info {"WELCOME TO MONOPOLY", "-------------------",
            "PLAYER'S TURN: "+current_player, "PLAYERS IN JAIL: " + jail_slots,
            "FREE PARKING JACKPOT: "+std::to_string(jackpot_money)+"$" };
            
            // Must count how much white space we need at the center of the board:
            //  Each line is size*(tile_width+1)+1 characters long.
            //  Hence, we must subtract from the total the amount corresponding to two tiles.
            //          Each tile takes tile_width + 2 (for the two edges) characters.
            // In total, we need size*(tile_width+1)+1  - 2*(tile_width + 2) characters of white space.
            
            // Center strings to required white space:
            int white_space_size = size*(tile_width+1)+1 - 2*(tile_width+2);
            std::vector<std::string> middle_info_center = center_strings(middle_info, white_space_size);
            
            std::vector<std::string> player_balances(5);
            player_balances[0] = "PLAYER BALANCES:";
            for (unsigned i{}; i< players.size(); i++) {
                player_balances[i+1] = players[i]-> plyr_name() +"'s balance: " + std::to_string(players[i]->current_balance())+"$";
            }
            // Center strings to required white space:
            std::vector<std::string> player_balances_ctr = center_strings(player_balances, white_space_size);
            
            // Print all the information more or less at the center of the board:
            if  (i == 0 && j==3)
                std::cout << middle_info_center[0];
            else if (i==1 && j==3)
                std::cout << middle_info_center[1];
            else if (i == 3 && j==3)
                std::cout << middle_info_center[2];
            else if (i == 0 && j==5)
                std::cout << middle_info_center[3];
            else if (i == 2 && j==5)
                std::cout << middle_info_center[4];
            else if (i==0 && j==7)
                std::cout << player_balances_ctr[0];
            else if (i==1 && j==7)
                std::cout << player_balances_ctr[1];
            else if (i==2 && j==7)
                std::cout << player_balances_ctr[2];
            else if (i==3 && j==7)
                std::cout << player_balances_ctr[3];
            else if (i==4 && j==7)
                std::cout << player_balances_ctr[4];
            else {
                // For those rows where no info is displayed, just print white space:
                for (int k{}; k<white_space_size; k++) std::cout << " ";
            }
                
            // Now do the last column:
            // This colum runs from tile 31 (Pacific Avenue) to 39 (Boardwalk)
            std::cout << "|";
            if (i==0) std::cout << first_line[3*(size-1)+j];
            else if (i==1) std::cout << second_line[3*(size-1)+j];
            else if (i==3) std::cout << player_pos[3*(size-1)+j];
            else if (i==tile_height) std::cout << purchase_costs[3*(size-1)+j];
            else {
                for (int i{}; i< tile_width; i++) std::cout << " ";
            }
            std::cout << "|" << '\n';
        }
        // Print spaced line for all but the penlutimate row in the board.
        if (j < size-2) print_spaced_line();
    }
}

board::board(int size_, int width, int height,  std::vector<std::shared_ptr<property>> & props)
{
    size=size_; tile_width=width; tile_height = height;
    
    // Will assume the monopoly is a standard 11*11 board from now on:
    // The positions WITHOUT properties (i.e. position of chance, community chest, corners and taxes) are:
    //  - Corners: 0 for the GO tile, 10 for JUST VISITING, 20 for FREE PARKING and 30 for GO TO JAIL.
    //  - Chance: At positions 7, 22 and 36.
    //  - Comunity chest: At positions 2, 17 and 33.
    //  - Taxes: 4 for income tax (pay 200) and 38 for luxuary tax (pay 100).
    
    // Setup all vectors of strings:
    for (int i{}; i < 4*(size-1); i++) {
        // GO TILE:
        if (i==0) {
            first_line.push_back("GO <-");
            second_line.push_back("Add $200");
            purchase_costs.push_back("");
        }
        // COMMUNITY CHEST:
        else if (i == 2 || i == 17 || i == 33) {
            first_line.push_back("COMMUNITY");
            second_line.push_back("CHEST");
            purchase_costs.push_back("");
        }
        // INCOME TAX:
        else if (i== 4) {
            first_line.push_back("INCOME");
            second_line.push_back("TAX");
            purchase_costs.push_back("PAY $200");
        }
        // CHANCE:
        else if (i == 7 || i == 22 || i == 36) {
            first_line.push_back("CHANCE");
            second_line.push_back("???");
            purchase_costs.push_back("");
        }
        // JUST VISITING
        else if (i== 10) {
            first_line.push_back("JUST");
            second_line.push_back("VISITING");
            purchase_costs.push_back("");
        }
        // FREE PARKING
        else if (i== 20) {
            first_line.push_back("FREE");
            second_line.push_back("PARKING");
            purchase_costs.push_back("");
        }
        // GO TO JAIL
        else if (i== 30) {
            first_line.push_back("GO TO");
            second_line.push_back("JAIL");
            purchase_costs.push_back("");
        }
        // LUXUARY TAX
        else if (i== 38) {
            first_line.push_back("LUXUARY");
            second_line.push_back("TAX");
            purchase_costs.push_back("PAY $100");
        }
        // PROPERTIES:
        else {
            std::shared_ptr<property> current;
            try {current = props[position_to_prop_vector(i)];}
            catch (int) {std::cout << "Error reading in property to board" << std::endl;}
            first_line.push_back(current -> get_first_line());
            second_line.push_back(current -> get_second_line());
            purchase_costs.push_back("$"+std::to_string(current -> get_cost()) );
        }
    }
}

void board::print_board(std::vector<std::string> player_positions, std::vector<std::shared_ptr<player>> & players)
{
    // Center all string to size tile_width:
    std::vector<std::string> first_line_cent       = center_strings(first_line, tile_width);
    std::vector<std::string> second_line_cent      = center_strings(second_line, tile_width);
    std::vector<std::string> purchase_costs_cent   = center_strings(purchase_costs, tile_width);
    std::vector<std::string> player_positions_cent = center_strings(player_positions, tile_width);
    
    // Print rows:
    first_row(first_line_cent, second_line_cent, purchase_costs_cent, player_positions_cent);
    middle_rows(first_line_cent, second_line_cent, purchase_costs_cent, player_positions_cent, players);
    last_row(first_line_cent, second_line_cent, purchase_costs_cent, player_positions_cent);
}
