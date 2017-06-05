#include "BaseVisitor.hpp"

namespace Visitors {

    using result_type = BaseVisitor::result_type;

    result_type BaseVisitor::operator()(unsigned int x) const {
        // int literal
        return result_type();
    }

    result_type BaseVisitor::operator()(ast::variable const &x) const {
        // variable usage or declaration
        return result_type();
    }

    result_type BaseVisitor::operator()(ast::operation const &x) const {
        // operation i.e. * 2
        boost::apply_visitor(*this, x.operand_);
        return result_type();
    }

    result_type BaseVisitor::operator()(ast::unary const &x) const {
        // unary operation
        boost::apply_visitor(*this, x.operand_);
        return result_type();
    }

    result_type BaseVisitor::operator()(ast::expression const &x) const {
        boost::apply_visitor(*this, x.first);
        for (ast::operation const &oper : x.rest) {
            (*this)(oper);
        }
        return result_type();
    }

    result_type BaseVisitor::operator()(ast::assignment const &x) const {
        (*this)(x.rhs);
        (*this)(x.lhs);
        return result_type();
    }

    result_type BaseVisitor::operator()(ast::variable_declaration const &x) const {
        (*this)(x.assign.lhs);
        (*this)(x.assign.rhs);
        return result_type();
    }

    result_type BaseVisitor::operator()(ast::statement const &x) const {
        boost::apply_visitor(*this, x);
        return result_type();
    }

    result_type BaseVisitor::operator()(ast::statement_list const &x) const {
        for (auto const &statement : x) {
            (*this)(statement);
        }
        return result_type();
    }

    result_type BaseVisitor::operator()(ast::if_statement const &x) const {
        (*this)(x.condition);
        (*this)(x.then);
        if (x.else_)
            (*this)(*x.else_);
        return result_type();
    }

    result_type BaseVisitor::operator()(ast::while_statement const &x) const {
        (*this)(x.condition);
        (*this)(x.body);
        return result_type();
    }

    result_type BaseVisitor::start(ast::statement_list const &x) const {
        (*this)(x);
        return result_type();
    }
}
