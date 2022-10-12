#include <iostream>
#include<fstream>
#include<vector>
#include "lab4.h"

using namespace std;

int find(int amount,vector<int>& coins,int len) {
    int Max = amount + 1;
    vector<int> dp(amount + 1, Max);
    dp[0] = 0;
    //从金额数为1开始推算
    for (int i = 1; i <= amount; ++i) {
        //遍历所有的硬币，如果硬币小于总额容量，那么递推
        for (int j = 0; j <len; ++j) {
            if (coins[j] <= i) {
                dp[i] = min(dp[i], dp[i - coins[j]] + 1);
            }
        }
    }
    return dp[amount] > amount ? -1 : dp[amount];
 }

//文件操作，从文件中读取数据
int findmin() {
    int count = 100;
    ifstream in;
    in.open("input.txt", ios::in);
    if (!in.is_open()) {
        cout << "打开文件出错" << endl;
        exit(1);
    }
    ofstream out;
    out.open("output.txt", ios::out);
    if (!out.is_open()) {
        cout << "打开文件出错" << endl;
        exit(1);
    }
    int sum, M;
    in >> sum;
    std::vector<int>   coin(sum);
    for (int i = 0; i < sum; i++) {
        in >> coin[i];
    }
     in >> M;
    int result=find(M, coin, sum);
    out << result;

    in.close();
    out.close();
    return result;
}

int main()
{   
    findmin();
    std::cout << "Hello World!\n";
}
