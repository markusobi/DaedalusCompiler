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
        result_type operator()(const T& x) const
        {
            return static_cast<const MyBase&>(*this).operator()(x);
        }
    };
}