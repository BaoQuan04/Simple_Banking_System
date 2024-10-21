#ifndef SIGNUP_LOGIN_H
#define SIGNUP_LOGIN_H

#include<iostream>
#include<string>
#include<algorithm>
#include<unordered_map>

using namespace std;

//--------------------------------------------



class Sign_Log{
    private :

        string username;
        string password;
        unordered_map<string, string> account;

    public :

        //Tao ra ma OTP
        bool CreateOTP();
        //----------------XU LY DANG KY TAI KHOAN CHO USER------------

        bool Check_HopLe();// Hàm kiểm tra tính hợp lệ của tên và mật khẩu 
        bool Check_TonTai(); // Hàm kiểm tra xem tên đăng nhập có tồn tại không
        bool Check_MatKhau(); // Hàm cho người dùng nhập và xác nhận mật khẩu
        bool NhapThongTin();// Hàm để người dùng nhập thông tin, bool : neu nguoi dung nhap sai mk, co the nhap lai hoac thoat chuong trinh

        void SuaMatKhau();
        void LuuTaiKhoan();
        void XuLyTK();// Hàm cho người dùng chọn retry hoặc login
        void DangKy(); // Hàm đăng ký tài khoản

        //----------------XU LY DANG NHAP CHO USER----------------

        bool Check_TaiKhoan( string username, string password );// Hàm kiểm tra tài khoản
        bool Check_Username( string username);// Hàm kiểm tra xem username có tồn tại
        void DangNhap();// Hàm xử lý đăng nhập


        void InThongTin();

        // Ham menu
        void Create_Menu();
};




#endif 
