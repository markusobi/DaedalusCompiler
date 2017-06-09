/*=============================================================================
    Copyright (c) 2001-2014 Joel de Guzman

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
=============================================================================*/
#include "compiler.hpp"
#include "vm.hpp"
#include <boost/foreach.hpp>
#include <boost/variant/apply_visitor.hpp>
#include <boost/assert.hpp>
#include <boost/lexical_cast.hpp>
#include <iostream>
#include <set>

namespace code_gen {
    using result_type = compiler::result_type;

    void program::op(int a) {
        code.push_back(a);
    }

    void program::op(int a, int b) {
        code.push_back(a);
        code.push_back(b);
    }

    void program::op(int a, int b, int c) {
        code.push_back(a);
        code.push_back(b);
        code.push_back(c);
    }

    int const *program::find_var(std::string const &name) const {
        auto i = variables.find(name);
        if (i == variables.end())
            return 0;
        return &i->second;
    }

    void program::add_var(std::string const &name) {
        std::size_t n = variables.size();
        variables[name] = int(n);
    }

    void program::print_variables(std::vector<int> const &stack) const {
        for (auto const &p : variables) {
            std::cout << "    " << p.first << ": " << stack[p.second] << std::endl;
        }
    }

    void program::print_assembler() const {
        auto pc = code.begin();

        std::vector<std::string> locals(variables.size());
        typedef std::pair<std::string, int> pair;
        for (pair const &p : variables) {
            locals[p.second] = p.first;
            std::cout << "local       "
                      << p.first << ", @" << p.second << std::endl;
        }

        std::map<std::size_t, std::string> lines;
        std::set<std::size_t> jumps;

        while (pc != code.end()) {
            std::string line;
            std::size_t address = pc - code.begin();

            switch (*pc++) {
                case op_pos:
                    line += "      op_pos";
                    break;

                case op_neg:
                    line += "      op_neg";
                    break;

                case op_lognot:
                    line += "      op_lognot";
                    break;

                case op_bitnot:
                    line += "      op_bitnot";
                    break;

                case op_add:
                    line += "      op_add";
                    break;

                case op_sub:
                    line += "      op_sub";
                    break;

                case op_mul:
                    line += "      op_mul";
                    break;

                case op_div:
                    line += "      op_div";
                    break;

                case op_mod:
                    line += "      op_mod";
                    break;

                case op_shift_left:
                    line += "      op_shift_left";
                    break;

                case op_shift_right:
                    line += "      op_shift_right";
                    break;

                case op_eq:
                    line += "      op_eq";
                    break;

                case op_neq:
                    line += "      op_neq";
                    break;

                case op_lt:
                    line += "      op_lt";
                    break;

                case op_lte:
                    line += "      op_lte";
                    break;

                case op_gt:
                    line += "      op_gt";
                    break;

                case op_gte:
                    line += "      op_gte";
                    break;

                case op_bitwise_or:
                    line += "      op_bitwise_or";
                    break;

                case op_bitwise_xor:
                    line += "      op_bitwise_xor";
                    break;

                case op_bitwise_and:
                    line += "      op_bitwise_and";
                    break;

                case op_logical_and:
                    line += "      op_logical_and";
                    break;

                case op_logical_or:
                    line += "      op_logical_or";
                    break;

                case op_load:
                    line += "      op_load     ";
                    line += boost::lexical_cast<std::string>(locals[*pc++]);
                    break;

                case op_store:
                    line += "      op_store    ";
                    line += boost::lexical_cast<std::string>(locals[*pc++]);
                    break;

                case op_int:
                    line += "      op_int      ";
                    line += boost::lexical_cast<std::string>(*pc++);
                    break;

                case op_jump: {
                    line += "      op_jump     ";
                    std::size_t pos = (pc - code.begin()) + *pc++;
                    if (pos == code.size())
                        line += "end";
                    else
                        line += boost::lexical_cast<std::string>(pos);
                    jumps.insert(pos);
                }
                    break;

                case op_jump_if: {
                    line += "      op_jump_if  ";
                    std::size_t pos = (pc - code.begin()) + *pc++;
                    if (pos == code.size())
                        line += "end";
                    else
                        line += boost::lexical_cast<std::string>(pos);
                    jumps.insert(pos);
                }
                    break;

                case op_stk_adj:
                    line += "      op_stk_adj  ";
                    line += boost::lexical_cast<std::string>(*pc++);
                    break;
            }
            lines[address] = line;
        }

        std::cout << "start:" << std::endl;
        for (auto const &l : lines) {
            std::size_t pos = l.first;
            if (jumps.find(pos) != jumps.end())
                std::cout << pos << ':' << std::endl;
            std::cout << l.second << std::endl;
        }

        std::cout << "end:" << std::endl;
    }

    result_type compiler::operator()(int& x) {
        program.op(op_int, x);
        return true;
    }

    result_type compiler::operator()(ast::variable& x) {
        int const *p = program.find_var(x.name);
        if (p == 0) {
            error_handler(x, "Undeclared variable: " + x.name);
            return false;
        }
        program.op(op_load, *p);
        return true;
    }

