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
    namespace {
        std::map<ast::optoken, std::string> op_names;

        x3::symbols <ast::optoken> compound_assignment;

        x3::symbols <ast::optoken> logical_or_op;
        x3::symbols <ast::optoken> logical_and_op;
        x3::symbols <ast::optoken> bitwise_or_op;
        x3::symbols <ast::optoken> bitwise_xor_op;
        x3::symbols <ast::optoken> bitwise_and_op;
        x3::symbols <ast::optoken> equality_op;
        x3::symbols <ast::optoken> relational_op;
        x3::symbols <ast::optoken> bitshift_op;
        x3::symbols <ast::optoken> additive_op;
        x3::symbols <ast::optoken> multiplicative_op;
        x3::symbols <ast::optoken> unary_op;
    }

    void add_keywords();

    ////////////////////////////////////////////////////////////////////////////
    // Main expression grammar
    ////////////////////////////////////////////////////////////////////////////

    struct logical_or_expr_class;
    struct logical_and_expr_class;
    struct bitwise_or_expr_class;
    struct bitwise_xor_expr_class;
    struct bitwise_and_expr_class;
    struct equality_expr_class;
    struct relational_expr_class;
    struct bitshift_expr_class;
    struct additive_expr_class;
    struct multiplicative_expr_class;
    struct unary_expr_class;
    struct primary_expr_class;
    struct func_call_class;

    typedef x3::rule <logical_or_expr_class, ast::expression> logical_or_expr_type;
    typedef x3::rule <logical_and_expr_class, ast::expression> logical_and_expr_type;
    typedef x3::rule <bitwise_or_expr_class, ast::expression> bitwise_or_expr_type;
    typedef x3::rule <bitwise_xor_expr_class, ast::expression> bitwise_xor_expr_type;
    typedef x3::rule <bitwise_and_expr_class, ast::expression> bitwise_and_expr_type;
    typedef x3::rule <equality_expr_class, ast::expression> equality_expr_type;
    typedef x3::rule <relational_expr_class, ast::expression> relational_expr_type;
    typedef x3::rule <bitshift_expr_class, ast::expression> bitshift_expr_type;
    typedef x3::rule <additive_expr_class, ast::expression> additive_expr_type;
    typedef x3::rule <multiplicative_expr_class, ast::expression> multiplicative_expr_type;
    typedef x3::rule <unary_expr_class, ast::operand> unary_expr_type;
    typedef x3::rule <primary_expr_class, ast::operand> primary_expr_type;
    typedef x3::rule <func_call_class, ast::func_call> func_call_type;

    operand_type const operand = "operand";

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
    array_access_type const array_access = "array_access";
    member_access_type const member_access = "member_access";

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
