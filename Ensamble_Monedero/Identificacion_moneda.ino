
void Identificacion_moneda(){
  for(int i=0; i<6; i++){
    if(CoinValor == coinValorArray[i]){
      Serial.println("$"+ String(Monedas_Aceptadas[i]));
      ValorCoinLCD = M[i];
      EnableValor =1;
    }
  }
}

/*********MENSAJES DE ERRORES***********/
void EnvioError(){
  switch (ValorError){
    case 1:
      Serial.println("E1"); //Monedas introducidas al mismo tiempo
     break;
     case 14:
      Serial.println("E14"); //No se reconocio moneda
     break;
     case 254:
      Serial.println("E254"); //Se presiono palanca
     break;
     case 8:
      Serial.println("E8"); //Dos monedas casi juntas
     break;
  }
}

/**************MENSAJES DE DESCONEXIÓN**************/
void EvioDesconexion(){
}
