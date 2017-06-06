/*=============================================================================
    Copyright (c) 2001-2014 Joel de Guzman

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
=============================================================================*/
#pragma once

#include <boost/spirit/home/x3.hpp>
#include "ast.hpp"
#include <map>

namespace x3 = boost::spirit::x3;
namespace parser {
    struct expression_class;
    typedef x3::rule<expression_class, ast::expression> expression_type;

    BOOST_SPIRIT_DECLARE(expression_type);
}

parser::expression_type const &getExpressionParser();
const std::map<ast::optoken, std::string>& getOpTokenLookup();
