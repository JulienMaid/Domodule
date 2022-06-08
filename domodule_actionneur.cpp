/*
 * domoduleactionneur.cpp
 *
 *  Created on: 2 avr. 2022
 *      Author: julien
 */

#include "domodule_actionneur.h"

#include <Arduino.h>

#ifdef TRACE_DEBUG_ACTIVES
#include "trace_debug.h"
#endif

Domodule_Actionneur::Domodule_Actionneur(const char i_ps8_nomModule[24], uint8_t i_u8_numPort,
    bool i_b_OutmodeInverse_InPullup) :
    Domodule(i_ps8_nomModule, i_u8_numPort, i_b_OutmodeInverse_InPullup), u8_EtatON(HIGH), u8_EtatOFF(LOW)
{
}

Domodule_Actionneur::~Domodule_Actionneur()
{
  m_TimerModule.Delete();
}

void Domodule_Actionneur::setConfigPort(uint8_t i_u8_numPort, bool i_b_OutmodeInverse_InPullup)
{
  if (i_b_OutmodeInverse_InPullup == true)
  {
    u8_EtatON = LOW;
    u8_EtatOFF = HIGH;
  }
  else
  {
    u8_EtatON = HIGH;
    u8_EtatOFF = LOW;
  }

  pinMode(i_u8_numPort, GPIO_OUTPUT);
  digitalWrite(i_u8_numPort, u8_EtatOFF);
}


void Domodule_Actionneur::moduleON(void)
{
#ifdef TRACE_DEBUG_ACTIVES
  Send_Trace_Text(DBG1, m_ConfigModule.ts8_nomModule, " : ON");
#endif

  m_TimerModule.Stop();
  digitalWrite(m_ConfigModule.u8_numPort, u8_EtatON);
  setNouvelEtat(e_etatModule_t::MODULE_ON);
}

void Domodule_Actionneur::moduleOFF(void)
{
#ifdef TRACE_DEBUG_ACTIVES
  Send_Trace_Text(DBG1, m_ConfigModule.ts8_nomModule, " : OFF");
#endif

  m_TimerModule.Stop();
  digitalWrite(m_ConfigModule.u8_numPort, u8_EtatOFF);
  setNouvelEtat(e_etatModule_t::MODULE_OFF);
}

void Domodule_Actionneur::moduleDRING(uint32_t i_u32_argument)
{
  m_TimerModule.Init(Domodule_Actionneur::triggerOFF, i_u32_argument * 1000, One_Shot_Timer, this);
  digitalWrite(m_ConfigModule.u8_numPort, u8_EtatON);

  m_TimerModule.Start();
  setNouvelEtat(e_etatModule_t::MODULE_DRING);
}

void Domodule_Actionneur::setNouvelEtat(e_etatModule_t i_e_nouvelEtat)
{
  if (i_e_nouvelEtat != m_ConfigModule.e_etatModule)
  {
    m_ConfigModule.e_etatModule = i_e_nouvelEtat;

    declencherTrigActions();
  }
}



void Domodule_Actionneur::triggerOFF(uint32_t valeur, void *Arg)
{
  (void) valeur;


  Domodule_Actionneur *DomoduleInterne = (Domodule_Actionneur*) Arg;

#ifdef TRACE_DEBUG_ACTIVES
  Send_Trace_Text(DBG3, DomoduleInterne->m_ConfigModule.ts8_nomModule, " - triggerOFF");
#endif

  DomoduleInterne->moduleOFF();
}

void Domodule_Actionneur::triggerON(uint32_t valeur, void *Arg)
{
  (void) valeur;

  Domodule_Actionneur *DomoduleInterne = (Domodule_Actionneur*) Arg;

#ifdef TRACE_DEBUG_ACTIVES
  Send_Trace_Text(DBG3, DomoduleInterne->m_ConfigModule.ts8_nomModule, " - triggerON");
#endif

  DomoduleInterne->moduleON();
}

uint8_t Domodule_Actionneur::decodeOrdre(const String &i_message, String &o_reponse)
{
  uint32_t u32_valeur = 1;
  String ordre("            ");

  if (Domodule::predecodeOrdre(i_message, ordre, u32_valeur) != 0)
  {
    return 1;
  }

  if (ordre.compareTo("ON") == 0)
  {
#ifdef TRACE_DEBUG_ACTIVES
    Send_Trace_Text(DBG1, m_ConfigModule.ts8_nomModule, " : Ordre ON");
#endif
    moduleON();
  }
  else if (ordre.compareTo("OFF") == 0)
  {
#ifdef TRACE_DEBUG_ACTIVES
    Send_Trace_Text(DBG1, m_ConfigModule.ts8_nomModule, " : Ordre OFF");
#endif
    moduleOFF();
  }
  else if (ordre.compareTo("DRING") == 0)
  {
#ifdef TRACE_DEBUG_ACTIVES
    Send_Trace_Text(DBG1, m_ConfigModule.ts8_nomModule, " : Ordre DRING");
    Send_Trace_Num(DBG1, "Valeur DRING", u32_valeur);
#endif
    moduleDRING(u32_valeur);
  }
  else if (ordre.compareTo("?") == 0)
  {
    o_reponse = m_ConfigModule.ts8_nomModule;

    o_reponse += " - ";

    switch (m_ConfigModule.e_etatModule)
    {
    case e_etatModule_t::MODULE_OFF:
      o_reponse += "OFF";
      break;
    case e_etatModule_t::MODULE_ON:
      o_reponse += "ON";
      break;
    case e_etatModule_t::MODULE_DRING:
      o_reponse += "DRING";
      o_reponse += " ";
      o_reponse += (uint32_t) (m_TimerModule.Time_Remaining() / 1000);
      break;
    default:
      return 1;
    }
  }
  else
  {
    return 1;
  }
  return 0;
}

