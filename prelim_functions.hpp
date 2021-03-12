#ifndef test_hpp
#define test_hpp

#include <stdio.h>
#include <string>

// -------------------------------------------------------------------
//                      PRELIMINARY FUNCTIONS:
// -------------------------------------------------------------------
// Includes:
//  - erase_substring: Function to erase a subtring from some main string.
//  - position_to_prop_vector: Finds the correspondance between position of
//      a property in the board and index in the vector of properties.
//  - center_strings: Centers a string given a certain linewidth.
//  - rng: Generate a random number from 0 up to some base using the
//         Mersenne Twister 19937 engine.
//  - shuffle_order: Shuffles a vector of players in a random order.

void erase_substring(std::string & mainStr, const std::string & toErase);
int position_to_prop_vector (int pos);
std::vector<std::string> center_strings(std::vector<std::string> strings2center, int line_size);
int rng(int base);

// Forward declare player class.
class player;
void shuffle_order(std::vector<std::shared_ptr<player>> & to_shuffle );

#endif /* test_hpp */
