/*
 * domodule_capteur_numerique.cpp
 *
 *  Created on: 2 avr. 2022
 *      Author: julien
 */

#include "domodule_capteur_numerique.h"
#ifdef TRACE_DEBUG_ACTIVES
#include "trace_debug.h"
#endif

Domodule_Capteur_Numerique::Domodule_Capteur_Numerique(const char i_ps8_nomModule[24], uint8_t i_u8_numPort,
    bool i_b_OutmodeInverse_InPullup) :
    Domodule_Capteur(i_ps8_nomModule, i_u8_numPort, i_b_OutmodeInverse_InPullup)
{
}

Domodule_Capteur_Numerique::~Domodule_Capteur_Numerique()
{
  m_TimerModule.Delete();
}

void Domodule_Capteur_Numerique::setConfigSurveillance(const Class_ConfigSurveillanceNumerique &i_t_ConfigSurveillance)
{
  Domodule_Capteur::setConfigSurveillance(i_t_ConfigSurveillance);
}

uint16_t Domodule_Capteur_Numerique::DonnerValeurCapteur(void)
{
  u16_valeurPort = digitalRead(m_ConfigModule.u8_numPort);
  return u16_valeurPort;
}

void Domodule_Capteur_Numerique::DemarrerSurveillance(void)
{
  u16_valeurPort = digitalRead(m_ConfigModule.u8_numPort);
  Domodule_Capteur::DemarrerSurveillance();
}

void Domodule_Capteur_Numerique::surveillanceNiveau(void)
{
  uint16_t u16_valeurPortCourante = digitalRead(m_ConfigModule.u8_numPort);
  bool b_TrigActionAEnvoyer = false;

  if ((e_declencherEtat == e_declencherEtat_t::declencherEtatHaut) && (u16_valeurPortCourante == HIGH))
  {
    if (u8_periodeFiltrageRestante == 0)
    {
#ifdef TRACE_DEBUG_ACTIVES
      Send_Trace_Text(DBG1, m_ConfigModule.ts8_nomModule, " : Port Etat Haut");
#endif
      u16_valeurPort = u16_valeurPortCourante;
      b_TrigActionAEnvoyer = true;
    }
    else
    {
      u8_periodeFiltrageRestante--;
    }
  }
  else if ((e_declencherEtat == e_declencherEtat_t::declencherEtatBas) && (u16_valeurPortCourante == LOW))
  {
    if (u8_periodeFiltrageRestante == 0)
    {
#ifdef TRACE_DEBUG_ACTIVES
      Send_Trace_Text(DBG1, m_ConfigModule.ts8_nomModule, " : Port Etat Bas");
#endif
      u16_valeurPort = u16_valeurPortCourante;
      b_TrigActionAEnvoyer = true;
    }
    else
    {
      u8_periodeFiltrageRestante--;
    }

  }
  // si pas de différence, rien à faire !
  else if (u16_valeurPortCourante != u16_valeurPort)
  {
    if (u8_periodeFiltrageRestante == 0)
    {
      u16_valeurPort = u16_valeurPortCourante;

      if (u16_valeurPortCourante == HIGH)
      {
        if ((e_declencherEtat == e_declencherEtat_t::declencherTransitionEtatHaut)
            || (e_declencherEtat == e_declencherEtat_t::declecherTransitionHautBas))
        {
#ifdef TRACE_DEBUG_ACTIVES
          Send_Trace_Text(DBG1, m_ConfigModule.ts8_nomModule, " : Port Transition Etat Haut");
#endif
          b_TrigActionAEnvoyer = true;
        }
      }
      else // donc valeur LOW
      {
        if ((e_declencherEtat == e_declencherEtat_t::declencherTransitionEtatBas)
            || (e_declencherEtat == e_declencherEtat_t::declecherTransitionHautBas))
        {
#ifdef TRACE_DEBUG_ACTIVES
          Send_Trace_Text(DBG1, m_ConfigModule.ts8_nomModule, " : Port Transition Etat Bas");
#endif
          b_TrigActionAEnvoyer = true;
        }
      }
    }
    else
    {
      u8_periodeFiltrageRestante--;
    }
  }
  else
  {
    u8_periodeFiltrageRestante = u8_nbrePeriodeFiltrage;
  }

  if (b_TrigActionAEnvoyer == true)
  {
    declencherTrigActions();
    b_TrigActionAEnvoyer = false;
  }
  
}

