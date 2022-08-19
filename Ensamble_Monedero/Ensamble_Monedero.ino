#include <WiFi.h>
#include <ArduinoWebsockets.h>
#include "FS.h"
#include "SD.h"
#include "SPI.h"
#include <LiquidCrystal_I2C.h>
//CCTALK
#define ALTSOFTCOMM
#ifdef ALTSOFTCOMM
#endif

#include "cctcom.h"
#include "headers.h"

const char* ssid = "RED ACCESA"; //"RED ACCESA"
const char* password = "037E32E7"; //"037E32E7"
const char* websockets_server_host = "192.168.1.61"; //Enter server adress
const uint16_t websockets_server_port = 81; // Enter server port
using namespace websockets;
WebsocketsClient client;
//WEBSOCKET
int EnableSendStatus;
int wsOK, EMensajeWS;

LiquidCrystal_I2C lcd(0x27, 20, 4); 

//15-Cobro, 33-IntContador, 32-Corte, 34-SM sup, 35 SM inf, 26 WakeupContador, 27 wakeupSensor
int Entradas[7] = {15, 33, 32, 34, 35, 26, 27};
int AntEntrada[7];
int EnableEntrada[7];

//ReleImpresora, ReleTorniquete, Buzzer
int Salidas[3] = {14, 13, 25};

//CCTALK
unsigned int CoinValor;
const float Monedas_Aceptadas[6] = { 1.0, 2.0, 5.0, 10.0, 20.0, 0.5};
unsigned int coinValorArray[6] = {10, 20, 50, 100, 470, 4294967146};
String M[6] = {"1","2","5","10","20","0.5"}; 
String ValorCoinLCD;
int ValorDesconexion, ValorError, EnableValor;
// some garbage , without it my compiler gives a fatal error , you can skip this if the sketch compiles.
long weqwerqr; long xxxx[1]; long sdasdasd[1]; long sdasdasa[1]; long sdasdase[1];
///
milistimer somedelay;// to handle the break after a comm error
unsigned int coin_value[16] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}; // to store the coin value

//SD
const int intervalSD = 3000;
unsigned long previousSD;
String mensaje_SD;
int EnableSD, AntSD;

//COMANDO POST IMPRESORA TERMICA
int Folio_Boleto_Pasajero;
String Cel_Reporte, Cel_ventas; 
int SinPapel;
int enableBoleto;

#define ESTADO_PAPEL  4
#define ESTADO_PRINT  1
#define CAUSA_OFFLINE 2
#define TIPO_ERROR    3
#define RXD1 36
#define TXD1 4

char IMPT_INIT[] = {27, 64, 0};       //BORRA BUFER Y RESETEA AJUSTES DE IMPRESION
char IMPT_MODO[] = {27, 33, 0};         //MODO DE IMPRESION, ***
char IMPT_ESP_PROG[] = {27, 51, 0};   //ESPACIADO ENTRE LINEAS n[0-255], MAX 1016mm
char IMPT_JUST[] = {27, 97, 0};       //JUSTIFICACION IZQ,CENT,DER n[0-2 || 48-50]  
char IMPT_SIZE_CHAR[] = {29, 33, 0};  //ALTURA POR ANCHO DEL CARACTER n = swap[0-7, 0-7]
char IMPT_NEGRITA[] = {27, 69, 0};    //ACTIVA O DESACTIVA LA LETRA NEGRITA n[0,1]
char IMPT_CORTE_POS[] = {29, 86, 0};  //POSICION DEL CORTE
char IMPT_STAT[] = {16, 4, 0};   //POSICION DEL CORTE
char CUT_POS_OFFSET = 66;        //CORTE PARCIAL CON OFFSET
char CUT_POS_ACT = 49;           //CORTA EN LA POSICION ACTUAL
char FONTB = 0x01;
char FONTA = 0x00;
char interlineado = 40;
 
char JUST_LEFT  = 0;
char JUST_CENTER = 1;


void setup(){
  Serial.begin(115200);
  Serial1.begin(38400, SERIAL_8N1, RXD1, TXD1);

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
  
  
  for(int i=0; i<7; i++){
    pinMode(Entradas[i], INPUT);
  }
  for(int i=0; i<3; i++){
    pinMode(Salidas[i], OUTPUT);
  }

  WiFi.begin(ssid, password);
  while(WiFi.status() != WL_CONNECTED) {
    delay(200);
  }
  Serial.println("WiFi conectado");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP()); 

  conexion_WS();
  
  TaskHandle_t LCD;
  xTaskCreatePinnedToCore(
  Task2code, "LCD" , 8192, NULL, 1 ,&LCD, 0 ); 
  
  TaskHandle_t CORE0; 
  xTaskCreatePinnedToCore(
  Task1code, "CORE0" , 16834, NULL, 1 ,&CORE0, 0 ); // CONFIGURA TAREA NUCLEO 0//

  TaskHandle_t CCTALK;
  xTaskCreatePinnedToCore(
  Task3code, "CCTALK" , 6144, NULL, 1 ,&CCTALK, 0 ); //4096

  TaskHandle_t WS;
  xTaskCreatePinnedToCore(
  Task4code, "WS" , 4096, NULL, 1 ,&WS, 0 ); //4096

   //digitalWrite(Salidas[0], HIGH);//DESABILITADO DEBIDO A QUE LA PLACA TIENE EL RELEVADOR EN NC
  //INICIALIZA IMPRESORA
  iniciaImpresora();
  Serial.println("Estado Impresora: " + String(obtenerEstado(ESTADO_PRINT)));
  
}

