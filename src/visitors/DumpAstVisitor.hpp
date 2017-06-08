#pragma once

#include "visitors/VisitorAdapter.hpp"

namespace ASTVisitors
{
    class DumpAstVisitor : public VisitorAdapter<DumpAstVisitor>
    {
    public:
        DumpAstVisitor(const ErrorHandler& errorHandler);

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