#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <Keypad.h>
#include <DHT.h>

LiquidCrystal_I2C lcd(0x27, 16, 2);
#define DHTPIN 10
#define DHTTYPE DHT22
#define LDR_PIN A0
#define BUZZER_PIN 11
#define LED_VERDE 12
#define LED_VERMELHO 13

DHT dht(DHTPIN, DHTTYPE);

// Teclado 
const byte ROWS = 4; 
const byte COLS = 4; 
char keys[ROWS][COLS] = {
  {'1','2','3','A'},
  {'4','5','6','B'},
  {'7','8','9','C'},
  {'*','0','#','D'}
};
byte rowPins[ROWS] = {9, 8, 7, 6}; 
byte colPins[COLS] = {5, 4, 3, 2}; 
Keypad keypad = Keypad(makeKeymap(keys), rowPins, colPins, ROWS, COLS);

//  Estados do Sistema 
enum State { HOME, MENU_PRESET, CUSTOM_TIME, RUNNING, ALARM, HELP };
State currentState = HOME;

// Variaveis de Tempo 
unsigned long pomodoroMinutes = 25; 
long timeRemaining = 0;
unsigned long lastMillis = 0; 
bool isPaused = false;
String inputBuffer = ""; 

// Controle de Hardware 
unsigned long lastSensorRead = 0;
bool environmentWasBad = false; 
unsigned long greenLedTimer = 0; 
unsigned long buzzerTimer = 0;   
bool greenLedActive = false;
bool buzzerActive = false;

//  Alerta de limites
const int LDR_MIN = 200;   
const int LDR_MAX = 1000;  
const float TEMP_MAX = 30.0;
const float TEMP_MIN = 10.0;

void setup() {
  Serial.begin(9600);
  lcd.init();
  lcd.backlight();
  dht.begin();
  
  pinMode(BUZZER_PIN, OUTPUT);
  pinMode(LDR_PIN, INPUT);
  pinMode(LED_VERDE, OUTPUT);
  pinMode(LED_VERMELHO, OUTPUT);

  lcd.setCursor(0,0);
  lcd.print(" FocusWork 2.0  ");
  delay(1500);
  goHome();
}

void loop() {
  char key = keypad.getKey();
  
  // Verifica sensores constantemente
  checkEnvironmentAndLeds();

  switch (currentState) {
    case HOME:
      handleHome(key);
      break;
    case MENU_PRESET:
      handlePresets(key);
      break;
    case CUSTOM_TIME:
      handleCustomInput(key);
      break;
    case RUNNING:
      handleTimer(key);
      break;
    case ALARM:
      handleAlarm(key);
      break;
    case HELP:
      handleHelp(key);
      break;
  }
}

// Logicas dos Leds
void checkEnvironmentAndLeds() {
  // Atualiza leitura a cada 500ms
  if (millis() - lastSensorRead > 500) {
    float t = dht.readTemperature();
    int l = analogRead(LDR_PIN);
    
    // Verifica Limites
    bool tempBad = (t > TEMP_MAX || t < TEMP_MIN);
    bool luzBad = (l < LDR_MIN || l > LDR_MAX);
    bool isBadNow = (tempBad || luzBad);
    
    if (isBadNow) {
      // Situação de Erro
      digitalWrite(LED_VERMELHO, HIGH); // Vermelho LIGADO
      digitalWrite(LED_VERDE, LOW);     // Verde APAGADO 
      greenLedActive = false;           // Cancela contagem do verde se houver erro
      
      // Pausa sistema se estiver rodando
      if (currentState == RUNNING && !isPaused) {
         isPaused = true; 
      }

      // Buzzer toca 1 seg no início do erro
      if (!environmentWasBad) {
        digitalWrite(BUZZER_PIN, HIGH);
        buzzerTimer = millis();
        buzzerActive = true;
      }
      environmentWasBad = true; // Marca que deu erro
    } 
    else {
      // Quando estiver estavel
      digitalWrite(LED_VERMELHO, LOW); 
      digitalWrite(BUZZER_PIN, LOW);   // Desliga o Buzzer
      buzzerActive = false;

      // Detecta a recuperação 
      if (environmentWasBad) {
        greenLedActive = true;
        greenLedTimer = millis();
        digitalWrite(LED_VERDE, HIGH); // Liga Verde
        environmentWasBad = false;     // Limpa a flag de erro
      }

      // Gerencia LED Verde
      if (greenLedActive) {
        // Se passar 2 seg ele apaga
        if (millis() - greenLedTimer > 2000) {
          digitalWrite(LED_VERDE, LOW);
          greenLedActive = false;
        }
      } else {
        digitalWrite(LED_VERDE, LOW);
      }
    }
    lastSensorRead = millis();
  }

  // Timer do Buzzer 
  if (buzzerActive) {
    if (millis() - buzzerTimer > 3000) { 
      digitalWrite(BUZZER_PIN, LOW);
      buzzerActive = false;
    }
  }
}

