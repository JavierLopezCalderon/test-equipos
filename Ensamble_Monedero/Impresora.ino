void iniciaImpresora(){
  for(byte i=0; i<2; i++){
    Serial1.write(IMPT_INIT[i]);
  }
  for(byte i=0; i<2; i++){
    Serial1.write(IMPT_MODO[i]);
  }
  Serial1.write(FONTA);
  for(byte i=0; i<2; i++){
    Serial1.write(IMPT_ESP_PROG[i]);
  }
  Serial1.write(interlineado);
  //digitalWrite(Relevador_Impresora, HIGH);
}

void ImprimeCorte(){ 

    for(byte i=0; i<2; i++){
      Serial1.write(IMPT_INIT[i]);
    }
    for(byte i=0; i<2; i++){
      Serial1.write(IMPT_MODO[i]);
    }
    Serial1.write(FONTA);
    for(byte i=0; i<2; i++){
      Serial1.write(IMPT_ESP_PROG[i]);
    }
    Serial1.write(20);
    ImpresionLinea();
    Serial1.println("");
    SetText(0x11, JUST_CENTER, 0x01);
    Serial1.println("CORTE");
    SetText(0x01, JUST_CENTER, 0x01);
    Serial1.print("RUTA: ");
    SetText(0x01, JUST_CENTER, 0x00);
    Serial1.println("45");
    
    SetText(0x01, JUST_CENTER, 0x01);
    Serial1.print("UNIDAD: ");
    SetText(0x01, JUST_CENTER, 0x00);
    Serial1.println("401");

    SetText(0x00, JUST_LEFT, 0x01);
    Serial1.print("FOLIO: ");
    SetText(0x01, JUST_LEFT, 0x00);
    Serial1.println("10");
    
    SetText(0x00, JUST_LEFT, 0x01);
    Serial1.print("FECHA: ");
    SetText(0x00, JUST_LEFT, 0x00);
    Serial1.println("20-08-2022");
    SetText(0x00, JUST_LEFT, 0x01);
    Serial1.print("LATITUD: ");
    SetText(0x00, JUST_LEFT, 0x00);
    Serial1.println("000");
    SetText(0x00, JUST_LEFT, 0x01);
    Serial1.print("LONGITUD: ");
    SetText(0x00, JUST_LEFT, 0x00);
    Serial1.println("000");
    
    Serial1.println("");
    ImpresionLinea();
    Serial1.println("");

    SetText(0x01, JUST_CENTER, 0x01);
    Serial1.println("MONEDAS RECAUDADAS");
    String M[6]={"$1.00:  ","$2.00:  ","$5.00:  ","$10.00: ","$20.00: ","$0.50:  "};
    for(int i=0; i<6; i++){
      SetText(0x01, JUST_LEFT, 0x01);
      Serial1.print("MONEDAS " + M[i]);
      SetText(0x01, JUST_LEFT, 0x00);
      Serial1.println("5");
    }
    ImpresionLinea();
    Serial1.println("");
    SetText(0x01, JUST_LEFT, 0x01);
    Serial1.print("ABERTURAS GAVETA SUPERIOR: ");
    SetText(0x01, JUST_LEFT, 0x00);
    Serial1.println("10");
    SetText(0x01, JUST_LEFT, 0x01);
    Serial1.print("ABERTURAS GAVETA INFERIOR: ");
    SetText(0x01, JUST_LEFT, 0x00);
    Serial1.println("10");
    ImpresionLinea();
    Serial1.println("");
 
    SetText(0x01, JUST_LEFT, 0x01);
    Serial1.print("TARIFA: $");
    SetText(0x01, JUST_LEFT, 0x00);
    Serial1.println("10");
    SetText(0x01, JUST_LEFT, 0x01);
    Serial1.print("TOTAL PERSONAS: ");
    SetText(0x01, JUST_LEFT, 0x00);
    Serial1.println("10");
    SetText(0x01, JUST_LEFT, 0x01);
    Serial1.print("TOTAL RECAUDADO: ");
    SetText(0x01, JUST_LEFT, 0x00);
    Serial1.println("100");
    
    SetText(0x01, JUST_LEFT, 0x00);
    Serial1.println("NOMBRE:_______________________________");
    Serial1.println("FIRMA:________________________________");
    ImpresionLinea();
    Serial1.println("");
    for(byte i=0; i<2;i++){
      Serial1.write(IMPT_CORTE_POS[i]);
    }
    Serial1.write(CUT_POS_OFFSET);
    Serial1.write(20);
}

