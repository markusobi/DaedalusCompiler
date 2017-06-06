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
    template <class Derived, class ResultType = void, class ErrorHandler_ = parser::error_handler_type>
    class BaseVisitor
    {
    public:
        // boost::apply_visitor demands a type named result_type
        typedef ResultType result_type;
        typedef ErrorHandler_ ErrorHandler;
        typedef std::function<void(boost::spirit::x3::position_tagged, std::string const&)> error_handler_type;

        BaseVisitor(ErrorHandler const& error_handler) :
                error_handler([&error_handler](boost::spirit::x3::position_tagged pos, std::string const& msg)
                { error_handler(pos, msg); }
            )
        {}

        result_type operator()(ast::nil)
        {
            BOOST_ASSERT(0);
            return result_type();
        }

        result_type operator()(int& x)
        {
            // int literal
            return result_type();
        }

        result_type operator()(ast::variable& x)
        {
            // variable usage or declaration
            return result_type();
        }

        result_type operator()(ast::operation& x)
        {
            // operation i.e. * 2
            return boost::apply_visitor(asDerived(), x.operand_);
        }

        result_type operator()(ast::unary& x)
        {
            // unary operation
            return boost::apply_visitor(asDerived(), x.operand_);
        }

        result_type operator()(ast::expression& x)
        {
            boost::apply_visitor(asDerived(), x.first);
            for (ast::operation& oper : x.rest) {
                asDerived()(oper);
            }
            return result_type();
        }

        result_type operator()(ast::assignment& x)
        {
            asDerived()(x.rhs);
            asDerived()(x.lhs);
            return result_type();
        }

        result_type operator()(ast::variable_declaration& x)
        {
            asDerived()(x.assign.lhs);
            asDerived()(x.assign.rhs);
            return result_type();
        }

        result_type operator()(ast::statement& x)
        {
            return boost::apply_visitor(asDerived(), x);
        }

        result_type operator()(ast::statement_list& x)
        {
            for (auto& statement : x) {
                asDerived()(statement);
            }
            return result_type();
        }

        result_type operator()(ast::if_statement& x)
        {
            asDerived()(x.condition);
            asDerived()(x.then);
            if (x.else_)
                asDerived()(*x.else_);
            return result_type();
        }

        result_type operator()(ast::while_statement& x)
        {
            asDerived()(x.condition);
            asDerived()(x.body);
            return result_type();
        }

        result_type start(ast::statement_list& x)
        {
            return asDerived()(x);
        }

        error_handler_type error_handler;


    protected:
        using MyBase = BaseVisitor<Derived>;

    private:
        inline Derived& asDerived()
        {
            return static_cast<Derived&>(*this);
        }
    };
}
