#include "ExpressionCollapse.hpp"


namespace ASTVisitors
{
    ExpressionCollapse::ExpressionCollapse(const ErrorHandler& errorHandler)
            : VisitorAdapter(errorHandler)
    {
    }
}