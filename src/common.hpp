/*=============================================================================
    Copyright (c) 2001-2014 Joel de Guzman

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
=============================================================================*/
#pragma once

#include <boost/spirit/home/x3.hpp>
#include <boost/spirit/home/x3/support/utility/annotate_on_success.hpp>
#include "real_policy.hpp"
#include "config.hpp"

namespace parser {
    using x3::raw;
    using x3::lexeme;
    using encoding::alnum;
    using encoding::digit;
    using encoding::char_;
    using x3::no_case;
    using x3::eol;
    using x3::eoi;

    namespace {
        x3::symbols<> keywords;
        x3::symbols<bool> var_const_mapping;
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
                    ("instance")
                    ("void")
                    ("func")*/
                    ("var")
                    ("const");
            return keywords;
        }

        x3::symbols<bool>& getVarConst()
        {
            static bool once = false;
            if (once)
                return var_const_mapping;
            once = true;

            var_const_mapping.add
                    ("const", true)
                    ("var", false);
            return var_const_mapping;
        }
    }

    struct identifier_class;
    struct variable_class;
    struct type_class;
    struct var_const_class;
    struct string_literal_class;

    typedef x3::rule <identifier_class, std::string> identifier_type;
    typedef x3::rule <variable_class, ast::variable> variable_type;
    typedef x3::rule <type_class, ast::type> type_type;
    typedef x3::rule <var_const_class, bool> var_const_type;
    typedef x3::rule <string_literal_class, std::string> string_literal_type;

    identifier_type const identifier = "identifier";
    variable_type const variable = "variable";
    type_type const type = "type";
    var_const_type const var_const = "type";
    string_literal_type const string_literal = "string_literal";

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
    const auto var_const_def = getVarConst();
    const auto string_literal_def = lexeme['"' >> *(char_ - '"') >> '"']; // TODO replace \n in string literals

    namespace {
        x3::real_parser<float, daedalus_real_policies<float>> strict_float;
    }

    BOOST_SPIRIT_DEFINE(identifier, variable, type, var_const, string_literal)

    struct variable_class : x3::annotate_on_success {
    };
    struct type_class : x3::annotate_on_success {
    };

}