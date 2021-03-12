#include "player.hpp"
#include "prelim_functions.hpp"


void player::place (std::vector<std::string> & player_pos, int pos)
{
    erase_substring(player_pos[current_pos], player_abreviation);
    // Use append in case there already is a player there.
    player_pos[pos].append(player_abreviation);
    current_pos = pos;
}

void player::send_to_jail()
{
    std::cout << "You were sent to jail." << std::endl;
    in_jail = true; // Sets jail status to true.
    for (int i{}; i<4; i++) {
        // Search for the next "empty slot" in the vector players_in_jail, and asign it to the player's name.
        if (players_in_jail[i] == "") {
            players_in_jail[i] = player_name+" ";
            break;
        }
    }
}

void player::free_from_jail(std::vector<std::string> & player_pos)
{
    std::cout << "You were freed from jail and placed on the just visiting tile." << std::endl;
    in_jail = false; // Sets jail status to false.
    place(player_pos, 10); // Place on just visiting.
    
    // Remove the player's name from the static vector players_in_jail.
    for (unsigned i{}; i<players_in_jail.size(); i++) {
        if (players_in_jail[i] == player_name+" ") players_in_jail[i]="";
    }
}

// In jail mechanic: This method will be triggered at the start of every turn while the player is in jail.
void player::in_jail_mechnic(std::vector<std::shared_ptr<player>> & players, std::vector<std::string> & player_pos)
{
   // In the standard game of Monopoly there are 4 ways to get out of jail:
   //  1. Using a get out of jail card of your own.
   //  2. Purchasing a card from one of the other players, and using it.
   //  3. Rolling doubles on any of your next three turns.
   //  4. Paying the $50 fee.
   
   std::cout << "You are currently in jail." << std::endl;
   std::cout << "There are 4 different ways to get out: " << std::endl;
   std::cout << "  1. Using a get out of jail card." << std::endl;
   std::cout << "  2. Purchasing a get out of jail card from a fellow player." << std::endl;
   std::cout << "  3. Rolling doubles on any of your next three turns." << std::endl;
   std::cout << "  4. Paying the $50 fee. " << '\n' << std::endl;
   
   // First method: Using one of your own get_out_of_jail_cards:
   std::cout << "You currently own " << get_out_of_jail_cards << " get out of jail cards." << std::endl;
   if (get_out_of_jail_cards > 0) {
       // Ask user whether he wants to use one:
       char yes_no;
       std::cout << "Do you want to use one? (y/n) ";
       std::cin >> yes_no;
    
       // Check for bad inputs:
       while (yes_no != 'n' && yes_no != 'y' && yes_no != 'N' && yes_no != 'Y') {
           std::cin.clear(); std::cin.ignore();
           std::cout << '\n' << "Sorry, I didn't catch that. Please try again: ";
           std::cin >> yes_no;
       }
                  
       if (yes_no == 'y' || yes_no == 'Y') {
           loose_get_out_of_jail_card();
           free_from_jail(player_pos);
       }
   }

   // Second method: Purchasing a card from a fellow player:
   if (in_jail)  {
       std::cout << "Checking get out of jail cards of fellow players..." << std::endl;
       
       // Iterate over all other players, get the number of cards they own.
       for (auto ptr : players) {
           if (ptr -> num_get_out_of_jail_cards() > 0 && ptr -> plyr_name() != player_name) {
               std::cout << ptr -> plyr_name() << " owns " << ptr -> num_get_out_of_jail_cards() << " get of jail cards." << std::endl;
               char yes_no;
               std::cout << "Do you want to purchase a get out of jail card from " << ptr -> plyr_name() << "? (y/n) ";
               std::cin >> yes_no;
               
               while (yes_no != 'n' && yes_no != 'y' && yes_no != 'N' && yes_no != 'Y') {
                   std::cin.clear(); std::cin.ignore();
                   std::cout << '\n' << "Sorry, I didn't catch that. Please try again: ";
                   std::cin >> yes_no;
               }
           
               // Player in jail can make an offer to the player who holds the card(s).
               if (yes_no == 'y' || yes_no == 'Y') {
                   int offer;
                   std::cout << "Make an offer (Current balance) $" << player_balance << ")   $";
                   std::cin >> offer;
                   while (offer > player_balance) {
                       std::cout << "Insufficient funds, please make a new offer: ";
                       std::cin.clear(); std::cin.ignore();
                       std::cin >> offer;
                   }
               
                   char offer_yes_no;
                   std::cout << ptr -> plyr_name() << ", do you accept this offer? (y/n) ";
                   std::cin >> offer_yes_no;
                   if (offer_yes_no == 'y' || offer_yes_no == 'Y') {
                       pay(offer);
                       ptr -> receive(offer);
                       ptr -> loose_get_out_of_jail_card(); // Player who accepts offer looses on of his cards.
                       free_from_jail(player_pos);          // Card gets automatically used up.
                       break;                               // Stops iterating over players.
                   }
               }
           }
       }
   }
       
   // Third method: Try to roll doubles (up to three attempts)
   if (in_jail && turns_passed_in_jail < 3) {
        std::cout << "You were unable to get your hands on a get out of jail card." << std::endl;
        std::cout << "Let's see if you are lucky with the dice... " << std::endl;
        int d_roll1 = rng(6)+1; int d_roll2 = rng(6)+1; // Rolls the dice.
        std::cout << "First dice roll: " << d_roll1 << ". Second dice roll: " << d_roll2 << "." << std::endl;
       
        if (d_roll1 == d_roll2) {
            std::cout << "You rolled doubles!" << std::endl;
            free_from_jail(player_pos);
        }
        else {
           std::cout << "Unlucky! You did not manage to roll doubles this turn." << std::endl;
           turns_passed_in_jail++;
       }
   }
   
   // Forth method: If player still in jail, pay the fine to get freed.
   if (in_jail) {
       std::cout << "Do you want to pay the fine ($50) to get freed? (y/n) ";
       char yes_no;
       std::cin >> yes_no;
       while (yes_no != 'y' && yes_no != 'n' && yes_no != 'Y' && yes_no != 'N') {
           std::cin.clear(); std::cin.ignore();
           std::cout << '\n' << "Sorry, I didn't catch that. Please try again: ";
           std::cin >> yes_no;
       }
       if (yes_no == 'y' || yes_no == 'Y') {
           pay(50);
           free_from_jail(player_pos);
       }
       else {
           std::cout << "You must remain in jail this turn." << std::endl;
           std::cout << "Try better luck next time." << std::endl;
       }
    std::cin.ignore();
   }
}

