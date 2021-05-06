void mostrar_terminal (String aviso){
  terminal.print(currentTime);
  terminal.print(" - ");
  terminal.println(aviso);
  terminal.flush();
}

void gravar(byte posicao, bool var){
  EEPROM.write(posicao,var);
  EEPROM.commit();
  delay(50);
}

void barra_progresso(int p1, int p2, int p3, int p4, int p5, int tempo){
  for (int x=0; x<p5; x++){ // p5 é o valor total da barra
    display1.drawProgressBar(p1, p2, p3, p4, x); // Onde começa a barra a esquerda
                                                 // Linha onde a barra vai ficar
                                                 // Onde finaliza a barra a Direita
                                                 // Largura da Barra
                                                 // Valor a ser mostrado na progressao da barra
    display1.display();
    delay(tempo);
  }
}

void mostrar_display(String t1, String t2, String t3, String t4, byte fonte, float segundos){
  esp_task_wdt_reset();
      int tempo = segundos * 1000;
      display1.clear();
      display1.setTextAlignment(TEXT_ALIGN_CENTER);
      if(fonte == 10){display1.setFont(ArialMT_Plain_10);}
      if(fonte == 16){display1.setFont(ArialMT_Plain_16);}
      display1.drawString(64,  0, t1);
      display1.drawString(64, 16, t2);
      display1.drawString(64, 32, t3);
      display1.drawString(64, 48, t4);
      display1.display();
      delay(tempo);
}

void iniciar_display(){
  display1.init();
  display1.flipScreenVertically();
  display1.setTextAlignment(TEXT_ALIGN_LEFT);
  display1.setFont(ArialMT_Plain_16);
  display1.drawString(0, 0, "Iniciando...");
  display1.display();
  barra_progresso(0, 30, 127, 10, 100, 20);
  display1.clear();
  display1.display();
}

void info(){
  esp_task_wdt_reset();
  uint32_t RAM_to = ESP.getHeapSize();        RAM_to = RAM_to / 1024;
  uint32_t RAM    = ESP.getFreeHeap();        RAM    = RAM  / 1024;
  uint32_t PROG   = ESP.getSketchSize();      PROG   = PROG / 1024;
  uint32_t FREE   = ESP.getFreeSketchSpace(); FREE   = FREE / 1024;
  uint32_t SIZE   = ESP.getFlashChipSize();   SIZE   = SIZE / 1024;
  int   FREQ = ESP.getCpuFreqMHz();

  int porcentagem = (PROG * 100) / FREE;
  String por_str = String(porcentagem) + "%";

  RAM_livre = "RAM Fre "   + String(RAM)  + "KB de " + String(RAM_to) + "KB";
  Programa  = "ROM Usado " + String(PROG) + "KB " + por_str;
  Free_Prog = "ROM Total " + String(FREE) + "KB 100%";
  Chip_Size = "FLASH "     + String(SIZE) + "KB";
  Freq_proc = " - "        + String(FREQ) + "MHz";

  Chip_Size += Freq_proc;

  mostrar_display(RAM_livre, Programa, Free_Prog, Chip_Size, 10, 3);
}

void verifica_conexao_blynk(){ // Pisca o LED do App.
  if(error_conect == true){
    conexao = "OnLine";
    if(x==0){x=1; led_ativo.on();  pino(led_azul, 1); pino(Reset_Sinal, 1); }
    if(x==2){x=0; led_ativo.off(); pino(led_azul, 0); pino(Reset_Sinal, 0); }
    if(x==1){x=2;} 
  }
  else{
    conexao = "OffLine";
    pino(led_azul, 0);
    x=0;
  }
}

boolean configSave() {
  // Grava configuração
  StaticJsonDocument<JSON_SIZE> jsonConfig;
  File file = SPIFFS.open(F("/Config.json"), "w+");
  if (file) {
    jsonConfig["device"] = device;
    jsonConfig["ssid"]   = ssid;
    jsonConfig["pw"]     = pw;

    serializeJsonPretty(jsonConfig, file);
    file.close();
    serializeJsonPretty(jsonConfig, Serial);
    mostrar_display("Gravado no", "", "SPIFFS", "", 16, 2);
    return true;
  }
  return false;
}

boolean configRead() {
  // Lê configuração
  StaticJsonDocument<JSON_SIZE> jsonConfig;

  File file = SPIFFS.open(F("/Config.json"), "r");
  if (deserializeJson(jsonConfig, file)) {
    // Falha na leitura, assume valores padrão
    mostrar_display("Falha ao", "ler o", "SPIFFS", ":(", 16, 2);
    return false;
  } else {
    // Sucesso na leitura
    strlcpy(device,  jsonConfig["device"] | "", sizeof(device)); 
    strlcpy(ssid,    jsonConfig["ssid"]   | "", sizeof(ssid));
    strlcpy(pw,      jsonConfig["pw"]     | "", sizeof(pw));
    device_str = device;
    
    file.close();
    mostrar_display("SPIFFS OK", ssid, pw, device_str,16, 1);
    serializeJsonPretty(jsonConfig, Serial);
    return true;
  }
}

