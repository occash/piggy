#pragma once

#include "type.h"

#include <string>
#include <memory>
#include <vector>

namespace piggy
{
    namespace ast
    {
        class node
        {
        public:
            virtual ~node() {}
        };

		template<class T>
		using ref = std::unique_ptr<T>;
		using noderef = ref<node>;

		template<typename Derived, typename Base>
		std::unique_ptr<Derived> ref_cast(std::unique_ptr<Base> &p)
		{
			auto d = static_cast<Derived *>(p.release());
			return std::unique_ptr<Derived>(d);
		}

		class scope : public node
		{
		public:
			std::vector<noderef> locals;

			void add(noderef &decl)
			{
				locals.push_back(noderef(decl.release()));
			}
		};

		class declaration : public node
		{
		public:
			std::string name;
			type kind;

		};

        /*class variable : public node
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
            noderef lhs;
			noderef rhs;
        };*/
    }
}