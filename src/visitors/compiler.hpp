/*=============================================================================
    Copyright (c) 2001-2014 Joel de Guzman

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
=============================================================================*/
#if !defined(BOOST_SPIRIT_X3_CALC9_COMPILER_HPP)
#define BOOST_SPIRIT_X3_CALC9_COMPILER_HPP

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
    struct compiler
    {
        typedef bool result_type;
        typedef std::function<
            void(boost::spirit::x3::position_tagged, std::string const&)>
        error_handler_type;

        template <typename ErrorHandler>
        compiler(
            code_gen::program& program
          , ErrorHandler const& error_handler)
          : program(program)
          , error_handler(
                [&](x3::position_tagged pos, std::string const& msg)
                {
                    BOOST_ASSERT_MSG(pos.id_first != -1 && pos.id_last != -1, "untagged ast object");
                    error_handler(pos, msg);
                }
            )
        {}

        result_type operator()(ast::nil) const { BOOST_ASSERT(0); return false; }
        result_type operator()(int x) const;
        result_type operator()(ast::variable const& x) const;
        result_type operator()(ast::operand const& x) const;
        result_type operator()(ast::operation const& x) const;
        result_type operator()(ast::unary const& x) const;
        result_type operator()(ast::expression const& x) const;
        result_type expression_visit_left_to_right(ast::expression const& x) const;
        result_type operator()(ast::assignment const& x) const;
        result_type operator()(ast::variable_declaration const& x) const;
        result_type operator()(ast::statement_list const& x) const;
        result_type operator()(ast::statement const& x) const;
        result_type operator()(ast::if_statement const& x) const;
        result_type operator()(ast::while_statement const& x) const;

        bool start(ast::program const& x) const;

        code_gen::program& program;
        error_handler_type error_handler;
    };
}

#endif
