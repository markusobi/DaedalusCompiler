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

    operand_type const operand = "operand";

    array_access_type const array_access = "array_access";
    member_access_type const member_access = "member_access";

    namespace {
        auto dummy = init_all();
    }

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

namespace parser
{
    const operand_type& getOperandParser()
    {
        add_keywords();
        return operand;
    }

    const array_access_type& getArrayAccessParser()
    {
        return array_access;
    }

    const member_access_type& getMemberAccessParser()
    {
        return member_access;
    }

    const std::map<ast::optoken, std::string>& getOpTokenLookup()
    {
        add_keywords();
        return op_names;
    }

    const x3::symbols<ast::optoken>& getCompoundAssignmentOperators()
    {
        add_keywords();
        return compound_assignment;
    }


    int init_all()
    {
        add_keywords();
        return 0;
    }

    void add_keywords() {
        static bool once = false;
        if (once)
            return;
        once = true;

/*        std::map<ast::optoken, std::string> op_names_local;
        auto registerOperator = [](x3::symbols<ast::optoken>& parser, const std::string name, ast::optoken id){
            op_names[id] = name;
            parser.add(name, id);
        };*/

        compound_assignment.add
                ("+=", ast::op_assign_plus)
                ("-=", ast::op_assign_minus)
                ("*=", ast::op_assign_times)
                ("/=", ast::op_assign_divide)
                ("%=", ast::op_assign_modulo)
                ("=", ast::op_assign);

        logical_or_op.add
                ("||", ast::op_logical_or);

        logical_and_op.add
                ("&&", ast::op_logical_and);

        bitwise_or_op.add
                ("|", ast::op_bitwise_or);

        bitwise_xor_op.add
                ("^", ast::op_bitwise_xor);

        bitwise_and_op.add
                ("&", ast::op_bitwise_and);

        equality_op.add
                ("==", ast::op_equal)
                ("!=", ast::op_not_equal);

        relational_op.add
                ("<", ast::op_less)
                ("<=", ast::op_less_equal)
                (">", ast::op_greater)
                (">=", ast::op_greater_equal);

        bitshift_op.add
                ("<<", ast::op_shift_left)
                (">>", ast::op_shift_right);

        additive_op.add
                ("+", ast::op_plus)
                ("-", ast::op_minus);

        multiplicative_op.add
                ("*", ast::op_times)
                ("/", ast::op_divide)
                ("%", ast::op_modulo);

        unary_op.add
                ("+", ast::op_positive)
                ("-", ast::op_negative)
                ("!", ast::op_logical_not)
                ("~", ast::op_bitwise_not);

        std::list<x3::symbols<ast::optoken>> tables = {compound_assignment, logical_or_op, logical_and_op, bitwise_or_op, bitwise_xor_op, bitwise_and_op,
                                                       equality_op, relational_op, bitshift_op, additive_op, multiplicative_op, unary_op};

        for (auto& table : tables)
        {
            table.add.sym.lookup.get()->for_each([](const std::string name, ast::optoken id){
                op_names[id] = name;
            });
        }
    }
}