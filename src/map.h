#pragma once

#include "type.h"
#include "helper.h"

#include <radix_tree.hpp>

#define SMALL_SIZE 8

namespace piggy
{
    template<class T>
    class smap
    {
    public:
        using item = std::pair<const char *, T>;

    public:
        // Initialize only with small entries
        template<unsigned int N>
        smap(item(&items)[N])
        {
            for (auto e : items)
                m_small.insert(e);
        }

        // Disable copy
        smap(const smap&) = delete;
        smap& operator=(const smap&) = delete;

        void add(const std::string &name, const T &t)
        {
            assert(name.length() <= SMALL_SIZE);
            m_small.insert(std::make_pair(name.c_str(), t));
        }

        bool check(const std::string &name)
        {
            assert(name.length() <= SMALL_SIZE);
            return m_small.find(name.c_str()) != m_small.end();
        }

        T get(const std::string &name, const T &t = T())
        {
            assert(name.length() <= SMALL_SIZE);

            auto result = m_small.find(name.c_str());
            if (result != m_small.end())
                return result->second;

            return t;
        }

    private:
        std::map<string::small, T> m_small;

    };

    template<class T>
    class map : public smap<T>
    {
    public:
        template<unsigned int N>
        map(item(&items)[N]) : smap(items) {}

        // Disable copy
        map(const map&) = delete;
        map& operator=(const map&) = delete;

        void add(const std::string &name, const T &t)
        {
            if (name.length() <= SMALL_SIZE)
                smap<T>::add(name, t);
            else
                m_large.insert(std::make_pair(name, t));
        }

        bool check(const std::string &name)
        {
            if (name.length() <= SMALL_SIZE)
                return smap<T>::check(name);
            else
                return m_large.find(name) != m_large.end();
        }

        T get(const std::string &name, const T &t = T())
        {
            if (name.length() <= SMALL_SIZE)
                return smap<T>::get(name, t);
            else
            {
                auto result = m_large.find(name);
                if (result != m_large.end())
                    return result->second;
            }

            return t;
        }

    private:
        radix_tree<std::string, T> m_large;

    };
}