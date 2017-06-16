/*=============================================================================
    Copyright (c) 2001-2014 Joel de Guzman

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
=============================================================================*/
#if !defined(BOOST_SPIRIT_X3_CALC9_AST_ADAPTED_HPP)
#define BOOST_SPIRIT_X3_CALC9_AST_ADAPTED_HPP

#include "ast.hpp"
#include <boost/fusion/include/adapt_struct.hpp>

BOOST_FUSION_ADAPT_STRUCT(ast::unary,
                          operator_, operand_
)

BOOST_FUSION_ADAPT_STRUCT(ast::operation,
                          operator_, operand_
)

BOOST_FUSION_ADAPT_STRUCT(ast::expression,
                          first, rest
)

BOOST_FUSION_ADAPT_STRUCT(ast::variable_declaration,
                          type_, var, rhs
)

BOOST_FUSION_ADAPT_STRUCT(ast::array_declaration,
                          type_, var, size, rhs
)

BOOST_FUSION_ADAPT_STRUCT(ast::assignment,
                          lhs, rhs
)

BOOST_FUSION_ADAPT_STRUCT(ast::return_statement,
                          operand_
)

BOOST_FUSION_ADAPT_STRUCT(ast::if_statement,
                          condition, then, else_
)

BOOST_FUSION_ADAPT_STRUCT(ast::while_statement,
                          condition, body
)

#endif