// Telas e Navegação 

void goHome() {
  currentState = HOME;
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Tempo: "); lcd.print(pomodoroMinutes); lcd.print(" min");
  lcd.setCursor(0,1);
  lcd.print("B:Start A/C:Menu");
}

void handleHome(char key) {
  if (key == 'B') startTimer();     
  if (key == 'A') {                 
    currentState = MENU_PRESET;
    lcd.clear();
    lcd.print("1:10 2:25 3:50");
    lcd.setCursor(0,1);
    lcd.print("#:Voltar");
  }
  if (key == 'C') {                 
    currentState = CUSTOM_TIME;
    inputBuffer = "";
    lcd.clear();
    lcd.print("Digite Minutos:");
    lcd.setCursor(0,1);
  }
  if (key == 'D') {                 
    currentState = HELP;
    lcd.clear();
    lcd.print("Info Teclas:");
  }
}

void handlePresets(char key) {
  if (key == '1') { pomodoroMinutes = 10; goHome(); }
  if (key == '2') { pomodoroMinutes = 25; goHome(); }
  if (key == '3') { pomodoroMinutes = 50; goHome(); }
  if (key == '#') goHome();
}

void handleCustomInput(char key) {
  lcd.setCursor(0, 1);
  lcd.print(inputBuffer);
  lcd.print("_ "); 

  if (key >= '0' && key <= '9') {
    if (inputBuffer.length() < 2) inputBuffer += key;
  }
  else if (key == '#') { 
    if (inputBuffer.length() > 0) {
      pomodoroMinutes = inputBuffer.toInt();
    }
    goHome();
  }
  else if (key == '*') { 
    inputBuffer = "";
    lcd.setCursor(0,1); lcd.print("        ");
  }
}

void startTimer() {
  timeRemaining = pomodoroMinutes * 60;
  isPaused = false;
  currentState = RUNNING;
  lcd.clear();
}

void handleTimer(char key) {
  if (key == '*') {
    isPaused = !isPaused;
    lcd.clear();
  }
  if (key == '#') { 
    goHome();
    return;
  }

  if (isPaused) {
    lcd.setCursor(0,0);
    if (environmentWasBad) {
      lcd.print("ALERTA AMBIENTE!"); 
    } else {
      lcd.print("     PAUSADO     "); 
    }
    
    lcd.setCursor(0,1); 
    if (environmentWasBad) {
        lcd.print("Corrija o erro! ");
    } else {
        lcd.print("*:Cont  #:Parar ");
    }
    return; 
  }

  if (millis() - lastMillis >= 1000) {
    lastMillis = millis();
    if (timeRemaining > 0) timeRemaining--;
    else {
      currentState = ALARM;
      lcd.clear();
    }
  }

  int m = timeRemaining / 60;
  int s = timeRemaining % 60;
  lcd.setCursor(0,0);
  lcd.print("Foco! ");
  if (m<10) lcd.print("0"); lcd.print(m); lcd.print(":");
  if (s<10) lcd.print("0"); lcd.print(s);
  
  lcd.setCursor(0,1);
  lcd.print("*:Pausa #:Parar"); 
}

void handleAlarm(char key) {
  lcd.setCursor(0,0); lcd.print("ACABOU O TEMPO!");
  lcd.setCursor(0,1); lcd.print("#:Parar o Alarme");
  
  tone(BUZZER_PIN, 1500); 
  
  if (key == '#') {
    noTone(BUZZER_PIN);
    goHome();
  }
}

void handleHelp(char key) {
  lcd.setCursor(0,0); lcd.print("A:Menu C:Definir");
  lcd.setCursor(0,1); lcd.print("*:Pausa #:Sair");
  if (key == '#') goHome();
}