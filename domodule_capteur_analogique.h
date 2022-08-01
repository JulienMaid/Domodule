/*
 * domodule_capteur_analogique.h
 *
 *  Created on: 6 avr. 2022
 *      Author: julien
 */

#pragma once

#include "domodule_capteur.h"

//! Configuration de la surveillance d'un port analogique
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

  //! Seuil de basculement analogique
  uint16_t u16_seuilDeclenchement;
  //! Hystérésis (non implémenté à ce jour)
  uint16_t u16_hystereris;
};

//! Classe de gestion de capteur analogique
class Domodule_Capteur_Analogique: public Domodule_Capteur
{
public:
  //! Constructeur de la classe
  //! @param i_ps8_nomModule Nom du module
  //! @param i_u8_numPort Numéro du port à utiliser
  //! @param i_b_OutmodeInverse_InPullup Active ou non le pullup interne du port
  Domodule_Capteur_Analogique(const char i_ps8_nomModule[24] = "", uint8_t i_u8_numPort = 0,
      bool i_b_OutmodeInverse_InPullup = false);
  //! Destructeur de la classe
  virtual ~Domodule_Capteur_Analogique();

  //! Configure la surveillance du port analogique
  virtual void setConfigSurveillance(const Class_ConfigSurveillanceAnalogique &i_t_ConfigSurveillance);

  //! Démarrer la surveillance par polling du port analogique
  virtual void DemarrerSurveillance(void) override;

  //! Retourne la valeur du port analogique
  virtual uint16_t DonnerValeurCapteur(void) override;

protected:
  //! Methode appellé chaque période (de polling) pour surveiller la valeur du port
  virtual void surveillanceNiveau(void);

  //! Seuil de déclenchement confiuré
  uint16_t u16_seuilDeclenchement;
  //! Hystérésis (non implémenté à ce jour)
  uint16_t u16_hystereris;

};

