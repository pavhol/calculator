#include "mycalculator.h"

/*
   Represents an expression in Reverse Polish Notation.
   This object basically acts as a FIFO queue of tokens
 */



    void RPNExpression::push(TokenBase *t) { stack_.push_back(t); }

    /* Grab the next token from the front of the expression */
    TokenBase* RPNExpression::pop() {
        TokenBase *t = stack_.front();
        stack_.erase(stack_.begin());
        return t;
    }

    bool RPNExpression::empty() const { return stack_.empty(); }

/*
   Convert an expression in infix format to RPN format
 */


    /* Returns a precedence value for the given operator */
    int ShuntingYard::precedence(char op) const { return op_precedence_[op]; }

    /* Returns the precedence of the top item in the stack */
    int ShuntingYard::stack_precedence() const {
        if (op_stack_.empty()) { return -1; }
        return precedence(op_stack_.top());
    }

    /* Reset precedence to allow for new scope */
    void ShuntingYard::handle_left_paren() { op_stack_.push('('); }

    /* Consume all operators in current scope and restore previous scope */
    void ShuntingYard::handle_right_paren() {
        while ('(' != op_stack_.top()) {
            rpn_.push(new Token<char>(op_stack_.top()));
            op_stack_.pop();
        }
        op_stack_.pop();
    }

    /* Consume operators with precedence >= than op then add op */
    void ShuntingYard::handle_op(char op) {
        while (!op_stack_.empty() &&
               precedence(op) <= stack_precedence()) {
            rpn_.push(new Token<char>(op_stack_.top()));
            op_stack_.pop();
        }
        op_stack_.push(op);
    }

    /* Convert infix to RPN via shunting-yard algorithm */
    RPNExpression ShuntingYard::convert(const std::string &infix) {

        const char * token = infix.c_str();

        while (token && *token) {
            while (*token && isspace(*token)) { ++token; }
            if (!*token) { break; }
            if (isdigit(*token)) {
                char * next_token = 0;
                rpn_.push(new Token<double>(strtod(token, &next_token)));
                token = next_token;
            } else {
                char op = *token;
                switch (op) {
                case '(':
                    handle_left_paren();
                    break;
                case ')':
                    handle_right_paren();
                    break;
                default:
                    handle_op(op);
                }
                ++token;
            }
        }
        while (!op_stack_.empty()) {
            rpn_.push(new Token<char>(op_stack_.top()));
            op_stack_.pop();
        }
        return rpn_;
    }

    ShuntingYard::ShuntingYard(const std::string& infix) : expr_(infix) {
        op_precedence_['('] = -1;
        op_precedence_['%'] = 3;
        op_precedence_['+'] = 2; op_precedence_['-'] = 2;
        op_precedence_['*'] = 3; op_precedence_['/'] = 3;
    }

    RPNExpression ShuntingYard::to_rpn() { return convert(expr_); }


    double Calculator::pop() {
        if (operands_.empty()) {
            return 0;
        }
        double d = operands_.top();
        operands_.pop();
        return d;
    }

    void Calculator::push(double d) { operands_.push(d); }

    /* Returns the most recent operation result (top of the operand stack) */
    double Calculator::result() const { return operands_.top(); }

    /* Empty the operand stack */
    void Calculator::flush() {
        while (!operands_.empty()) { operands_.pop(); }
    }

    /* Process an operand token from the input stream */
    void Calculator::consume(double value) { push(value); }

    /* Process an operator token from the input stream */
    void Calculator::consume(char op) {
        switch (op) {
        case '+':
            push(pop() + pop());
            break;
        case '*':
        {
            double left = pop();
            if (operands_.empty())
                push(left);
            else
            push(left * pop());
        }
        break;
        case '-':
        {
            double right = pop();
            if (operands_.empty())
                push(right);
            else
            push(pop() - right);
        }
        break;
        case '/':
        {
            double right = pop();
            if (operands_.empty())
                push(right);
            else
            push(pop() / right);
        }
        break;
        case '%':
        {
            int right = pop();
            push((int)pop() % right);
        }
        break;
        case '(':
        case '.':
            break;
        default:
            throw std::domain_error("Unknown Operator");
        }
    }

    Calculator::Calculator(QObject *parent) : QObject{parent} {}
    /*
        Evaluate expression
        Note: Expression is expected to be in infix form.
     */
    double Calculator::calculate (const std::string& expr) {
        ShuntingYard shunting(expr);
        RPNExpression rpn = shunting.to_rpn();
        flush();
        while (!rpn.empty()) {
            TokenBase * token = rpn.pop();
            token->evaluate(this);
            delete token;
        }
        return result();
    }
    double Calculator::calculate (const QString& expr) {
        ShuntingYard shunting(expr.toStdString());
        RPNExpression rpn = shunting.to_rpn();
        flush();
        while (!rpn.empty()) {
            TokenBase * token = rpn.pop();
            token->evaluate(this);
            delete token;
        }
        return result();
    }



template<class T>
void Token<T>::evaluate (Calculator * c) { c->consume(token_); }

