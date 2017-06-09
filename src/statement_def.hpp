/*=============================================================================
    Copyright (c) 2001-2014 Joel de Guzman

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
=============================================================================*/
#if !defined(BOOST_SPIRIT_X3_CALC9_STATEMENT_DEF_HPP)
#define BOOST_SPIRIT_X3_CALC9_STATEMENT_DEF_HPP

#include <boost/spirit/home/x3.hpp>
#include <boost/spirit/home/x3/support/utility/annotate_on_success.hpp>
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
    struct variable_declaration_class;
    struct if_statement_class;
    struct while_statement_class;
    struct assignment_class;
    struct variable_class;

    typedef x3::rule <program_class, ast::program> program_type;
    typedef x3::rule <statement_class, ast::statement> statement_type;
    typedef x3::rule <block_class, ast::block> block_type;
    typedef x3::rule <variable_declaration_class, ast::variable_declaration> variable_declaration_type;
    typedef x3::rule <if_statement_class, ast::if_statement> if_statement_type;
    typedef x3::rule <while_statement_class, ast::while_statement> while_statement_type;
    typedef x3::rule <assignment_class, ast::assignment> assignment_type;
    typedef x3::rule <variable_class, ast::variable> variable_type;

    program_type const program("program");
    statement_type const statement("statement");
    block_type const block("block");
    variable_declaration_type const variable_declaration("variable_declaration");
    if_statement_type const if_statement("if_statement");
    while_statement_type const while_statement("while_statement");
    assignment_type const assignment("assignment");
    variable_type const variable("variable");

    // Import the expression rule
    namespace { auto const &expression = getExpressionParser(); }

    auto const statement_def =
            variable_declaration
            | if_statement
            | while_statement
            | assignment
    ;

    auto const block_def =
            '{'
            > *statement
//            > *lit(';') > statement % *lit(';') > *lit(';') // skip ; variant
            > '}'
    ;

    auto const variable_declaration_def =
            nocase_wholeword("var")
            > assignment
    ;

    auto const if_statement_def =
            nocase_wholeword("if") > expression
            > block
            > -(nocase_wholeword("else") > block)
            > -lit(';')
    ;

    auto const while_statement_def =
            nocase_wholeword("while") > expression
            > block
            > -lit(';')
    ;

    auto const assignment_def =
            variable
            > '='
            > expression
            > ';'
    ;

    auto const variable_def = identifier;
    auto const program_def = *statement;

    BOOST_SPIRIT_DEFINE(
            statement, block, variable_declaration, if_statement, while_statement, assignment, variable, program
    );

    struct program_class : error_handler_base, x3::annotate_on_success {
    };
    struct assignment_class : x3::annotate_on_success {
    };
    struct variable_class : x3::annotate_on_success {
    };
}

parser::program_type const &getProgramParser() {
    return parser::program;
}

#endif
