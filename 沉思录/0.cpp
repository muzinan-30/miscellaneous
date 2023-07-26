#include <cstdio>
//数据隐藏
//inline内联
//printf(args) fprintf(stdout,args)
//类会提供一个位置来存储状态信息
class Trace
{
public:
    Trace() {notify=0; f=stdout;}
    Trace(FILE* ff){ notify=0; f=ff;}
    void print(char *s)
    {
        if (notify)
        {
            fprintf(f,"%s\n", s);
        }
    }
    void off() { notify = 0; }
    void on() { notify = 1; }

private:
    int notify;
    FILE* f;
};


int main(){
    Trace t(stderr);
    t.on();
    char s[12]="123";
    t.print(s);
    return 0;
}
