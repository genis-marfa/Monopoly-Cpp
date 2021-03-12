#include "main_functions.hpp"
#include <memory>
#include <cassert>

// Draws a random chance /  community chest card from the deck according to their draw probabilities.
void draw_card(std::vector<std::unique_ptr<card>> & deck, std::shared_ptr<player> & plyr, int & new_pos)
{
    // Confirm probabilities are normalised.
    int total_prob{0};
    for (unsigned i{}; i<deck.size(); i++) total_prob += deck[i] -> get_probab();
    assert(total_prob == 100);
    
    // Draw random number from 1 to 10.
    int rand_num = rng(10)+1;
    
    // If random number lies between two probabilities, draw that card:
    int probab0 = 0;
    for (unsigned i{}; i<deck.size(); i++) {
        int probab1 = probab0 + (deck[i] -> get_probab()) / 10;
        if (rand_num > probab0 && rand_num <= probab1) deck[i] -> draw(plyr, new_pos);
        probab0 = probab1;
    }
}

// Print all the deeds held by a player.
void print_player_deeds(std::shared_ptr<player> & plyr, std::vector<property_group> all_groups)
{
    std::cout << '\n';
    std::cout << "Deeds currently held by " << plyr -> plyr_name() << ":" << std::endl;
    std::cout << '\n';
    for (auto ptr : plyr -> properties_owned) ptr -> print_title_deed(all_groups);
}

void list_props_held(std::shared_ptr<player> & plyr, std::vector<property_group> all_groups)
{
    std::cout << '\n';
    std::cout << "Properties currently held by " << plyr -> plyr_name() << ":" << std::endl;
    std::cout << '\n';
    
    // Print out property name, colour group, # buildings constructed, mortaged state.
    std::vector<std::string> prop_names, prop_colours, buildings_constructed, props_mortgaged;
    for (auto ptr : plyr -> properties_owned) {
        prop_names.push_back(ptr -> get_property_name());
        prop_colours.push_back(ptr -> get_group_string(all_groups));
        buildings_constructed.push_back(std::to_string(ptr -> get_num_buildings() ) );
        
        std::string yes_no;
        if (ptr -> mortgaged) yes_no = "Y";
        else yes_no = "N";
        props_mortgaged.push_back(yes_no);
    }
    
    for (unsigned i{}; i<prop_colours.size(); i++) {
       // Store information in a vector:
        std::vector <std::string> info {"Property name", "Group", "Houses/Hotels Built", "Mortgaged",
                                        "---------------", "-------", "---------------------", "-------------",
                                        prop_names[i], prop_colours[i], buildings_constructed[i], props_mortgaged[i]};
        
        // Center the strings:
        std::vector <std::string> info_cent = center_strings(info, 24);
        
        // Print out the headers (and underlines) only once:
        if(i==0) {
            std::cout << info_cent[0] << info_cent[1] << info_cent[2] << info_cent[3] << std::endl;
            std::cout << info_cent[4] << info_cent[5] << info_cent[6] << info_cent[7] << std::endl;
        }
        
        // Print out the information:
        std::cout << info_cent[8] << info_cent[9] << info_cent[10] << info_cent[11] <<  std::endl;
        
    }
    std::cout << '\n';
}

void print_all_groups (std::vector<property_group> all_groups)
{
    for(unsigned i{}; i<all_groups.size(); i++) all_groups[i].print_property_group();
}

// Search for a property, return it's index within a vector of pointers to properties.
int prop_search(std::vector<std::shared_ptr<property>> & properties, std::vector<property_group> all_groups)
{
    bool found{false};
    std::string prop_name;
    std::cout << "Enter CANCEL at any time to cancel the operation." << std::endl;
        
    while (!found && prop_name != "CANCEL") {
        std::cout << "Enter the property name: ";
        std::getline (std::cin, prop_name);
        int i{};
        
        // Iterate over properties vector and check if names match:
        for (auto prop : properties) {
            if (prop_name == prop -> get_property_name()) {
                std::cout << "You entered " << prop -> get_property_name() << " (Group: " << prop -> get_group_string(all_groups) << ")." << '\n';
                return i;
            }
            i++;
        }
        if (prop_name != "CANCEL") std::cerr << "Did not find the property." << std::endl;
    }
    if (prop_name == "CANCEL") throw 1;
        
    return 0;
}

// Search for a group, return it's index within a vector of groups
int group_search(std::vector<property_group> all_groups)
{
    bool found{false};
    std::string group_name;
    
    std::cout << "Enter CANCEL at any time to cancel the operation." << std::endl;
    while (!found && group_name != "CANCEL") {
        std::cout << "Enter the group name: ";
        std::getline (std::cin, group_name);
        
        // Iterate over groups and check group names match
        for (unsigned j{}; j<all_groups.size(); j++) {
            if (group_name == all_groups[j].get_group_name()) {
                std::cout << "You entered " << all_groups[j].get_group_name() << std::endl;
                found = true;
                return j;
            }
        }
        if (group_name != "CANCEL") std::cerr << "Did not find the property group." << std::endl;
    }
    
    if (group_name == "CANCEL") throw 1;
    
    return 0;
}

