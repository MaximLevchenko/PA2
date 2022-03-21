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

class CGarage
{

public:
    CGarage(void);
    bool addCar(const string &rz, const string &owner, int age);
    bool delCar(const string &rz);
    void sortCars(void);
    void printCars(void);

private:
    TCar m_Db[CAR_MAX];
    size_t m_DbLen;
};
//----------------------------------------
CGarage::CGarage(void)
{
    m_DbLen = 0;
}
bool CGarage::addCar(const string &rz, const string &owner, int age)
{
    if (m_DbLen >= CAR_MAX)
        return false;
    for (size_t i = 0; i < m_DbLen; i++)
    {
        if (m_Db[i].m_RZ == rz)
            return false;
    }

    m_Db[m_DbLen].m_RZ = rz;
    m_Db[m_DbLen].m_Owner = owner;
    m_Db[m_DbLen].m_Age = age;
    m_DbLen++;
    return true;
}
bool CGarage::delCar(const string &rz)
{
    for (size_t i = 0; i < m_DbLen; i++)
        if (m_Db[i].m_RZ == rz)
        {
            m_Db[i] = m_Db[--m_DbLen]; // we copy the last element of the array and just say that we decrement num of cars

            return true;
        }
    return false;
}
void CGarage::sortCars(void)
{
    for (size_t i = 0; i < m_DbLen; i++) // select sort
    {
        size_t minIdx = i;
        for (size_t j = i + 1; j < m_DbLen; j++)
        {
            if (m_Db[j].m_Age < m_Db[minIdx].m_Age)
                minIdx = j;
        }

        TCar tmp = m_Db[i];
        m_Db[i] = m_Db[minIdx];
        m_Db[minIdx] = tmp;
    }
}
void CGarage::printCars(void)
{
    cout << setfill('-') << setw(40) << "" << endl;
    for (size_t i = 0; i < m_DbLen; i++)
    {
        cout << m_Db[i].m_RZ << " " << m_Db[i].m_Owner << " " << m_Db[i].m_Age << endl;
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