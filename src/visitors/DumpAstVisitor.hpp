#pragma once

#include "visitors/BaseVisitor.hpp"

namespace ASTVisitors
{
    class DumpAstVisitor : public BaseVisitor<DumpAstVisitor>
    {
    public:
        DumpAstVisitor(const ErrorHandler& errorHandler);

        /**
         * default case: call base function
         */
        template <class T>
        result_type operator()(T& x)
        {
            return static_cast<MyBase&>(*this).operator()(x);
        }
    };
}