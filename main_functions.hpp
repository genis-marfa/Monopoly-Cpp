#include <stdio.h>
#include "cards.hpp"
#include <thread>
#include <chrono>
#include <algorithm>

#ifndef main_functions_hpp
#define main_functions_hpp

// ----------------------------------------------------------------------------------
//                          FUNCTIONS TO PLAY GAME:
// ----------------------------------------------------------------------------------
// Includes all the necesary functions to play the game:
//  - Draw card: Draws a random chance /  community chest card from the deck
//                  according to their draw probabilities.
//  - Print player deeds: Prints all the deeds held by a player.
//  - List properties held: Print (in a list) all the properties owned by a player.
//  - Print all groups: Prints all the properties in the game, sorted by group.
//  - Property search: Searches for a property by name.
//  - Group search: Search for a group by name.
//  - Get info: Get information about a certain property.
//  - Sell property: Player can sell a property to fellow players.
//  - Construct (single_property): Construct a house / hotel at an individual property.
//  - Construct (full_group): Construct houses at all of the properties in a group simulatneously.
//  - Sell building (single_property): Sell a house / hotel at an individual property.
//  - Sell building (full_group): Sell houses at all of the properties in a group simulatneously.
//  - Mortgage: Mortgage a property.
//  - Lift mortgage: Lifts a mortgage.
//  - Go bankrupt: Player surrenders from the game.
//  - In debt: Gives a options to a player whose balance
//          has fallen negative throughout the turn.
//  - Get Command: Prints out all the in-game commands available, and
//                  gathers user input.
//  - Make move: Move's the player and interacts with the board.


void draw_card(std::vector<std::unique_ptr<card>> & deck, std::shared_ptr<player> & plyr, int & new_pos);
void print_player_deeds(std::shared_ptr<player> & plyr, std::vector<property_group> all_groups);
void list_props_held(std::shared_ptr<player> & plyr, std::vector<property_group> all_groups);
void print_all_groups (std::vector<property_group> all_groups);
int prop_search(std::vector<std::shared_ptr<property>> & properties, std::vector<property_group> all_groups);
int group_search(std::vector<property_group> all_groups);
void get_info(std::vector<std::shared_ptr<property>> & properties, std::vector<property_group> all_groups);
void sell_property (std::shared_ptr<player> & seller, std::vector<std::shared_ptr<property>> & properties, std::vector<std::shared_ptr<player>>                 & players, std::vector<property_group> all_groups);

// Distinguish if we are going to construct / sell a building on a single property
//      or for the whole group by using namespaces.
namespace single_property
{
    void construct(std::shared_ptr<player> & plyr, std::vector<property_group> all_groups, std::vector<std::shared_ptr<property>> & properties);
    void sell_building(std::shared_ptr<player> & plyr, std::vector<property_group> all_groups, std::vector<std::shared_ptr<property>> & properties);
}

namespace full_colour_group
{
    void construct (std::shared_ptr<player> & plyr, std::vector<property_group> all_groups, std::vector<std::shared_ptr<property>> & properties);
    void sell_building(std::shared_ptr<player> & plyr, std::vector<property_group> all_groups, std::vector<std::shared_ptr<property>> & properties);
}

void mortgage_a_property(std::shared_ptr<player> & plyr, std::vector<property_group> all_groups,
                         std::vector<std::shared_ptr<property>> & properties);
void lift_a_mortgage(std::shared_ptr<player> & plyr, std::vector<property_group> all_groups,
                     std::vector<std::shared_ptr<property>> & properties);
void go_bankrupt(std::shared_ptr<player> & plyr, std::vector<std::shared_ptr<player>> & players,
                 std::vector<std::string> & player_pos, int & num_players);

// If a player's balance falls negative during a turn, he must either:
//   - Sell buildings erected at one of their properties.
//   - Sell a property to a fellow player.
//   - Mortgage one of their properties.
//   - Go bankrupt.
void in_debt(std::shared_ptr<player> & plyr, std::vector<property_group> all_groups, std::vector<std::shared_ptr<property>> & properties,                  std::vector<std::shared_ptr<player>> & players, std::vector<std::string> & player_pos, int & num_players);

std::string get_command();
void sleep_2_seconds(); // Console sleeps for 2 seconds.

void play_turn(std::shared_ptr<player> & plyr, std::vector<std::shared_ptr<player>> & players, board & play_board, std::vector<std::string> &                player_pos, std::vector<std::shared_ptr<property>> & properties, std::vector<property_group> all_groups,                                      std::vector<std::unique_ptr<card>> & deck, int & num_players);

#endif /* main_functions_hpp */