void iniciar_SPIFFS(){
  if(!SPIFFS.begin(FORMAT_SPIFFS_IF_FAILED)){
    mostrar_display("Falha SPIFFS", ":(", "Contacte o", "Suporte", 16, 2); 
    while(true){}
    }
  else{
    mostrar_display("Sucesso ao", "iniciar o", "SPIFFS", ":)", 16, 1);
    configRead();  // Ler conteudo da SPIFFS
    }
}

void handleRoot() {
  //Go to http://192.168.4.1 in a web browser
String html =  "<!DOCTYPE html><head><html lang='pt-br'><meta charset='UTF-8'><meta http-equiv='X-UA-Compatible' content='IE=edge'><meta name='viewport' content='width=device-width, initial-scale=1'><title>Configurar WIFI</title></head><body align='center'>";
       html += "<h3>CONFIGURAÇÃO<br> WIFI.</h3>";
       html += "<form method='POST' action='/form'>";
       html += "<input type=text placeholder='DIGITE O NOME DA REDE'  name=txt_ssid   /><br><br>";
       html += "<input type=text placeholder='DIGITE A SENHA DA REDE' name=txt_pw     /><br><br>";
       html += "<input type=text placeholder='NOME DO DISPOSITIVO'    name=txt_device /><br><br>";
       html += "<input type=submit name=botao value=Enviar />";
       html += "</form></body></html>";
server.send(200, "text/html", html);
mostrar_display("Digite", " as informações", "Solicitadas", "", 16, 1);
}

void formularioEnviado() {
String html =  "<!DOCTYPE html><head><html lang='pt-br'><meta charset='UTF-8'><meta http-equiv='X-UA-Compatible' content='IE=edge'><meta name='viewport' content='width=device-width, initial-scale=1'><title>WIFI Configurado</title></head><body align='center'><h3>Seu dispositivo foi<br> configurado</h3>";
       html += "<h1><font color='red'>SSID: ";
       html += server.arg("txt_ssid");
       html += "</h1></font><br><h1><font color='green'>SENHA: ";
       html += server.arg("txt_pw");
       html += "</font></h1></body></html>";
       server.send(200, "text/html", html);

  device_str   = server.arg("txt_device");
  ssid_str     = server.arg("txt_ssid");
  pw_str       = server.arg("txt_pw");

  device_str.trim(); strlcpy(device,  device_str.c_str(), sizeof(device));
  ssid_str.trim();   strlcpy(ssid,    ssid_str.c_str(),   sizeof(ssid));
  pw_str.trim();     strlcpy(pw,      pw_str.c_str(),     sizeof(pw));

  mostrar_display("Dados:", ssid_str, pw_str, "Gravados!", 16, 3);
  configSave(); // salva na SPIFFS
  
}

void config_OTA_WebServer(){
  /* Usa MDNS para resolver o DNS */
    if (!MDNS.begin(host)) { 
        //http://esp32.local
        mostrar_display("ERRO", "mDNS", "", "ESP32 vai reiniciar", 16, 2);
        ESP.restart();        
    }
   
    mostrar_display("", "mDNS", "", "Configurado", 16, 1);
   
    /* Configfura as páginas de login e upload de firmware OTA */
    server.on("/", HTTP_GET, []() {
        server.sendHeader("Connection", "close");
        server.send(200, "text/html", loginIndex);
    });
     
    server.on("/serverIndex", HTTP_GET, []() {
        server.sendHeader("Connection", "close");
        server.send(200, "text/html", serverIndex);
    });
   
    /* Define tratamentos do update de firmware OTA */
    server.on("/update", HTTP_POST, []() {
        server.sendHeader("Connection", "close");
        server.send(200, "text/plain", (Update.hasError()) ? "FAIL" : "OK");
        ESP.restart();
    }, []() {
        HTTPUpload& upload = server.upload();
         
             if (upload.status == UPLOAD_FILE_START) { /* Inicio do upload de firmware OTA */
          SPIFFS.end(); // SPIFFS deve ser finalizada
          mostrar_display("Iniciando", "UPDATE", "", "...", 16, 0);
          if (!Update.begin(UPDATE_SIZE_UNKNOWN)) Update.printError(Serial);
        }
        else if (upload.status == UPLOAD_FILE_WRITE) { /* Escrevendo firmware enviado na flash do ESP32 */
            if (Update.write(upload.buf, upload.currentSize) != upload.currentSize) Update.printError(Serial);
            else mostrar_display("Atualizando", "", "Firmware...", "", 16, 0);
        } 
        else if (upload.status == UPLOAD_FILE_END)   { /* Final de upload */
            if (Update.end(true)) mostrar_display("Sucesso", "UPDATE", "", "OK", 16, 1);
            else mostrar_display("ERROR", "UPDATE", "", ":(", 16, 1);
        }   
    
    });
    server.begin();
}

