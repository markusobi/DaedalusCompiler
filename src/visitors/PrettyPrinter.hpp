#pragma once

#include "BaseVisitor.hpp"
#include <ostream>
#include <fstream>
#include <expression.hpp>

namespace ASTVisitors
{
    class PrettyPrinter : public BaseVisitor<PrettyPrinter>
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
            return boost::apply_visitor(*this, x.operand_);
        }

        result_type operator()(ast::unary& x)
        {
            *this << getOpTokenLookup().at(x.operator_);
            return boost::apply_visitor(*this, x.operand_);
        }

        result_type operator()(ast::expression& x)
        {
            if (!x.rest.empty() || true)
                *this << "(";

            boost::apply_visitor(*this, x.first);
            for (ast::operation& oper : x.rest) {
                (*this)(oper);
            }

            if (!x.rest.empty() || true)
                *this << ")";
        }

        result_type operator()(ast::assignment& x)
        {
            asDerived()(x.lhs);
            *this << " = ";
            asDerived()(x.rhs);
        }

        result_type operator()(ast::variable_declaration& x)
        {
            *this << "var ";
            asDerived()(x.assign);
        }

        result_type operator()(ast::statement& x)
        {
            writeIndented("");
            boost::apply_visitor(asDerived(), x);
            *this << ";\n";
        }

        result_type operator()(ast::statement_list& x)
        {
            for (auto& statement : x) {
                asDerived()(statement);
            }
        }

        result_type operator()(ast::if_statement& x)
        {
            writeIndented("if (");
            asDerived()(x.condition);
            *this << ")\n";
            writeIndentedLine("{");
            {
                Indentation ident(*this);
                asDerived()(x.then);
            }
            writeIndentedLine("}");
            if (x.else_)
            {
                writeIndentedLine("{");
                {
                    Indentation ident(*this);
                    asDerived()(*x.else_);
                }
                writeIndentedLine("}");
            }
        }

        result_type operator()(ast::while_statement& x)
        {
            writeIndented("while (");
            asDerived()(x.condition);
            *this << ")\n";

            writeIndentedLine("{");
            {
                Indentation ident(*this);
                asDerived()(x.body);
            }
            writeIndentedLine("}");
        }

        result_type start(ast::statement_list& x)
        {
            return asDerived()(x);
        }

        result_type operator()(ast::nil)
        {
            BOOST_ASSERT(0);
        }
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
