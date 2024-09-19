#include <iostream>
#include <string>
#include <functional>
#include <unordered_map>
#include <cmath>
#include <cctype>
#include <cassert>
#include <algorithm>
#include <stack>
#include <stdexcept>
#include <memory>
 
// operations
// abstract class operation
 
class operation
{
public:
    virtual void exec(std::stack<double> &) const = 0;
    virtual ~operation() = default;
};
 
class op_greater : public operation
{
public:
    void exec(std::stack<double> & stack) const override
    {
        const auto arg1 = stack.top();
        stack.pop();
        const auto arg2 = stack.top();
        stack.pop();
 
        stack.push((arg1 > arg2) ? true : false);
    }
};
 
class op_less: public operation
{
public:
    void exec(std::stack<double> & stack) const override
    {
        const auto arg1 = stack.top();
        stack.pop();
        const auto arg2 = stack.top();
        stack.pop();
 
        stack.push((arg1 < arg2) ? true : false);
    }
};
 
class op_assign : public operation
{
public:
    void exec(std::stack<double> & stack) const override
    {
        const auto arg1 = stack.top();
        stack.pop();
        const auto arg2 = stack.top();
        stack.pop();
 
        stack.push((arg1 == arg2) ? true : false);
    }
};
 
class op_add : public operation
{
public:
    void exec(std::stack<double> & stack) const override
    {
        const auto arg1 = stack.top();
        stack.pop();
        const auto arg2 = stack.top();
        stack.pop();
 
        stack.push(arg2 + arg1);
    }
};
 
class op_subtract : public operation
{
public:
    void exec(std::stack<double> & stack) const override
    {
        const auto arg1 = stack.top();
        stack.pop();
        const auto arg2 = stack.top();
        stack.pop();
 
        stack.push(arg2 - arg1);
    }
};
 
class op_multiply : public operation
{
public:
    void exec(std::stack<double> & stack) const override
    {
        const auto arg1 = stack.top();
        stack.pop();
        const auto arg2 = stack.top();
        stack.pop();
 
        stack.push(arg2 * arg1);
    }
};
 
class op_divide : public operation
{
public:
    void exec(std::stack<double> & stack) const override
    {
        const auto arg1 = stack.top();
        stack.pop();
        const auto arg2 = stack.top();
        stack.pop();
        assert(arg1 != 0);
 
        stack.push(arg2 / arg1);
    }
};
 
class op_mod : public operation
{
public:
    void exec(std::stack<double> & stack) const override
    {
        const auto arg1 = stack.top();
        stack.pop();
        const auto arg2 = stack.top();
        stack.pop();
 
        stack.push(std::fmod(arg2, arg1));
    }
};
 
class op_pow : public operation
{
public:
    void exec(std::stack<double> & stack) const override
    {
        const auto arg1 = stack.top();
        stack.pop();
        const auto arg2 = stack.top();
        stack.pop();
 
        stack.push(std::pow(arg2, arg1));
    }
};
 
class op_fact : public operation
{
    double fact(double val) const
    {
        if (val == 0)
            return 1;
        return val * fact(val - 1);
    }
public:
    void exec(std::stack<double> & stack) const override
    {
        const auto arg = stack.top();
        stack.pop();
        assert(arg > 0);
 
        stack.push(fact(arg));
    }
};
 
class op_unary_plus : public operation
{
public:
    void exec(std::stack<double> & stack) const override
    {
        const auto arg = stack.top();
        stack.pop();
 
        stack.push(arg);
    }
};
 
class op_unary_minus : public operation
{
public:
    void exec(std::stack<double> & stack) const final override
    {
        const auto arg = stack.top();
        stack.pop();
 
        stack.push(-arg);
    }
};
 
// factory
 
class ops_factory
{
public:
    using object_uptr = std::unique_ptr<operation>;
    using creator_function = std::function<object_uptr(void)>;
 
private:
    std::unordered_map<char, creator_function> _creation_data;
 
private:
    ops_factory() = default;
    ops_factory(const ops_factory &) = delete;
 
public:
    static ops_factory & instance()
    {
        static ops_factory factory;
        return factory;
    }
 
    static void register_creation(const char key, creator_function creator)
    {
        ops_factory & factory = instance();
 
        factory._creation_data[key] = creator;
    }
 
    static object_uptr create_object(const char key)
    {
        const ops_factory & factory = instance();
 
        const auto iter = factory._creation_data.find(key);
        if (iter == factory._creation_data.end())
            throw std::invalid_argument("ops_factory: bad key");
 
        creator_function creator = iter->second;
        return creator();
    }
};
 
// registration
 
namespace creator_functions
{
    std::unique_ptr<operation> create_op_assign()
    {
        return std::make_unique<op_assign>();
    }
 
    std::unique_ptr<operation> create_op_add()
    {
        return std::make_unique<op_add>();
    }
 
    std::unique_ptr<operation> create_op_subtract()
    {
        return std::make_unique<op_subtract>();
    }
 
