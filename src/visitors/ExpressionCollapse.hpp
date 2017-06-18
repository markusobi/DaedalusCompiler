#pragma once

#include "visitors/VisitorAdapter.hpp"

namespace ASTVisitors
{
    class ExpressionCollapse : public VisitorAdapter<ExpressionCollapse, boost::optional<ast::operand>>
    {
    public:
        ExpressionCollapse(const ErrorHandler& errorHandler);

        result_type operator()(ast::operand& x)
        {
            auto result = visitBase(x);
            if (result)
                x = *result;
            return x;
        }

        result_type operator()(ast::unary& x)
        {
            visitBase(x);
            return ast::operand(x);
        }

        result_type operator()(ast::expression& x)
        {
            if (x.rest.empty())
                return visitDerived(x.first);
            else{
                visitBase(x);
                return result_type();
            }
        }
        
        /**
         * default case: call base function
         */
        template <class T>
        result_type operator()(T& x)
        {
            return visitBase(x);
        }
    };
}
