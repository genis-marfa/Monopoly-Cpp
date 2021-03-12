# include <stdio.h>
# include "board.hpp"

// Here an interface is created for the different card objects in the game.
// Both chance and community chest cards will be treated equally.
// 4 different types of cards will be considered: (otherwise would be very time consiming):
//   1. Go to jail card: Send the player to jail.
//   2. Get out of jail card: Can be used to get freed from jail.
//   3. Pay Tax or Recieve dividends:
//   4. Go tile card: Advances player to the GO tile and collects salary.
// Use an interface to store card. Each type of card will be a derived class.
// Each card will have a draw probability (e.g. go to jail cards 10%, tax/dividends cards 40%).
// Each card has a method draw() which implements it's corresponding action.

#ifndef cards_hpp
#define cards_hpp

// Interface:
class card
{
    public:
        virtual void draw(std::shared_ptr<player> & plyr, int & new_pos) = 0;
        virtual int get_probab() = 0; // will be overwritten to return the draw probability.
        virtual ~card() {};
};

class to_jail_card : public card
{
    private:
        int draw_propability = 10;

    public:
        to_jail_card() {};
        int get_probab() {return draw_propability;}
        void draw(std::shared_ptr<player> & plyr, int & new_pos);
        ~to_jail_card() {};
};

class out_of_jail_card : public card
{
    private:
        int draw_propability = 20;
    
    public:
        out_of_jail_card() {};
        int get_probab() {return draw_propability;}
        void draw(std::shared_ptr<player> & plyr, int & new_pos);
        ~out_of_jail_card() {};
};

class tax_or_dividend_card : public card
{
    private:
        int draw_propability = 40;
    
    public:
        tax_or_dividend_card() {};
        int get_probab() {return draw_propability;}
        void draw(std::shared_ptr<player> & plyr, int & new_pos);
        ~tax_or_dividend_card() {};
};

class go_tile_card : public card
{
    private:
        int draw_propability = 30;
    
    public:
        go_tile_card () {};
        int get_probab() {return draw_propability;}
        void draw(std::shared_ptr<player> & plyr, int & new_pos);
        ~go_tile_card() {};
};

#endif /* cards_hpp */
