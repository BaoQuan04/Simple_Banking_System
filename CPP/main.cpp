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
        cout << "3. Thoat!\n";
        cout << "Chon chuc nang: ";
        int choice;
        cin >> choice;
        cin.ignore();
        cout << endl;
        if(choice == 1){
            cout << endl;
            DangKyofUser(conn);
        }
        else if(choice == 2){
            cout << endl;
            DangNhapofUser(conn);
            }
        else if(choice == 3) return ;
        else {
            cout<< "Lua chon khong hop le. Vui long thu lai !\n";
            cout << endl;
        }
    }
}

void DangKyofUser(PGconn *conn){
   
   string sdt;
    while(1){
        cout << "=========DANG KY TAI KHOAN==========\n";
        while (1)
        {
        cout << "Nhap sdt: " ;
        getline(cin, sdt);
        if(User().Check_Sdt(sdt)) break;
        else {
        while(1) {   
                cout << "So dien thoat khong hop le. Vui long thu lai !\n";
                cout << "1. Thu lai.\n";
                cout << "0. Thoat!\n";
                int chk ;
                cin >> chk;
                cin.ignore();
                if(chk == 0){
                    cout << "Dang ky khong thanh cong!\n";
                    cout << endl;
                    return;
                }
                else if (chk == 1) break;
                else {
                    cout << "Lua chon khong hop le !\n";
                    cout << endl;
                    continue;}
            }
       }
       }
       
       string password1, password2;
       cout << "Nhap mat khau: ";
       cin >> password1;
       cout << "Nhap lai mat khau: ";
       cin >> password2;
       cin.ignore();
       if (password1 != password2) {
           while(1) {
                cout << "Mat khau khong khop. Vui long thu lai !" << endl;   
                cout << "1. Thu lai.\n";
                cout << "0. Thoat!\n";
                int chk ;
                cout << "Chon chuc nang: ";
                cin >> chk;
                cin.ignore();
                cout << endl;
                if(chk == 0){
                    cout << "Dang ky khong thanh cong!\n";
                    cout << endl;
                    return;
                }
                else if (chk == 1) break;
                else {
                    cout << "Lua chon khong hop le !\n";
                    cout << endl;
                    continue;}
            }
       }
       if (sdt == "" || password1 == ""){
            while(1) {
                cout << "Sdt hoac mat khau khong duoc de trong." << endl;   
                cout << "1. Thu lai.\n";
                cout << "0. Thoat!\n";
                int chk ;
                cout << "Chon chuc nang: ";
                cin >> chk;
                cin.ignore();
                cout << endl;
                if(chk == 0){
                    cout << "Dang ky khong thanh cong!\n";
                    cout << endl;
                    return;
                }
                else if (chk == 1) break;
                else {
                    cout << "Lua chon khong hop le !\n";
                    cout << endl;
                    continue;}
            }
       }
        int mk = 0;
        string query;
        query = "SELECT * FROM check_user('" + sdt + "');";
        PGresult *res = PQexec(conn, query.c_str());
        string utelephone = PQgetvalue(res, 0, 0);
        PQclear(res);
        if (sdt == utelephone) mk = 1;
        if(mk == 1){
            int choice;
            cout << "--------Sdt da ton tai--------." << endl;
            cout << "1. Thu lai (Retry)" << endl;
            cout << "2. Dang nhap (Login)" << endl;
            cout << "3. Thoat" << endl; 
            cout << "Chon chuc nang: " ;
            cin >> choice;
            cin.ignore(); // Để bỏ qua ký tự '\n' trong buffer
            cout << endl;
            if (choice == 1) {
               DangKyofUser(conn); // Thử lại đăng ký
           } else if (choice == 2) {
               //chuyển tới hàm Đăng nhập
               DangNhapofUser(conn);
               }
            else if (choice == 3) return ;
            else {
               cout << "Lua chon khong hop le!" << endl;
               cout << endl;
           }
       }
        else{
            ull en_password1 = getHash(password1);
            string query;
            query = "SELECT * FROM create_user('" + sdt + "',"+ to_string(en_password1) +");";
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
        cout << "4. Thoat!\n";
        cout << "--------------------------------------\n";
        int chk;
        cout << "Chon chuc nang: " ;
        cin >> chk;
        cin.ignore();
        cout << endl;
        if(chk == 1) DangNhapofUser(conn);
        else if(chk == 2){
            string sdt, query;
            int wallet_id;
            while (1) {
                cout << "Nhap sdt nguoi dung: ";
                getline(cin, sdt);

                query = "SELECT * FROM login('" + sdt + "');";
                PGresult *res = PQexec(conn, query.c_str());


        // Kiểm tra kết quả truy vấn
                if (PQresultStatus(res) != PGRES_TUPLES_OK) {
                    cerr << "Query failed: " << PQerrorMessage(conn) << endl;
                    PQclear(res);
                    system("pause");
                    continue;
                }
                string utelephone = PQgetvalue(res, 0, 0);
                string uwallet_id = PQgetvalue(res, 0, 3);
                wallet_id = stoi(uwallet_id);
                PQclear(res); // Giải phóng kết quả truy vấn sau khi sử dụng

                if (utelephone == "0") {
                    cout << "So dien thoai khong ton tai" << endl;
                    cout << "1. Thu lai.\n";
                    cout << "2. Quay lai.\n";
                    int choice;
                    cin.ignore();
                    if (choice == 2) return;
                    else continue;
                }
                else break;
            }
            User().suaMatKhau(conn, wallet_id);
            return;
        }
        else if(chk == 3) DangKyofUser(conn);
        else if(chk == 4) {
            PQfinish(conn);
            cout<<"Chuc ban 1 ngay tot lanh.\n";
            system("pause");
            exit(1);
        }
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
        cout << "=========DANG NHAP TAI KHOAN==========\n";
        string sdt, mk;
        cout << "Nhap Sdt: ";
        getline(cin, sdt);
        cout << "Nhap mat khau: ";
        cin >> mk;
        cin.ignore();
        ull en_password = getHash(mk);

        // Tạo câu truy vấn với tên người dùng
        query = "SELECT * FROM login('" + sdt + "');";
        PGresult *res = PQexec(conn, query.c_str());

        // Kiểm tra kết quả truy vấn
        if (PQresultStatus(res) != PGRES_TUPLES_OK) {
            cerr << "Query failed: " << PQerrorMessage(conn) << endl;
            PQclear(res);
            system("pause");
            continue;
        }

        // Lấy giá trị các cột từ dòng đầu tiên
        string utelephone = PQgetvalue(res, 0, 0);
        string uname = PQgetvalue(res, 0, 1);
        string upassword = PQgetvalue(res, 0, 2);
        string uwallet_id = PQgetvalue(res, 0, 3);
        string user_role = PQgetvalue(res, 0, 4);

        // Chuyển đổi `uwallet_id` sang `int`
        int wallet_id = stoi(uwallet_id);
        ull en_mk = stoi(upassword);
        // Kiểm tra tên đăng nhập và mật khẩu
        if (utelephone == "0" || en_password != en_mk) {
            cout << "Ten dang nhap hoac mat khau khong dung!" << endl;
            cnt++;
        } else {
            if (user_role == "admin") {
                cout << "Xin chao "<< utelephone << "\n";
                cout << endl;
                ChucNangAdmin(conn);
                }
            else {
                cout << "Xin chao "<< uname << "\n";
                cout << endl;
                ChucNangUser(conn, wallet_id, en_password, utelephone, en_mk);
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
        cout << endl;
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
        string sdt;
        while(1) {   
                cout << "Nhap sdt nguoi dung: ";
                getline(cin, sdt);
                if(User().Check_Sdt(sdt)) break;
                else {
                    cout << "So dien thoat khong hop le. Vui long thu lai !\n";
                    cout << "1. Thu lai.\n";
                    cout << "0. Thoat!\n";
                    int chk ;
                    cin >> chk;
                    cin.ignore();
                    if(chk == 0){
                        cout << "Dang ky khong thanh cong!\n";
                        return;
                    }
                    continue;
            }
        }
        int mk = 0;
        string query;
        query = "SELECT * FROM check_user('" + sdt + "');";
        PGresult *exec = PQexec(conn, query.c_str());
        string utelephone = PQgetvalue(exec, 0, 0);
        if (sdt == utelephone) mk = 1;
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
            if(!createOTP()) return ;
            string query;
            ull rest = getHash("1");
            query = "SELECT * FROM create_user('" + sdt + "', "+ to_string(rest) + ");";
            PGresult *exct = PQexec(conn, query.c_str());
            cout << PQgetvalue(exct, 0, 0) << ". Mat khau sinh tu dong cua ban la: 1\n";
            PQclear(exct);
            return;
        }
    }
}

void ChucNangUser(PGconn *conn, int wallet_id, ull en_password, string utelephone, ull en_mk){
    string new_password1, new_password2, uname, en_uname;
    ull en_newpassword1, en_newpassword2;
    
    if (en_password == 49) {
        User().capnhapThongTin(conn, wallet_id);
        }

    while(1){
        cout << "=============USER=============\n";
        cout << "Cac chuc nang:\n";
        cout << "1. Xem thong tin.\n";
        cout << "2. Thay doi thong tin.\n";
        cout << "3. Quan ly vi diem thuong.\n";
        cout << "4. Quay lai.\n";
        cout << "5. Thoat!\n";
        int choice;
        cout << "Chon chuc nang: ";
        cin >> choice;
        cin.ignore();
        cout << endl;
        if(choice == 1 ) xemThongTin(conn, wallet_id);
        else if(choice == 2 ) User().menuSuaThongTin(conn, wallet_id);
        else if(choice == 3) quanlyVi(conn, wallet_id);
        else if(choice == 4) return;
        else if (choice == 5)
        {
            PQfinish(conn);
            cout<<"Chuc ban 1 ngay tot lanh.\n";
            system("pause");
            exit(1);
        }
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
        cout << "4. Quay lai.\n";
        cout << "5. Thoat!\n";
        cout << "Chon chuc nang: ";
        int choice;
        cin >> choice ;
        cout << endl;
        if(choice == 1) total.Check_Balance(conn, wallet_id);
        else if(choice == 2) total.Chuyen_Diem(conn, wallet_id);
        else if(choice == 3) total.Check_history(conn, wallet_id);
        else if(choice == 4) return;
        else if (choice == 5)
        {
            PQfinish(conn);
            cout<<"Chuc ban 1 ngay tot lanh.\n";
            system("pause");
            exit(1);
        }
        else {
            cout << "Lua chon khong hop le. Vui long thu lai !\n";
        }
        cout << endl;
    }
}

void ChucNangAdmin(PGconn *conn){
	while(1){
        cout << "================ADMIN==============\n";
        cout << "Cac chuc nang: \n";
        cout << "1. Tao tai khoan.\n";
        cout << "2. Thay doi thong tin khach hang.\n";
        cout << "3. Quan ly khach hang.\n";
        cout << "4. Quay lai.\n";
        cout << "5. Thoat!\n";
        cout << "Chon chuc nang: ";
        int choice ;
        cin >> choice;
        cin.ignore();
        cout << endl;
        if(choice == 1) DangKyofAdmin(conn);
        else if (choice == 2) suaThongTinofAdmin(conn);
        else if(choice == 3) quanlyKhachHang(conn);
        else if(choice == 4) return ;
        else if (choice == 5)
        {
            PQfinish(conn);
            cout<<"Chuc ban 1 ngay tot lanh.\n";
            system("pause");
            exit(1);
        }
        
        else {
            cout << "Lua chon khong hop le. Vui long thu lai !\n";
        }
        cout << endl;
    }
}

//===========================================================================================

int main(){
    // Chuỗi kết nối tới PostgreSQL
    const char* conninfo = "dbname=******* user=postgres password=***** hostaddr=****** port=5432";
    
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
    cout<<"Chuc ban 1 ngay tot lanh.\n";
    system("pause");
    return 0;
}
