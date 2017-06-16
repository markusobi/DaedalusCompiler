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

    struct variable : x3::position_tagged {
        variable(std::string const &name = "") : name(name) {}
        std::string name;
    };

    struct type : x3::position_tagged {
        type(std::string const &name = "") : name(name) {}
        std::string name;
    };

    struct operand :
            x3::variant<
                    nil, int, variable, x3::forward_ast<unary>, x3::forward_ast<expression>
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

    struct assignment : x3::position_tagged {
        variable lhs;
        operand rhs;
    };

    struct variable_declaration {
        type type_;
        variable var;
        boost::optional<operand> rhs;
    };

    struct array_declaration {
        type type_;
        variable var;
        operand size;
        boost::optional<std::list<operand>> rhs;
    };

    struct if_statement;
    struct while_statement;
    struct statement_list;

    struct statement :
            x3::variant<
                    variable_declaration,
                    array_declaration,
                    assignment,
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

    typedef std::list<variable_declaration> param_list;

    struct function
    {
        // TODO use variable or other
        std::string identifier;
        param_list params;
    };

    typedef statement_list program;

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
