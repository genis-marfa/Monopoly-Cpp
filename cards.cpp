# include "cards.hpp"

void to_jail_card::draw(std::shared_ptr<player> & plyr, int &new_pos)
{
   std::cout << '\n' << "Card draw output:" << std::endl;
   std::cout << "Oh no! You drew a go to jail card." << std::endl;
   plyr -> send_to_jail(); // Send player to jail.
}

void out_of_jail_card::draw(std::shared_ptr<player> & plyr, int &new_pos)
{
    std::cout << '\n' << "Card draw output:" << std::endl;
    std::cout << "You drew a get out of jail card!." << std::endl;
    std::cout << "Use this card to get out of jail." << std::endl;
    plyr -> add_get_out_of_jail_card(); // Add a new get_out_of_jail_card to player counter.
}

void tax_or_dividend_card::draw(std::shared_ptr<player> & plyr, int &new_pos)
{
    std::cout << '\n' << "Card draw output:" << std::endl;
    // Draw random number 0 or 1: If 1, pay tax, if 0 recieve dividends.
    
    int n = rng(2);
    // Amount recieved / paid will be a random number multiple of 50$ (up to 200):
    int amount = (rng(4) + 1) * 50;
    
    if (n==0) {
        std::cout << "The bank pays you dividends. " << std::endl;
        plyr -> receive(amount);
    }
    else {
        std::cout << "Pay poor tax of " << amount << "$" << std::endl;
        plyr -> pay(amount);
        board::jackpot_money += amount;
    }
}

void go_tile_card::draw(std::shared_ptr<player> & plyr, int &new_pos)
{
    std::cout << '\n' << "Card draw output:" << std::endl;
    std::cout << "Advance to the GO tile and collect 200$." << std::endl;
    new_pos = 0;          // set's player's position to 0.
    plyr -> receive(200); // Recieve salary.
}

