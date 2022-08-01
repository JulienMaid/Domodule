/*
 * domoduleactionneur.h
 *
 *  Created on: 2 avr. 2022
 *      Author: julien
 */

#pragma once

#include "domodule.h"

class Domodule_Actionneur: public Domodule
{
public:
  //! Constructeur de la classe
  //! @param i_ps8_nomModule Nom du module
  //! @param i_u8_numPort Numéro du port à utiliser
  //! @param i_b_OutmodeInverse_InPullup utilisation du port en inversé si sortie
  Domodule_Actionneur(const char i_ps8_nomModule[24] = "", uint8_t i_u8_numPort = 0, bool i_b_OutmodeInverse_InPullup =
      false);
  //! Destructeur de la classe
  virtual ~Domodule_Actionneur();

  //! Passe le module en mode ON
  virtual void moduleON(void) override;
  //! Passe le module en mode OFF
  virtual void moduleOFF(void) override;
  //! Passe le module en mode DRING (mode monostable: mode ON pour une durée de "i_u32_argument" secondes)
  virtual void moduleDRING(uint32_t i_u32_argument) override;

  //! Décode et exécute l'ordre contenu dans i_message et si besoin place une réponse dans o_reponse
  virtual uint8_t decodeOrdre(const String &i_message, String &o_reponse);

protected:
  //! Configure le port utilisé par le module
  virtual void setConfigPort(uint8_t i_u8_numPort, bool i_b_OutmodeInverse_InPullup = false) override;

  //! Methode statique pour les déclenchement OFF externe de la classe
  static void triggerOFF(uint32_t valeur, void *Arg);
  //! Methode statique pour les déclenchement ON externe de la classe
  static void triggerON(uint32_t valeur, void *Arg);

  //! Met le module dans un nouvel etat
  virtual void setNouvelEtat(e_etatModule_t i_e_nouvelEtat) override;

  //! Valeur de l'état ON sur le port de sortie
  uint8_t u8_EtatON;
  //! Valeur de l'état OFF sur le port de sortie
  uint8_t u8_EtatOFF;
};
