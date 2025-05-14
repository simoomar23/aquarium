#!/bin/bash

if [ -z "$1" ]; then
  echo "Usage: $0 <ID (N1 à N4)>"
  exit 1
fi

ID=$1


case "$ID" in
  N1) poisson="PoissonNain" ;;
  N2) poisson="PoissonRouge" ;;
  N3) poisson="PoissonClown" ;;
  N4) poisson="PoissonNain1" ;;
  *) echo "ID inconnu : $ID" ; exit 1 ;;
esac

# Coordonnées aléatoires entre 20 et 80
p=$((RANDOM % 61 + 20))
q=$((RANDOM % 61 + 20))
r=$((RANDOM % 61 + 20))
t=$((RANDOM % 61 + 20))


javac --module-path ~/javafx-sdk/lib --add-modules javafx.controls,javafx.fxml Poisson.java AquariumApp.java Client.java


(
  sleep 5
  echo "addFish $poisson at ${p}x${q}, ${t}x${r}, RandomWayPoint"
  sleep 5
  echo "status"
  sleep 5
  echo "startFish $poisson"
  sleep 5
  echo "status"
  sleep 30
  echo "delFish $poisson"
  sleep 10
  echo "log out"
) | java --module-path ~/javafx-sdk/lib --add-modules javafx.controls,javafx.fxml Client 0.0.0.0 "$ID"
exit 0