// Get information about a certain property:
void get_info(std::vector<std::shared_ptr<property>> & properties, std::vector<property_group> all_groups)
{
    std::cout << "For what property do you wish to get information?" << std::endl;
    int idx;
    try {idx = prop_search(properties, all_groups);}
    catch(int) {
        std::cout << "Cancelling operation." << std::endl;
        return;
    }
    
    // Print out title deed, current owner and number of buildings constructed:
    std::cout << "Information:" << std::endl;
    properties[idx] -> print_title_deed(all_groups);
    std::cout << "Owner: " << properties[idx] -> get_ownership() << std::endl;
    std::cout << "Houses/Hotels erected: " << properties[idx] -> get_num_buildings() << std::endl;
}

void sell_property (std::shared_ptr<player> & seller, std::vector<std::shared_ptr<property>> & properties, std::vector<std::shared_ptr<player>> &                       players, std::vector<property_group> all_groups)
{
    // Confirm the player has properties to sell: You can only sell a property if no other properties
    //      within it's colour group have buildings erected.
    size_t sellable_props = seller -> properties_owned.size();
    for (unsigned i{}; i< seller -> properties_owned.size(); i++) {
        // Get the colour group.
        property_group colour("");
        try {
            colour = seller -> properties_owned[i] -> get_group(all_groups);
        }
        catch (int) {
            std::cerr << "Fatal error. Group not found!" << std::endl;
            std::cerr << "Terminating process." << std::endl;
            return;
        }
        // Check if group is constructed, if so, property becomes unsellable.
        if (colour.is_constructed()) sellable_props -= 1;
    }
    std::cout << "You have " << sellable_props << " sellable properties." << std::endl;
    
    // If no properties to sell, terminate the function.
    if (sellable_props == 0) {return;}
    
    // Otherwise, display properties held by calling list_props_held():
    std::cout << "You currently own the following properties:" << std::endl;
    list_props_held(seller, all_groups);
    
    bool valid_property{false};
    int prop_idx{0};
    while (!valid_property) {
        std::cout << "Which one do you want to sell? " << std::endl;
        // Get property index within vector of properties:
        try {prop_idx = prop_search(properties, all_groups);}
        catch(int) {
            std::cout << "Cancelling operation." << std::endl;
            return;
        }
        // Get colour group:
        property_group colour("");
        try {
            colour = properties[prop_idx] -> get_group(all_groups);
        }
        catch (int) {
            std::cerr << "Fatal error. Group not found!" << std::endl;
            std::cerr << "Terminating process." << std::endl;
            return;
        }
        
        // Verify player owns that property:
        if (properties[prop_idx] -> get_ownership() != seller -> plyr_name() )
            std::cout << "You do not own that property." << std::endl;
        
        // Verifty group has no other properties with buildings erected:
        else if (colour.is_constructed()) {
            std::cout << "A property in that colour group has a building erected." << '\n'
                << "You cannot sell properties where buildings are standing on any of the properties in that colour group." << '\n';
        }
        // Carry out the sale:
        else {
            std::cout << "You chose to sell " << properties[prop_idx] -> get_property_name() << ". Beware that if more than one player " <<
            "is interested " << '\n' << "and the property is auctioned, you will not be able to cancel the sale." << std::endl;
        
            char yes_no;
            std::cout << "Are you sure? (y/n) ";
            std::cin >> yes_no;
            if (yes_no == 'y' || yes_no == 'Y') valid_property = true;
        }
    }
    
    // If more than one player is interested. Auction property between those who are interested.
    // Create a vector of players and store all players interested in purchasing the property.
    std::vector<std::shared_ptr<player>> interested_players;
    for (unsigned i{}; i<players.size(); i++) {
        // Exclude player who is selling:
        if  (players[i] -> plyr_name() != seller -> plyr_name()) {
            char yes_no;
            std::cout << players[i] -> plyr_name() << " are you interested in this property? (y/n) ";
            std::cin >> yes_no;
            while (yes_no != 'y' && yes_no != 'Y' && yes_no != 'n' && yes_no != 'N'){
                std::cin.clear(); std::cin.ignore();
                std::cout << "Sorry I didn't catch that: ";
                std::cin >> yes_no;
            }
            // If player is interested, push back to the vector of interested players.
            //  Make a local copy of pointer in vector, which will get deallocated when function body goes out of scope.
            if ( yes_no == 'y' || yes_no == 'Y')  interested_players.push_back(players[i]);
        }
    }
    // At this point, there are three possibilities:
    // 1. Nobody was interested -> Terminate function.
    // 2. One person was interested -> He will make a private offer to be accepted by the seller.
    // 3. More than one person interested -> Property is auctioned.
    
    // Case 1: Nobody interested:
    if (interested_players.size() == 0) std::cout << "Sorry. Nobody was interested in this property." << std::endl;
    
    // Case 2: One player interested:
    else if (interested_players.size() == 1) {
        std::shared_ptr<player> interested_player = interested_players[0];
        std::cout << "Only " << interested_player ->plyr_name() << " is interested in this property." << std::endl;
        
        // Player makes a private offer:
        int offer;
        std::cout << interested_player ->plyr_name() << ", make an offer: $";
        std::cin >> offer;
        std::cin.ignore();
        
        // Seller must accept this offer:
        char yes_no;
        std::cout << seller -> plyr_name() << ", do you accept selling this property for $" << offer << "? (y/n) ";
        std::cin >> yes_no;
        if (yes_no == 'y' || yes_no =='Y') {
           std::cout <<  properties[prop_idx] -> get_property_name() << " was sold to " << interested_player -> plyr_name()
                                        << " for $" << offer << std::endl;
            interested_player -> pay(offer);
            seller -> receive(offer);
            properties[prop_idx] -> change_owner(interested_player);
        }
    }
    // Case 3: More than one player interested:
    else if (interested_players.size() > 1) {
        std::cout << "More than one player was interested. Auctioning property..." << std::endl;
        int highest_bid = properties[prop_idx] -> auction(interested_players, all_groups);
        seller -> receive(highest_bid);
    }
}

