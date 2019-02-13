# LoRaGAN-Project


Note de dev :

- Penser à lancer les IDE en root ou admin, problème de droit lors du flashing de mote.
cf (https://github.com/arduino/ArduinoCore-samd/issues/36)
- Pense à check le port COM qui change régulièrement sous VSCode et Arduino (origine des problème de not found)

-lib pour mote : 
    + To use the board in the Arduino IDE you need to load a custom board file for the SODAQ SAMD boards.
http://downloads.sodaq.net/package_sodaq_samd_index.json
    + Install the latest version of the SODAQ SAMD boards
    + Install “Sodaq_RN2483” and “Sodaq_wdt”
    + Install TTN library
- Lien utile 
    + Pour mote
        . https://support.sodaq.com/sodaq-one