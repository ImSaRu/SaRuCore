#include <Arduino.h>

#define Im_defult_speed_of_sound 0.34029 //預設在一般大氣壓力中常態下音速(單位公釐/秒)

void hc_sr04_init(int trig_pin, int echo_pin);
/*建議使用本Library前先進行初始化，將會調整應對數值
 @param trig_pin: 模組上的trig pin
 @param echo_pin: 模組上的echo pin
 */
void hc_sr04_verify(int speed_of_sound);
/*調整音速使用，可能應用是在介質之中，預設的音速0.34029mm/s是指在一般空氣環境中的音速
 @param speed_of_sound: 需要變成多少mm/s聲波傳遞速度
 */
float hc_sr04_dist_raw();
/*本功能會透過超聲波返回模組正前方障礙物與模組間的直線距離。又，本功能屬於raw data，因此偶爾會有偏差的錯誤值跳出
若想使用raw data，可直接調用本功能，若希望使用平均校正過的資料可參考以下功能
 @return 距離(單位mm)，可精確至小數點後兩位
 */
float hc_sr04_dist(int time_to_averge);
/*本功能為校正輸出距離數據，避免偶爾的雜訊影響整體判斷，可以直接使用本功能就能得到校正後的距離值(不需使用hc_sr04_dist_raw())
 @return 距離(單位mm)，可精確至小數點後兩位
 @param time_to_averge: 需要平均的次數，建議5次就夠了
 */