void single_property::construct(std::shared_ptr<player> & plyr, std::vector<property_group> all_groups, std::vector<std::shared_ptr<property>> & properties)
{
    std::cout << '\n' << "Attempting to construct a new house / hotel..." << std::endl;
    std::cout << '\n' << "You can only construct in properties from groups you fully own." << std::endl;
    std::cout << "Furthermore, houses must be constructed evenly. i.e. you cannot erect " << std::endl;
    std::cout << "more than one house on any one property of any color-group until you " << std::endl;
    std::cout << "have built one house on every property of that group." << '\n' << std::endl;
    
    // First check that the player owns at least  one group fully.
    std::cout << "You fully own the following groups:" << std::endl;
    int count{}, idx{};
    // Store in a vector the indices of the group, within the vector of groups.
    std::vector<int> owned_group_indices;
    
    // Iterate up to (size-2) to discard the two final groups in the vector: Railroads
    //               and utilities, where you cannot construct.
    std::for_each(all_groups.begin(), all_groups.end()-2,
        [&count, &owned_group_indices, &idx, &plyr] (property_group colour_group)
        {
            if (colour_group.owns_full_group(plyr)) {
                colour_group.print_property_group();
                count++;
                owned_group_indices.push_back(idx);
            }
            idx++;
        });
    
    // Can only construct if at least one group is fully owned.
    if (count > 0) {
        bool valid_property{false};
        int prop_idx;
        
        while (!valid_property) {
            std::cout << "In which of the above properties would you like to construct?" << std::endl;
            try {prop_idx = prop_search(properties, all_groups);}
            catch(int) {
                std::cout << "Cancelling operation." << std::endl;
                return;
            }
            
            for (unsigned i{}; i<owned_group_indices.size(); i++) {
                // First verify the property entered belongs to one of the groups fully owned:
                if (all_groups[owned_group_indices[i]].in_group(properties[prop_idx])) {
                    // Verify construction is carried out evenly. If the operation is valid,
                    //            it will be carried out in the if statement.
                    if (properties[prop_idx] -> add_new_building(all_groups)) {
                        // Get construction information:
                        int builds_constructed = properties[prop_idx] -> get_num_buildings();
                        int house_price        = properties[prop_idx] -> get_house_price();
                        int hotel_price        = properties[prop_idx] -> get_hotel_price();
                        
                        // Player must pay the price for the construction.
                        if (builds_constructed < 5) plyr -> pay(house_price);
                        else if (builds_constructed == 5) plyr -> pay(hotel_price);
                        
                        valid_property = true; // Terminate while loop.
                    }
                }
            }
            if (!valid_property) std::cout << "You cannot construct there." << std::endl;
         }
    }
}


void single_property::sell_building(std::shared_ptr<player> & plyr, std::vector<property_group> all_groups, std::vector<std::shared_ptr<property>> & properties)
{
    // Check player has buildings to sell, by verify they satisfy the sell_evenly condition:
    int props_with_sellable_buildings{};
    std::cout << "Atempting to sell a building..." << std::endl;
    
    // Count how many properties have sellable buildings:
    std::for_each(plyr -> properties_owned.begin(), plyr -> properties_owned.end(),
        [&props_with_sellable_buildings, &all_groups] (std::shared_ptr<property> prop)
        {
            // Get group colour and verify sell evenly condition is met.
            property_group colour("");
            try {
                colour = prop -> get_group(all_groups);
            }
            catch (int) {
                std::cerr << "Fatal error. Group not found!" << std::endl;
                std::cerr << "Terminating process." << std::endl;
                return;
            }
        
            if (colour.sell_evenly(prop) && prop -> get_num_buildings() > 0 ) props_with_sellable_buildings++;
        });
    
    std::cout << "You have " << props_with_sellable_buildings << " properties where buildings can be sold." << std::endl;
    
    // If no buildings to sell, terminate function.
    if (props_with_sellable_buildings == 0) return;
    
    // Display properties held:
    list_props_held(plyr, all_groups);
    
    bool valid_property{false};
    int prop_idx{0};
    while (!valid_property) {
        std::cout << "In which of the above property/properties do you want to sell a building?" << std::endl;
        try {prop_idx = prop_search(properties, all_groups);}
        catch(int) {
            std::cout << "Cancelling operation." << std::endl;
            return;
        }
        
        // Verify player owns that property:
        if (properties[prop_idx]-> get_ownership() != plyr -> plyr_name() )
            std::cerr << "You don't own that property" << std::endl;
        
        // Verify there are buildings there:
        else if (properties[prop_idx] -> get_num_buildings() == 0)
            std::cerr << "No buildings erected there." << std::endl;
        else {
            // Verify sell_evenly condition is met. If so, the operation will be carried out.
            if (properties[prop_idx] -> sell_building(all_groups)) {
                // Terminate loop.
                valid_property = true;
                // Recieve half the house construction price.
                plyr -> receive(0.5* properties[prop_idx] -> get_house_price() );
            }
        }
    }
}


