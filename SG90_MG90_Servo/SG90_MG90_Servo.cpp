#include <SG90_MG90_Servo.h>
//Arduino有內建<Servo.h>這個Library可以操作伺服馬達，本Library為目前版本為Arduino專用來操作SG90/ MG90用途
int Im_last_degree; //用來保存最後轉到位置用的
int Im_data_pin; //設定為MG90 or SG90控制線腳位
int Im_deg0_signal_duration_us; //為預設轉角角度0時的拉高訊號持續時間(單位us)
int Im_deg180_signal_duration_us; //為預設轉角角度180時的拉高訊號持續時間(單位us)
int Im_total_signal_duration_us; //為一組完整訊號的總持續時間(單位us)

void m_sg90_init(int data_pin){
    //初始化本code，請填入將使用的data_pin(沒意外是橘色那條，但不保證每個都一樣)
    pinMode(data_pin, OUTPUT);
    Im_data_pin = data_pin;
    Im_deg0_signal_duration_us = Im_default_deg0_signal_duration_us;
    Im_deg180_signal_duration_us = Im_default_deg180_signal_duration_us;
    Im_total_signal_duration_us = Im_default_total_signal_duration_us;
    m_sg90_go(0);//歸零，若您不需要歸零，建議您要修正Im_last_degree數值
}

void m_sg90_verify(int set_deg0_signal_duration_us, int set_deg180_signal_duration_us, int set_total_signal_duration_us){
    //設置訊號長度用
    //set_deg0_signal_duration_us為轉角角度0時的拉高訊號持續時間(單位us)
    //set_deg180_signal_duration_us為轉角角度180時的拉高訊號持續時間(單位us)
    //set_total_signal_duration_us為一組完整訊號的總持續時間(單位us)
    Im_deg0_signal_duration_us = set_deg0_signal_duration_us;
    Im_deg180_signal_duration_us = set_deg180_signal_duration_us;
    Im_total_signal_duration_us = set_total_signal_duration_us;
}

void m_sg90_go(int degree0_to_degree180){
    //請在degree0_to_degree180輸入0 ~ 180的角度
    int ima, imb, imc, imd, ime;
    imd = (Im_deg180_signal_duration_us - Im_deg0_signal_duration_us) / 180; //平均每度多少持續時間
    ima = (degree0_to_degree180 * imd) + Im_deg0_signal_duration_us;//請看以下說明
    //本商品訊號有幾個特徵，第一，每組訊號共20000us = ima + imb
    //第二，拉高部份大約從550us ~ 2500us(分別對應為0 ~ 180度) = ima
    //第三，拉低部份就等於20000us - ima = imb
    //第四，需要算一下每個轉角度的工作時間並持續訊號輸出
    //(這邊使用Im_last_degree來保存最後位置方便計算需要轉幾度)否則會被截斷(大約1秒/180度) = imc
    //以下便是根據以上規則所設計的SG90功能實現
    ime = Im_total_signal_duration_us / 1000; //將ms等級位數分出來否則delayMicroseconds函數數字太大會不準
    imb = (Im_total_signal_duration_us % 1000) - ima;//計算大概需準動時間尾數us
    if(degree0_to_degree180 > Im_last_degree){//這個if是去除負數用途
      imc = ((degree0_to_degree180 - Im_last_degree) / 9) + 1;
    }else{
      imc = ((Im_last_degree - degree0_to_degree180) / 9) + 1;
     }
    for(byte i = 0; i < imc; i++){
        digitalWrite(Im_data_pin, HIGH);
        delayMicroseconds(ima);
        digitalWrite(Im_data_pin, LOW);
        delay(ime);//delayMicroseconds超過萬會很不準所以拿17ms給delay算
        delayMicroseconds(imb);//剩下的3000us拿來這邊分配
    }
    Im_last_degree = degree0_to_degree180;//更新最後角度
}

int m_sg90_deg(){
    //會回傳目前伺服馬達所在角度
    return Im_last_degree;
}