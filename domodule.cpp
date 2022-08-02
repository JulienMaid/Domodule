/*
 * domodule.cpp
 *
 *  Created on: 31 mars 2022
 *      Author: julien
 */

#include "domodule.h"
#include <Arduino.h>
#ifdef TRACE_DEBUG_ACTIVES
#include "trace_debug.h"
#endif

Domodule::Domodule(const char i_ps8_nomModule[24], uint8_t i_u8_numPort, bool i_b_OutmodeInverse_InPullup)
{
  Class_ConfigModule *temp_configModule = new Class_ConfigModule(i_ps8_nomModule, i_u8_numPort,
      i_b_OutmodeInverse_InPullup);
  setConfigModule(*temp_configModule);
  delete temp_configModule;
}

Domodule::~Domodule()
{
  m_TimerModule.Delete();
}

char* Domodule::getModuleName(void)
{
  return m_ConfigModule.ts8_nomModule;
}

String Domodule::getEtatModuleString(void)
{
  String l_t_EtatModule = "";

  if (m_ConfigModule.e_etatModule == e_etatModule_t::MODULE_ON)
  {
    l_t_EtatModule = "ON";
  }
  else if (m_ConfigModule.e_etatModule == e_etatModule_t::MODULE_OFF)
  {
    l_t_EtatModule = "OFF";
  }
  else if (m_ConfigModule.e_etatModule == e_etatModule_t::MODULE_DRING)
  {
    l_t_EtatModule = "DRING";
  }
  else
  {
    // Laissé vide volontairement
#ifdef TRACE_DEBUG_ACTIVES
    Send_Trace(ERROR, "Etat module inconnu");
#endif
  }

  return l_t_EtatModule;
}

uint8_t Domodule::setPfTrigAction(pf_trigAction_t i_pf_trigAction, Domodule *Arg, uint8_t i_u8_index)
{
  if (i_u8_index < u8_tailleActionADeclencher)
  {
    ActionADeclencher[i_u8_index].pf_trigAction = i_pf_trigAction;
    ActionADeclencher[i_u8_index].Arg_trigAction = Arg;
  }
  else
  {
    return 1;
  }

  return 0;
}

void Domodule::setConfigModule(const Class_ConfigModule &i_m_configModule)
{
  m_ConfigModule = i_m_configModule;
  m_ConfigModule.e_etatModule = e_etatModule_t::MODULE_OFF;
  m_TimerModule.Stop();

  if (i_m_configModule.u8_numPort != 0)
  {
    setConfigPort(m_ConfigModule.u8_numPort, m_ConfigModule.b_OutmodeInverse_InPullup);
  }

  setActionTriggered(e_typeActionTriggered_t::Action_Identique);

#ifdef TRACE_DEBUG_ACTIVES
  SEND_TRACE_TEXT(DBG1, this->getModuleName(), ": Config OK");
#endif
}

uint8_t Domodule::predecodeOrdre(const String &i_message, String &o_ordre, uint32_t &o_u32_valeur)
{
  String module("                    ");
  uint8_t Nbre = 0;
  String nomModule;

  Nbre = sscanf((const char*) i_message.begin(), "%30s - %20s %d", module.begin(), o_ordre.begin(), &o_u32_valeur);

  if (Nbre < 2)
  {
#ifdef TRACE_DEBUG_ACTIVES
    Send_Trace(DBG1, "Taille Trame invalide");
#endif
    return 1;
  }

  nomModule = m_ConfigModule.ts8_nomModule;

  if (nomModule.compareTo(module) != 0)
  {
#ifdef TRACE_DEBUG_ACTIVES
    SEND_TRACE_TEXT(DBG1, m_ConfigModule.ts8_nomModule, ": Nom Module KO");
#endif
    return 1;
  }

#ifdef TRACE_DEBUG_ACTIVES
  SEND_TRACE_TEXT(DBG1, m_ConfigModule.ts8_nomModule, ": Nom Module OK");
#endif
  return 0;
}

