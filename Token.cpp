#include <iostream>
#include <stack>
#include <vector>
#include <string>
using namespace std;
bool isoperator(string s)
{
    return s == "+" || s == "-" || s == "*" || s == "/";
}
int precedence(string op)
{
    if (op == "+" || op == "-")
    {
        return 1;
    }
    if (op == "*" || op == "/")
    {
        return 2;
    }
    return 0;
}
bool isdigit(char c)
{
    return c >= '0' && c <= '9';
}
bool isletter(char c)
{
    return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z');
}
bool isnumber(string s)
{
    if (s.length() == 0)
    {
        return false;
    }
    for (int i = 0; i < s.length(); i++)
    {
        if (!isdigit(s[i]))
        {
            return false;
        }
    }

    return true;
}
bool isvariable(string s)
{
    if (!(isletter(s[0]) || s[0] == '_'))
    {
        return false;
    }
    for (int i = 0; i < s.length(); i++)
    {
        if (!(isletter(s[i]) ||  isdigit(s[i]) ||  s[i] == '_'))
        {
            return false;
        }
    }
    return true;
}
int stringtoint(string s)
{
    int num = 0;
    for (int i = 0; i < s.length(); i++)
    {
        num = num * 10 + (s[i] - '0');
    }
    return num;
}
bool alreadyexists(vector<string> vars, string var)
{
    for (int i = 0; i < vars.size(); i++)
    {
        if (vars[i] == var)
        {
            return true;
        }
    }

    return false;
}
vector<string> tokenize(string expr)
{
    vector<string> tokens;
    int i = 0;
    while (i < expr.length())
    {
        if (expr[i] == ' ')
        {
            i++;
        }
        else if (isdigit(expr[i]))
        {
            string num = "";
            while (i < expr.length() && isdigit(expr[i]))
            {
                num += expr[i];
                i++;
            }
            tokens.push_back(num);
        }
        else if (isletter(expr[i]) || expr[i] == '_')
        {
            string var = "";
            while (i < expr.length() && (isletter(expr[i]) || isdigit(expr[i]) ||  expr[i] == '_'))
            {
                var += expr[i];
                i++;
            }
            tokens.push_back(var);
        }
        else
        {
            string op = "";
            op += expr[i];
            if (op == "+" || op == "-" || op == "*" || op == "/" || op == "(" || op == ")" || op == "[" || op == "]" || op == "{" || op == "}")
            {
                tokens.push_back(op);
                i++;
            }
            else
            {
                cout << "Syntax Error\n";
                return {};
            }
        }
    }
    return tokens;
}
vector<string> infixtopostfix(vector<string> tokens)
{
    vector<string> postfix;
    stack<string> st;
    for (int i = 0; i < tokens.size(); i++)
    {
        string token = tokens[i];
        if (isnumber(token) ||isvariable(token))
        {
            postfix.push_back(token);
        }
        else if (isoperator(token))
        {
            while (!st.empty() && isoperator(st.top()) &&precedence(st.top()) >= precedence(token))
            {
                postfix.push_back(st.top());
                st.pop();
            }
            st.push(token);
        }
        else if (token == "(" || token == "[" || token == "{")
        {
            st.push(token);
        }
        else if (token == ")" || token == "]" || token == "}")
        {
            bool found = false;
            while (!st.empty())
            {
                if ((token == ")" && st.top() == "(") || (token == "]" && st.top() == "[") || (token == "}" && st.top() == "{"))
                {
                    found = true;
                    st.pop();
                    break;
                }
                postfix.push_back(st.top());
                st.pop();
            }
            if (!found)
            {
                cout << "Syntax Error\n";
                return {};
            }
        }
    }
    while (!st.empty())
    {
        if (st.top() == "(" || st.top() == "[" || st.top() == "{")
        {
            cout << "Syntax Error\n";
            return {};
        }
        postfix.push_back(st.top());
        st.pop();
    }
    return postfix;
}
int getValue(vector<string> names,vector<int> values,string var)
{
    for (int i = 0; i < names.size(); i++)
    {
        if (names[i] == var)
        {
            return values[i];
        }
    }
    return 0;
}
int evaluate(vector<string> postfix,vector<string> names,vector<int> values)
{
    stack<int> st;
    for (int i = 0; i < postfix.size(); i++)
    {
        string token = postfix[i];
        if (isnumber(token))
        {
            st.push(stringtoint(token));
        }
        else if (isvariable(token))
        {
            st.push(getValue(names, values, token));
        }
        else if (isoperator(token))
        {
            if (st.size() < 2)
            {
                cout << "Logical Error\n";
                return 0;
            }
            int b = st.top();
            st.pop();
            int a = st.top();
            st.pop();
            int result = 0;
            if (token == "+")
            {
                result = a + b;
            }
            else if (token == "-")
            {
                result = a - b;
            }
            else if (token == "*")
            {
                result = a * b;
            }
            else if (token == "/")
            {
                if (b == 0)
                {
                    cout << "Runtime Error\n";
                    return 0;
                }
                result = a / b;
            }
            st.push(result);
        }
    }
    if (st.size() != 1)
    {
        cout << "Logical Error\n";
        return 0;
    }
    return st.top();
}
int main()
{
    string expr;
    cout<<"Enter the string :";
    getline(cin, expr);
    vector<string> tokens = tokenize(expr);
    vector<string> postfix = infixtopostfix(tokens);
    cout << "Postfix Expression: ";
    for (int i = 0; i < postfix.size(); i++)
    {
        cout << postfix[i] << " ";
    }
    cout << endl;
    vector<string> names;
    vector<int> values;
    for (int i = 0; i < postfix.size(); i++)
    {
        if (isvariable(postfix[i]))
        {
            if (!alreadyexists(names, postfix[i]))
            {
                names.push_back(postfix[i]);
                int val;
                cout << "Enter value for " << postfix[i] << ": ";
                cin >> val;
                values.push_back(val);
            }
        }
    }
    int answer = evaluate(postfix, names, values);
    cout << answer << endl;
    return 0;
}