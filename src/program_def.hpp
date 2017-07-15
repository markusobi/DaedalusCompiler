#pragma once

#include "program.hpp"

namespace parser
{
    program_type const program("program");

    auto const program_def =
            *lit(';') // skip ;
            > -(global_decl % *lit(';'))
            > *lit(';') // skip ;
    ;

    BOOST_SPIRIT_DEFINE(program)

    struct program_class : error_handler_base, x3::annotate_on_success {
    };
}


parser::program_type const &getProgramParser() {
    return parser::program;
}