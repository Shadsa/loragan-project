# LoRaGAN-Project


Note de dev :

- Penser à lancer les IDE en root ou admin, problème de droit lors du flashing de mote.
cf (https://github.com/arduino/ArduinoCore-samd/issues/36)
- Pense à check le port COM qui change régulièrement sous VSCode et Arduino (origine des problème de not found)
- Avoir la config suivante dans cpp_properties pour les libs (afin d'éviter le plantage intellisens de VSCode) : (A adapter selon la plateforme, le début des commande a été enlevé + mettre des / pour linux)

{
    "configurations": [
        {
            "name": "Win32", => A changer selon.
            "includePath": [
                "\\Arduino15\\packages\\SODAQ\\hardware\\samd\\1.6.19\\**",
                "\\Arduino\\**",
                "\\Arduino\\libraries\\TheThingsNetwork\\src\\**",
                "Arduino15\\packages\\**"
                "${workspaceRoot}\\include" => ligne très importante !!!! permet de lire nos propres lib
            ],
            "forcedInclude": [
                "Arduino15\\packages\\SODAQ\\hardware\\samd\\1.6.19\\cores\\arduino\\Arduino.h",
                Arduino\\libraries\\TheThingsNetwork\\src\\TheThingsNetwork.h"
            ],
            "intelliSenseMode": "msvc-x64",
            "compilerPath": "Microsoft Visual Studio/2017/Community/VC/Tools/MSVC/14.14.26428/bin/Hostx64/x64/cl.exe",
            "cStandard": "c11",
            "cppStandard": "c++17"
        }
    ],
    "version": 4


-lib pour mote : 
    + To use the board in the Arduino IDE you need to load a custom board file for the SODAQ SAMD boards.
http://downloads.sodaq.net/package_sodaq_samd_index.json
    + Install the latest version of the SODAQ SAMD boards
    + Install “Sodaq_RN2483” and “Sodaq_wdt”
    + Install TTN library
- Lien utile 
    + Pour mote
        . https://support.sodaq.com/sodaq-one




- Usage du runner : Lancer le script de positionnement des fichiers, puis lancer le sketch présent dans le runner. Le runner est ignoré par le git.