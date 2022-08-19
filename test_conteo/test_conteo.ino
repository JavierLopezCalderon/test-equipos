//LIBRERIA LCD
#include <LiquidCrystal_I2C.h>
//LIBRERIA TIMER
#include <Ticker.h>
//LIBRERIA MEMORIA
#include <Preferences.h>

//VARIABLES
#include "Variables.h"

void setup(){
  Serial.begin(115200);
  preferences.begin("Var_Flash", false);

  readFileCONTADORES();

  //DECLARA LOS SEIS PINES DE LOS SENSORES COMO ENTRADAS
  for(int i=0; i<6; i++){
    pinMode(Sensor[i].Pin[i], INPUT);
  }
  //DECLARA ENTRADAS Y SALIDAS
  pinMode(PinBotonReset, INPUT_PULLUP);
  
  for(int i=0; i<3; i++){
    pinMode(LedIndicadorSensores[i],OUTPUT);
  }
  pinMode(PinBotonReset, INPUT_PULLUP);
  pinMode(Relevador, OUTPUT);
  
  Ticker1.attach_ms(timer_interrupcion, Lecura_sensor); //CONFIGURA TIMER_INTERRUPCIONES //

  // PROCESO DE ENVIO DE SEÑAL PWM //
  ledcSetup(ledChannel, freq, resolution);
  ledcAttachPin(ledPin, ledChannel);
  ledcWrite(ledChannel, 179);//75% Ancho de pulso//191//166//204
  //Estable con 166 %65//179 70%

  // INSTANCIA TAREA
  TaskHandle_t CORE0; 
  TaskHandle_t LCD;
  TaskHandle_t CORE0T2;

  xTaskCreatePinnedToCore(
  Task1code, "CORE0" , 65536, NULL, 2 ,&CORE0, 0 ); // CONFIGURA TAREA NUCLEO 0//
  xTaskCreatePinnedToCore(
  Task3code, "LCD" , 2048, NULL, 3 ,&LCD, 0 );
  xTaskCreatePinnedToCore(
  Task5code, "CORE0T2" , 2048, NULL, 2 ,&CORE0T2, 0 );  
}

void loop(){

//LOGICA DE SENSADO

  for(int y=0; y<6; y++){
    if((digitalRead(Sensor[y].Pin[y])) == 1){     
      Sensor[y].Estado = 0;
      Sensor[y].Count = 0;
      Sensor[y].previousMillisBloqueo = millis();
      Sensor[y].Bloqueo = 0; //Quita bloqueo de sensor//aqui inicia registro de cuando se desploquea sensor
      Sensor[y].HabilitarRegistroBloqueo = 0;
      Sensor[y].EnableB = 0;
    }else{
      if( Sensor[y].Count >= limit){
        Sensor[y].Estado = 1;
        if(Sensor[y].EnableB == 0){
          Sensor[y].previousMillisBloqueo = millis();
          Sensor[y].EnableB = 1;
        }     
      }
    }
  }

  //TIMER PARA HABILITAR SUMA INDIVIDUAL
  for(int i=0; i<3; i++){
    if((TimerCurrient-Linea[i].previousMillisSensado>=intervalEnable)&& Linea[i].Enable == 1){
      Linea[i].Enable = 0;
    }
  }
  logica_lineas();//REVISA SI SE CUMPLIO EL PROCESO PARA CONTAR UNA PERSONA 
  
  Suma(); //LOGICA PARA SUMAR AL CONTADOR

  /***Print en monitor serial para hacer pruebas de sensado***/
  /*for(int i=0; i<6; i++){
    Serial.print("S"); 
    Serial.print(i+1); 
    Serial.print(":");
    Serial.print(Sensor[i].Estado);
    Serial.print(" ");
  } 
  Serial.println("");*/
  
}

