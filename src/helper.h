#pragma once

#include <cassert>
#include <cstdio>
#include <memory>
#include <string>
#include <sstream>

#include "compat.h"

namespace piggy
{
    namespace string
    {
        inline char* strrncpy(char* dest, const char* src, size_t n)
        {
            while (n && *src)
            {
                dest[--n] = *src;
                src++;
            }
            while (n)
                dest[--n] = '\0';
            return dest;
        }

        template<typename... Args>
        std::string format(const char *format, Args... args)
        {
            unsigned int size = snprintf(nullptr, 0, format, args...) + 1;
            std::unique_ptr<char[]> buf(new char[size]);
            snprintf(buf.get(), size, format, args...);
            return std::string(buf.get(), buf.get() + size - 1);
        }

        template <typename Iter>
        std::string join(Iter begin, Iter end, std::string const& separator)
        {
            std::ostringstream result;
            if (begin != end)
                result << *begin++;
            while (begin != end)
                result << separator << *begin++;
            return result.str();
        }

        template <typename T>
        void concat_p(std::ostream& o, T t)
        {
            o << t << std::endl;
        }

        template<typename T, typename... Args>
        void concat_p(std::ostream& o, T t, Args... args) // recursive variadic function
        {
            concat_p(o, t);
            concat_p(o, args...);
        }

        template<typename... Args>
        std::string concat(Args... args)
        {
            std::ostringstream result;
            concat_p(result, args...);
            return result.str();
        }
    }
}
