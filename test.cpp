#include <iostream>
#include <cmath>
#include <cstring>
#include <fstream>
#include <iomanip>
#include <cassert>
using namespace std;

int foo(char a, double b)
{
    return 10;
}
int foo(int a, int b)
{
    return 20;
}

int main(void)
{
    cout << foo('a', 12);

    return 0;
}