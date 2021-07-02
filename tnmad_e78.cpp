#include "ttnmad_e78.h"
#define TTNMAD_E78_DEBUG //Comentar esta línea si no queremos mensajes en la consola

HardwareSerial *e78HwSerial;
HardwareSerial *e78HwDebug;

TTNMAD_E78::TTNMAD_E78(Stream &ser) {
  e78Serial = &ser;
}
TTNMAD_E78::TTNMAD_E78(Stream &ser, Stream &debug) {
  e78Serial = &ser;
  e78Debug = &debug;
}

byte TTNMAD_E78::sendCommand(char *command, char * respuesta, uint16_t timeout) {
  while (e78Serial->available()) {
    e78Serial->read(); // Clear input buffer
  }
#ifdef TTNMAD_E78_DEBUG
  e78Debug->print(F("\t---> "));
  e78Debug->println(command);
#endif
  e78Serial->println(command);

  bool gotReply = false;
  timer = millis();

  while (!gotReply && millis() - timer < timeout) {
    if (e78Serial->available()) {
      gotReply = true;
    }
  }

  if (gotReply) {
    int indice = 0;
    delay(100);
    while (indice < 299 && e78Serial->available()) {
      respuesta[indice] = e78Serial->read();
      indice++;
      delay(50);//La comunicación es muy lenta
    }
    respuesta[indice] = '\0';
#ifdef TTNMAD_E78_DEBUG
    e78Debug->print(F("\t---> "));
    e78Debug->println(respuesta);
#endif
    return 1;
  }  else {
    //No hay respuesta
#ifdef TTNMAD_E78_DEBUG
    e78Debug->print(F("\t---> ERROR"));
#endif
    return 0;
  };

}

byte TTNMAD_E78::getManufacturer(char * respuesta, uint16_t timeout) {
  char rawRespuesta[65];
  if (sendCommand("AT+CGMI?", rawRespuesta, timeout)) {
    if (memcmp(rawRespuesta, "\r\n+CGMI?=", 9) == 0) {
      char * fin = strstr(rawRespuesta, "\r\nOK");
      if (fin) {
        memcpy( respuesta, &rawRespuesta[9], (fin - rawRespuesta) - 9 );
        respuesta[(fin - rawRespuesta) - 9 ] = '\0';
        return 1;
      } else {
        return 0;
      }
    } else {
      return 0;
    }
  } else {
    return 0;
  }
}
byte TTNMAD_E78::getModel(char * respuesta, uint16_t timeout) {
  char rawRespuesta[65];
  if (sendCommand("AT+CGMM?", rawRespuesta, timeout)) {
    if (memcmp(rawRespuesta, "\r\n+CGMM?=", 9) == 0) {
      char * fin = strstr(rawRespuesta, "\r\nOK");
      if (fin) {
        memcpy( respuesta, &rawRespuesta[9], (fin - rawRespuesta) - 9 );
        respuesta[(fin - rawRespuesta) - 9 ] = '\0';
        return 1;
      } else {
        return 0;
      }
    } else {

      return 0;
    }
  } else {
    return 0;
  }
}

byte TTNMAD_E78::getRevision(char * respuesta, uint16_t timeout) {
  char rawRespuesta[65];
  if (sendCommand("AT+CGMR?", rawRespuesta, timeout)) {
    if (memcmp(rawRespuesta, "\r\n+CGMR?=", 9) == 0) {
      char * fin = strstr(rawRespuesta, "\r\nOK");
      if (fin) {
        memcpy( respuesta, &rawRespuesta[9], (fin - rawRespuesta) - 9 );
        respuesta[(fin - rawRespuesta) - 9 ] = '\0';
        return 1;
      } else {
        return 0;
      }
    } else {

      return 0;
    }
  } else {
    return 0;
  }
}

byte TTNMAD_E78::getSN(char * respuesta, uint16_t timeout) {
  char rawRespuesta[65];
  if (sendCommand("AT+CGSN?", rawRespuesta, timeout)) {
    if (memcmp(rawRespuesta, "\r\n+CGSN?=", 9) == 0) {
      char * fin = strstr(rawRespuesta, "\r\nOK");
      if (fin) {
        memcpy( respuesta, &rawRespuesta[9], (fin - rawRespuesta) - 9 );
        respuesta[(fin - rawRespuesta) - 9 ] = '\0';
        return 1;
      } else {
        return 0;
      }
    } else {
      return 0;
    }
  } else {
    return 0;
  }
}

