#ifndef EXPR_TYPE_CHECK_HPP
#define EXPR_TYPE_CHECK_HPP

#include "expr.hpp"

class TypeCheckVisitor : public ExprNodeVisitor {
public:
    void visit(Variable &node) override {
        node.setType(node.getType());
    }

    void visit(IntegerConstant &node) override {
        node.setType(ExprType::Integer);
    }

    void visit(FloatConstant &node) override {
        node.setType(ExprType::Float);
    }

    void visit(BooleanConstant &node) override {
        node.setType(ExprType::Boolean);
    }

    void visit(UnaryArithOp &node) override {
        node.getOperand().accept(*this);
        ExprType operandType = node.getOperand().getType();

        if (operandType == ExprType::Integer || operandType == ExprType::Float) {
            node.setType(operandType);
        } else {
            node.setType(ExprType::ErrorType);
        }
    }

    void visit(LogicalNotOp &node) override {
        node.getOperand().accept(*this);
        ExprType operandType = node.getOperand().getType();

        if (operandType == ExprType::Boolean) {
            node.setType(ExprType::Boolean);
        } else {
            node.setType(ExprType::ErrorType);
        }
    }

    void visit(BinaryArithOp &node) override {
        node.getLHS().accept(*this);
        node.getRHS().accept(*this);
        ExprType leftType = node.getLHS().getType();
        ExprType rightType = node.getRHS().getType();

        if ((leftType == ExprType::Integer && rightType == ExprType::Integer) ||
            (leftType == ExprType::Float && rightType == ExprType::Float) ||
            (leftType == ExprType::Integer && rightType == ExprType::Float) ||
            (leftType == ExprType::Float && rightType == ExprType::Integer)) {
            node.setType(leftType == ExprType::Float || rightType == ExprType::Float ? ExprType::Float : ExprType::Integer);
        } else {
            node.setType(ExprType::ErrorType);
        }
    }

    void visit(EqualityOp &node) override {
        node.getLHS().accept(*this);
        node.getRHS().accept(*this);
        ExprType leftType = node.getLHS().getType();
        ExprType rightType = node.getRHS().getType();

        if ((leftType == rightType && leftType != ExprType::ErrorType)|| 
            ((leftType == ExprType::Integer || leftType == ExprType::Float) &&
            (rightType == ExprType::Integer || rightType == ExprType::Float))) {
            node.setType(ExprType::Boolean);
        } else {
            node.setType(ExprType::ErrorType);
        }
    }

    void visit(RelationalOp &node) override {
        node.getLHS().accept(*this);
        node.getRHS().accept(*this);
        ExprType leftType = node.getLHS().getType();
        ExprType rightType = node.getRHS().getType();

        if ((leftType == ExprType::Integer || leftType == ExprType::Float) &&
            (rightType == ExprType::Integer || rightType == ExprType::Float)) {
            node.setType(ExprType::Boolean);
        } else {
            node.setType(ExprType::ErrorType);
        }
    }

    void visit(BinaryLogicalOp &node) override {
        node.getLHS().accept(*this);
        node.getRHS().accept(*this);
        ExprType leftType = node.getLHS().getType();
        ExprType rightType = node.getRHS().getType();

        if (leftType == ExprType::Boolean && rightType == ExprType::Boolean) {
            node.setType(ExprType::Boolean);
        } else {
            node.setType(ExprType::ErrorType);
        }
    }

    void visit(ConditionalOp &node) override {
        node.getCondition().accept(*this);
        node.getExprTrue().accept(*this);
        node.getExprFalse().accept(*this);
        ExprType condType = node.getCondition().getType();
        ExprType trueType = node.getExprTrue().getType();
        ExprType falseType = node.getExprFalse().getType();

        if (condType == ExprType::Boolean && (trueType == falseType || 
                                            ((trueType == ExprType::Integer || trueType == ExprType::Float) &&
                                            (falseType == ExprType::Integer || falseType == ExprType::Float)))) {
            if (trueType == falseType) {
                node.setType(trueType);
            } else {
                node.setType(trueType == ExprType::Float || falseType == ExprType::Float ? ExprType::Float : ExprType::Integer);
            }
        } 
        else {
            node.setType(ExprType::ErrorType);
        }
    }


};

/// @brief Performs type checking on the expression @c expr, including all of
/// its subexpressions.
/// @param expr The expression, which is the root of the AST.
void typeCheck(Expr &expr) {
    TypeCheckVisitor visitor;
    expr.accept(visitor);
}

#endif // EXPR_TYPE_CHECK_HPP
