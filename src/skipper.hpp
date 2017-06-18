#pragma once

#include <boost/spirit/home/x3.hpp>

namespace parser
{
    namespace x3 = boost::spirit::x3;

    using skipper_type = x3::rule<struct skipper_class, const x3::unused_type>;

    BOOST_SPIRIT_DECLARE(skipper_type)

    const skipper_type& getSkipper();
}