byte TTNMAD_E78::getBaudRate(char * respuesta, uint16_t timeout) {
  char rawRespuesta[65];
  if (sendCommand("AT+CGBR?", rawRespuesta, timeout)) {
    if (memcmp(rawRespuesta, "\r\n+CGBR:", 8) == 0) {
      char * fin = strstr(rawRespuesta, "\r\nOK");
      if (fin) {
        memcpy( respuesta, &rawRespuesta[8], (fin - rawRespuesta) - 8 );
        respuesta[(fin - rawRespuesta) - 8 ] = '\0';
        return 1;
      } else {
        return 0;
      }
    } else {

      return 0;
    }
  } else {
    return 0;
  }
}

byte TTNMAD_E78::setBaudRate(unsigned long baudrate, char * respuesta, uint16_t timeout ) {

}

byte TTNMAD_E78::getClass(char * respuesta, uint16_t timeout) {
  char rawRespuesta[65];
  if (sendCommand("AT+CCLASS?", rawRespuesta, timeout)) {
    if (memcmp(rawRespuesta, "\r\n+CCLASS:", 10) == 0) {
      char * fin = strstr(rawRespuesta, "\r\nOK");
      if (fin) {
        memcpy( respuesta, &rawRespuesta[10], (fin - rawRespuesta) - 10 );
        respuesta[(fin - rawRespuesta) - 10 ] = '\0';
        return 1;
      } else {
        return 0;
      }
    } else {

      return 0;
    }
  } else {
    return 0;
  }
}

byte TTNMAD_E78::setClass(char * lorawanClass, char * respuesta, uint16_t timeout ) {
  char rawRespuesta[65];
  if (sendCommand(strcat("AT+CCLASS=", lorawanClass), rawRespuesta, timeout)) {
    if (memcmp(rawRespuesta, "\r\nOK", 4) == 0) {
      return 1;
    } else {
      return 0;
    }
  } else {
    return 0;
  }
}


