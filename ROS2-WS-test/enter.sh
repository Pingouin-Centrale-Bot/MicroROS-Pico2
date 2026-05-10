#!/bin/bash

# Chemin absolu du dossier actuel
PROJECT_PATH=$(pwd)

# Lancement du container
# --workdir /work permet d'arriver directement dans le bon dossier
docker run -it --rm \
  --net=host \
  -v "$PROJECT_PATH":/work \
  --workdir /work \
  osrf/ros:jazzy-desktop
