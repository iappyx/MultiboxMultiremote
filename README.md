# MultiboxMultiremote for Itho
*Remote control for multiple Itho fans using IDs of (multiple) own physical remotes.*

Includes webinterface using Bootstrap and supports OTA upgrades, an API, MQTT and a DHT22 sensor. A CC1101 868Mhz transmitter is needed. The 433Mhz version also seems to work, but range may be limited and commands sent by physical remotes are not always detected.

Using a modified version of the ITHORadio library (included) from https://github.com/philipsen/IthoRadio. In turn that ITHORadio library is originally based on the library from https://github.com/supersjimmie/IthoEcoFanRFT/tree/master/Master/Itho made by 'supersjimmie' and 'klusjesman'. 

### Background
The ITHORadio library was used as it allows custom remote IDs to be configured. Originally I tried to create a custom version of the ESP Easy plugin from https://github.com/svollebregt/ESPEASY_Plugin_ITHO. This succeeded but the plugin was not very stable and some settings could not be configured due to how ESPEasy works. Therefore I decided to create this dedicated remote. The ESP Easy plugin is still available here: https://github.com/iappyx/ESPEASY_Plugin_ITHORadio/, but will not be maintained or updated. For more generic info, background and history see: https://gathering.tweakers.net/forum/list_messages/1690945

## Set-up and configuration

### Hardware
Although you can configure many settings it is recommended to use the following configuration. Tested with a Wemos D1 Mini.

|CC11xx pin    |ESP pins|Description                                        |
|:-------------|:-------|:--------------------------------------------------|
|1 - VCC       |VCC     |3v3                                                |
|2 - GND       |GND     |Ground                                             |
|3 - MOSI      |13=D7   |Data input to CC11xx                               |  
|4 - SCK       |14=D5   |Clock pin                                          |
|5 - MISO/GDO1 |12=D6   |Data output from CC11xx / serial clock from CC11xx |
|6 - GDO2      |04=D1*  |output as a symbol of receiving or sending data    |
|7 - GDO0      |        |output as a symbol of receiving or sending data    |
| 8 - CSN      |15=D8   |Chip select / (SPI_SS)                             |

Not recommended pins for intterupt:
- Boot pins D3(GPIO0) and D4 (GPIO2) 
- Pin with no interrupt support: D0 (GPIO16)

### Software
The software will work with one or multiple Ithoboxes with one or mutiple physical remotes paired to the Ithobox.

The set-up is a multi-step process:

#### Phase 1 - Initial set-up
1) Set-up a Mqtt-server in your home network, with a fixed IP address, using port 1883. Protect it with username and password.
2) Configure the Mqtt-settings, SSID and PSDK in *config.h*. Also make sure the CC110_PIN and DHT_PIN settings are right.
3) In the same file configure the DHT22-settings or disable it if you don't use one.
4) Build the firmware and upload it to your device

#### Phase 2 - Sniffing the remote IDs and command IDs
5) You can either get the remote IDs and command IDs by watching the ITHO/Log topic via Mqtt or using a serial monitor (Baud 115200).
6) Get close to a physical remote and press the first button. Note the remote ID and command ID for this command. In the output (Mqtt or Serial) they are shown as "Copy this command --> xx:xx:xx:xx:xx:xx" and "Copy this remote  --> xx:xx:xx".
7) Repeat this for all the buttons of the remote. Note: some buttons support multiple commands, if you press them two times or three times. This is the case with Timer commands or Auto-commands, depending on the remote. You can also snif Pair and Unpair commands, but these are not used in this software as we do not require it to pair with the Ithobox, since the remote is 'cloned' by the software.

#### Phase 3 - Configure the remote IDs and command IDs in the software
8) Configure CONFIG_NUMBEROFITHOBOXES (number of Ithoboxes you have) and CONFIG_MAXREMOTESPERBOX (maximum number of remotes you have per Ithobox) in *config.h*
8) Change any other settinfs in *config.h* if appropriate
9) Open *config_boxes_and_remotes.h* and configure the remote IDs and command IDs you sniffed in step 6. Make sure the Itho-boxes object (boxesAndRemotes[]) is correct. How the object is meant to be set-up is described in the object definition in config.h (line #76 to #105). Use the filled in data as an example. Please note that remoteIDs should be entered as as string (xx:xx:xx) and commands as bytes. You can remove unused remotes, or replace them with NULL-values. For privacy reasons my remote IDs are shown as 00:00:00 in the example.

Please note, you should at least set:
- name, friendly_name and numremotes of Ithoboxes; 
- remote_id and friendly_name for IthoRemotes; 
- name, friendly_name, bytes and length for RemoteCommandExtended. 

I will post better instructions for this later in time.

10) Build the firmware and upload it to your device

### Control your Itho fans:
There are three ways to control your Itho-fans:
1) Using the webinterface on: http://YOURIP/.
2) Using API-calls on: http://YOURIP/api/. Example commands: http://<YOURIP>/api?remote=xx:xx:xx&action=state&value=high to change state to high; http://YOURIP/api?action=reset&value=true to reset the device.
3) Using Mqtt. Set the payload of the ITHO/BOXNAME/Fan/Cmd topic to an appropiate state. I use this to control my Ithoboxes via HomeAssistant (see sample config below).

### Firmware OTA upgrades
If you want to do an OTA upgrade of the firmware, go to the webinterface. Click on "System" and then "Switch off loops before OTA updates". Then again click on "System" and then "Update firmware OTA".

### Sample configuration for HomeAssistant
This is the configuration I use for HomeAssistant in configuration.yaml. Example shown is for 1 box:
```
fan:
  - platform: mqtt
    command_topic: "ITHO/ITHO_1/Fan/Cmd"
    state_topic : "ITHO/ITHO_1/Fan/State"
    state_value_template: "on"
    speed_command_topic: "ITHO/ITHO_1/Fan/Cmd"
    speed_state_topic : "ITHO/ITHO_1/Fan/State"
    payload_on: "on"
    payload_off: "off"
    payload_low_speed: "low"
    payload_medium_speed: "auto1"
    payload_high_speed: "high"
    optimistic: "true"
    name: Afzuiging badkamer
    speeds:
      - low
      - medium
      - high
      
 sensor:
  - platform: mqtt
    name: Tijd
    state_topic: "ITHO/ITHO_1/Fan/Timer"
    value_template: "{{value}}"
    unit_of_measurement: 's'
  - platform: mqtt
    name : Snelheid
    state_topic: "ITHO/ITHO_1/Fan/State"
    value_template: >
      {% if value=="off" %}Standby{% endif %}
      {% if value=="low" %}Laag{% endif %}
      {% if value=="auto1" %}Auto 1{% endif %}
      {% if value=="auto2" %}Auto 2{% endif %}
      {% if value=="high" %}Hoog{% endif %}
      {% if value=="full" %}Full{% endif %}
      {% if value=="timer1" %}Hoog (10 min){% endif %}
      {% if value=="timer2" %}Hoog (20 min){% endif %}
      {% if value=="timer3" %}Hoog (30 min){% endif %}
  - platform: mqtt
    name: "Itho Temperature"
    state_topic: "ITHO/Temperature"
    unit_of_measurement: '°C'
  - platform: mqtt
    name: "Itho Humidity"
    state_topic: "ITHO/Humidity"
    unit_of_measurement: '%'
```
