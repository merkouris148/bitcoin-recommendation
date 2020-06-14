#ifndef SET_OPERATIONS_H

#define SET_OPERATIONS_H

#include <set>

#include <iostream>

//show
template<typename t>
std::ostream& operator<<(std::ostream& out, const std::set<t>& S);

#include "set_operations.tpp"

#endif
