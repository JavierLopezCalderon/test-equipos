void Lecura_sensor() {
  for(int i=0; i<6; i++){
    Sensor[i].Lectura = digitalRead(Sensor[i].Pin[i]);
    if(Sensor[i].Lectura == 0){
      Sensor[i].Count++;
    }
  }
}

void logica_lineas(){
  int num_LCD = 0, num_LCD2 = 0;
  for(int i=0; i<3; i++){
    num_LCD = num_LCD + 2;
    num_LCD2 = num_LCD -1;
    
    if(Sensor[num_LCD2-1].Estado == 1 && Sensor[num_LCD-1].Estado == 0 && 
    Linea[i].Entrando == 0 && Linea[i].Bajando == 0){
      Linea[i].Sentido = 2;
      Linea[i].Posicion = 1;
      Linea[i].Entrando = 1;
      Linea[i].Aux = 1;
    }
    else if(Sensor[num_LCD2-1].Estado == 1 && Sensor[num_LCD-1].Estado == 0 && 
    Linea[i].Entrando == 1 && Linea[i].Bajando == 0){
      Linea[i].Posicion = 2;
    }
    else if(Sensor[num_LCD2-1].Estado == 1 && Sensor[num_LCD-1].Estado == 1 && 
    Linea[i].Entrando == 1 && Linea[i].Bajando == 0){
      Linea[i].Posicion = 3;   
      Linea[i].Aux = 0;  
    }
    else if(Sensor[num_LCD2-1].Estado == 0 && Sensor[num_LCD-1].Estado == 1 && 
    Linea[i].Entrando == 1 && Linea[i].Bajando == 0){
      Linea[i].Posicion = 4;     
    }
    else if(Sensor[num_LCD2-1].Estado == 0 && Sensor[num_LCD-1].Estado == 0 && 
    Linea[i].Entrando == 1 && Linea[i].Aux == 0 && Linea[i].Bajando == 0){
      Linea[i].Sentido = 0;
      if(Linea[i].Posicion == 4){
        Linea[i].Posicion = 5;
        Linea[i].Entrando = 0;
      }else{
        Linea[i].Posicion = 6;
        Linea[i].Entrando = 0;
        Linea[i].Aux = 0;
      }
    }
    else if(Sensor[num_LCD2-1].Estado == 0 && Sensor[num_LCD-1].Estado == 0 && 
    Linea[i].Entrando == 1 && Linea[i].Aux == 1 && Linea[i].Bajando == 0){
      Linea[i].Sentido = 0;
      Linea[i].Posicion = 6; 
      Linea[i].Entrando = 0;    
      Linea[i].Aux = 0;
    }  
    else if(Sensor[num_LCD2-1].Estado == 0 && Sensor[num_LCD-1].Estado == 1 &&
    Linea[i].Entrando == 0 && Linea[i].Bajando == 0){
      Linea[i].Sentido = 1;
      Linea[i].Posicion = 7;
      Linea[i].Bajando = 1;
    }
    else if(Sensor[num_LCD2-1].Estado == 0 && Sensor[num_LCD-1].Estado == 1 &&
    Linea[i].Entrando == 0 && Linea[i].Bajando == 1){
      Linea[i].Posicion = 8;
      Linea[i].Aux = 1;
    } 
    else if(Sensor[num_LCD2-1].Estado == 1 && Sensor[num_LCD-1].Estado == 1 &&
    Linea[i].Entrando == 0 && Linea[i].Bajando == 1){
      Linea[i].Posicion = 9;
      Linea[i].Aux = 0;
    }
    else if(Sensor[num_LCD2-1].Estado == 1 && Sensor[num_LCD-1].Estado == 0 &&
    Linea[i].Entrando == 0 && Linea[i].Bajando == 1){
      Linea[i].Posicion = 10;
    }
    else if(Sensor[num_LCD2-1].Estado == 0 && Sensor[num_LCD-1].Estado == 0 &&
    Linea[i].Entrando == 0 && Linea[i].Aux == 0 && Linea[i].Bajando == 1){
      Linea[i].Sentido = 0;
      if(Linea[i].Posicion == 10){
        Linea[i].Posicion = 11;
        //Linea[i].Baj.Suma++;
        Linea[i].Bajando = 0;
      }else{
        Linea[i].Posicion = 12;
        Linea[i].Bajando = 0;
        Linea[i].Aux = 0;
      }
    }
    else if(Sensor[num_LCD2-1].Estado == 0 && Sensor[num_LCD-1].Estado == 0 &&
    Linea[i].Entrando == 0 && Linea[i].Aux == 1 && Linea[i].Bajando == 1){
      Linea[i].Sentido = 0;
      Linea[i].Posicion = 12;
      Linea[i].Bajando = 0;
      Linea[i].Aux = 0;
    }
  }
  maquina_estados();
}

