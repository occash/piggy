#include "typemap.h"

void piggy::typemap::add(const std::string &name, const type &t)
{
	m_typenames.push_back(name);
	m_types.push_back(t);
	
	// TODO: Optimize build type tree
	std::string result("\\b");
	for (auto type : m_typenames) {
		result.push_back('(');
		result.append(type);
		result.push_back(')');
	}
	result.append("\\b");

	m_typetree.assign(result);
}

bool piggy::typemap::check(const std::string &name)
{
	std::regex_search(name.c_str(), m_result, m_typetree);
	return !m_result.empty();
}
