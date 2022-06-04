/*
 * domoduleactionneur.h
 *
 *  Created on: 2 avr. 2022
 *      Author: julien
 */

#ifndef DOMODULE_ACTIONNEUR_H_
#define DOMODULE_ACTIONNEUR_H_

#include "domodule.h"

class Domodule_Actionneur: public Domodule
{
public:
  Domodule_Actionneur(const char i_ps8_nomModule[24] = "", uint8_t i_u8_numPort = 0, bool i_b_OutmodeInverse_InPullup =
      false);
  virtual ~Domodule_Actionneur();

  virtual void moduleON(void) override;
  virtual void moduleOFF(void) override;
  virtual void moduleDRING(uint32_t i_u32_argument) override;

  uint8_t decodeOrdre(const String &i_message, String &o_reponse);

protected:
  virtual void setConfigPort(uint8_t i_u8_numPort, bool i_b_OutmodeInverse_InPullup = false) override;

  static void triggerOFF(uint32_t valeur, void *Arg);
  static void triggerON(uint32_t valeur, void *Arg);

  virtual void setNouvelEtat(e_etatModule_t i_e_nouvelEtat) override;

  uint8_t u8_EtatON;
  uint8_t u8_EtatOFF;
};

#endif /* DOMODULE_ACTIONNEUR_H_ */
