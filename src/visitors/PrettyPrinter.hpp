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

        result_type operator()(unsigned int& x)
        {
            *this << x;
        }

        result_type operator()(float& x)
        {
            *this << std::fixed << x;
        }

        result_type operator()(std::string& x)
        {
            *this << '"' << x << '"';
        }

        result_type operator()(ast::variable& x)
        {
            *this << x.name;
        }

        result_type operator()(ast::memberAccess& x)
        {
            visitDerived(x.object);
            *this << '.';
            visitDerived(x.member);
        }

        result_type operator()(ast::func_call& x)
        {
            visitDerived(x.var);
            *this << '(';
            join(x.args, ", ");
            *this << ')';
        }

        result_type operator()(ast::type& x)
        {
            *this << x.name;
        }

        result_type operator()(ast::operation& x)
        {
            *this << " " << parser::getOpTokenLookup().at(x.operator_) << " ";
            return visitBase(x);
        }

        result_type operator()(ast::unary& x)
        {
            *this << parser::getOpTokenLookup().at(x.operator_);
            return visitBase(x);
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
            *this << ' ' << parser::getOpTokenLookup().at(x.operator_) << ' ';
            visitDerived(x.rhs);
        }

        result_type operator()(ast::variable_declaration& x)
        {
            visitDerived(x.typed_var_);
            if (x.rhs)
            {
                *this << " = ";
                visitDerived(*x.rhs);
            }
        }

        result_type operator()(ast::statement& x)
        {
            writeIndented("");
            visitBase(x);
            *this << ";\n";
        }

        result_type operator()(ast::block& x)
        {
            writeIndentedLine("{");
            {
                Indentation ident(*this);
                visitBase(x);
            }
            writeIndented("}");
        }

        result_type operator()(ast::condition_block& x)
        {
            *this << "if (";
            visitDerived(x.condition);
            *this << ")\n";
            visitDerived(x.then);
        }

        result_type operator()(ast::if_statement& x)
        {
            join(x.condition_blocks, " else ");
            if (x.else_)
            {
                *this << " else\n";
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

        result_type operator()(ast::return_statement& x)
        {
            *this << "return";
            if (x.operand_)
            {
                *this << " ";
                visitBase(x);
            }
        }

        result_type operator()(ast::function& x)
        {
            writeIndented("func ");
            visitDerived(x.returnType);
            *this << ' ';
            visitDerived(x.name);
            *this << "(";
            join(x.params, ", ");
            *this << ")\n";
            visitDerived(x.body);
        }

        result_type operator()(ast::prototype& x)
        {
            writeIndented("prototype ");
            visitDerived(x.name);
            *this << "(";
            visitDerived(x.baseClassName);
            *this << ")\n";
            visitDerived(x.body);
        }

        result_type operator()(ast::instance_var_decl& x)
        {
            writeIndented("instance ");
            join(x.vars, ", ");
            *this << "(";
            visitDerived(x.type_);
            *this << ")";
        }

        result_type operator()(ast::instance& x)
        {
            writeIndented("instance ");
            visitDerived(x.name);
            *this << "(";
            visitDerived(x.className);
            *this << ")\n";
            visitDerived(x.body);
        }

        result_type operator()(ast::extern_class& x)
        {
            writeIndented("class ");
            visitDerived(x.name);
            *this << '\n';
            visitDerived(x.body);
        }

        result_type operator()(ast::multi_variable_declaration& x)
        {
            *this << (x.isConst ? "const " : "var ");
            visitDerived(x.type_);
            *this << ' ';
            join(x.vars, ", ");
        }

        result_type operator()(ast::typed_var& x)
        {
            *this << (x.isConst ? "const " : "var ");
            visitDerived(x.type_);
            *this << ' ';
            visitDerived(x.var);
        }

        result_type operator()(ast::array_declaration& x)
        {
            visitDerived(x.typed_var_);
            *this << '[';
            visitDerived(x.size);
            *this << ']';
            if (x.rhs)
            {
                *this << " = {";
                join(*x.rhs, ", ");
                *this << '}';
            }
        }

        result_type operator()(ast::array_access& x)
        {
            visitDerived(x.var);
            *this << '[';
            visitDerived(x.index);
            *this << ']';
        }

        result_type operator()(ast::global_decl& x)
        {
            writeIndented("");
            visitBase(x);
            *this << ";\n\n";
        }

        template <typename T>
        void join(T& iterable, const std::string& delim)
        {
            const auto begin = std::begin(iterable);
            const auto end = std::end(iterable);
            for (auto it = begin; it != end; ++it)
            {
                if (it != begin)
                    *this << delim;
                visitDerived(*it);
            }
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
