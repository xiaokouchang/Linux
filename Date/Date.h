#include<iostream>
#include<assert.h>
using namespace std;
class Date
{
    friend ostream& operator<<(ostream& out,const Date& x);
    friend istream& operator>>(istream& in,Date& x);
public:
    Date(int year = 1,int month = 1,int day = 1);
    Date& operator+=(int day);
    Date& operator-=(int day);
    Date operator+(int day);
    Date operator-(int day);
    Date& operator++();
    Date operator++(int);
    Date& operator--();
    Date operator--(int);
    int GetMonthDay(int year,int month);
    bool operator>(const Date& x);
    bool operator>=(const Date& x);
    bool operator<(const Date& x);
    bool operator<=(const Date& x);
    bool operator==(const Date& x);
    bool operator!=(const Date& x);
    void Print();
private:
    int _year;
    int _month;
    int _day;
};
