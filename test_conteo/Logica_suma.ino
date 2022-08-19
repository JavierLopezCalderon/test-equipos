void Suma(){
  // SUMA CONTADOR SIN BLOQUEOS //
  if(Sensor[0].Estado==0 && Sensor[1].Estado==0 &&
     Sensor[2].Estado==0 && Sensor[3].Estado==0 &&
     Sensor[4].Estado==0 && Sensor[5].Estado==0 ){
     
     STS = Linea[0].Sub.ES + Linea[1].Sub.ES + Linea[2].Sub.ES;
     STB = Linea[0].Baj.ES + Linea[1].Baj.ES + Linea[2].Baj.ES;
    
     if((STS>=2)||(STB>=2)){
        if(STS>STB){ 
          CountSubida++;   
          enableSaveCount = 1;  
          //envioCuenta =1;
          //RSubida.REvento = 1;
          //RBajada.REvento = 0;
          EnableActivateRele = 1;
        }else{
          CountBajada++;
          enableSaveCount = 1;
          //envioCuenta =1;
          //RBajada.REvento = 1;
          //RSubida.REvento = 0;
        }
     }
     finSuma();
  }
  //SUMA CON BLOQUEOS 
  else if((Sensor[0].Bloqueo != 0||Sensor[1].Bloqueo != 0||Sensor[2].Bloqueo != 0||
       Sensor[3].Bloqueo !=0||Sensor[4].Bloqueo!=0||Sensor[5].Bloqueo!=0)){
     for(int i=0;i<6;i++){
        BS=0;      
        BS=Sensor[i].Bloqueo;
        if(BS == 1 || BS == 2){
            Linea[0].BloqueoSuma =1;
        }else if(BS == 3 || BS == 4){
            Linea[1].BloqueoSuma =1;
        }else if(BS == 5 || BS == 6){
            Linea[2].BloqueoSuma =1;
        }
     }
     SBL = Linea[0].BloqueoSuma + Linea[1].BloqueoSuma + Linea[2].BloqueoSuma;
     // SUMA CON UN BLOQUEO
     if(SBL == 1){
      //SumaEnable=0;
        for(int i=0; i<3; i++){
          if(Linea[i].BloqueoSuma == 1 && SumaEnable == 0){
            STS = Linea[0].Sub.ES + Linea[1].Sub.ES + Linea[2].Sub.ES;
            STB = Linea[0].Baj.ES + Linea[1].Baj.ES + Linea[2].Baj.ES;
            if((STS>=1)||(STB>=1)){
               if(STS>STB){
                  CountSubida++;
                  enableSaveCount = 1;
                  SumaEnable =1;
                  //RSubida.REvento = 1;
                  //RBajada.REvento = 0;
                  EnableActivateRele = 1;
                  previousMillisBloqueoSuma = millis();
               }else{
                  CountBajada++;
                  enableSaveCount = 1;
                  SumaEnable =1;
                  //RBajada.REvento = 1;
                  //RSubida.REvento = 0;
                  previousMillisBloqueoSuma = millis();
               }
            }
            finSuma();
          }
        }
     }
     // SUMA CON DOS BLOQUEOS
     else if(SBL == 2){
        for(int i=0; i<3; i++){
           int a=0, b=0;
           if(i == 0){
              a=0; b=1;
           }else if(i == 1){
              a=1; b=2;
           }else if(i == 2){
              a=0; b=2;
           }
           if(Linea[a].BloqueoSuma == 1 && Linea[b].BloqueoSuma == 1){
              STS = Linea[0].Sub.ES + Linea[1].Sub.ES + Linea[2].Sub.ES;
              STB = Linea[0].Baj.ES + Linea[1].Baj.ES + Linea[2].Baj.ES;
              if((STS>=1)||(STB>=1)){
                if(STS>STB){
                   CountSubida++;
                   //RSubida.REvento = 1;
                   //RBajada.REvento = 0;
                   Serial.println(CountSubida);
                   enableSaveCount = 1;     
                   EnableActivateRele = 1;
                }else{
                   CountBajada++;
                   //RBajada.REvento = 1;
                   ///RSubida.REvento = 0;
                   Serial.println(CountBajada);
                   enableSaveCount = 1;
                }
                 Serial.println("******************ENVIA MSG WS CONTADORES 2 BLOQUEOS*******************");
                 //SerializarDatosTiempoReal();
             }
             finSuma();
           }
        }
     }
  }
}

void finSuma(){
   for(int i=0; i<3; i++){
      Linea[i].Sub.ES = 0;
      Linea[i].Baj.ES = 0;
      Linea[i].BloqueoSuma =0;
   }
   STS = STB = 0;
   edo_ent = edo_sal =0;
}
