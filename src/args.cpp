#include "args.h"


namespace piggy
{
    namespace args
    {
        args::parser::parser(int argc, char *argv[]) :
            m_arguments{ argv, argv + argc}
        {
        }

        bool parser::parse()
        {
            for (auto i = 0; i < m_arguments.size(); ++i) {
                auto argument = m_arguments.at(i);
                auto size = strlen(argument);

                // Flag
                if (size > 0 && argument[0] == '-')
                {
                    auto found = false;

                    for (auto action : m_action_flags)
                    {
                        if (strcmp(argument, action.fullflag) == 0 ||
                            strcmp(argument, action.shortflag) == 0)
                        {
                            m_actions[action.name] = true;
                            found = true;
                        }
                    }

                    if (found)
                        continue;

                    for (auto value : m_value_flags)
                    {
                        if (strcmp(argument, value.fullflag) == 0 ||
                            strcmp(argument, value.shortflag) == 0)
                        {
                            if (++i >= m_arguments.size())
                                return false;

                            m_values[value.name] = m_arguments.at(i);
                            found = true;
                        }
                    }

                    if (!found)
                        return false;
                }
                else
                    m_positional.push_back(argument);
            }

            return true;
        }

        void parser::print()
        {
        }

        int parser::size()
        {
            return m_positional.size();
        }

        bool parser::action(const char *name)
        {
            auto value = m_actions.find(name);
            if (value == m_actions.end())
                return false;

            return value->second;
        }

        std::string parser::value(const char *name)
        {
            auto value = m_values.find(name);
            if (value == m_values.end())
                return nullptr;

            return value->second;
        }

        std::string parser::value(int index)
        {
            if (index < 0 || index >= m_positional.size())
                return nullptr;

            return m_positional.at(index);
        }
    }
}
