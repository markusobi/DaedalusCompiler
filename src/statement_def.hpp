/*=============================================================================
    Copyright (c) 2001-2014 Joel de Guzman

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
=============================================================================*/
#if !defined(BOOST_SPIRIT_X3_CALC9_STATEMENT_DEF_HPP)
#define BOOST_SPIRIT_X3_CALC9_STATEMENT_DEF_HPP

#include <boost/spirit/home/x3.hpp>
#include "ast.hpp"
#include "ast_adapted.hpp"
#include "statement.hpp"
#include "expression.hpp"
#include "common.hpp"
#include "error_handler.hpp"

namespace parser {
    using x3::raw;
    using x3::lexeme;
    using encoding::lit;

    struct program_class;
    struct statement_class;
    struct block_class;
    //struct var_decl_class;
    class operand_list_class;
    struct var_decl_statement_class;
    struct array_decl_statement_class;
    struct return_statement_class;
    struct if_statement_class;
    struct while_statement_class;
    struct assignment_class;

    typedef x3::rule <program_class, ast::program> program_type;
    typedef x3::rule <statement_class, ast::statement> statement_type;
    typedef x3::rule <block_class, ast::block> block_type;
    //typedef x3::rule <var_decl_class, ast::variable_declaration> var_decl_type;
    typedef x3::rule <operand_list_class, std::list<ast::operand>> operand_list_type;
    typedef x3::rule <var_decl_statement_class, ast::variable_declaration> var_decl_statement_type;
    typedef x3::rule <array_decl_statement_class, ast::array_declaration> array_decl_statement_type;
    typedef x3::rule <return_statement_class, ast::return_statement> return_statement_type;
    typedef x3::rule <if_statement_class, ast::if_statement> if_statement_type;
    typedef x3::rule <while_statement_class, ast::while_statement> while_statement_type;
    typedef x3::rule <assignment_class, ast::assignment> assignment_type;

    program_type const program("program");
    statement_type const statement("statement");
    block_type const block("block");
    //var_decl_type const var_decl("var_decl");
    operand_list_type const operand_list("operand_list");
    var_decl_statement_type const var_decl_statement("var_decl_statement");
    array_decl_statement_type const array_decl_statement("array_decl_statement");
    return_statement_type const return_statement("return_statement");
    if_statement_type const if_statement("if_statement");
    while_statement_type const while_statement("while_statement");
    assignment_type const assignment("assignment");

    // Import the expression rule
    namespace { auto const &expression2 = getExpressionParser(); }
    namespace { auto const &array_access2 = getArrayAccessParser(); }

    auto const statement_def =
            if_statement
            | while_statement
            | return_statement
            | var_decl_statement
            | array_decl_statement
//            | func_call
            | assignment
    ;

    auto const block_def =
            '{'
            > *statement
//            > *lit(';') > statement % *lit(';') > *lit(';') // skip ; version
            > '}'
    ;

    auto const operand_list_def = expression2 % ',';

    auto var_decl =
            (nocase_wholeword("var") | nocase_wholeword("const"))
            > type
            > variable
    ;

    auto const var_decl_statement_def =
            var_decl
            >> !lit('[')
            > -('=' > expression2)
            > ';'
    ;

    auto const array_decl_statement_def =
            var_decl
            > '[' > expression2 > ']'
            > -(lit('=') > '{' > operand_list > '}')
            > ';'
    ;

    auto const return_statement_def =
            nocase_wholeword("return")
            > expression2;
    ;

    auto const if_statement_def =
            nocase_wholeword("if") > expression2
            > block
            > -(nocase_wholeword("else") > block)
            > -lit(';')
    ;

    auto const while_statement_def =
            nocase_wholeword("while") > expression2
            > block
            > -lit(';')
    ;

    auto const assignment_def =
            (array_access2 | variable)
            >> '=' // FIXME might be operator ==
            > expression2
            > ';'
    ;

    auto const program_def = *statement;

    BOOST_SPIRIT_DEFINE(
            statement,
            block,
            operand_list,
            var_decl_statement,
            array_decl_statement,
            return_statement,
            if_statement,
            while_statement,
            assignment,
            program
    );

    struct program_class : error_handler_base, x3::annotate_on_success {
    };
    struct assignment_class : x3::annotate_on_success {
    };
}

parser::program_type const &getProgramParser() {
    return parser::program;
}

#endif