byte TTNMAD_E78::initOTAA(char * DevEUI, char * AppEUI, char * AppKey, char * respuesta, uint16_t timeout ) {
  //IREBOOT=0
  //CJOIN=1,0,10,1 Para que no haga join al iniciarse
  //Creo que no hace falta CSAVE
  //IREBOOT=0
  //AT+CFREQBANDMASK=0001
  //AT+CULDLMODE=2 y demás parámetros MAC
  //CSAVE
  char rawRespuesta[65];
  char comando[100];
  byte reintentos = 0;
  byte maxReintentos = 5;
  while (reintentos < maxReintentos) {
    if (sendCommand("AT+IREBOOT=0", rawRespuesta, timeout)) {
      if (memcmp(rawRespuesta, "\r\nOK\r\n", 6) == 0) {
        //Continuar
        break;
      } else {
        //Reintentar?
        delay(500);
      }
    } else {
      //Reintentar?
      delay(500);
    }
    reintentos++;

  }
  if (reintentos == maxReintentos) {
    strcpy(respuesta,"ERROR IREBOOT 1");    
    return 0;
  }

  //El reboot puede llevar unos intantes => hago reintentos
  reintentos = 0;
  while (reintentos < maxReintentos) {
    if (sendCommand("AT+CJOIN=1,0,10,1", rawRespuesta, timeout)) {
      if (memcmp(rawRespuesta, "\r\nOK\r\n", 6) == 0) {
        //Continuar
        break;
      } else {
        //Reintentar?
        delay(500);
      }
    } else {
      //Reintentar?
      delay(500);
    }
    reintentos++;
  }
  if (reintentos == maxReintentos) {
    strcpy(respuesta,"ERROR CJOIN=1,0,10,1");    
    return 0;
  }

  reintentos = 0;
  while (reintentos < maxReintentos) {
    if (sendCommand("AT+IREBOOT=0", rawRespuesta, timeout)) {
      if (memcmp(rawRespuesta, "\r\nOK\r\n", 6) == 0) {
        //Continuar
        break;
      } else {
        //Reintentar?
        delay(500);
      }
    } else {
      //Reintentar?
      delay(500);
    }
    reintentos++;
  }
  if (reintentos == maxReintentos) {
    strcpy(respuesta,"ERROR IREBOOT 2");   
    return 0;
  }
  //El reboot puede llevar unos intantes => hago reintentos
  reintentos = 0;
  while (reintentos < maxReintentos) {
    if (sendCommand("AT+CFREQBANDMASK=0001", rawRespuesta, timeout)) {
      if (memcmp(rawRespuesta, "\r\nOK\r\n", 6) == 0) {
        //Continuar
        break;
      } else {
        //Reintentar?
        delay(500);
      }
    } else {
      //Reintentar?
      delay(500);
    }
    reintentos++;
  }
  if (reintentos == maxReintentos) {
    strcpy(respuesta,"ERROR CFREQBANDMASK");
    return 0;
  }
  if (sendCommand("AT+CULDLMODE=2", rawRespuesta, timeout)) {
    if (memcmp(rawRespuesta, "\r\nOK\r\n", 6) == 0) {
      //Continuar
    } else {
      return 0;
    }
  } else {
    return 0;
  }

  if (sendCommand("AT+CJOINMODE=0", rawRespuesta, timeout)) {
    if (memcmp(rawRespuesta, "\r\nOK\r\n", 6) == 0) {
      //Continuar
    } else {
      return 0;
    }
  } else {
    return 0;
  }

  strcpy(comando, "AT+CDEVEUI=");
  if (sendCommand(strcat(comando, DevEUI), rawRespuesta, timeout)) {

    if (memcmp(rawRespuesta, "\r\nOK\r\n", 6) == 0) {
      //Continuar
    } else {
      return 0;
    }
  } else {
    return 0;
  }
  strcpy(comando, "AT+CAPPEUI=");
  if (sendCommand(strcat(comando, AppEUI), rawRespuesta, timeout)) {

    if (memcmp(rawRespuesta, "\r\nOK\r\n", 6) == 0) {
      //Continuar
    } else {
      return 0;
    }
  } else {
    return 0;
  }
  strcpy(comando, "AT+CAPPKEY=");
  if (sendCommand(strcat(comando, AppKey), rawRespuesta, timeout)) {

    if (memcmp(rawRespuesta, "\r\nOK\r\n", 6) == 0) {
      //Continuar
    } else {
      return 0;
    }
  } else {
    return 0;
  }

  if (sendCommand("AT+CSAVE", rawRespuesta, timeout)) {
    if (memcmp(rawRespuesta, "\r\nOK\r\n", 6) == 0) {
      //Continuar
      OTAA = true;
      strcpy(respuesta,"OK");     
      return 1;
    } else {
      return 0;
    }
  } else {
    return 0;
  }

}
byte TTNMAD_E78::getStatus(char * respuesta, uint16_t timeout) {

  char rawRespuesta[65];
  if (sendCommand("AT+CSTATUS?", rawRespuesta, timeout)) {
    if (memcmp(rawRespuesta, "\r\n+CSTATUS:", 11) == 0) {
      char * fin = strstr(rawRespuesta, "\r\nOK");
      if (fin) {
        memcpy( respuesta, &rawRespuesta[11], (fin - rawRespuesta) - 11 );
        respuesta[(fin - rawRespuesta) - 11 ] = '\0';
        return 1;
      } else {
        return 0;
      }
    } else {

      return 0;
    }
  } else {
    return 0;
  }
}
byte  TTNMAD_E78::join(char * respuesta, uint16_t timeout ) {
  //El proceso de joining puede requerir hasta 5  minutos
  //Después del JOIN parece que el E78 envía uno o incluso dos uplinks
  //por lo que podríamos recibir un downlink
  // AT+CJOIN=1,0,10,1
  //OK
  //+CJOIN:OK
  //OK+SENT:01
  //OK+RECV:02,00,00
  char rawRespuesta[300];
  JOINED = false;
  byte reintentos = 0;
  byte maxReintentos = 5;
  while (reintentos < maxReintentos) {
    if (sendCommand("AT+CJOIN=1,0,10,1", rawRespuesta, timeout)) {
      if (memcmp(rawRespuesta, "\r\nOK\r\n", 6) == 0) {
        //Continuar
        break;
      } else {
        //Reintentar?
        delay(500);
      }
    } else {
      //Reintentar?
      delay(500);
    }
    reintentos++;

  }
  if (reintentos == maxReintentos) {
    return 0;
  }
  //Si llegamos aquí es que ha empezado el proceso de JOINING
  //Simplemente esperamos a tener una respuesta antes de 5 minutos
  //Si la respuesta no es +CJOIN:OK => ERROR
  //Pero si la respuesta es +CJOIN:OK => Tenemos que esperar una respuesta
  bool gotReply = false;
  timer = millis();
  while (!gotReply && millis() - timer < 5 * 60 * 1000) {
    if (e78Serial->available()) {
      gotReply = true;
    }
  }
  if (!gotReply) {
    //No se ha obtenido respuesta del comando JOIN
    strcpy(respuesta,"JOIN NO RESPONDE");
    return 0;
  } else {
    int indice = 0;
    delay(100);
    while (indice < 299 && e78Serial->available()) {
      rawRespuesta[indice] = e78Serial->read();
      indice++;
      delay(50);//La comunicación es muy lenta
    }
    rawRespuesta[indice] = '\0';
    if (memcmp(rawRespuesta, "+CJOIN:OK\r\n", 11) == 0) {
      //Continuar
    }  else {
      strcpy(respuesta,"JOIN NO OK");
      return 0;
    };
    //Si llego aquí es porque ha conseguido el JOIN OK, y tengo que esperar una posible respuesta al
    //UPLINK que envía el E78 (voy a suponer que el RX1 puede ser a lo sumo de 10 segundos)
    JOINED = true;
    timer = millis();
    gotReply = false;
    while (!gotReply && millis() - timer < 10000) {
      if (e78Serial->available()) {
        gotReply = true;
      }
    }
    if (!gotReply) {
      //No se ha recibido nada más después del JOIN
      strcpy(respuesta,"");
      return 1;
    } else {
      indice = 0;
      delay(100);
      while (indice < 299 && e78Serial->available()) {
        rawRespuesta[indice] = e78Serial->read();
        indice++;
        delay(50);//La comunicación es muy lenta
      }
      rawRespuesta[indice] = '\0';

      if (memcmp(rawRespuesta, "\r\nOK+SENT:01\r\n\r\nOK+RECV:", 24) == 0) {
        //Tenemos respuesta
        memcpy( respuesta, &rawRespuesta[24], indice);
        return 1;
      }  else {
        strcpy(respuesta,"");
        return 1;
      };
    }
  }
}

