#pragma once

#include "VisitorAdapter.hpp"
#include <ostream>
#include <fstream>
#include <expression.hpp>

namespace ASTVisitors
{
    class PrettyPrinter : public VisitorAdapter<PrettyPrinter>
    {
    public:
        PrettyPrinter(const ErrorHandler& errorHandler, const std::string& filename);
        PrettyPrinter(const ErrorHandler& errorHandler, std::ostream& out);

        void writeIndentedLine(const std::string &line);
        void writeIndented(const std::string& str);

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

        result_type operator()(ast::nil x)
        {
            *this << "<nil>";
        }

        result_type operator()(int& x)
        {
            *this << x;
        }

        result_type operator()(ast::variable& x)
        {
            *this << x.name;
        }

        result_type operator()(ast::operation& x)
        {
            *this << " " << getOpTokenLookup().at(x.operator_) << " ";
            return visitBase(x);
        }

        result_type operator()(ast::unary& x)
        {
            *this << getOpTokenLookup().at(x.operator_);
            return visitBase(x.operand_);
        }

        result_type operator()(ast::expression& x)
        {
            *this << "(";
            visitBase(x);
            *this << ")";
        }

        result_type operator()(ast::assignment& x)
        {
            visitDerived(x.lhs);
            *this << " = ";
            visitDerived(x.rhs);
        }

        result_type operator()(ast::variable_declaration& x)
        {
            *this << "var ";
            visitBase(x);
        }

        result_type operator()(ast::statement& x)
        {
            writeIndented("");
            visitBase(x);
            *this << ";\n";
        }

        result_type operator()(ast::statement_list& x)
        {
            writeIndentedLine("{");
            {
                Indentation ident(*this);
                visitBase(x);
            }
            writeIndentedLine("}");
        }

        result_type operator()(ast::if_statement& x)
        {
            *this << "if (";
            visitDerived(x.condition);
            *this << ")\n";
            visitDerived(x.then);
            if (x.else_)
            {
                writeIndentedLine("else");
                visitDerived(*x.else_);
            }
        }

        result_type operator()(ast::while_statement& x)
        {
            *this << "while (";
            visitDerived(x.condition);
            *this << ")\n";
            visitDerived(x.body);
        }

        result_type start(ast::statement_list& x)
        {
            return visitDerived(x);
        }

        /**
         * default case: call base function
         */
        template <class T>
        result_type operator()(T& x)
        {
            return visitBase(x);
        }

        /**
         * ---------------------------------------------------------------
         */

    protected:

        class Indentation
        {
        public:
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
