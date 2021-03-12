# include "prelim_functions.hpp"
# include <iostream>
# include <vector>
# include <string>
# include <random>

// Set up a "random device" that generates a new random number each time the program is run.
std::random_device rand_dev;
// Set up a pseudo-random number generater "rnd", seeded with a random number
std::mt19937 rnd(rand_dev());
 
void erase_substring(std::string & main_string, const std::string & to_erase)
{
    // Search for the substring in string
    size_t pos = main_string.find(to_erase);
 
    if (pos != std::string::npos) {
        // If found then erase it from string
        main_string.erase(pos, to_erase.length());
    }
}

int position_to_prop_vector (int pos)
{
    // Locations of non-property type spaces:
    std::vector<int> non_prop_positions{2, 4, 7, 10, 17, 20, 22, 30, 33, 36, 38};
    std::vector<int>::iterator it = std::find(non_prop_positions.begin(), non_prop_positions.end(), pos);
    
    // If you land on non-property type. throw exception;
    if (it != non_prop_positions.end()) {
        std::cout << "Landed on non-property type." << std::endl;
        throw 1;
    }
    
    // Otherwise, count how many non-property tiles were passed until poistion, and subtract them.
    else {
        int count{1}; // Start counter at 1 to account for the initial 'GO' tile.
        for (unsigned i{}; i<non_prop_positions.size(); i++) {
            if (pos > non_prop_positions[i]) count++;
        }
        return pos-count;
    }
}

std::vector<std::string> center_strings(std::vector<std::string> strings2center, int line_size)
{
    std::vector<std::string> centered_strings;
    for (unsigned i{}; i<strings2center.size(); i++) {
        size_t string_size = strings2center[i].size(); // Size of particular string in vector.
        size_t size_diff   = line_size-string_size;    // Number of blank spaces up to line size.
        assert(size_diff>=0);
        
        // If the number of blank spaces  up to line_size is even, add equally many blank
        //      spaces in the front and back of the string:
        if ( size_diff % 2 == 0 ) {
            // String of white space to add at the front and back:
            std::string white_space;
            for (int i{}; i<size_diff/2; i++) white_space+=" ";
            
            // Add whitespace to front and back and pushback to vector of centred strings.
            std::string centred_string = white_space+strings2center[i]+white_space;
            
            // Confirm inteded size:
            assert(centred_string.size() == line_size);
            centered_strings.push_back(centred_string);
            
        }
        // If the number of blank spaces up to linesize is odd, add one more space at
        //      the back of the string.
        
        else {
            // String of white space to add at the front and back. Since looping terminal condition uses
            //      integer division, remainder is discarded (which we want).
            std::string white_space;
            for (int i{}; i<size_diff/2; i++) white_space+=" ";
        
            // Add white space at front and back. Add an addition space at the back of the string.
            std::string centred_string = white_space+strings2center[i]+" "+white_space;
            // Confirm intended size and pushback to vector of centred strings.
            assert(centred_string.size() == line_size);
            centered_strings.push_back(centred_string);
        }
    }
    return centered_strings;
}

int rng(int base)
{
    return rnd() % base;
}

class player;

void shuffle_order( std::vector<std::shared_ptr<player>> & to_shuffle )
{
    std::cout << "Randomising order of play..." << std::endl;
    std::shuffle(to_shuffle.begin(), to_shuffle.end(), rnd);
}
