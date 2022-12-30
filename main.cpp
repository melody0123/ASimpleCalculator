#include <iostream>
#include <cstdio>
#include "Stack.h"
#include "Calculator.h"

using namespace std;

int main()
{
    while(true)
    {
        char opt; //选择数据类型的开关
        printf("请选择你想要计算的数据类型(d:double/i:int/q:quit)：");
        fflush(stdin);
        scanf("%c", &opt);
        if(opt == 'd')
        {
            Calculator<double> C;
            C.run();
        }
        else if(opt == 'i')
        {
            Calculator<int> C;
            C.run();
        }
        else if(opt == 'q')
            break;
        else
            printf("错误的数据类型！\n");
    }
    return 0;
}
