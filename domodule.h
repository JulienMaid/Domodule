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

class Domodule;

//! Etat possible du Module
enum class e_etatModule_t
{
  MODULE_OFF = 0, MODULE_ON, MODULE_DRING
};

enum class e_typeModule_t
{
  Module_Actionneur, Module_Capteur_Digital_Polling, Module_Capteur_Digital_IRQ, Module_Capteur_Analogique
};

//! type d'action à accomplir quand le module est déclenché
enum class e_typeActionTriggered_t : uint8_t
{
  Action_Identique, Action_Inverse, Action_Specifique,
};

//! configuration possible du port utilisé
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

//! Configuration d'un Module
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

  //! Nom du module
  char ts8_nomModule[24];

  //! Numero du port à utiliser
  uint8_t u8_numPort;

  //! Utilisation du port (si O: false=normal ou true=inversée, si I: pullup=true, sans=false)
  bool b_OutmodeInverse_InPullup;

  //! Etat par défaut du module
  e_etatModule_t e_etatModule;

};

//! Classe pour déterminer le comportement du module quand il est "déclenché"
class Class_ActionTriggered_t
{
public:
  //! Type de comportement quand le module est déclenché
  e_typeActionTriggered_t e_typeActionTriggered;

  //! Si e_typeActionTriggered == Action_Specifique, le module prendra cet état
  e_etatModule_t e_etatModule;

  //! Si e_typeActionTriggered == Action_Specifique, le module utilisera cette valeur optionnelle
  uint32_t u32_valeurDring;
};

//! Définition du type de pointeur de fonction à utiliser pour les événements à déclencher
typedef void (*pf_trigAction_t)(e_etatModule_t, uint32_t, Domodule*);

//! Classe pour stocker un action à déclencher
class Class_ActionADeclencher
{
public:
  Class_ActionADeclencher(void) :
      pf_trigAction(NULL), Arg_trigAction(NULL)
  {
  }

  //! pointeur vers la fonction à exécuter
  pf_trigAction_t pf_trigAction;

  //! Argument à fournir la fonction pointée par pf_trigAction
  Domodule *Arg_trigAction;
};



class Domodule {
public:
  //! Constructeur de la classe
  //! @param i_ps8_nomModule Nom du module
  //! @param i_u8_numPort Numéro du port à utiliser
  //! @param i_b_OutmodeInverse_InPullup utilisation du port en inversé si sortie, Pullup activée si entrée
  Domodule(const char i_ps8_nomModule[24] = "", uint8_t i_u8_numPort = 0, bool i_b_OutmodeInverse_InPullup = false);
  //! Destructeur de la classe
  virtual ~Domodule();

  //! Definir les actions à executer lors d'un changement d'état, ou niveau atteint dans le cas d'un capteur, etc...
  //! @param pf_trigAction Pointeur vers la fonction à exécuter
  //! @param Arg Pointeur vers les argument passé à la fonction à exécuter
  //! @param i_u8_index Numéro d'index des actions (max 10)
  uint8_t setPfTrigAction(pf_trigAction_t pf_trigAction,
      Domodule *Arg, uint8_t i_u8_index = 0);

  //! Configure le module (numéro de port, etc...)
  //! @param i_m_configModule Configuration du module à appliquer
  virtual void setConfigModule(const Class_ConfigModule &i_m_configModule);

  //! Retourne une chaine de caractère contenant le nom du module
  char* getModuleName(void);

  //! Retourne une String avec l'état en cours (ON, OFF, etc...)
  String getEtatModuleString(void);

  e_etatModule_t getEtatModule(void);

  //! Définie l'action du module quand il est déclenché par un événement extérieur
  void setActionTriggered(e_typeActionTriggered_t i_e_typeAction =
      e_typeActionTriggered_t::Action_Identique,
      e_etatModule_t i_e_etatModule =
      e_etatModule_t::MODULE_OFF,
      uint32_t i_u32_dureeDring = 0);
  

  //! Décode et exécute l'ordre contenu dans i_message et si besoin place une réponse dans o_reponse
  virtual uint8_t decodeOrdre(const String &i_message, String &o_reponse) = 0;

  //! Passe le module en mode ON
  virtual void moduleON(void) = 0;
  //! Passe le module en mode OFF
  virtual void moduleOFF(void) = 0;
  //! Passe le module en mode DRING (mode monostable: mode ON pour une durée de "i_u32_argument" secondes)
  virtual void moduleDRING(uint32_t i_u32_argument) = 0;

  //! Fonction statique appelée pour déclencher les modules par des événement extérieurs (ou par d'autres modules)
  static void trigAction(e_etatModule_t i_e_etatModule, uint32_t i_u32_valeur, Domodule *Arg);

protected:
  //! Configure le port utilisé par le module
  virtual void setConfigPort(uint8_t i_u8_numPort, bool i_b_OutmodeInverse_InPullup) = 0;

  //! Recerche dans i_message le nom du module destinataire et l'ordre associé
  //! @return return 0 si l'odre le concerne, 1 sinon
  uint8_t predecodeOrdre(const String &i_message, String &o_ordre, uint32_t &o_u32_valeur);

  //! Met le module dans un nouvel etat
  virtual void setNouvelEtat(e_etatModule_t i_e_nouvelEtat) = 0;

  //! Timer intégré dans le module pour les durée monostable, les périodes de polling pour les capteurs, etc...
  TimerEvent_t m_TimerModule;

  //! Stockage de la configuration du module
  Class_ConfigModule m_ConfigModule;
  //! Stockage du comportement à adopter dans le module reçoit un déclenchement externe
  Class_ActionTriggered_t m_ActionTriggered;

  //! Fonction qui va lire le tableau ActionADeclencher et y lancer les actions renseignées dedans
  virtual void declencherTrigActions(void);

  //! Taille max du tableau ActionADeclencher
  static const uint8_t u8_tailleActionADeclencher = 10;
  //! Tableau listant toutes les actions à déclencher
  Class_ActionADeclencher ActionADeclencher[u8_tailleActionADeclencher];
};

#endif /* DOMODULE_H_ */
