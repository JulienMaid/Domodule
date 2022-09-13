/*
 * domodule_watchdog.cpp
 *
 *  Created on: 3 juin 2022
 *      Author: julien
 */

#include "domodule_watchdog.h"

Domodule_Watchdog::Domodule_Watchdog(const char i_ps8_nomModule[24],
    TimerTime_t i_u64_tempsWatchdog) :
    Domodule_Capteur(i_ps8_nomModule, 0, false)
{
  m_TimerModule.Init(Domodule_Watchdog::trigWatchdogStatic, i_u64_tempsWatchdog, false, this);
}

Domodule_Watchdog::~Domodule_Watchdog()
{
  m_TimerModule.Stop();
}

void Domodule_Watchdog::setConfigModule(const Class_ConfigModuleWatchdog &i_m_configModuleWatchdog)
{
  m_TimerModule.Init(Domodule_Watchdog::trigWatchdogStatic, i_m_configModuleWatchdog.Periode, false,
      this);

  Domodule::setConfigModule(i_m_configModuleWatchdog);
}

void Domodule_Watchdog::moduleON(void)
{
  m_TimerModule.Start();
}

void Domodule_Watchdog::moduleOFF(void)
{
  m_TimerModule.Stop();
}

void Domodule_Watchdog::resetWatchdog(void)
{
  m_TimerModule.Reload();
}

void Domodule_Watchdog::trigWatchdogStatic(uint32_t i_u32_valeur, void *Arg)
{
  Domodule_Watchdog *Module = (Domodule_Watchdog*) Arg;
  (void) i_u32_valeur;

  Module->declencherTrigActions();
}
