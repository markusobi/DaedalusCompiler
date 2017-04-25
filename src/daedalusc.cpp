#include <iostream>
#include <vector>
#include <boost/config/warning_disable.hpp>
#include <boost/spirit/include/qi.hpp>
#include <boost/spirit/include/phoenix_core.hpp>
#include <boost/spirit/include/phoenix_operator.hpp>
#include <boost/fusion/include/adapt_struct.hpp>
#include <boost/variant.hpp>

namespace qi = boost::spirit::qi;
namespace ascii = boost::spirit::ascii;
namespace latin1 = boost::spirit::iso8859_1;
namespace encoding = latin1;
namespace phoenix = boost::phoenix;


qi::symbols<char> keywords;

struct Identifier{
    std::string name;
};

struct Type{
    std::string name;
};

struct VarDecl{
    Type type;
    Identifier identifier;
};


using Expression = boost::variant<Identifier, int, float, std::string>;

struct AddExpr{
    Expression left;
    char operatorToken;
    Expression right;
};

struct ExprStatement{
    Expression expression;
};

using Statement = boost::variant<VarDecl, Expression>;

struct Block{
    std::vector<Statement> statements;
};

struct FuncDecl{
    VarDecl varDecl;
    std::vector<VarDecl> parameters;
    //Block block;
};
BOOST_FUSION_ADAPT_STRUCT(
        Identifier,
        (std::string, name)
)

BOOST_FUSION_ADAPT_STRUCT(
        Type,
        (std::string, name)
)

BOOST_FUSION_ADAPT_STRUCT(
        VarDecl,
        (Type, type)
        (Identifier, identifier)
)

BOOST_FUSION_ADAPT_STRUCT(
        FuncDecl,
        (VarDecl, varDecl)
        (std::vector<VarDecl>, parameters)
)

BOOST_FUSION_ADAPT_STRUCT(
        Block,
        (std::vector<Statement>, statements)
)

using Declaration = boost::variant<VarDecl, FuncDecl>;

struct Program{
    std::vector<Declaration> declarations;
};

struct ExpressionVisitor{
    void operator()(const int& value) const{
        std::cout << "IntLiteral: " << value << std::endl;
    }
    void operator()(const float& value) const{
        std::cout << "FloatLiteral: " << value << std::endl;
    }
    void operator()(const std::string& value) const{
        std::cout << "StringLiteral: " << value << std::endl;
    }
    void operator()(const Identifier& identifier) const{
        std::cout << "Identifier: " << identifier.name << std::endl;
    }
};
using SkipperType = encoding::space_type;

template <class Iterator>
struct DaedalusProgramParser : qi::grammar<Iterator, Expression(), SkipperType>{
    DaedalusProgramParser() : DaedalusProgramParser::base_type(assignExpression)
    {
        using qi::lexeme;
        using qi::char_;
        using qi::lit;
        using qi::int_;
        using qi::raw;
        using ascii::alpha;
        using ascii::alnum;

        keywords.add("return")
                    ("if")
                    ("else");

        quotedString %= lexeme['"' >> *(char_ - '"') >> '"'];

        name %= !lexeme[keywords >> !(alnum | '_')] // necessary to not accidentally exclude if1 or returnfoo
                >> raw[lexeme[('_' | alpha) >> *('_' | alnum)]];
        identifier = name;
        type = name;


        qi::real_parser<float, qi::strict_real_policies<float>> strict_float;
        literal %= (strict_float | int_ | quotedString);
        lvalue %= identifier;
        expression %= identifier | literal;
        rvalue %= literal | identifier;
        assignExpression %= lvalue >> '=' >> rvalue;

        varDecl %= type >> identifier;
        varDeclStatement %= varDecl >> ';';
        paramList %= -(varDecl % ',');

        statement = (varDecl | expression) >> ';';
        statementList %= -(statement % "");
        block %= '{' >> statementList >> '}';

        funcDecl %= varDecl >> '(' >> paramList >> block;
    }
    qi::rule<Iterator, std::string(), SkipperType> quotedString;
    qi::rule<Iterator, std::string(), SkipperType> name;
    qi::rule<Iterator, Identifier(), SkipperType> identifier;
    qi::rule<Iterator, Type(), SkipperType> type;
    qi::rule<Iterator, VarDecl(), SkipperType> varDecl;
    qi::rule<Iterator, VarDecl(), SkipperType> varDeclStatement;
    qi::rule<Iterator, std::vector<VarDecl>(), SkipperType> paramList;
    qi::rule<Iterator, Expression(), SkipperType> literal;
    qi::rule<Iterator, Expression(), SkipperType> expression;
    qi::rule<Iterator, Expression(), SkipperType> lvalue;
    qi::rule<Iterator, Expression(), SkipperType> rvalue;
    qi::rule<Iterator, Expression(), SkipperType> assignExpression;
    qi::rule<Iterator, Statement(), SkipperType> statement;
    qi::rule<Iterator, std::vector<Statement>(), SkipperType> statementList;
    qi::rule<Iterator, Block(), SkipperType> block;
    qi::rule<Iterator, FuncDecl(), SkipperType> funcDecl;
    qi::rule<Iterator, Program(), SkipperType> program;
};

using namespace std;
int main() {
    typedef std::string::const_iterator iterator_type;
    DaedalusProgramParser<iterator_type> parser;
    string line;
    while (getline(cin, line))
    {
        if (line.empty())
            break;
        iterator_type begin = line.begin();
        iterator_type end = line.end();
        Expression attribute;
        bool success = qi::phrase_parse(begin, end, parser, encoding::space, attribute);
        cout << (success ? "success: " : "fail!") << endl;
        if (success)
        {
            boost::apply_visitor(ExpressionVisitor(), attribute);
        }
    }
}
