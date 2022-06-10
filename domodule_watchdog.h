/*
 * domodule_watchdog.h
 *
 *  Created on: 3 juin 2022
 *      Author: julien
 */

#ifndef DOMODULE_WATCHDOG_H_
#define DOMODULE_WATCHDOG_H_

#include "domodule_capteur.h"

//! Classe de configuration du module watchdog
class Class_ConfigModuleWatchdog: public Class_ConfigModule
{
public:
  //! Constructeur de la classe de configuration
  //! @param i_ps8_nomModule Nom du module
  //! @param i_u64_periode Période du watchdog en secodnes
  Class_ConfigModuleWatchdog(const char i_ps8_nomModule[24] = "", TimerTime_t i_u64_periode = 0) :
      Class_ConfigModule(i_ps8_nomModule), Periode(i_u64_periode)
  {
  }

  //! Période du watchdog
  TimerTime_t Periode;
};

//! Classe de Domodule Watchdog
class Domodule_Watchdog: public Domodule_Capteur
{
public:
  //! Constructeur de la classe
  //! @param i_ps8_nomModule Nom du module
  //! @param i_u64_periode Période du watchdog en secodnes
  Domodule_Watchdog(const char i_ps8_nomModule[24] = "", TimerTime_t i_u64_tempsWatchdog = 0);
  //! Destructeur de la classe
  virtual ~Domodule_Watchdog();
  Domodule_Watchdog(const Domodule_Watchdog &other) = delete;
  Domodule_Watchdog(Domodule_Watchdog &&other) = delete;
  Domodule_Watchdog& operator=(Domodule_Watchdog &&other) = delete;
  Domodule_Watchdog& operator=(const Domodule_Watchdog &other) = delete;

  virtual void setConfigModule(const Class_ConfigModuleWatchdog &i_m_configModuleWatchdog);

  //! Activation du module (démarrage du watchdog)
  virtual void moduleON(void) override;
  //! Arrêt du module (arrêt du watchdog)
  virtual void moduleOFF(void) override;

  //! Reset la compte à rebour du watchdog
  virtual void resetWatchdog(void);

protected:
  //! Methode statique appelé par le timer logiciel en fin de compte à rebours
  static void trigWatchdogStatic(uint32_t i_u32_valeur, void *Arg);
};

#endif /* DOMODULE_WATCHDOG_H_ */
