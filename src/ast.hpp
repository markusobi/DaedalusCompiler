/*=============================================================================
    Copyright (c) 2001-2014 Joel de Guzman

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
=============================================================================*/
#pragma once

#include <boost/spirit/home/x3/support/ast/variant.hpp>
#include <boost/spirit/home/x3/support/ast/position_tagged.hpp>
#include <boost/fusion/include/io.hpp>
#include <boost/optional.hpp>
#include <list>

namespace ast
{
    ///////////////////////////////////////////////////////////////////////////
    //  The AST
    ///////////////////////////////////////////////////////////////////////////
    namespace x3 = boost::spirit::x3;

    struct nil {
    };
    struct unary;
    struct expression;
    struct func_call;
    struct array_access;

    struct variable : x3::position_tagged {
        variable(std::string const &name = "") : name(name) {}
        std::string name;
    };

    struct type : x3::position_tagged {
        type(std::string const &name = "") : name(name) {}
        std::string name;
    };

    struct typed_var
    {
        type type_;
        variable var;
    };

    struct operand :
            x3::variant<
                    nil, int, variable,
                    x3::forward_ast<unary>,
                    x3::forward_ast<expression>,
                    x3::forward_ast<func_call>,
                    x3::forward_ast<array_access>
            > {
        using base_type::base_type;
        using base_type::operator=;
    };

    enum optoken {
        op_plus,
        op_minus,
        op_times,
        op_divide,
        op_modulo,
        op_positive,
        op_negative,
        op_logical_not,
        op_bitwise_not,
        op_shift_left,
        op_shift_right,
        op_equal,
        op_not_equal,
        op_less,
        op_less_equal,
        op_greater,
        op_greater_equal,
        op_bitwise_and,
        op_bitwise_xor,
        op_bitwise_or,
        op_logical_and,
        op_logical_or
    };

    struct unary {
        optoken operator_;
        operand operand_;
    };

    struct operation : x3::position_tagged {
        optoken operator_;
        operand operand_;
    };

    struct binary_expression : x3::position_tagged {
        operand left;
        operand right;
        optoken operator_;
    };

    struct expression : x3::position_tagged {
        operand first;
        std::list<operation> rest;
    };

    struct func_call : x3::position_tagged {
        variable var;
        std::list<operand> args;
    };

    struct array_access : x3::position_tagged {
        variable var;
        operand index;
    };

    struct assignment : x3::position_tagged {
        operand lhs;
        operand rhs;
    };

    struct variable_declaration {
        typed_var typed_var_;
        boost::optional<operand> rhs;
    };

    struct array_declaration {
        typed_var typed_var_;
        operand size;
        boost::optional<std::list<operand>> rhs;
    };

    struct return_statement
    {
        operand operand_;
    };

    struct if_statement;
    struct while_statement;
    struct statement_list;

    struct statement :
            x3::variant<
                    variable_declaration,
                    array_declaration,
                    assignment,
                    return_statement,
                    operand,
                    boost::recursive_wrapper<if_statement>,
                    boost::recursive_wrapper<while_statement>,
                    boost::recursive_wrapper<statement_list>
            > {
        using base_type::base_type;
        using base_type::operator=;
    };

    typedef statement_list block;

    struct statement_list : std::list<statement> {
    };

    struct if_statement {
        operand condition;
        block then;
        boost::optional<block> else_;
    };

    struct while_statement {
        operand condition;
        block body;
    };

    typedef std::list<typed_var> param_list;

    struct function
    {
        type type_;
        variable var;
        param_list params;
        block body;
    };

    typedef x3::variant<
            variable_declaration, // TODO visitor check forbid global write on non-const
            array_declaration, // TODO visitor check forbid global write on non-const
            function
            // TODO more
    > global_decl;

    typedef std::list<global_decl> program;

    // print functions for debugging
    inline std::ostream &operator<<(std::ostream &out, nil) {
        out << "nil";
        return out;
    }

    inline std::ostream &operator<<(std::ostream &out, variable const &var) {
        out << var.name;
        return out;
    }
}