void BoletoAlcancia(){
  ImpresionLinea();
  Serial1.println("");
  SetText(0x01, JUST_CENTER, 0x01);
  Serial1.print("RUTA: ");
  SetText(0x01, JUST_CENTER, 0x00);
  Serial1.println("45");
  
  SetText(0x00, JUST_LEFT, 0x01);
  Serial1.print("UNIDAD: ");
  SetText(0x01, JUST_LEFT, 0x00);
  Serial1.print("401");
  Serial1.print("      ");
  SetText(0x00, JUST_LEFT, 0x01);
  Serial1.print("FOLIO: ");
  SetText(0x00, JUST_LEFT, 0x00);
  Serial1.println("10");
  
  SetText(0x00, JUST_LEFT, 0x01);
  Serial1.print("TARIFA: ");
  SetText(0x01, JUST_LEFT, 0x00);
  Serial1.print("10");
  Serial1.print("  ");
  
  SetText(0x00, JUST_LEFT, 0x01);
  Serial1.print("FECHA: ");
  SetText(0x00, JUST_LEFT, 0x00);
  Serial1.println("05-08-2022");
  ImpresionLinea();
  Serial1.println("");
  SetText(0x00, JUST_CENTER, 0x00);
  for(byte i=0; i<2; i++){Serial1.write(IMPT_MODO[i]);}
  Serial1.write(FONTA);
  Serial1.println("SI NO RECIBE SU BOLETO O");
  Serial1.println("NO CORRESPONDE A SU TARIFA");
  Serial1.println("REPORTE AL CEL: " + Cel_Reporte);
  Serial1.println("CONSERVE ESTE BOLETO,\r\n ES SU SEGURO DE VIAJERO.");

  //IMPRESION DEL QR: Ancho
  Serial1.write('\x1D'); Serial1.write('\x28'); Serial1.write('\x6B');
  Serial1.write('\x03'); Serial1.write('\x00'); Serial1.write('\x31');
  Serial1.write('\x43');
  Serial1.write('\x04');  
  //Ancho del QR
  //QR: Memoria
  Serial1.write('\x1D'); Serial1.write('\x28'); Serial1.write('\x6B');
  Serial1.write(strlen("www.accesa.me")+3);  //LongitudQR+3
  Serial1.write('\x00'); Serial1.write('\x31');
  Serial1.write('\x50'); Serial1.write('\x30');
  //QR:Data and imprimir
  Serial1.print("www.accesa.me");
  Serial1.write('\x1D'); Serial1.write('\x28'); Serial1.write('\x6B');
  Serial1.write('\x03'); Serial1.write('\x00'); Serial1.write('\x31');
  Serial1.write('\x51'); Serial1.write('\x30');

  SetText(0x00, JUST_CENTER, 0x00);
  for(byte i=0; i<2; i++){Serial1.write(IMPT_MODO[i]);}
  Serial1.write(FONTA);
  Serial1.println("VENTAS: "+ Cel_ventas);
  Serial1.println("www.accesa.me | automatizacion@accesa.me");
  ImpresionLinea();
  Serial1.println("");
  for(byte i=0; i<2;i++){Serial1.write(IMPT_CORTE_POS[i]);}
  Serial1.write(CUT_POS_OFFSET);
  Serial1.write(20);
}


byte obtenerEstado(byte solicitud){
  byte j=0, retorno;
  
  for(byte i=0; i<2; i++){
    Serial1.write(IMPT_STAT[i]);
  }
  Serial1.write(solicitud);
  while(Serial1.available()){
    byte estado = Serial1.read();
    retorno = estado;
    switch(solicitud){
      case 1:
      Serial.print("Estado Impresora: ");
      Serial.println(estado, DEC);
      return estado;
      break;
      case 2:
      Serial.print("Causa Offline: ");
      Serial.println(estado, DEC);
      return estado;
      break;
      case 3:
      Serial.print("Tipo Error: ");
      Serial.println(estado, DEC);
      return estado;
      break;
      case 4:
      Serial.print("Estado Papel: ");
      Serial.println(estado, DEC);
      return estado;
      break;
    }
  }
  return 0;
} 

void ImpresionLinea(){
  for(byte i=0; i<2; i++){ Serial1.write(IMPT_SIZE_CHAR[i]); }
  Serial1.write(0x00);
  for(byte i=0; i<2; i++){ Serial1.write(IMPT_JUST[i]); }
  Serial1.write(JUST_LEFT);
  for(byte i=0; i<2; i++){ Serial1.write(IMPT_NEGRITA[i]); }
  Serial1.write(0x01);
  Serial1.printf("------------------------------------------");
}

void SetText(int SizePrint, int JustPrint, int NegritaPrint){
  for(byte i=0; i<2; i++){ Serial1.write(IMPT_SIZE_CHAR[i]); }
  Serial1.write(SizePrint);
  for(byte i=0; i<2; i++){ Serial1.write(IMPT_JUST[i]); }
  Serial1.write(JustPrint);
  for(byte i=0; i<2; i++){ Serial1.write(IMPT_NEGRITA[i]); }
  Serial1.write(NegritaPrint);
}
