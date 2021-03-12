#ifndef railroad_hpp
#define railroad_hpp

#include <stdio.h>
#include "property_group.hpp"

// -------------------------------------------------------------------------------
//                         RAILROAD UTILITY CLASS:
// -------------------------------------------------------------------------------
// Second of the derived classes: Railiroads. Houses cannot be constructed here.
// Hence, the only methods that need overwritting are:
// - print_print_title_deed(): Print the property's title deed.
// - purchase(): Property gets purchase by a player.
// - charge_rent(): Charge rent depending of number of buildings constructed.
// - clear_owner(): Make the property become unowned again. (reset it to default).

class railroad : public property
{
private:
    int rent_one_RR = 25;
    int rent_two_RR = 50;
    int rent_three_RR = 100;
    int rent_four_RR = 200;

public:
    railroad(std::string first) :  property{first,"RAILROAD", 200, 100} {}
    void print_title_deed(std::vector<property_group> all_groups);
    void purchase(std::shared_ptr<player> & plyr);
    void charge_rent(std::shared_ptr<player> & plyr, int dice_roll, std::vector<property_group> all_groups);
    void clear_owner();
    ~railroad() {};
};

#endif /* railroad_hpp */
