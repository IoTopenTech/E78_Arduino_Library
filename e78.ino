//http://www.hoperf.com/data/upload/back/20190605/ASR650X%20AT%20Command%20Introduction-20190605.pdf
//https://github.com/Edragon/ASK6501-SDK2/tree/main/DOCs
//Uso la 4.4 del firmware de alios
//Para evitar que el E78 sea demasiado verboso, comento esta línea
//PRINTF_RAW("\r\n%s%s:~# ", CONFIG_MANUFACTURER, CONFIG_DEVICE_MODEL);
//en el archivo linkwan_ica_at.c
//y añado este define en las definiciones del preprocesador
//CONFIG_PRINT_ECHO_DISABLE 
//El E78 parece venir por defecto con baudrate 115200
//Pero con el cristal de 8MHz va muy al límite, por lo que lo cambio a 57600
//AT+CGBR=57600
//También le pongo el log level a 0
//AT+ILOGLVL=0



#include "ttnmad_e78.h"

//Provisión para poder usar SoftwareSerial
//SoftwareSerial e78SS = SoftwareSerial(MCU_RX, MCU_TX);
//SoftwareSerial *e78Serial = &e78SS;


HardwareSerial *e78Serial = &Serial;
HardwareSerial *e78Debug = &Serial1;

char e78Respuesta[300]; //Buffer para las respuestas de los comandos

void setup() {
  Serial.begin(57600); //e78
  Serial1.begin(57600); //debug
  delay(1000);
  TTNMAD_E78 e78(*e78Serial, *e78Debug);

  if (e78.initOTAA("DEVEUI", "APPEUI", "APPKEY", e78Respuesta, 1000)) {
    Serial1.println(e78Respuesta);
  } else {
    Serial1.println("ERROR");
  }
  
  if(e78.join(e78Respuesta, 1000)){
    Serial1.print("Respuesta del JOIN: ");
    Serial1.println(e78Respuesta);
    e78.send(false, "ABCDEF", e78Respuesta, 1000);
    Serial1.print("Respuesta del SEND: ");
    Serial1.println(e78Respuesta);
  }else{
    Serial1.println("JOIN MAL");
  }
  
  /*
    byte status;
    int respuestaInt;
    do{
    delay(2000);
    status=e78.getStatus(e78Respuesta, 1000);
    respuestaInt=atoi(e78Respuesta);
    }while(!status||(respuestaInt!=0&&respuestaInt!=7&&respuestaInt!=8));

    if (e78.setOTAA("0000000000000001", "ABCDEFABCDEF0001", "F53F8C86223FD5666A3698507F971227", e78Respuesta, 1000)) {
      Serial1.println(e78Respuesta);
    } else {
      Serial1.println("ERROR");
    }



    Serial1.println("____");
    delay(10000);
    //e78.send(false, 1, "ABCDEF", e78Respuesta, 1000);
  */

}

void loop() {
  while (Serial.available() > 0) {
    Serial1.write(Serial.read());
  }
  while (Serial1.available() > 0) {
    Serial.write(Serial1.read());
  }
}
