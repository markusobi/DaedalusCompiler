/*=============================================================================
    Copyright (c) 2001-2014 Joel de Guzman

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
=============================================================================*/
#pragma once

#include <boost/spirit/home/x3.hpp>
#include <boost/spirit/home/x3/support/utility/annotate_on_success.hpp>
#include "ast.hpp"
#include "ast_adapted.hpp"
#include "expression.hpp"
#include "common.hpp"
#include "error_handler.hpp"
#include <iostream>

namespace parser {
    using x3::int_;
    using x3::uint_;
    using x3::raw;
    using x3::lexeme;
    using encoding::lit;

    ////////////////////////////////////////////////////////////////////////////
    // Tokens
    ////////////////////////////////////////////////////////////////////////////

    ////////////////////////////////////////////////////////////////////////////
    // Main expression grammar
    ////////////////////////////////////////////////////////////////////////////

    logical_or_expr_type const logical_or_expr = "logical_or_expr";
    logical_and_expr_type const logical_and_expr = "logical_and_expr";
    bitwise_or_expr_type const bitwise_or_expr = "bitwise_or_expr";
    bitwise_xor_expr_type const bitwise_xor_expr = "bitwise_xor_expr";
    bitwise_and_expr_type const bitwise_and_expr = "bitwise_and_expr";
    equality_expr_type const equality_expr = "equality_expr";
    relational_expr_type const relational_expr = "relational_expr";
    bitshift_expr_type const bitshift_expr = "bitshift_expr";
    additive_expr_type const additive_expr = "additive_expr";
    multiplicative_expr_type const multiplicative_expr = "multiplicative_expr";
    unary_expr_type const unary_expr = "unary_expr";
    primary_expr_type const primary_expr = "primary_expr";
    func_call_type const func_call = "func_call";

    auto dummy = init_all();

    auto const logical_or_expr_def =
            logical_and_expr
                    >> *(logical_or_op > logical_and_expr);

    auto const logical_and_expr_def =
            bitwise_or_expr
                    >> *(logical_and_op > bitwise_or_expr);

    auto const bitwise_or_expr_def =
            bitwise_xor_expr
                    >> *(bitwise_or_op >> !bitwise_or_op > bitwise_xor_expr);

    auto const bitwise_xor_expr_def =
            bitwise_and_expr
                    >> *(bitwise_xor_op > bitwise_and_expr);

    auto const bitwise_and_expr_def =
            equality_expr
                    >> *(bitwise_and_op >> !bitwise_and_op > equality_expr);

    auto const equality_expr_def =
            relational_expr
                    >> *(equality_op > relational_expr);

    auto const relational_expr_def =
            bitshift_expr
                    >> *(relational_op > bitshift_expr);

    auto const bitshift_expr_def =
            additive_expr
                    >> *(bitshift_op > additive_expr);

    auto const additive_expr_def =
            multiplicative_expr
                    >> *(additive_op > multiplicative_expr);

    auto const multiplicative_expr_def =
            unary_expr
                    >> *(multiplicative_op > unary_expr);

    auto const unary_expr_def =
            primary_expr
            | (unary_op > unary_expr);

    auto const primary_expr_def =
            string_literal
            | strict_float
            | int_
            | func_call
            | array_access
            | member_access
            | variable
            | ('(' > operand > ')');

    const auto func_call_def = variable
                               >> '('
                               > -(operand % ',')
                               > ')'
    ;

    const auto array_access_def = (member_access | variable)
                                  >> '['
                                  > operand
                                  > ']'
    ;

    const auto member_access_def = variable
                                   >> '.'
                                   >variable
    ;

    auto const operand_def = logical_or_expr;

    BOOST_SPIRIT_DEFINE(
            operand,
            logical_or_expr, logical_and_expr,
            bitwise_or_expr, bitwise_xor_expr, bitwise_and_expr,
            equality_expr, relational_expr,
            bitshift_expr,
            additive_expr, multiplicative_expr,
            unary_expr,
            primary_expr,
            func_call,
            array_access,
            member_access
    )

    struct unary_expr_class : x3::annotate_on_success {
    };
    struct primary_expr_class : x3::annotate_on_success {
    };

}
