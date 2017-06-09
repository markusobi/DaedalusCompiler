#pragma once

#include "ast.hpp"

namespace ASTVisitors
{
    class BaseVisitor
    {
    public:
        BaseVisitor(const std::string name)
        : name(name)
        {}

        virtual void start(ast::program& ast) {}

        virtual ~BaseVisitor() {}

        const std::string name;
    };
}