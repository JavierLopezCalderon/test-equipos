void printASCIIdata() { // prints the ascii data field
  for (int i = 4 ; i < RX_buffer[1] + 4; i++) {
    Serial.write(RX_buffer[i]);
    //Serial.print(RX_buffer[i]);
  }
  Serial.println();
}

void printBINdata() { // prints the bin data field
  for (int i = 4 ; i < RX_buffer[1] + 4; i++) {
    Serial.print(RX_buffer[i]);
  }
  Serial.println();
}

void cctalk_loop(){
  unsigned int coineventcounter;//used to follow the events from read credit or error codes
  unsigned int buffered_events; //how many events are queued

  coineventcounter = 0;// prepare for reading events
start:
  cctsend(simple_poll, 2, 0);
  while (RX_state < RXcomplete) {
    ccTalkReceive(); // wait for an answer or error
  }
  vTaskDelay(1);
  if (RX_state != RXcomplete) {
    Serial.println(RX_msg[RX_state]);// Show the error
    Serial.println("No answer to simple pool");
    ValorDesconexion = 1;
    EvioDesconexion();
    //while (1); // stop
  } else{
    Serial.println("ccTalk device present");// simple pool was ok
    ValorDesconexion = 0;
    EvioDesconexion();
  }
  cctsend(request_manufacturer_id, 2, 0);
  while (RX_state < RXcomplete) {
    ccTalkReceive();
  }
  vTaskDelay(1);
  if (RX_state != RXcomplete) {
    Serial.println(RX_msg[RX_state]);// Show the error if any
    Serial.println("No answer to Request manufacturer ID");
    ValorDesconexion = 2;
    EvioDesconexion();
  } else{
    printASCIIdata(); // Print the manufacturer ID
    ValorDesconexion = 0;
    EvioDesconexion();
  }
  cctsend(request_equipment_category_id, 2, 0);
  while (RX_state < RXcomplete) {
    ccTalkReceive();
  }
  vTaskDelay(1);
  if (RX_state != RXcomplete) {
    Serial.println(RX_msg[RX_state]);// Show the error if any
    Serial.println("No answer to Request equipment category id");
    ValorDesconexion = 3;
    EvioDesconexion();
  } else{
    printASCIIdata(); // Print the equipment category ID
    ValorDesconexion = 0;
    EvioDesconexion();
  }
  cctsend(request_product_code, 2, 0);
  while (RX_state < RXcomplete) {
    ccTalkReceive();
  }
  vTaskDelay(1);
  if (RX_state != RXcomplete) {
    Serial.println(RX_msg[RX_state]);// Show the error if any
    Serial.println("No answer to request_product_code");
    ValorDesconexion = 4;
    EvioDesconexion();
  } else{
    printASCIIdata(); // Print the product code
    ValorDesconexion = 0;
    EvioDesconexion();
  }
  cctsend(request_software_revision, 2, 0);
  while (RX_state < RXcomplete) {
    ccTalkReceive();
  }
  vTaskDelay(1);
  if (RX_state != RXcomplete) {
    Serial.println(RX_msg[RX_state]);// Show the error if any
    Serial.println("No answer to request_software_revision");
    ValorDesconexion = 5;
    EvioDesconexion();
  } else{
    printASCIIdata(); // Print the Software revision
    ValorDesconexion = 0;
    EvioDesconexion();
  }
  for (int i = 0; i < 16 ; i++) coin_value[i] = 0; // Clean the coin value array

  // Get coin ID, filter garbage convert and store in coin_value as unsigned int
  for (unsigned char i = 1; i < 17; i++) {
    somedelay.startt(2);//for some unknown reason a little delay is needed between polls
    do {
    } while (!somedelay.isready());
    cctsend(request_coin_id, 2, 1, i);
    
    while (RX_state < RXcomplete) {
      ccTalkReceive();
    }
    vTaskDelay(1);
    
    if (RX_state != RXcomplete) {
      Serial.print(RX_msg[RX_state]);// Show the error if any
      Serial.print(" at coin channel ");
      Serial.println(i);
      break;
    }
   // if ((RX_buffer[4] == 32) && (RX_buffer[5] == 32) && (RX_buffer[6] == 32) && (RX_buffer[7] == 32) && (RX_buffer[8] == 32) && (RX_buffer[9] == 32)) continue;
   // if ((RX_buffer[4] == 46) && (RX_buffer[5] == 46) && (RX_buffer[6] == 46) && (RX_buffer[7] == 46) && (RX_buffer[8] == 46) && (RX_buffer[9] == 46)) continue;
   // if ((65 > RX_buffer[4]) || (RX_buffer[4] > 90) || (65 > RX_buffer[5]) || (RX_buffer[5] > 90) || (65 > RX_buffer[9]) || (RX_buffer[9] > 90)) break;
   // if ((48 > RX_buffer[6]) || (RX_buffer[6] > 57) || (48 > RX_buffer[7]) || (RX_buffer[7] > 57) || (48 > RX_buffer[8]) || (RX_buffer[8] > 57)) break;
    printASCIIdata();// Print ASCII coin ID
    coin_value[i - 1] = (RX_buffer[6] - 48) * 100 + (RX_buffer[7] - 48) * 10 + RX_buffer[8] - 48;
  }
  Serial.println("Coin values OK");
  Serial.println("Setting individual inhibits");
  cctsend(modify_inhibit_status, 2, 2, 255, 255); //just enable all channels
  while (RX_state < RXcomplete) {
    ccTalkReceive();
  }
  vTaskDelay(1);
  if (RX_state != RXcomplete) {
    Serial.println(RX_msg[RX_state]);// Show the error if any
    Serial.println("No answer to modify_inhibit_status");
    ValorDesconexion = 6;
    EvioDesconexion();
  }else{ ValorDesconexion = 0; EvioDesconexion();}
  
  if (RX_buffer[3] == 0) Serial.println("OK");
  else {
    Serial.println("Error setting inhibits");
  }
  Serial.println("Setting master inhibit");
  cctsend(modify_master_inhibit_status, 2, 1, 1);
  while (RX_state < RXcomplete) {
    ccTalkReceive();
  }
  vTaskDelay(1);
  if (RX_state != RXcomplete) {
    Serial.println(RX_msg[RX_state]);// Show the error if any
    Serial.println("No answer to modify_master_inhibit_status");
    ValorDesconexion = 7;
    EvioDesconexion();
  }else{
    ValorDesconexion = 0;
    EvioDesconexion();
  }
  
  if (RX_buffer[3] == 0){ Serial.println("OK");
    ValorDesconexion = 0;
    EvioDesconexion();
  }else {
    Serial.println("Error setting master inhibit");
    ValorDesconexion = 8;
    EvioDesconexion();
    //while(1);
  }
  // read buffer credit or error codes
  Serial.println("reading credits");
  ///////////////////BUCLE
  while (1) {
    //Core1_loop();/**********/
    //reconectarWIFIyWS();//************/
    
    cctsend(read_buffered_credit_or_error_codes, 2, 0);
    while (RX_state < RXcomplete) {
      ccTalkReceive();
    }
    vTaskDelay(1);
    if (RX_state != RXcomplete) {
      Serial.println(RX_msg[RX_state]);// Show the error if any
      Serial.println("Stopped");
      ValorDesconexion = 9;
      EvioDesconexion();
      ccTalk_comm_init();
      while ( RX_state != RXidle) {
        ccTalkReceive();
      } goto start;// Some error handling needed here
    }else{
      ValorDesconexion = 0;
      EvioDesconexion();
    }

    somedelay.startt(2);//for some unknown reason a little delay is needed between polls
    do {
    } while (!somedelay.isready());
    // find how many new events are queued, RX_buffer[4] data contains the event counter
    if (RX_buffer[4] >= coineventcounter)  buffered_events = RX_buffer[4] - coineventcounter;
    else buffered_events = RX_buffer[4] + 255 - coineventcounter;

    if (buffered_events > 5) {// overflow,events lost, put some error handling here
      buffered_events = 0;
      coineventcounter = RX_buffer[4]; //Clear the queued events
    }

    while (buffered_events > 0) {// Read buffered events one by one
      buffered_events--;
      coineventcounter = (coineventcounter + 1) % 256;//increment event counter
      if (coineventcounter == 0) coineventcounter = 1; // skip 0
      if (RX_buffer[(buffered_events << 2) + 5] == 0) { // event A = 0 mans an error or coin rejected
        Serial.print("Some error or coin rejected, error code: ");
        Serial.println(RX_buffer[(buffered_events << 2) + 6]);// then event B is the error code
        ValorError= RX_buffer[(buffered_events << 2) + 6];
        EnvioError();
      } else {
        Serial.print("Coin value ");
        //Serial.println(coin_value[RX_buffer[(buffered_events << 2) + 5] - 1]);//The corresponding coin value in the table
          CoinValor = coin_value[RX_buffer[(buffered_events << 2) + 5] - 1];
          Serial.println(CoinValor);
          for(int i=0; i<6; i++){
            if(CoinValor == coinValorArray[i]){
              Identificacion_moneda();
            }
          }
                               
        /////Serial.print("Sorter path ");
        /////Serial.println(RX_buffer[(buffered_events << 2) + 6]); // event B show the sorter path
      }
    }
    vTaskDelay(1);
  }
}
