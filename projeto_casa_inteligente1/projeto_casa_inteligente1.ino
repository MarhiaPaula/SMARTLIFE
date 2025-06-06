//Variáveis Detector de presença
const int ledAmarelo = 13; // pino do led
const int PIR = 2; // pino do pir

//Variáveis detectação do gás
const int mq135 = A0;
const int buzzer = 12;
int contador = 0;

void acenderLEDAoDetectarPresenca(){
  int estadoPIR = digitalRead(PIR);
  
  if(estadoPIR == HIGH){
	digitalWrite(ledAmarelo, HIGH);
	Serial.println("Movimento detectado! LED aceso.");
	delay(5000);
  }
  else {
   digitalWrite(ledAmarelo, LOW);
   Serial.println("Sem movimento. LED apagado.");
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

void verificarVazamentoDeGas(){
  int estadoMQ135 = analogRead(mq135);
  
  if (estadoMQ135 > 700) {
    while (contador == 0) {
	 Serial.println("Detectamos que ha vazamento de gas, iremos acionar o alarme em:");
	 Serial.println("3");
	 delay(1000);
	 Serial.println("2");
	 delay(1000);
	 Serial.println("1");
	 delay(1000);
	 contador = 1;
  }
    
    
	alarme_dois_tons();
  }
	else {
		if (contador == 1) {
			contador = 0;
			noTone(buzzer); // Para o som
			delay(10000);
		}
      
      
Serial.println("Ufa!!! Nao ha detecção de gas no ambiente.");
	}
}

void setup()
{
  Serial.begin(9600);
  pinMode(ledAmarelo, OUTPUT);
  pinMode(PIR, INPUT);
  
  pinMode(mq135, INPUT);
  pinMode(buzzer, OUTPUT);

  Serial.println("Calibrando o sensor PIR...");
  Serial.println("Por favor, nao se mova na frente do sensor.");
  delay(5000);
  Serial.println("Sensor PIR calibrado e pronto!");
}

void loop()
{
  acenderLEDAoDetectarPresenca();
  verificarVazamentoDeGas();
}