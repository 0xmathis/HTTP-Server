#!/bin/bash

VAR1=$(cat /etc/hosts)
VAR2=$(cat hosts)

if [ "$VAR1" != "$VAR2" ]; then
  sudo mv /etc/hosts /etc/hosts.temp
  sudo cp hosts /etc/hosts
fi
