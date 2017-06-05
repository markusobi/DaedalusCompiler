#include "BaseVisitor.hpp"
/*
namespace Visitors {

    using result_type = BaseVisitor::result_type;

    result_type BaseVisitor::operator()(unsigned int x) const {
        program.op(op_int, x);
        return true;
    }

    result_type BaseVisitor::operator()(ast::variable const &x) const {
        int const *p = program.find_var(x.name);
        if (p == 0) {
            error_handler(x, "Undeclared variable: " + x.name);
            return false;
        }
        program.op(op_load, *p);
        return true;
    }

    result_type BaseVisitor::operator()(ast::operation const &x) const {
        if (!boost::apply_visitor(*this, x.operand_))
            return false;
        switch (x.operator_) {
            case ast::op_plus:
                program.op(op_add);
                break;
            case ast::op_minus:
                program.op(op_sub);
                break;
            case ast::op_times:
                program.op(op_mul);
                break;
            case ast::op_divide:
                program.op(op_div);
                break;
            case ast::op_modulo:
                program.op(op_mod);
                break;

            case ast::op_shift_left:
                program.op(op_shift_left);
                break;
            case ast::op_shift_right:
                program.op(op_shift_right);
                break;

            case ast::op_equal:
                program.op(op_eq);
                break;
            case ast::op_not_equal:
                program.op(op_neq);
                break;
            case ast::op_less:
                program.op(op_lt);
                break;
            case ast::op_less_equal:
                program.op(op_lte);
                break;
            case ast::op_greater:
                program.op(op_gt);
                break;
            case ast::op_greater_equal:
                program.op(op_gte);
                break;

            case ast::op_bitwise_or:
                program.op(op_bitwise_or);
                break;
            case ast::op_bitwise_xor:
                program.op(op_bitwise_xor);
                break;
            case ast::op_bitwise_and:
                program.op(op_bitwise_and);
                break;

            case ast::op_logical_and:
                program.op(op_logical_and);
                break;
            case ast::op_logical_or:
                program.op(op_logical_or);
                break;
            default:
                BOOST_ASSERT(0);
                return false;
        }
        return true;
    }

    result_type BaseVisitor::operator()(ast::unary const &x) const {
        if (!boost::apply_visitor(*this, x.operand_))
            return false;
        switch (x.operator_) {
            case ast::op_positive:
                program.op(op_pos);
                break;
            case ast::op_negative:
                program.op(op_neg);
                break;
            case ast::op_logical_not:
                program.op(op_lognot);
                break;
            case ast::op_bitwise_not:
                program.op(op_bitnot);
                break;
            default:
                BOOST_ASSERT(0);
                return false;
        }
        return true;
    }

    result_type BaseVisitor::expression_visit_left_to_right(ast::expression const &x) const {
        if (!boost::apply_visitor(*this, x.first))
            return false;
        for (ast::operation const &oper : x.rest) {
            if (!(*this)(oper))
                return false;
        }
        return true;
    }

    result_type BaseVisitor::expression_visit_right_to_left(ast::expression const &x) const {
        if (!boost::apply_visitor(*this, x.first))
            return false;
        for (ast::operation const &oper : x.rest) {
            if (!(*this)(oper))
                return false;
        }
        return true;
    }

    result_type BaseVisitor::operator()(ast::expression const &x) const {
        return expression_visit_left_to_right(x);
    }

    result_type BaseVisitor::operator()(ast::assignment const &x) const {
        if (!(*this)(x.rhs))
            return false;
        int const *p = program.find_var(x.lhs.name);
        if (p == 0) {
            error_handler(x.lhs, "Undeclared variable: " + x.lhs.name);
            return false;
        }
        program.op(op_store, *p);
        return true;
    }

    result_type BaseVisitor::operator()(ast::variable_declaration const &x) const {
        int const *p = program.find_var(x.assign.lhs.name);
        if (p != 0) {
            error_handler(x.assign.lhs, "Duplicate variable: " + x.assign.lhs.name);
            return false;
        }
        bool r = (*this)(x.assign.rhs);
        if (r) // don't add the variable if the RHS fails
        {
            program.add_var(x.assign.lhs.name);
            program.op(op_store, *program.find_var(x.assign.lhs.name));
        }
        return r;
    }

    result_type BaseVisitor::operator()(ast::statement const &x) const {
        return boost::apply_visitor(*this, x);
    }

    result_type BaseVisitor::operator()(ast::statement_list const &x) const {
        for (auto const &s : x) {
            if (!(*this)(s))
                return false;
        }
        return true;
    }

    result_type BaseVisitor::operator()(ast::if_statement const &x) const {
        if (!(*this)(x.condition))
            return false;
        program.op(op_jump_if, 0);                      // we shall fill this (0) in later
        std::size_t skip = program.size() - 1;            // mark its position
        if (!(*this)(x.then))
            return false;
        program[skip] = int(program.size() - skip);       // now we know where to jump to (after the if branch)

        if (x.else_)                                    // We got an alse
        {
            program[skip] += 2;                         // adjust for the "else" jump
            program.op(op_jump, 0);                     // we shall fill this (0) in later
            std::size_t exit = program.size() - 1;        // mark its position
            if (!(*this)(*x.else_))
                return false;
            program[exit] = int(program.size() - exit);   // now we know where to jump to (after the else branch)
        }

        return true;
    }

    result_type BaseVisitor::operator()(ast::while_statement const &x) const {
        std::size_t loop = program.size();              // mark our position
        if (!(*this)(x.condition))
            return false;
        program.op(op_jump_if, 0);                      // we shall fill this (0) in later
        std::size_t exit = program.size() - 1;            // mark its position
        if (!(*this)(x.body))
            return false;
        program.op(op_jump,
                   int(loop - 1) - int(program.size()));         // loop back
        program[exit] = int(program.size() - exit);       // now we know where to jump to (to exit the loop)
        return true;
    }

    result_type BaseVisitor::start(ast::statement_list const &x) const {
        program.clear();
        // op_stk_adj 0 for now. we'll know how many variables we'll have later
        program.op(op_stk_adj, 0);

        if (!(*this)(x)) {
            program.clear();
            return false;
        }
        program[1] = int(program.nvars());              // now store the actual number of variables
        return true;
    }
}
}
*/