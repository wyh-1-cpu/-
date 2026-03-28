/*
ESP32 多档位触摸调速呼吸灯 简化稳定版
用analogWrite实现PWM，完全避开LEDC编译报错
硬件完全匹配：触摸GPIO4，LED GPIO2
*/

// ===================== 硬件定义（和之前完全一致，无需修改）=====================
#define TOUCH_PIN 4   // 你的触摸引脚
#define LED_PIN 2     // 你的LED引脚

// ！！！这里直接填你之前跑通的触摸阈值！！！
const int touchThreshold = 800;

// ===================== 呼吸灯核心配置 =====================
int brightness = 0;            // 当前LED亮度（0-255）
int fadeDirection = 1;         // 呼吸方向：1=渐亮，-1=渐暗

// 3档速度配置（数值越大，呼吸越快）
const int speedConfig[3][2] = {
  {1, 15},   // 1档：最慢
  {2, 10},   // 2档：中等
  {5, 5}     // 3档：最快
};
int currentLevel = 1;  // 初始档位1档

// ===================== 触摸防抖&边缘检测 =====================
bool lastTouchState = false;
const unsigned long debounceInterval = 500;
unsigned long lastTriggerTime = 0;

// ===================== 初始化 =====================
void setup() {
  Serial.begin(115200);
  delay(1000);
  pinMode(LED_PIN, OUTPUT);
  analogWrite(LED_PIN, 0); // 初始熄灭

  Serial.println("===== 多档位触摸调速呼吸灯 就绪 =====");
  Serial.print("当前触摸阈值：");
  Serial.println(touchThreshold);
  Serial.print("初始档位：");
  Serial.println(currentLevel);
  Serial.println("操作说明：每触摸一次GPIO4，切换一次呼吸速度档位");
}

// ===================== 主循环 =====================
void loop() {
  // 1. 触摸检测&档位切换（和你跑通的逻辑完全一致）
  int touchValue = touchRead(TOUCH_PIN);
  Serial.print("Touch Value: ");
  Serial.print(touchValue);
  bool currentTouchState = (touchValue < touchThreshold);

  // 边缘检测+防抖，仅触摸瞬间触发
  if (currentTouchState == true && lastTouchState == false) {
    if (millis() - lastTriggerTime > debounceInterval) {
      // 档位循环切换
      currentLevel = currentLevel % 3 + 1;
      
      // 串口打印信息
      Serial.print(" | 【档位切换成功】当前档位：");
      Serial.print(currentLevel);
      Serial.print(" | 呼吸速度：");
      Serial.println(currentLevel == 1 ? "慢" : (currentLevel == 2 ? "中" : "快"));
      
      lastTriggerTime = millis();
    }
  }
  lastTouchState = currentTouchState;

  // 2. 非阻塞式呼吸灯更新
  int step = speedConfig[currentLevel - 1][0];
  int updateInterval = speedConfig[currentLevel - 1][1];

  static unsigned long lastFadeTime = 0;
  if (millis() - lastFadeTime >= updateInterval) {
    lastFadeTime = millis();

    // 更新亮度
    brightness += fadeDirection * step;
    // 反转呼吸方向
    if (brightness >= 255 || brightness <= 0) {
      fadeDirection = -fadeDirection;
      brightness = constrain(brightness, 0, 255);
    }

    // 设置LED亮度
    analogWrite(LED_PIN, brightness);
  }

  Serial.println();
  delay(10);
}