void loop(){
  for(int l=0; l<7; l++ ){
    if(digitalRead(Entradas[l])==HIGH && !EnableEntrada[l]){
      EnableEntrada[l] = 1;
    }else if(digitalRead(Entradas[l])==LOW && EnableEntrada[l]){
      EnableEntrada[l] = 0;
    }
  }

    if(EnableEntrada[2]){
      digitalWrite(Salidas[1], HIGH);
    }else{
      digitalWrite(Salidas[1], LOW);
    }

    if(EnableEntrada[1] && !enableBoleto){
      enableBoleto =1;
      BoletoAlcancia();
      delay(1000);
      ImprimeCorte();
      Serial.println("IMPRIMIO");
    }else if (!EnableEntrada[1] && enableBoleto){
      enableBoleto =0;
      delay(1000);
    }
}

void Task1code( void * pvParameters ){
  while(1){
 
    if((millis() - previousSD >= intervalSD)){
      previousSD = millis();
      appendFile(SD, "/TEST.txt", "TEST CONTADORES ESP32");
      readFile(SD, "/TEST.txt");
      deleteFile(SD, "/TEST.txt");
  
      if(mensaje_SD == "TEST CONTADORES ESP32" ){
        EnableSD =1;
        vTaskDelay(1000);
        EnableSD =0;
      }

     EnableSendStatus = 1; //HABILITA ENVIO DE MENSAJE POR WS
    }
    vTaskDelay(10);
  }
}


void Task2code( void * pvParameters ){
//15-Cobro, 33-IntContador, 32-Corte, 34-SM sup, 35 SM inf, 26 WakeupContador, 27 wakeupSensor
  const int xt[7] = {0,9,0,9,0,9,0};
  const int yt[7] = {0,0,1,1,2,2,3};
  const int xv[7] = {7,16,7,16,7,16,7};
  const int yv[7] = {0,0,1,1,2,2,3};
  
  String titulo[7] = {"Cobro", "Relay", "Corte","senMU", "senMD", "WakeUp", "Prese"};
  delay(500);
  lcd.init(); 
  lcd.backlight();
  for(int l=0; l<7; l++){
    lcd.setCursor (xt[l],yt[l]);
    lcd.print(titulo[l]);
    lcd.setCursor (xv[l],yv[l]);
    lcd.print(EnableEntrada[l]);
  }
    lcd.setCursor (10,3);
    lcd.print("SD");
    lcd.setCursor (16,3);
    lcd.print("$");
  while(1){
    for(int l=0; l<7; l++){
      if(AntEntrada[l] != EnableEntrada[l]){
        AntEntrada[l] = EnableEntrada[l];
        lcd.setCursor (xv[l],yv[l]);
        lcd.print(EnableEntrada[l]);
      }
      if(AntSD != EnableSD){
         AntSD = EnableSD;
         if(EnableSD){
           lcd.setCursor (13,3);
           lcd.print("OK");
         }else{
           lcd.setCursor (13,3);
           lcd.print("  ");
         }  
      }
      if(EnableValor){
        EnableValor=0;
        lcd.setCursor (17,3);
        lcd.print("   ");        
        lcd.setCursor (17,3);
        lcd.print(ValorCoinLCD);
      }
      if(wsOK){  
        /*lcd.setCursor (19,0);
        lcd.print("w");*/
        digitalWrite(Salidas[2], HIGH); 
      }else{     
        digitalWrite(Salidas[2], LOW); 
      }
      
    }
    vTaskDelay(10);
  }
}

void Task3code( void * pvParameters ){

  Serial.println("inicia cctalk");
  ccTalk_comm_init();
  while ( RX_state != RXidle) {
    ccTalkReceive();
  }
  Serial.println("cctalk ok");
  while(1){
    Serial.println("Entrando a loop cctalk");
    cctalk_loop();
    vTaskDelay(1);
  }
}

void Task4code( void * pvParameters ){
  while(1){
    webSocketLoop();
    vTaskDelay(100);
  }
}
