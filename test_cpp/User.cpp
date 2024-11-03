#include<iostream>
#include<string>
#include <algorithm>
#include <sstream>
#include <regex>
#include<cstring>
#include<stdlib.h>
#include<ctime>
#include<map>
#include<unordered_map>
#include <iomanip>
using namespace std;
using ull = unsigned long long;

//========Khai bao cac class=====
class User;
class TransactionLog;
class TotalWallet;

//========Khai bao cac container luu tru========
vector<User> account;
map<string, string> admin;

//===========Khai bao cac ham chuc nang============
bool createOTP();
void createMenu();
ull getHash(string);
//============
void DangKyofUser(PGconn *conn);
void DangNhapofUser(PGconn *conn);
void createMenuSaiDangNhap(PGconn *conn);
//=================
void ChucNangUser(PGconn *conn, int wallet_id, ull en_password, string name, ull en_mk);
void quanlyVi(PGconn *conn, int wallet_id);
void suaThongTinofUser(PGconn *conn, string name);
void suaMatKhauofUser(PGconn *conn, int wallet_id, ull en_mk);
void xemThongTin(PGconn *conn, int wallet_id);
//====================
void DangKyofAdmin(PGconn *conn);
void quanlyKhachHang(PGconn *conn);
void suaThongTinofAdmin();
void suaMatKhauofAdmin();
void ChucNangAdmin(PGconn *conn);


void xemdanhsach();
//===========Khai bao=============================================
class User{
    private :
    //Cac thong tin cua vi:
        string maDinhDanh;
        int balance;

    //Thon tin dang nhap cua khach hang
        string username;
        ull password;

    //Cac thong tin cua khach hang
        string ID;
        static int num;
		string Ten;
		string NgaySinh;
		string GioiTinh;
		string Cccd;
		string Sdt;
		string DiaChi;
		string Email;





    public :

        //bool createOTP();

        User();
        User(string, string, ull);
            
        //----------------XU LY DANG KY TAI KHOAN CHO USER------------
        string createID();
        //----------------XU LY DANG NHAP CHO USER----------------
		string getTen();
		string getUsername();
		ull getPassword();
        void setPassword(ull);
        //----------------Cac ham chuan hoa-------------------
        string ChuanHoa_Ten(string);
		string ChuanHoa_NgaySinh(string);
		string ChuanHoa_GioiTinh(string);
		string ChuanHoa_DiaChi(string);

		

		//---------------Ham kiem tra input thong tin user -------------------
		bool Check_GioiTinh(string&);
		bool Check_NgaySinh(string&);
		bool Check_Cccd(string&);
		bool Check_Sdt(string&);
		bool Check_Email(string&);


		//---------------------Nhap suat sua thong tin user------------------
		void capnhapThongTin();
		void xuatThongTin();
        void menuSuaThongTin();
		void SuaTen();
		void SuaNgaySinh();
		void SuaGioiTinh();
		void SuaCccd();
		void SuaSdt();
		void SuaDiaChi();
		void SuaEmail();
        void suaMatKhau();
        void inDanhSach();

        //----------------------Quan ly vi diem thuong----------------

        User(string, int);

        string getmaDinhDanh();

        int getBalance();

        void setBalance(int);

        void setmaDinhDanh(string);

        void xemallthongtin();

};


class TransactionLog {
    public:
        string transactionid;
        string fromwallet;
        string towallet;
        int point;
        string status;
        int balance2;

        TransactionLog(string magd, string vinguon, string vidich, int diem,string trangthai, int sodu2) {
            this->transactionid = magd;
            this->fromwallet = vinguon;
            this->towallet = vidich;
            this->point = diem;
            this->status =trangthai;
            this->balance2 = sodu2;
        }
};

class TotalWallet{
    private:
        static int Total;
        unordered_map<string, User> list; //Tu dong them vao voi key "id"
        vector<TransactionLog> history;
    public:

    //Them 5 point vao tai khoan moi khoi tao tu vi tong
        void AddPoint(User&);

    //Them wallet moi tao vao list 
        void addwallet(User );
    
    //Kiem tra xem vi co ton tai hay khong (bang cach check id wallet) 
        bool Check_Wallet(User);

    //ham chuyen diem
        bool Chuyen_Diem(PGconn *conn,int wallet_id);

        void Check_Balance(PGconn *conn, int wallet_id);
    //Lich su giao dich cua vi ca nhan
        void Check_history(PGconn *conn,int wallet_id);
};



//===========================HAM THUOC CLASS==============================
int User::num = 0;
int TotalWallet :: Total = 5000;

