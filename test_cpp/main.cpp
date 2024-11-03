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
#include "User.cpp"
using ull = unsigned long long;
using namespace std;

//===========================Khai bao ham chuc nang==========================

void createMenu(PGconn *conn){
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
            DangKyofUser(conn);
        }
        else if(choice == 2){
            DangNhapofUser(conn);
            }
        else if(choice == 0) return ;
    }
}

void DangKyofUser(PGconn *conn){
   
   
    while(1){
        cout << "=========DANG KY TAI KHOAN==========\n";
        cout << "Nhap ten dang nhap: " ;
       string name;
       getline(cin, name);
       string password1, password2;
       cout << "Nhap mat khau: ";
       cin >> password1;
       cout << "Nhap lai mat khau: ";
       cin >> password2;
       cin.ignore();
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
        string query;
        query = "SELECT * FROM check_user('" + name + "');";
        PGresult *res = PQexec(conn, query.c_str());
        string uname = PQgetvalue(res, 0, 0);
        PQclear(res);
        if (name == uname) mk = 1;
        if(mk == 1){
           int choice;
           cout << "--------Ten dang nhap da ton tai--------." << endl;
           cout << "1. Thu lai (Retry)" << endl;
           cout << "2. Dang nhap (Login)" << endl;
           cout << "3. Thoat" << endl; 
           cout << "Chon chuc nang: " ;
           cin >> choice;
           cin.ignore(); // Để bỏ qua ký tự '\n' trong buffer

           if (choice == 1) {
               DangKyofUser(conn); // Thử lại đăng ký
           } else if (choice == 2) {
               //chuyển tới hàm Đăng nhập
               DangNhapofUser(conn);
               }
            else if (choice == 3) return ;
            else {
               cout << "Lua chon khong hop le!" << endl;
           }
       }
        else{
            ull en_password1 = getHash(password1);
            string query;
            query = "SELECT * FROM create_user('" + name + "',"+ to_string(en_password1) +");";
            PGresult *exct = PQexec(conn, query.c_str());
            cout << PQgetvalue(exct, 0, 0) << endl;
            PQclear(exct);
            return;
        }
    }

}


void createMenuSaiDangNhap(PGconn *conn){
        
    while(1){
        cout << "---------Chon chuc nang-------------\n";
        cout << "1. Dang nhap lai.\n";
        cout << "2. Quen mat khau.\n";
        cout << "3. Tao tai khoan.\n";
        cout << "0. Thoat!\n";
        cout << "--------------------------------------\n";
        int chk;
        cout << "Chon chuc nang: " ;
        cin >> chk;
        cin.ignore();
        if(chk == 1) DangNhapofUser(conn);
        else if(chk == 2){
            string name;
            cout << "Nhap ten dang nhap: ";
            getline(cin, name);
            
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
                        // lấy wallet_id của người dùng
                        ull en_mk1 = getHash(mk1);
                        string query;
                        query = "SELECT * FROM check_user('" + name + "');";
                        PGresult *exec = PQexec(conn, query.c_str());
                        string uwallet_id = PQgetvalue(exec, 0, 1);
                        PQclear(exec);

                        // cập nhật mật khẩu mới
                        query = "SELECT *FROM update_uinfo("+ uwallet_id + ", "+ to_string(en_mk1) +");";
                        PGresult *res = PQexec(conn, query.c_str());
                        cout<< PQgetvalue(res, 0, 0) << endl;
                        PQclear(res);
                        cout << "Thay doi mat khau thanh cong. Vui long dang nhap lai !\n";
                        return ;
                    }
                }
                
            
            cout << "Tai khoan khong ton tai!\n";
        }
        else if(chk == 3) DangKyofUser(conn);
        else if(chk == 0) return ;
        else {
            cout<< "Lua chon khong hop le. Vui long thu lai !"<< endl;
            continue;
        }
    }
}

