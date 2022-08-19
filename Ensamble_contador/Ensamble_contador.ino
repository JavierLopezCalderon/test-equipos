#include <WiFi.h>
#include <ArduinoWebsockets.h>
#include <Ticker.h>
#include "FS.h"
#include "SD.h"
#include "SPI.h"
#include <LiquidCrystal_I2C.h>

const char* ssid = "RED ACCESA"; //"RED ACCESA"
const char* password = "037E32E7"; //"037E32E7"
const char* websockets_server_host = "192.168.1.61"; //Enter server adress
const uint16_t websockets_server_port = 81; // Enter server port
using namespace websockets;
WebsocketsClient client;

//SALIDAS
int IndicadoresGenerales[3] = {12, 14, 13}; //12-WIFI, 14-Actividad, 13-relevador 
int IndicadoresSensores[3] = {2, 4, 16};
int LedSenalModulada = 15;
//ENTRADAS
int Reset = 27;
int EntradasSensores[2] = {36, 39}; //(36 Y 39)SENSOR MAG 
//const int SensoresInfrarojos[6] = {25, 26, 32, 33, 34, 35}; //PINES DE SENSORES

//SEÑAL MODULADA
int countLecturaSM[2];
const int freq = 38000;//38kHz   //50000//50KHz
const int ledChannel = 0;
const int resolution = 8;

//VARIABLES SENSORES INFRAROJOS
class Sensor_var{   
  public:
    const int Pin[6] = {25, 26, 32, 33, 34, 35}; //PINES DE SENSORES
    int Estado, Lectura, Count;
};
Sensor_var Sensor[6];
Ticker Ticker1;
const int limit = 5;
const int timer_interrupcion = 5;
int ReleEnable;

//SD
const int intervalSD = 3000;
unsigned long previousSD;
String mensaje_SD;
int EnableSD;

//WEBSOCKET
int EnableSendStatus;

LiquidCrystal_I2C lcd(0x27, 20, 4); 
 
void setup() {
  Serial.begin(115200);
  
  if(!SD.begin()){
        Serial.println("Card Mount Failed");
        ESP.restart();
    }
    uint8_t cardType = SD.cardType();

    if(cardType == CARD_NONE){
        Serial.println("No SD card attached");
        return;
    }

    Serial.print("SD Card Type: ");
    if(cardType == CARD_MMC){
        Serial.println("MMC");
    } else if(cardType == CARD_SD){
        Serial.println("SDSC");
    } else if(cardType == CARD_SDHC){
        Serial.println("SDHC");
    } else {
        Serial.println("UNKNOWN");
    }

  uint64_t cardSize = SD.cardSize() / (1024 * 1024);
  Serial.printf("SD Card Size: %lluMB\n", cardSize);
  
  for(int i=0; i<3; i++){
    pinMode(IndicadoresGenerales[i], OUTPUT);
    pinMode(IndicadoresSensores[i], OUTPUT);
  }
  pinMode(Reset, INPUT_PULLUP);
  for(int i=0; i<6; i++){
    pinMode(Sensor[i].Pin[i], INPUT);
  }
  digitalWrite(IndicadoresGenerales[0], LOW);

  WiFi.begin(ssid, password);
  while(WiFi.status() != WL_CONNECTED) {
    delay(200);
  }
  Serial.println("WiFi conectado");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP()); 

  conexion_WS();

  Ticker1.attach_ms(timer_interrupcion, Lecura_sensor); //CONFIGURA TIMER_INTERRUPCIONES //

  // PROCESO DE ENVIO DE SEÑAL PWM //
  ledcSetup(ledChannel, freq, resolution);
  ledcAttachPin(LedSenalModulada, ledChannel);
  ledcWrite(ledChannel, 179);//75% Ancho de pulso//191//166//204

  TaskHandle_t CORE0; 
  xTaskCreatePinnedToCore(
  Task1code, "CORE0" , 16834, NULL, 1 ,&CORE0, 0 ); // CONFIGURA TAREA NUCLEO 0//

  TaskHandle_t LCD;
  xTaskCreatePinnedToCore(
  Task2code, "LCD" , 16834, NULL, 1 ,&LCD, 0 ); 

  TaskHandle_t SM;
  xTaskCreatePinnedToCore(
  Task3code, "SM" , 8000, NULL, 1 ,&SM, 0 );
   
}

void loop() {
  
  if(digitalRead(Reset) == LOW){
    digitalWrite(IndicadoresGenerales[1], HIGH);
  }else{
    digitalWrite(IndicadoresGenerales[1], LOW);
  }
  SensoresInfrarrojos();
}

