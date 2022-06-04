/*
 * domodule.h
 *
 *  Created on: 31 mars 2022
 *      Author: julien
 */

#ifndef DOMODULE_H_
#define DOMODULE_H_

#define TRACE_DEBUG_ACTIVES

#include <stdint.h>
#include "timer_sw.h"
#include <Arduino.h>

enum class e_etatModule_t
{
  MODULE_OFF = 0, MODULE_ON, MODULE_DRING
};

enum class e_typeModule_t
{
  Module_Actionneur, Module_Capteur_Digital_Polling, Module_Capteur_Digital_IRQ, Module_Capteur_Analogique
};

enum class e_typeActionTriggered_t : uint8_t
{
  Action_Identique, Action_Inverse, Action_Specifique,
};

typedef enum
{
  GPIO_INPUT = INPUT, GPIO_OUTPUT = OUTPUT, GPIO_INPUT_PULLUP = INPUT_PULLUP,

} e_configPort_t;



class s_configPort_t
{
public:
  uint8_t u8_numPort;
  e_configPort_t e_configPort;
  uint8_t u8_valeurDefaut;
};

class Class_ConfigModule
{
public:
  Class_ConfigModule(const char i_ps8_nomModule[24] = "", uint8_t i_u8_numPort = 0, bool i_b_OutmodeInverse_InPullup =
      false) :
      u8_numPort(i_u8_numPort), b_OutmodeInverse_InPullup(i_b_OutmodeInverse_InPullup), e_etatModule(
          e_etatModule_t::MODULE_OFF)
  {
    if (i_ps8_nomModule != nullptr)
    {
      uint8_t i;
      for (i = 0; i < 24; i++)
      {
        ts8_nomModule[i] = i_ps8_nomModule[i];
      }
    }
  }

  char ts8_nomModule[24];
  uint8_t u8_numPort;
  bool b_OutmodeInverse_InPullup;
  e_etatModule_t e_etatModule;

};

class Class_ActionTriggered_t
{
public:
  e_typeActionTriggered_t e_typeActionTriggered;
  e_etatModule_t e_etatModule;
  uint32_t u32_valeurDring;
};

typedef void (*pf_trigAction_t)(e_etatModule_t, uint32_t, void*);

class Class_ActionADeclencher
{
public:
  Class_ActionADeclencher(void) :
      pf_trigAction(NULL), Arg_trigAction(NULL)
  {
  }

  pf_trigAction_t pf_trigAction;
  void *Arg_trigAction;
};



class Domodule {
public:
  Domodule(const char i_ps8_nomModule[24] = "", uint8_t i_u8_numPort = 0, bool i_b_OutmodeInverse_InPullup = false);
  virtual ~Domodule();

  uint8_t setPfTrigAction(pf_trigAction_t pf_trigAction,
      void *Arg, uint8_t i_u8_index = 0);

  virtual void setConfigModule(const Class_ConfigModule &i_m_configModule);

  char* getModuleName(void);
  String getEtatModule(void);


  void setActionTriggered(e_typeActionTriggered_t i_e_typeAction =
      e_typeActionTriggered_t::Action_Identique,
      e_etatModule_t i_e_etatModule =
      e_etatModule_t::MODULE_OFF,
      uint32_t i_u32_dureeDring = 0);
  

  virtual uint8_t decodeOrdre(const String &i_message, String &o_reponse) = 0;

  virtual void moduleON(void) = 0;
  virtual void moduleOFF(void) = 0;
  virtual void moduleDRING(uint32_t i_u32_argument) = 0;

  static void trigAction(e_etatModule_t i_e_etatModule, uint32_t i_u32_valeur, void *Arg);


protected:
  virtual void setConfigPort(uint8_t i_u8_numPort, bool i_b_OutmodeInverse_InPullup) = 0;

  uint8_t predecodeOrdre(const String &i_message, String &o_ordre, uint32_t &o_u32_valeur);

  virtual void setNouvelEtat(e_etatModule_t i_e_nouvelEtat) = 0;

  TimerEvent_t m_TimerModule;

  Class_ConfigModule m_ConfigModule;
  Class_ActionTriggered_t m_ActionTriggered;

  virtual void declencherTrigActions(void);

  static const uint8_t u8_tailleActionADeclencher = 10;
  Class_ActionADeclencher ActionADeclencher[u8_tailleActionADeclencher];
};

#endif /* DOMODULE_H_ */