void full_colour_group::construct(std::shared_ptr<player> & plyr, std::vector<property_group> all_groups, std::vector<std::shared_ptr<property>> & properties)
{
    std::cout << '\n' << "You have selected to construct across all the properties in a colour group." << std::endl;
    std::cout << "You can only construct in properties from groups you fully own." << '\n' << std::endl;

    std::cout << "You fully own the following colour groups:" << std::endl;
    int count{}, idx{};
    std::vector<int> owned_group_indices;
    
    // Again, iterate up to (size-2) to discard the two final groups in the vector: Railroads
    //   and utilities, where you cannot construct.
    std::for_each(all_groups.begin(), all_groups.end()-2,
    [&count, &owned_group_indices, &idx, &plyr] (property_group colour_group)
    {
        if (colour_group.owns_full_group(plyr)) {
            colour_group.print_property_group();
            count++;
            owned_group_indices.push_back(idx);
        }
        idx++;
    });

    std::cout << '\n';
    
    // Can only construct if at least 1 group fully owned.
    if (count > 0) {
        bool valid_group = false;
        while (!valid_group) {
            std::cout << "In which group do you want to contruct?" << std::endl;
            int group_idx{};
            try { group_idx = group_search(all_groups);}
            catch (int) {
                std::cout << "Cancelling operation." << std::endl;
                return;
            }
            
            for (unsigned i{}; i<owned_group_indices.size(); i++) {
                // // First verify the group entered is indeed fully owned:
                if (group_idx == owned_group_indices[i]) {
                    // Request number of houses to construct at once:
                    unsigned num_houses_to_construct;
                    std::cout << "Enter the number of houses you wish to construct at once: ";
                    std::cin >> num_houses_to_construct;
                    std::cin.ignore();
                    
                    // Get the maximum number of buildings constructed in any one property within the group:
                    int max_num_constructed = all_groups[group_idx].max_number_of_buildings();
                    
                    // Get the cost of the operation:
                    size_t cost = all_groups[group_idx].get_construction_price(num_houses_to_construct);
                    
                    // Verify player can afford the operation:
                    if (plyr -> current_balance() > cost) {
                        // Verify no more than 5 buildings are constructed at none of the properties:
                        if (num_houses_to_construct > 5-max_num_constructed)
                            std::cout << "You can't construct more than 5 buildings" << std::endl;
                        else {
                            // Carry out the operation:
                            all_groups[group_idx].construct_across_all_group(num_houses_to_construct, all_groups);
                            plyr -> pay(cost);
                            valid_group = true; // Terminate loop.
                        }
                    }
                    else std::cout << "Insufficient funds." << std::endl;
                }
            }
            if (!valid_group) std::cout << "Operation failed. Trying again..." << std::endl;
        }
    }
}

void full_colour_group::sell_building(std::shared_ptr<player> & plyr, std::vector<property_group> all_groups, std::vector<std::shared_ptr<property>> & properties)
{
    std::cout << '\n' << "You have selected to sell buildings across all the properties in a colour group." << std::endl;
    std::cout << "You have at least one building on the following colour groups:" << std::endl;
    int count{}, idx{};
    
    // Get indices of groups with at least one building erected:
    std::vector<int> constructed_groups_indices;
    
    std::for_each(all_groups.begin(), all_groups.end()-2,
    [&count, &constructed_groups_indices, &idx, &plyr] (property_group colour_group)
    {
        if (colour_group.owns_full_group(plyr) && colour_group.max_number_of_buildings() > 0) {
            colour_group.print_property_group();
            count++;
            constructed_groups_indices.push_back(idx);
        }
        idx++;
    });
    
    if (count > 0) {
        bool valid_group = false;
        while (!valid_group) {
            std::cout << "In which group do you want to sell your buildings?" << std::endl;
            int group_idx{};
            try { group_idx = group_search(all_groups);}
            catch (int) {
                std::cout << "Cancelling operation." << std::endl;
                return;
            }
            for (unsigned i{}; i<constructed_groups_indices.size(); i++) {
                // Verify inputed group is fully owned and at least one building:
                if (group_idx == constructed_groups_indices[i]) {
                    // Request number of buildings to sell:
                    unsigned num_buildings_to_sell;
                    std::cout << "Enter the number of buildings you wish to sell: ";
                    std::cin >> num_buildings_to_sell;
                    std::cin.ignore();
                    
                    // Get minimum number of buildings constructed in any one property within the group:
                    int min_number_of_buildings = all_groups[group_idx].min_number_of_buildings();
                    
                    // Verify the number of buildings to sell is plausible:
                    if (num_buildings_to_sell > min_number_of_buildings)
                            std::cout << "You don't have that many buildings in your properties." << std::endl;
                    else {
                        // Carry out operation and recieve sale value:
                        int sale_value = all_groups[group_idx].sell_across_all_group(num_buildings_to_sell, all_groups);
                        plyr -> receive(sale_value);
                        valid_group = true; // Terminate loop.
                    }
                }
            }
            if (!valid_group) std::cout << "Operation failed. Trying again..." << std::endl;
        }
    }
}

