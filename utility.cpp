#include "utility.hpp"

// Utility derived class:
void utility::print_title_deed(std::vector<property_group> all_groups)
{
    std::vector<std::string> title = {"Title Deed", first_line+" "+second_line, "Group: "+get_group_string(all_groups)};
    std::vector<std::string> title_cent = center_strings(title, 28);

    std::cout << "   ------------------------------" << std::endl;
    std::cout << "   |"+title_cent[0]+"|" << std::endl;
    std::cout << "   |"+title_cent[1]+"|" << std::endl;
    std::cout << "   |"+title_cent[2]+"|" << std::endl;
    std::cout << "   |............................|" << std::endl;
    std::cout << "   |   If one utility is owned, |" << std::endl;
    std::cout << "   |      rent is "<< dice_multiplier_1 << "x amount     |" << std::endl;
    std::cout << "   |        shown on dice.      |" << std::endl;
    std::cout << "   |                            |" << std::endl;
    std::cout << "   |If both utilities are owned,|" << std::endl;
    std::cout << "   |      rent is "<< dice_multiplier_2 << "x amount    |" << std::endl;
    std::cout << "   |        shown on dice.      |" << std::endl;
    std::cout << "   ------------------------------" << std::endl;
}
void utility::purchase(std::shared_ptr<player> & plyr)
{
    if (owner->plyr_name() == "") {
        std::shared_ptr<property> ptr_to_this = get_shared_ptr_to_this();
        plyr -> pay(cost);
        plyr -> properties_owned.push_back(std::move(ptr_to_this));
        owner = plyr;
        owner -> add_new_utility();
    }
    else std::cout << "Sorry, this property is already owned by " << owner->plyr_name() << "." << std::endl;
}
void utility::charge_rent(std::shared_ptr<player> & plyr, int dice_roll, std::vector<property_group> all_groups)
{
    if (mortgaged){
        std::cout << "This property is morgaged." << owner -> plyr_name() << " will not received any rent" << std::endl;
        std::cout << "until he lifts the mortgage" << std::endl;
        return;
    }
    int num_ut_owned = owner -> utilities_owned_by_player();
    if (num_ut_owned == 1){
        plyr -> pay(dice_roll * dice_multiplier_1);
        owner -> receive(dice_roll * dice_multiplier_1);
    }
    else if (num_ut_owned == 2){
        plyr -> pay(dice_roll * dice_multiplier_2);
        owner -> receive(dice_roll * dice_multiplier_2);
    }
}
void utility::clear_owner()
{
   mortgaged = false;
   owner = std::make_shared<player>(); // Call default constructor
   std::cout << get_property_name() << " is now " << get_ownership() << std::endl;
   // In the above line, the get_ownership() method should return "Unowned" by definition.
}
