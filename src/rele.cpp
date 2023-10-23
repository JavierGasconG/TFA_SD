
#include <Arduino.h>
#define signal_rele 2

void ini_rele() {


  
  pinMode(signal_rele, OUTPUT);  



}




void close_rele(){
    digitalWrite(signal_rele,HIGH);
}
void open_rele(){
    digitalWrite(signal_rele,LOW);
}