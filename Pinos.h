void iniciar_PINs(){
  pinMode(led_azul,    OUTPUT); digitalWrite(led_azul,    LOW);
  pinMode(rele1,       OUTPUT); digitalWrite(rele1,       LOW);
  pinMode(rele2,       OUTPUT); digitalWrite(rele2,       LOW);
  pinMode(Reset_Sinal, OUTPUT); digitalWrite(Reset_Sinal, LOW);
  pinMode(config_wifi, INPUT);

}

void pino(byte pin, byte nivel){
  if(nivel == 0){digitalWrite(pin, LOW);}
  if(nivel == 1){digitalWrite(pin, HIGH);}
}