void DangNhapofUser(PGconn *conn){
    string query;
    int cnt=0;
	string fail = "";
    while (1) {
        string name, mk;
        cout << "Nhap ten nguoi dung: ";
        getline(cin, name);
        cout << "Nhap mat khau: ";
        cin >> mk;
        cin.ignore();
        ull en_password = getHash(mk);

        // Tạo câu truy vấn với tên người dùng
        query = "SELECT * FROM login('" + name + "');";
        PGresult *res = PQexec(conn, query.c_str());

        // Kiểm tra kết quả truy vấn
        if (PQresultStatus(res) != PGRES_TUPLES_OK) {
            cerr << "Query failed: " << PQerrorMessage(conn) << endl;
            PQclear(res);
            system("pause");
            continue;
        }

        // Lấy giá trị các cột từ dòng đầu tiên
        string uname = PQgetvalue(res, 0, 0);
        string upassword = PQgetvalue(res, 0, 1);
        string uwallet_id = PQgetvalue(res, 0, 2);
        string user_role = PQgetvalue(res, 0, 3);

        // Chuyển đổi `uwallet_id` sang `int`
        int wallet_id = stoi(uwallet_id);
        ull en_mk = stoi(upassword);
        // Kiểm tra tên đăng nhập và mật khẩu
        if (uname == "0" || en_password != en_mk) {
            cout << "Ten dang nhap hoac mat khau khong dung!" << endl;
            cnt++;
        } else {
            if (user_role == "admin") {
                cout << "Chao mung admin "<< uname << "\n";
                ChucNangAdmin(conn);
                }
            else {
                cout << "Chao mung user "<< uname << "\n";
                ChucNangUser(conn, wallet_id, en_password, name, en_mk);
                }
            PQclear(res); // Giải phóng kết quả truy vấn sau khi sử dụng
            break;  // Thoát khỏi vòng lặp sau khi đăng nhập thành công
        }

        // Kiểm tra số lần thử đăng nhập
        if (cnt == 3) {
            cout << "Ban da nhap sai qua so lan quy dinh.\n";
            PQclear(res); // Giải phóng kết quả truy vấn trước khi thoát
            break;
        }

        // Hiển thị lựa chọn tiếp tục
        cout << "1. Thu lai.\n";
        cout << "0. Thoat!\n";
        int choice;
        cout << "Chon chuc nang: ";
        cin >> choice;
        cin.ignore();
        
        if (choice == 0) {
            cout << "Dang nhap khong thanh cong!\n";
            PQclear(res); // Giải phóng kết quả truy vấn trước khi thoát
            break;
        }

        PQclear(res); // Giải phóng bộ nhớ sau mỗi lần dùng `res`
    }
    createMenuSaiDangNhap(conn);
    return;
}
void DangKyofAdmin(PGconn *conn){
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
        if(!createOTP()) return ;
        int mk = 0;
        string query;
        query = "SELECT * FROM check_user('" + name + "');";
        PGresult *exec = PQexec(conn, query.c_str());
        string uname = PQgetvalue(exec, 0, 0);
        if (name == uname) mk = 1;
        if(mk == 1){
           int choice;
           cout << "--------Ten dang nhap da ton tai--------." << endl;
           cout << "1. Thu lai (Retry)" << endl;
           cout << "2. Dang nhap (Login)" << endl;
           cout << "3. Thoat" << endl; 
           cout << "Chon chuc nang: " ;
           cin >> choice;
           cin.ignore(); // Để bỏ qua ký tự '\n' trong buffer

            if (choice == 1) {
                DangKyofAdmin(conn); // Thá»­ láº¡i Ä‘Äƒng kÃ½
            } 
            else {
                cout << "Tao tai khoan khong thanh cong!\n";
            }
        }
        else {
            string query;
            ull rest = getHash("1");
            query = "SELECT * FROM create_user('" + name + "', "+ to_string(rest) + ");";
            PGresult *exct = PQexec(conn, query.c_str());
            cout << PQgetvalue(exct, 0, 0) << ". Mat khau sinh tu dong cua ban la: 1\n";
            PQclear(exct);
            return;
        }
    }
}

