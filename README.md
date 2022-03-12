# Affichage_Synchrone_Video

Réalisation d'un logiciel en C++ permettant l'affichage de vidéos synchronisées avec une musique. Le projet à été réalise par 4 personnes.

- Un programme "client" et "master" permettant la détection des appareils clients sur le réseau par le master ( UDP ) ainsi que la sauvegarde d'une configuration réseau pour une utilisation future ( XML et bibliothèque TinyXML). Le master transfère les vidéos au client et la configuration réseau grâce au TFTP. 

- Ma partie était de réaliser le transfert de vidéos par l'intermédiaire du TFTP et la création d'une IHM pour le Master développé avec wxWidget. Dans le code du Master vous retrouverez le code de la configuration réseau, mais elle n'a pas été réalisé par moi même.

- Parmis les fichiers il y a la partie complète du master et le TFTP du client, il manque la plus grande partie coté client qui n'a pas été réalisé par moi même
