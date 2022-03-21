#include <iostream>
#include <cmath>
#include <cstring>
#include <fstream>
#include <iomanip>
#include <cassert>
using namespace std;

const int CAR_MAX = 100;

struct TCar
{
    string m_RZ;
    string m_Owner;
    int m_Age;
};

bool addCar(TCar *db, size_t &dbLen, const string &rz, const string &owner, int age)
{
    if (dbLen >= CAR_MAX)
        return false;
    for (size_t i = 0; i < dbLen; i++)
    {
        if (db[i].m_RZ == rz)
            return false;
    }

    db[dbLen].m_RZ = rz;
    db[dbLen].m_Owner = owner;
    db[dbLen].m_Age = age;
    dbLen++;
    return true;
}
bool delCar(TCar *db, size_t &dbLen, const string &rz)
{
    for (size_t i = 0; i < dbLen; i++)
        if (db[i].m_RZ == rz)
        {
            db[i] = db[--dbLen]; // we copy the last element of the array and just say that we decrement num of cars

            return true;
        }
    return false;
}
void sortCars(TCar *db, size_t dbLen)
{
    for (size_t i = 0; i < dbLen; i++) // select sort
    {
        size_t minIdx = i;
        for (size_t j = i + 1; j < dbLen; j++)
        {
            if (db[j].m_Age < db[minIdx].m_Age)
                minIdx = j;
        }

        TCar tmp = db[i];
        db[i] = db[minIdx];
        db[minIdx] = tmp;
    }
}
void printCars(const TCar *db, size_t dbLen)
{
    cout << setfill('-') << setw(40) << "" << endl;
    for (size_t i = 0; i < dbLen; i++)
    {
        cout << db[i].m_RZ << " " << db[i].m_Owner << " " << db[i].m_Age << endl;
    }
}

int main(void)
{
    TCar db[CAR_MAX];
    size_t dbLen = 0;
    assert(addCar(db, dbLen, "ABC-12-34", "Maks Levchenko", 19));
    assert(addCar(db, dbLen, "NICECOCK", "Maks NELEVCHENKO", 17));
    assert(addCar(db, dbLen, "FENOMEN", "HI PRIVET", 50));

    printCars(db, dbLen);
    sortCars(db, dbLen);
    printCars(db, dbLen);

    assert(delCar(db, dbLen, "FENOMEN"));

    printCars(db, dbLen);

    return 0;
}