void Formatar_data_hora(){ // A cada 8 segundos atualiza a Hora, Apita de Hora em Hora, Avisa se Iniciou e Envia os LOGs OffLine
       if(weekday() == 1){dia_semana = "Dom";}
  else if(weekday() == 2){dia_semana = "Seg";}
  else if(weekday() == 3){dia_semana = "Ter";}
  else if(weekday() == 4){dia_semana = "Qua";}
  else if(weekday() == 5){dia_semana = "Qui";}
  else if(weekday() == 6){dia_semana = "Sex";}
  else if(weekday() == 7){dia_semana = "Sab";}

  String hora1 = hour()   < 10 ? "0" + String(hour())   : String(hour());
  String minu1 = minute() < 10 ? "0" + String(minute()) : String(minute());
  String segu1 = second() < 10 ? "0" + String(second()) : String(second());
  String dia1  = day()    < 10 ? "0" + String(day())    : String(day());
  String mes1  = month()  < 10 ? "0" + String(month())  : String(month());
  String ano1  = year()   < 10 ? "0" + String(year())   : String(year());

  IP = WiFi.localIP().toString().c_str();
  sinal_wifi = String(WiFi.RSSI()) + "dB";
  temp_int = ((temprature_sens_read() - 32 ) / 1.8); // Temperatura do ESP32
  
  hora  = hora1 + ":" + minu1 + ":" + segu1;
  data_ = dia1  + "/" + mes1  + "/" + ano1;
  
  currentTime = hora + " " + data_;

  if(error_conect == true && iniciar == false){  // apenas quando ligar
    iniciar = true;
    mostrar_terminal("Iniciado");
  }
}

void iniciar_OTA(){
  if(WiFi.status() == WL_CONNECTED){
    ArduinoOTA.setHostname("Monitor_Tensoes");
    ArduinoOTA.begin();
    ArduinoOTA.onStart([]() {
      SPIFFS.end(); // SPIFFS deve ser finalizada
      display1.clear();
      display1.setFont(ArialMT_Plain_16);
      display1.setTextAlignment(TEXT_ALIGN_CENTER_BOTH);
      display1.drawString(display1.getWidth()/2, display1.getHeight()/2 - 10, "Atualizando...");
      display1.display(); });
    ArduinoOTA.onProgress([](unsigned int progress, unsigned int total) {
      esp_task_wdt_reset();
      display1.drawProgressBar(4, 32, 120, 8, progress / (total / 100) );
      display1.display(); });
    ArduinoOTA.onEnd([]() {
      display1.clear();
      display1.setFont(ArialMT_Plain_16);
      display1.setTextAlignment(TEXT_ALIGN_CENTER_BOTH);
      display1.drawString(display1.getWidth()/2, display1.getHeight()/2, "Reiniciando...");
      display1.display(); });
      mostrar_display("UPDATE", "OTA", "", "OK", 16, 1);
  }
}

void conecta_wifi_blynk(){

  if(WiFi.status() != WL_CONNECTED){
    mostrar_display("Desconectando", "WIFI", "", "", 16, 1);
    WiFi.disconnect(); 
    delay(100);
    WiFi.mode(WIFI_STA);
    mostrar_display("Conectando", "WIFI", "", ssid, 16, 1);
    delay(100);
    WiFi.begin(ssid, pw);
    delay(100);
    int x = 0;
    String y = "";
    while ( x <= 20) {
      if (WiFi.status() == WL_CONNECTED){ x = 50; break; }
      x++;
      y = String(x) + " de 20";
      mostrar_display("Conectando", "Ao WIFI", y, "", 16, 1);
    }
    if (WiFi.status() == WL_CONNECTED){
      mostrar_display("", "WIFI OK", "", "",16, 1);
    }
    else{
      mostrar_display("", "WIFI", "Nao conectado", "",16, 1);
    }
  }
  
  if(!Blynk.connected()) {
      if (WiFi.status() == WL_CONNECTED){ // Só entra se já tiver conectado no WIFI
        Blynk.disconnect();
        mostrar_display("Conectando", "", "Blynk" , "",16, 1);
        Blynk.config(auth);
        delay(500);
        Blynk.connect(9000);
        error_conect = Blynk.connected();
        if (!error_conect) {
          mostrar_display("CONEXAO", "Blynk", "FALHOU" , "",16, 1);
        }
        else{
          Blynk.run();
          mostrar_display("CONEXAO", "Blynk", "OK", "",16, 1);
        }
        error_conect = Blynk.connected();
    }
  }

}
