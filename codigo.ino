#include "definicoes.h"
#include "configs.h"
#include "variaveis.h"
#include "Pinos.h"
#include "funcoes_gerais.h"
#include "Modulos.h"

BLYNK_READ(V2) {Blynk.virtualWrite(V2, dht_str + " - " + bat1_str + " - " + bat2_str);}
BLYNK_READ(V3) {Blynk.virtualWrite(V3, AC_str1 + " - " + AC_str2 + " - " + AC_str3 + " - " + AC_str4);}
BLYNK_WRITE(V4) { // Relé 1
  int buttonState = param.asInt();
    if (buttonState == 1) { pino(rele1, 1); mostrar_terminal ("Relé1 ON");  }
    if (buttonState == 0) { pino(rele1, 0); mostrar_terminal ("Relé1 OFF"); }
}
BLYNK_WRITE(V5) { // Relé 2
  int buttonState = param.asInt();
    if (buttonState == 1) { pino(rele2, 1); mostrar_terminal ("Relé2 ON");  }
    if (buttonState == 0) { pino(rele2, 0); mostrar_terminal ("Relé2 OFF"); }
}

BLYNK_WRITE(V1){ // Comandos terminal
  String log_terminal = param.asStr();
       if(log_terminal.substring(0,4)  == "Info")        {
        terminal.clear();
        terminal.println("IP: " + IP + " " + sinal_wifi); terminal.flush();
        terminal.println("Rede: " + String(ssid));        terminal.flush();
        terminal.println("Temp. Interna:   " + String(temp_int) + "°C"); terminal.flush();
        terminal.println("Limite RedeAC-:  " + String(limite_AC)      + "V");     terminal.flush();
        terminal.println("Limite RedeAC+:  " + String(limite_AC_mais) + "V");     terminal.flush();
        terminal.println("Limite AC Mono-: " + String(limite_AC_mono) + "V"); terminal.flush();
        terminal.println("Limite AC Mono+: " + String(limite_AC_mais_mono) + "V"); terminal.flush();
        terminal.println("Limite Baterias: " + String(tensao_bateria) + "V"); terminal.flush();
  }
  else if(log_terminal.substring(0,3)  == "Cmd")         {
        terminal.clear();
        terminal.println(F("Limpar Limpar o terminal"));                 terminal.flush();
        terminal.println(F("Info Mostra as configurações salvas "));     terminal.flush();
        terminal.println(F("Tensaoac-105 Limite rede AC-: 105v")); terminal.flush();
        terminal.println(F("Tensaoac+140 Limite rede AC+: 140v")); terminal.flush();
        terminal.println(F("Bat2_45 Limite minimo da bateria para 45v")); terminal.flush();
        terminal.println(F("Monofasica-105 Limite rede AC Mono-: 105v")); terminal.flush();
        terminal.println(F("Monofasica+130 Limite rede AC Mono+: 130v")); terminal.flush();
        terminal.println(F("Reset Da um reboot no sistema"));            terminal.flush();
  }
  else if(log_terminal.substring(0,6)  == "Limpar")      {
    terminal.clear();
  }
  else if(log_terminal.substring(0,5)  == "Reset")       {
  terminal.clear();
  delay(500);
  mostrar_terminal ("Resetando...");
  esp_task_wdt_init(1,true); // define o whachDog pra 1 segundo
  esp_task_wdt_add(NULL);    // adiciona o processo atual
  while(true); // loop infinito pra resetar o ESP32
  }
  else if(log_terminal.substring(0,9)  == "Tensaoac-")   {
    String local = log_terminal.substring(9);
    limite_AC = local.toInt();
    if(limite_AC == 0){
      terminal.println("Digite um número válido!"); terminal.flush();
    }
    else{
    EEPROM.write(0, limite_AC);
    EEPROM.commit();
    terminal.println("Limite Tensão AC: -" + String(limite_AC) + " Volts"); terminal.flush();
    }
  }
  else if(log_terminal.substring(0,9)  == "Tensaoac+")   {
    String local = log_terminal.substring(9);
    limite_AC_mais = local.toInt();
    if(limite_AC_mais == 0){
      terminal.println("Digite um número válido!"); terminal.flush();
    }
    else{
    EEPROM.write(1, limite_AC_mais);
    EEPROM.commit();
    terminal.println("Limite Tensão AC: +" + String(limite_AC_mais) + " Volts"); terminal.flush();
    }
  }
  else if(log_terminal.substring(0,11)  == "Monofasica-"){
    String local = log_terminal.substring(11);
    limite_AC_mono = local.toInt();
    if(limite_AC_mono == 0){
      terminal.println("Digite um número válido!"); terminal.flush();
    }
    else{
    EEPROM.write(3, limite_AC_mono);
    EEPROM.commit();
    terminal.println("Limite AC Mono: -" + String(limite_AC_mono) + " Volts"); terminal.flush();
    }
  }
  else if(log_terminal.substring(0,11)  == "Monofasica+"){
    String local = log_terminal.substring(11);
    limite_AC_mais_mono = local.toInt();
    if(limite_AC_mais_mono == 0){
      terminal.println("Digite um número válido!"); terminal.flush();
    }
    else{
    EEPROM.write(4, limite_AC_mais_mono);
    EEPROM.commit();
    terminal.println("Limite AC Mono: +" + String(limite_AC_mais_mono) + " Volts"); terminal.flush();
    }
  }
  else if(log_terminal.substring(0,5)  == "Bat2_")       {
    String local = log_terminal.substring(5);
    tensao_bateria = local.toInt();
    if(tensao_bateria == 0){
      terminal.println("Digite um número válido!"); terminal.flush();
    }
    else{
    EEPROM.write(2, tensao_bateria);
    EEPROM.commit();
    terminal.println("Limite Bateria: " + String(tensao_bateria) + " Volts"); terminal.flush();
    }
  }
  else{
    terminal.println("Comando Inválido!");        terminal.flush();  
  }
}