int player::dice_roll()
{
    // Make a counter to store the number of times the function is called. Declare it static.
    static int count{0};
    count++;
    
    // Roll two different dice:
    int dice_1 = rng(6) + 1;
    int dice_2 = rng(6) + 1;
    int sum = dice_1 + dice_2;
    
    // If static counter gets to 4, it means the dice roll function was called 4 times.
    //   This can only happen if a player rolled doubles 3 times, in which case, they get sent to jail.
    //   Hence, only print dice outcome if the function was called 3 or less times.
    if (count < 4) std::cout << "First dice: " << dice_1 << ". Second Dice: " << dice_2 << ". Sum: " << sum << std::endl;
    
    // Declare an integer to store further dice rolls whenever player rolls doubles.
    int additional_roll {0};
    if (dice_1 == dice_2 && count < 4) {
        if (count < 3) std::cout  << "You rolled doubles! Rolling again..." << std::endl;
        // If count == 3, player gets sent to jail, so no need to display the above message.
        
        // Call function recursively.
        additional_roll = dice_roll();
    }
    
    // Send to jail if player rolls doubles 3 times:
    if (count == 4){
        std::cout << "Oh dear, you were caught speeding!" << std::endl;
        send_to_jail();
    }
    
    // Reset counter for next player / turn.
    count  = 0;
    return sum+additional_roll;
}

void player::go_bankrupt(std::vector<std::shared_ptr<player>> & players, std::vector<std::string> & player_pos)
{
    std::cout << plyr_name() << " you have declared bankrupcy. Thank you for playing Monopoly C++."
                << std::endl;
    bankrupt = true;
    
    properties_owned.clear();
    
    // Delete from the board:
    erase_substring(player_pos[current_pos], player_abreviation);
    
    // Delete from vector of players:
    int count{}, idx{};
    std::string this_player_name = this -> plyr_name();
    
    // Search for player index within vector smart pointers to players:
    std::for_each(players.begin(), players.end(),
    [& count, &this_player_name, &idx] (std::shared_ptr<player> & plyr)
    {
        if (plyr -> plyr_name() == this_player_name) idx = count;
        count++;
    });

    // Remove from vector of players, so that they don't get any more moves.
    players.erase (players.begin()+idx);
    
    // Check number of players left:
    // If only one player left, notify the game is over and announce winner.
    if (players.size() == 1)
        std::cout << players[0] -> plyr_name() << " won the game. Congratulations!." << std::endl;
    // Otherwise list remaining players:
    else {
        std::cout << '\n' << "Remaining players:" << std::endl;
        for (size_t i{}; i<players.size(); i++) std::cout << players[i] -> plyr_name() << std::endl;
    }
}

// Out-of-class declaration of static vector: Use size 4, since this is the maximum number of players allowed.
std::vector<std::string> player::players_in_jail(4);
