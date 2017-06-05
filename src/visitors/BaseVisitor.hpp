/*=============================================================================
    Copyright (c) 2001-2014 Joel de Guzman

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
=============================================================================*/
#pragma once

#include "ast.hpp"
#include "error_handler.hpp"
#include "config.hpp"
#include <vector>
#include <map>

namespace ASTVisitors
{
    ////////////////////////////////////////////////////////////////////////////
    //  The Base AST Visitor
    ////////////////////////////////////////////////////////////////////////////
    template <class Derived, class ErrorHandler_ = parser::error_handler_type>
    class BaseVisitor
    {
    public:
        typedef void ResultType;
        typedef ErrorHandler_ ErrorHandler;
        typedef std::function<void(boost::spirit::x3::position_tagged, std::string const&)> error_handler_type;

        BaseVisitor(ErrorHandler const& error_handler) :
                error_handler([&](boost::spirit::x3::position_tagged pos, std::string const& msg)
                { error_handler(pos, msg); }
            )
        {}

        ResultType operator()(ast::nil) const {
            BOOST_ASSERT(0); return ResultType();
        }

        ResultType operator()(unsigned int x) const {
            // int literal
            return ResultType();
        }

        ResultType operator()(ast::variable const& x) const {
            // variable usage or declaration
            return ResultType();
        }

        ResultType operator()(ast::operation const& x) const {
            // operation i.e. * 2
            boost::apply_visitor(*this, x.operand_);
            return ResultType();
        }

        ResultType operator()(ast::unary const& x) const {
            // unary operation
            boost::apply_visitor(*this, x.operand_);
            return ResultType();
        }

        ResultType operator()(ast::expression const& x) const {
            boost::apply_visitor(*this, x.first);
            for (ast::operation const &oper : x.rest) {
                asDerived()(oper);
            }
            return ResultType();
        }

        ResultType operator()(ast::assignment const& x) const {
            asDerived()(x.rhs);
            asDerived()(x.lhs);
            return ResultType();
        }

        ResultType operator()(ast::variable_declaration const& x) const {
            asDerived()(x.assign.lhs);
            asDerived()(x.assign.rhs);
            return ResultType();
        }

        ResultType operator()(ast::statement const& x) const {
            boost::apply_visitor(*this, x);
            return ResultType();
        }

        ResultType operator()(ast::statement_list const& x) const {
            for (auto const &statement : x) {
                asDerived()(statement);
            }
            return ResultType();
        }

        ResultType operator()(ast::if_statement const& x) const {
            asDerived()(x.condition);
            asDerived()(x.then);
            if (x.else_)
                asDerived()(*x.else_);
            return ResultType();
        }

        ResultType operator()(ast::while_statement const& x) const {
            asDerived()(x.condition);
            asDerived()(x.body);
            return void();
        }

        ResultType start(ast::statement_list const& x) const {
            asDerived()(x);
            return ResultType();
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
