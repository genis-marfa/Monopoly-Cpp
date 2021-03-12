#include "property_group.hpp"

// Property group class:
bool property_group::in_group(std::shared_ptr<property> & prop)
{
    // Iterate through properties in group. Return true if found.
    std::vector<std::shared_ptr<property>>::iterator it = std::find(group.begin(), group.end(), prop);
    if (it != group.end()) return true;
    else return false;
}

void property_group::print_property_group()
{
   std::cout << '\n';
   std::cout << group_name << " GROUP:" << std::endl;
   std::cout << '\n';
   for (unsigned i{}; i<group.size(); i++) {
       std::vector <std::string> info
       {
           "Property name", "Owner", "Houses/Hotels built",
           "---------------", "-------",   "---------------------",
           group[i] -> get_property_name(), group[i] -> get_ownership(),
           std::to_string(group[i] -> get_num_buildings())
       };
       
       // Center the strings to linespace 23:
       std::vector <std::string> info_cent = center_strings(info, 23);
       
       if(i==0) { // Print the list headers ( Property name, Owner, Houses built) and underlines only once.
           std::cout << info_cent[0] << info_cent[1] << info_cent[2] <<  std::endl;
           std::cout << info_cent[3] << info_cent[4] << info_cent[5] << std::endl;
       }
       std::cout << info_cent[6] << info_cent[7] << info_cent[8] << std::endl;
   }
   std::cout << '\n';
}

bool property_group::owns_full_group(std::shared_ptr<player> plyr)
{
    for (unsigned i{}; i<group.size(); i++) {
        if (group[i] -> get_ownership() != plyr -> plyr_name() ) return false;
    }
    return true;
}

bool property_group::construct_evenly(std::shared_ptr<property> & prop)
{
    // Store number of buildings standing at the prop. we wish to construct at
    int num_builds_input_prop = prop -> get_num_buildings();
    
    // Iterate through group, check construct evenly condition.
    for (unsigned i{}; i<group.size(); i++) {
        int num_builds = group[i] -> get_num_buildings();
        if (num_builds_input_prop > num_builds) return false;
    }
    return true;
}

bool property_group::sell_evenly(std::shared_ptr<property> & prop)
{
    int num_builds_input_prop = prop -> get_num_buildings();
    for (unsigned i{}; i<group.size(); i++) {
        int num_builds = group[i] -> get_num_buildings();
        if (num_builds_input_prop < num_builds) return false;
    }
    return true;
}

void property_group::construct_across_all_group(unsigned num_houses, std::vector<property_group> all_groups)
{
    std::cout << "This operation will cost $" << get_construction_price(num_houses) << "." << std::endl;
    char yes_no;
    std::cout << "Are you sure? (y/n) ";
    std::cin >> yes_no;
    while (yes_no != 'y' && yes_no != 'Y' && yes_no != 'n' && yes_no != 'N') {
        std::cin.clear(); std::cin.ignore();
        std::cout << "I didn't catch that. Please try again: ";
        std::cin >> yes_no;
    }
    std::cin.ignore();
    
    if (yes_no == 'Y' || yes_no == 'y') {
        for (unsigned i{}; i<group.size(); i++) group[i] -> add_multiple_buildings(num_houses);
    }
}

int property_group::sell_across_all_group(unsigned num_houses, std::vector<property_group> all_groups)
{
    int sale_value = 0.5 * group.size() * num_houses * group[0] -> get_house_price();
    for (unsigned i{}; i<group.size(); i++) group[i] -> sell_multiple_buildings(num_houses);
    return sale_value;
}

bool property_group::is_constructed()
{
    for (unsigned i{}; i<group.size(); i++) {
        if (group[i] -> get_num_buildings() > 0) return true;
    }
    return false;
}

int property_group::max_number_of_buildings()
{
    int max_num{};
    for (size_t i{}; i<group.size(); i++) {
        int buidlings_constructed = group[i] -> get_num_buildings();
        if (buidlings_constructed > max_num) max_num=buidlings_constructed;
    }
    return max_num;
}

int property_group::min_number_of_buildings()
{
    int min_num{5};
    for (size_t i{}; i<group.size(); i++) {
        int buidlings_constructed = group[i] -> get_num_buildings();
        if (buidlings_constructed < min_num ) min_num=buidlings_constructed;
    }
    return min_num;
}
// ------------------------------------------------------------------------

// Go back and complete functions forward declared in the property base class:

// Returns the string contiaing the property's group name.
std::string property::get_group_string(std::vector<property_group> all_groups)
{
    // Search for the property within the group:
    std::shared_ptr<property> shared_ptr_to_this = get_shared_ptr_to_this();
    
    for (unsigned i{}; i<all_groups.size(); i++) {
        if (all_groups[i].in_group(shared_ptr_to_this)) return all_groups[i].get_group_name();
    }
    return "Ungrouped";
}
// Returns the group object contiaing the property.
property_group property::get_group(std::vector<property_group> all_groups)
{
    std::shared_ptr<property> shared_ptr_to_this = get_shared_ptr_to_this();
    
    // Search for the property within the group:
    for (unsigned i{}; i<all_groups.size(); i++) {
        if (all_groups[i].in_group(shared_ptr_to_this)) return all_groups[i];
    }
    
    // If it doesn't find the group, throw an excpetion.
    throw 1;
}
