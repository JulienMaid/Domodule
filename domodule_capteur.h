/*
 * domodule_capteur.h
 *
 *  Created on: 2 avr. 2022
 *      Author: julien
 */

#ifndef DOMODULE_CAPTEUR_H_
#define DOMODULE_CAPTEUR_H_

#include "domodule.h"

enum class e_declencherEtat_t
{
  declencherTransitionEtatHaut, declencherEtatHaut, declencherTransitionEtatBas, declencherEtatBas, declecherTransitionHautBas,
};

class Class_ConfigSurveillance
{
public:
  Class_ConfigSurveillance(e_declencherEtat_t i_e_declencherEtat = e_declencherEtat_t::declencherTransitionEtatBas,
      uint32_t i_u32_periodePollingMs = 100,
      e_etatModule_t i_e_etatModuleATransmettre =
      e_etatModule_t::MODULE_OFF, uint8_t i_u8_nbrePeriodeFiltrage = 0) :
      u32_periodePollingMs(i_u32_periodePollingMs), e_etatModuleATransmettre(i_e_etatModuleATransmettre), u8_nbrePeriodeFiltrage(
          i_u8_nbrePeriodeFiltrage), e_declencherEtat(i_e_declencherEtat)
  {
  }

  uint32_t u32_periodePollingMs;
  e_etatModule_t e_etatModuleATransmettre;
  uint8_t u8_nbrePeriodeFiltrage;
  e_declencherEtat_t e_declencherEtat;
};

class Domodule_Capteur: public Domodule
{
public:
  Domodule_Capteur(const char i_ps8_nomModule[24] = "", uint8_t i_u8_numPort = 0, bool i_b_OutmodeInverse_InPullup =
      false);

  virtual ~Domodule_Capteur();

  virtual uint8_t decodeOrdre(const String &message, String &o_reponse) override;

  virtual void setConfigSurveillance(const Class_ConfigSurveillance &i_t_configSurveillance);

  virtual void DemarrerSurveillance(void);
  virtual void ArreterSurveillance(void);

  virtual uint16_t DonnerValeurCapteur(void) = 0;

  virtual void moduleON(void) override;
  virtual void moduleOFF(void) override;
  virtual void moduleDRING(uint32_t i_u32_argument) override;

protected:
  virtual void setConfigPort(uint8_t i_u8_numPort, bool i_b_OutmodeInverse_InPullup = false) override;

  virtual void setNouvelEtat(e_etatModule_t i_e_nouvelEtat) override;

  static void fonctionSurveillance(uint32_t valeur, void *Arg);

  virtual void surveillanceNiveau(void) = 0;

  virtual void declencherTrigActions(void) override;

  uint32_t u32_periodePollingMs = 100;
  e_etatModule_t e_etatModuleATransmettre = e_etatModule_t::MODULE_ON;
  e_declencherEtat_t e_declencherEtat = e_declencherEtat_t::declencherTransitionEtatBas;
  uint8_t u8_nbrePeriodeFiltrage = 0;

  uint16_t u16_valeurPort;
  uint8_t u8_periodeFiltrageRestante;

};

#endif /* DOMODULE_CAPTEUR_H_ */
