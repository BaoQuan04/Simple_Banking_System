
#include "Signup_Login.h"

#include<iostream>
#include<string>
#include <algorithm>
#include <sstream>
#include <regex>
#include<cstring>
#include<stdlib.h>
#include<ctime>


#include <unordered_map>

using namespace std;

//Tao ra ma OTP
bool Sign_Log::CreateOTP(){
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

//----------------XU LY DANG KY TAI KHOAN CHO USER------------


//Kiem tra du lieu user nhap vao co bi bo trong hay khong
bool Sign_Log::Check_HopLe(){
    if (this->username.empty() || this->password.empty()){
        cout << "Ten dang nhap hoac mat khau khong duoc de trong." << endl;
        return false;
    }
    return true;
}

//Kiem tra username nhap vao co ton tai hay khong
bool Sign_Log::Check_TonTai(){
    return account.find(this->username) !=account.end();
}


//Nhap mk 2 lan va kiem tra mk co trung khop voi nhau hay khong
bool Sign_Log::Check_MatKhau(){
    string mk1, mk2;
    cout << "Nhap mat khau: ";
    cin >> mk1;

    cout << "Nhap lai mat khau: ";
    cin >> mk2;
    cin.ignore();
    if (mk1 != mk2) {
        cout << "Mat khau khong khop. Vui long thu lai!" << endl;
        return false;
    }
    else this-> password = mk1 ;
    return true;
} 

//Xu ly khi ten nguoi dung da ton tai
void Sign_Log::XuLyTK(){
    int choice;
    cout << "--------Ten dang nhap da ton tai--------." << endl;
    cout << "1. Thu lai (Retry)" << endl;
    cout << "2. Dang nhap (Login)" << endl;
    cout << "Chon chuc nang: " ;
    cin >> choice;
    cin.ignore(); // Để bỏ qua ký tự '\n' trong buffer

    if (choice == 1) {
        DangKy(); // Thử lại đăng ký
    } else if (choice == 2) {
        //chuyển tới hàm Đăng nhập
        DangNhap();
    } else {
        cout << "Lua chon khong hop le!" << endl;
    }
}


//Nguoi dung quen mk va muon tao lai mk
void Sign_Log::SuaMatKhau(){
    cout << "Nhap ten dang nhap: " ;
    string chk;
    getline(cin , chk);
    if(Check_Username(chk)){
        if(CreateOTP()){
            while (!Check_MatKhau()){// Nếu mật khẩu không khớp, lặp lại yêu cầu
                cout << "1. Thu lai.\n"; //Neu chon 1 thi nguoi dung quay lai nhap lai mk
                cout << "0. Thoat!\n";//Neu nguoi dung chon 0 thi dung chuong trinh
                int chk ;
                cin >> chk;
                if(chk == 0){
                    return;
                }
            } 
        }
        else return;
    }
}

//Luu tk vao trong map
void Sign_Log::LuuTaiKhoan(){
    account[username] = password;
    cout << "Dang ky thanh cong!" << endl;
}

//Nhap cac thong tin va check mk
bool Sign_Log::NhapThongTin(){
    cout << "Nhap ten dang nhap: ";
    getline(cin, username);
    
    // Nhập và xác nhận mật khẩu
    while (!Check_MatKhau()){// Nếu mật khẩu không khớp, lặp lại yêu cầu
        cout << "1. Thu lai.\n";
        cout << "0. Thoat!\n";
        int chk ;
        cin >> chk;
        cin.ignore();
        if(chk == 0){
            return false ;//Neu nguoi dung chon thoat thi tra ve gia tri false 
        }
    }   
    return true;
}


void Sign_Log::DangKy(){
    cout << "==============SIGNUP=============\n" ;
    if(!NhapThongTin()){
        cout <<"Dang ky khong thanh cong!";
        return ;//Dung chuong trinh
    }
    if (Check_HopLe()) {
        if(Check_TonTai()) {
            XuLyTK(); // Nếu tên đã tồn tại, xử lý tình huống
        }else {
            // if(!CreateOTP()) return;
            LuuTaiKhoan();
        }
    }
    cout << "=================================\n";
}

//----------------XU LY DANG NHAP CHO USER----------------

//Check xem ten nguoi dung va mk co trung khop hay khong
bool Sign_Log::Check_TaiKhoan( string username, string password ){
    unordered_map<string, string >::iterator it = account.find(username);
    if (it !=  account.end() && it->second == password) {
        return true;
    }
    return false;
}   


//Check xem username co ton tai hay khong
bool Sign_Log::Check_Username( string username){
    return  account.find(username) !=  account.end();
}  


//Xu ly dang nhap
void Sign_Log::DangNhap(){
    cout << "===========LOGIN=============\n" ;
    string username, password, role;
    int a = 0;
    //Neu nguoi dung nhap sai qua 3 lan thi chuyen den menu
    while (a < 3) {
        cout << "Nhap ten dang nhap: ";
        getline(cin, username);
        cout << "Nhap mat khau: ";
        cin >> password;
        cin.ignore();
        if (Check_TaiKhoan(username, password)) {
            cout << "Dang nhap thanh cong!" << endl;
            cout <<"=================================\n";
            return;
            // // Kiểm tra adim hay user
            // cout << "Bạn có tài khoản. Chọn 1 trong 2 quyền: " << endl;
            // cout << "1. Đăng nhập với quyền user" << endl;
            // cout << "2. Đăng nhập với quyền admin" << endl;
            // int choice;
            // cout << "Nhập lựa chọn của bạn: ";
            // cin >> choice;
            // if (choice == 1) {
            //     cout << "Đăng nhập với quyền user." << endl;
            //     // Thực hiện các chức năng dành cho user
            //     //
            // } else if (choice == 2) {
            //     cout << "Đăng nhập với quyền admin." << endl;
            //     // Thực hiện các chức năng dành cho admin
            //     //
            // } else {
            //     cout << "Lựa chọn không hợp lệ." << endl;
            // }
            // return;
        } else {
            a ++;
            if(a == 3) break;
            cout << "Ten dang nhap hoac mat khau khong dung. Vui long thu lai!" << endl;
        }

        if (a  == 3) {
            cout << "Dang nhap that bai qua so lan quy dinh !\n";
            while (true) {//menu hien ra cac chuc nang cho den khi nguoi dung thoat
                cout << "---------Chon chuc nang-------------\n";
                // cout << "1. Dang nhap bang ma OTP.\n";
                cout << "1. Dang nhap lai.\n";
                cout << "2. Quen mat khau.\n";
                cout << "3. Tao tai khoan.\n";
                cout << "0. Thoat!\n";
                cout << "--------------------------------------";
                int chk;
                cin >> chk;
                cin.ignore();
                // if(chk == 1){
                //     if(CreateOTP() == true) cout << "Dang nhap thanh cong!\n";
                // }
                if(chk == 1) DangNhap();
                else if(chk == 2) SuaMatKhau();
                else if(chk == 3) DangKy();
                else if(chk == 0) return ;
            }
        }
    }
}

void Sign_Log::InThongTin(){
    for(auto x : account){
        cout << x.first << " " << x.second << endl;
    }
}

void Sign_Log::Create_Menu(){
    while(1){
        cout << "HE THONG DANG KY VA DANG NHAP TAI KHOAN VA QUAN LI DIEM THUONG\n" ;
        cout << "Cac chuc nang:\n";
        cout << "1. Dang ky tai khoan.\n";
        cout << "2. Dang nhap tai khoan nguoi dung.\n";
        cout << "0. Thoat!\n";
        cout << "Chon chuc nang: ";
        int choice;
        cin >> choice;
        cin.ignore();
        if(choice == 1) DangKy();
        else if(choice == 2) DangNhap();
        else if(choice == 0){
            cout << "Ban da thoat chuong trinh!";
            return;
        }
        else {
        	do{
        		cout << "Chuc nang nhap vao khong hop le! Vui long thu lai.\n";
        		cout << "Chon chuc nang: ";
        		cin >> choice;
        		cin.ignore();
        		if(choice == 1) DangKy();
        		else if(choice == 2) DangNhap();
        		else if(choice == 0){
            		cout << "Ban da thoat chuong trinh!";
            		return;
       			}
			}while(choice != 1 || choice != 2 || choice != 0);
		}
    }
}


int main(){
    Sign_Log a;
    a.Create_Menu();
    return 0;
}
