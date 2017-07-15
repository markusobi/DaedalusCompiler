#include "expression_def.hpp"
#include "statement_def.hpp"
#include "program_def.hpp"

namespace parser {
    //BOOST_SPIRIT_INSTANTIATE(operand_type, iterator_type, context_type)
    BOOST_SPIRIT_INSTANTIATE(program_type, iterator_type, context_type)
}