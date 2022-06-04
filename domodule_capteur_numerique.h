/*
 * domodule_capteur_numerique.h
 *
 *  Created on: 2 avr. 2022
 *      Author: julien
 */

#ifndef DOMODULE_CAPTEUR_NUMERIQUE_H_
#define DOMODULE_CAPTEUR_NUMERIQUE_H_

#include "domodule_capteur.h"

/**
 @startuml
 skinparam shadowing false

 title Class Diagram

 skinparam class {
 BackgroundColor #94de5e
 ArrowColor #darkblue
 BorderColor black
 }

 class Domodule {
 setPfTrigAction()
 setConfigModule()
 setConfigPort()
 getModuleName()
 setActionTriggered()
 decodeOrdre()
 }

 class Domodule_Capteur {
 setConfigModule()
 setConfigPort()
 setConfigSurveillance()
 demarreSurveillance()
 arreteSurveillance()
 }

 class Domodule_Capteur_Numerique {
 demarreSurveillance()
 decodeOrdre()
 }

 class Domodule_Actionneur {
 setConfigModule()
 setConfigPort()
 moduleON()
 moduleOFF()
 moduleDRING()
 decodeOrdre()


 }

 Domodule <|-- Domodule_Capteur
 Domodule_Capteur <|-- Domodule_Capteur_Numerique
 Domodule <|-- Domodule_Actionneur

 legend
 <size:18>Key</size>
 |<#94de5e> Class |
 endlegend
 @enduml
 */

class Class_ConfigSurveillanceNumerique: public Class_ConfigSurveillance
{
public:
  Class_ConfigSurveillanceNumerique(e_declencherEtat_t i_e_declencherEtat = e_declencherEtat_t::declencherTransitionEtatBas,
      uint32_t i_u32_periodePollingMs = 1000,
      e_etatModule_t i_e_etatModuleATransmettre = e_etatModule_t::MODULE_OFF,
      uint8_t i_u8_nbrePeriodeFiltrage = 0) :
      Class_ConfigSurveillance(i_e_declencherEtat, i_u32_periodePollingMs, i_e_etatModuleATransmettre,
          i_u8_nbrePeriodeFiltrage)
  {
  }
};

class Domodule_Capteur_Numerique: public Domodule_Capteur
{
public:
  Domodule_Capteur_Numerique(const char i_ps8_nomModule[24] = "", uint8_t i_u8_numPort = 0,
      bool i_b_OutmodeInverse_InPullup = false);
  virtual ~Domodule_Capteur_Numerique();

  virtual void setConfigSurveillance(const Class_ConfigSurveillanceNumerique &i_t_ConfigSurveillance);

  virtual void DemarrerSurveillance(void) override;

  virtual uint16_t DonnerValeurCapteur(void) override;


protected:
  virtual void surveillanceNiveau(void) override;

};

#endif /* DOMODULE_CAPTEUR_NUMERIQUE_H_ */