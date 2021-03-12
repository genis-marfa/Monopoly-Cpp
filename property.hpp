#include <stdio.h>
#include "player.hpp"
#include "prelim_functions.hpp"
#include "property_group.hpp"

#ifndef property_hpp
#define property_hpp

// Forward declare property group class.
class property_group;

// -------------------------------------------------------------------------------
//                              BASE CLASS:
// -------------------------------------------------------------------------------
// Each property HAS:
// - A purchase cost.
// - An owner (pointer to a player).
// - A name, broken down in two lines to display on the board tile. For example,
//      'KENTUCKY AVENUE' will be displayed as 'KENTUCKY' on the fist line in the tile
//      and 'AVENUE' on the second line. Each line can hold up to 12 characters in
//      current configuration.
// - A mortgage value and a mortgaged status.

// Each property CAN:
// - Change owner.
// - Be purchased and/or auctioned.
// - Belong to a property group.
// - Have buildings / hotels constructed / sold.
// - Charge rent to a player who lands on it's tile.
// - Be mortgaged / lift the mortgage.
// - Display it's corresponding title deed.

// Inherit from std::enable_shared_from_this to safely generate additional std::shared_ptr instances
//   of the current object (this).
class property : public std::enable_shared_from_this<property>
{
protected:
    int cost;
    std::shared_ptr<player> owner;
    std::string first_line;
    std::string second_line;
    int mortgage;

public:
    bool mortgaged{false};
    
    property(std::string first, std::string second, int cost_, int mortgage_) :
        cost{cost_}, first_line{first}, second_line{second}, mortgage{mortgage_}
    {
        owner = std::make_shared<player>(); // Call default constructor
    }
    
    // Make additional shared pointer instance to this.
    std::shared_ptr<property> get_shared_ptr_to_this() {return shared_from_this();}
    
    //      PROPERTY NAME FUNCTIONS:
    //     -------------------------
    std::string get_first_line()  {return first_line;}
    std::string get_second_line() {return second_line;}
    std::string get_property_name()
    {
        if (second_line != "") return first_line + " " + second_line;
        // If property takes only one line (e.g. BOARDWALK), return first line only.
        else return first_line;
    }
    
    //        OWNERSHIP FUNCTIONS:
    //     -------------------------
    // Returns true if the property is owned by somone, and false otherwise:
    bool owned()
    {
        std::string owner_name = owner->plyr_name();
        if (owner_name == "") return false;
        else return true;
    }
    // Returns name of the current owner, and "Unowned" if it is not currently owned:
    std::string get_ownership()
    {
        std::string current_owner = owner->plyr_name();
        if (current_owner == "") current_owner = "Unowned";
        return current_owner;
    }
    // Changes the owner from one player to another, e.g. if a player sells this property to another player.
    void change_owner(std::shared_ptr<player> & new_owner);
    
    // Auction a property between all players.
    int auction (std::vector<std::shared_ptr<player>> & players, std::vector<property_group> all_groups);
    
    //      FUNCTIONS ABOUT THE PROPERTY'S GROUP:
    //     ---------------------------------------
    // Forward declare these functions and complete them once the property_group class is defined.
    std::string get_group_string(std::vector<property_group> all_groups);
    property_group get_group(std::vector<property_group> all_groups);
    
    
    //      CONSTRUCTION FUNCTIONS:
    //    --------------------------
    // ALL declared virtual in the base clase, and overwritten in the 'street' derived class:
    virtual int get_house_price() {return 0;}       // Gets cost of constructing a house.
    virtual int get_hotel_price() {return 0;}       // Gets cost of constructing a hotel.
    virtual int get_num_buildings() {return 0;}     // Gets total number of buildings constructed.
    // Add a new building at a property, if possible
    virtual bool add_new_building(std::vector<property_group> all_groups) {return false;}
    // Add multiple buildings at a property at once.
    virtual void add_multiple_buildings(int num_buildings) {return;}
    // Sell a building at a property, if possible
    virtual bool sell_building(std::vector<property_group> all_groups) {return false;}
    // Sell multiple buildings at a property at once.
    virtual void sell_multiple_buildings(int num_buildings) {return;}
    
    //      MORTAGE FUNCTIONS:
    //    ---------------------
    // Checks wether property can be mortgaged. Can only do so if NO buildings have been constructed.
    // If it can be mortgaged, proceed to mortgaging the property and return true.
    bool mortgage_property(std::shared_ptr<player> & plyr);
    
    // Lift a mortgage by paying 10% interest rate to the bank. Again start by
    //      checking the mortgage can indeed be lifted.
    bool lift_mortage(std::shared_ptr<player> & plyr);
    
    // Return purchase cost:
    int get_cost() {return cost;}
    int get_mortgage_value() {return mortgage;}
    
    // General virtual functions to overwrite in derived classes:
    virtual void print_title_deed(std::vector<property_group> all_groups) {return;}
    virtual void purchase(std::shared_ptr<player> & plyr) {return;}
    virtual void charge_rent(std::shared_ptr<player> & plyr, int dice_roll, std::vector<property_group> all_groups) {return;}
    virtual void clear_owner() {return;}
};


#endif /* property_hpp */
