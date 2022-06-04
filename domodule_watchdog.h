/*
 * domodule_watchdog.h
 *
 *  Created on: 3 juin 2022
 *      Author: julien
 */

#ifndef DOMODULE_WATCHDOG_H_
#define DOMODULE_WATCHDOG_H_

#include "domodule_capteur.h"

class Class_ConfigModuleWatchdog: public Class_ConfigModule
{
public:
  Class_ConfigModuleWatchdog(const char i_ps8_nomModule[24] = "", TimerTime_t i_u64_periode = 0) :
      Class_ConfigModule(i_ps8_nomModule), Periode(i_u64_periode)
  {
  }

  TimerTime_t Periode;
};


class Domodule_Watchdog: public Domodule_Capteur
{
public:
  Domodule_Watchdog(const char i_ps8_nomModule[24] = "", TimerTime_t i_u64_tempsWatchdog = 0);
  virtual ~Domodule_Watchdog();
  Domodule_Watchdog(const Domodule_Watchdog &other) = delete;
  Domodule_Watchdog(Domodule_Watchdog &&other) = delete;
  Domodule_Watchdog& operator=(Domodule_Watchdog &&other) = delete;
  Domodule_Watchdog& operator=(const Domodule_Watchdog &other) = delete;

  virtual void setConfigModule(const Class_ConfigModuleWatchdog &i_m_configModuleWatchdog);

  virtual void moduleON(void) override;
  virtual void moduleOFF(void) override;

  virtual void resetWatchdog(void);

protected:
  static void trigWatchdogStatic(uint32_t i_u32_valeur, void *Arg);


};

#endif /* DOMODULE_WATCHDOG_H_ */