void Domodule::setActionTriggered(e_typeActionTriggered_t i_e_typeAction, e_etatModule_t i_e_etatModule,
    uint32_t i_u32_dureeDring)
{
  m_ActionTriggered.e_typeActionTriggered = i_e_typeAction;

  if ((i_e_typeAction != e_typeActionTriggered_t::Action_Identique)
      && (i_e_typeAction != e_typeActionTriggered_t::Action_Inverse))
  {
    m_ActionTriggered.e_etatModule = i_e_etatModule;
    m_ActionTriggered.u32_valeurDring = i_u32_dureeDring;
  }
  else
  {
    m_ActionTriggered.e_etatModule = e_etatModule_t::MODULE_OFF;
    m_ActionTriggered.u32_valeurDring = 0;
  }
}

void Domodule::trigAction(e_etatModule_t i_e_etatModule, uint32_t i_u32_valeur, Domodule *Arg)
{
  e_etatModule_t e_etatModule = i_e_etatModule;
  uint32_t u32_valeurDring = i_u32_valeur;

  Domodule *DomoduleInterne = (Domodule*) Arg;

#ifdef TRACE_DEBUG_ACTIVES
  SEND_TRACE_TEXT(DBG1, "trigAction => ", DomoduleInterne->m_ConfigModule.ts8_nomModule);
#endif

  if (DomoduleInterne->m_ActionTriggered.e_typeActionTriggered == e_typeActionTriggered_t::Action_Identique)
  {
    // rien à faire
  }
  else if (DomoduleInterne->m_ActionTriggered.e_typeActionTriggered == e_typeActionTriggered_t::Action_Inverse)
  {
    if (e_etatModule == e_etatModule_t::MODULE_OFF)
    {
      e_etatModule = e_etatModule_t::MODULE_ON;
    }
    else if (e_etatModule == e_etatModule_t::MODULE_ON)
    {
      e_etatModule = e_etatModule_t::MODULE_OFF;
    }
    else
    {
#ifdef TRACE_DEBUG_ACTIVES
      SEND_TRACE_TEXT(DBG1, DomoduleInterne->m_ConfigModule.ts8_nomModule,
          " : Action Triggered DRING interdite en module Inversé");
#endif
      return;
    }
  }
  else if (DomoduleInterne->m_ActionTriggered.e_typeActionTriggered == e_typeActionTriggered_t::Action_Specifique)
  {
    if (e_etatModule != e_etatModule_t::MODULE_OFF)
    {
      e_etatModule = DomoduleInterne->m_ActionTriggered.e_etatModule;
      u32_valeurDring = DomoduleInterne->m_ActionTriggered.u32_valeurDring;
    }
  }
  else
  {
#ifdef TRACE_DEBUG_ACTIVES
    SEND_TRACE_TEXT(DBG1, DomoduleInterne->m_ConfigModule.ts8_nomModule, " : Action Triggered inconnue");
#endif
    return;
  }

  switch (e_etatModule)
  {
  case e_etatModule_t::MODULE_OFF:
    DomoduleInterne->moduleOFF();
    break;
  case e_etatModule_t::MODULE_ON:
    DomoduleInterne->moduleON();
    break;
  case e_etatModule_t::MODULE_DRING:
    DomoduleInterne->moduleDRING(u32_valeurDring);
    break;
  default:
    break;
  }
}

void Domodule::declencherTrigActions(void)
{
  uint8_t u8_index = 0;

  for (u8_index = 0; u8_index < u8_tailleActionADeclencher; u8_index++)
  {
    if (ActionADeclencher[u8_index].pf_trigAction != NULL)
    {
      ActionADeclencher[u8_index].pf_trigAction(m_ConfigModule.e_etatModule, 0,
          ActionADeclencher[u8_index].Arg_trigAction);
    }
  }
}

e_etatModule_t Domodule::getEtatModule(void)
{
  return m_ConfigModule.e_etatModule;
}
