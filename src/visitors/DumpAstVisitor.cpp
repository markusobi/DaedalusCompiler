#include "DumpAstVisitor.hpp"

namespace ASTVisitors
{
    DumpAstVisitor::DumpAstVisitor(const ErrorHandler& errorHandler)
            : VisitorAdapter(errorHandler)
    {
    }
}

