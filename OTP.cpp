#include <iostream>
#include <cstring>
#include <cstdlib> //Thu vien tao ham phat sinh so ngau nhien: rand(), srand();
#include <ctime> // time()

using namespace std;

#define size 7


//Ham sinh so ngau nhien trong khoang min, max
int GetRandom(int min, int max){
    return min + (int) (rand() * (max - min + 1.0) / (1.0 + RAND_MAX)); 
}

bool CreateOTP(){
    //Khoi tao seed cho rand() de sinh so ngau nhien thuc su
    srand(time(0)); //Su dung thoi gian hien tai lam seed
    int cnt =3;
    while(cnt--){
        char code[size], ch ,otp[size];
        int n = 0;
        while(n < size -1){
            int res = 0 + (int) (rand() * (127 - 0 + 1.0) / (1.0 + RAND_MAX));
            ch = (char) res;
            if( (ch >= '0' && ch <= '9') ){
                code[n] = ch;
                n++;
            }
        }
        code[size] =  '\0';
        
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

int main(){
    CreateOTP();
    return 0;
}