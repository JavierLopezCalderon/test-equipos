void setup_LCD(){
  lcd.init();
  //lcd.backlight();
  //lcd.clear();
  int num_LCD = 0, num_LCD2 = 0;   
  for(int i=0; i<3; i++){
    num_LCD = num_LCD + 2;
    num_LCD2 = num_LCD -1;      
    lcd.setCursor (0,(i+1));
    lcd.print(num_LCD);
    lcd.print(":");      
    lcd.setCursor (4,(i+1));
    lcd.print(num_LCD2);
    lcd.print(":");
    lcd.setCursor (6,(i+1));
    lcd.print(Sensor[num_LCD2-1].Estado);          
    lcd.setCursor (2,(i+1));
    lcd.print(Sensor[num_LCD-1].Estado);        
  }     

  for(int i=0; i<6; i++){
    Sensor[i].AntEstado = Sensor[i].Estado;
  }
  for(int i=0; i<3; i++){
    Linea[i].Sub.Valant = Linea[i].Sub.Suma;
    //Linea[i].Baj.Valant = Linea[i].Baj.Suma;
    lcd.setCursor (8,(i+1));
    lcd.print(Linea[i].Sub.Suma);
    lcd.setCursor (13,(i+1));
    lcd.print(Linea[i].Baj.Suma);
  }

  for(int i=0; i<3; i++){
    lcd.setCursor(19,(i+1));
    lcd.print(Linea[i].Bloqueo);
  }    
    lcd.setCursor(0,0);
    lcd.print("ST:");
    lcd.print(CountSubida);
    lcd.setCursor(10,0);
    lcd.print("BT:");
    lcd.print(CountBajada);
}

void loop_LCD(){
  int num_LCD = 0, num_LCD2 = 0;
  for(int i=0; i<3; i++){
    num_LCD = num_LCD +2;
    num_LCD2 = num_LCD -1;
      
    if(Sensor[num_LCD2-1].AntEstado != Sensor[num_LCD2-1].Estado){  
      setup_LCD();        
      //lcd.setCursor (6,(i+1));
      //lcd.print(Sensor[num_LCD2-1].Estado);
      Sensor[num_LCD2-1].AntEstado = Sensor[num_LCD2-1].Estado ;
    }
    if(Sensor[num_LCD-1].AntEstado != Sensor[num_LCD-1].Estado){ 
      setup_LCD();        
      //lcd.setCursor (2,(i+1));
      //lcd.print(Sensor[num_LCD-1].Estado);
      Sensor[num_LCD-1].AntEstado = Sensor[num_LCD-1].Estado;
    }

    if(Linea[i].Sub.Valant != Linea[i].Sub.Suma){
        setup_LCD();
        //lcd.setCursor (8,(i+1));
        //lcd.print(Linea[i].Sub.Suma);
        Linea[i].Sub.Valant = Linea[i].Sub.Suma;
    }
    if(Linea[i].Baj.Valant != Linea[i].Baj.Suma){
        setup_LCD();
        //lcd.setCursor (13,(i+1));
        //lcd.print(Linea[i].Baj.Suma);
        Linea[i].Baj.Valant = Linea[i].Baj.Suma;
    }

    /*if(Linea[i].Sentido == 1){
      lcd.setCursor (7,(i+1));
      lcd.write(126);
    }else if(Linea[i].Sentido == 2){
      lcd.setCursor (7,(i+1));
      lcd.write(127);
    }else if(Linea[i].Sentido == 0){
      lcd.setCursor (7,(i+1));
      lcd.print(" ");
    }*/
  }
   /* for(int i=0; i<3; i++){
      lcd.setCursor(19,(i+1));
      lcd.print(Linea[i].Bloqueo);
    }    
      lcd.setCursor(0,0);
      lcd.print("ST:");
      lcd.print(CountSubida);
      lcd.setCursor(10,0);
      lcd.print("BT:");
      lcd.print(CountBajada);*/
}