// Mortgage one of your properties. This can only be done if no buildings are standing.
void mortgage_a_property(std::shared_ptr<player> & plyr, std::vector<property_group> all_groups, std::vector<std::shared_ptr<property>> & properties)
{   std::cout << "Attempting to mortgage a property..." << std::endl;
    std::cout << "Remember you cannot mortgage a property if it has a building standing." << std::endl;
    std::cout << '\n' << "Currently, you can mortgage the following properties:" << std::endl;
    
    // Check the number of properties player can mortgage:
    int props_to_mortgage{0};
    std::for_each(plyr-> properties_owned.begin(), plyr-> properties_owned.end(),
        [&props_to_mortgage] (std::shared_ptr<property> & prop)
        {
            if ( !prop -> mortgaged && prop -> get_num_buildings() == 0) {
                std::cout << prop -> get_property_name() << " (Mortgage Value: $" <<
                prop -> get_mortgage_value() << ")" << std::endl;
                props_to_mortgage++;
            }
        });
    std::cout << '\n';
    
    if (props_to_mortgage == 0) return;
    
    bool valid_prop = false;
    while (!valid_prop){
        std::cout << "Which property do you wish to mortgage?" << std::endl;
        int prop_idx{};
        try {prop_idx = prop_search(properties, all_groups);}
        catch(int) {
            std::cout << "Cancelling operation." << std::endl;
            return;
        }
        
        // Carry out mortgage if possible, and terminate loop.
        if (properties[prop_idx] -> mortgage_property(plyr)) valid_prop = true;
    }
}

// Lift a mortgage by paying 10% interest back to the bank.
void lift_a_mortgage(std::shared_ptr<player> & plyr, std::vector<property_group> all_groups, std::vector<std::shared_ptr<property>> & properties)
{
    // Verifty that player has mortgaged properties.
    int count{};
    std::cout << "Attempting to lift a mortgage..." << '\n' << std::endl;
    std::cout << plyr ->plyr_name() << ", you currenlty have the following properties mortgaged:" << std::endl;
    std::for_each(plyr-> properties_owned.begin(), plyr-> properties_owned.end(),
        [&count] (std::shared_ptr<property> & prop)
        {
            if ( prop -> mortgaged ) {
                std::cout << prop -> get_property_name() <<  " (Cost: $"
                        << 1.1*prop-> get_mortgage_value() << ")" << std::endl;
                count++;
            }
        });
    std::cout << '\n';
    
    // If player holds no mortgaged properties, terminate the function.
    if (count == 0) {
        std::cout << "You have no mortgaged properties!." << std::endl;
        return;
    }

    bool valid_prop = false;
    while (!valid_prop) {
        std::cout << "For which property do you want to lift the mortgage? " << std::endl;
        int prop_idx{};
        try {prop_idx = prop_search(properties, all_groups);}
        catch(int) {
            std::cout << "Cancelling operation." << std::endl;
            return;
        }
        if (properties[prop_idx] -> lift_mortage(plyr))
            valid_prop = true;
    }
}

// Player declares bankrupcy and surrenders from the game.
void go_bankrupt(std::shared_ptr<player> & plyr, std::vector<std::shared_ptr<player>> & players, std::vector<std::string> & player_pos, int & num_players)

{
    std::cout << "Are you sure you want to surrender? (y/n): ";
    char yes_no;
    std::cin >> yes_no;
    while (yes_no != 'Y' && yes_no != 'y' && yes_no != 'N' && yes_no != 'N') {
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cout << "I didn't catch that, please try again: ";
        std::cin >> yes_no;
    }
    std::cin.ignore();
    if (yes_no == 'n' || yes_no == 'N') throw 1;
    
    // Clear all properties owned.
    for (size_t i{}; i < plyr -> properties_owned.size(); i++) {
        plyr -> properties_owned[i] -> clear_owner();
    }
    // Call declare bankrupcy method from player class. This will:
    // - Delete player abbreviation (token) from board.
    // - Delete player from vector of players.
    // - Delete the pointer to this player.
    // - Checks if the game is finished. (Only one player left).
    plyr -> go_bankrupt(players, player_pos);
    
    num_players-=1;
}

