#ifndef street_hpp
#define street_hpp

#include <stdio.h>
#include "property_group.hpp"

// -------------------------------------------------------------------------------
//                         STREET UTILITY CLASS:
// -------------------------------------------------------------------------------
// First of the derived classes: Streets. Houses can be constructed here.

// Methods that must be overwritten:
// - get_house_price(): Gets cost of constructing a house.
// - get_hotel_price(): Gets cost of constructing a hotel.
// - get_num_buildings(): Gets total number of buildings constructed.
// - print_print_title_deed() : Print the property's title deed.
// - purchase(): Property gets purchase by a player.
// - charge_rent(): Charge rent depending of number of buildings constructed.
// - add_new_building(): Increment the number of buildings standing at the property.
// - add_multiple_buildings(): Increment the number of buildings standing at the property
//                              by a certain number at once.
// - sell_building(): Decrease the number of buildings standing at the property.
// - sell_multiple_buildings(): Decrease the number of buildings standing at the property
//                              by a certain number at once.
// - clear_owner(): Make the property become unowned again. (reset it to default values).

class street : public property
{
private:
    int rent_price;
    int rent_one_house;
    int rent_two_houses;
    int rent_three_houses;
    int rent_four_houses;
    int rent_with_hotel;
    int house_cost;
    int hotel_cost;
    int num_buildings_constructed{0};
 
public:
    street (std::string first, std::string second, int cost_, int rent_price_, int one_house, int two_houses,
            int three_houses, int four_houses, int hotel, int mortgage_, int house_cost_, int hotel_cost_);
    
    void print_title_deed(std::vector<property_group> all_groups);
    void purchase(std::shared_ptr<player> & plyr);
    void charge_rent(std::shared_ptr<player> & plyr, int dice_roll, std::vector<property_group> all_groups);
    bool add_new_building(std::vector<property_group> all_groups);
    void add_multiple_buildings(int num_buildings);
    bool sell_building(std::vector<property_group> all_groups);
    void sell_multiple_buildings(int num_buildings);
    void clear_owner();
    
    // Additional functions:
    int get_num_buildings() {return num_buildings_constructed;}
    int get_house_price()   {return house_cost;}
    int get_hotel_price()   {return hotel_cost;}
    
    ~ street() {};
};



#endif /* street_hpp */
