/*
 * domodule_capteur.h
 *
 *  Created on: 2 avr. 2022
 *      Author: julien
 */

#pragma once

#include "domodule.h"

//! Transitions possible à surveiller sur le port
enum class e_declencherEtat_t
{
  declencherTransitionEtatHaut, declencherEtatHaut, declencherTransitionEtatBas, declencherEtatBas, declecherTransitionHautBas,
};

//! Classe de configuration de la surveillance du port
class Class_ConfigSurveillance
{
public:
  //! Constructeur de la classe
  //! @param i_e_declencherEtat Transition à rechercher
  //! @param i_u32_periodePollingMs Période en ms de polling de la surveillance
  //! @param i_e_etatModuleATransmettre Etat de module à transmettre lors de la détection (filtrage de bruit)
  //! @param i_u8_nbrePeriodeFiltrage Période nécessaire pour valider la détection
  Class_ConfigSurveillance(e_declencherEtat_t i_e_declencherEtat = e_declencherEtat_t::declencherTransitionEtatBas,
      uint32_t i_u32_periodePollingMs = 100,
      e_etatModule_t i_e_etatModuleATransmettre = e_etatModule_t::MODULE_OFF,
      uint8_t i_u8_nbrePeriodeFiltrage = 0) :
      u32_periodePollingMs(i_u32_periodePollingMs), e_etatModuleATransmettre(i_e_etatModuleATransmettre), u8_nbrePeriodeFiltrage(
          i_u8_nbrePeriodeFiltrage), e_declencherEtat(i_e_declencherEtat)
  {
  }

  //! Periode de polling de surveillance en ms
  uint32_t u32_periodePollingMs;
  //! Etat de module à transmettre lors de la détection
  e_etatModule_t e_etatModuleATransmettre;
  //! Nbre de période nécessaire pour valider la détection (filtrage de bruit)
  uint8_t u8_nbrePeriodeFiltrage;
  //! Transition d'état du port à rechercher
  e_declencherEtat_t e_declencherEtat;
};

//! Classe Domodule de type capteur
class Domodule_Capteur: public Domodule
{
public:
  //! Constructeur de la classe
  //! @param i_ps8_nomModule Nom du module
  //! @param i_u8_numPort Numéro du port à utiliser
  //! @param i_b_OutmodeInverse_InPullup Active ou non le pullup interne du port
  Domodule_Capteur(const char i_ps8_nomModule[24] = "", uint8_t i_u8_numPort = 0, bool i_b_OutmodeInverse_InPullup =
      false);

  //! Destructeur de la classe
  virtual ~Domodule_Capteur();

  //! Fonction pour décoder et exécuter si destinataire un ordre reçu
  //! @param message Message contenant l'ordre à décoder
  //! @param o_reponse String contenant la réponse à renvoyer
  virtual uint8_t decodeOrdre(const String &message, String &o_reponse) override;

  //! Active la configuration de la surveillance de port
  virtual void setConfigSurveillance(const Class_ConfigSurveillance &i_t_configSurveillance);

  //! Démarre la surveillance du port
  virtual void DemarrerSurveillance(void);
  //! Arrête la surviellance du port
  virtual void ArreterSurveillance(void);

  //! Retourne la valeur du port
  virtual uint16_t DonnerValeurCapteur(void) = 0;

  //! Démarre la surveillance du port
  virtual void moduleON(void) override;
  //! Arrête la surviellance du port
  virtual void moduleOFF(void) override;
  //! Equivalent à moduleON dans cette classe
  virtual void moduleDRING(uint32_t i_u32_argument) override;

protected:
  //! Active la configuration du port
  //! @param i_u8_numPort numéro du port à surveiller
  //! @param i_b_OutmodeInverse_InPullup Activer ou non le pullup du port
  virtual void setConfigPort(uint8_t i_u8_numPort, bool i_b_OutmodeInverse_InPullup = false) override;

  //! Mettre à jour l'était du module
  //! @param i_e_nouvelEtat Nouvel état à assigner au module
  virtual void setNouvelEtat(e_etatModule_t i_e_nouvelEtat) override;

  //! Méthode statique appeler par le timer de surveillance
  //! @param valeur non utilisée
  //! @param Arg Pointeur vers l'instance de la classe à surveiller
  static void fonctionSurveillance(uint32_t valeur, void *Arg);

  //! Méthode de surveillance du port
  virtual void surveillanceNiveau(void) = 0;

  //! Délencher les actions associé à une transition du port
  virtual void declencherTrigActions(void) override;

  //! Periode de surveillance du port
  uint32_t u32_periodePollingMs = 100;
  //! Etat interne du module
  e_etatModule_t e_etatModuleATransmettre = e_etatModule_t::MODULE_ON;
  //! Type de transition à rechercher
  e_declencherEtat_t e_declencherEtat = e_declencherEtat_t::declencherTransitionEtatBas;
  //! Nombre de période pour valider une transition
  uint8_t u8_nbrePeriodeFiltrage = 0;

  //! Dernière valeur mesurée du port
  uint16_t u16_valeurPort;
  //! Nombre de période restante pour valider la transition
  uint8_t u8_periodeFiltrageRestante = 0;
};
