// 完全基于你实验的原版基础代码修改，只加自锁逻辑，100%兼容你的硬件
// 定义触摸引脚（T0对应GPIO4）
#define TOUCH_PIN 4
// 定义LED引脚（ESP32 DevKit板载LED通常是GPIO2）
#define LED_PIN 2

// ！！！这里直接填你第三步跑通的那个阈值！！！不要改别的！！！
int threshold = 800;
// 触摸值（和实验代码完全一致）
int touchValue;

// ========== 题目要求的自锁核心变量 ==========
bool ledState = false;          // 布尔型LED状态变量，默认熄灭
bool lastTouchState = false;    // 上一次的触摸状态，用于边缘检测
// 软件防抖：两次触发的最小间隔，防止手抖误触发
const unsigned long debounceInterval = 500;
unsigned long lastTriggerTime = 0;
// ==============================================

void setup() {
  // 和你的实验代码完全一致，一字不差
  Serial.begin(115200);
  delay(1000); // 等待串口稳定
  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, LOW); // 上电默认熄灭
  Serial.println("===== 触摸自锁开关 实验专属版 =====");
  Serial.print("当前触发阈值：");
  Serial.println(threshold);
  Serial.println("功能：摸一下亮，再摸一下灭");
}

void loop() {
  // 这部分和你实验的基础代码完全一致，一字不差
  touchValue = touchRead(TOUCH_PIN);
  Serial.print("Touch Value: ");
  Serial.println(touchValue);

  // 1. 判断触摸状态，和实验逻辑完全一致：小于阈值=正在触摸
  bool currentTouchState = (touchValue < threshold);

  // 2. 题目要求的核心逻辑：边缘检测 + 软件防抖
  // 仅在【上一次未触摸，当前刚触摸】的瞬间触发，长按不会重复触发
  if (currentTouchState == true && lastTouchState == false) {
    // 防抖：两次触发间隔必须大于500ms，防止手抖导致连续翻转
    if (millis() - lastTriggerTime > debounceInterval) {
      // 翻转LED状态，实现自锁
      ledState = !ledState;
      digitalWrite(LED_PIN, ledState ? HIGH : LOW);
      
      // 串口打印触发日志，方便你调试
      Serial.print("【触发成功】LED当前状态：");
      Serial.println(ledState ? "亮起" : "熄灭");
      
      // 更新上次触发时间，完成防抖
      lastTriggerTime = millis();
    }
  }

  // 3. 更新上一次的触摸状态，为下一次循环的边缘检测做准备
  lastTouchState = currentTouchState;

  // 和你的实验代码完全一致的100ms延时，绝对兼容
  delay(100);
}
