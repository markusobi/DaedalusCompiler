/*=============================================================================
    Copyright (c) 2001-2014 Joel de Guzman

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
=============================================================================*/
#pragma once

#include <boost/spirit/home/x3.hpp>
#include <boost/spirit/home/x3/support/utility/annotate_on_success.hpp>

namespace parser {
    using x3::raw;
    using x3::lexeme;
    namespace encoding = x3::iso8859_1;
    using encoding::alnum;
    using encoding::digit;
    using x3::no_case;

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
/*                    ("int")
                    ("float")
                    ("string")
                    ("instance")*/
                    ("void")
                    ("func")
                    ("var")
                    ("const");
            return keywords;
        }
    }

    struct identifier_class;
    struct variable_class;
    struct type_class;

    typedef x3::rule <identifier_class, std::string> identifier_type;
    typedef x3::rule <variable_class, ast::variable> variable_type;
    typedef x3::rule <type_class, ast::type> type_type;

    identifier_type const identifier = "identifier";
    variable_type const variable = "variable";
    type_type const type = "type";

    const auto ident_char = alnum | '_';
    const auto word_end = !ident_char;

    template <class Parser>
    auto nocase_wholeword(Parser& parser)
    {
        return lexeme[no_case[parser] >> word_end];
    }

    // exclude keywords, but not names starting with keywords
    // lexeme: disable skipper (whitespace/comments inside variable names)
    auto const identifier_def = raw[lexeme[
            !nocase_wholeword(getKeywords())
            >>
            (ident_char - digit) >> *ident_char
    ]];

    const auto variable_def = identifier;
    auto const type_def = identifier;

    //qi::real_parser<float, qi::strict_real_policies<float>> strict_float;

    BOOST_SPIRIT_DEFINE(identifier, variable, type);

    struct variable_class : x3::annotate_on_success {
    };
    struct type_class : x3::annotate_on_success {
    };
}