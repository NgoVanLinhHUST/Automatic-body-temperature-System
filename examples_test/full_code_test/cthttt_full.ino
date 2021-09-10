#include <Wire.h>
#include <SPI.h>
#include <Adafruit_GFX.h>
#include <Adafruit_PCD8544.h>
#include <Adafruit_MLX90614.h>
/* Declare LCD object for SPI
  Adafruit_PCD8544(CLK,DIN,D/C,CE,RST);*/
Adafruit_PCD8544 display = Adafruit_PCD8544(13, 12, 14, 27, 26); /* */
int contrastValue = 60; /* Default Contrast Value */
Adafruit_MLX90614 mlx = Adafruit_MLX90614();
float t_room = mlx.readAmbientTempC();

const int trigPin = 5;
const int echoPin = 18;
long duration;
int distance = 0;
int ENA = 15;
boolean flag = false;
int IN1 = 2;

int IN2 = 4;
int giatri = 0;
const int congtachanhtrinh = 23;
void hamDoKhoangCach()
{
  //Serial.begin(9600);
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  duration = pulseIn(echoPin, HIGH);
  distance = duration * 0.034 / 2;
  Serial.print("Distance: ");
  Serial.println(distance);
  delay(1000);
}
void congTacHanhTrinh(){
  giatri = digitalRead(congtachanhtrinh);
  Serial.print("GIA TRI NHAN DUOC: ");
  Serial.println(giatri);
  if (giatri == HIGH) {
    digitalWrite(ENA, LOW);
    Serial.println("LED HIGH");
    flag = false;
  }
}
void setup()
{
  Serial.begin(9600);
  mlx.begin();
  display.begin();
  pinMode(19, OUTPUT);
  pinMode(congtachanhtrinh, INPUT);
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  pinMode(ENA, OUTPUT);
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  
}
void loop() {
  
  //Serial.begin(9600);
  hamDoKhoangCach();
  display.setContrast(contrastValue);
  display.clearDisplay();
  display.display();
  display.setTextColor(BLACK);
  display.setTextSize(1);
  display.setCursor(0, 0);
  display.println(distance);
  display.display();
  delay(100);

  if (flag) {
    congTacHanhTrinh();
  }
  
  if (distance < 15 && flag == false)
  {
    hamXuong();
    hamDoNhietDo();
    if (mlx.readObjectTempC() > 38)
    {
      hamCanhBao();
    }
    //delay(5000);
    hamLen();
    flag = true;
  }
}
void hamCanhBao() {
  digitalWrite(19, HIGH);
  delay(2000);
  digitalWrite(19, LOW);
  display.setContrast(contrastValue);
  display.clearDisplay();
  display.display();
  display.setTextColor(BLACK);
  display.setTextSize(1);
  display.setCursor(0, 0);
  display.println("Nhiet Do Cao");
  display.display();
  delay(2000);
}
void hamDoNhietDo()
{
  display.setContrast(contrastValue);
  display.clearDisplay();
  display.display();
  display.setTextColor(BLACK);
  display.setTextSize(1);
  display.setCursor(0, 0);
  display.print("Temp:");
  display.print(mlx.readObjectTempC());
  display.print("*C");
  display.display();
  delay(2000);
}
void hamXuong()
{
  display.setContrast(contrastValue);
  display.clearDisplay();
  display.display();
  display.setTextColor(BLACK);
  display.setTextSize(1);
  display.setCursor(0, 0);
  display.print("Move Down");
  display.display();
  delay(1000);
  digitalWrite(ENA, HIGH);
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
  delay(500);
  hamDoKhoangCach();
  if (distance > 15)
  {
    hamDoKhoangCach();
    delay(1000);
  }
  digitalWrite(ENA, LOW);

}
void hamLen()
{
  display.setContrast(contrastValue);
  display.clearDisplay();
  display.display();
  display.setTextColor(BLACK);
  display.setTextSize(1);
  display.setCursor(0, 0);
  display.print("Move Up");
  display.display();
  delay(500);
  digitalWrite(ENA, HIGH);
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
}
