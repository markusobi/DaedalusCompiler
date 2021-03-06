/*=============================================================================
    Copyright (c) 2001-2014 Joel de Guzman

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
=============================================================================*/
#pragma once

#include "VisitorAdapter.hpp"
#include "ast.hpp"
#include "error_handler.hpp"
#include <vector>
#include <map>

namespace code_gen
{
    ///////////////////////////////////////////////////////////////////////////
    //  The Program
    ///////////////////////////////////////////////////////////////////////////
    struct program
    {
        void op(int a);
        void op(int a, int b);
        void op(int a, int b, int c);

        int& operator[](std::size_t i) { return code[i]; }
        int operator[](std::size_t i) const { return code[i]; }
        void clear() { code.clear(); variables.clear(); }
        std::size_t size() const { return code.size(); }
        std::vector<int> const& operator()() const { return code; }

        std::size_t nvars() const { return variables.size(); }
        int const* find_var(std::string const& name) const;
        void add_var(std::string const& name);

        void print_variables(std::vector<int> const& stack) const;
        void print_assembler() const;

    private:

        std::map<std::string, int> variables;
        std::vector<int> code;
    };

    ////////////////////////////////////////////////////////////////////////////
    //  The Compiler
    ////////////////////////////////////////////////////////////////////////////
    struct compiler : public ASTVisitors::VisitorAdapter<compiler, bool>
    {
        template <typename ErrorHandler>
        compiler(code_gen::program& program, ErrorHandler const& error_handler)
                : VisitorAdapter(error_handler)
                , program(program)
        {}

        result_type operator()(ast::nil) { BOOST_ASSERT(0); return false; }
        result_type operator()(unsigned int& x);
        result_type operator()(ast::variable& x);
        result_type operator()(ast::operation& x);
        result_type operator()(ast::unary& x);
        result_type operator()(ast::expression& x);
        result_type operator()(ast::assignment& x);
        result_type operator()(ast::variable_declaration& x);
        result_type operator()(ast::block& x);
        result_type operator()(ast::if_statement& x);
        result_type operator()(ast::while_statement& x);

        /**
         * default case: call base function
         */
        template <class T>
        result_type operator()(T& x)
        {
            return visitBase(x);
        }

        bool compile(ast::program& x);

        code_gen::program& program;
    };
}
