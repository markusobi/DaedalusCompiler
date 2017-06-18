#pragma once

#include "config.hpp"
#include "skipper.hpp"

namespace parser
{
    namespace
    {
        skipper_type const skipper = "skipper";

        auto const skipper_def =
                encoding::space
                | (x3::lit("/*") > x3::seek[x3::lit("*/")])
                | (x3::lit("//") > x3::seek[x3::eol | x3::eoi])
        ;
    }

    BOOST_SPIRIT_DEFINE(skipper)

    const skipper_type& getSkipper()
    {
        return skipper;
    }
}