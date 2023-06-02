#!/bin/bash

VAR1=$(cat /etc/hosts)
VAR2=$(cat scripts/hosts)

if [ "$VAR1" != "$VAR2" ]; then
  echo On remplace le fichier /etc/hosts par notre propre fichier hosts
  echo L\'ancien fichier hosts est copié dans /etc/hosts.temp
  echo Pour restaurer l\'ancien fichier, faites make unprepa
  echo \(Permissions nécessaires pour modifier /etc/hosts\)
  sudo mv /etc/hosts /etc/hosts.temp
  sudo cp scripts/hosts /etc/hosts
fi

echo Remplacement effectué