void in_debt(std::shared_ptr<player> & plyr, std::vector<property_group> all_groups, std::vector<std::shared_ptr<property>> & properties,                                      std::vector<std::shared_ptr<player>> & players, std::vector<std::string> & player_pos, int & num_players)
{
    std::cout << "Enter a command: ";
    std::string command;
    std::getline (std::cin, command);
    // Store valid commands in a vector of strings.
    std::vector<std::string> valid_comands = {"SELL BUILDING", "SELL PROPERTY", "MORTGAGE", "GO BANKRUPT"};
    std::vector<std::string>::iterator it = std::find(valid_comands.begin(), valid_comands.end(), command);
    
    // Verify the command is valid by finding it in the vector of valid commands.
    while (it == valid_comands.end() ) {
        std::cout << "Sorry, I didn't catch that. Please try again: ";
        std::getline (std::cin, command);
        it = std::find(valid_comands.begin(), valid_comands.end(), command);
    }
    // Execute whichever action is specified by the command.
    if (command == "SELL BUILDING")         single_property::sell_building(plyr, all_groups, properties);
    else if (command == "SELL PROPERTY")    sell_property(plyr, properties, players, all_groups);
    else if (command == "MORTGAGE")         mortgage_a_property(plyr, all_groups, properties);
    else if (command == "GO BANKRUPT")      go_bankrupt(plyr, players, player_pos, num_players);
}


// Print a table of the available commands before the player rolls the dice.
// Return the command string.
std::string get_command()
{
    std::cout <<'\n' << "List of commands:" << std::endl;
    std::cout << "  --------------------------------------------------------------------------------------------" << std::endl;
    std::cout << "  |       COMMAND       |                               ACTION                               |" << std::endl;
    std::cout << "  |---------------------|--------------------------------------------------------------------|" << std::endl;
    std::cout << "  |    DISPLAY BOARD    |              Displays the playboard's current status.              |" << std::endl;
    std::cout << "  |·····················|····································································|" << std::endl;
    std::cout << "  |       GET INFO      |             Print information about a certain property.            |"<< std::endl;
    std::cout << "  |·····················|····································································|" << std::endl;
    std::cout << "  |    DISPLAY DEEDS    |                Displays all the title deeds you hold.              |" << std::endl;
    std::cout << "  |·····················|····································································|" << std::endl;
    std::cout << "  | LIST MY PROPERTIES  |                  Lists all the properties you own.                 |" << std::endl;
    std::cout << "  |·····················|····································································|" << std::endl;
    std::cout << "  | LIST ALL PROPERTIES |        Lists all the properties in the game, sorted by group.      |" << std::endl;
    std::cout << "  |·····················|····································································|" << std::endl;
    std::cout << "  |    SELL PROPERTY    |             Sell one of your properties to a fellow player.        |" << std::endl;
    std::cout << "  |·····················|····································································|" << std::endl;
    std::cout << "  |      CONSTRUCT      |              Build a house/hotel at one of your properties.        |" << std::endl;
    std::cout << "  |·····················|····································································|" << std::endl;
    std::cout << "  |   CONSTRUCT COLOUR  |    Build any number of houses/hotels at once for all of the        |" << std::endl;
    std::cout << "  |                     |            properties in a colour group you fully own.             |" << std::endl;
    std::cout << "  |·····················|····································································|" << std::endl;
    std::cout << "  |    SELL BUILDING    |   Sell a house/hotel at one of your properties back to the         |" << std::endl;
    std::cout << "  |                     |                      bank for half it's price.                     |" << std::endl;
    std::cout << "  |·····················|····································································|" << std::endl;
    std::cout << "  |     SELL BUILDING   |    Sell back to the bank any number of houses/hotels at once for   |" << std::endl;
    std::cout << "  |       COLOUR        |        all of the properties in a colour group you fully own.      |" << std::endl;
    std::cout << "  |·····················|····································································|" << std::endl;
    std::cout << "  |       MORTGAGE      |                    Mortage one of your properties.                 |" << std::endl;
    std::cout << "  |·····················|····································································|" << std::endl;
    std::cout << "  |    LIFT MORTGAGE    |         Pay off one of your mortages at a 10% interest rate.       |" << std::endl;
    std::cout << "  |·····················|····································································|" << std::endl;
    std::cout << "  |     GO BANKRUPT     |      Declare bankruptcy and loose all your properties and cash.    |" << std::endl;
    std::cout << "  |                     |               You will be eliminated from the game.                |" << std::endl;
    std::cout << "  |·····················|····································································|" << std::endl;
    std::cout << "  |         ROLL        |  Rolls the dice and plays the turn. None of the above actions will |" << std::endl;
    std::cout << "  |                     |             become available until your next turn of play.         |" << std::endl;
    std::cout << "  --------------------------------------------------------------------------------------------" << std::endl;
    
    std::string command;
    std::cout << "Enter a command: ";
    std::getline (std::cin, command);
    
    // Store valid commands in a vector of strings.
    std::vector<std::string> valid_comands = {"DISPLAY BOARD","GET INFO", "DISPLAY DEEDS", "LIST MY PROPERTIES", "LIST ALL PROPERTIES",
    "SELL PROPERTY", "CONSTRUCT", "CONSTRUCT COLOUR", "SELL BUILDING", "SELL BUILDING COLOUR", "MORTGAGE", "LIFT MORTGAGE", "GO BANKRUPT", "ROLL"};
    std::vector<std::string>::iterator it = std::find(valid_comands.begin(), valid_comands.end(), command);
    
    // Verify command is valid by finding it in the vector of valid commands.
    while (it == valid_comands.end() ) {
        std::cout << "Sorry, I didn't catch that. Please try again: ";
        std::getline (std::cin, command);
        it = std::find(valid_comands.begin(), valid_comands.end(), command);
    }
    return command;
}