User::User()  : maDinhDanh(""), balance(0), ID(""), username(""), password(), Ten(""), NgaySinh(""), GioiTinh(""), Cccd(""), Sdt(""), DiaChi("") {}
User::User(string a, string  b, ull c) : maDinhDanh(""), balance(0), ID(a), username(b), password(c), Ten(""), NgaySinh(""), GioiTinh(""), Cccd(""), Sdt(""), DiaChi("") {}
User::User(string ma, int sodu) : maDinhDanh(ma), balance(sodu) {}

string User::getTen(){
    return this->Ten;
}

string User::getUsername(){
    return this->username;
}

ull User::getPassword(){
    return this->password;
}

string User::getmaDinhDanh(){
    return this->maDinhDanh;
}

int User::getBalance(){
    return this->balance;
}

void User::setPassword(ull a){
    this->password = a;
}

void User::setBalance(int sodu){
    this->balance = sodu;
}

void User::setmaDinhDanh(string mk){
	this->maDinhDanh = mk ;
}

string User::createID(){
    num++;
    return string(3- to_string(num).length(), '0') + to_string(num);
}

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

void TotalWallet::AddPoint(User& a){
        a.setBalance(5);
        Total -= 5;
}

void TotalWallet::addwallet(User a){
        cout << "Vui long nhap MA DINH DANH de khoi tao vi : ";
        string Id; cin >> Id;
        a.setmaDinhDanh(Id);
        list[a.getmaDinhDanh()] = User(a.getmaDinhDanh(),a.getBalance());
        cout << "Khoi tao vi thanh cong!\nMA DINH DANH cua ban la: " << Id << endl;
}

bool TotalWallet::Check_Wallet(User a){
    if (list.find(a.getmaDinhDanh()) != list.end()) return true;
    else return false ;
}

bool TotalWallet::Chuyen_Diem(PGconn *conn,int wallet_id){
    string v1, v2, query;
    int d;

    cout << "Nhap ten nguoi dung ma ban muon chuyen tien : ";
    cin >> v2;
    query = "SELECT * FROM check_user('" + v2 + "');";
    PGresult *res = PQexec(conn, query.c_str());
    string to_user_name = PQgetvalue(res, 0, 0);
    string uwallet_id = PQgetvalue(res, 0, 1);
    int to_wallet_id = stoi(uwallet_id);
    if (PQresultStatus(res) != PGRES_TUPLES_OK) {
        cerr << "Query failed: " << PQerrorMessage(conn) << endl;
        PQclear(res);
        PQfinish(conn);
        system("pause");
        return false;
    }
    PQclear(res);

    if (to_user_name == "0") {
        cout << "Ten nguoi chuyen den khong ton tai !" << endl;
        return false;
    };
    
    if (to_wallet_id == wallet_id) {
        cout << "Vi chu va vi dich phai khac nhau !" << endl;
        return false;
    };
    cout << "Nhap so tien ban muon chuyen cho vi dich : "; 
    cin >> d;
    if(!createOTP()) return false;
    query = "SELECT complex_transaction("+ to_string(wallet_id) +","+ uwallet_id +","+ to_string(d) +");";
    PGresult *exac = PQexec(conn, query.c_str());
    cout << PQgetvalue(exac, 0, 0) << endl;
    PQclear(exac);
   return true;
}

void TotalWallet::Check_Balance(PGconn *conn,int wallet_id){
    string query;
    query = "SELECT balance FROM wallet WHERE wallet_id = " + to_string(wallet_id) + ";";
    PGresult *res = PQexec(conn, query.c_str());
    cout << "--------------------------------------" << endl;
    cout << "Ma Vi: " << wallet_id << endl;
    cout << "So du kha dung : " << PQgetvalue(res, 0, 0) << endl;
    cout << "--------------------------------------" << endl;
    PQclear(res);
}

void TotalWallet::Check_history(PGconn *conn,int wallet_id){
    string query;
    query = "SELECT * FROM transfer_log(" + to_string(wallet_id) + ", 0);";
    PGresult *res = PQexec(conn, query.c_str());

    int rows = PQntuples(res);
    int cols = PQnfields(res);

    // Đặt độ rộng cho 4 cột đầu tiên
    int column_widths[] = {25, 25, 15, 23, 23, 15};  // Độ rộng cho từng cột, bao gồm "Trang thai giao dich"
 
    // In tiêu đề của các cột với độ rộng tương ứng
    for (int i = 0; i < cols; i++) {  // Duyệt qua tất cả các cột
        cout << left << setw(column_widths[i]) << PQfname(res, i);
    }
    cout << endl;

    // In dòng phân cách
    for (int i = 0; i < cols; i++) {
        cout << string(column_widths[i], '-');
    }
    cout << endl;

    // In dữ liệu của từng hàng
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            cout << left << setw(column_widths[j]) << PQgetvalue(res, i, j);
        }
        cout << endl;
    }
    PQclear(res);
    return;
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

