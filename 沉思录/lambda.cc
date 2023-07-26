#include <functional>
#include <list>
#include <iostream>
//匿名函数
//
using namespace std;
int funA(int a, int b)
{
	printf("funA\n");
	return 0;
}
class A
{
public:
    A() { a = 100; }
    const int getA(){return a;}
    int getA1(){return a;}
private:
    int a=0;
};
int main()
{

    function<int(int, int)> call;
    // function<int(int, int)> call=funA; 指定函数的定义
    int n = 10;
    A a1;
    cout << "n的值为" << n << endl;
    //捕获 mutable exception attribute属性
    call = [n,a1](int a, int b)mutable-> int
    {
        printf("%d\n", n);
        // n = 100;
        cout<<"a1对象的值为"<<a1.getA()<<endl;
        cout<<"a1对象的值为"<<a1.getA1()<<endl;
        return 2;
    };
    int res = call(3, 1);
    cout << res << endl;
    cout << "n的值为" << n << endl;
    return 0;
}
