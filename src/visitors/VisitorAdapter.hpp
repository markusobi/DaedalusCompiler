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
#include "BaseVisitor.hpp"

namespace ASTVisitors
{
    ////////////////////////////////////////////////////////////////////////////
    //  The Adapted AST Visitor
    //  Curiously Recurring Template Pattern (CRTP)
    ////////////////////////////////////////////////////////////////////////////
    template <class Derived, class ResultType = void>
    class VisitorAdapter :  public BaseVisitor, public boost::static_visitor<ResultType>
    {
    public:
        // for the sake of readability of template error messages, ErrorHandler_ is not a template parameter anymore
        typedef parser::error_handler_type ErrorHandler_;
        typedef ErrorHandler_ ErrorHandler;
        typedef std::function<void(boost::spirit::x3::position_tagged, std::string const&)> error_handler_type;
        typedef VisitorAdapter<Derived, ResultType> BaseType;

        VisitorAdapter(ErrorHandler const& error_handler, const std::string& name="UnnamedVisitor") :
                BaseVisitor(name),
                error_handler(
                        [&error_handler](boost::spirit::x3::position_tagged pos, std::string const& msg)
                        {
                            BOOST_ASSERT_MSG(pos.id_first != -1 && pos.id_last != -1, "untagged ast object");
                            error_handler(pos, msg);
                        }
            )
        {}

        void start(ast::program& x) override
        {
            visitDerived(x);
        }

    protected:
        ResultType operator()(ast::nil)
        {
            BOOST_ASSERT(0);
            return ResultType();
        }

        ResultType operator()(unsigned int& x)
        {
            // int literal
            return ResultType();
        }

        ResultType operator()(float& x)
        {
            // float literal
            return ResultType();
        }

        ResultType operator()(std::string& x)
        {
            // string literal
            return ResultType();
        }

        ResultType operator()(ast::variable& x)
        {
            // variable usage or declaration
            return ResultType();
        }

        ResultType operator()(ast::memberAccess& x)
        {
            // member access on object
            visitDerived(x.object);
            visitDerived(x.object);
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
            visitDerived(x.operand_);
            return ResultType();
        }

        ResultType operator()(ast::unary& x)
        {
            // unary operation
            visitDerived(x.operand_);
            return ResultType();
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
            return ResultType();
        }

        ResultType operator()(ast::expression& x)
        {
            visitDerived(x.first);
            for (ast::operation& oper : x.rest) {
                visitDerived(oper);
            }
            return ResultType();
        }

        ResultType operator()(ast::func_call& x)
        {
            visitDerived(x.var);
            // TODO add template std::list<T> ?
            for (auto& op : x.args)
                visitDerived(op);
            return ResultType();
        }

        ResultType operator()(ast::array_access& x)
        {
            visitDerived(x.var);
            visitDerived(x.index);
            return ResultType();
        }

        ResultType operator()(ast::assignment& x)
        {
            visitDerived(x.rhs);
            visitDerived(x.lhs);
            return ResultType();
        }

        ResultType operator()(ast::typed_var& x)
        {
            visitDerived(x.type_);
            visitDerived(x.var);
            return ResultType();
        }

        ResultType operator()(ast::variable_declaration& x)
        {
            visitDerived(x.typed_var_);
            if (x.rhs)
                visitDerived(*x.rhs);
            return ResultType();
        }

        ResultType operator()(ast::array_declaration& x)
        {
            visitDerived(x.typed_var_);
            visitDerived(x.size);
            if (x.rhs)
            {
                for (auto& op : *x.rhs)
                    visitDerived(op);
            }
            return ResultType();
        }

        ResultType operator()(ast::type& x)
        {
            return ResultType();
        }

        ResultType operator()(ast::return_statement& x)
        {
            if (x.operand_)
                visitDerived(*x.operand_);
            return ResultType();
        }

        ResultType operator()(ast::statement& x)
        {
            // dispatch at runtime dependent on the type inside the variant
            return boost::apply_visitor(derived(), x);
        }

        ResultType operator()(ast::program& x)
        {
            for (auto& global_decl : x)
                visitDerived(global_decl);
            return ResultType();
        }

        ResultType operator()(ast::global_decl& x)
        {
            // dispatch at runtime dependent on the type inside the variant
            return boost::apply_visitor(derived(), x);
        }

        ResultType operator()(ast::function& x)
        {
            visitDerived(x.returnType);
            visitDerived(x.name);
            for (auto& decl : x.params)
                visitDerived(decl);
            visitDerived(x.body);
            return ResultType();
        }

        ResultType operator()(ast::prototype& x)
        {
            visitDerived(x.name);
            visitDerived(x.baseClassName);
            visitDerived(x.body);
            return ResultType();
        }

        ResultType operator()(ast::instance& x)
        {
            visitDerived(x.type_);
            visitDerived(x.name);
            visitDerived(x.body);
            return ResultType();
        }

        ResultType operator()(ast::instance_var_decl& x)
        {
            visitDerived(x.type_);
            for (auto& decl : x.vars) {
                visitDerived(decl);
            }
            return ResultType();
        }

        ResultType operator()(ast::extern_class& x)
        {
            visitDerived(x.name);
            visitDerived(x.body);
            return ResultType();
        }

        ResultType operator()(std::list<ast::multi_variable_declaration>& x)
        {
            for (auto& multi_decl : x) {
                visitDerived(multi_decl);
            }
            return ResultType();
        }

        ResultType operator()(ast::multi_variable_declaration& x)
        {
            visitDerived(x.type_);
            for (auto& decl : x.vars) {
                visitDerived(decl);
            }
            return ResultType();
        }

        ResultType operator()(ast::block& x)
        {
            for (auto& statement : x) {
                visitDerived(statement);
            }
            return ResultType();
        }

        ResultType operator()(ast::condition_block& x)
        {
            visitDerived(x.condition);
            visitDerived(x.then);
            return ResultType();
        }

        ResultType operator()(ast::if_statement& x)
        {
            for (auto& cb : x.condition_blocks)
                visitDerived(cb);
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

        /**
         * @return reference to this object as derived,
         * to delegate the dispatching to the derived class (static polymorphism)
         */
        inline Derived& derived()
        {
            return static_cast<Derived&>(*this);
        }

        /**
         * calls the derived visitor method
         * @param x ast node to visit
         */
        template <typename T>
        inline ResultType visitDerived(T& x)
        {
            return static_cast<Derived*>(this)->operator()(x);
        }

        /**
         * calls the base visitor method
         * @param x ast node to visit
         */
        template <typename T>
        inline ResultType visitBase(T& x)
        {
            return (*this)(x);
        }

        error_handler_type error_handler;
    };
}
