// include library
#include <WiFi.h>
#include <Wire.h>
#include <SPI.h>
#include "ESPAsyncWebServer.h"
#include <AsyncTCP.h>
#include <Adafruit_MLX90614.h>
#include <Adafruit_GFX.h>
#include <Adafruit_PCD8544.h>

#define YOUR_SSID       "Lan Dat"
#define YOUR_PASSWORD   "88888888"


// set up webserver and wifi
AsyncWebServer server(80);
const char *ssid = YOUR_SSID;
const char *pass = YOUR_PASSWORD;



/* Declare LCD object for SPI
  Adafruit_PCD8544(CLK,DIN,D/C,CE,RST);*/
Adafruit_PCD8544 display = Adafruit_PCD8544(13, 12, 14, 27, 26); /* */
int contrastValue = 60; /* Default Contrast Value */
Adafruit_MLX90614 mlx = Adafruit_MLX90614();  
float t_room = mlx.readAmbientTempC();
int count = 0;  
const int trigPin = 5; //trigPin HCSR-04
const int echoPin = 18; //echoPin HCSR-04
long duration;
boolean flag = false;   // flag cong tac hanh trinh
int distance = 0;

/* declare L298N  */
int ENA = 15;   
int IN1 = 2;
int IN2 = 4;
int giatri = 0;
const int congtachanhtrinh = 23;

// AsyncWebserver send temperature 
const char html[] PROGMEM = R"rawliteral(
<!DOCTYPE html>
<html>
<body>
<p style="color:blue;text-align:center; font-size: 100px">Your Temp: </p>
<span id="presscount" style="color:blue; font-size: 100px"><center>  %t_object%</center><span>
  <script>
      setInterval(function () {
          var xhttp = new XMLHttpRequest();
          xhttp.onreadystatechange = function() {
              if (this.readyState == 4 && this.status == 200) {
                  document.getElementById("presscount").innerHTML =
                      this.responseText;
              }
          };
          xhttp.open("GET", "/presscount", true);
          xhttp.send();
      },100);
    </script>
</body>
</html>
)rawliteral";


String processor(const String& var)
{
if(var == "t_object")
return String(mlx.readObjectTempC());
}

void setup() {
  Serial.begin(9600);
  mlx.begin();
  display.begin();
  WiFi.begin(ssid,pass); // set up wifi sta
  while(WiFi.status()!=WL_CONNECTED);
  Serial.println(WiFi.localIP());
  pinMode(19, OUTPUT);
  pinMode(congtachanhtrinh, INPUT);
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  pinMode(ENA, OUTPUT);
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);

  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
  request->send_P(200, "text/html", html,processor);
  });
  server.on("/presscount", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(200, "text/plain", String(mlx.readObjectTempC()));
  });
  server.begin();
}

