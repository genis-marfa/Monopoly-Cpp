#include <iostream>
#include <vector>
#include <string>
#include <random>
#include <algorithm>
#include <fstream>
#include <locale>
#include <memory>

#include "prelim_functions.hpp"
#include "player.hpp"
#include "property.hpp"
#include "property_group.hpp"
#include "street.hpp"
#include "railroad.hpp"
#include "utility.hpp"
#include "board.hpp"
#include "cards.hpp"
#include "main_functions.hpp"

// Standard rules by Hasbro implemented.
// Additional free parking jackpot house rule:
//      All tax payments are gathered at the center of the board, and the first player to land on
//      free parking gets the loot. A minimum payout of $100 is guaranteed.
void standard_rules()
{
    std::cout << "Do you wish to be reminded about the standard monopoly rules (y/n)? ";
    char yes_no;
    std::cin >> yes_no;
    while (yes_no != 'Y' && yes_no != 'y' && yes_no != 'n' && yes_no != 'N') {
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cout << "I didn't catch that, please try again: ";
        std::cin >> yes_no;
    }
    if ( yes_no == 'y' || yes_no == 'Y') {
        std::cout << "-----------------------------------------------------------------------------" << '\n';
        std::cout << "                          WELCOME TO MONOPOLY C++                            " << '\n';
        std::cout << "-----------------------------------------------------------------------------" << '\n';
        std::cout << '\n';
        std::cout << "The objective of the game is to become the wealthiest player by buying, sell-" << '\n';
        std::cout << "ing and renting property. The last player left without going bankrupt wins   " << '\n';
        std::cout << "the game. The order of play will be randomised at the start of the game. Each" << '\n';
        std::cout << "player will then proceed to roll the dice and to move around the play board. " << '\n';
        std::cout << "Everyone will start with a balance of $1500. Every time a player lands       " << '\n';
        std::cout << "on or passes over the GO tile, the bank pays them a salary of $200. De-" << '\n';
        std::cout << "pending on the tile you land on, you may be entitled to buy property, or obl-" << '\n';
        std::cout << "iged to pay rent, tax or draw a chance / community chest card.               " << '\n';
        std::cout << '\n';
        std::cout << "DICE ROLL: Two dice are rolled each turn. If you throw doubles, you can re-  " << '\n';
        std::cout << "roll the dice. If you roll doubles three consecutive times in a turn, you    " << '\n';
        std::cout << "will be caught speeding and sent to jail. "<< '\n';
        std::cout << '\n';
        std::cout << "BUYING PROPERTY: If you land on an unowned property, you may buy it from the  " << '\n';
        std::cout << "bank. If you don't, the property will be auctioned among all players. The     " << '\n';
        std::cout << "owner will recieve a title deed for that property." << '\n';
        std::cout << '\n';
        std::cout << "RENT: If you land on an property owned by another player, you must pay rent " << '\n';
        std::cout << "back to the owner according to the number of buildings constructed as printed" << '\n';
        std::cout << "in the title deed. If the property is mortgaged, no rent can be collected. " << '\n';
        std::cout << "Also, if the player owns all the properties in that colour group, they can  " << '\n';
        std::cout << "charge double rent.  " << '\n';
        std::cout << '\n';
        std::cout << "CONSTRUCTING: When a player owns all the properties in a colour group, they " << '\n';
        std::cout << "may buy houses from the bank and erect them on those properties. This must be" << '\n';
        std::cout << "done evenly, i.e. you cannot erect more than one house on any one property  " << '\n';
        std::cout << "until you have built a house on every property on that group. Similarly,    " << '\n';
        std::cout << "houses may be sold back to the bank for half their purchase price. This   " << '\n';
        std::cout << "must also be done evenly. When a player has four houses on each property in " << '\n';
        std::cout << "a group, they may construct a hotel. No buildings can be constructed on ra-" << '\n';
        std::cout << "ilroads or utilities, but these will also charge bonus rent if multiple are" << '\n';
        std::cout << "owned." << '\n';
        std::cout << '\n';
        std::cout << "CHANCE / COMMUNITY CHEST: If a player lands on a chance / community chest ti-" << '\n';
        std::cout << "le, they must draw a card from the deck. In the current version of Monopoly  " << '\n';
        std::cout << "C++ there are 4 possibilities: Pay tax or recieve dividends, go to jail, get " << '\n';
        std::cout << "out of jail free card or advance to the go tile and collect salary. " << '\n';
        std::cout << '\n';
        std::cout << "JAIL: You can be sent to jail by either: (1) Landing on the GO TO JAIL tile, " << '\n';
        std::cout << "(2) drawing a GO TO JAIL card or (3) rolling doulbes 3 consecutive times. While" << '\n';
        std::cout << "in jail, you may still participate in auction, buy and sell property and " << '\n';
        std::cout << "buildings and collect rent. You will be unable to move around the board and" << '\n';
        std::cout << "collect salary. There are 4 ways to get out of jail. (1) Using a get of out of" << '\n';
        std::cout << "jail free card, (2) purchasing one such card from a fellow player, (3) rolling" << '\n';
        std::cout << "doubles on any of your next three turns or (4) paying the $50 fee. Whenever a " << '\n';
        std::cout << "player is freed from jail, they will be placed on the JUST VISITING tile. " << '\n';
        std::cout << '\n';
        std::cout << "TAXES: If you land on a tax tile, or are forced to pay tax via a card, you" << '\n';
        std::cout << "must pay the prescribed amount. This will be gathered at the FREE PARKING " << '\n';
        std::cout << "JACKPOT. Any player landing on the free parking tile they will collect all" << '\n';
        std::cout << "the money accumulated. A minimum payout of $100 is guaranteed.  " << '\n';
        std::cout << '\n';
        std::cout << "SELLING PROPERTY: Unimproved properties may be sold to any player. This cannot" << '\n';
        std::cout << "be done if buildings are standing on any porperties in that group. Any buildings" << '\n';
        std::cout << "so located must be sold to the bank before the property can be sold. If only one" << '\n';
        std::cout << "player is interested in purchasing that property, a private negotiation is carried" << '\n';
        std::cout << "out. If more than one player is interested, an auction is held between all inter-" << '\n';
        std::cout << "ested players." << '\n';
        std::cout << '\n';
        std::cout << "MORTGAGE: All buildings must be sold before a property can be mortgaged. " << '\n';
        std::cout << "A player who opts to mortgage a property will recieve it's prescribed    " << '\n';
        std::cout << "mortgage value, but will be unable to collect rent while the property is mortgaged." << '\n';
        std::cout << "To lift the mortgage, the player must pay back the mortgage value to the bank at " << '\n';
        std::cout << "10% interest rate." << '\n';
        std::cout << '\n';
        std::cout << "BAKRUPTCY: If your balance falls negative during a turn, you must sell " << '\n';
        std::cout << "buildings or properties, or mortgage a property. If you are unable to do" << '\n';
        std::cout << "any of these actions, you must surrender from the game and declare bankrupcy." << '\n';
        std::cout << "All the properties owned by a player who goes bankrupt become unowned and may" << '\n';
        std::cout << "be purchased by another player who lands on them on a latter turn. " << '\n';
        std::cout << '\n';
        std::cout << "-----------------------------------------------------------------------------" << '\n';
        std::cout << '\n';
    }
}

