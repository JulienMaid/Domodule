/*
 * domodule_pilotage_voyant.h
 *
 *  Created on: 30 avr. 2022
 *      Author: julien
 */

#pragma once

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
  //! Constructeur de la classe
  Domodule_Pilotage_Voyant();
  //! Destructeur de la classe
  virtual ~Domodule_Pilotage_Voyant();

  //! Active la configuration du module
  //! @param i_m_configModuleVoyant Classe contenant la configuration à appliquer
  virtual void setConfigModule(const Class_ConfigModuleVoyant &i_m_configModuleVoyant);

  //! Active le clignotement
  virtual void moduleON(void) override;
  //! Arrête le clignotement
  virtual void moduleOFF(void) override;
  //! Active le clignotement pendant i_u32_argument secondes
  //! @param i_u32_argument durée d'activation en secondes
  virtual void moduleDRING(uint32_t i_u32_argument) override;

protected:
  //! Timer logiciel utilisé pour gérer le clignotement du port
  Super_TimerEvent_t m_TimerBlink;

  //! Valeur de l'état courant du port
  uint8_t u8_etatPort;

  //! Méthode de gestion du clignotement (appelée par triggerSuperTimer)
  void BlinkPort(void);

  //! Méthode statique appelée par le Super_Timer
  //! @param valeur Non utilisé
  //! @param Arg Pointeur vers l'instance de la classe à appeler
  static void triggerSuperTimer(uint32_t valeur, void *Arg);

};