void loop() {
  Serial.println(mlx.readObjectTempC());
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

void hamDoKhoangCach()
{
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
    Serial.println("End...");
    flag = false;
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

// login form
const char html1[] PROGMEM = R"rawliteral(
<!DOCTYPE html>
<html>
<head>
    <meta name="viewport" content="width=device-width, initial-scale=1">
    <style>
        html {
            background-color: #56baed;
        } 
        body {
            font-family: "Poppins", sans-serif;
            height: 100vh;
        } 
        a {
            color: #92badd;
            display: inline-block;
            text-decoration: none;
            font-weight: 400;
        }
        h2 {
            text-align: center;
            font-size: 16px;
            font-weight: 600;
            text-transform: uppercase;
            display: inline-block;
            margin: 40px 8px 10px 8px;
            color: #cccccc;
        }
        .wrapper {
            display: flex;
            align-items: center;
            flex-direction: column;
            justify-content: center;
            width: 100%;
            min-height: 100%;
            padding: 20px;
        }
        #formContent {
            -webkit-border-radius: 10px 10px 10px 10px;
            border-radius: 10px 10px 10px 10px;
            background: #fff;
            padding: 30px;
            width: 90%;
            max-width: 450px;
            position: relative;
            padding: 0px;
            -webkit-box-shadow: 0 30px 60px 0 rgba(0, 0, 0, 0.3);
            box-shadow: 0 30px 60px 0 rgba(0, 0, 0, 0.3);
            text-align: center;
        }
        #formFooter {
            background-color: #f6f6f6;
            border-top: 1px solid #dce8f1;
            padding: 25px;
            text-align: center;
            -webkit-border-radius: 0 0 10px 10px;
            border-radius: 0 0 10px 10px;
        }
        h2.inactive {
            color: #cccccc;
        }
        h2.active {
            color: #0d0d0d;
            border-bottom: 2px solid #5fbae9;
        }
        input[type=button],
        input[type=submit],
        input[type=reset] {
            background-color: #56baed;
            border: none;
            color: white;
            padding: 15px 80px;
            text-align: center;
            text-decoration: none;
            display: inline-block;
            text-transform: uppercase;
            font-size: 13px;
            -webkit-box-shadow: 0 10px 30px 0 rgba(95, 186, 233, 0.4);
            box-shadow: 0 10px 30px 0 rgba(95, 186, 233, 0.4);
            -webkit-border-radius: 5px 5px 5px 5px;
            border-radius: 5px 5px 5px 5px;
            margin: 5px 20px 40px 20px;
            -webkit-transition: all 0.3s ease-in-out;
            -moz-transition: all 0.3s ease-in-out;
            -ms-transition: all 0.3s ease-in-out;
            -o-transition: all 0.3s ease-in-out;
            transition: all 0.3s ease-in-out;
        }
        input[type=button]:hover,
        input[type=submit]:hover,
        input[type=reset]:hover {
            background-color: #39ace7;
        }
        input[type=button]:active,
        input[type=submit]:active,
        input[type=reset]:active {
            -moz-transform: scale(0.95);
            -webkit-transform: scale(0.95);
            -o-transform: scale(0.95);
            -ms-transform: scale(0.95);
            transform: scale(0.95);
        }
        input[type=text] {
            background-color: #f6f6f6;
            border: none;
            color: #0d0d0d;
            padding: 15px 32px;
            text-align: center;
            text-decoration: none;
            display: inline-block;
            font-size: 16px;
            margin: 5px;
            width: 85%;
            border: 2px solid #f6f6f6;
            -webkit-transition: all 0.5s ease-in-out;
            -moz-transition: all 0.5s ease-in-out;
            -ms-transition: all 0.5s ease-in-out;
            -o-transition: all 0.5s ease-in-out;
            transition: all 0.5s ease-in-out;
            -webkit-border-radius: 5px 5px 5px 5px;
            border-radius: 5px 5px 5px 5px;
        }
        input[type=text]:focus {
            background-color: #fff;
            border-bottom: 2px solid #5fbae9;
        }
        input[type=text]:placeholder {
            color: #cccccc;
        }
        .fadeInDown {
            -webkit-animation-name: fadeInDown;
            animation-name: fadeInDown;
            -webkit-animation-duration: 1s;
            animation-duration: 1s;
            -webkit-animation-fill-mode: both;
            animation-fill-mode: both;
        }
        @-webkit-keyframes fadeInDown {
            0% {
                opacity: 0;
                -webkit-transform: translate3d(0, -100%, 0);
                transform: translate3d(0, -100%, 0);
            }
            100% {
                opacity: 1;
                -webkit-transform: none;
                transform: none;
            }
        }
        @keyframes fadeInDown {
            0% {
                opacity: 0;
                -webkit-transform: translate3d(0, -100%, 0);
                transform: translate3d(0, -100%, 0);
            }
            100% {
                opacity: 1;
                -webkit-transform: none;
                transform: none;
            }
        }
        @-webkit-keyframes fadeIn {
            from {
                opacity: 0;
            }
            to {
                opacity: 1;
            }
        }  
        @-moz-keyframes fadeIn {
            from {
                opacity: 0;
            }
            to {
                opacity: 1;
            }
        }
        @keyframes fadeIn {
            from {
                opacity: 0;
            }
            to {
                opacity: 1;
            }
        }
        .fadeIn {
            opacity: 0;
            -webkit-animation: fadeIn ease-in 1;
            -moz-animation: fadeIn ease-in 1;
            animation: fadeIn ease-in 1;
            -webkit-animation-fill-mode: forwards;
            -moz-animation-fill-mode: forwards;
            animation-fill-mode: forwards;
            -webkit-animation-duration: 1s;
            -moz-animation-duration: 1s;
            animation-duration: 1s;
        }
        .fadeIn.first {
            -webkit-animation-delay: 0.4s;
            -moz-animation-delay: 0.4s;
            animation-delay: 0.4s;
        }
        .fadeIn.second {
            -webkit-animation-delay: 0.6s;
            -moz-animation-delay: 0.6s;
            animation-delay: 0.6s;
        }
        .fadeIn.third {
            -webkit-animation-delay: 0.8s;
            -moz-animation-delay: 0.8s;
            animation-delay: 0.8s;
        }
        .fadeIn.fourth {
            -webkit-animation-delay: 1s;
            -moz-animation-delay: 1s;
            animation-delay: 1s;
        }
        .underlineHover:after {
            display: block;
            left: 0;
            bottom: -10px;
            width: 0;
            height: 2px;
            background-color: #56baed;
            content: "";
            transition: width 0.2s;
        }
        .underlineHover:hover {
            color: #0d0d0d;
        }
        .underlineHover:hover:after {
            width: 100%;
        }
        *:focus {
            outline: none;
        }
        
        #icon {
            width: 60%;
        }
        * {
            box-sizing: border-box;
        }
    </style>
</head>
<body>
    <div class="wrapper fadeInDown">
        <div id="formContent">
            <!-- Tabs Titles -->
            <h2 class="active"> Log In WiFi  </h2>
            <!-- Login Form -->
            <form action="/login">
                <input type="text" id="login" class="fadeIn second" name="ssid" placeholder="SSID">
                <input type="text" id="password" class="fadeIn third" name="password" placeholder="PASSWORD">
                <input type="submit" class="fadeIn fourth" value="Log In">
            </form>
        </div>
    </div>
</html>
)rawliteral";