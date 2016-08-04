#pragma once

#include <vector>
#include <map>

namespace piggy
{
    namespace args
    {
        struct action_flag
        {
            const char *name;
            const char *fullflag;
            const char *shortflag;
            const char *help;
            bool default;
        };

        struct value_flag
        {
            const char *name;
            const char *fullflag;
            const char *shortflag;
            const char *help;
            const char *default;
        };

        class parser
        {
        public:
            parser(int argc, char *argv[]);

            template<unsigned int N>
            void setup(action_flag(&flags)[N]);
            template<unsigned int N>
            void setup(value_flag(&flags)[N]);

            bool parse();
            void print();

            int size();
            bool action(const char *name);
            std::string value(const char *name);
            std::string value(int index);

        private:
            std::vector<const char *> m_arguments;
            std::vector<action_flag> m_action_flags;
            std::vector<value_flag> m_value_flags;

            std::vector<const char *> m_positional;
            std::map<const char *, bool> m_actions;
            std::map<const char *, const char *> m_values;
        };

        template<unsigned int N>
        inline void parser::setup(action_flag(&flags)[N])
        {
            m_action_flags.reserve(N);
            m_action_flags.assign(flags, flags + N);

            for (auto flag : flags)
                m_actions.insert(std::make_pair(flag.name, flag.default));
        }

        template<unsigned int N>
        inline void parser::setup(value_flag(&flags)[N])
        {
            m_value_flags.reserve(N);
            m_value_flags.assign(flags, flags + N);

            for (auto flag : flags)
                m_values.insert(std::make_pair(flag.name, flag.default));
        }
    }
}