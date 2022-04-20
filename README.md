# Domodule

Jeux de classes de bases pour des applications de domotique en environnement Arduino liké (fonctionne sur atmega328, Esp8266, Esp32, etc...)

Domodule est une classe virtuelle de base. 

Domodule_actionneur permet de contrôler un port en sortie selon les ordres reçus. 

Domodule_capteur_xxx permettent de surveiller un port (numérique ou analogique selon la classe) et de déclencher une call back si l'état du port. 