// Reminder about how to play the C++ version of the game.
void cpp_rules()
{
   std::cout << "Do you wish to be reminded about the monopoly C++ rules (y/n)? ";
   char yes_no;
   std::cin >> yes_no;
   while (yes_no != 'Y' && yes_no != 'y' && yes_no != 'n' && yes_no != 'N') {
       std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
       std::cout << "I didn't catch that, please try again: ";
       std::cin >> yes_no;
   }
   if ( yes_no == 'y' || yes_no == 'Y') {
       std::cout << '\n' << "To play monopoly C++ a list of commands is available at each turn. " << '\n';
       std::cout << "Each command will lead to a different action: (e.g. MORTGAGE, CONS-" << '\n';
       std::cout << "TRUCT, DISPLAY BOARD, etc). Each player must enter the ROLL command" << '\n';
       std::cout << "to roll the dice and play their turn, even if they are in jail. " << '\n';
       std::cout << '\n';
       std::cout << "CAPS. LOCK is recommended to play the game, since all commands are in" << '\n';
       std::cout << "capitals. The current version supports a maximum of 4 players and a " << '\n';
       std::cout << "minimum of two. Other than that, all rules match that of the standard" << '\n';
       std::cout << "monopoly game." << '\n';
       std::cout << '\n';
       std::cout << "Enjoy the game!" << std::endl;
       std::cout << "-----------------------------------------------------------------------" << '\n';
       std::cout << '\n';
   }
}

