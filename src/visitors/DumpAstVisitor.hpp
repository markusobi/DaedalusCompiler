#pragma once

#include "visitors/BaseVisitor.hpp"

namespace ASTVisitors
{
    class DumpAstVisitor : public BaseVisitor<DumpAstVisitor>
    {
    public:
        DumpAstVisitor(const ErrorHandler& errorHandler);

    };
}