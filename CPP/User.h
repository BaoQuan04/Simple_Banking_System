#ifndef USER_H
#define USER_H

#include<iostream>
#include<string>
using namespace std;

//Khoi tao chinh sua cac thong tin cua user
class User;




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
		//Contructor khoi tao cac gia tri la mot xau rong
		User();


		//Ham tao ma OTP
		bool CreateOTP();
		
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
		
		
		
		//Cac ham chuan hoa thong tin cua user
		string ChuanHoa_Ten(string);
		string ChuanHoa_NgaySinh(string);
		string ChuanHoa_GioiTinh(string);
		string ChuanHoa_DiaChi(string);



		//Ham kiem tra input thong tin user 
		bool Check_NgaySinh(string&);
		bool Check_GioiTinh(string&);
		bool Check_Cccd(string&);
		bool Check_Sdt(string&);
		bool Check_Email(string&);


		//Nhap suat sua thong tin user
		void NhapThongTin();
		void XuatThongTin();
		void SuaThongTin();
		void SuaTen();
		void SuaNgaySinh();
		void SuaGioiTinh();
		void SuaCccd();
		void SuaSdt();
		void SuaDiaChi();
		void SuaEmail();

		
};


#endif
