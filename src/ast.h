#pragma once

#include <string>
#include <memory>

namespace piggy
{
    namespace ast
    {
        class node
        {
        public:
            virtual ~node() {}
        };

        class variable : public node
        {
            std::string name;

        public:
            variable(std::string n) : name(n) {}
        };

        class number : public node
        {
            double value;

        public:
            number(double v) : value(v) {}
        };

        class binary : public node
        {
            int operation;
            std::unique_ptr<node> lhs;
            std::unique_ptr<node> rhs;
        };
    }
}