void config_rede(){
  WiFi.softAP("Monitor_Tensoes");
  server.on("/", handleRoot);
  server.on("/form", HTTP_POST, formularioEnviado);
  server.begin();
  mostrar_display("Modo", "Config", "Ativado", "", 16, 1);
}

void normal(){
  esp_task_wdt_init(40, true); // Segundos de TimeOut e True é para reiniciar
  esp_task_wdt_add(NULL);
  info();
  conecta_wifi_blynk();
  iniciar_modulos();
  iniciar_OTA();
  //Testar_Reles();
  setSyncInterval(10 * 60);
  timer.setInterval( 1000L,  verifica_conexao_blynk);
  timer.setInterval( 3000L,  ler_modulos_I2C);
  timer.setInterval( 5000L,  Formatar_data_hora);
  timer.setInterval( 9000L,  leituras_dht);
  timer.setInterval(15000L,  verificar_limites_tensao);
  timer.setInterval(30000L,  conecta_wifi_blynk);
  config_OTA_WebServer();
}

void normal_loop(){
  ArduinoOTA.handle();
  server.handleClient();
  timer.run();
  if(error_conect == true){ Blynk.run(); }
  error_conect = Blynk.connected();
}

void ler_modulos_I2C(){
  bat1 = ADS1115("ADS1", 3, 10, 9.07); bat1_str = String(bat1,1) + "Vdc";
  bat2 = ADS1115("ADS1", 2, 10, 21.2); bat2_str = String(bat2,1) + "Vdc";

  tensaoAC1 = ADS1115("ADS2", 0, 10, 707); AC_str1  = String(tensaoAC1,0)  + "Ac1";
  tensaoAC2 = ADS1115("ADS2", 1, 10, 707); AC_str2  = String(tensaoAC2,0)  + "Ac2";
  tensaoAC3 = ADS1115("ADS2", 2, 10, 707); AC_str3  = String(tensaoAC3,0)  + "Ac3";
  tensaoAC4 = ADS1115("ADS2", 3, 10, 707); AC_str4  = String(tensaoAC4,0)  + "Ac Mono";

  troca_display++;

  if(troca_display % 2 == 0){mostrar_display(conexao, bat1_str, bat2_str, dht_str, 16, 0);}
  else{mostrar_display(AC_str4, AC_str1, AC_str2, AC_str3,   16, 0);}
  
}

void verificar_limites_tensao(){
  if(limite_AC1_trava == false){ if(tensaoAC1  < limite_AC) {temp = device_str + " - VAC1 MENOR que o limite."; limite_AC1_trava = true; Blynk.notify(temp); mostrar_terminal("VAC1 Baixa"); } }
  if(limite_AC2_trava == false){ if(tensaoAC2  < limite_AC) {temp = device_str + " - VAC2 MENOR que o limite."; limite_AC2_trava = true; Blynk.notify(temp); mostrar_terminal("VAC2 Baixa"); } }
  if(limite_AC3_trava == false){ if(tensaoAC3  < limite_AC) {temp = device_str + " - VAC3 MENOR que o limite."; limite_AC3_trava = true; Blynk.notify(temp); mostrar_terminal("VAC3 Baixa"); } }
  if(limite_AC_trava_mono == false){ if(tensaoAC4  < limite_AC_mono) {temp = device_str + " - AC Mono MENOR que o limite."; limite_AC_trava_mono = true; Blynk.notify(temp); mostrar_terminal("VAC Mono Baixa"); } } 
  
  if(limite_AC1_trava == true){ if(tensaoAC1  > limite_AC_mais) { limite_AC1_trava = false; mostrar_terminal("VAC1 Normali."); } }
  if(limite_AC2_trava == true){ if(tensaoAC2  > limite_AC_mais) { limite_AC2_trava = false; mostrar_terminal("VAC2 Normali."); } }
  if(limite_AC3_trava == true){ if(tensaoAC3  > limite_AC_mais) { limite_AC3_trava = false; mostrar_terminal("VAC3 Normali."); } }
  if(limite_AC_trava_mono == true){ if(tensaoAC4  > limite_AC_mais_mono) { limite_AC_trava_mono = false; mostrar_terminal("AC Mono Normali."); } }

  if(bateria_trava == false) { if(bat2 < tensao_bateria)     { bateria_trava = true; temp = device_str + " - Bateria MENOR que o limite estabelecido!";   Blynk.notify(temp); mostrar_terminal("Bateria Baixa"); } }
  if(bateria_trava == true)  { if(bat2 > tensao_bateria + 5) { bateria_trava = false; mostrar_terminal("Bateria Normali."); } }  

}

void setup() {
  delay(50);
  EEPROM.begin(6);
  limite_AC      = EEPROM.read(0);
  limite_AC_mais = EEPROM.read(1);
  tensao_bateria = EEPROM.read(2);
  limite_AC_mono = EEPROM.read(3);
  limite_AC_mais_mono = EEPROM.read(4);
  
  iniciar_PINs();
  iniciar_display();
  iniciar_SPIFFS();
  WiFi.disconnect(); 
  delay(500);

  if(!digitalRead(config_wifi)){modo_config = true;} // Iniciar normalmente ou modo Config
  if(modo_config == true){config_rede();} else {normal();}
}

void loop() {
  esp_task_wdt_reset();
  if(modo_config == true){ server.handleClient(); }
  else{ normal_loop(); }
}