void Task1code( void * pvParameters ){
  while(1){
 
    if((millis() - previousSD >= intervalSD)&& EnableSD){
      previousSD = millis();
      appendFile(SD, "/TEST.txt", "TEST CONTADORES ESP32");
      readFile(SD, "/TEST.txt");
      deleteFile(SD, "/TEST.txt");
  
      if(mensaje_SD == "TEST CONTADORES ESP32" && EnableSD){
        digitalWrite(IndicadoresGenerales[2], HIGH);
        Serial.println("SD OK");
        vTaskDelay(500);
        digitalWrite(IndicadoresGenerales[2], LOW);
      }

      EnableSendStatus = 1; //HABILITA ENVIO DE MENSAJE POR WS
    }
    vTaskDelay(10);
  }
}

void Task2code( void * pvParameters ){
  lcd.init();
  lcd.backlight();
  lcd.setCursor (0,1);
  lcd.print("TEST CONTADOR");
  lcd.setCursor (0,2);
  lcd.print("ESP32");
  
  while(1){
    webSocketLoop();
    vTaskDelay(10);
  }
}

void Task3code( void * pvParameters ){
  while(1){
    Lectura_Sensores_Magneticos();
    
    vTaskDelay(10);
  }
}

void Lectura_Sensores_Magneticos(){
   
   if ( analogRead(EntradasSensores[1]) < 1500){
      if(countLecturaSM[1] > 20){
        countLecturaSM[1] = 25;
      }else{
        countLecturaSM[1]++;
      }
   }else if (analogRead(EntradasSensores[1]) >= 1500 ){
      countLecturaSM[1] = 0;
   }

   if ( analogRead(EntradasSensores[0]) < 1800){
      if(countLecturaSM[0] > 20){
        countLecturaSM[0] = 25;
      }else{
        countLecturaSM[0]++;
      }
   }else if (analogRead(EntradasSensores[0]) >= 1800 ){
      countLecturaSM[0] = 0;
   }
   
   //Serial.println("SM1: " + String(countLecturaSM[0]) + "SM2: " + String(countLecturaSM[1]));

   if(countLecturaSM[0] < 20 && ReleEnable == 0){
      EnableSD = 0;
      digitalWrite(IndicadoresGenerales[2], HIGH);
      ReleEnable = 1;
      //Serial.println("S1 OK");
   } 
   if(countLecturaSM[1] < 20 && ReleEnable == 1){
      EnableSD = 1;
      digitalWrite(IndicadoresGenerales[2], LOW);
      ReleEnable = 0;
      //Serial.println("S1 OK");
   }

}

void Lecura_sensor() {
  for(int i=0; i<6; i++){
    Sensor[i].Lectura = digitalRead(Sensor[i].Pin[i]);
    if(Sensor[i].Lectura == 0){
      Sensor[i].Count++;
    }
  }
}

void SensoresInfrarrojos(){
  for(int y=0; y<6; y++){
    if((digitalRead(Sensor[y].Pin[y])) == 1){     
      Sensor[y].Estado = 0;
      Sensor[y].Count = 0;
    }else{
      if( Sensor[y].Count >= limit){
        Sensor[y].Estado = 1;     
      }
    }
  }

  /*for(int i=0; i<6; i++){
    Serial.print("S"); 
    Serial.print(i+1); 
    Serial.print(":");
    Serial.print(Sensor[i].Estado);
    Serial.print(" ");
  } 
  Serial.println("");*/

  int num_LED = 0, num_LED2 = 0;  
  for(int i=0; i<3; i++){ 
    num_LED = num_LED + 2;
    num_LED2 = num_LED - 1;
    
    if(Sensor[num_LED2-1].Estado == 0 && Sensor[num_LED-1].Estado == 0 ){
      digitalWrite(IndicadoresSensores[i], HIGH); 
    }else{
      digitalWrite(IndicadoresSensores[i], LOW);
    } 
  }
}

void conexion_WS(){
  // Connect to the websocket server
  bool connected  = client.connect(websockets_server_host, websockets_server_port, "/");
  
  if (connected) {
     digitalWrite(IndicadoresGenerales[0], HIGH);
     Serial.println("Connected!");
     client.send("HOLA, ESTE ES UN MENSAJE DE WEBSOCKET DE TESTEO PARA CONTADOR");
  } else {
    Serial.println("Not Connected!");
    digitalWrite(IndicadoresGenerales[0], LOW);
    //ws_desconectado = 1;
  }

   // run callback when messages are received
   client.onMessage([&](WebsocketsMessage message){
     Serial.print("Got Message: ");
     Serial.println(message.data());
   });
}

void webSocketLoop(){
  
  if (client.available()) {
    client.poll();
    
      //ENVIA STATUS 
      if(EnableSendStatus){
        client.send("HOLA, ESTE ES UN MENSAJE DE TESTEO PARA CONTADOR");
        Serial.println("ENVIO MENSAJE");
        EnableSendStatus = 0;
      }   
  } else {
    digitalWrite(IndicadoresGenerales[0], LOW);
    bool connected = client.connect(websockets_server_host, websockets_server_port, "/");

    if(connected) {
      Serial.println("Connected!");
      digitalWrite(IndicadoresGenerales[0], HIGH);;
    } else {
      digitalWrite(IndicadoresGenerales[0], LOW);
    }
  }
}
