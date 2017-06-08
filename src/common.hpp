/*=============================================================================
    Copyright (c) 2001-2014 Joel de Guzman

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
=============================================================================*/
#if !defined(BOOST_SPIRIT_X3_CALC9_COMMON_HPP)
#define BOOST_SPIRIT_X3_CALC9_COMMON_HPP

#include <boost/spirit/home/x3.hpp>

namespace parser {
    using x3::raw;
    using x3::lexeme;
    using x3::alnum;
    using x3::digit;

    namespace {
        x3::symbols<> keywords;
        const x3::symbols<>& getKeywords()
        {
            static bool once = false;
            if (once)
                return keywords;
            once = true;

            // reserved keywords:
            // forbidden in variable names
            keywords.add
                    ("class")
                    ("prototype")
                    ("if")
                    ("else")
                    ("while")
                    ("return")
                    ("int")
                    ("float")
                    ("string")
                    ("instance")
                    ("func")
                    ("void")
                    ("var")
                    ("const");
            return keywords;
        }
    }

    struct identifier_class;
    typedef x3::rule <identifier_class, std::string> identifier_type;
    identifier_type const identifier = "identifier";

    // exclude keywords, but not names starting with keywords
    // lexeme: disable skipper (whitespace/comments inside variable names)
    const auto ident_char = alnum | '_';
    auto const identifier_def = raw[lexeme[
            !(getKeywords() >> !ident_char)
            >>
            (ident_char - digit) >> *ident_char
    ]];

    BOOST_SPIRIT_DEFINE(identifier);
}

#endif