    result_type compiler::operator()(ast::operation& x) {
        if (!visitDerived(x.operand_))
            return false;
        switch (x.operator_) {
            case ast::op_plus:
                program.op(op_add);
                break;
            case ast::op_minus:
                program.op(op_sub);
                break;
            case ast::op_times:
                program.op(op_mul);
                break;
            case ast::op_divide:
                program.op(op_div);
                break;
            case ast::op_modulo:
                program.op(op_mod);
                break;

            case ast::op_shift_left:
                program.op(op_shift_left);
                break;
            case ast::op_shift_right:
                program.op(op_shift_right);
                break;

            case ast::op_equal:
                program.op(op_eq);
                break;
            case ast::op_not_equal:
                program.op(op_neq);
                break;
            case ast::op_less:
                program.op(op_lt);
                break;
            case ast::op_less_equal:
                program.op(op_lte);
                break;
            case ast::op_greater:
                program.op(op_gt);
                break;
            case ast::op_greater_equal:
                program.op(op_gte);
                break;

            case ast::op_bitwise_or:
                program.op(op_bitwise_or);
                break;
            case ast::op_bitwise_xor:
                program.op(op_bitwise_xor);
                break;
            case ast::op_bitwise_and:
                program.op(op_bitwise_and);
                break;

            case ast::op_logical_and:
                program.op(op_logical_and);
                break;
            case ast::op_logical_or:
                program.op(op_logical_or);
                break;
            default:
                BOOST_ASSERT(0);
                return false;
        }
        return true;
    }

    result_type compiler::operator()(ast::unary& x) {
        if (!visitDerived(x.operand_))
            return false;
        switch (x.operator_) {
            case ast::op_positive:
                program.op(op_pos);
                break;
            case ast::op_negative:
                program.op(op_neg);
                break;
            case ast::op_logical_not:
                program.op(op_lognot);
                break;
            case ast::op_bitwise_not:
                program.op(op_bitnot);
                break;
            default:
                BOOST_ASSERT(0);
                return false;
        }
        return true;
    }

    result_type compiler::operator()(ast::expression& x)
    {
        if (!visitDerived(x.first))
            return false;
        for (ast::operation& oper : x.rest) {
            if (!visitDerived(oper))
                return false;
        }
        return true;
    }

    result_type compiler::operator()(ast::assignment& x) {
        if (!visitDerived(x.rhs))
            return false;
        int const *p = program.find_var(x.lhs.name);
        if (p == 0) {
            error_handler(x.lhs, "Undeclared variable: " + x.lhs.name);
            return false;
        }
        program.op(op_store, *p);
        return true;
    }

    result_type compiler::operator()(ast::variable_declaration& x) {
        int const *p = program.find_var(x.assign.lhs.name);
        if (p != 0) {
            error_handler(x.assign.lhs, "Duplicate variable: " + x.assign.lhs.name);
            return false;
        }
        bool r = visitDerived(x.assign.rhs);
        if (r) // don't add the variable if the RHS fails
        {
            program.add_var(x.assign.lhs.name);
            program.op(op_store, *program.find_var(x.assign.lhs.name));
        }
        return r;
    }

    result_type compiler::operator()(ast::statement_list& x) {
        for (auto& s : x) {
            if (!visitDerived(s))
                return false;
        }
        return true;
    }

    result_type compiler::operator()(ast::if_statement& x) {
        if (!visitDerived(x.condition))
            return false;
        program.op(op_jump_if, 0);                      // we shall fill this (0) in later
        std::size_t skip = program.size() - 1;            // mark its position
        if (!visitDerived(x.then))
            return false;
        program[skip] = int(program.size() - skip);       // now we know where to jump to (after the if branch)

        if (x.else_)                                    // We got an alse
        {
            program[skip] += 2;                         // adjust for the "else" jump
            program.op(op_jump, 0);                     // we shall fill this (0) in later
            std::size_t exit = program.size() - 1;        // mark its position
            if (!visitDerived(*x.else_))
                return false;
            program[exit] = int(program.size() - exit);   // now we know where to jump to (after the else branch)
        }

        return true;
    }

    result_type compiler::operator()(ast::while_statement& x) {
        std::size_t loop = program.size();              // mark our position
        if (!visitDerived(x.condition))
            return false;
        program.op(op_jump_if, 0);                      // we shall fill this (0) in later
        std::size_t exit = program.size() - 1;            // mark its position
        if (!visitDerived(x.body))
            return false;
        program.op(op_jump,
                   int(loop - 1) - int(program.size()));         // loop back
        program[exit] = int(program.size() - exit);       // now we know where to jump to (to exit the loop)
        return true;
    }

    result_type compiler::compile(ast::program& x) {
        program.clear();
        // op_stk_adj 0 for now. we'll know how many variables we'll have later
        program.op(op_stk_adj, 0);

        if (!visitDerived(x)) {
            program.clear();
            return false;
        }
        program[1] = int(program.nvars());              // now store the actual number of variables
        return true;
    }
}

