//
// Created by markus on 28.06.17.
//
#include "skipper_def.hpp"
#include "expression_def.hpp"
#include "statement_def.hpp"
#include "program_def.hpp"
#include "config.hpp"

namespace parser {
    BOOST_SPIRIT_INSTANTIATE(skipper_type, iterator_type, x3::unused_type)


    BOOST_SPIRIT_INSTANTIATE(operand_type, iterator_type, context_type)



    BOOST_SPIRIT_INSTANTIATE(program_type, iterator_type, context_type)
}