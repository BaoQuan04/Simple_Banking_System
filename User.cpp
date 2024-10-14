#include "User.h"
#include<iostream>
#include<string>

using namespace std;

//--------------------Define cac ham get thong tin--------------------
string User::getMaKhachHang(){
    return this->MaKhachHang;
}
string User::getTen(){
    return this->Ten;
}
string User::getNgaySinh(){
    return this->NgaySinh;
}
string User::getGioiTinh(){
    return this->GioiTinh;
}
string User::getCccd(){
    return this->Cccd;
}
string User::getSdt(){
    return this->Sdt;
}
string User::getDiaChi(){
    return this->DiaChi;
}
string User::getEmail(){
    return this->Email;
}


//---------------------Define cac ham set thong tin------------------
void User::setTen(string a){
    this->Ten = a;
}
void User::setNgaySinh(string a){
    this->NgaySinh = a;
}
void User::setGioiTinh(string a){
    this->GioiTinh = a;
}
void User::setCccd(string a){
    this->Cccd = a;
}
void User::setSdt(string a){
    this->Sdt = a;
}
void User::setDiaChi(string a){
    this->DiaChi = a;
}
void User::setEmail(string a){
    this->Email = a;
}



//----------------------Cac ham nhap xuat sua thong tin user-------------------
int main(){
	return 0;
}