void Task1code( void * pvParameters ){
  while(1){
   //GUARDA EN LA MEMORIA MICROSD EL CONTEO DE PERSONAS
    if (enableSaveCount ==  1){
        sd_saveFileCONTADORES();
        enableSaveCount =0;
        Serial.println("******************GUARDA EL CONTEO*******************");
        Serial.println("Subidas: " + String(CountSubida) + " Bajadas: "+ String(CountBajada));
    }

    Bloqueo_sensores();
    Bloqueo_Total();

    //GUARDA EL REGISTRO DEL BLOQUEO DE LOS SENSORES 
    for(int x=0; x<6; x++){
       if(Sensor[x].GuardarRegistroBloqueo == 0 && Sensor[x].HabilitarRegistroBloqueo == 1){
          Sensor[x].GuardarRegistroBloqueo = 1;
          CountSensoresBloqueo[x]++; //SUMA CONTEO DE BLOQUEOS SENSORES
          sd_saveFileCONTADORES();
          //preferences.putUInt(File_Estado_Bloqueo_Sensor[x], Sensor[x].GuardarRegistroBloqueo); 
          //Serial.println("Guardar Registro Sensor Bloqueo: "+String(Sensor[x].GuardarRegistroBloqueo));
          //Serial.println("sensor" + String(x+1) + ": intenta hacer registro");
          //AQUI GUARDA EL REGISTRO SENSORES
          //SerializarDatosTiempoReal();
          //PartRegistroBloqueoSensor[x]= 1;
          //preferences.putUInt(File_Part_Bloqueo_Sensor[x], PartRegistroBloqueoSensor[x]); 
          //Serial.println("Parte de registro de Sensor Bloqueo: "+ String(PartRegistroBloqueoSensor[x]));
          //registro_bloqueos_Sensores(x);
       }
    }
    //GUARDA EL REGISTRO DEL BLOQUEO DE LAS LINEAS
    for(int i=0; i<3; i++){
      //SaveJ = i;
      if(Linea[i].GuardarRegistroBloqueo == 0 && Linea[i].HabilitarRegistroBloqueo == 1){
        Linea[i].GuardarRegistroBloqueo = 1;
        CountLineaBloqueos[i]++; //SUMA CONTEO DE BLOQUEOS LINEAS
        sd_saveFileCONTADORES();
        //SerializarDatosTiempoReal();
        /*preferences.putUInt(File_Estado_Bloqueo_Linea[i], Linea[i].GuardarRegistroBloqueo); Serial.println("Guardar Registro linea Bloqueo: "+String(Linea[i].GuardarRegistroBloqueo));
        Serial.println("Linea" + String(i+1) + ": intenta hacer registro");
        PartRegistroBloqueoLinea[i] = 1;
        preferences.putUInt(File_Part_Bloqueo_Linea[i], PartRegistroBloqueoLinea[i]); Serial.println("Parte de registro de linea Bloqueo: "+String(PartRegistroBloqueoLinea[i]));
        registro_bloqueos_Lineas(i);*/
      }
    }
    //GUARDA EL REGISTRO DE BLOQUEOS TOTALES
    if(GuardarRegistroBloqueoTotal == 0 && HabilitarRegistroBloqueoTotal == 1 ){
      GuardarRegistroBloqueoTotal=1;
      CountBloqueoTotal++; //SUMA CONTEO DE BLOQUEOS TOTALES
      sd_saveFileCONTADORES();
      //SerializarDatosTiempoReal();
      /*preferences.putUInt(File_Estado_Bloqueo_Total, GuardarRegistroBloqueoTotal); Serial.println("Guardar Registro Total Bloqueo: "+String(GuardarRegistroBloqueoTotal));
      PartRegistroBloqueoTotal= 1;
      preferences.putUInt(File_Part_Bloqueo_Total, PartRegistroBloqueoTotal); Serial.println("Parte de registro de Total Bloqueo: "+String(PartRegistroBloqueoTotal));
      Serial.println("Bloqueo total: intenta hacer registro");
      registro_bloqueos_totales();*/
    }

    //RESET BLOQUEO SENSORES
    /*for(int n=0; n<6; n++){
      if(Sensor[n].Bloqueo == 0){
        if(Sensor[n].GuardarRegistroBloqueo == 1 && PartRegistroBloqueoSensor[n]== 1){
          SerializarDatosTiempoReal();
          Serial.println("**************Entra para guardar la segunda parte**************");
          PartRegistroBloqueoSensor[n] = 2;
          registro_bloqueos_Sensores(n);
        }
        if(PartRegistroBloqueoSensor[n] != 0){
          Serial.println("Regresa a 0 variables sensor");
          PartRegistroBloqueoSensor[n] = 0;
          preferences.putUInt(File_Part_Bloqueo_Sensor[n], PartRegistroBloqueoSensor[n]); Serial.println("Parte de registro de Sensor Bloqueo: "+String(PartRegistroBloqueoSensor[n]));
          Sensor[n].GuardarRegistroBloqueo = 0;
          preferences.putUInt(File_Estado_Bloqueo_Sensor[n], Sensor[n].GuardarRegistroBloqueo); Serial.println("Guardar Registro Sensor Bloqueo: "+String(Sensor[n].GuardarRegistroBloqueo));  
        }     
      }
    }*/

    //RESET BLOQUEO DE LINEA Y TOTAL
    int num_Loop = 0, num_Loop2 = 0;
    for(int i=0; i<3; i++){
      num_Loop = num_Loop + 2;
      num_Loop2 = num_Loop - 1;
      if((Sensor[num_Loop2-1].Estado == 0 || Sensor[num_Loop-1].Estado == 0)&&(Sensor[num_Loop2-1].GuardarRegistroBloqueo == 0 || Sensor[num_Loop-1].GuardarRegistroBloqueo == 0)){
        if(Linea[i].Bloqueo == 1){
          Linea[i].Bloqueo = 0;
          
          //SerializarDatosTiempoReal();
          /*if(Linea[i].GuardarRegistroBloqueo == 1 && PartRegistroBloqueoLinea[i]== 1){
            Serial.println("************intenta guardar parte 2 de bloqueo linea***********");
            PartRegistroBloqueoLinea[i]= 2;
            registro_bloqueos_Lineas(i);
          }*/
          /*Linea[i].HabilitarRegistroBloqueo = 0;
          Linea[i].GuardarRegistroBloqueo = 0;
          PartRegistroBloqueoLinea[i] = 0;
          preferences.putUInt(File_Part_Bloqueo_Linea[i], PartRegistroBloqueoLinea[i]); Serial.println("Parte de registro de Linea Bloqueo: "+String(PartRegistroBloqueoLinea[i]));
          Linea[i].GuardarRegistroBloqueo = 0;
          preferences.putUInt(File_Estado_Bloqueo_Linea[i], Linea[i].GuardarRegistroBloqueo); Serial.println("Guardar Registro Linea Bloqueo: "+String(Linea[i].GuardarRegistroBloqueo));
          */
          BloqueoTotal =0;
          /*if(GuardarRegistroBloqueoTotal == 1 && PartRegistroBloqueoTotal== 1){
            Serial.println("************intenta guardar parte 2 de bloqueo total***********");
            PartRegistroBloqueoTotal= 2;
            registro_bloqueos_totales();
          }*/
          
          /*GuardarRegistroBloqueoTotal=0;
          HabilitarRegistroBloqueoTotal=0;
          PartRegistroBloqueoTotal = 0;
          preferences.putUInt(File_Part_Bloqueo_Total, PartRegistroBloqueoTotal); Serial.println("Parte de registro de Total Bloqueo: "+String(PartRegistroBloqueoTotal));
          Linea[i].GuardarRegistroBloqueo = 0;
          preferences.putUInt(File_Estado_Bloqueo_Total, GuardarRegistroBloqueoTotal); Serial.println("Guardar Registro Total Bloqueo: "+String(GuardarRegistroBloqueoTotal));
          */
        }
      }
    }
    num_Loop = num_Loop2 = 0;
    Reset_contadores();
    TimerBloqueoSensores();
    TimerBloqueoLinea();
    
    vTaskDelay(10);
  }
}