void ChucNangUser(PGconn *conn, int wallet_id, ull en_password, string name, ull en_mk){
    if (en_password == 49) {
        string new_password1, new_password2;
        ull en_newpassword1, en_newpassword2;
        while (true) {
            cout << "Nhap mat khau moi: ";
            cin >> new_password1;
            cout << "Nhap lai mat khau: ";
            cin >> new_password2;
            cin.ignore();
            en_newpassword1 = getHash(new_password1);
            en_newpassword2 = getHash(new_password2);

            // Kiểm tra nếu mật khẩu mới trùng với mật khẩu cũ
            if (en_newpassword1 == 49) {  // Giả sử "1" là mật khẩu cũ
                cout << "Mat khau moi khong duoc trung voi mat khau cu." << endl;
            } 
            // Kiểm tra nếu mật khẩu nhập lại không khớp
            else if (new_password1 != new_password2) {
                cout << "Mat khau khong khop. Vui long thu lai!" << endl;
            } 
            // Nếu tất cả các điều kiện đều thỏa mãn
            else {
                cout << "Mat khau da duoc dat thanh cong!" << endl;
                break;  // Thoát vòng lặp nếu mật khẩu hợp lệ
            }

            // Yêu cầu người dùng lựa chọn nếu mật khẩu không hợp lệ
            cout << "1. Thu lai.\n";
            cout << "0. Thoat!\n";
            int chk;
            cin >> chk;
            cin.ignore();
            if (chk == 0) {
                cout << "Dang ky khong thanh cong!" << endl;
                return;
            }
}
        cout<< en_newpassword1 << endl;
        string query;
        query = "SELECT *FROM update_uinfo("+ to_string(wallet_id) + ", "+ to_string(en_newpassword1) +");";
        PGresult *res = PQexec(conn, query.c_str());
        cout<< PQgetvalue(res, 0, 0) << endl;
        PQclear(res);

       }
    while(1){
        cout << "=============USER=============\n";
        cout << "Cac chuc nang:\n";
        cout << "1. Xem thong tin.\n";
        //cout << "2. Thay doi thong tin.\n";
        cout << "3. Sua mat khau.\n";
        cout << "4. Quan ly vi diem thuong.\n";
        cout << "0. Thoat!\n";
        int choice;
        cout << "Chon chuc nang: ";
        cin >> choice;
        cin.ignore();
        if(choice == 1 ) xemThongTin(conn, wallet_id);
        else if(choice == 2 ) suaThongTinofUser(conn, name);
        else if(choice == 3) suaMatKhauofUser(conn, wallet_id, en_mk);
        else if(choice == 4) quanlyVi(conn, wallet_id);
        else if(choice == 0) return;
        else cout << "Lua chon khong hop le. Vui long thu lai!" << endl;
    }
}

void quanlyVi(PGconn *conn, int wallet_id){
    TotalWallet total;

    while(1){
        cout << "==========Quan ly vi==========\n";
        cout << "1. Xem so du.\n";
        cout << "2. Chuyen diem.\n";
        cout << "3. Xem lich su giao dich.\n";
        cout << "0. Thoat!\n";
        cout << "Chon chuc nang: ";
        int choice;
        cin >> choice ;
        if(choice == 1) total.Check_Balance(conn, wallet_id);
        else if(choice == 2) total.Chuyen_Diem(conn, wallet_id);
        else if(choice == 3) total.Check_history(conn, wallet_id);
        else return;
    }
}

void ChucNangAdmin(PGconn *conn){
	while(1){
        cout << "================ADMIN==============\n";
        cout << "Cac chuc nang: \n";
        cout << "1. Tao tai khoan.\n";
        //cout << "2. Thay doi thong tin.\n";
        cout << "3. Quan ly khach hang.\n";
        cout << "0. Thoat!\n";
        cout << "Chon chuc nang: ";
        int choice ;
        cin >> choice;
        cin.ignore();
        if(choice == 1) DangKyofAdmin(conn);
        //else if(choice == 2) suaThongTinofAdmin();
        else if(choice == 3) quanlyKhachHang(conn);
        else if(choice == 0) return ; 
    }
}

//===========================================================================================

int main(){
    // Chuỗi kết nối tới PostgreSQL
    const char* conninfo = "dbname=**** user=postgres password=***** hostaddr=****** port=5432";
    
    // Kết nối với PostgreSQL
    PGconn *conn = PQconnectdb(conninfo);

    // Kiểm tra kết nối có thành công hay không
    if (PQstatus(conn) != CONNECTION_OK) {
        cerr << "Connection to database failed: " << PQerrorMessage(conn) << endl;
        PQfinish(conn);
        return 0;
    }

    createMenu(conn);

    PQfinish(conn);
    cout<<"ngat.\n";
    system("pause");
    return 0;
}
