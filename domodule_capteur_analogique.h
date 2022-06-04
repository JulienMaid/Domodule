/*
 * domodule_capteur_analogique.h
 *
 *  Created on: 6 avr. 2022
 *      Author: julien
 */

#ifndef DOMODULE_CAPTEUR_ANALOGIQUE_H_
#define DOMODULE_CAPTEUR_ANALOGIQUE_H_

#include "domodule_capteur.h"

class Class_ConfigSurveillanceAnalogique: public Class_ConfigSurveillance
{
public:
  Class_ConfigSurveillanceAnalogique(uint16_t i_u16_seuilDeclenchement = 512, e_declencherEtat_t i_e_declencherEtat =
      e_declencherEtat_t::declencherTransitionEtatBas,
      uint16_t i_u16_hystereris = 0, uint32_t i_u32_periodePollingMs =
      1000, e_etatModule_t i_e_etatModuleATransmettre = e_etatModule_t::MODULE_OFF,
      uint8_t i_u8_nbrePeriodeFiltrage = 0) :
      Class_ConfigSurveillance(
          i_e_declencherEtat, i_u32_periodePollingMs, i_e_etatModuleATransmettre,
          i_u8_nbrePeriodeFiltrage), u16_seuilDeclenchement(i_u16_seuilDeclenchement), u16_hystereris(i_u16_hystereris)
  {
  }

  uint16_t u16_seuilDeclenchement;
  uint16_t u16_hystereris;
};


class Domodule_Capteur_Analogique: public Domodule_Capteur
{
public:
  Domodule_Capteur_Analogique(const char i_ps8_nomModule[24] = "", uint8_t i_u8_numPort = 0,
      bool i_b_OutmodeInverse_InPullup = false);
  virtual ~Domodule_Capteur_Analogique();

  virtual void setConfigSurveillance(const Class_ConfigSurveillanceAnalogique &i_t_ConfigSurveillance);

  virtual void DemarrerSurveillance(void) override;

  virtual uint16_t DonnerValeurCapteur(void) override;

protected:
  virtual void surveillanceNiveau(void);

  uint16_t u16_seuilDeclenchement;
  uint16_t u16_hystereris;

};

#endif /* DOMODULE_CAPTEUR_ANALOGIQUE_H_ */
