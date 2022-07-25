/*
 * domodule_capteur.cpp
 *
 *  Created on: 2 avr. 2022
 *      Author: julien
 */

#include "domodule_capteur.h"

#ifdef TRACE_DEBUG_ACTIVES
#include "trace_debug.h"
#endif

Domodule_Capteur::Domodule_Capteur(const char i_ps8_nomModule[24], uint8_t i_u8_numPort,
    bool i_b_OutmodeInverse_InPullup) :
    Domodule(i_ps8_nomModule, i_u8_numPort, i_b_OutmodeInverse_InPullup)
{
}


Domodule_Capteur::~Domodule_Capteur()
{
  m_TimerModule.Delete();
}

void Domodule_Capteur::setConfigPort(uint8_t i_u8_numPort, bool i_b_OutmodeInverse_InPullup)
{
  if (i_b_OutmodeInverse_InPullup == false)
  {
    pinMode(i_u8_numPort, GPIO_INPUT);
  }
  else
  {
    pinMode(i_u8_numPort, GPIO_INPUT_PULLUP);
  }
}

void Domodule_Capteur::setConfigSurveillance(const Class_ConfigSurveillance &i_t_ConfigSurveillance)
{
  u32_periodePollingMs = i_t_ConfigSurveillance.u32_periodePollingMs;
  e_etatModuleATransmettre = i_t_ConfigSurveillance.e_etatModuleATransmettre;
  e_declencherEtat = i_t_ConfigSurveillance.e_declencherEtat;
  u8_nbrePeriodeFiltrage = i_t_ConfigSurveillance.u8_nbrePeriodeFiltrage;
}

void Domodule_Capteur::DemarrerSurveillance(void)
{
  u8_periodeFiltrageRestante = u8_nbrePeriodeFiltrage;
  m_TimerModule.Init(fonctionSurveillance, u32_periodePollingMs, Periodic_Timer,
      this);
  m_TimerModule.Start();

  setNouvelEtat(e_etatModule_t::MODULE_ON);
}

void Domodule_Capteur::ArreterSurveillance(void)
{
  m_TimerModule.Stop();

  setNouvelEtat(e_etatModule_t::MODULE_OFF);
}

void Domodule_Capteur::fonctionSurveillance(uint32_t valeur, void *Arg)
{
  Domodule_Capteur *Capteur = (Domodule_Capteur*) Arg;

  Capteur->surveillanceNiveau();
}

void Domodule_Capteur::moduleON(void)
{
  DemarrerSurveillance();
}

void Domodule_Capteur::moduleOFF(void)
{
  ArreterSurveillance();
}

void Domodule_Capteur::moduleDRING(uint32_t i_u32_argument)
{
  (void) i_u32_argument;
  DemarrerSurveillance();
}

uint8_t Domodule_Capteur::decodeOrdre(const String &i_message, String &o_reponse)
{
  uint32_t u32_valeur = 0;
  String ordre("            ");

  if (predecodeOrdre(i_message, ordre, u32_valeur) != 0)
  {
    return 1;
  }

  if (ordre.compareTo("?") == 0)
  {
    o_reponse = m_ConfigModule.ts8_nomModule;

    o_reponse += " - ";

    o_reponse += getEtatModuleString();

    o_reponse += " ";

    o_reponse += DonnerValeurCapteur();
  }
  else if (ordre.compareTo("ON") == 0)
  {
    moduleON();
  }
  else if (ordre.compareTo("OFF") == 0)
  {
    moduleOFF();
  }
  else
  {
    return 1;
  }

  return 0;
}

void Domodule_Capteur::setNouvelEtat(e_etatModule_t i_e_nouvelEtat)
{
  if (i_e_nouvelEtat != m_ConfigModule.e_etatModule)
  {
    m_ConfigModule.e_etatModule = i_e_nouvelEtat;
  }
}

void Domodule_Capteur::declencherTrigActions(void)
{
  uint8_t u8_index = 0;

#ifdef TRACE_DEBUG_ACTIVES
  Send_Trace_Text(INFO, this->getModuleName(), ": Déclenchement Trig Actions");
#endif

  for (u8_index = 0; u8_index < u8_tailleActionADeclencher; u8_index++)
  {
    if (ActionADeclencher[u8_index].pf_trigAction != NULL)
    {
      ActionADeclencher[u8_index].pf_trigAction(e_etatModuleATransmettre, u16_valeurPort,
          ActionADeclencher[u8_index].Arg_trigAction);
    }
  }
}
