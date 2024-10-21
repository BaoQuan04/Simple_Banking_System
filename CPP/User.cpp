#include "User.h"
#include<iostream>
#include<string>
#include <algorithm>
#include <sstream>
#include <regex>
#include<cstring>
#include<stdlib.h>
#include<ctime>
using namespace std;

//--------Dinh Nghia Cons----------------
User::User() : Ten(""), NgaySinh(""), GioiTinh(""), Cccd(""), Sdt(""), DiaChi("") {}

//-------Khoi tao ma OTP va tra ve T or F------------
bool User::CreateOTP(){
    //Khoi tao seed cho rand() de sinh so ngau nhien thuc su
    const int nums = 7;
    srand(time(0)); //Su dung thoi gian hien tai lam seed
    int cnt =3;
    while(cnt--){
        char code[nums], ch ,otp[nums];
        int n = 0;
        while(n < nums -1){
            int res = 0 + (int) (rand() * (127 - 0 + 1.0) / (1.0 + RAND_MAX));
            ch = (char) res;
            if( (ch >= '0' && ch <= '9') ){
                code[n] = ch;
                n++;
            }
        }
        code[nums] =  '\0';
        
        cout << "Ma OTP cua ban la: " << code << endl;
        cout << "Nhap ma OTP : ";
        cin >> otp ;
        int cnt  =0;
        if(strcmp(otp,code) == 0){
            cout << "Ma OTP hop le !" << endl;
            return true;
        } else {
            cout << "Ma OTP khong hop le !" << endl;
        }
    }
    cout << "Ban da nhap sai qua so lan quy dinh! Thao tac bi huy.\n";
    return false ;
}


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

    string ngay = "", thang = "", nam = "";
    int n,t,y;
    stringstream ss(a);
    getline(ss,ngay,'/');
    n = stoi(ngay);
    getline(ss,thang,'/');
    t = stoi(thang);
    getline(ss,nam);
    y = stoi(nam);


    if((t == 1)  || (t == 3) || (t == 5) || (t == 7) || (t == 8) || (t == 10) || (t == 12)){
        if(n < 1 || n > 31){
            return false;
        }
    } 
    if((t == 4) || (t == 6) || (t == 9) || (t == 11)){
        if(n < 1 || n > 30){
            return false;
        } 
    }

    bool namnhuan = false ;
        if(y % 4 == 0 && y % 100 != 0 && y != 0 || y % 400 ==0){
            namnhuan = true;
        }
    if(t == 2 && namnhuan){
        if(n < 0 || n > 29){
            return false;
        }
    }
    if(t == 2 && !namnhuan){
        if(n < 0 || n > 28){
            return false;
        }
    }

    return true;
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
    getline(ss,t,'@');
    //update truong hop mail khong co gi
    if(t == res){
       return false;
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
   
//----------Nhap Ten-------------
    string name;
    cout << "Nhap Ho va Ten (Vi du: Nguyen Van A) :" ;
    cin.ignore();
    getline(cin, name);
    name = ChuanHoa_Ten(name);

//---------Nhap Ngay sinh------
    string birth;
    cout << "Nhap ngay sinh (Vi du: 01/01/2000) :" ;
    cin >> birth ;
    birth =ChuanHoa_NgaySinh(birth);
    if(!Check_NgaySinh(birth)){
        do{
            cout << "Ngay sinh khong hop le! Vui long nhap lai :" ;
            cin >> birth;
            birth =ChuanHoa_NgaySinh(birth);
        }while(!Check_NgaySinh(birth));
    }
    
//------Nhap Gioi tinh----------
    string sex;
    cout << "Nhap gioi tinh (Nam hoac Nu) :" ;
    cin >> sex;
    if(!Check_GioiTinh(sex)){
        do{
            cout << "Gioi tinh khong hop le! Vui long nhap lai :" ;
            cin >> sex;
        }while(!Check_GioiTinh(sex));
    }
    sex = ChuanHoa_GioiTinh(sex);

//--------Nhap CCCD------------
    string id;
    cout << "Nhap so Can cuoc cong dan :" ;
    cin >> id;
    if(!Check_Cccd(id)){
        do{
            cout << "So can cuoc cong dan khong hop le! Vui long nhap lai :" ;
            cin >> id ;
        }while(!Check_Cccd(id));
    }
    
//--------Nhap Sdt-----------
    string number;
    cout <<  "Nhap so dien thoai (Vi du: 0123456789):" ;
    cin >> number;
    if(!Check_Sdt(number)){
        do{
            cout << "So dien thoai nhap vao khong hop le! Vui long nhap lai :" ;
            cin >> number;
        }while(!Check_Sdt(number));
    }

//---------Nhap Dia Chi----------
    string address;
    cout << "Nhap dia chi (Vi du: Dong Thap) :" ;
    cin.ignore();
    getline(cin , address);
    address = ChuanHoa_DiaChi(address);

//-----------Nhap email-------------
    string mail;
    cout << "Nhap dia chi email (Vi du: nguyenvana@gmail.com) :" ;
    cin >> mail ;
    if(!Check_Email(mail)){
        do{
            cout << "Email nhap vao khong hop le! Vui long nhap lai: ";
            cin >> mail;
        }while(!Check_Email(mail));
    }
    
    int check = CreateOTP();
    if(check == true){
        this->Ten = name;
        this->NgaySinh = birth;
        this->GioiTinh = sex;
        this->Cccd = id;
        this->Sdt = number;
        this->DiaChi = address;
        this->Email = mail;
    }
    else return;

}