void User::capnhapThongTin(){
    cout << "CAP NHAP THONG TIN USER !" << endl;
   
//----------Nhap Ten-------------
    string name;
    cout << "Nhap Ho va Ten (Vi du: Nguyen Van A) :" ;
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
    
    int check = createOTP();
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

void User::xuatThongTin(){
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

void User::menuSuaThongTin(){
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
        cin.ignore();
        if(a == 1){
            capnhapThongTin();
            cout << "Thay doi thong tin thanh cong.\n";
        }
        else if(a == 2) SuaTen();
        else if(a == 3) SuaNgaySinh();
        else if(a == 4) SuaGioiTinh();
        else if(a == 5) SuaCccd();
        else if(a == 6) SuaSdt();
        else if(a == 7) SuaDiaChi();
        else if(a == 8) SuaEmail();
        else if(a == 9) xuatThongTin();
        else return;
    }
}

void User::SuaTen(){
    string a;
    cout << "Nhap Ho va Ten (Vi du: Nguyen Van A) :" ;
    getline(cin, a);
    a = ChuanHoa_Ten(a);
    int check =createOTP();
    if(check == true){
        this->Ten = a;
        cout << "Thay doi thong tin thanh cong.\n";
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
    int check =createOTP();
    if(check == true){
        this->NgaySinh = a;
        cout << "Thay doi thong tin thanh cong.\n";
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
    int check =createOTP();
    if(check == true){
        this->GioiTinh = a;
        cout << "Thay doi thong tin thanh cong.\n";
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
    int check =createOTP();
    if(check == true){
        this->Cccd = a;
        cout << "Thay doi thong tin thanh cong.\n";
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
    int check =createOTP();
    if(check == true){
        this->Sdt = a;
        cout << "Thay doi thong tin thanh cong.\n";
    }
    else return;
}

void User::SuaDiaChi(){
    string a;
    cout << "Nhap dia chi (Vi du: Dong Thap) :" ;
    cin.ignore();
    getline(cin , a);
    a = ChuanHoa_DiaChi(a);
    int check =createOTP();
    if(check == true){
        this->DiaChi = a;
        cout << "Thay doi thong tin thanh cong.\n";
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
    int check =createOTP();
    if(check == true){
        this->Email = a;
        cout << "Thay doi thong tin thanh cong.\n";
    }
    else return;
}

void User::suaMatKhau(){
    while(1){
        cout << "Nhap mat khau cu: ";
        string lastpass;
        cin >> lastpass;
        ull tmp = getHash(lastpass);
        if(this->password != tmp){
            cout << "Mat khau khong chinh xac! Vui long thu lai\n";
            continue;
        }
        else{
            while(1){
                string mk1, mk2;
                cout << "Nhap mat khau moi: ";
                cin >> mk1;
                cout << "Nhap lai mat khau: ";
                cin >> mk2;
                if(mk1 != mk2 ){
                    cout << "Mat khau khong khop! Vui long thu lai.\n";
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
                int check =createOTP();
                if(check == true){
                    ull res = getHash(mk1);
                    this->password = res;
                    cout << "Thay doi mat khau thanh cong!\n";
                    return;
                }
                else return;
            }
        }
    }
}

void User::inDanhSach(){
    cout << this->ID << " " << this->Ten << " " << this->NgaySinh << endl;
}

void User::xemallthongtin(){
    cout << this->maDinhDanh << " " << this->balance << " " << this->ID << " " << this->username << " " << this->password << " " << this->Ten << " " << this->NgaySinh<<" " << this->GioiTinh << " " << this->Cccd << " " << this->Sdt << " " << this->DiaChi <<" " << this->Email << endl;
}

//+=================================HAM RIENG==============================

bool createOTP(){
    //Khoi tao seed cho rand() de sinh so ngau nhien thuc su
    const int nums = 7;
    srand(time(0)); //Su dung thoi gian hien tai lam seed
    int cnt =3;
    while(cnt--){
        char code[nums], ch ,otp[nums];
        int n = 0;
        while(n < nums -1){
            int res = 48 + (int) (rand() * (57 - 48 + 1.0) / (1.0 + RAND_MAX));
            ch = (char) res;
            if( (ch >= '0' && ch <= '9') ){
                code[n] = ch;
                n++;
            }
        }  
        code[nums -1] = '\0' ;
        cout << "Ma OTP da duoc gui den tai khoan cua ban! Nhap ma OTP de xac nhan.\n";    
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

ull getHash(string mk){
    ull Value = 0;
    for(int i = 0; i < mk.size(); i++){
        Value = Value * 6 + (ull) mk[i];
    }
    return Value;
}

void xemThongTin(PGconn *conn, int wallet_id){
    string query;
    query = "SELECT user_id, uname FROM users WHERE wallet_id = "+ to_string(wallet_id) + ";";
    PGresult *res = PQexec(conn, query.c_str());

    // Lấy số dòng và số cột trong kết quả
    int rows = PQntuples(res);
    int cols = PQnfields(res);

    // In tên các cột
    cout<<"user_id" << "\t";
    cout<<"Ten nguoi dung" << "\t";
    cout << endl;

    // Duyệt qua từng dòng và cột để in dữ liệu
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            cout << PQgetvalue(res, i, j) << "\t";  // In giá trị tại dòng i, cột j
        }
        cout << endl;
    }

    // Giải phóng kết quả truy vấn và đóng kết nối
    PQclear(res);

}

void suaThongTinofUser(PGconn *connn, string name){
    for(int i = 0 ; i<account.size(); i++){
        /* if(account[i].getUsername() == a){
            account[i].menuSuaThongTin();
        } */
    }
}



void xemdanhsach(){
    cout << "======ACCOUMT=====\n";
    for(auto x : account){
        x.xemallthongtin();      
    }
    cout << "==================\n";
}

void suaMatKhauofUser(PGconn *conn, int wallet_id, ull en_mk){
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
            if (new_password1 != new_password2) {
                cout << "Mat khau khong khop. Vui long thu lai!" << endl;
                cout << "1. Thu lai.\n";
                cout << "0. Thoat!\n";
                int chk ;
                cin >> chk;
                cin.ignore();
                if(chk == 0){
                    cout << "Cap nhat khong thanh cong!\n";
                    return;
                    }
                else continue;
            }
            if (new_password1 == ""){
            cout << "Mat khau khong duoc de trong!" << endl;   
            cout << "1. Thu lai.\n";
            cout << "0. Thoat!\n";
            int chk ;
            cin >> chk;
            cin.ignore();
            if(chk == 0){
                cout << "Cap nhat khong thanh cong!\n";
                return;
                }
            else continue;
            }
            if (en_newpassword1 == en_mk){
            cout << "Mat khau moi khong duoc trung voi mat khau cu!" << endl;   
            cout << "1. Thu lai.\n";
            cout << "0. Thoat!\n";
            int chk ;
            cin >> chk;
            cin.ignore();
            if(chk == 0){
                cout << "Cap nhat khong thanh cong!\n";
                return;
                }
            else continue;
            }
            if(!createOTP()) return;
            string query;
            query = "SELECT *FROM update_uinfo("+ to_string(wallet_id) + ", "+ to_string(en_newpassword1) +");";
            PGresult *res = PQexec(conn, query.c_str());
            cout<< PQgetvalue(res, 0, 0) << endl;
            PQclear(res);
            return;
    }
}
void quanlyKhachHang(PGconn *conn){
    cout << "Danh sach khach hang:\n";
    string query;
    query = "SELECT user_id, uname FROM users;";
    PGresult *res = PQexec(conn, query.c_str());

    // Lấy số dòng và số cột trong kết quả
    int rows = PQntuples(res);
    int cols = PQnfields(res);

    // In tên các cột
    cout<<"user_id" << "\t";
    cout<<"Ten nguoi dung" << "\t";
    cout << endl;

    // Duyệt qua từng dòng và cột để in dữ liệu
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            cout << PQgetvalue(res, i, j) << "\t";  // In giá trị tại dòng i, cột j
        }
        cout << endl;
    }

    // Giải phóng kết quả truy vấn và đóng kết nối
    PQclear(res);
    cout << "=====================\n";
}

/* void suaThongTinofAdmin(){
    int cnt=0;
    while(1){
        string name, mk;
        cout << "Dang nhap tai khoan khach hang nguoi dung:\n";
        cout << "Nhap ten nguoi dung:" ;
        getline(cin, name);
        cout << "Nhap mat khau: ";
        cin >> mk;
        ull res = getHash(mk);
        cin.ignore();
        for(int i=0;i<account.size();i++){
            if(account[i].getUsername() == name && account[i].getPassword() == res){
                cout << "Dang nhap thanh cong!\n";
                account[i].menuSuaThongTin();
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
} */

