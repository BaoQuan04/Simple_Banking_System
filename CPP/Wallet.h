#ifndef WALLET_H
#define WALLET_H

#include<iostream>
#include<string>



using namespace std;

class Wallet{
    private :
        int Point;
        string MaVi;
        int Spent;
        int Claim;
        static int Num;
        
    public :

        // //Khoi tao vi tong va quan li tong chi thu cua vi tong 
        // //Vi tong dc khoi tao san mot luong diem lon, khi mot user dc khoi tao, mot luong diem nhat dinh se dc chuyen vao vi cua user tu vi tong
        // void Create_TotalWallet();
        // bool Check_SpCl();//Kiem tra xem tong chi va tong thu co bang nhau hay khong
 

        //Constructor : chuyen san mot luong diem vao vi nguoi dung khi khoi tao 
        Wallet();
        void Create_Mavi();

        void Transfer();
        void Minus_Point(string);// Minus the point of the user A to transfer to user B, string truyen vao la ma vi cua user A
        void Add_Point(string); // Add the point transfered from A wallet to B Wallet, string truyen vao la ma vi cua user B

        void Check_Balance();
        void Check_History();
        void Check_TransUser();// Transfered User

        bool CreateOTP();


};






#endif 