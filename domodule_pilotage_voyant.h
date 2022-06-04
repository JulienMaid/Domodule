/*
 * domodule_pilotage_voyant.h
 *
 *  Created on: 30 avr. 2022
 *      Author: julien
 */

#ifndef DOMODULE_PILOTAGE_VOYANT_H_
#define DOMODULE_PILOTAGE_VOYANT_H_

#include "domodule_actionneur.h"
#include "super_timer_sw.h"

class Class_ConfigModuleVoyant: public Class_ConfigModule
{
public:
  Class_ConfigModuleVoyant(const char i_ps8_nomModule[24] = "", uint8_t i_u8_numPort = 0,
      bool i_b_OutmodeInverse_InPullup = false, TimerTime_t i_u64_mini_periode = 0, uint8_t i_u8_nbre_mini_periodes = 0,
      TimerTime_t i_u64_periode = 0) :
      Class_ConfigModule(i_ps8_nomModule, i_u8_numPort, i_b_OutmodeInverse_InPullup), Mini_Periode(
          i_u64_mini_periode), Nbre_Mini_Periodes(
          i_u8_nbre_mini_periodes), Periode(i_u64_periode)
  {
  }

  TimerTime_t Mini_Periode;
  uint8_t Nbre_Mini_Periodes;
  TimerTime_t Periode;
};




class Domodule_Pilotage_Voyant: public Domodule_Actionneur
{
public:
  Domodule_Pilotage_Voyant();
  virtual ~Domodule_Pilotage_Voyant();

  virtual void setConfigModule(const Class_ConfigModuleVoyant &i_m_configModuleVoyant);

  virtual void moduleON(void) override;
  virtual void moduleOFF(void) override;
  virtual void moduleDRING(uint32_t i_u32_argument) override;

protected:
  Super_TimerEvent_t m_TimerBlink;

  uint8_t u8_etatPort;

  void BlinkPort(void);

  static void triggerSuperTimer(uint32_t valeur, void *Arg);

};

#endif /* DOMODULE_PILOTAGE_VOYANT_H_ */
