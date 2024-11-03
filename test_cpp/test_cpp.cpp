#include<iostream>
#include<string>
#include <algorithm>
#include <sstream>
#include <regex>
#include<cstring>
#include<stdlib.h>
#include<ctime>
#include<map>
#include <libpq-fe.h>
#include <cstdlib> 
using ull = unsigned long long;
using namespace std;


ull getHash(string mk){
    ull Value = 0;
    for(int i = 0; i < mk.size(); i++){
        Value = Value * 6 + (ull) mk[i];
    }
    return Value;
}

int main() {
    string mk;
    cin >> mk;
    cin.ignore();
    ull res = getHash(mk);
    cout<< res << endl;
    system("pause");
    return 0;
}