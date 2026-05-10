# MicroROS-Pico2

Ouvrir le projet avec l'extension VSCode PlatformIO IDE.

## Upload via USB (picotool)

1. Brancher le Pi Pico en mode bootload (maintenir le bouton BOOTSEL en branchant)
2. Dans `platformio.ini`, définir :
   ```ini
   upload_protocol = picotool
   ```
3. Cliquer sur la flèche Upload dans la barre inférieure

## Upload via adaptateur CMSIS-DAP

1. Vérifier que le Pico apparaît dans `ls /dev/tty*` (généralement `/dev/ttyACM*`)  
   Si absent, effectuer d'abord un upload avec `upload_protocol = picotool`
2. Dans `platformio.ini`, définir :
   ```ini
   upload_protocol = cmsis-dap
   ```
3. Cliquer sur la flèche Upload, sélectionner le port si non détecté automatiquement

## Problèmes courants

| Problème | Solution |
|----------|----------|
| Le Pico n'apparaît pas en USB | Brancher en mode bootload (maintenir BOOTSEL en branchant) · Vérifier `upload_protocol = picotool` · Vérifier le port |
| Upload échoue — `Err OpenOCD` | Vérifier l'alimentation du Pico · Vérifier les connexions : `SWCLK ↔ TCK`, `SWDIO ↔ TMS`, `GND` · Vérifier que le SLogic est en mode DAP (vert) |
| Autre erreur d'upload | Débrancher / rebrancher l'USB |

## Voir les logs Serial1

1. Connecter les pins RX, TX et GND du port de debug au SLogic
2. Sélectionner le mode DAP (vert) — UART (rouge) fonctionne aussi, mais DAP permet en plus l'upload
3. Identifier le port :
   ```bash
   ls /dev/tty*
   ```
4. Installer `tio` si nécessaire :
   ```bash
   sudo apt update && sudo apt install tio
   ```
5. Se connecter :
   ```bash
   tio /dev/ttyACM0
   ```
   Quitter avec **Ctrl+T** puis **Q**

## Lancement de MicroROS

Ouvrir le container de test :
```bash
./ROS2-WS-test/enter.sh
```

Lancer l'agent micro-ROS :
```bash
docker run -it --rm --privileged -v /dev:/dev --net=host microros/micro-ros-agent:jazzy serial --dev /dev/ttyACM1
```
