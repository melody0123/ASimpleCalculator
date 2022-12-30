#ifndef STACK_H_INCLUDED
#define STACK_H_INCLUDED

#include <cstdio>
#include "StatusCode.h"

using namespace std;

template<class stack_element>
class Stack //顺序栈模板
{
private:
    int counter; //元素个数计数器
    int max_size; //栈顶“指针”，用于记录最多可以容纳的元素个数
    stack_element *sta; //指向数组（顺序栈）的指针
    bool full() const; //判满
    //void init(int siz); //初始化一个大小为 siz 的空栈
public:
    Stack(int siz = 1024); //构造函数
    Stack(const Stack<stack_element> &stack_obj); //复制构造函数
    Stack<stack_element> &operator=(const Stack<stack_element> &stack_obj); //赋值运算符重载
    virtual ~Stack(); //析构函数
    int length() const; //求栈长度
    bool empty() const; //判空
    void clear(); //清空栈
    StatusCode push(const stack_element &e); //压栈 1 个元素
    StatusCode pop(stack_element &e); //弹出栈顶元素
    StatusCode top(stack_element &e); //返回栈顶元素
};

template <class stack_element>
bool Stack<stack_element>::full() const
{
    return counter == max_size; //栈满返回 true
}

//template <class stack_element>
//void Stack<stack_element>::init(int siz)
//{
//    counter = 0;
//    max_size = siz;
//    if(sta != nullptr)
//    {
//        delete []sta; //如果栈不为空，则需要先释放内存空间
//        sta = nullptr;
//    }
//    sta = new stack_element[max_size];
//    if(sta == nullptr)
//    {
//        printf("内存分配失败！\n");
//        exit(1);
//    }
//
//    return;
//}

template <class stack_element>
Stack<stack_element>::Stack(int siz)
{
    counter = 0;
    max_size = siz;
//    if(sta != nullptr)
//    {
//        delete []sta; //如果栈不为空，则需要先释放内存空间
//        sta = nullptr;
//    }
    sta = new stack_element[max_size];
    if(sta == nullptr)
    {
        printf("内存分配失败！\n");
        exit(1);
    }
}

template <class stack_element>
Stack<stack_element>::Stack(const Stack<stack_element> &stack_obj)
{
    //init(stack_obj.max_size); //初始化新栈
    counter = 0;
    max_size = 1024;
//    if(sta != nullptr)
//    {
//        delete []sta; //如果栈不为空，则需要先释放内存空间
//        sta = nullptr;
//    }
    sta = new stack_element[max_size];
    if(sta == nullptr)
    {
        printf("内存分配失败！\n");
        exit(1);
    }

    counter = stack_obj.counter; //移动栈顶指针
    int temp_cnt = 0; //临时“指针”，用于指向当前位置的元素
    while(temp_cnt <= counter - 1)  //从栈底开始挨个复制元素
    {
        sta[temp_cnt] = stack_obj.sta[temp_cnt];
        temp_cnt++;
    }
}

template <class stack_element>
Stack<stack_element> &Stack<stack_element>::operator=(const Stack<stack_element> &stack_obj)
{
    if(&stack_obj != this) //检查是否是自己给自己赋值
    {
        counter = 0;
        max_size = 1024;
//        if(sta != nullptr)
//        {
//            delete []sta; //如果栈不为空，则需要先释放内存空间
//            sta = nullptr;
//        }
        sta = new stack_element[max_size];
        if(sta == nullptr)
        {
            printf("内存分配失败！\n");
            exit(1);
        }

        counter = stack_obj.counter; //移动栈顶指针
        int temp_cnt = 0; //临时“指针”，用于指向当前位置的元素
        while(temp_cnt <= counter - 1) //从栈底开始挨个复制元素
        {
            sta[temp_cnt] = stack_obj.sta[temp_cnt];
            temp_cnt++;
        }
    }

    return *this;
}

template <class stack_element>
Stack<stack_element>::~Stack()
{
    delete []sta;
}

template <class stack_element>
int Stack<stack_element>::length() const
{
    return counter;
}

template <class stack_element>
bool Stack<stack_element>::empty() const
{
    return counter == 0;
}

template <class stack_element>
void Stack<stack_element>::clear()
{
    counter = 0;
    return;
}

template <class stack_element>
StatusCode Stack<stack_element>::push(const stack_element &e)
{
    if(full())
    {
        printf("栈已满！\n");
        return OVER_FLOW;
    }
    else
    {
        sta[counter++] = e;
        return SUCCESS;
    }
}

template <class stack_element>
StatusCode Stack<stack_element>::pop(stack_element &e)
{
    if(empty())
    {
        printf("栈已空！\n");
        return UNDER_FLOW;
    }
    else
    {
        e = sta[--counter];
        return SUCCESS;
    }
}

template <class stack_element>
StatusCode Stack<stack_element>::top(stack_element &e)
{
    if(empty())
    {
        printf("栈已空！\n");
        return UNDER_FLOW;
    }
    else
    {
        e = sta[counter - 1];
        return SUCCESS;
    }
}

#endif // STACK_H_INCLUDED
