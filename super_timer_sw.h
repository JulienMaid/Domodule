/*
 * super_timer_sw.h
 *
 *  Created on: 19 janv. 2018
 *      Author: julien
 */

#pragma once

#include "timer_sw.h"

typedef struct
{
    TimerTime_t Mini_Periode;
    uint8_t Nbre_Mini_Periodes;
    TimerTime_t Periode;
} Super_TimerTime_t;

class Super_TimerEvent_t: public virtual TimerEvent_t
{
protected:
    TimerTime_t Mini_Periode;
    uint8_t Nbre_Mini_Periodes;
    uint8_t Index_Scenario;
    TimerTime_t Periode;
    bool Super_Periodic;

    Super_TimerTime_t * Nouvelle_Config;

public:
  Super_TimerEvent_t();
  virtual ~Super_TimerEvent_t();


  void Init(void (*Callback_Fct)(uint32_t, void*), TimerTime_t Arg_Mini_Periode, uint8_t Arg_Nbre_Mini_Periodes,
      TimerTime_t Arg_Periode, bool Arg_Perodic = true, void *ArgCallBack = NULL, const char Label[] = NULL);
  void Start(void);

  void SetValue(TimerTime_t Arg_Mini_Periode, uint8_t Arg_Nbre_Mini_Periodes, TimerTime_t Arg_Periode);

  void Reload(void);
};
