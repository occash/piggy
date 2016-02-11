#pragma once

#include "type.h"

#include <string>
#include <regex>
#include <vector>

namespace piggy
{
	class typemap
	{
	public:
		// for built-in types
		//typemap(std::initializer_list);

		void add(const std::string &name, const type &t);
		bool check(const std::string &name);

	private:
		std::vector<std::string> m_typenames;
		std::regex m_typetree;
		std::vector<type> m_types;
		std::cmatch m_result;

	};
}