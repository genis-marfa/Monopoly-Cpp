#include "street.hpp"

street::street (std::string first, std::string second, int cost_, int rent_price_, int one_house, int two_houses,
                int three_houses, int four_houses, int hotel, int mortgage_, int house_cost_, int hotel_cost_) :
    property{first, second, cost_, mortgage_}, // Call base class constructor.
    rent_price{rent_price_},
    rent_one_house{one_house},
    rent_two_houses{two_houses},
    rent_three_houses{three_houses},
    rent_four_houses{four_houses},
    rent_with_hotel{hotel},
    house_cost{house_cost_},
    hotel_cost{hotel_cost_}
    {}// std::cout << "Derived Constructor Called" << std::endl;}

void street::print_title_deed(std::vector<property_group> all_groups)
{
    std::vector<std::string> title = {"Title Deed", first_line+" "+second_line, "Group: "+get_group_string(all_groups)};
    std::vector<std::string> title_cent = center_strings(title, 25);
    
    std::vector<std::string> data{10};
    data[0] = " Price: " + std::to_string(cost)+"$";
    data[1] = " Rent: " + std::to_string(rent_price)+"$";
    data[2] = " - With 1 house: " + std::to_string(rent_one_house)+"$";
    data[3] = " - With 2 houses: " + std::to_string(rent_two_houses)+"$";
    data[4] = " - With 3 houses: " + std::to_string(rent_three_houses)+"$";
    data[5] = " - With 4 houses: " + std::to_string(rent_four_houses)+"$";
    data[6] = " - With hotel: " + std::to_string(rent_with_hotel)+"$";
    data[7] = " Mortgage Value: " +  std::to_string(mortgage)+"$";
    data[8] = " Each house costs: " +  std::to_string(house_cost)+"$";
    data[9] = " Hotel cost: " + std::to_string(hotel_cost)+"$";
    
    std::cout << "   ---------------------------" << std::endl;
    std::cout << "   |"+title_cent[0]+"|" << std::endl;
    std::cout << "   |"+title_cent[1]+"|" << std::endl;
    std::cout << "   |"+title_cent[2]+"|" << std::endl;
    std::cout << "   |.........................|" << std::endl;
    
    for (unsigned i{}; i<data.size(); i++) {
        size_t white_sp = 25-data[i].size();
        std::cout << "   |" + data[i];
        for (unsigned j{}; j<white_sp; j++) {
            std::cout << " ";
        }
        std::cout << "|" << std::endl;
    }
    std::cout << "   ---------------------------" << std::endl;
}
void street::purchase(std::shared_ptr<player> & plyr)
{
    if (owner->plyr_name() == "") {
        std::shared_ptr<property> ptr_to_this = get_shared_ptr_to_this();
        plyr -> pay(cost);
        plyr -> properties_owned.push_back(std::move(ptr_to_this));
        owner = plyr;
    }
    else  std::cout << "Sorry, this property is already owned by " << owner->plyr_name() << "." << std::endl;
}

