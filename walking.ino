#include <ESP32Servo.h> 
#include <BluetoothSerial.h>

class LoggedServo : public Servo {
 public:
  int last = 90;
  void write(int v) { last = constrain(v, 0, 180); Servo::write(v); }
};

const int s1 = 105;
const int s2 = 140 + 90;
const int s3 = 75;
const int s4 = 65 - 90;
const int s5 = 145;
const int s6 = 70 - 90;
const int s7 = 50;
const int s8 = 130 + 90;
int servo1 = 13;
int servo2 = 14;
int servo3 = 27;
int servo4 = 4;
int servo5 = 33;
int servo6 = 17;
int servo7 = 18;
int servo8 = 19;
LoggedServo Servo1;
LoggedServo Servo2;
LoggedServo Servo3;
LoggedServo Servo4;
LoggedServo Servo5;
LoggedServo Servo6;
LoggedServo Servo7;
LoggedServo Servo8;

float leg1PositionTop;
float leg1PositionBottom;
float legCornerTop;
float legCornerBottom;

float startPositionFrontTop;
float startPositionFrontBottom;
float startPositionBackTop;
float startPositionBackBottom;

float frontRearCornerTop;
float frontRearCornerBottom;
float frontCornerTop;
float frontCornerBottom;
float frontMiddleCornerTop;
float frontMiddleCornerBottom;
float frontBeforeCornerTop;
float frontBeforeCornerBottom;

float backMiddleCornerTop;
float backMiddleCornerBottom;
float backRearCornerTop;
float backRearCornerBottom;
float backCornerTop;
float backCornerBottom;
float backBeforeCornerTop;
float backBeforeCornerBottom;

