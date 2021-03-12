#include "railroad.hpp"

// Railroad derived class:
void railroad::print_title_deed(std::vector<property_group> all_groups)
{
   std::vector<std::string> title = {"Title Deed", first_line+" "+second_line, "Group: "+get_group_string(all_groups)};
   std::vector<std::string> title_cent = center_strings(title, 32);
   
   std::vector<std::string> data{6};
   data[0] = " Price: " + std::to_string(cost)+"$";
   data[1] = " Rent: " + std::to_string(rent_one_RR)+"$";
   data[2] = " - If two RR are owned: " + std::to_string(rent_two_RR)+"$";
   data[3] = " - If three RR are owned: " + std::to_string(rent_three_RR)+"$";
   data[4] = " - If four RR are owned: " + std::to_string(rent_four_RR)+"$";
   data[5] = " Mortgage value: " + std::to_string(mortgage) +"$";
   
   std::cout << "   ----------------------------------" << std::endl;
   std::cout << "   |"+title_cent[0]+"|" << std::endl;
   std::cout << "   |"+title_cent[1]+"|" << std::endl;
   std::cout << "   |"+title_cent[2]+"|" << std::endl;
   std::cout << "   |................................|" << std::endl;
       
   for (unsigned i{}; i<data.size(); i++) {
       unsigned long white_sp = 32-data[i].size();
       std::cout << "   |" + data[i];
       for (unsigned j{}; j<white_sp; j++) {
           std::cout << " ";
       }
       std::cout << "|" << std::endl;
   }
   std::cout << "   ----------------------------------" << std::endl;
}

void railroad::purchase(std::shared_ptr<player> & plyr)
{
    if (owner->plyr_name() == "") {
        std::shared_ptr<property> ptr_to_this = get_shared_ptr_to_this();
        plyr -> pay(cost);
        plyr -> properties_owned.push_back(std::move(ptr_to_this));
        owner = plyr;
        plyr -> add_new_railroad();
        // Rent is charged according to the number of railroads owned by the player.
    }
    else std::cout << "Sorry, this property is already owned by " << owner->plyr_name() << "." << std::endl;
}

void railroad::charge_rent(std::shared_ptr<player> & plyr, int dice_roll, std::vector<property_group> all_groups)
{
    if (mortgaged) {
        std::cout << "This property is morgaged." << owner -> plyr_name() << " will not received any rent" << std::endl;
        std::cout << "until he lifts the mortgage" << std::endl;
        return;
    }

    int num_rr_owned = owner -> railroads_owned_by_player();
    
    if (num_rr_owned == 1){
        plyr -> pay(rent_one_RR);
        owner -> receive(rent_one_RR);
    }
    else if (num_rr_owned == 2){
        plyr -> pay(rent_two_RR);
        owner -> receive(rent_two_RR);
    }
    else if (num_rr_owned == 3){
        plyr -> pay(rent_three_RR);
        owner -> receive(rent_three_RR);
    }
    else if (num_rr_owned == 4){
        plyr -> pay(rent_four_RR);
        owner -> receive(rent_four_RR);
    }
}
void railroad::clear_owner()
{
    mortgaged = false;
    owner = std::make_shared<player>(); // Call default constructor
    std::cout << get_property_name() << " is now " << get_ownership() << std::endl;
    // In the above line, the get_ownership() method should return "Unowned" by definition.
}
