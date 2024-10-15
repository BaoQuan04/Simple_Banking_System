#include "User.h"
#include<iostream>
#include<string>
#include <algorithm>
#include <sstream>
#include <regex>
using namespace std;


//--------Dinh Nghia Cons----------
User::User() : Ten(""), NgaySinh(""), GioiTinh(""), Cccd(""), Sdt(""), DiaChi("") {}


//--------------------Define cac ham get thong tin------------------------------
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


//---------------------Define cac ham set thong tin------------------------------
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


//------------------Ham chuan hoa------------------------------------------
string User::ChuanHoa_Ten(string a){
    string res = "", t;
    stringstream ss(a);
    while(ss >> t){
        res += toupper(t[0]);
        for(int i = 1; i < t.size(); i++){
            res += tolower(t[i]);
        }
        res += " ";
    }
    a = res;
    return a;
}


string User::ChuanHoa_NgaySinh(string a){
    if(a[2] != '/'){
        a = '0' + a;
    }
    if(a[5] != '/'){
        a.insert(3,"0");
    }
    return a;
}


string User::ChuanHoa_GioiTinh(string a){
    string res = "";
    res += toupper(a[0]);

    for(int i = 1; i < a.size(); i++){
        res += tolower(a[i]);
    }
    a = res;
    return a;
}

string User::ChuanHoa_DiaChi(string a){
    string res = "", t;
    stringstream ss(a);
    while(ss >> t){
        res += toupper(t[0]);
        for(int i = 1; i < t.size(); i++){
            res += tolower(t[i]);
        }
        res += " ";
    }
    a = res;
    return a;
}


//------------------------Ham kiem tra----------------------------------------
bool User::Check_NgaySinh(string& a){

    // Định dạng DD/MM/YYYY: Ngày (01-31), Tháng (01-12), Năm (1000-9999)
    regex datePattern(R"(^((0[1-9])|([12][0-9])|(3[01]))\/((0[1-9])|(1[0-2]))\/(\d{4})$)");

    // Kiểm tra xem chuỗi date có khớp với định dạng không
    return regex_match(a, datePattern);
}
bool User::Check_GioiTinh(string& a){
    string res = "";
    for(char& x : a){
        res += tolower(x);
    }
    if(res == "nam" || res == "nu"){
        return true;
    }
    return false;
}
bool User::Check_Cccd(string& a){
    if(a.size() != 12){
        return false;
    }
    for(char& x : a){
        if(x < '0' && x > '9'){
            return false;
        }
    }
    return true;
}
bool User::Check_Sdt(string& a){
    //Kiem tra xem so dau co phai la so 0 hay khong
    if(a[0] != '0'){
        return false;
    }
    //Kiem tra co du 10 ki tu hay khong
    if(a.size() != 10){
        return false;
    }
    //Kiem tra co phai la cac chu so hay khong
    for(char& x : a){
        if(x < '0' && x > '9'){
            return false;
        }
    }    

    return true;
}
bool User::Check_Email(string& a){
    string mail = "@gmail.com";
    if(a.find(mail) == string :: npos){
        return false;
    }

    string res = "", t;
    stringstream ss(a);
    while(getline(ss,t,'@') ){
        res = t;
    }
    for(int i = 0; i < t.size(); i++){
        if(t[i] < 'a' && t[i] > 'z'){
            return false;
        }
    }
    return true;
}



//----------------------Cac ham nhap xuat sua thong tin user-------------------
void User::NhapThongTin(){
    cout << "CAP NHAP THONG TIN USER !" << endl;
    string a;

    cout << "Nhap Ho va Ten (Vi du: Nguyen Van A) :" ;
    getline(cin, a);
    cin.ignore();
    this->Ten = ChuanHoa_Ten(a);

    cout << "Nhap ngay sinh (Vi du: 01/01/2000) :" ;
    cin >> a ;
    a=ChuanHoa_NgaySinh(a);
    if(!Check_NgaySinh(a)){
        do{
            cout << "Ngay sinh khong hop le! Vui long nhap lai :" ;
            cin >> a;
            a=ChuanHoa_NgaySinh(a);
        }while(!Check_NgaySinh(a));
    }
    this->NgaySinh = a;
    a = "";

    cout << "Nhap gioi tinh (Nam hoac Nu) :" ;
    cin >> a;
    //if(a == "Les" || a == "Gay" ||  a == "Bisexual" || a == "Transgender") return 0;
    if(!Check_GioiTinh(a)){
        do{
            cout << "Gioi tinh khong hop le! Vui long nhap lai :" ;
            cin >> a;
        }while(!Check_GioiTinh(a));
    }
    this->GioiTinh = ChuanHoa_GioiTinh(a);
    a ="" ;

    cout << "Nhap so Can cuoc cong dan :" ;
    cin >> a;
    if(!Check_Cccd(a)){
        do{
            cout << "So can cuoc cong dan khong hop le! Vui long nhap lai :" ;
            cin >> a ;
        }while(!Check_Cccd(a));
    }
    this->Cccd = a;
    a = "";

    cout <<  "Nhap so dien thoai (Vi du: 0123456789):" ;
    cin >> a;
    if(!Check_Sdt(a)){
        do{
            cout << "So dien thoai nhap vao khong hop le! Vui long nhap lai :" ;
            cin >> a;
        }while(!Check_Sdt(a));
    }
    this->Sdt = a;
    a = "";

    cout << "Nhap dia chi (Vi du: Dong Thap) :" ;
    getline(cin , a);
    cin.ignore();
    this->DiaChi = ChuanHoa_DiaChi(a);
    a = "" ;

    cout << "Nhap dia chi email (Vi du: nguyenvana@gmail.com) :" ;
    cin >> a ;
    if(!Check_Email(a)){
        do{
            cout << "Email nhap vao khong hop le! Vui long nhap lai: ";
            cin >> a;
        }while(!Check_Email(a));
    }
    this->Email = a;
}

void User::XuatThongTin(){
    cout << this->Ten << " " << this->NgaySinh << " " << this->GioiTinh << " " << this->Cccd << " " << this->Sdt << " " << this->DiaChi << " " << this-> Email << endl;
}





int main(){
    User a;
    a.NhapThongTin();
    a.XuatThongTin();
	return 0;
}
