#include "property.hpp"

// Property base class;
void property::change_owner(std::shared_ptr<player> & new_owner)
{
    std::shared_ptr<property> shared_to_this = get_shared_ptr_to_this();
    // Pointer deleted after function goes out of scope.
    
    // If the property was previously owned by someone, remove it from the old owner's vector of properties owned:
    if (this -> get_ownership() != "Unowned") {
        std::vector<std::shared_ptr<property>>::iterator it;
        it = std::find(owner->properties_owned.begin(), owner->properties_owned.end(), shared_to_this);
        size_t index = std::distance(owner->properties_owned.begin(), it); // Gets index within vector of properties.
        owner->properties_owned.erase (owner->properties_owned.begin()+index);
    }
    // Add to the new owner's vector of owned properties:
    new_owner -> properties_owned.push_back(shared_to_this);
    owner = new_owner;
    std::cout << new_owner -> plyr_name() << " is now the new owner of " << get_property_name() << std::endl;
}

int property::auction (std::vector<std::shared_ptr<player>> & players, std::vector<property_group> all_groups)
{
    std::cout << '\n' << "STARTING AUCTION FOR THE FOLLOWING PROPERTY:" << '\n' << std::endl;
    print_title_deed(all_groups);
    std::cout << '\n';
    
    // Declare a pointer to the higherst bidder (player)
    std::shared_ptr<player> highest_bidder{players[0]}; // Initialise to some random player.
    int highest_bid {0};
    
    // First round of offers: Loop over all players and ask them to make an offer. Keep track of highest bid and bidder.
    std::cout << "Starting first round of bidding. Everyone gets to make an initial offer." << std::endl;
    for (unsigned i{}; i<players.size(); i++) {
        int offer;
        std::cout << players[i] -> plyr_name() << ", make an offer: $";
        std::cin >> offer;
        while (offer < 0 || !std::cin.good()) {
            std::cin.clear(); std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cout<< "Invalid offer, make an offer: $";
            std::cin >> offer;
        }
        if (offer > players[i] -> current_balance() ) {std::cout << "Invalid offer. Insufficient funds." << std::endl; offer = 0;}
        if (offer > highest_bid) {
            highest_bid = offer;
            highest_bidder = players[i];
        }
    }
    std::cout << '\n';
    
    // Start second round. Create a boolean variable that will only get set to true
    //      whenever all players reject making a better offer.
    bool bid_accepted{false};
    while (!bid_accepted) {
        for (int i{0}; i<players.size(); i++)
        {
            // Display highest bid / bidder.
            if (i==0) std::cout << highest_bidder -> plyr_name() << " currently holds the highest bid ($" << highest_bid << ")." << std::endl;
            // Request for a higher offer: Ignore current holder of highest bid.
            if (highest_bidder == players[i] ) continue;
            // Ignore players who can't afford making a better offer.
            else if (players[i] -> current_balance() < highest_bid) continue;
            else {
                char yes_no;
                std::cout << players[i] -> plyr_name() << ", do you want to make a new offer? (y/n) ";
                std::cin >> yes_no;
                while (yes_no != 'y' && yes_no != 'Y' && yes_no != 'n' && yes_no != 'N'){
                   std::cin.clear(); std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                   std::cout << "Sorry I didn't catch that: ";
                   std::cin >> yes_no;
                }
                if (yes_no == 'y' || yes_no == 'Y') {
                    int new_offer;
                    std::cout << "Make your new offer: $";
                    std::cin >> new_offer;
                    while (new_offer < 0 || !std::cin.good()) {
                        std::cin.clear(); std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                        std::cout<< "Invalid offer, make an offer: $";
                        std::cin >> new_offer;
                    }
                    while (new_offer > players[i] -> current_balance() ) {
                        std::cout << "Insufficient funds. Make a new offer: ";
                        std::cin >> new_offer;
                    }
                    while (new_offer < highest_bid) {
                        std::cout << "This offer is less than the current highest bid ($" << highest_bid << "). Make a new offer: ";
                        std::cin >> new_offer;
                    }
                    // If the offer is valid, update highest bid / bidder.
                    highest_bidder = players[i];
                    highest_bid = new_offer;
                    i = -1; // Restart for loop and go back to step 2.
                }
            }
        }
        // If nobody wants to make a better offer. Terminate loop. Highest bidder wins the auction.
        bid_accepted = true;
    }
    std::cout << '\n';
    std::cout << highest_bidder -> plyr_name() << " won the auction for " << get_property_name() << "." << std::endl;
    highest_bidder -> pay(highest_bid);
    change_owner(highest_bidder);
    std::cin.ignore();
    return highest_bid;
}

bool property::mortgage_property(std::shared_ptr<player> & plyr)
{
    // Check player owns the property.
    if (plyr -> plyr_name() != owner -> plyr_name()) {
        std::cout << "You don't own this property." << std::endl;
        return false;
    }
    // Checks it is not already mortgaged.
    else if (mortgaged) {
        std::cout << "This property is already mortgaged." << std::endl;
        return false;
    }
    // Check no buildings standing.
    else if (get_num_buildings() > 0) {
        std::cout << "This property has buildings standing." << std::endl;
        std::cout << "You must first sell all buildings." << std::endl;
        return false;
    }
    else {
        owner -> receive(mortgage);
        mortgaged = true;
        std::cout << get_property_name() << " is now mortgaged." << std::endl;
        return true;
    }
}

bool property::lift_mortage(std::shared_ptr<player> & plyr)
{
    if (plyr -> plyr_name() != owner -> plyr_name()) {
        std::cout << "You don't own this property." << std::endl;
        return false;
    }
    else if (!mortgaged) {
        std::cout << "This property is not mortgaged." << std::endl;
        return false;
    }
    else if (plyr -> current_balance() < 1.1*mortgage) {
        std::cout << "Insufficient funds" << std::endl;
        return false;
    }
    else {
        plyr -> pay(1.1*mortgage);
        mortgaged = false;
        std::cout << "You succesfully lifted the mortgage at " << get_property_name() << "." << std::endl;
        return true;
    }
}