void maquina_estados(){
  for(int i=0; i <3; i++){
    switch(Linea[i].Posicion){
      case 1: //ENTRANDO
      break;
      case 5: //ENTRO
        if(Linea[i].Enable== 0){
          Linea[i].Sub.ES = 1;
          Linea[i].Sub.Suma++;
          Linea[i].Enable = 1; 
          Linea[i].previousMillisSensado = millis();        
        } 
        Linea[i].Baj.ES == 0;    
        Linea[i].Posicion = 0;
      break;
      case 6: //ENTRANDO Y REGRESO
        Linea[i].Posicion = 0;
      break;
      case 7: //SALIENDO
      break;
      case 11: //SALIO
        if(Linea[i].Enable== 0){
          Linea[i].Baj.ES = 1;
          Linea[i].Baj.Suma++;
          Linea[i].Enable = 1;  
          Linea[i].previousMillisSensado = millis();       
        } 
        Linea[i].Sub.ES = 0;     
        Linea[i].Posicion = 0;
      break;
      case 12: //SALIENDO Y REGRESO
        Linea[i].Posicion = 0;
      break;
    } 
  }
}

void Reset_contadores(){
  if((digitalRead(PinBotonReset) == LOW)||(Reset)){
    Serial.println("Inicia reset");

    /*for(int i=0; i<2; i++){
      preferences.putUInt(File_Count_Sensores_Magneticos[i], 0);
      countSensorMagnetico[i] = 0;
    }*/
    
    /*for(int i=0; i<6; i++){
    preferences.putUInt(File_Estado_Bloqueo_Sensor[i], 0); 
    preferences.putUInt(File_Part_Bloqueo_Sensor[i], 0);
    }*/
    /*for(int i=0; i<3; i++){
    preferences.putUInt(File_Estado_Bloqueo_Linea[i], 0);
    preferences.putUInt(File_Part_Bloqueo_Linea[i], 0);
    }*/
    /*
    preferences.putUInt(File_Estado_Bloqueo_Total, 0);
    preferences.putUInt(File_Part_Bloqueo_Total, 0); */
    Serial.println("Reset estados bloqueos ok");
    
    CountSubida = CountBajada = 0; 
    for(int i=0; i<3; i++){
      Linea[i].Sub.Suma = 0;
      Linea[i].Baj.Suma = 0;
    }
    for(int i=0; i<6; i++){
      CountSensoresBloqueo[i]=0;
      //Read.SensorCountBloqueo_String[i] = "0";
    }
    for(int i=0; i<3; i++){
      CountLineaBloqueos[i]=0;
      //Read.LineaCountBloqueo_String[i] = "0";
    }
    CountBloqueoTotal=0;
    /*Read.CountBloqueoTotal_String="0";
    
    Write.CountSubida_String = "0";   Read.CountSubida_String = "0";
    Write.CountBajada_String = "0";   Read.CountBajada_String = "0";
    Write.Linea1Sub_String   = "0";   Read.Linea1Sub_String   = "0";
    Write.Linea2Sub_String   = "0";   Read.Linea2Sub_String   = "0";
    Write.Linea3Sub_String   = "0";   Read.Linea3Sub_String   = "0";
    Write.Linea1Baj_String   = "0";   Read.Linea1Baj_String   = "0";
    Write.Linea2Baj_String   = "0";   Read.Linea2Baj_String   = "0";
    Write.Linea3Baj_String   = "0";   Read.Linea3Baj_String   = "0";
    
    Write.CountSubida_R_String = "0"; Read.CountSubida_R_String = "0";
    Write.CountBajada_R_String = "0"; Read.CountBajada_R_String = "0";
    Write.Linea1Sub_R_String = "0";   Read.Linea1Sub_R_String = "0";
    Write.Linea2Sub_R_String = "0";   Read.Linea2Sub_R_String = "0";
    Write.Linea3Sub_R_String = "0";   Read.Linea3Sub_R_String = "0";
    Write.Linea1Baj_R_String = "0";   Read.Linea1Baj_R_String = "0";
    Write.Linea2Baj_R_String = "0";   Read.Linea2Baj_R_String = "0";
    Write.Linea3Baj_R_String = "0";   Read.Linea3Baj_R_String = "0";*/
        
    //deleteCONTADORES();
    sd_saveFileCONTADORES();
    Serial.println("Reset contadores ok");
    Reset=0;
//    ResetLCD =1;
    Serial.println("Reset LCD ok");

    /*RSubida.REvento = 1;
    RBajada.REvento = 0;*/
    //guardar_registros_pasajeros(); 
    //status_server = "0"; 
    //status_bd = "0";
    //delay(500);
    Serial.println("Finaliza reset");
//    client.send("RESET EXITOSO");
    
    ESP.restart();
  }
}
