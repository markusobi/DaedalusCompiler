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
    struct operand_class;
    struct array_access_class;
    typedef x3::rule<operand_class, ast::operand> operand_type;
    typedef x3::rule<array_access_class, ast::operand> array_access_type;

    BOOST_SPIRIT_DECLARE(operand_type);
}

const parser::operand_type& getOperandParser();

const parser::array_access_type& getArrayAccessParser();

const std::map<ast::optoken, std::string>& getOpTokenLookup();
