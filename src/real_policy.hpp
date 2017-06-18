#pragma once

#include <boost/spirit/home/x3.hpp>

template <typename T>
struct daedalus_real_policies : boost::spirit::x3::strict_ureal_policies<T>
{
    //  don't parse inf
    template <typename Iterator, typename Attribute>
    static bool
    parse_inf(Iterator&, Iterator const&, Attribute&)
    {
        return false;
    }
    //  don't parse nan
    template <typename Iterator, typename Attribute>
    static bool
    parse_nan(Iterator&, Iterator const&, Attribute&)
    {
        return false;
    }
};
