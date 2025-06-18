  //bibliotecas
  #include <ESP32Servo.h>
  #include <WiFi.h>
  #include <PubSubClient.h>


    // --- WiFi & MQTT ---
    const char* ssid = "ProjetoIoT_Esp32";//sua rede wifi
    const char* password = "Sen@i134";//senha da sua rede wifi
    const char* mqtt_server = "172.16.39.79";//endereço do broker público
    const int mqtt_port = 1883;//porta do broker público, geralmente 1883

    //Tópicos
    const char* topic_led = "smartlife/sala/luzsala";
    const char* topic_porta = "smartlife/sala/porta";
    const char* topic_trava = "smartlife/sala/trava";
    const char* topic_buzzer = "smartlife/sala/buzzer";

    WiFiClient espClient;
    PubSubClient client(espClient);


    

  //Variaveis-acenderLEDAoDetectarPresenca

  const int ledVerde = 4;
  const int trigPin = 18;
  const int echoPin = 19;
  const int distanciaLimite = 5;
  long duracao;
  int distanciaCm;


  //Variaveis-verificarVazamentodeGas()
  const int buzzer = 4;

  //Variáveis gloabias abrir e fechar trava
  const int rele = 25;


  //
  Servo motor;  //variavel do tipo servo
  const int servoMotor = 26;


// --- Funções WiFi e MQTT ---
void conectarWiFi() {//verifica conexão wifi para somente depois iniciar o sistema
  Serial.println("Conectando ao WiFi...");
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nWiFi conectado!");
}

void reconectarMQTT() {//verifica e reconecta a conexão com o broker mqtt
  while (!client.connected()) {
    Serial.print("Reconectando MQTT...");
    if (client.connect("ESP32ClientTest")) {
      Serial.println("Conectado!");
      client.subscribe(topic_led);//conecta ao topico do led assim que estabelecer ligação com o broker
      client.subscribe(topic_porta);//conecta ao topico da porta assim que estabelecer ligação com o broker
    } else {
      Serial.print("Falha: ");
      Serial.println(client.state());
      delay(5000);
    }
  }
}

void tratarMensagem(char* topic, byte* payload, unsigned int length) {//
  String mensagem = "";
  for (int i = 0; i < length; i++) {//concatena todas os char* para se ter o texto completo em String
    mensagem += (char)payload[i];
  }

  Serial.printf("Mensagem recebida [%s]: %s\n", topic, mensagem.c_str());
  
  //led - luz da sala
  if (strcmp(topic, topic_led) == 0) {//tópico atual é o do led?
    if (mensagem == "ligar") {
      digitalWrite(luzSala, HIGH);
    } else if (mensagem == "desligar") {
      digitalWrite(luzSala, LOW);
    }
  }
  
  /*
    Verifica se o tópico recebido é o topico da porta
  é uma função da linguagem C que compara duas strings (topic e topic_porta)
  */
  //porta
  if (strcmp(topic, topic_porta) == 0) {//tópico atual é o da porta?
    if (mensagem == "abrir") {
      destrancarPorta();
      delay(500);
      abrirPortaAutomatico();
    } else if (mensagem == "fechar") {
      fecharPortaAutomatico();
      delay(500);
      trancarPorta();
    }
  }
}



 void acenderLEDAoDetectarPresenca() {
  // Envia pulso de 10 microssegundos no TRIG
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  // Mede o tempo de resposta no ECHO (timeout de 30ms)
  duracao = pulseIn(echoPin, HIGH, 30000);

  if (duracao == 0) {
    Serial.println("Sensor sem resposta ou objeto fora do alcance.");
    digitalWrite(ledVerde, LOW);
  } else {
    // Converte duração em distância (cm)
    distanciaCm = (duracao * 0.0343) / 2;

    // Verifica se há objeto dentro do limite
    if (distanciaCm >= 2 && distanciaCm <= distanciaLimite) {
      digitalWrite(ledVerde, HIGH);
      Serial.println("Algo está se aproximando, está há: " + String(distanciaCm) + "cm");
    } else {
      digitalWrite(ledVerde, LOW);
      Serial.println("Nenhum objeto no campo de " + String(distanciaLimite) + "cm");
    }
  }

  delay(500);
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

    //digitalWrite(trigPin, LOW);

    conectarWiFi();//conecta no wifi
    client.setServer(mqtt_server, mqtt_port);//conecta no broker server
    client.setCallback(tratarMensagem);//trata as mensagens recebidas do broker

    Serial.println("Sistema iniciado!");
  }

  void loop() {
    //acenderLEDAoDetectarPresenca();
    //alarme_dois_tons();
    //destrancarPorta();
    //trancarPorta();
  }
