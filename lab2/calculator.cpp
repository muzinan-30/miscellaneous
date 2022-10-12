#include <iostream>
#include "Mystack.h"
#include"calculator.h"

int main()
{
    Calculator cal(300);//输入的表达式的字符个数不会超过300
    cal.run();
}