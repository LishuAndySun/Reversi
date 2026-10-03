#include<iostream>
#include<thread>
#include<chrono>
using namespace std;

int main()
{
    cout << 114514;
    this_thread::sleep_for(chrono::seconds(2));
    return 0;
}