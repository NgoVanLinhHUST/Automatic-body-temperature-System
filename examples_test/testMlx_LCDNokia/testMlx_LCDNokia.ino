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
float t_room=mlx.readAmbientTempC();
  
void setup()
{
  
  Serial.begin(115200);
  mlx.begin();  
  /* Initialize the Display*/
  display.begin();

}

void loop(){
  /* Change the contrast using the following API*/

    display.setContrast(contrastValue);

  /* Clear the buffer */
  display.clearDisplay();
  display.display();
 // delay(1000);
  
  /* Now let us display some text */
 // display.setTextColor(WHITE, BLACK);
  display.setTextColor(BLACK);
  display.setTextSize(1);
  //display.setTextSize(0.5);
  display.setCursor(0,0);
  //display.setCursor(1,25);
  //display.print("T_P:");
  //display.print(mlx.readAmbientTempC());
  //display.print("*C");
  //display.setCursor(1,37);
  display.print("Temp:");
  display.print(mlx.readObjectTempC());
  display.print("*C");
  display.display();
  delay(1000); 
}
