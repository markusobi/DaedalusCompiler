/*=============================================================================
    Copyright (c) 2001-2014 Joel de Guzman

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
=============================================================================*/
#if !defined(BOOST_SPIRIT_X3_CALC9_STATEMENT_HPP)
#define BOOST_SPIRIT_X3_CALC9_STATEMENT_HPP

#include <boost/spirit/home/x3.hpp>
#include "ast.hpp"

namespace parser {
    struct program_class;
    typedef boost::spirit::x3::rule<program_class, ast::program> program_type;

    BOOST_SPIRIT_DECLARE(program_type)
}

parser::program_type const &getProgramParser();


#endif
