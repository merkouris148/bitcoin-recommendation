

/* appends the list K to list L, L *= K
 * (this function changes the list L but
 * leaves the list K untouched)*/
template <typename T>
void append(std::list<T>& L, std::list<T>& K){
	L.insert(L.end(), K.begin(), K.end());
}

/* K = L1 * L2*/
template <typename T>
void concat(std::list<T>& L1, std::list<T>& L2, std::list<T>& K){
	append(K, L1); append(K, L2);
}

//show
template <typename T>
std::ostream& operator<<(std::ostream& out, const std::list<T>& L){
	out << "[" << std::endl;
	for(auto l : L) out << "\t" << l << "," << std::endl;
	out << "]" << std::endl;
	
	return out;
}
