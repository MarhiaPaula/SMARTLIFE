//bibliotecas
#include <DHT11.h>
#include <DHT.h>
#include <DHT_U.h>
#include <LiquidCrystal_I2C.h>
#include <Servo.h>

//Variaveis-acenderLEDAoDetectarPresenca
int PIR = 2;
const int LedVermelho = 13; 

//Variaveis-verificarVazamentodeGas()
const int MQ135 = A0;
const int buzzer = 12;


//
#define DHTPINO A1
#define DHTTIPO DHT11

DHT dht(DHTPINO, DHT11);
LiquidCrystal_I2C lcd(0x27, 20, 4);



Servo motor; //variavel do tipo servo

void acenderLEDAoDetectarPresenca(){
int estadoPIR = digitalRead(PIR);

 if(estadoPIR == HIGH){  
  digitalWrite(LedVermelho, HIGH);
  Serial.println("Led ligado");
 } else {
  digitalWrite(LedVermelho, LOW);
  Serial.println("Led desligado");
 }

}

void verificarVazamentodeGas(){
  int estadoMQ135 = analogRead(MQ135);
  
  //Serial.println(estadoMQ135);

if(estadoMQ135 >=50){
  alarme_dois_tons();
 }else {
   noTone(buzzer);
  }
}

void alarme_dois_tons() {
  int freqAlta = 2000;
  int freqBaixa = 800;
  int duracaoTom = 250;

  tone(buzzer, freqAlta, duracaoTom);
  delay(duracaoTom);
  tone(buzzer, freqBaixa, duracaoTom);
  delay(duracaoTom);
}

void verificarTemperaturaEUmidade(){
 
  float temperatura = dht.readTemperature();
  float umidade = dht.readHumidity();
  
  lcd.clear();//limpaxo lcd
  lcd.setCursor(0 ,0);
  lcd.print("Temperatura: " + String(temperatura) + "C");
  lcd.setCursor(0, 1);
  lcd.print("Umidade:" + String(umidade) + "%");

 // Serial.println(("temperatura:") + String(temperatura) + "C");
//Serial.println(("umidade:") + String(umidade) + "%");
  delay(5000);
}

void abrirPortaAutomatica(){
  motor.write(180);
  delay(1500);
}

void fecharPortaAutomatica(){
  motor.write(0);
  delay(1500);

}
 




void setup() {
  Serial.begin(9600);
  
dht.begin(); //inicializando o DHT11
lcd.init();

lcd.backlight();



  pinMode(LedVermelho, OUTPUT);
  pinMode(PIR, INPUT);
  pinMode(buzzer, OUTPUT);
  pinMode(MQ135, INPUT);
  motor.attach(3);


  //
  motor.write(0);

  Serial.println("Sensonres sendo calibrados! Aguente firme :P");
  delay(1000);
    Serial.println("Sensores calibrados! Obrigado por esperar.");

    lcd.setCursor(0, 0);
    lcd.print("Iniciando leituras..");
    lcd.setCursor(0, 1); //segunda linha
    lcd.print("Obrigado!");

}

void loop() { 
//acenderLEDAoDetectarPresenca();
//verificarVazamentodeGas();

 //alarme_dois_tons();
 verificarTemperaturaEUmidade();
 abrirPortaAutomatica();
 
}