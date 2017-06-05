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

namespace Visitors
{
    ////////////////////////////////////////////////////////////////////////////
    //  The Base Visitor
    ////////////////////////////////////////////////////////////////////////////
    struct BaseVisitor
    {
        typedef void result_type;
        typedef std::function<void(boost::spirit::x3::position_tagged, std::string const&)> error_handler_type;

        template <typename ErrorHandler>
        BaseVisitor(ErrorHandler const& error_handler) :
                error_handler([&](boost::spirit::x3::position_tagged pos, std::string const& msg)
                { error_handler(pos, msg); }
            )
        {}

        result_type operator()(ast::nil) const { BOOST_ASSERT(0); return result_type(); }
        result_type operator()(unsigned int x) const;
        result_type operator()(bool x) const;
        result_type operator()(ast::variable const& x) const;
        result_type operator()(ast::operation const& x) const;
        result_type operator()(ast::unary const& x) const;
        result_type operator()(ast::expression const& x) const;
        result_type operator()(ast::assignment const& x) const;
        result_type operator()(ast::variable_declaration const& x) const;
        result_type operator()(ast::statement_list const& x) const;
        result_type operator()(ast::statement const& x) const;
        result_type operator()(ast::if_statement const& x) const;
        result_type operator()(ast::while_statement const& x) const;

        result_type start(ast::statement_list const& x) const;

        error_handler_type error_handler;
    };
}

#endif
