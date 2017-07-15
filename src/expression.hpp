/*=============================================================================
    Copyright (c) 2001-2014 Joel de Guzman

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
=============================================================================*/
#pragma once

#include <boost/spirit/home/x3.hpp>
#include "ast.hpp"
#include <map>

namespace x3 = boost::spirit::x3;
namespace parser {
    struct operand_class;
    struct array_access_class;
    struct member_access_class;

    typedef x3::rule<operand_class, ast::operand> operand_type;
    typedef x3::rule<array_access_class, ast::array_access> array_access_type;
    typedef x3::rule <member_access_class, ast::memberAccess> member_access_type;


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
    int init_all();

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

    array_access_type const array_access = "array_access";
    member_access_type const member_access = "member_access";
}

const parser::operand_type& getOperandParser();

const parser::array_access_type& getArrayAccessParser();

const parser::member_access_type& getMemberAccessParser();

const std::map<ast::optoken, std::string>& getOpTokenLookup();

const x3::symbols<ast::optoken>& getCompoundAssignmentOperators();
