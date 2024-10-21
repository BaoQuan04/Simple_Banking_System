#include "Wallet.h"

#include<iostream>
#include<string>
#include <algorithm>
#include <sstream>
#include <regex>
#include<cstring>
#include<stdlib.h>
#include<ctime>

using namespace std;

int Wallet::Num = 0;

// void Wallet::Create_TotalWallet(){
//     this->MaVi = string(3, '0');
//     this->Point = 50000 ;
//     this->Spent =0;
//     this->Claim =0;
// }
// bool Wallet::Check_SpCl(){
//     if(this->Claim == this->Spent) return true;
//     else return false;
// }


       
Wallet::Wallet(){
    this->Point = 5000;
    this->MaVi ="";
    this->Claim =5000;
    this->Spent =0;
}
void Wallet::Create_Mavi(){
    Num++;
    this->MaVi = string(3-to_string(Num).length(), '0') + to_string(Num);
}

void Wallet::Minus_Point(string){
    
}
void Wallet::Add_Point(string){

}

void Wallet::Check_Balance(){

}
void Wallet::Check_History(){

}

void Wallet::Check_TransUser(){

}

bool Wallet::CreateOTP(){
    
}

void Wallet::Transfer(){
    
}

int main(){
    return 0;
}
