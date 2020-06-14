
template<typename t>
std::ostream& operator<<(std::ostream& out, const std::set<t>& S){
	out << "{ ";
	for(auto s : S) out << s << ", ";
	out << "}" << std::endl;
	
	return out;
}
