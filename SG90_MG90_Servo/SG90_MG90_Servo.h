#include <Arduino.h>

#define Im_default_deg0_signal_duration_us   550   //為預設轉角角度0時的拉高訊號持續時間(單位us)
#define Im_default_deg180_signal_duration_us 2500  //為預設轉角角度180時的拉高訊號持續時間(單位us)
#define Im_default_total_signal_duration_us  20000 //為一組完整訊號的總持續時間(單位us)

void m_sg90_init(int data_pin);
/*建議使用本Library前先進行初始化，將會調整應對數值，若您有一組以上複用，目前版本使用中直接初始化將要控制的對象即可
 @param data_pin: MG90 OR SG90的DATA腳位
 */
void m_sg90_go(int degree0_to_degree180);
/*本功能即為根據需要角度進行轉動之功能
 @param degree0_to_degree180: MG90 OR SG90儘可以轉0到180度，請您填寫任意需要轉動的角度
 */
int m_sg90_deg();
/*本功能將會回傳目前角度的正確數值
 @return int資料格式，角度數值
 */
void m_sg90_verify(int set_deg0_signal_duration_us, int set_deg180_signal_duration_us, int set_total_signal_duration_us);
/*本功能不建議使用，為調整訊號參數使用，一般的sg90與mg90訊號大致上是如預設那樣，若您需要對拉高波形做調整可以使用本功能
  此外，若是轉動角度不夠時也可以試著調整deg0或deg180的訊號長度
 @param set_deg0_signal_duration_us: 為轉角角度0時輸出訊號的拉高訊號持續時間(單位us)
 @param set_deg180_signal_duration_us: 為轉角角度180時輸出訊號的拉高訊號持續時間(單位us)
 @param set_total_signal_duration_us: 為一組完整訊號的總持續時間(單位us)
 */