int numberOfSteps = 40;   //Количество шагов
float h = 6;
float l = 0;
float B;
int d = 1;
float s;
int pauseFlag1;
int pauseFlag2;
int pauseFlag3;
int pauseFlag4;
int k1;
int k2;
int k3;
int k4;
int direction = 1;
int const x = 5;
static unsigned long previousMillis = 0; // Время последнего обновления
const unsigned long interval = 30;       // Интервал между шагами (мс)
static int i1 = 0;                       // Текущие шаги
static int i2 = 20;  
static int i3 = 10;
static int i4 = 30;
BluetoothSerial ESP_BT; // Объект для Bluetooth
int incoming;
struct Degrees {
  float servTop;
  float servBottom;
};
Degrees adjustment[4];
Degrees calculateDegrees(float h, float l);
//-----------------------------------------------------------
void setup() {
  Serial.begin(115200);
  ESP_BT.begin("Sobaka");// Задаем имя
  Serial.println("------------");

  Servo1.attach(servo1);
  Servo2.attach(servo2);
  Servo3.attach(servo3);
  Servo4.attach(servo4);
  Servo5.attach(servo5);
  Servo6.attach(servo6);
  Servo7.attach(servo7);
  Servo8.attach(servo8);
  legCornerTop = calculateDegrees(5.5, 0).servTop;//Верхний верхний угол
  legCornerBottom = calculateDegrees(5.5, 0).servBottom;//Нижний верхний угол
//--------------# Перед #---------------
  frontRearCornerTop = calculateDegrees(7, -4).servTop; //Верхний стартовый угол
  frontRearCornerBottom = calculateDegrees(7, -4).servBottom; //Нижний стартовый угол
  frontBeforeCornerTop = calculateDegrees(7, 1).servTop; //Верхний передний угол
  frontBeforeCornerBottom = calculateDegrees(7, 1).servBottom; //Нижний передний угол
  frontMiddleCornerTop = calculateDegrees(7, 0).servTop;
  frontMiddleCornerBottom = calculateDegrees(7, 0).servBottom;
  frontCornerTop = (calculateDegrees(7, 1).servTop - calculateDegrees(7, -4).servTop) / numberOfSteps; //Верхний шаг
  frontCornerBottom = (calculateDegrees(7, 1).servBottom - calculateDegrees(7, -4).servBottom) / numberOfSteps;//Нижный шаг
  
//--------------# Зад #---------------
  backRearCornerTop = calculateDegrees(7.5, -1).servTop; //Верхний стартовый угол
  backRearCornerBottom = calculateDegrees(7.5, -1).servBottom; //Нижний стартовый угол
  backBeforeCornerTop = calculateDegrees(7.5, 4).servTop; //Верхний передний угол
  backBeforeCornerBottom = calculateDegrees(7.5, 4).servBottom; //Нижний передний угол
  backMiddleCornerTop = calculateDegrees(7, 0).servTop;
  backMiddleCornerBottom = calculateDegrees(7, 0).servBottom;
  backCornerTop = (calculateDegrees(7, 4).servTop - calculateDegrees(7, -1).servTop) / numberOfSteps; //Верхний шаг
  backCornerBottom = (calculateDegrees(7, 4).servBottom - calculateDegrees(7, -1).servBottom) / numberOfSteps;//Нижный шаг

//--------------# Команды #---------------

//              # Лежать #

Servo1.write(s1 + frontMiddleCornerTop);
Servo2.write(s2 - frontMiddleCornerBottom);
Servo3.write(s3 - frontMiddleCornerTop);
Servo4.write(s4 + frontMiddleCornerBottom);
Servo5.write(s5 - backMiddleCornerTop);
Servo6.write(s6 + backMiddleCornerBottom);
Servo7.write(s7 + backMiddleCornerTop);
Servo8.write(s8 - backMiddleCornerBottom);
delay (2000);
}
//----------------------------------
void loop() {

  unsigned long currentMillis = millis();

  static unsigned long lastReport = 0;
  if (currentMillis - lastReport >= 20) 
    { 
    lastReport = currentMillis; reportServos(); 
    }

  if (ESP_BT.available())//Проверяем, не получили ли
    {
    incoming = ESP_BT.read(); // Читаем,что получили
    direction = incoming;
    }  
  k1 = 1;
  k2 = 1;
  k3 = 1;
  k4 = 1;
  if (i1 >= numberOfSteps/ 5 ){
    k1 = -1;
  }
  if (i2 >= numberOfSteps/ 5 ){
    k2 = -1;
  }
  if (i3 >= numberOfSteps/ 5 ){
    k3 = -1;
  }
  if (i4 >= numberOfSteps/ 5 ){
    k4 = -1;
  }
  if (direction == 0){
    pauseFlag1++;
    pauseFlag2++;
    pauseFlag3++;
    pauseFlag4++;
  }
  if (direction == 1){
    d = 1;
    startPositionFrontTop = frontRearCornerTop;
    startPositionFrontBottom = frontRearCornerBottom;
    startPositionBackTop = backRearCornerTop;
    startPositionBackBottom = backRearCornerBottom;
  }
  if (direction == -1){
    d = -1;
    startPositionFrontTop = frontBeforeCornerTop;
    startPositionFrontBottom = frontBeforeCornerBottom;
    startPositionBackTop = backBeforeCornerTop;
    startPositionBackBottom = backBeforeCornerBottom;
  }
  if (currentMillis - previousMillis >= interval) {
    previousMillis = currentMillis; // Обновляем время последнего шага

    // Устанавливаем углы для сервоприводов
    //-----------------# Нога №1 #------------------
    if (pauseFlag1 > 0){
      pauseFlag1--;
    }
    else {
      leg1PositionTop = (s1 + (frontCornerTop * i1)* d + startPositionFrontTop);
      leg1PositionBottom = (s2 + ((frontCornerBottom * i1) * k1) * d - startPositionFrontBottom);
      
      i1++;
      if (i1 > numberOfSteps) {
        leg1PositionTop = (s1 + legCornerTop);
        leg1PositionBottom = (s2 - legCornerBottom);
        i1 = 0; // Сбрасываем счётчик, если достигли конца
        pauseFlag1= 3;
      }
      Servo1.write(leg1PositionTop);
      Servo2.write(leg1PositionBottom);
    }
    //-----------------# Нога №2 #------------------
    if (pauseFlag2 > 0){
      pauseFlag2--;
    }
    else {
      Servo3.write(s3 - (frontCornerTop * i2) * d - startPositionFrontTop);
      Servo4.write(s4 - ((frontCornerBottom * i2) * k2) * d + startPositionFrontBottom);
      i2++;
      if (i2 > numberOfSteps) {
        Servo3.write(s3 - legCornerTop);
        Servo4.write(s4 + legCornerBottom);
        i2 = 0; // Сбрасываем счётчик, если достигли конца
        pauseFlag2= 3;
      }
    }

    //-----------------# Нога №3 #------------------
    if (pauseFlag3 > 0){
      pauseFlag3--;
    }
    else {
      Servo5.write(s5 - (backCornerTop * i3) * d - startPositionBackTop);
      Servo6.write(s6 - ((backCornerBottom * i3)* k3) *d + startPositionBackBottom);
      i3++;
      if (i3 > numberOfSteps) {
        Servo5.write(s5 - legCornerTop);
        Servo6.write(s6 + legCornerBottom);
        i3 = 0; // Сбрасываем счётчик, если достигли конца
        pauseFlag3= 3;
      }
    }
    //-----------------# Нога №4 #------------------
    if (pauseFlag4 > 0){
      pauseFlag4--;
    }
    else {
      Servo7.write(s7 + (backCornerTop * i4) * d + startPositionBackTop);
      Servo8.write(s8 + ((backCornerBottom * i4)* k4) *d - startPositionBackBottom);
      i4++;
      if (i4 > numberOfSteps) {
        Servo7.write(s7 + legCornerTop);
        Servo8.write(s8 - legCornerBottom);
        i4 = 0; // Сбрасываем счётчик, если достигли конца
        pauseFlag4= 3;
      }
    }
  }
}
//-----------------------------------------------------------
Degrees calculateDegrees(float h, float l) {
  Degrees result;
  float b = (sqrt(h * h + l * l));
  float G = degrees(acos(((2 * x * x) - b * b) / (2 * x * x)));
  if (l > 0) {
    B = ((180 - G) / 2) + degrees(acos(h / b));
  } 
  else {
    B = ((180 - G) / 2) - degrees(acos(h / b));
  }
  result.servTop = B;
  result.servBottom = G;
  return result;
}
void reportServos() {
  Serial.printf("S,%d,%d,%d,%d,%d,%d,%d,%d\n",
    Servo1.last, Servo2.last, Servo3.last, Servo4.last,
    Servo5.last, Servo6.last, Servo7.last, Servo8.last);
}