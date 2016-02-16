#pragma once

#include "type.h"

#include <string>
#include <memory>
#include <vector>

namespace piggy
{
    namespace ast
    {
        enum class type
        {
            node = 0,
            scope,
            decl,
            init
        };

        template<class T>
        using ref = std::unique_ptr<T>;

        template<typename Derived, typename Base>
        ref<Derived> ref_cast(ref<Base> &p)
        {
            auto d = static_cast<Derived *>(p.release());
            return ref<Derived>(d);
        }

        class node
        {
        public:
            type kind;
            virtual ~node() {}
        };

		using noderef = ref<node>;

		class scope : public node
		{
		public:
			std::vector<noderef> locals;

			void add(noderef &decl)
			{
				locals.push_back(noderef(decl.release()));
			}
		};

		class decl : public node
		{
		public:
			std::string name;
			piggy::type type;
            noderef init;

		};

        class init : public node
        {
        public:
            piggy::type type;
            std::vector<char> data;

        };
    }
}

namespace std
{
    template<>
    struct less<piggy::ast::noderef>
    {
        bool operator()(const piggy::ast::noderef& x, const piggy::ast::noderef& y) const
        {
            return static_cast<int>(x->kind) < static_cast<int>(y->kind);
        }
    };
}