    std::unique_ptr<operation> create_op_multiply()
    {
        return std::make_unique<op_multiply>();
    }
 
    std::unique_ptr<operation> create_op_divide()
    {
        return std::make_unique<op_divide>();
    }
 
    std::unique_ptr<operation> create_op_mod()
    {
        return std::make_unique<op_mod>();
    }
 
    std::unique_ptr<operation> create_op_pow()
    {
        return std::make_unique<op_pow>();
    }
 
    std::unique_ptr<operation> create_op_fact()
    {
        return std::make_unique<op_fact>();
    }
 
    std::unique_ptr<operation> create_op_unary_plus()
    {
        return std::make_unique<op_unary_plus>();
    }
 
    std::unique_ptr<operation> create_op_unary_minus()
    {
        return std::make_unique<op_unary_minus>();
    }
}
 
class static_registrator
{
private:
    static_registrator()
    {
        ops_factory & factory = ops_factory::instance();
 
        factory.register_creation('=', &creator_functions::create_op_assign);
        factory.register_creation('+', &creator_functions::create_op_add);
        factory.register_creation('-', &creator_functions::create_op_subtract);
        factory.register_creation('*', &creator_functions::create_op_multiply);
        factory.register_creation('/', &creator_functions::create_op_divide);
        factory.register_creation('%', &creator_functions::create_op_mod);
        factory.register_creation('^', &creator_functions::create_op_pow);
        factory.register_creation('!', &creator_functions::create_op_fact);
        factory.register_creation(-'+', &creator_functions::create_op_unary_plus);
        factory.register_creation(-'-', &creator_functions::create_op_unary_minus);
    }
 
    static static_registrator _instance;
};
 
static_registrator static_registrator::_instance;
 
// parse expression
 
class expression_parser
{
private:
    std::unordered_map<char, std::size_t> prior;
    std::unordered_map<char, char> parentheses;
 
public:
    expression_parser()
    {
        prior.insert(std::make_pair('=', 0));
        prior.insert(std::make_pair('+', 1));
        prior.insert(std::make_pair('-', 1));
        prior.insert(std::make_pair('*', 2));
        prior.insert(std::make_pair('/', 2));
        prior.insert(std::make_pair('%', 2));
        prior.insert(std::make_pair('^', 3));
        prior.insert(std::make_pair('!', 3));
        prior.insert(std::make_pair(-'+', 4));
        prior.insert(std::make_pair(-'-', 4));
 
        parentheses.insert(std::make_pair(')', '('));
        parentheses.insert(std::make_pair(']', '['));
        parentheses.insert(std::make_pair('}', '{'));
    }
     
    ~expression_parser() = default;
 
    double calc(std::string & expr)
    {
        std::stack<char> oper;
        std::stack<double> stack;
 
        if (!lays_check(expr))
            throw std::invalid_argument("error bracket");
        else if (!incorrect_symbol(expr))
            throw std::invalid_argument("incorrect symbol");
 
        expr.erase(std::remove(std::begin(expr),
                  std::end(expr), ' '),
                  std::end(expr));
        expr.insert(0, "0");
        for (
            auto iter = std::begin(expr);
            iter != std::end(expr);
            ++iter
            )
        {
            if (open_brackets(*iter))
            {
                oper.push(*iter);
                if ((*std::next(iter) == '+') || (*std::next(iter) == '-'))
                    expr.insert(std::next(iter), '0');
            }
             
            else if ((close_brackets(*iter)) || (*iter == ','))
            {
                while (!open_brackets(oper.top()))
                    process_operation(stack, oper.top()),
                    oper.pop();
                if (close_brackets(*iter)) oper.pop();
            }
 
            else if (is_operation(*iter))
            {
                char op_sign = *iter;
                if ((*iter == '+' || *iter == '-') &&
                    (std::begin(expr) == iter || (!std::isdigit(*std::prev(iter)) &&
                     *std::prev(iter) != '.' &&
                     !close_brackets(*std::prev(iter)) &&
                     *std::prev(iter) != '!')))
                         op_sign = -op_sign;
                while (!oper.empty() && prior[oper.top()] >= prior[op_sign])
                    process_operation(stack, oper.top()),
                    oper.pop();
                oper.push(op_sign);
            }
 
            else if (std::isalpha(*iter))
            {
                auto delim = std::find_if_not(iter, std::end(expr), std::isalpha);
                std::string op_fn(iter, delim);
                str_tolower(op_fn);
                if ((std::tolower(*std::prev(delim)) == 'i') ||
                    (std::tolower(*std::prev(delim)) == 'e')) {
                    if (op_fn == "pi")
                        stack.push(std::acos(-1));
                    else if (op_fn == "e")
                        stack.push(std::exp(1));
                    else
                        throw std::invalid_argument("incorrect symbol");
                    iter = std::prev(delim);
                    continue;
                }
                iter = find_bracket(delim, std::end(expr));
                double op_res = calc(std::string(delim, std::next(iter)));
                if (iter == std::find(std::next(delim), iter, ','))
                    process_function(stack, op_fn, op_res);
                else {
                    auto comma = find_delim(std::next(delim), iter, ',');
                    double res_rhs = calc(std::string(std::next(delim), comma));
                    process_function(stack, op_fn, op_res, res_rhs);
                }
            }
 
            else
            {
                auto not_digit = std::find_if_not(iter,
                                                  std::end(expr),
                                                  [this](auto ch) {
                                                      return std::isdigit(ch) || ch == '.';
                                                  });
                stack.push(std::stod(std::string(iter, not_digit)));
                iter = std::prev(not_digit);
            }
        }
 
        while (!oper.empty())
            process_operation(stack, oper.top()),
            oper.pop();
 
        return stack.top();
    }
 
private:
    bool is_operation(const char op_sign)
    {
        return (op_sign == '=' ||
                op_sign == '+' ||
                op_sign == '-' ||
                op_sign == '*' ||
                op_sign == '/' ||
                op_sign == '%' ||
                op_sign == '^' ||
                op_sign == '!');
    }
 