// Creates all property "groups": Each property is either a railroad / utility, or it belongs to a particular colour group.
std::vector<property_group> create_property_groups ()
{
    property_group brown  ("BROWN");
    property_group l_blue ("LIGHT BLUE");
    property_group pink   ("PINK");
    property_group orange ("ORANGE");
    property_group red    ("RED");
    property_group yellow ("YELLOW");
    property_group green  ("GREEN");
    property_group d_blue ("DARK BLUE");
    property_group railroads ("RAILROAD");
    property_group utilities ("UTILITY");
    std::vector<property_group> all_groups{brown, l_blue, pink, orange, red, yellow, green, d_blue, railroads, utilities};
    
    return all_groups;
}

// Use the semicolon character as a delimiter to parse data from get_line.
std::string parse_line(std::string & line)
{
    std::string delimiter = ";";
    std::string string = line.substr(0, line.find(delimiter));
    // Erase data that has already been read in from line.
    erase_substring(line, string+";");
    return string;
}

// Given a string with the group's name, return it's index within vector of property groups.
int convert_to_colour_group_idx(std::string colour_group)
{
    // Find index within vector of groups:
    //  0 - BROWN
    //  1 - LIGHT BLUE
    //  2 - PINK
    //  3 - ORANGE
    //  4 - RED
    //  5 - YELLOW
    //  6 - GREEN
    //  7 - DARK BLUE

    if      (colour_group == "BROWN") return 0;
    else if (colour_group == "LIGHT BLUE") return 1;
    else if (colour_group == "PINK") return 2;
    else if (colour_group == "ORANGE") return 3;
    else if (colour_group == "RED") return 4;
    else if (colour_group == "YELLOW") return 5;
    else if (colour_group == "GREEN") return 6;
    else if (colour_group == "DARK BLUE") return 7;
    else throw 1;
}

// Read in data from "DATA.csv" file, pass through constructor and store in vector of smart pointers to properties.
std::vector<std::shared_ptr<property>> read_in_property_data(std::vector<property_group>  & groups)
{
    std::cout << '\n' << "Loading properties..." << std::endl;
    std::vector<std::shared_ptr<property>> properties;
    
    // Load DATA.csv which contains all the required information.
    std::ifstream InputFile;
    InputFile.open("DATA.csv");
    if (!InputFile) {
        std::cerr << "Unable to open file DATA.csv" << '\n';
        throw 1;
    }
    else std::cout << "Succesfully opened file." << '\n';
    std::string line;
    
    while (std::getline(InputFile, line)){
        std::string delimiter = ";";
        // Get the property type: Either street, railroad or utility.
        std::string type = line.substr(0, line.find(delimiter));
        
        // Each call to parse_line() erases the parsed data from the string.
        if (type == "STREET") {
            erase_substring(line, "STREET;");
            std::string first = parse_line(line);
            std::string second = parse_line(line);
            std::string colour_group = parse_line(line);
            int cost = std::stoi(parse_line(line));
            int rent = std::stoi(parse_line(line));
            int one_house = std::stoi(parse_line(line));
            int two_house = std::stoi(parse_line(line));
            int three_house = std::stoi(parse_line(line));
            int four_house = std::stoi(parse_line(line));
            int hotel = std::stoi(parse_line(line));
            int mortgage = std::stoi(parse_line(line));
            int house_cost = std::stoi(parse_line(line));
            int hotel_cost = std::stoi(parse_line(line));
            
            int colour_grop_idx;
            try { colour_grop_idx = convert_to_colour_group_idx(colour_group); }
            catch (int) { std::cerr << "Failed to determine property colour group!" << std::endl; }
            std::cout << "Loaded " << first << " " << second << " (Group: " << colour_group << ")." << std::endl;
            
            // Pass in data through street constructor:
            std::shared_ptr<property> ptr = std::make_shared<street>(first, second, cost, rent, one_house, two_house, three_house, four_house, hotel, mortgage, house_cost,hotel_cost);
            // Store a copy of the pointer in the colour_group, then move ownership to vector.
            groups[colour_grop_idx].add_to_property_group(ptr); properties.push_back(std::move(ptr));
            
        }
        if (type == "RAILROAD") {
            erase_substring(line, "RAILROAD;");
            std::string name = parse_line(line);
            std::cout << "Loaded " << name << " " << "RAILROAD" << std::endl;
            
            std::shared_ptr<property> ptr = std::make_shared<railroad>(name);
            groups[8].add_to_property_group(ptr); properties.push_back(std::move(ptr));
        }
        if (type == "UTILITY") {
            erase_substring(line, "UTILITY;");
            std::string first = parse_line(line);
            std::string second = parse_line(line);
            std::cout << "Loaded " << first << " " << second << std::endl;
            
            std::shared_ptr<property> ptr = std::make_shared<utility>(first, second);
            groups[9].add_to_property_group(ptr); properties.push_back(std::move(ptr));
        }
    }
    return properties;
}

