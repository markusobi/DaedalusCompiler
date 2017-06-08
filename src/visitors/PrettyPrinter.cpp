#include "PrettyPrinter.hpp"


namespace ASTVisitors
{
    PrettyPrinter::PrettyPrinter(const ErrorHandler& errorHandler, const std::string& filename)
            : VisitorAdapter(errorHandler),
              m_File(filename),
              m_Out(m_File)
    {
        init();
    }

    PrettyPrinter::PrettyPrinter(const ErrorHandler& errorHandler, std::ostream& out)
            : VisitorAdapter(errorHandler),
              m_Out(out)
    {
        init();
    }

    void PrettyPrinter::init()
    {
        m_Indentation = 0;
    }

    void PrettyPrinter::addIndentation(int indent)
    {
        m_Indentation += indent;
    }

    void PrettyPrinter::writeIndentedLine(const std::string &line)
    {
        writeIndented(line);
        *this << '\n';
    }

    void PrettyPrinter::writeIndented(const std::string &str)
    {
        *this << indent() << str;
    }

    std::string PrettyPrinter::indent()
    {
        return std::string(m_Indentation * 4u, ' ');
    }

    PrettyPrinter::Indentation::Indentation(PrettyPrinter &prettyPrinter, int addIndent)
    : m_PrettyPrinter(prettyPrinter),
      m_IndentChange(addIndent)
    {
        m_PrettyPrinter.addIndentation(m_IndentChange);
    }

    PrettyPrinter::Indentation::~Indentation()
    {
        m_PrettyPrinter.addIndentation(-m_IndentChange);
    }
}