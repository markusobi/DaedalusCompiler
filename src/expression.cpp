/*=============================================================================
    Copyright (c) 2001-2014 Joel de Guzman

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
=============================================================================*/
#include "config.hpp"

const parser::operand_type& getOperandParser()
{
    parser::add_keywords();
    return parser::operand;
}

const parser::array_access_type& getArrayAccessParser()
{
    return parser::array_access;
}

const parser::member_access_type& getMemberAccessParser()
{
    return parser::member_access;
}

const std::map<ast::optoken, std::string>& getOpTokenLookup()
{
    parser::add_keywords();
    return parser::op_names;
}

const x3::symbols<ast::optoken>& getCompoundAssignmentOperators()
{
    parser::add_keywords();
    return parser::compound_assignment;
}

namespace parser
{
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