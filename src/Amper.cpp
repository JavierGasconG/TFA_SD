

#include <SPI.h> 
#include <Arduino.h>
#define CS 4
#define MUESTRAS 100 

double offset = -1.4937; 

double get_mA( SPISettings settings) {
  SPI.beginTransaction(settings);

  double sum = 0;
  for (int i = 0; i < MUESTRAS; i++) {
    int aux = 0;
    digitalWrite(CS, LOW);      
    delay(1);
    aux = SPI.transfer(0x00);   
    aux <<= 8; 
    delay(1);
    aux |= SPI.transfer(0x00);   
    delay(1);
    digitalWrite(CS, HIGH);    
    delay(1);
    double result = aux / 4096.0 * (-3.0); 
    result = (result - offset) / 0.066; 
    sum += result;  
  }
  sum /= MUESTRAS; 
  SPI.endTransaction();
  
  return sum;
}
void ini_amper() {


  
  pinMode(CS, OUTPUT);  



}

