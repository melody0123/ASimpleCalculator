#ifndef CALCULATOR_H_INCLUDED
#define CALCULATOR_H_INCLUDED

#include <iostream>
#include <cstdio>
#include <cmath>
#include <typeinfo>
#include "Stack.h"
#include "StatusCode.h"

using namespace std;

template <class opnd_type>
class Calculator
{
private:
    Stack<opnd_type> opnd; //操作数栈
    Stack<char> optr; //操作符栈

    int optr_prior(char op); //判断操作符优先级
    StatusCode get_2_opnd(opnd_type &left, opnd_type &right); //获得两个操作数
    StatusCode calc(opnd_type left, char op, opnd_type right, opnd_type &res); //执行运算
    bool is_optr(char ch); //判断该字符是不是操作符
    bool is_opnd(char ch); //判断该字符是不是操作数
public:
    Calculator(){} //构造函数
    virtual ~Calculator(){} //析构函数
    void run(); //计算器入口
};

template <class opnd_type>
int Calculator<opnd_type>::optr_prior(char op)
{
    if(op == '=') return 1;
    else if(op == '(' || op == ')') return 2;
    else if(op == '+' || op == '-') return 3;
    else if(op == '*' || op == '/') return 4;
    else if(op == '^') return 5;
}

template <class opnd_type>
StatusCode Calculator<opnd_type>::get_2_opnd(opnd_type &left, opnd_type &right)
{
    if(opnd.pop(right) == UNDER_FLOW || opnd.pop(left) == UNDER_FLOW)
        return UNDER_FLOW;
    else
        return SUCCESS;

}

template <class opnd_type>
StatusCode Calculator<opnd_type>::calc(opnd_type left, char op, opnd_type right, opnd_type &res)
{
    if(op == '+')
    {
        res = left + right;
        return SUCCESS;
    }
    else if(op == '-')
    {
        res = left - right;
        return SUCCESS;
    }
    else if(op == '*')
    {
        res = left * right;
        return SUCCESS;
    }
    else if(op == '/')
    {
        if(right == 0)
            return FAIL;
        else
        {
            res = left / right;
            return SUCCESS;
        }
    }
    else if(op == '^')
    {
        if(right == 0 && left == 0)
            return FAIL;
        else
        {
            res = pow(left, right);
            return SUCCESS;
        }
    }
//    else if(op == '%')
//    {
//        if(typeid(opnd_type) != typeid(int))
//            return FAIL;
//        else
//        {
//            res = left % right;
//            return SUCCESS;
//        }
//    }
}

template <class opnd_type>
bool Calculator<opnd_type>::is_optr(char ch)
{
    if(ch == '=' || ch == '(' || ch == ')' || ch == '+'
       || ch == '-' || ch == '*' || ch == '/' || ch == '^')
        return true;
    else
        return false;
}

template <class opnd_type>
bool Calculator<opnd_type>::is_opnd(char ch)
{
    if((ch >= '0' && ch <= '9') || ch == '.')
        return true;
    else
        return false;
}

