#ifndef property_group_hpp
#define property_group_hpp

#include <stdio.h>
#include "property.hpp"

// -------------------------------------------------------------------------------
//                              PROPERTY GROUP:
// -------------------------------------------------------------------------------
// Property group class:
// Each group as a name and a vector of properties containted in that group.

class property_group
{
private:
    std::string group_name;
    std::vector<std::shared_ptr<property>> group;

public:
    property_group(std::string name) {group_name = name;}
    std::string get_group_name() {return group_name;}
    void add_to_property_group(std::shared_ptr<property> & prop) {group.push_back(prop);}
    size_t get_group_size() {return group.size();}
    
    // Checks wether a property is in that group.
    bool in_group (std::shared_ptr<property> & prop);
    
    // Prints all the properties in the group, along with a few features: Owner, buildings constructed, etc.
    void print_property_group();
    
    //      FUNCTIONS FOR INDIVIDUAL CONSTRUCTION:
    //     ----------------------------------------
    // Ckeck if a player owns the entire group (of streets). Only then will he be
    //      able to construct houses / hotels.
    bool owns_full_group(std::shared_ptr<player> plyr);
    
    // Checks wether a contruction is valid: Can only add a new building at a property if
    //  the number of buildings constructed in all the other properties in the group is equal,
    //  greater by one, or smaller by one to the number of buildings here.
    bool construct_evenly(std::shared_ptr<property> & prop);
    
    // Checks whether selling a property is valid. Again, this must be done evenly along the group.
    bool sell_evenly(std::shared_ptr<property> & prop);
    
    //       FUNCTIONS FOR MULTIPLE CONTRUCTION:
    //      -------------------------------------
    // Say we want to construct n buildings in ALL of the properties in a group, at once.
    // This will always be a valid operation since it fulfills the "construct evenly" condition.
    // Need a few functions:
    
    // Get the total cost:
    size_t get_construction_price(unsigned num_houses)
    {
        return num_houses * group.size()*(group[0] -> get_house_price());
    }
    
    // Construct num_houses on each of the properties of the group:
    void construct_across_all_group(unsigned num_houses, std::vector<property_group> all_groups);
    
    // In a similar way, we might want to sell n houses all at once.
    int sell_across_all_group(unsigned num_houses, std::vector<property_group> all_groups);
    
    //      ADDITIONAL FUNCTIONS ABOUT NUMBER OF HOUSES CONSTRUCTED:
    //     ------------------------------------------------------------
    // Checks wether any of the properties in a group has a building.
    // If this is the case. The properties in that group cannot be sold.
    bool is_constructed();
    
    // Get the maximum number of buildings constructed in any one property within the group.
    int max_number_of_buildings();
    
    // Get the minimum number of buildings constructed in any one property within the group.
    int min_number_of_buildings();
};


#endif /* property_group_hpp */
