/*
 * domodule_capteur_analogique.cpp
 *
 *  Created on: 6 avr. 2022
 *      Author: julien
 */

#include "domodule_capteur_analogique.h"
#ifdef TRACE_DEBUG_ACTIVES
#include "trace_debug.h"
#endif

Domodule_Capteur_Analogique::Domodule_Capteur_Analogique(const char i_ps8_nomModule[24],
    uint8_t i_u8_numPort, bool i_b_OutmodeInverse_InPullup) :
    Domodule_Capteur(i_ps8_nomModule, i_u8_numPort, i_b_OutmodeInverse_InPullup), u16_seuilDeclenchement(
        512), u16_hystereris(0)
{
}

Domodule_Capteur_Analogique::~Domodule_Capteur_Analogique()
{
}

void Domodule_Capteur_Analogique::setConfigSurveillance(
    const Class_ConfigSurveillanceAnalogique &i_t_ConfigSurveillance)
{
  u16_seuilDeclenchement = i_t_ConfigSurveillance.u16_seuilDeclenchement;
  u16_hystereris = i_t_ConfigSurveillance.u16_hystereris;
  Domodule_Capteur::setConfigSurveillance(i_t_ConfigSurveillance);

}

void Domodule_Capteur_Analogique::DemarrerSurveillance(void)
{
  u16_valeurPort = analogRead(m_ConfigModule.u8_numPort);
  Domodule_Capteur::DemarrerSurveillance();
}

uint16_t Domodule_Capteur_Analogique::DonnerValeurCapteur(void)
{
  u16_valeurPort = analogRead(m_ConfigModule.u8_numPort);
  return u16_valeurPort;
}

void Domodule_Capteur_Analogique::surveillanceNiveau(void)
{
  uint16_t u16_valeurPortCourante = analogRead(m_ConfigModule.u8_numPort);
  bool b_TrigActionAEnvoyer = false;

  if (e_declencherEtat == e_declencherEtat_t::declencherEtatHaut)
  {
    if (u16_valeurPortCourante > u16_seuilDeclenchement)
    {
      if (u8_periodeFiltrageRestante == 0)
      {
        u16_valeurPort = u16_valeurPortCourante;
        b_TrigActionAEnvoyer = true;
      }
      else
      {
        u8_periodeFiltrageRestante--;
      }
    }
    else
    {
      u8_periodeFiltrageRestante = u8_nbrePeriodeFiltrage;
      u16_valeurPort = u16_valeurPortCourante;
    }
  }
  else if (e_declencherEtat == e_declencherEtat_t::declencherEtatBas)
  {
    if (u16_valeurPortCourante < u16_seuilDeclenchement)
    {
      if (u8_periodeFiltrageRestante == 0)
      {
        u16_valeurPort = u16_valeurPortCourante;
        b_TrigActionAEnvoyer = true;
      }
      else
      {
        u8_periodeFiltrageRestante--;
      }
    }
    else
    {
      u8_periodeFiltrageRestante = u8_nbrePeriodeFiltrage;
      u16_valeurPort = u16_valeurPortCourante;
    }
  }
  else if (e_declencherEtat == e_declencherEtat_t::declencherTransitionEtatHaut)
  {
    if ((u16_valeurPortCourante > u16_seuilDeclenchement)
        && (u16_valeurPort < u16_seuilDeclenchement))
    {
      if (u8_periodeFiltrageRestante == 0)
      {
        u16_valeurPort = u16_valeurPortCourante;
#ifdef TRACE_DEBUG_ACTIVES
        SEND_VTRACE(DBG1, "%s : Port Analogique Haut", m_ConfigModule.ts8_nomModule);
#endif
        b_TrigActionAEnvoyer = true;
      }
      else
      {
        u8_periodeFiltrageRestante--;
      }
    }
    else
    {
      u8_periodeFiltrageRestante = u8_nbrePeriodeFiltrage;
      u16_valeurPort = u16_valeurPortCourante;
    }
  }
  else if (e_declencherEtat == e_declencherEtat_t::declencherTransitionEtatBas)
  {
    if ((u16_valeurPortCourante < u16_seuilDeclenchement)
        && (u16_valeurPort > u16_seuilDeclenchement))
    {
      if (u8_periodeFiltrageRestante == 0)
      {
        u16_valeurPort = u16_valeurPortCourante;
#ifdef TRACE_DEBUG_ACTIVES
        SEND_VTRACE(DBG1, "%s : Port Analogique Bas", m_ConfigModule.ts8_nomModule);
#endif
        b_TrigActionAEnvoyer = true;
      }
      else
      {
        u8_periodeFiltrageRestante--;
      }
    }
    else
    {
      u8_periodeFiltrageRestante = u8_nbrePeriodeFiltrage;
      u16_valeurPort = u16_valeurPortCourante;
    }
  }
  else
  {

  }

  if (b_TrigActionAEnvoyer == true)
  {
    declencherTrigActions();
    b_TrigActionAEnvoyer = false;
  }

}
