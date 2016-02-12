#include "typemap.h"
#include "helper.h"

using typeinfo = std::pair<const char *, piggy::type>;

typeinfo builtin[] {
	{ "any", { 16, 4, false } },
	{ "int", { 8, 4, false } },
	{ "uint", { 8, 4, true } },
	{ "float", { 8, 4, false } },
	{ "double", { 16, 4, false } },
	{ "vec2", { 16, 16, false } },
	{ "vec3", { 32, 16, false } },
	{ "vec4", { 32, 16, false } },
	{ "mat3", { 96, 16, false } },
	{ "mat4", { 128, 16, false } },
};

namespace piggy
{
	typemap::typemap()
	{
		unsigned int size = sizeof(builtin) / sizeof(typeinfo);

		m_typenames.reserve(size);
		m_types.reserve(size);

		for (auto info : builtin)
		{
			m_typenames.push_back(info.first);
			m_types.push_back(info.second);
		}

		build();
	}

	void typemap::add(const std::string &name, const type &t)
	{
		m_typenames.push_back(name);
		m_types.push_back(t);

		build();
	}

	bool typemap::check(const std::string &name)
	{
		std::regex_search(name.c_str(), m_result, m_typetree);
		return !m_result.empty();
	}

	type typemap::get(const std::string &name)
	{
		std::regex_search(name.c_str(), m_result, m_typetree);
		if (!m_result.empty())
		{
			for (unsigned int i = 1; i < m_result.size(); ++i)
			{
				if (m_result[i].matched)
					return m_types[i - 1];
			}
		}

		return m_types[0]; // any
	}

	void typemap::build()
	{
		// Build regex
		std::string names = string::join(m_typenames.begin(), m_typenames.end(), ")|(");
		std::string result = string::format("\\b(%s)\\b", names.c_str());

		m_typetree.assign(result);
	}
}
