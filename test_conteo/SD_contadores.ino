
void sd_saveFileCONTADORES(){
  int AD = 0;
  for(int SC=0; SC<18; SC++ ){
    if(SC == 0){ preferences.putUInt(File_Contadores_Flash[SC], CountSubida);
    }else if(SC == 1){ preferences.putUInt(File_Contadores_Flash[SC], CountBajada);
    }else if(SC>=2 && SC<5){ AD = SC -2; preferences.putUInt(File_Contadores_Flash[SC], Linea[AD].Sub.Suma);
    }else if(SC>=5 && SC<8){ AD = SC -5; preferences.putUInt(File_Contadores_Flash[SC], Linea[AD].Baj.Suma);
    }else if(SC>=8 && SC<14){ AD = SC -8; preferences.putUInt(File_Contadores_Flash[SC], CountSensoresBloqueo[AD]);
    }else if(SC>=14 && SC<17){ AD = SC -14; preferences.putUInt(File_Contadores_Flash[SC], CountLineaBloqueos[AD]);
    }else if(SC == 17){ preferences.putUInt(File_Contadores_Flash[SC], CountBloqueoTotal);
    }  
  }
}

void readFileCONTADORES(){
  int AE=0;
  Serial.print("Read from file CONTADORES: ");
  for(int RC=0; RC<18; RC++){
    if(RC == 0){ 
      CountSubida = preferences.getUInt(File_Contadores_Flash[RC], 0);
      Serial.println("CountSubida: "+ String(CountSubida));
    }else if(RC == 1){ 
      CountBajada = preferences.getUInt(File_Contadores_Flash[RC], 0);
      Serial.println("CountBajada: "+ String(CountBajada));
    }else if(RC>=2 && RC<5){ 
      AE = RC -2; 
      Linea[AE].Sub.Suma = preferences.getUInt(File_Contadores_Flash[RC], 0);
      Serial.println("Linea.Sub.Suma[" + String(AE)+"]: "+ String(Linea[AE].Sub.Suma));
    }else if(RC>=5 && RC<8){ 
      AE = RC -5; 
      Linea[AE].Baj.Suma = preferences.getUInt(File_Contadores_Flash[RC], 0);
      Serial.println("Linea.Baj.Suma[" + String(AE)+"]: "+ String(Linea[AE].Baj.Suma));
    }else if(RC>=8 && RC<14){
      AE = RC -8; 
      CountSensoresBloqueo[AE] = preferences.getUInt(File_Contadores_Flash[RC], 0);
      Serial.println("CountSensoresBloqueo[" + String(AE)+"]: "+ String(CountSensoresBloqueo[AE]));
    }else if(RC>=14 && RC<17){ 
      AE = RC -14; 
      CountLineaBloqueos[AE] = preferences.getUInt(File_Contadores_Flash[RC], 0);
      Serial.println("CountLineaBloqueos[" + String(AE)+"]: "+ String(CountLineaBloqueos[AE]));
    }else if(RC == 17){ 
      CountBloqueoTotal = preferences.getUInt(File_Contadores_Flash[RC], 0);
      Serial.println("CountBloqueoTotal: "+ String(CountBloqueoTotal));
    }  
  }
}
