#!/bin/bash

if [ -z "$1" ]; then
  echo "Usage: $0 <ID (N1 à N4)>"
  exit 1
fi

ID=$1


case "$ID" in
  N1) poisson1="PoissonNain" && poisson2="PoissonRouge" ;;
  N2) poisson1="PoissonRouge" && poisson2="PoissonNain" ;;
  N3) poisson1="PoissonClown" && poisson2="PoissonClown" ;;
  N4) poisson1="PoissonNain1" && poisson2="PoissonNain1" ;;
  *) echo "ID inconnu : $ID" ; exit 1 ;;
esac


# Coordonnées aléatoires entre 20 et 80
p=$((RANDOM % 61))
q=$((RANDOM % 61))
r=$((RANDOM % 31))
t=$((RANDOM % 31))


javac --module-path ~/javafx-sdk/lib --add-modules javafx.controls,javafx.fxml Poisson.java AquariumApp.java Client.java


(
  sleep 5
  echo "addFish $poisson1 at 0x0, 30x20, RandomWayPoint"
  echo "startFish $poisson1"

  sleep 50
  echo "delFish $poisson2"
  sleep 10
) | java --module-path ~/javafx-sdk/lib --add-modules javafx.controls,javafx.fxml Client
exit 0
