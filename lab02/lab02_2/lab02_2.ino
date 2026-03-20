// 定义两个LED引脚
#define LED1_PIN 13
#define LED2_PIN 14

void setup() {
  // 设置两个引脚为输出模式
  pinMode(LED1_PIN, OUTPUT);
  pinMode(LED2_PIN, OUTPUT);
}

void loop() {
  // LED1亮，LED2灭
  digitalWrite(LED1_PIN, HIGH);
  digitalWrite(LED2_PIN, LOW);
  delay(500);  // 保持500ms

  // LED1灭，LED2亮
  digitalWrite(LED1_PIN, LOW);
  digitalWrite(LED2_PIN, HIGH);
  delay(500);  // 保持500ms
}
