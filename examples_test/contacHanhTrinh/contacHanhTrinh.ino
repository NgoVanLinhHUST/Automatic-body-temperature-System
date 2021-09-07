const int congtachanhtrinh = 23;  // Dây ra công tăc hành trình dây màu xanh nối chân 3   
const int led =  33;  
int giatri = 0;     
void setup() {
  Serial.begin(9600);
  pinMode(led, OUTPUT);
  pinMode(congtachanhtrinh, INPUT);
}
void loop() {
  giatri = digitalRead(congtachanhtrinh); // Đọc tín hiệu từ chân số 3
  Serial.print("GIA TRI NHAN DUOC: ");
 Serial.println(giatri);
 // giá trị nhận được từ công tắc hành trình bằng 1 thì led sáng 
 // còn giá trị nhận được từ công tắc hành trình bằng 0 thì led tắt
  if (giatri == HIGH) {
    digitalWrite(led, HIGH);
    Serial.println("LED HIGH");
  } else {
    digitalWrite(led, LOW);
    Serial.println("LED LOW");
  }
  delay(1000);// delay 1 giây
}
