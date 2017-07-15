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

BOOST_FUSION_ADAPT_STRUCT(ast::func_call,
                          var, args
)

BOOST_FUSION_ADAPT_STRUCT(ast::array_access,
                          var, index
)

BOOST_FUSION_ADAPT_STRUCT(ast::typed_var,
                          isConst, type_, var
)

BOOST_FUSION_ADAPT_STRUCT(ast::memberAccess,
                          object, member
)

BOOST_FUSION_ADAPT_STRUCT(ast::variable_declaration,
                          typed_var_, rhs
)

BOOST_FUSION_ADAPT_STRUCT(ast::multi_variable_declaration,
                          isConst, type_, vars
)

BOOST_FUSION_ADAPT_STRUCT(ast::array_declaration,
                          typed_var_, size, rhs
)

BOOST_FUSION_ADAPT_STRUCT(ast::assignment,
                          lhs, operator_, rhs
)

BOOST_FUSION_ADAPT_STRUCT(ast::return_statement,
                          operand_
)

BOOST_FUSION_ADAPT_STRUCT(ast::condition_block,
                          condition, then
)

BOOST_FUSION_ADAPT_STRUCT(ast::if_statement,
                          condition_blocks, else_
)

BOOST_FUSION_ADAPT_STRUCT(ast::while_statement,
                          condition, body
)

BOOST_FUSION_ADAPT_STRUCT(ast::function,
                          returnType, name, params, body
)

BOOST_FUSION_ADAPT_STRUCT(ast::prototype,
                          name, baseClassName, body
)

BOOST_FUSION_ADAPT_STRUCT(ast::instance,
                          name, type_, body
)

BOOST_FUSION_ADAPT_STRUCT(ast::instance_var_decl,
                          vars, type_
)

BOOST_FUSION_ADAPT_STRUCT(ast::extern_class,
                          name, body
)

#endif
