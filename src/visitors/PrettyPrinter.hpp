#pragma once

#include "BaseVisitor.hpp"
#include <ostream>
#include <fstream>

namespace ASTVisitors
{
    class PrettyPrinter : public BaseVisitor<PrettyPrinter>
    {
    public:
        PrettyPrinter(const ErrorHandler& errorHandler, const std::string& filename);
        PrettyPrinter(const ErrorHandler& errorHandler, std::ostream& out);

        void writeLine(const std::string& line);

        std::string indent();

        void addIndentation(int indent);

        template <class T>
        std::ostream& operator<<(T value)
        {
            return m_Out << value;
        }

        /**
         * ---------------------------------------------------------------
         * Visitor methods
         */


        /**
         * default case: call base function
         */
        template <class T>
        result_type operator()(T& x)
        {
            return static_cast<MyBase&>(*this).operator()(x);
        }

        /**
         * ---------------------------------------------------------------
         */

    protected:

        class Indentation
        {
            Indentation(PrettyPrinter& prettyPrinter, int indent = 1);

            ~Indentation();

        private:
            PrettyPrinter& m_PrettyPrinter;
            int m_IndentChange;
        };

        void init();

        std::ofstream m_File;
        std::ostream& m_Out;
        unsigned int m_Indentation;
    };
}
