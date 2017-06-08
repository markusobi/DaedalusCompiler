/*=============================================================================
    Copyright (c) 2001-2014 Joel de Guzman

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
=============================================================================*/
#pragma once

#include <iostream>
#include "ast.hpp"
#include "error_handler.hpp"
#include "config.hpp"

namespace ASTVisitors
{
    ////////////////////////////////////////////////////////////////////////////
    //  The Base AST Visitor
    //  Curiously Recurring Template Pattern (CRTP)
    ////////////////////////////////////////////////////////////////////////////
    template <class Derived, class ResultType = void>
    class BaseVisitor : public boost::static_visitor<ResultType>
    {
    public:
        // for the sake of readability of template error messages, ErrorHandler_ is not a template parameter anymore
        typedef parser::error_handler_type ErrorHandler_;
        typedef ErrorHandler_ ErrorHandler;
        typedef std::function<void(boost::spirit::x3::position_tagged, std::string const&)> error_handler_type;

        BaseVisitor(ErrorHandler const& error_handler) :
                error_handler([&error_handler](boost::spirit::x3::position_tagged pos, std::string const& msg)
                { error_handler(pos, msg); }
            )
        {}

        ResultType operator()(ast::nil)
        {
            BOOST_ASSERT(0);
            return ResultType();
        }

        ResultType operator()(int& x)
        {
            // int literal
            return ResultType();
        }

        ResultType operator()(ast::variable& x)
        {
            // variable usage or declaration
            return ResultType();
        }

        ResultType operator()(ast::operand& x)
        {
            // dispatch at runtime dependent on the type inside the variant
            return boost::apply_visitor(derived(), x);
        }

        ResultType operator()(ast::operation& x)
        {
            // operation i.e. * 2
            return visitDerived(x.operand_);
        }

        ResultType operator()(ast::unary& x)
        {
            // unary operation
            return visitDerived(x.operand_);
        }

        template <class Inner>
        ResultType operator()(x3::forward_ast<Inner>& x)
        {
            // access wrapped inner object
            return visitDerived(static_cast<Inner&>(x));
        }

        template <class T>
        ResultType operator()(T& x)
        {
            static_assert(sizeof(T) == 0, "missing visitor method overload in base visitor for type T");
        }

        ResultType operator()(ast::expression& x)
        {
            visitDerived(x.first);
            for (ast::operation& oper : x.rest) {
                visitDerived(oper);
            }
            return ResultType();
        }

        ResultType operator()(ast::assignment& x)
        {
            visitDerived(x.rhs);
            visitDerived(x.lhs);
            return ResultType();
        }

        ResultType operator()(ast::variable_declaration& x)
        {
            visitDerived(x.assign);
            return ResultType();
        }

        ResultType operator()(ast::statement& x)
        {
            // dispatch at runtime dependent on the type inside the variant
            return boost::apply_visitor(derived(), x);
        }

        ResultType operator()(ast::statement_list& x)
        {
            for (auto& statement : x) {
                visitDerived(statement);
            }
            return ResultType();
        }

        ResultType operator()(ast::if_statement& x)
        {
            visitDerived(x.condition);
            visitDerived(x.then);
            if (x.else_)
                visitDerived(*x.else_);
            return ResultType();
        }

        ResultType operator()(ast::while_statement& x)
        {
            visitDerived(x.condition);
            visitDerived(x.body);
            return ResultType();
        }

        ResultType start(ast::statement_list& x)
        {
            return visitDerived(x);
        }

        error_handler_type error_handler;


    protected:
        using MyBase = BaseVisitor<Derived>;

        /**
         * @return reference to this object as derived,
         * to delegate the dispatching to the derived class (static polymorphism)
         */
        inline Derived& derived()
        {
            return static_cast<Derived&>(*this);
        }

        template <typename T>
        inline ResultType visitDerived(T& x)
        {
            return static_cast<Derived*>(this)->operator()(x);
        }

        template <typename T>
        inline ResultType visitBase(T& x)
        {
            return static_cast<BaseVisitor<Derived>*>(this)->operator()(x);
        }

    };
}
