#ifndef USER_H
#define USER_H

#include<iostream>
#include<string>
using namespace std;


//Luu cac thanh phan cua User
class User{
//-----------------Cac thong tin cua user-----------------------------
	
	private:
		string MaKhachHang;
		string Ten;
		string NgaySinh;
		string GioiTinh;
		string Cccd;
		string Sdt;
		string DiaChi;
		string Email;
		
		
//-----------------Cac ham xu li thong tin cua user----------------------
	public:
		
		//Ham lay ra cac thong tin cua user
		string getMaKhachHang();
		string getTen();
		string getNgaySinh();
		string getGioiTinh();
		string getCccd();
		string getSdt();
		string getDiaChi();
		string getEmail();
		
		//Ham dieu chinh cac thong tin cua khach hang
		void setTen(string);
		void setNgaySinh(string);
		void setGioiTinh(string);
		void setCccd(string);
		void setSdt(string);
		void setDiaChi(string);
		void setEmail(string);
		
		//Nhap suat sua thong tin user
		void NhapThongTin();
		void XuatThongTin();
		void SuaThongTin();
		
		
		
		
};


#endif 
