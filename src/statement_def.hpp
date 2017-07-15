#pragma once

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

    struct statement_class;
    struct block_class;
    struct var_decl_class;
    class operand_list_class;
    struct var_decl_statement_class;
    struct multi_var_decl_class;
    struct array_decl_statement_class;
    struct return_statement_class;
    struct if_statement_class;
    struct while_statement_class;
    struct assignment_class;
    struct function_class;
    struct prototyp_class;
    struct instance_class;
    struct extern_class_class;
    struct instance_var_decl_class;

    typedef x3::rule <statement_class, ast::statement> statement_type;
    typedef x3::rule <block_class, ast::block> block_type;
    typedef x3::rule <var_decl_class, ast::typed_var> var_decl_type;
    typedef x3::rule <operand_list_class, std::list<ast::operand>> operand_list_type;
    typedef x3::rule <var_decl_statement_class, ast::variable_declaration> var_decl_statement_type;
    typedef x3::rule <multi_var_decl_class, ast::multi_variable_declaration> multi_var_decl_type;
    typedef x3::rule <array_decl_statement_class, ast::array_declaration> array_decl_statement_type;
    typedef x3::rule <return_statement_class, ast::return_statement> return_statement_type;
    typedef x3::rule <if_statement_class, ast::if_statement> if_statement_type;
    typedef x3::rule <while_statement_class, ast::while_statement> while_statement_type;
    typedef x3::rule <assignment_class, ast::assignment> assignment_type;
    typedef x3::rule <function_class, ast::function> function_type;
    typedef x3::rule <prototyp_class, ast::prototype> prototyp_type;
    typedef x3::rule <instance_class, ast::instance> instance_type;
    typedef x3::rule <extern_class_class, ast::extern_class> extern_class_type;
    typedef x3::rule <instance_var_decl_class, ast::instance_var_decl> instance_var_decl_type;

    statement_type const statement("statement");
    block_type const block("block");
    var_decl_type const var_decl("var_decl");
    operand_list_type const operand_list("operand_list");
    var_decl_statement_type const var_decl_statement("var_decl_statement");
    array_decl_statement_type const array_decl_statement("array_decl_statement");
    return_statement_type const return_statement("return_statement");
    if_statement_type const if_statement("if_statement");
    while_statement_type const while_statement("while_statement");
    assignment_type const assignment("assignment");
    function_type const function("function");
    prototyp_type const prototype("prototype");
    instance_type const instance("instance");
    extern_class_type const extern_class("extern_class");
    multi_var_decl_type const multi_var_decl("multi_var_decl");
    instance_var_decl_type const instance_var_decl("instance_var_decl");

    // Import the expression rule
    namespace { auto const &operand2 = getOperandParser(); }
    namespace { auto const &array_access2 = getArrayAccessParser(); }
    namespace { auto const &member_access2 = getMemberAccessParser(); }

    auto const statement_def =
            if_statement
            | while_statement
            | return_statement
            | var_decl_statement
            | array_decl_statement
            | assignment
            | (operand2 > ';')
    ;

    auto const block_def =
            '{'
            > *lit(';') // skip empty ; statements
            > -(statement % *lit(';'))
            > *lit(';') // skip empty ; statements
            > '}'
    ;

    auto const operand_list_def = operand2 % ',';

    auto var_decl_def =
            nocase_wholeword(var_const)
            > type
            > variable
    ;

    auto const var_decl_statement_def =
            var_decl
            >> !lit('[')
            > -('=' > operand2)
            > ';'
    ;

    auto const multi_var_decl_def =
            nocase_wholeword(var_const)
            >> type
            >> ((variable >> !lit('[')) % ',')
            >> ';'
    ;

    auto const array_decl_statement_def =
            var_decl
            > '[' > operand2 > ']'
            > -(lit('=') > '{' > operand_list > '}')
            > ';'
    ;

    auto const return_statement_def =
            nocase_wholeword("return")
            > -operand2
            > ';'
    ;

    typedef x3::rule <struct condition_block_class, ast::condition_block> condition_block_type;
    const auto condition_block2 = nocase_wholeword("if") > operand2 > block;


    auto const if_statement_def =
            (condition_block2 % nocase_wholeword("else"))
            > -(nocase_wholeword("else") > block)
    ;

    auto const while_statement_def =
            nocase_wholeword("while") > operand2
            > block
    ;

    auto const assignment_def =
            (array_access2 | member_access2 | variable)
            >> getCompoundAssignmentOperators() >> !lit('=') // exclude operator ==
            > operand2
            > ';'
    ;

    const auto instance_var_decl_def =
            (
                nocase_wholeword("instance")
                > (variable % ',') > '(' > type > ')'
            )
            >> ';'
    ;

    const auto global_decl =
            function
            | prototype
            | instance_var_decl
            | instance
            | extern_class
            | var_decl_statement
            | array_decl_statement
    ;

    const auto function_def =
            nocase_wholeword("func") > type > variable
            > '(' > -(var_decl % ',') > ')' // TODO forbid const parameters?
            > block
    ;

    const auto prototype_def =
            nocase_wholeword("prototype")
            > variable > '(' > variable > ')'
            > block
    ;

    const auto instance_def =
            (
                nocase_wholeword("instance")
                > variable > '(' > variable > ')'
            )
            > block
    ;

    const auto extern_class_def =
            nocase_wholeword("class") > variable
            > '{'
            > *(multi_var_decl | array_decl_statement)
            > '}'
    ;

    BOOST_SPIRIT_DEFINE(
            statement,
            block,
            operand_list,
            var_decl,
            var_decl_statement,
            array_decl_statement,
            instance_var_decl,
            return_statement,
            if_statement,
            while_statement,
            assignment,
            function,
            prototype,
            instance,
            multi_var_decl,
            extern_class
    )

    struct assignment_class : x3::annotate_on_success {
    };
}