template <class opnd_type>
void Calculator<opnd_type>::run()
{
    while(true)
    {
        fflush(stdin);
        bool except = false; //是否出错
        char cur_ch = '\0'; //当前字符
        char prior_ch = '\0'; //当前字符的前一个字符
        char optr_top = '\0'; //用于接收 optr stack 的栈顶元素

        opnd_type operand = 0; //临时变量，用于存储从opnd stack中读出的栈顶元素
        char op = '\0'; //临时变量，用于送calc()计算结果

        //开始计算前的预处理
        optr.clear(); //清空 optr 栈
        optr.push('='); //在 optr 栈中压入 =
        opnd.clear(); //清空 opnd 栈
        prior_ch = '='; //前一个字符设置成 =
        printf("请输入表达式：");
        cur_ch = cin.get(); //当前字符是输入的表达式的第一个字符

        while(cur_ch != '=' || optr_top != '=') //开始对表达式求值，求值结束的条件见while的条件
        {
            if(is_opnd(cur_ch)) //当前字符是数字
            {
                cin.putback(cur_ch); //放回输入流
                cin >> (operand); //放到中间变量里，实现了 char->opnd_type 的转变
                opnd.push(operand); //操作数入栈
                prior_ch = '0'; //更新前一个字符，由于不是操作符，标记为‘0’
                cur_ch = cin.get(); //再次读入一个字符
            }
            else if(is_optr(cur_ch)) //当前字符是操作符
            {
                if((prior_ch == '=' || prior_ch == '(') && (cur_ch == '+' || cur_ch == '-')) //cur_char 是正负号，转化成双元运算符
                {
                    if(cur_ch == '+') //正号不处理
                    {
                        cur_ch = cin.get(); //读入下一个字符
                        continue;
                    }
                    else //负号
                    {
                        cur_ch = cin.get(); //读入绝对值
                        if(is_opnd(cur_ch)) //当前字符是数字
                        {
                            cin.putback(cur_ch); //放回输入流
                            cin >> (operand); //放到中间变量里，实现了 char->opnd_type 的转变
                            opnd.push(-operand); //操作数取反入栈
                            prior_ch = '0'; //更新前一个字符，由于不是操作符，标记为‘0’
                            cur_ch = cin.get(); //再次读入一个字符
                        }
                        else //错了
                        {
                            except = true;
                            break;
                        }
                    }
                }
                else if((optr_top == ')' && cur_ch == '(')
                        || (optr_top == '=' && cur_ch == ')')
                        || (optr_top == '=' && cur_ch == '(')) //括号不匹配
                {
                    except = true;
                    break;
                }
                else if(optr_top == '(' && cur_ch == ')') //括号可以匹配
                {
                    if(optr.pop(optr_top) == UNDER_FLOW) //出错了
                    {
                        except = true;
                        break;
                    }
                    else
                    {
                        cur_ch = cin.get();
                        prior_ch = ')';
                    }
                }
                else if(cur_ch == '(' || optr_prior(optr_top) < optr_prior(cur_ch)) //()内的符号是严格单调的
                {
                    if(optr.push(cur_ch) == OVER_FLOW)
                    {
                        except = true;
                        break;
                    }
                    else
                    {
                        prior_ch = cur_ch;
                        cur_ch = cin.get();
                    }
                }
                else//单调性被破坏
                {
                    if(optr.pop(op) == UNDER_FLOW) //弹出一个操作符
                    {
                        except = true;
                        break;
                    }
                    else
                    {
                        opnd_type left, right, res;
                        if(get_2_opnd(left, right) == UNDER_FLOW) //弹出两个操作数
                        {
                            except = true;
                            break;
                        }
                        else
                        {
                            if(calc(left, op, right, res) == FAIL) //计算结果
                            {
                                except = true;
                                break;
                            }
                            else if(opnd.push(res) == OVER_FLOW) //结果压栈
                            {
                                except = true;
                                break;
                            }
                        }
                    }
                }
            }
            else //既不是操作数，也不是操作符
            {
                except = true; //出错位置位
                break;
            }

            if(optr.top(optr_top) == UNDER_FLOW) // 更新 optr_top
            {
                except = true;
                break;
            }
        }

        fflush(stdin);
        if(except == true) //是否进行异常处理
        {
            char tmp;
            printf("非法的表达式！请问是否继续计算？(y/n):");
            scanf("%c", &tmp);
            if(tmp == 'y' || tmp == 'Y')
                continue;
            else
                break;
        }
        else
            if(opnd.top(operand) == UNDER_FLOW) //同样非法
            {
                char tmp;
                printf("非法的表达式！请问是否继续计算？(y/n):");
                scanf("%c", &tmp);
                if(tmp == 'y' || tmp == 'Y')
                    continue;
                else
                    break;
            }
            else
            {
                cout << operand << endl;
                char tmp;
                printf("请问是否继续计算？(y/n):");
                scanf("%c", &tmp);
                if(tmp == 'y' || tmp == 'Y')
                    continue;
                else
                    break;
            }
    }
    return;

}

#endif // CALCULATOR_H_INCLUDED
