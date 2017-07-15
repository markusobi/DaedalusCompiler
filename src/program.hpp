#pragma once

#include <boost/spirit/home/x3.hpp>
#include "ast.hpp"

namespace parser {
    struct program_class;
    typedef boost::spirit::x3::rule<program_class, ast::program> program_type;

    BOOST_SPIRIT_DECLARE(program_type)
}

const parser::program_type& getProgramParser();
