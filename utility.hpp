#ifndef utility_hpp
#define utility_hpp

#include <stdio.h>
#include "property_group.hpp"

// -------------------------------------------------------------------------------
//                         DERIVED UTILITY CLASS:
// -------------------------------------------------------------------------------
// Third of the derived classes: Utilities. Agian, houses cannot be constructed here.
// Hence, the only methods that need overwritting are:
// - print_print_title_deed() : Print the property's title deed.
// - purchase(): Property gets purchase by a player.
// - charge_rent(): Charge rent depending of number of buildings constructed.
// - clear_owner(): Make the property become unowned again. (reset it to default).

class utility : public property
{
private:
    // Rent amount charged is a multiple of the dice roll output.
    int dice_multiplier_1 = 4;      // If one utility is owned, rent is 4x dice value.
    int dice_multiplier_2 = 10;     // If two utilities are owned, it is 10x.

public:
    utility(std::string first, std::string second) :  property{first, second, 150, 100} {}
    void print_title_deed(std::vector<property_group> all_groups);
    void purchase(std::shared_ptr<player> & plyr);
    void charge_rent(std::shared_ptr<player> & plyr, int dice_roll, std::vector<property_group> all_groups);
    void clear_owner();
    ~ utility() {};
};

#endif /* utility_hpp */
