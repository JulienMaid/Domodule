/*
 * domodule_pilotage_voyant.cpp
 *
 *  Created on: 30 avr. 2022
 *      Author: julien
 */

#include "domodule_pilotage_voyant.h"
#ifdef TRACE_DEBUG_ACTIVES
#include "trace_debug.h"
#endif

Domodule_Pilotage_Voyant::Domodule_Pilotage_Voyant() :
    u8_etatPort(u8_EtatON)
{
}

Domodule_Pilotage_Voyant::~Domodule_Pilotage_Voyant()
{
  m_TimerModule.Delete();
  m_TimerBlink.Delete();
}

void Domodule_Pilotage_Voyant::moduleON(void)
{
#ifdef TRACE_DEBUG_ACTIVES
  SEND_TRACE_TEXT(DBG1, m_ConfigModule.ts8_nomModule, " : ON");
#endif
  digitalWrite(m_ConfigModule.u8_numPort, u8_EtatOFF);
  u8_etatPort = u8_EtatON;
  m_TimerModule.Stop();
  m_TimerBlink.Start();
  setNouvelEtat(e_etatModule_t::MODULE_ON);
}

void Domodule_Pilotage_Voyant::moduleOFF(void)
{
#ifdef TRACE_DEBUG_ACTIVES
  SEND_TRACE_TEXT(DBG1, m_ConfigModule.ts8_nomModule, " : OFF");
#endif

  m_TimerModule.Stop();
  m_TimerBlink.Stop();

  digitalWrite(m_ConfigModule.u8_numPort, u8_EtatOFF);
  setNouvelEtat(e_etatModule_t::MODULE_OFF);
}

void Domodule_Pilotage_Voyant::moduleDRING(uint32_t i_u32_argument)
{
  digitalWrite(m_ConfigModule.u8_numPort, u8_EtatOFF);

  m_TimerModule.Init(Domodule_Actionneur::triggerOFF, i_u32_argument * 1000, One_Shot_Timer, this);
  m_TimerModule.Start();

  m_TimerBlink.Start();
  u8_etatPort = u8_EtatON;

  setNouvelEtat(e_etatModule_t::MODULE_DRING);
}

void Domodule_Pilotage_Voyant::setConfigModule(
    const Class_ConfigModuleVoyant &i_m_configModuleVoyant)
{
  m_TimerBlink.Init(Domodule_Pilotage_Voyant::triggerSuperTimer,
      i_m_configModuleVoyant.Mini_Periode, i_m_configModuleVoyant.Nbre_Mini_Periodes,
      i_m_configModuleVoyant.Periode, true, this, NULL);

  Domodule::setConfigModule(i_m_configModuleVoyant);
}

void Domodule_Pilotage_Voyant::triggerSuperTimer(uint32_t valeur, void *Arg)
{
  (void) valeur;
  Domodule_Pilotage_Voyant *module = (Domodule_Pilotage_Voyant*) Arg;

  module->BlinkPort();
}

void Domodule_Pilotage_Voyant::BlinkPort()
{
  digitalWrite(m_ConfigModule.u8_numPort, u8_etatPort);
  u8_etatPort ^= 1;
}
