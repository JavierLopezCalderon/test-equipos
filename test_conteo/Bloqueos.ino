//CAMBIA EL ESTADO DE BLOQUEO LAS LINEAS
void Bloqueo_sensores(){
  int num_LCD = 0, num_LCD2 = 0;  
  for(int i=0; i<3; i++){  
    num_LCD = num_LCD + 2;
    num_LCD2 = num_LCD - 1;
    
    if(Sensor[num_LCD2-1].Bloqueo != 0 && Sensor[num_LCD-1].Bloqueo != 0 && Linea[i].Bloqueo == 0 ){
        Linea[i].Bloqueo = 1;
        Linea[i].previousMillisBloqueoLinea = millis(); //No se reinicia timer hasta que se desbloquean los sensores  
    }
  }
}

void Bloqueo_Total(){
  //ACTIVA EL BLOQUEO TOTAL
  if(Linea[0].Bloqueo == 1 && Linea[1].Bloqueo == 1 && Linea[2].Bloqueo == 1 && BloqueoTotal == 0){
     BloqueoTotal =1;
     ///CountBloqueoTotal++; //SUMA CONTEO DE BLOQUEOS TOTALES
     previousMillisBloqueoTotal = millis();
     
  }

  if(BloqueoTotal == 1){
    // TIEMPO DE BLOQUEO TOTAL
    HabilitarRegistroBloqueoTotal = 1;
    TimepoBloqueoTotal = (TimerCurrient-previousMillisBloqueoTotal)/1000;
    //Serial.println(TimepoBloqueoTotal);
  }
}

void TimerBloqueoSensores(){
  for(int i=0; i<6; i++){
    if((millis()-Sensor[i].previousMillisBloqueo>=intervalBloqueo)&& Sensor[i].EnableB == 1){    
       Sensor[i].Bloqueo= i+1; 
       //Serial.println("Sensor bloqueado: " + String(i+1));
       Sensor[i].HabilitarRegistroBloqueo = 1;

       // TIEMPO DE SENSOR BLOQUEADO
       Sensor[i].TiempoBloqueado = (TimerCurrient-Sensor[i].previousMillisBloqueo)/1000; 
      // Serial.println(Sensor[0].TiempoBloqueado);
    }
  }
}

void TimerBloqueoLinea(){
  for(int i=0; i<3; i++){
    //TIMER DE ESPERA PARA LINEA BLOQUEADA
    if(Linea[i].Bloqueo == 1){
      // TIEMPO DE LINEA BLOQUEADO
      Linea[i].HabilitarRegistroBloqueo = 1;      
      Linea[i].TiempoLineaBloqueado = (TimerCurrient-Linea[i].previousMillisBloqueoLinea)/1000;
      //Serial.println(Linea[0].TiempoLineaBloqueado);
    }
  }
}
