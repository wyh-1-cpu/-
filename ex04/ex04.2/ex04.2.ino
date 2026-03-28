// 定义触摸引脚：T0对应ESP32的GPIO4，是原生触摸引脚
#define TOUCH_PIN 4    
// 定义LED引脚：ESP32 DevKit板载LED默认对应GPIO2
#define LED_PIN 2      
// 触摸触发阈值：触摸时数值会变小，低于此值触发中断
// 请通过串口监视器观察实际触摸值，按需调整
#define THRESHOLD 500   
// 触摸消抖时间(ms)：避免单次触摸触发多次中断
#define DEBOUNCE_TIME 500

// LED当前状态，初始为熄灭
bool ledState = false;
// 上次触摸触发的时间，用于消抖，volatile标记确保中断中可正常修改
volatile unsigned long lastTouchTime = 0;

// 触摸中断服务函数（ISR）
// IRAM_ATTR 修饰：将函数放在内部RAM中，大幅提升中断响应稳定性，避免崩溃
void IRAM_ATTR gotTouch() {
  unsigned long currentTime = millis();
  // 消抖判断：仅当距离上次触发超过消抖时间，才响应触摸
  if (currentTime - lastTouchTime > DEBOUNCE_TIME) {
    ledState = !ledState;  // 翻转LED状态
    digitalWrite(LED_PIN, ledState);  // 更新LED输出
    lastTouchTime = currentTime;  // 记录本次触发时间
  }
}

void setup() {
  // 初始化串口，波特率115200，用于观察触摸值调试
  Serial.begin(115200);
  delay(1000);  // 等待串口稳定
  // 初始化LED引脚为输出模式
  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, ledState);  // 初始化LED为初始熄灭状态

  // 核心：绑定触摸中断
  // 参数：触摸引脚、中断服务函数、触发阈值
  touchAttachInterrupt(TOUCH_PIN, gotTouch, THRESHOLD);

  // 串口输出提示信息
  Serial.println("===== ESP32触摸中断实验 =====");
  Serial.print("触摸引脚：GPIO");
  Serial.println(TOUCH_PIN);
  Serial.print("LED引脚：GPIO");
  Serial.println(LED_PIN);
  Serial.print("当前触发阈值：");
  Serial.println(THRESHOLD);
  Serial.println("功能：触摸一次翻转LED状态");
  Serial.println("==============================");
}

void loop() {
  // 循环读取并打印当前触摸值，方便你观察和调整阈值
  int touchValue = touchRead(TOUCH_PIN);
  Serial.print("当前触摸值：");
  Serial.print(touchValue);
  Serial.print(" | LED状态：");
  Serial.println(ledState ? "亮" : "灭");

  delay(100);  // 控制串口输出频率，避免刷屏
}
