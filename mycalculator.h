#ifndef MYCALCULATOR_H_
#define MYCALCULATOR_H_
#include <stack>
#include <vector>
#include <string>
#include <cstdlib>
#include <map>
#include <stdexcept>
#include <QObject>
class Calculator;

/*
   Represents a 'token' in an RPN expression.
   An RPN expression looks something like:
     2 3 4 + *
   This class provides a common interface for handling both
   operators and operands.
 */
struct TokenBase {
    virtual void evaluate(Calculator *) = 0;
    virtual ~TokenBase() {}
};

/*
   Concrete 'token' of an RPN expression.
   Operators are of type Token< char >
   Operands are of type Token< double >
 */
template<class T>
class Token : public TokenBase {

    T token_;

public:

    /* Allow a calculator to consume this token */
    void evaluate(Calculator  *c);

    Token(T t) : token_(t) {}
};

/*
   Represents an expression in Reverse Polish Notation.
   This object basically acts as a FIFO queue of tokens
 */
class RPNExpression {

    std::vector<TokenBase*> stack_;

public:

    /* Add a token to the end of the expression */
    void push(TokenBase *t);

    /* Grab the next token from the front of the expression */
    TokenBase* pop();

    bool empty() const;
};

/*
   Convert an expression in infix format to RPN format
 */
class ShuntingYard {

    const std::string expr_;
    RPNExpression rpn_;
    std::stack<char> op_stack_;
    mutable std::map<char, int> op_precedence_;

    /* Returns a precedence value for the given operator */
    int precedence(char op) const;
    /* Returns the precedence of the top item in the stack */
    int stack_precedence() const;
    /* Reset precedence to allow for new scope */
    void handle_left_paren();
    /* Consume all operators in current scope and restore previous scope */
    void handle_right_paren();
    /* Consume operators with precedence >= than op then add op */
    void handle_op(char op);
    /* Convert infix to RPN via shunting-yard algorithm */
    RPNExpression convert(const std::string &infix);

public:
    ShuntingYard(const std::string& infix);
    RPNExpression to_rpn();

};

/*
   A calculator that evaluates expressions by first converting them to
   reverse polish notation then processing the result.
 */

class Calculator : public QObject {

    Q_OBJECT

    std::stack<double> operands_;

    double pop();

    void push(double d);

    /* Returns the most recent operation result(top of the operand stack) */
    double result() const;

    /* Empty the operand stack */
    void flush();

protected:

    /* Process an operand token from the input stream */
    void consume(double value);

    /* Process an operator token from the input stream */
    void consume(char op);
public:

    /*
        Evaluate expression
        Note: Expression is expected to be in infix form.
     */
    explicit Calculator(QObject *parent = nullptr);

    Q_INVOKABLE double calculate(const std::string& expr);
    Q_INVOKABLE double calculate(const QString& expr);
    /* Expose the consume() methods to the Tokens */
    template<class T> friend class Token;
};

#endif