boolean  TTNMAD_E78::isJoined() {
  return JOINED;
}
byte  TTNMAD_E78::send(bool confirmacion, char * payload, char * respuesta, uint16_t timeout ) {
  char rawRespuesta[300];
  char comando[255];
  //OTAA o ABP?
  if (OTAA) {
    if (!JOINED) {
      strcpy(respuesta,"NOT JOINED");
      return 0;
    }
  }

  //Con el timeout normal podemos esperar OK+SEND:
  //Para el timeout del sent debemos tener en cuenta el de RX1
  uint16_t timeoutRX1;
  if (sendCommand("AT+CRX1DELAY?", rawRespuesta, timeout)) {
    if (memcmp(rawRespuesta, "\r\n+CRX1DELAY:", 13) == 0) {
      char * fin = strstr(rawRespuesta, "\r\nOK");
      if (fin) {
        memcpy( respuesta, &rawRespuesta[13], (fin - rawRespuesta) - 13 );
        respuesta[(fin - rawRespuesta) - 13 ] = '\0';
        //Continuar
      } else {
        strcpy(respuesta, "ERROR AT+CRX1DELAY?");
        return 0;
      }
    } else {

      strcpy(respuesta, "ERROR AT+CRX1DELAY?");
      return 0;
    }
  } else {
    strcpy(respuesta, "ERROR AT+CRX1DELAY?");
    return 0;
  }
  timeoutRX1 = atoi(respuesta) * 1000;

  //No hago un reintentos
  if (confirmacion) {
    sprintf(comando, "AT+DTXR=1,%d,%d,%s", 0, strlen(payload), payload);
  } else {
    sprintf(comando, "AT+DTRX=0,%d,%d,%s", 0, strlen(payload), payload);
  }

  if (sendCommand(comando, rawRespuesta, timeout)) {
    if (memcmp(rawRespuesta, "\r\nOK+SEND:", 10) == 0) {
      //Continuar
    } else {
      strcpy(respuesta, "ERROR SEND");
      return 0;
    }
  } else {
    strcpy(respuesta, "ERROR SEND");
    return 0;
  }

  bool gotReply = false;
  timer = millis();
  while (!gotReply && ((millis() - timer) < (timeout + timeoutRX1+2000))) {
    if (e78Serial->available()) {
      gotReply = true;
    }
  }

  if (gotReply) {
    int indice = 0;
    while (indice < 299 && e78Serial->available()) {
      respuesta[indice] = e78Serial->read();
      indice++;
      delay(50);//La comunicación es muy lenta
    }
    respuesta[indice] = '\0';
    return 1;
  } else {
    strcpy(respuesta, "");
    return 0;
  }
}

byte TTNMAD_E78::reset( char * respuesta, uint16_t timeout ) {
  char rawRespuesta[65];
  if (sendCommand("AT+IREBOOT=1", rawRespuesta, timeout)) {
    if (memcmp(rawRespuesta, "\r\nOK", 4) == 0) {
      return 1;
    } else {
      return 0;
    }
  } else {
    return 0;
  }
}
