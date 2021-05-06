void iniciar_modulos(){
  mostrar_display("Iniciando", "", "Modulos", "", 16, 1);
  dht.begin(); delay(10);
  ads1.setGain(GAIN_ONE); delay(10); ads1.begin(0X48); delay(10);
  ads2.setGain(GAIN_ONE); delay(10); ads2.begin(0X49); delay(10);
}

void leituras_dht(){
  umi_dht  = dht.readHumidity();
  temp_dht = dht.readTemperature();
  
  if (isnan(umi_dht) || isnan(temp_dht)){
    dht_str = "Erro DHT22";
    return;
  }
  else{
    dht_str = String(temp_dht,1) + "C, " + String(umi_dht,0) + "%";
  }
}

float ADS1115(String ADSx, byte porta_adc, byte QTD_leituras, float multiplicador){
  float media     = 0.0;
  float tensao    = 0.0;

  for ( int h=0; h < QTD_leituras; h++) {
    if(ADSx == "ADS1"){media+= ads1.readADC_SingleEnded(porta_adc);}
    if(ADSx == "ADS2"){media+= ads2.readADC_SingleEnded(porta_adc);}
    delay(4);
  }
  
  media = media / QTD_leituras;
  if(ADSx == "ADS1"){tensao = ads1.computeVolts(media);}
  if(ADSx == "ADS2"){tensao = ads2.computeVolts(media);}
  tensao = tensao * multiplicador;
  delay(10); // Tempo entre leituras
  esp_task_wdt_reset();
  return tensao;
}

void Testar_Reles(){
  mostrar_display("Testando", "", "RELES", "", 16, 0);
  pino(rele1, 1);
  delay(500);
  pino(rele2, 1);
  delay(500);
  pino(rele1, 0);
  delay(500);
  pino(rele2, 0);
}
