  int ENA1 = 15;
  
  int INA1 = 2;
  
  int INA2 = 4;
void setup() {
  pinMode(ENA1, OUTPUT);
  pinMode(INA1, OUTPUT);
  pinMode(INA2, OUTPUT);
}

void hamLen()
{
  digitalWrite(ENA1, HIGH);
  digitalWrite(INA1, HIGH);
  digitalWrite(INA2, LOW);
}
void hamXuong()
{
  digitalWrite(ENA1, HIGH);
  digitalWrite(INA1, LOW);
  digitalWrite(INA2, HIGH);
}
void loop() {
    hamLen();
    delay(3000);
    hamXuong();
    delay(3000);
}