void street::charge_rent(std::shared_ptr<player> & plyr, int dice_roll, std::vector<property_group> all_groups)
{
    if (mortgaged) {
        std::cout << "This property is mortgaged." << owner -> plyr_name() << " will not received any rent" << std::endl;
        std::cout << "until they lift the mortgage" << std::endl;
        return;
    }
    
    if (num_buildings_constructed == 0) {
        // Get the colour group.
        property_group colour("");
        try {colour = get_group(all_groups);}
        catch (int) {
            std::cerr << "Fatal error. Group not found!" << std::endl;
            std::cerr << "Terminating process." << std::endl;
            return;
        }
        
        // Double rent is charged if a player owns the full group.
        if (colour.owns_full_group(owner)){
            std::cout << owner -> plyr_name() << " owns the full colour group. Charging double rent." << std::endl;
            plyr -> pay(2*rent_price);
            owner -> receive(2*rent_price);
        }
        else {
            plyr -> pay(rent_price);
            owner -> receive(rent_price);
        }
    }
    else if (num_buildings_constructed == 1) {
        std::cout <<  owner -> plyr_name() << " has a house constructed here." << std::endl;
        plyr -> pay(rent_one_house);
        owner -> receive(rent_one_house);
    }
    else if (num_buildings_constructed == 2) {
        std::cout << owner -> plyr_name() << " has two houses constructed here." << std::endl;
        plyr -> pay(rent_two_houses);
        owner -> receive(rent_two_houses);
    }
    else if (num_buildings_constructed == 3) {
        std::cout << owner -> plyr_name() << " has three houses constructed here." << std::endl;
        plyr -> pay(rent_three_houses);
        owner -> receive(rent_three_houses);
    }
    else if (num_buildings_constructed == 4) {
        std::cout << owner -> plyr_name() << " has four houses constructed here." << std::endl;
        plyr -> pay(rent_four_houses);
        owner -> receive(rent_four_houses);
    }
    else if (num_buildings_constructed == 5) {
        std::cout << owner -> plyr_name() << " has a hotel constructed here." << std::endl;
        plyr -> pay(rent_with_hotel);
        owner -> receive(rent_with_hotel);
    }
}

bool street::add_new_building(std::vector<property_group> all_groups)
{
    if (num_buildings_constructed < 5) {
        // Get the colour group.
        property_group colour("");
        try {colour = get_group(all_groups);}
        catch (int) {
            std::cerr << "Fatal error. Group not found!" << std::endl;
            std::cerr << "Terminating process." << std::endl;
            return false;
        }
        std::shared_ptr<property> ptr_to_this = get_shared_ptr_to_this();
        
        // Verify the construction is allowed.
        if (colour.construct_evenly(ptr_to_this) ) {
            num_buildings_constructed += 1;
            std::cout << get_property_name() << " now has " << num_buildings_constructed << " houses constructed." << std::endl;
            return true;
        }
        else {
            std::cout << "You must construct evenly." << std::endl;
            return false;
        }
    }
    else {
        std::cout << "You already have 5 buildings here." << std::endl;
        return false;
    }
}

void street::add_multiple_buildings(int num_buildings)
{
    num_buildings_constructed += num_buildings;
    std::cout << get_property_name() << " now has " << num_buildings_constructed << " buildings constructed." << '\n';
}

bool street::sell_building(std::vector<property_group> all_groups)
{
    std::shared_ptr<property> ptr_to_this = get_shared_ptr_to_this();
    if (num_buildings_constructed > 0) {
        
        // Get the colour group. No default constructor defined so initialise group name
        //      with empty string.
        property_group colour("");
        try {colour = get_group(all_groups);}
        catch (int) {
            std::cerr << "Fatal error. Group not found!" << std::endl;
            std::cerr << "Terminating process." << std::endl;
            return false;
        }
        
        // Verify the sale is valid.
        if (colour.sell_evenly(ptr_to_this) ) {
            num_buildings_constructed -= 1;
            std::cout << get_property_name() << " now has " << num_buildings_constructed << " houses constructed." << std::endl;
            return true;
        }
        else {
            std::cout << "You must sell buildings evenly." << std::endl;
            return false;
        }
    }
    else {
        std::cout << "You have no buildings here." << std::endl;
        return false;
    }
}

void street::sell_multiple_buildings(int num_buildings)
{
    num_buildings_constructed -= num_buildings;
    std::cout << get_property_name() << " now has " << num_buildings_constructed << " buildings constructed." << '\n';
}

void street::clear_owner()
{
    mortgaged = false;
    num_buildings_constructed = 0;
    owner = std::make_shared<player>(); // Call default constructor
    std::cout << get_property_name() << " is now " << get_ownership() << std::endl;
    // In the above line, the get_ownership() method should return "Unowned" by definition.
}
