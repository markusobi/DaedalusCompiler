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

        result_type operator()(ast::nil) const {
            BOOST_ASSERT(0);
            return result_type();
        }

        result_type operator()(unsigned int x) const {
            // int literal
            return result_type();
        }

        result_type operator()(ast::variable const& x) const {
            // variable usage or declaration
            return result_type();
        }

        result_type operator()(ast::operation const& x) const {
            // operation i.e. * 2
            boost::apply_visitor(asDerived(), x.operand_);
            return result_type();
        }

        result_type operator()(ast::unary const& x) const {
            // unary operation
            boost::apply_visitor(asDerived(), x.operand_);
            return result_type();
        }

        result_type operator()(ast::expression const& x) const {
            boost::apply_visitor(asDerived(), x.first);
            for (ast::operation const &oper : x.rest) {
                asDerived()(oper);
            }
            return result_type();
        }

        result_type operator()(ast::assignment const& x) const {
            asDerived()(x.rhs);
            asDerived()(x.lhs);
            return result_type();
        }

        result_type operator()(ast::variable_declaration const& x) const {
            asDerived()(x.assign.lhs);
            asDerived()(x.assign.rhs);
            return result_type();
        }

        result_type operator()(ast::statement const& x) const {
            boost::apply_visitor(asDerived(), x);
            return result_type();
        }

        result_type operator()(ast::statement_list const& x) const {
            for (auto const &statement : x) {
                asDerived()(statement);
            }
            return result_type();
        }

        result_type operator()(ast::if_statement const& x) const {
            asDerived()(x.condition);
            asDerived()(x.then);
            if (x.else_)
                asDerived()(*x.else_);
            return result_type();
        }

        result_type operator()(ast::while_statement const& x) const {
            asDerived()(x.condition);
            asDerived()(x.body);
            return void();
        }

        result_type start(ast::statement_list const& x) const {
            asDerived()(x);
            return result_type();
        }

        error_handler_type error_handler;


    protected:
        using MyBase = BaseVisitor<Derived>;

    private:
        inline const Derived& asDerived() const
        {
            return static_cast<const Derived&>(*this);
        }
    };
}
