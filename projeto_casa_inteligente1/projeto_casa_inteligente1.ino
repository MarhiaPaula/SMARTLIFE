//bibliotecas
#include <ESP32Servo.h>

//Variaveis-acenderLEDAoDetectarPresenca

const int ledVerde = 4;
const int trigPin = 18;
const int echoPin = 19;
const int distanciaLimite = 5;
long duracao;
int distanciaCm;


//Variaveis-verificarVazamentodeGas()
const int MQ135 = 34;
const int buzzer = 5;

//Variáveis gloabias abrir e fechar trava
const int rele = 25;


//
Servo motor;  //variavel do tipo servo
const int servoMotor = 26;



void acenderLEDAoDetectarPresenca() {

  // Envia pulso de 10 microssegundos no TRIG
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  // Mede o tempo de resposta no ECHO
  duracao = pulseIn(echoPin, HIGH, 30000);  // Timeout de 30 ms para evitar travamento

  if (duracao == 0) {
    Serial.println("Sensor sem resposta ou objeto fora do alcance.");
    digitalWrite(ledVerde, LOW);
  } else {
    // Converte duração em distância (cm)
    distanciaCm = (duracao * 0.0343) / 2;

    if (distanciaCm > 0 && distanciaCm <= distanciaLimite) {
      digitalWrite(ledVerde, HIGH);
      Serial.println("Algo está se aproximando, está há: " + String(distanciaCm) + "cm");
    } else {
      digitalWrite(ledVerde, LOW);
      Serial.println("Nenhum objeto no campo de " + String(distanciaLimite) + "cm");
    }
  }

  delay(500);
}


void verificarVazamentodeGas() {
  int estadoMQ135 = analogRead(MQ135);

  //Serial.println(estadoMQ135);

  if (estadoMQ135 >= 50) {
    alarme_dois_tons();
  } else {
    noTone(buzzer);
  }
}

/*void alarme_dois_tons() {
  int freqAlta = 2000;
  int freqBaixa = 800;
  int duracaoTom = 250;

  if(motor.read() != 60){

  tone(buzzer, freqAlta, duracaoTom);
  delay(duracaoTom);
  }
  if else (motor.read() == 60)
  tone(buzzer, freqBaixa, duracaoTom);
  delay(duracaoTom);
}*/

void abrirPortaAutomatica() {
  motor.write(180);
  delay(3000);
}

void fecharPortaAutomatica() {
  motor.write(60);
  delay(3000);
}

void trancarPorta() {
  digitalWrite(rele, LOW);
  Serial.println("Porta Trancada");
  delay(3000);
}

// void destrancarPorta() {
//   String senhaDigitada;

//   Serial.println("Por gentileza, digite a senha para destravar a porta");
//   while (!Serial.available())
//     ;
//   senhaDigitada = Serial.readString();

//   if (senhaDigitada == "Senai@134") {
//     digitalWrite(rele, HIGH);
//     Serial.println("Porta Destrancada");

//     abrirPortaAutomatica();  //chamando a função que irá abrir a porta
//     trancarPorta();          //fehca a porta após 3s
//   } else {
//     if (motor.read() != 60) {
//       fecharPortaAutomatica();
//     }

//     Serial.println("Senha inválida! Na 3 tentativa irá ser bloqueada e também iremos acionar a policia, fica esperto(a).");
//   }
// }

void destrancarPorta() {
  digitalWrite(rele, HIGH);
  Serial.println("Porta Destrancada");
  delay(3000);
}

void setup() {
  Serial.begin(115200);

  pinMode(ledVerde, OUTPUT);
  pinMode(buzzer, OUTPUT);
  pinMode(MQ135, INPUT);
  motor.attach(servoMotor);
  pinMode(rele, OUTPUT);
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  pinMode(ledVerde, OUTPUT);

  //Deixa trancado a mini solonoide
  digitalWrite(rele, LOW);

  //
  motor.write(60);

  digitalWrite(trigPin, LOW);

  Serial.println("Sensonres sendo calibrados! Aguente firme :P");
  delay(1000);
  Serial.println("Sensores calibrados! Obrigado por esperar.");
}

void loop() {
  //acenderLEDAoDetectarPresenca();
  //verificarVazamentodeGas();
  //alarme_dois_tons();
  //destrancarPorta();
  //trancarPorta();
}
