#ifndef TTNMAD_E78_H
#define TTNMAD_E78_H

#define default_timeout_ms 1000
#define CLASS_A "0"
#define CLASS_B "1"
#define CLASS_C "2"

#include <Arduino.h>

class TTNMAD_E78 {
  public:
    TTNMAD_E78(Stream &ser);
    TTNMAD_E78(Stream &ser, Stream &debug);
    byte getManufacturer(char * respuesta, uint16_t timeout = default_timeout_ms);
    byte getModel(char * respuesta, uint16_t timeout = default_timeout_ms);
    byte getRevision(char * respuesta, uint16_t timeout = default_timeout_ms);
    byte getSN(char * respuesta, uint16_t timeout = default_timeout_ms);
    byte getBaudRate(char * respuesta, uint16_t timeout = default_timeout_ms);
    byte setBaudRate(unsigned long baudrate, char * respuesta, uint16_t timeout = default_timeout_ms);
    byte getClass(char * respuesta, uint16_t timeout = default_timeout_ms);
    byte setClass(char * lorawanClass, char * respuesta, uint16_t timeout = default_timeout_ms);
    byte initOTAA(char * DevEUI, char * AppEUI, char * AppKey, char * respuesta, uint16_t timeout = default_timeout_ms);
    byte getStatus(char * respuesta, uint16_t timeout = default_timeout_ms);
    byte send(bool confirmacion, char * payload, char * respuesta, uint16_t timeout = default_timeout_ms);
    byte reset(char * respuesta, uint16_t timeout = default_timeout_ms);
    byte join(char * respuesta, uint16_t timeout = default_timeout_ms);
    boolean isJoined();

  private:
    byte sendCommand(char *command, char * respuesta, uint16_t timeout = default_timeout_ms);
    bool JOINED = false;
    bool OTAA = false;
    unsigned long timer;
    Stream *e78Serial;
    Stream *e78Debug;
    //char error[6] = {'E', 'R', 'R', 'O', 'R', '\0'};
    //char ok[3] = {'O', 'K', '\0'};
};


#endif