void Task5code( void * pvParameters ){
  while(1){
      //HABILITA RELEVADOR   
      if((millis() - previousActivateRele > intervalActivateRele) && EnableActivateRele == 1){
        previousActivateRele = millis();
        digitalWrite(Relevador, HIGH);
        EnableActivateRele =2;
      }else if((millis() - previousActivateRele > intervalActivateRele) && EnableActivateRele == 2){
        digitalWrite(Relevador, LOW);
        EnableActivateRele =0;
      }       
      //INDICADORES LED
      int num_LED = 0, num_LED2 = 0;  
      for(int i=0; i<3; i++){ 
        num_LED = num_LED + 2;
        num_LED2 = num_LED - 1;
        
        if(Sensor[num_LED2-1].Estado == 0 && Sensor[num_LED-1].Estado == 0 ){
          digitalWrite(LedIndicadorSensores[i], HIGH); 
        }else{
          digitalWrite(LedIndicadorSensores[i], LOW);
        } 
      }
    vTaskDelay(10);
  }
}

void Task3code( void * pvParameters ){
  lcd.init();
  lcd.backlight();
  setup_LCD();
  while(1){
        digitalWrite(IndicadorActividad,HIGH);
        //REINCIAR LCD
    /*    if((TimerCurrient - PreviousResetLCD> intervalResetLCD)){
            PreviousResetLCD = TimerCurrient;
            setup_LCD();
        }*/
        /*if(ResetLCD){
          lcd.clear();
          setup_LCD();
          ResetLCD=0;
        }*/
        //loop_LCD();
        setup_LCD();
  }
  vTaskDelay(10);
}