void User::SuaTen(){
    string a;
    cout << "Nhap Ho va Ten (Vi du: Nguyen Van A) :" ;
    cin.ignore();
    getline(cin, a);
    a = ChuanHoa_Ten(a);
    int check =CreateOTP();
    if(check == true){
        this->Ten = a;
    }
    else return;
    
}
void User::SuaNgaySinh(){
    string a;
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
    int check =CreateOTP();
    if(check == true){
        this->NgaySinh = a;
    }
    else return;
}
void User::SuaGioiTinh(){
    string a;
    cout << "Nhap gioi tinh (Nam hoac Nu) :" ;
    cin >> a;
    if(!Check_GioiTinh(a)){
        do{
            cout << "Gioi tinh khong hop le! Vui long nhap lai :" ;
            cin >> a;
        }while(!Check_GioiTinh(a));
    }
    a = ChuanHoa_GioiTinh(a);
    int check =CreateOTP();
    if(check == true){
        this->GioiTinh = a;
    }
    else return;
}
void User::SuaCccd(){
    string a;
    cout << "Nhap so Can cuoc cong dan :" ;
    cin >> a;
    if(!Check_Cccd(a)){
        do{
            cout << "So can cuoc cong dan khong hop le! Vui long nhap lai :" ;
            cin >> a ;
        }while(!Check_Cccd(a));
    }
    int check =CreateOTP();
    if(check == true){
        this->Cccd = a;
    }
    else return;
}
void User::SuaSdt(){
    string a;
    cout <<  "Nhap so dien thoai (Vi du: 0123456789):" ;
    cin >> a;
    if(!Check_Sdt(a)){
        do{
            cout << "So dien thoai nhap vao khong hop le! Vui long nhap lai :" ;
            cin >> a;
        }while(!Check_Sdt(a));
    }
    int check =CreateOTP();
    if(check == true){
        this->Sdt = a;
    }
    else return;
}
void User::SuaDiaChi(){
    string a;
    cout << "Nhap dia chi (Vi du: Dong Thap) :" ;
    cin.ignore();
    getline(cin , a);
    a = ChuanHoa_DiaChi(a);
    int check =CreateOTP();
    if(check == true){
        this->DiaChi = a;
    }
    else return;
}
void User::SuaEmail(){
    string a;
    cout << "Nhap dia chi email (Vi du: nguyenvana@gmail.com) :" ;
    cin >> a ;
    if(!Check_Email(a)){
        do{
            cout << "Email nhap vao khong hop le! Vui long nhap lai: ";
            cin >> a;
        }while(!Check_Email(a));
    }
    int check =CreateOTP();
    if(check == true){
        this->Email = a;
    }
    else return;
}



void User::XuatThongTin(){
    cout << "------THONG TIN NGUOI DUNG------\n";
    cout << "Ho va ten : " << this->Ten << endl;
    cout << "Ngay sinh : " << this->NgaySinh << endl;
    cout << "Gioi tinh : " << this->GioiTinh << endl;
    cout << "So can cuoc cong dan : " << this->Cccd << endl;
    cout << "So dien thoai : " << this->Sdt << endl;
    cout << "Dia chi : " << this->DiaChi << endl;
    cout << "Email : " << this->Email << endl;
    cout << "---------------------------------\n" ;
}



void User::SuaThongTin(){
    while(1){
        cout << "==========Chon thong tin can thay doi=========\n" ;
        cout << "1. Thay doi toan bo thong tin.\n" ;
        cout << "2. Thay doi ten.\n";
        cout << "3. Thay doi ngay sinh.\n";
        cout << "4. Thay doi gioi tinh.\n";
        cout << "5. Thay doi so Cccd.\n";
        cout << "6. Thay doi Sdt.\n";
        cout << "7. Thay doi dia chi.\n";
        cout << "8. Thay doi email.\n";
        cout << "9. Xem thong tin hien tai.\n";
        cout << "0. Thoat !\n" ;
        cout << "==============================================\n" ;
        int a;
        cout << "Chon chuc nang: " ;
        cin >> a;
        if(a == 1) NhapThongTin();
        else if(a == 2) SuaTen();
        else if(a == 3) SuaNgaySinh();
        else if(a == 4) SuaGioiTinh();
        else if(a == 5) SuaCccd();
        else if(a == 6) SuaSdt();
        else if(a == 7) SuaDiaChi();
        else if(a == 8) SuaEmail();
        else if(a == 9) XuatThongTin();
        else break;
    }
}



int main(){
    User a;
    a.SuaThongTin();
	return 0;
}
