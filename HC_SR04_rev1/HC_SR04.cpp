#include "HC_SR04.h"
//hc_sr04使用上算是非常簡單的，基本上難的部分(訊號解析)模組都搞定了，使用上只要使用trig腳位打出聲波並用echo腳位接收訊號即可輕鬆使用
//但是超聲波產品由於其聲波特性，若在太接近的地方同時有兩個相同裝置在運作時可能聲波會互相影響
float Im_speed_of_sound; //音速(單位公釐/秒)
int Im_Trig;             //trig腳位
int Im_Echo;             //echo腳位

void hc_sr04_init(int trig_pin, int echo_pin){
    //初始化本code，請填入將使用的trig腳位與echo腳位
    pinMode(trig_pin, OUTPUT);//trig腳位拉高會打出聲波
    pinMode(echo_pin, INPUT);//echo準備好工作時會先拉高電位直到收到聲波時會拉低電位結束一次工作循環
    digitalWrite(trig_pin, LOW);//拉低待機
    Im_Trig = trig_pin;
    Im_Echo = echo_pin;
    Im_speed_of_sound = Im_defult_speed_of_sound;//將音速設置為常態音速，若需更改請使用verify功能或更該header
}

void hc_sr04_verify(int speed_of_sound){
    //調整音速使用，可能應用是在介質之中，預設的音速0.34029mm/s是指在一般空氣環境中的音速
    //若需要調整，設置音速單位為公釐/秒
    Im_speed_of_sound = speed_of_sound;
}

float hc_sr04_dist_raw(){
    //本功能會透過超聲波返回模組正前方障礙物與模組間的直線距離，返回格式為float，可精確至小數點後兩位
    float Im_flying_time = 0;//聲波走的時間(us)
    float Im_distance = 0;//換算後的距離(單位mm)，保留小數點讓精確度更高
    digitalWrite(Im_Trig, HIGH);//需要模組工作需要至少拉高10us的訊號
    delayMicroseconds(10);
    digitalWrite(Im_Trig, LOW);//拉回原位
    /*本模組的用法就是trig發送工作訊號後，模組就會打出超聲波，接著等待輸入訊號
    拉高後開始計時，直到下次拉低為止為聲波走的時間，就可以根據音速轉換成距離*/
    Im_flying_time = pulseIn(Im_Echo, HIGH);//用pulseIn函數來讀出拉高時間
    //在這邊計算使用音速約一秒鐘走340290mm，在除以2(來回時間)
    Im_distance = (Im_flying_time * Im_speed_of_sound) /2;
    return Im_distance; //會根據使用的MCU有不同長度的浮點數返回，在這邊測試是使用Arduino返回僅會有小數點後兩位
}

float hc_sr04_dist(int time_to_averge){
    //本功能為校正輸出距離數據，避免偶爾的雜訊影響整體判斷，請在time_to_averge變數中填入平均次數，越多越準，但是讀取時間會拉長
    //測試時習慣都是平均5次，建議可以使用5就好，本功能主要是剃除突然雜訊造成整體距離值大幅增加及縮小的bug使用

    float ima[time_to_averge];//裝raw data
    float imb = 0;//平均值及剃除雜訊後的平均值
    float imc = 0;//總和沒有雜訊的距離值
    float imd = 0;//計算有幾次雜訊
    for(int y = 0; y < time_to_averge; y++){
        ima[y] = hc_sr04_dist_raw();//讀取平均次數的raw data
    }
    for(int y = 0; y < time_to_averge; y++){
        imb += ima[y];//先算raw data平均值
    }
    imb /= time_to_averge;//先算raw data平均值
    for(int y = 0; y < time_to_averge; y++){
        if(ima[y] <= imb){//找出大幅影響的雜訊(本模組使用時雜訊基本是輸出大距離而非小距離)
            imc += ima[y];//總和沒有雜訊值的數據
        }else imd++;//若有雜訊產生計算其數量
    }
    imb = imc / (time_to_averge - imd);//取沒有雜訊的平均值
    return imb;
}