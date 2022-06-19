# Domodule

## Synthèse

Jeux de classes de bases pour des applications de domotique en environnement Arduino liké (fonctionne sur atmega328, Esp8266, Esp32, etc...)

Ces modules peuvent, soit réagir des commandes d'autres modules auquel ils sont liés, soit réagir à des ordres réçus par un quelconque moyen. Ceux-ci devront être sous la forme d'une chaine de caractères sous la forme: "NOM_MODULE - ORDRE" sachant que ORDRE peut être suivi d'un argument facultatif sous la forme d'un entier.

## Détails par type de module

- Domodule:
Classe abstraite de base contenant toutes les attributs et méthodes (ou interfaces) communes à tous les modules. Toutes les autres classes dérivent de Domodule.

- Domodule_Actionneur:
Classe pour controler un port en sortie binaire selon les ordres reçus.
Le module peut gérer trois états: ON, OFF, et un module monostable appelé DRING dans lequel le module passe en ON x secondes (argument passé par l'ordre) puis repasse en OFF.

- Domodule_Capteur
Classe abstraite permettant de surveiller un port (numérique ou analogique, etc.. selon la classe fille) et de déclencher des call back si l'évolution de l'état du port selon les réglages définis dans Config_Surveillance. La surveillance est réalisée uniquement en polling en ms (sachant que la résolution du timer logiciel le gérant est de 50 ms), et peut en option être filtrée (x périodes sont nécessaires pour valider l'état)



