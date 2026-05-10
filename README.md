# MicroROS-Pico2

Ouvrir le projet avec l'extension VSCode Platformio IDE

Upload par le port micro-usb :
- Brancher le Pi Pico (en mode ?) en usb
- Dans le fichier platformio.ini, mettre ```upload_protocol = picotool```, 
- Upload sur la carte (Fleche dans la barre inférieure)

Upload avec l'adaptateur CMSIS-DAP :
- Le Pico doit apparaitre dans la liste ```ls /dev/tty*``` (Généralement `/dev/ttyACM*`), s'il n'apparait pas, upload un code avec ```upload_protocol = picotool``` d'abord.
- Dans le fichier platformio.ini, mettre ```upload_protocol = cmsis-dap```
- Upload sur la carte (Fleche dans la barre inférieure), eventuellement selectionné le bon port s'il n'est pas automatiquement détecté.


Possibles problèmes et solution :
|Probleme|Solution|
|Le Pico n'apparait pas quand je le branche en usb|Brancher en mode bootload (maintenir le bouton en branchant)  \n- vérifier que `upload_protocol = picotool` \n- Vérifier le port |
|L'upload échoue Err OpenOCR| - Vérifier que le Pico est bien alimenté, - Vérifier Que SWCLK <> TCK, SWIO <> ? et le GND sont bien connectés au boitié SLogic|
|Autre Erreur d'upload| Débrancher/rebrancher l'USB|


## Lancement de MicroROS