// Create a vector of players. Store in vector of shared smart pointers.
std::vector<std::shared_ptr<player>> create_players(int num_players, std::vector<std::string> & player_positions)
{
    std::vector<std::shared_ptr<player>> players;
    for (int i{}; i<num_players; i++) {
        std::string play_name;
        std::cout << "Player " << i+1 << ", please enter your name: ";
        std::cin >> play_name;
        if (play_name.size() < 2) play_name += " ";
        std::shared_ptr<player> p = std::make_shared<player>(play_name);
        p -> place(player_positions, 0);
        players.push_back(std::move(p)); // Move ownership into vector.
    }
    // Clear buffer
    std::cin.ignore();
    
    // Shuffle order of play:
    shuffle_order(players);
    
    return players;
}

// Create deck of chance / community chest cards. Store in vector of unique smart pointers.
std::vector<std::unique_ptr<card>> create_deck()
{
    std::cout << "Creating deck of Chance / Community cards..." << std::endl;
    std::vector<std::unique_ptr<card>> deck(4);
    deck[0] = std::unique_ptr<card> (new tax_or_dividend_card);
    deck[1] = std::unique_ptr<card> (new go_tile_card);
    deck[2] = std::unique_ptr<card> (new out_of_jail_card);
    deck[3] = std::unique_ptr<card> (new to_jail_card);
    
    return deck;
}

int main()
{
    // Display standard MONOPOLY game rules and MONOPOLY C++ rules.
    standard_rules();
    cpp_rules();
    
    // Create a vector of strings with the current positions of the players in the board.
    std::vector<std::string> player_positions(40);
    for (int i{0}; i<40; i++) player_positions[i]="";
    
    // Create the players for the game:
    int num_players;
    std::cout << "Please enter the number of players (minimum 2, maximum 4): ";
    std::cin >> num_players;
    while (!std::cin.good() || num_players > 5 || num_players < 2) {
        std::cin.clear(); std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cout << "Invalid input. Please try again: ";
        std::cin >> num_players;
    }
     std::vector<std::shared_ptr<player>> players = create_players(num_players, player_positions);
    
    std::cout << "Are you all set (y/n)? ";
    char yes_no;
    std::cin >> yes_no;
    while (yes_no != 'Y' && yes_no != 'y' && yes_no != 'n' && yes_no != 'N') {
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cout << "I didn't catch that, please try again: ";
        std::cin >> yes_no;
    }
    // If player is not ready, restart the programme.
    if (yes_no == 'n' || yes_no == 'N') {
        players.clear();
        main();
    }
    std::cin.ignore();
    
    // Create objects required to play game:
    std::vector<property_group> all_groups = create_property_groups();
    
    
    std::vector<std::shared_ptr<property>> properties;
    try {properties = read_in_property_data(all_groups);}
    
    // If DATA.csv fails to load, terminate the programme with exit code 1.
    catch (int) {
        std::cout << "Fatal error. Terminating programme." << std::endl;
        return 1;
    }
    
    // Store deck of chance / community chest cards
    std::vector<std::unique_ptr<card>> deck = create_deck();
    
    // Create 11x11 baord, with tilesize 12 x 5:
    board play_board(11, 12, 5, properties);
    
    // Play the game by calling the main function: 'make_move'. This function will allow the player to
    //      buy / sell properties and houses, mortgage properties, roll the dice and interact with the board, etc.
    while (players.size() > 1) {
        for (int i{}; i < num_players; i++) {
            play_turn(players[i], players, play_board, player_positions, properties, all_groups, deck, num_players);
        }
    }
    // Game ends when only one player is left without going bankrupt.
    
    properties.clear();
    players.clear();
    deck.clear();
    
    return 0;
}