void sleep_2_seconds()
{
   std::this_thread::sleep_until(std::chrono::system_clock::now() + std::chrono::seconds(2));
}

// MAIN FUNCTION: Move the player and interact with the board:
void play_turn(std::shared_ptr<player> & plyr, std::vector<std::shared_ptr<player>> & players, board & play_board, std::vector<std::string> &                   player_pos, std::vector<std::shared_ptr<property>> & properties, std::vector<property_group> all_groups,                                         std::vector<std::unique_ptr<card>> & deck, int & num_players)
{
    // Set board current_player variable to current player's turn.
    board::current_player = plyr -> plyr_name();
    
    // Display player's turn and opening balance:
    std::cout << '\n';
    std::cout << plyr -> plyr_name() << "'s turn: " << std::endl;
    // Underline the above statement:
    for(unsigned i{}; i< plyr -> plyr_name().size()+8; i++) std::cout << "-";
    std::cout << '\n';
    std::cout << "Opening balance: " << plyr -> current_balance() << "$" << std::endl;
    
    // Store player position and current position within the board.
    int current_pos = plyr -> current_position();
    std::string player_abreviation = plyr -> plyr_abrev();

    // Keep requesting commands until player wishes to roll the dice.
    std::string command = get_command();
    while (command != "ROLL")
    {
        if (command == "DISPLAY BOARD")             play_board.print_board(player_pos, players);
        else if (command == "GET INFO")             get_info(properties, all_groups);
        else if (command == "DISPLAY DEEDS")        print_player_deeds(plyr, all_groups);
        else if (command == "LIST MY PROPERTIES")   list_props_held(plyr, all_groups);
        else if (command == "LIST ALL PROPERTIES")  print_all_groups(all_groups);
        else if (command == "SELL PROPERTY")        sell_property(plyr, properties, players, all_groups);
        else if (command == "CONSTRUCT")            single_property::construct(plyr, all_groups, properties);
        else if (command == "CONSTRUCT COLOUR")     full_colour_group::construct(plyr, all_groups, properties);
        else if (command == "SELL BUILDING")        single_property::sell_building(plyr, all_groups, properties);
        else if (command == "SELL BUILDING COLOUR") full_colour_group::sell_building(plyr, all_groups, properties);
        else if (command == "MORTGAGE")             mortgage_a_property(plyr, all_groups, properties);
        else if (command == "LIFT MORTGAGE")        lift_a_mortgage(plyr, all_groups, properties);
        else if (command == "GO BANKRUPT") {
            try {
                go_bankrupt(plyr, players, player_pos, num_players);
                // If no exceptions, erase player abreviation from board and terminate function.
                erase_substring(player_pos[current_pos], player_abreviation);
                return;
            }
            catch (int){std::cout << "Cancelling operation" << std::endl;}
        }
        // Between each action, wait a couple of seconds.
        sleep_2_seconds();
        command = get_command();
    }
    
    // Remove player aberviation from his current position in the vector of player positions.
    erase_substring(player_pos[current_pos], player_abreviation);
    
    // If player is in jail, call in_jail_mechnic method from player class.
    if (plyr -> in_jail) plyr -> in_jail_mechnic(players, player_pos);
    
    // Otherwise roll the dice and interact with the board:
    else {
        int roll = plyr -> dice_roll();
        
        // If player is caught speeding during dice roll and is sent to jail, end turn.
        if (plyr -> in_jail) return;
        
        int new_pos = (current_pos+roll);
        
        // If player completes a turn, recieve a salary of $200.
        if (new_pos >= 40) {
            std::cout << "You completed a turn. Collect your salary." << std::endl;
            plyr -> receive(200);
            // Perform modulo operation to get new position.
            new_pos = new_pos%40;
        }
    
        // Player may either land of a property, or in a non-property tile.
        // Store positions of non-property tiles:
        std::vector<int> non_prop_positions{0, 2, 4, 7, 10, 17, 20, 22, 30, 33, 36, 38};
        // Check whether player landed in one such tile.
        std::vector<int>::iterator it = std::find(non_prop_positions.begin(), non_prop_positions.end(), new_pos);
        // If he doesn't, it must mean he/she landed on a property:
        if (it == non_prop_positions.end() ) {
            // Print out the name of the property where player landed:
            std::shared_ptr<property> property_landed;
            try {property_landed =properties[position_to_prop_vector(new_pos)];}
            catch (int) {std::cout << "Error reading in property." << std::endl;}
        
            std::cout << "Landed on " << property_landed -> get_property_name()  <<"."<< std::endl;
            
            // Check if owned, if so, pay rent, otherwise, ask player if he wishes to purchase:
            if (!property_landed -> owned() ) {
                std::cout << "This property is currently unowned. " << std::endl;
                char yes_no;
                int cost = property_landed -> get_cost();
                std::cout << "Do you wish to purchase (Cost $" <<  cost <<  ") (y/n)? ";
                
                // Ask player if he/she wishes to purchase the property.
                std::cin >> yes_no;
                while (yes_no != 'n' && yes_no != 'y' && yes_no != 'N' && yes_no != 'Y') {
                    std::cin.clear(); std::cin.ignore();
                    std::cout << '\n' << "Sorry, I didn't catch that. Please try again: ";
                    std::cin >> yes_no;
                }
                // If so, check if they have enough funds. If not, auction property.
                if (yes_no == 'y' || yes_no == 'Y') {
                    std::cin.ignore();
                    if (plyr -> current_balance() > cost) {
                        property_landed -> purchase(plyr);
                        std::cout << plyr -> plyr_name() << " obtained the following property: " << std::endl;
                        property_landed -> print_title_deed(all_groups);
                    }
                    else {
                    std::cout << "Insufficient funds. Auctioning property..." << std::endl;
                    property_landed -> auction(players, all_groups);
                    }
                }
                else property_landed -> auction(players, all_groups);
            }
            // If property was already owned, charge rent.
            else {
                std::string owner = property_landed -> get_ownership();
                std::cout << "The property "<< property_landed -> get_property_name() << " is currently owned by: "
                                        << owner << "." << std::endl;
                if (owner != plyr -> plyr_name() ) property_landed -> charge_rent(plyr, roll, all_groups);
            }
        }
        // Now look at cases where player lands on a non-property tile:
        else
        {   // Chance and community chest:
            if (new_pos == 2 || new_pos == 17 || new_pos == 33) {
                std::cout << "Landed on a Community Chest tile." << std::endl;
                draw_card(deck, plyr, new_pos);
            }
            else if (new_pos == 7 || new_pos == 22 || new_pos == 36) {
                std::cout << "Landed on a Chance tile." << std::endl;
                draw_card(deck, plyr, new_pos);
            }
            
            // Taxes:
            else if (new_pos == 4) {
                std::cout << "Landed on income tax." << std::endl;
                plyr-> pay(200);
                board::jackpot_money += 200;
            }
            else if (new_pos == 38) {
                std::cout << "Landed on luxuary tax." << std::endl;
                plyr-> pay(100);
                board::jackpot_money += 100;
            }
        
            // Corner tiles:
            else if (new_pos == 0) std::cout  << "Landed on the GO tile." << std::endl;
            else if (new_pos == 10) std::cout << "Landed on just visiting." << std::endl;
            else if (new_pos == 20) {
                std::cout << "It's your lucky day! You landed on free parking." << std::endl;
                plyr -> receive(board::jackpot_money);
                // Reset to some standard value to ensure mimimum payout.
                board::jackpot_money = 100;
            }
            if (new_pos == 30) {
                std::cout << "Oh dear, you landed on the go to jail tile." << std::endl;
                plyr -> send_to_jail();
            }
        
        }
        // If player got sent to jail during the turn, erase it's abbreviation from board:
        if (plyr -> in_jail) erase_substring(player_pos[new_pos], player_abreviation);
    
        // Otherwise, append abbreviation to new position in vector of player positions.
        else {
            player_pos[new_pos].append(player_abreviation);
            plyr -> update_postion(new_pos);
        }
    }
    // End of turn:
    // Display closing balance:
    std::cout <<  "Closing balance: $" << plyr -> current_balance()  << std::endl;
    
    // If player ends turn with a negative balance, they are in debt.
    // Call in debt function until balance becomes positive or player goes bankrupt.
    bool negative_balance{plyr -> current_balance() < 0};
    
    if (negative_balance) {
        std::cout << '\n' << "Oh no! Your balance became negative this turn. You are in debt." << std::endl;
        std::cout << "You can sell one of your properties, buildings or mortgage a property." << std::endl;
        std::cout << "If you can't do any of the above, you will have to declare bankruptcy." << std::endl;

        // Player must input of the following commands to decide what action is taken.
        std::cout << '\n' << "What do you want to do? " << std::endl;
        std::cout << "Enter:  SELL BUILDING to sell a building. " << std::endl;
        std::cout << "        SELL PROPERTY to sell a property. " << std::endl;
        std::cout << "        MORTGAGE to mortage a property. " << std::endl;
        std::cout << "        GO BANKRUPT to declare bankruptcy." << std::endl;
    }
    
    while (negative_balance)
    {
        in_debt(plyr, all_groups, properties, players, player_pos, num_players);
        negative_balance = plyr -> current_balance() < 0 && !plyr -> bankrupt;
    }
    sleep_2_seconds();
}
