/*=============================================================================
    Copyright (c) 2001-2014 Joel de Guzman

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
=============================================================================*/
///////////////////////////////////////////////////////////////////////////////
//
//  Now we'll introduce boolean expressions and control structures.
//  Is it obvious now what we are up to? ;-)
//
//  [ JDG April 9, 2007 ]       spirit2
//  [ JDG February 18, 2011 ]   Pure attributes. No semantic actions.
//  [ JDG June 6, 2014 ]        Ported from qi calc8 example.
//
///////////////////////////////////////////////////////////////////////////////
#define BOOST_SPIRIT_X3_DEBUG

#include <boost/program_options.hpp>
#include "ast.hpp"
#include "vm/vm.hpp"
#include "visitors/compiler.hpp"
#include "statement.hpp"
#include "error_handler.hpp"
#include "config.hpp"
#include <iostream>
#include "common.hpp"
#include "visitors/PrettyPrinter.hpp"
#include "visitors/DumpAstVisitor.hpp"
#include "utils.hpp"

///////////////////////////////////////////////////////////////////////////////
//  Main program
///////////////////////////////////////////////////////////////////////////////
int main(int argc, char* argv[]) {
    namespace po = boost::program_options;

    po::options_description desc("Allowed options");
    desc.add_options()
            ("help", "produce help message")
            ("input-file", po::value<std::string>(), "daedalus input file")
            ("o", po::value<std::string>(),
             "output file. usage:\n--o <filename>")
            ;
    po::positional_options_description positional;
    positional.add("input-file", -1);
    po::variables_map var_map;
    po::store(po::command_line_parser(argc, argv).
            options(desc).positional(positional).run(), var_map);
    po::notify(var_map);
    if (var_map.count("help")) {
        std::cout << desc << std::endl;
        return 0;
    }

    std::string daedalus_filename;
    if (var_map.count("input-file"))
        daedalus_filename = var_map["input-file"].as<std::string>();

    std::string ofilename = "opcodes.txt";
    if (var_map.count("o"))
        ofilename = var_map["o"].as<std::string>();

    std::string sourceCode;
    if (!daedalus_filename.empty())
        sourceCode = Utils::readAllText(daedalus_filename);
    else
    {
        std::istreambuf_iterator<char> stdinIter(std::cin), inputEnd;
        sourceCode = std::string(stdinIter, inputEnd);
    }

    using parser::iterator_type;
    iterator_type iter(sourceCode.begin());
    iterator_type end(sourceCode.end());

    vmachine vm;
    code_gen::program program;
    ast::program ast;

    using boost::spirit::x3::with;
    using parser::error_handler_type;
    error_handler_type error_handler(iter, end, std::cerr); // Our error handler

    // Our compiler
    code_gen::compiler compile(program, error_handler);

    // Our parser
    auto const parser =
            // we pass our error handler to the parser so we can access
            // it later on in our on_error and on_success handlers
            with<parser::error_handler_tag>(std::ref(error_handler))
            [
                    getProgramParser()
            ];

    using parser::encoding::space;
    bool success = phrase_parse(iter, end, parser, space, ast);
    if (!success || iter != end)
    {
        std::cerr << "Parsing failed. Compilation aborted" << std::endl;
        return 1;
    }

    // Visitors
    using namespace ASTVisitors;
    std::list<std::unique_ptr<BaseVisitor>> visitors;
    visitors.push_back(std::make_unique<DumpAstVisitor>(error_handler));
    visitors.push_back(std::make_unique<PrettyPrinter>(error_handler, std::cout));

    // Run visitors
    for (auto& visitor : visitors)
    {
        visitor->start(ast);
    }

    std::cout << "-------------------------\n";

    if (compile.compile(ast)) {
        std::cout << "Success\n";
        std::cout << "-------------------------\n";
        vm.execute(program());

        std::cout << "-------------------------\n";
        std::cout << "Assembler----------------\n\n";
        program.print_assembler();

        std::cout << "-------------------------\n";
        std::cout << "Results------------------\n\n";
        program.print_variables(vm.get_stack());
    } else {
        std::cout << "Compile failure\n";
    }


    std::cout << "-------------------------\n\n";
    return 0;
}
