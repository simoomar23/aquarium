#!/bin/bash

# Définir le module-path JavaFX pour éviter la répétition
JAVAFX_OPTS="--module-path ~/javafx-sdk/lib --add-modules javafx.controls,javafx.fxml"

# 1ère fenêtre : lancement du client, pause 3 s, puis commande addFish
gnome-terminal --title="Client 1" -- bash -c "
  exec java $JAVAFX_OPTS Client 0.0.0.0 <<'EOF'
addFish PoissonNain at 0x0, 20x16, RandomWayPoint
startFish PoissonNain

EOF
"


# 2ème fenêtre : lancement du client seul
gnome-terminal --title="Client 2" -- bash -c "
  java $JAVAFX_OPTS Client 0.0.0.0;
  exec bash
"

# 2ème fenêtre : lancement du client seul
gnome-terminal --title="Client 3" -- bash -c "
  java $JAVAFX_OPTS Client 0.0.0.0;
  exec bash
"

# 2ème fenêtre : lancement du client seul
gnome-terminal --title="Client 4" -- bash -c "
  java $JAVAFX_OPTS Client 0.0.0.0;
  exec bash
"