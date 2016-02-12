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
		// Init with built-in types
		typemap();

		void add(const std::string &name, const type &t);
		bool check(const std::string &name);
		type get(const std::string &name);

	private:
		void build();

	private:
		std::vector<std::string> m_typenames;
		std::regex m_typetree;
		std::vector<type> m_types;
		std::cmatch m_result;

	};
}