#include<iostream>
#include<string>
#include <algorithm>
#include <sstream>
#include <regex>
#include<cstring>
#include<stdlib.h>
#include<ctime>
#include<map>
#include "User.cpp"

using namespace std;

//===========================Khai bao ham chuc nang==========================

void createMenu(){
    while(1){
        cout << "HE THONG DANG KY VA DANG NHAP TAI KHOAN VA QUAN LI DIEM THUONG\n" ;
        cout << "1. Dang ky.\n";
        cout << "2. Dang nhap.\n";
        cout << "0. Thoat!\n";
        cout << "Chon chuc nang: ";
        int choice;
        cin >> choice;
        cin.ignore();
        if(choice == 1){
            DangKyofUser();
        }
        else if(choice == 2){
            while(1){
                cout << "Chon doi tuong dang nhap :\n";
                cout << "1. USER.\n";
                cout << "2. ADMIN.\n ";
                cout << "3. Quay lai!\n";
                int c;
                cout << "Chon doi tuong: ";
                cin >> c;
                cin.ignore();
                if(c==1){
                    string res = DangNhapofUser();
                    if(res != "") ChucNangUser(res);
                    else{
                        cout << "Dang nhap khong thanh cong! Vui long thu lai.\n";
                        break;
                    }
                }
                else if(c==2){
                    int tmp = DangNhapofAdmin();
                    if(tmp == 1) ChucNangAdmin();
                    else{
                        cout << "Dang nhap khong thanh cong! Vui long thu lai.\n";
                        break;
                    }
                }
                else if(c == 3) break;
            }

        }
        else if(choice == 0) return ;
    }
}

void DangKyofUser(){
   
   while(1){
       cout << "Nhap ten dang nhap: " ;
       string name;
       getline(cin , name);
       string password1, password2;
       cout << "Nhap mat khau: ";
       cin >> password1;
       cout << "Nhap lai mat khau: ";
       cin >> password2;
       if (password1 != password2) {
           cout << "Mat khau khong khop. Vui long thu lai!" << endl;
           cout << "1. Thu lai.\n";
           cout << "0. Thoat!\n";
           int chk ;
           cin >> chk;
           cin.ignore();
           if(chk == 0){
               cout << "Dang ky khong thanh cong!\n";
               return;
           }
           else continue;
       }
       if (name == "" || password1 == ""){
           cout << "Ten dang nhap hoac mat khau khong duoc de trong." << endl;   
           cout << "1. Thu lai.\n";
           cout << "0. Thoat!\n";
           int chk ;
           cin >> chk;
           cin.ignore();
           if(chk == 0){
               cout << "Dang ky khong thanh cong!\n";
               return;
           }
           else continue;
        }
        int mk = 0;
        for(int i =0;i<account.size();i++){
            if(account[i].getUsername() == name) mk = 1;
        }


       if(mk == 1){
           int choice;
           cout << "--------Ten dang nhap da ton tai--------." << endl;
           cout << "1. Thu lai (Retry)" << endl;
           cout << "2. Dang nhap (Login)" << endl;
           cout << "Chon chuc nang: " ;
           cin >> choice;
           cin.ignore(); // Để bỏ qua ký tự '\n' trong buffer

           if (choice == 1) {
               DangKyofUser(); // Thử lại đăng ký
           } else if (choice == 2) {
               //chuyển tới hàm Đăng nhập
               DangNhapofUser();
           } else {
               cout << "Lua chon khong hop le!" << endl;
           }
       }
       else{
            User a;
            string res = a.createID();
            ull tmp = getHast(password1);
            account.push_back(User(res, name, tmp));
            cout << "Dang ky thanh cong!\n";
       }
   }

}

void DangKyofAdmin(){
    while(1){
        cout << "Nhap ten nguoi dung: ";
        string name;
        getline(cin, name);
        if(name == ""){
            cout << "Ten dang nhap khong duoc de trong!\n";
            cout << "1. Thu lai.\n";
            cout << "0. Thoat!\n";
            int chk ;
            cin >> chk;
            cin.ignore();
            if(chk == 0){
                cout << "Dang ky khong thanh cong!\n";
                return;
            }
            else continue;
        }
        int mk = 0;
        for(int i = 0 ; i < account.size();i++){
            if(account[i].getUsername() == name) mk = 1;
        }
        if(mk == 1){
            int choice;
            cout << "--------Ten dang nhap da ton tai--------." << endl;
            cout << "1. Thu lai (Retry)" << endl;
            cout << "0. Thoat!\n";
            cout << "Chon chuc nang: " ;
            cin >> choice;
            cin.ignore(); // Để bỏ qua ký tự '\n' trong buffer

            if (choice == 1) {
                DangKyofAdmin(); // Thử lại đăng ký
            } 
            else {
                cout << "Tao tai khoan khong thanh cong!\n";
            }
        }
        else {
            User a;
            string res = a.createID();
            ull tmp = getHast("1");
            account.push_back(User(res, name, tmp));
            cout << "Dang ky thanh cong!. Mat khau sinh tu dong cua ban la: 1\n";
            return;
        }
    }

}