    bool open_brackets(const char symb)
    {
        return (symb == '(' ||
                symb == '[' ||
                symb == '{');
    }
 
    bool close_brackets(const char symb)
    {
        return (symb == ')' ||
                symb == ']' ||
                symb == '}');
    }
 
    bool incorrect_symbol(std::string const & expr)
    {
        return std::all_of(std::begin(expr),
                           std::end(expr),
                           [this](auto ch) {
                               return (std::isspace(ch) ||
                                       std::isdigit(ch) ||
                                       std::isalpha(ch) ||
                                       is_operation(ch) ||
                                       open_brackets(ch) ||
                                       close_brackets(ch) ||
                                       ch == '.' || ch == ',');
                           });
    }
 
    bool lays_check(std::string const & str)
    {
        std::stack<char> stack;
        for (
            const auto ch : str
            )
        {
            if (open_brackets(ch))
                stack.push(ch);
            else if (close_brackets(ch)) {
                if (!stack.empty() && stack.top() == parentheses[ch])
                    stack.pop();
                else
                    return false;
            }
        }
        return stack.empty();
    }
 
    std::string str_tolower(std::string & str)
    {
        std::transform(std::begin(str),
                       std::end(str),
                       std::begin(str),
                       [this](auto ch) {
                           return std::tolower(ch);
                       });
        return str;
    }
 
    template<class Iter>
    Iter find_delim(Iter first, Iter last, const char delim)
    {
        Iter iter = first;
        while (iter != last) {
            if (*iter == delim)
                if (lays_check(std::string(first, iter)))
                    return iter;
            ++iter;
        }
        return last;
    }
 
    template<class Iter>
    Iter find_bracket(Iter first, Iter last)
    {
        std::stack<char> stack;
        Iter iter = first;
        while (iter != last) {
            if (open_brackets(*iter))
                stack.push(*iter);
            else if (close_brackets(*iter)) {
                    stack.pop();
                if (stack.empty())
                    break;
            }
            ++iter;
        }
        return iter;
    }
 
    void process_function(std::stack<double> & stack, std::string & func, double arg)
    {
        if (func == "sin")
            stack.push(std::sin(arg));
        else if (func == "cos")
            stack.push(std::cos(arg));
        else if (func == "tg")
            stack.push(std::tan(arg));
        else if (func == "сtg")
            assert(std::tan(arg) != 0),
            stack.push(1 / std::tan(arg));
        else if (func == "lg")
            assert(arg > 0),
            stack.push(std::log10(arg));
        else if (func == "ln")
            assert(arg > 0),
            stack.push(std::log(arg));
        else if (func == "log")
            assert(arg > 0),
            stack.push(std::logb(arg));
        else if (func == "exp")
            stack.push(std::exp(arg));
        else if (func == "abs")
            stack.push(std::abs(arg));
        else if (func == "sqrt")
            assert(arg > 0),
            stack.push(std::sqrt(arg));
        else
            throw std::invalid_argument("unknow function");
    }
 
    void process_function(std::stack<double> & stack, std::string & func, double arg1, double arg2)
    {
         if (func == "pow")
             stack.push(std::pow(arg2, arg1));
         else if (func == "min")
             stack.push(std::min(arg1, arg2));
         else if (func == "max")
             stack.push(std::max(arg1, arg2));
         else
             throw std::invalid_argument("unknow function");
    }
 
    void process_operation(std::stack<double> & stack, const char op_sign)
    {
        auto op_object = ops_factory::instance().create_object(op_sign); 
        op_object->exec(stack);
    }
};
 
int main(int argc, char* argv[])
{
    std::string expr;
    expression_parser param;
    while (std::cout << ">> " && std::getline(std::cin, expr) && !expr.empty())
        std::cout << "\nresult: " << param.calc(expr) << std::endl;
    return 0;
}
