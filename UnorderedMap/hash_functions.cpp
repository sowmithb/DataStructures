#include "hash_functions.h"

size_t polynomial_rolling_hash::operator() (std::string const & str) const {
    /* TODO */
    size_t b = 19;
    auto m = 3298534883309ul;
    size_t p = 1;
    size_t hash = 0;

    for(char c: str) {
        hash += c * p;
        p = (p*b) % m;
    }
    return hash;


}

size_t fnv1a_hash::operator() (std::string const & str) const {
    /* TODO */
    size_t prime = 0x00000100000001B3;
    size_t basis = 0xCBF29CE484222325;
    
    size_t hash = basis;

    for (char c : str)
    {
        hash = hash ^ c;
        hash *= prime;
    }
    return hash;
}