void createMenuSaiDangNhap(){
        
    while(1){
        cout << "---------Chon chuc nang-------------\n";
        
        cout << "1. Dang nhap lai.\n";
        cout << "2. Quen mat khau.\n";
        cout << "3. Tao tai khoan.\n";
        cout << "0. Thoat!\n";
        cout << "--------------------------------------";
        int chk;
        cin >> chk;
        cin.ignore();
        if(chk == 1) DangNhapofUser();
        else if(chk == 2){
            string res;
            cout << "Nhap ten dang nhap: ";
            getline(cin, res);
            int i;
            for( i = 0 ; i<account.size(); i++){
                if(account[i].getUsername() == res){
                    while(1){
                        if(!createOTP()) return ;
                        string mk1, mk2;
                        cout << "Nhap mat khau: ";
                        cin >> mk1;
                        cout << "Nhap lai mat khau: ";
                        cin >> mk2;
                        cin.ignore();
                        if (mk1 != mk2){
                            cout << "Mat khau khong khop. Vui long thu lai!" << endl;
                            cout << "1. Thu lai.\n";
                            cout << "0. Thoat!\n";
                            int chk ;
                            cin >> chk;
                            cin.ignore();
                            if(chk == 0){
                                cout << "Dang ky khong thanh cong!\n";
                                break;
                            }
                            else continue;
                        }
                        else{
                            ull res = getHast(mk1);
                            account[i].setPassword(res);
                            cout << "Thay doi mat khau thanh cong!\n";
                        }
                    }
                }
            }
            cout << "Tai khoan khong ton tai!\n";
        }
        else if(chk == 3) DangKyofUser();
        else if(chk == 0) return ;
    }
}

string DangNhapofUser(){
    //User a;
    int cnt=0;
    while(1){
        string name, mk;
        cout << "Nhap ten nguoi dung:" ;
        getline(cin, name);
        cout << "Nhap mat khau: ";
        cin >> mk;
        cin.ignore();
        for(int i=0;i<account.size();i++){
            ull tmp = getHast(mk);
            if(account[i].getUsername() == name && account[i].getPassword() == tmp){
                cout << "Dang nhap thanh cong!\n";
                return account[i].getUsername();
            }
        }
        cout << "Ten dang nhap hoac mat khau khong dung! Vui long thu lai.\n";
        cnt++;
        if(cnt == 3){
            cout << "Ban da nhap sai qua so lan quy dinh.\n";
            break;
        }
        else cout << "Ten dang nhap hoac mat khau khong dung! Vui long thu lai.\n";
    }
    createMenuSaiDangNhap();
    string  sai = "";
    return sai;
}

int DangNhapofAdmin(){
    int cnt=0;
    while(1){
        string name, mk;
        cout << "Nhap ten nguoi dung:" ;
        getline(cin, name);
        cout << "Nhap mat khau: ";
        cin >> mk;
        cin.ignore();
        for(map<string,string> :: iterator it = admin.begin();it!=admin.end();it++){
            if( (*it).first == name && (*it).second == mk){
                cout << "Dang nhap thanh cong!\n";
                return 1;
            }
        }
        cout << "Ten dang nhap hoac mat khau khong dung! Vui long thu lai.\n";
        cnt++;
        if(cnt == 3){
            cout << "Ban da nhap sai qua so lan quy dinh.\n";
            break;
        }
        else cout << "Ten dang nhap hoac mat khau khong dung! Vui long thu lai.\n";
    }
    createMenuSaiDangNhap();
    return 0;
}

void ChucNangUser(string a){
    for(int i = 0;i < account.size(); i++){
        if(account[i].getUsername() == a){
            if(account[i].getTen() == "") account[i].capnhapThongTin();
        }
    }
    
    while(1){
        cout << "=============USER=============\n";
        cout << "Cac chuc nang:\n";
        cout << "1. Xem thong tin.\n";
        cout << "2. Thay doi thong tin";
        cout << "3. Sua mat khau.\n";
        cout << "4. Quan ly vi diem thuong.\n";
        cout << "0. Thoat!\n";
        int choice;
        cout << "Chon chuc nang: ";
        cin >> choice;
        cin.ignore();
        if(choice == 1 ) xemThongTin(a);
        else if(choice == 2 ) suaThongTinofUser(a);
        else if(choice == 3) suaMatKhauofUser(a);
        else if(choice == 4) quanlyVi(a);
        else if(choice == 0) return;
    }
}

void quanlyVi(string a){
    TotalWallet total;
    int i;
    for(i=0;i<account.size();i++){
        if(account[i].getUsername() == a ) break;
    }
    if(!total.Check_Wallet(account[i])){
        total.AddPoint(account[i]);
        total.addwallet(account[i]);
    }
    while(1){
        cout << "==========Quan ly vi==========\n";
        cout << "1. Xem so du.\n";
        cout << "2. Chuyen diem.\n";
        cout << "3. Xem lich su giao dich.\n";
        cout << "0. Thoat!\n";
        cout << "Chon chuc nang: ";
        int choice;
        cin >> choice ;
        if(choice == 1) total.Check_Balance(account[i]);
        else if(choice == 2) total.Chuyen_Diem(account[i]);
        else if(choice == 3) total.Check_history(account[i]);
        else return;
    }
}

void ChucNangAdmin(){

	while(1){
        cout << "================ADMIN==============\n";
        cout << "Cac chuc nang: \n";
        cout << "1. Tao tai khoan.\n";
        cout << "2. Thay doi thong tin.\n";
        cout << "3. Quan ly khach hang.\n";
        cout << "0. Thoat!\n";
        cout << "Chon chuc nang: ";
        int choice ;
        cin >> choice;
        cin.ignore();
        if(choice == 1) DangKyofAdmin();
        else if(choice == 2) suaThongTinofAdmin();
        else if(choice == 3) quanlyKhachHang();
        else if(choice == 0) return ;
    }

}

//===========================================================================================

int main(){
    createMenu();
    return 0;
}
