#ifndef _LIPOTESTER_H
#define _LIPOTESTER_H

#include <Arduino.h>

class Lipotester {
  public:
    Lipotester();
    void setup();
    void loop();
  private:
    void getValues();
    void showValues();
};

#endif