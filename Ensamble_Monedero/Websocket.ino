void conexion_WS(){
  // Connect to the websocket server
  bool connected  = client.connect(websockets_server_host, websockets_server_port, "/");
  
  if (connected) {
     wsOK = 1;
     Serial.println("Connected!");
     client.send("HOLA, ESTE ES UN MENSAJE DE WEBSOCKET DE TESTEO PARA CONTADOR");
  } else {
    Serial.println("Not Connected!");
    wsOK = 0;
  }

   // run callback when messages are received
   client.onMessage([&](WebsocketsMessage message){
     Serial.print("Got Message: ");
     Serial.println(message.data());
     if(message.data() == "HOLA, ESTE ES UN MENSAJE DE WEBSOCKET DE TESTEO PARA CONTADOR"){
        //AQUI HABILITAR VAR PARA LCD 
     }
   });
}

void webSocketLoop(){
  
  if (client.available()) {
    client.poll();
    
      //ENVIA STATUS 
      if(EnableSendStatus){
        client.send("HOLA, ESTE ES UN MENSAJE DE TESTEO PARA CONTADOR");
        Serial.println("ENVIO MENSAJE");
        EnableSendStatus = 0;
      }   
  } else {
    wsOK = 0;
    bool connected = client.connect(websockets_server_host, websockets_server_port, "/");

    if(connected) {
      Serial.println("Connected!");
      wsOK = 1;
    } else {
      wsOK = 0;
    }
  }
}
