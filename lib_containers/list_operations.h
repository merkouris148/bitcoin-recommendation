#ifndef LIST_OPERATIONS_H

#define LIST_OPERATIONS_H

#include <list>
#include <iterator>

#include <iostream>


/* appends the list K to list L, L *= K
 * (this function changes the list L but
 * leaves the list K untouched)*/
template <typename T>
void append(std::list<T>& L, std::list<T>& K);

/* K = L1 * L2 */
template <typename T>
void concat(std::list<T>& L1, std::list<T>& L2, std::list<T>& K);

//show
template <typename T>
std::ostream& operator<<(std::ostream& out, const std::list<T>& L);

#include "list_operations.tpp"

#endif
