#include <TimerOne.h>
#include "kernel.h"

// Botões
const int button1 = 4; 
const int button2 = 3; 
// Leds
const int LED[] = {13,12,11,10,9,8,7,6,5};
//Buzzer
const int buzzer = 16;
int noteC4 = 262;
// Variáveis globais
unsigned long interval = 500UL; // Intervalo padrão
unsigned long fastInterval = 250UL; // Intervalo para velocidade aumentada
unsigned long originalInterval = 500UL; // Guardar o intervalo original para restaurar depois
int dir = 1; // Direção inicial
int pos = 4; // Posição inicial da bola
int A = 0;
int B = 0;

void setup() {
  // Criação dos Processos
  process moveBall = {moveBallProc, 2, 1};
  process checkButtons = {buttonProc, 1, 0};
  process getWinner = {checkWinner, 4, 0};

  // Inicialização dos periféricos
  Serial.begin(9600);

  // Configuração dos botões
  pinMode(button1, INPUT_PULLUP);
  pinMode(button2, INPUT_PULLUP);

  // Configuração dos LEDs
  for (int i = 0; i <= 8; i++) {
    pinMode(LED[i], OUTPUT);
  }
  pinMode(buzzer,OUTPUT);

  // Inicialização do kernel
  kernelInit();
  kernelAddProc(&moveBall);
  kernelAddProc(&checkButtons);
  kernelAddProc(&getWinner);

  // Setup do tempo do tick (microsegundos)
  Timer1.initialize(10000);
  // Definir função que executa na interrupção de Timer
  Timer1.attachInterrupt(tick);

  // Execução do kernel
  kernelLoop();
}

void tick() {
  kernelTick();  
}

char moveBallProc() {
  static unsigned long previousMillis = 0;
  unsigned long currentMillis = millis();

  if (currentMillis - previousMillis > interval) {
    digitalWrite(LED[pos], LOW);
    Serial.println("LOW");
    Serial.println(LED[pos]);
    pos += dir;

    // Verifica se a bola atingiu as extremidades e checa se há um vencedor
    if (pos == 0 || pos == 8) {
      checkWinner();
      interval = originalInterval; // Restaura a velocidade original
    }

    // Verifica se a bola atingiu a penúltima LED e acelera o movimento
    if ((pos == 1 && dir == -1) || (pos == 7 && dir == 1)) {
      interval = interval/2; // Aumenta a velocidade
      Serial.println("Velocidade aumentada!");
    }

    digitalWrite(LED[pos], HIGH);
    Serial.println("HIGH");
    Serial.println(LED[pos]);
    previousMillis = currentMillis;
  }

  return REPEAT;
}

char buttonProc() {
  int buttonState1 = digitalRead(button1);
  int buttonState2 = digitalRead(button2);

  // Verifica se o botão 1 foi pressionado e altera a direção
  if (buttonState1 == LOW && dir > 0) {
    dir = -1;
    Serial.println("Ligado");
    Serial.println(String("clicou, direcao: ") + dir);
  } else {
    Serial.println("Desligado");
  }
  
  // Verifica se o botão 2 foi pressionado e altera a direção
  if (buttonState2 == LOW && dir < 0) {
    dir = 1;
    Serial.println("Ligado");
    Serial.println(String("clicou, direcao: ") + dir);
  } else {
    Serial.println("Desligado");
  }
  
  return REPEAT;
}

char checkWinner() {
  if (pos == 0) {
    Serial.println("Jogador 2 venceu!");
    sound();
    blinkLED(0, 5);
    resetGame();
  }

  if (pos == 8) {
    Serial.println("Jogador 1 venceu!");
    sound();
    blinkLED(8, 5);
    resetGame();
  }

  return REPEAT;
}

void blinkLED(int ledIndex, int times) {
  for (int i = 0; i < times; i++) {
    digitalWrite(LED[ledIndex], HIGH);
    delay(200);
    digitalWrite(LED[ledIndex], LOW);
    delay(200);
  }
}

void sound(){
  tone(buzzer, noteC4);   
  delay(10);
  noTone(buzzer);
}

void resetGame() {
  digitalWrite(LED[pos], LOW);
  pos = 4;
  dir = 1;
  interval = originalInterval; // Restaura o intervalo original
  digitalWrite(LED[pos], HIGH);
}

void loop() {}

