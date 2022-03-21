#include <iostream>
#include <cmath>
#include <cstring>
#include <fstream>
#include <iomanip>

using namespace std;

bool hexDump(const char *fileName)
{
    fstream ifs(fileName, ios::in | ios::binary);
    if (ifs.fail())
        return false;
    int ofs = 0;

    for (char x; ifs.get(x);)
    {
        if (ofs % 16 == 0)
        {
            cout << setw(8) << ofs;
        }
        cout << ' ' << hex << setw(2) << setfill('0') << (int)(unsigned char)x;
        if (ofs % 16 == 15)
            cout << endl;
        ofs++;
    }
    ifs.close();
    return true;
}

int main(int argc, char *argv[])
{
    for (int i = 1; i < argc; i++)

        hexDump(argv[i]);

    return 0;
}