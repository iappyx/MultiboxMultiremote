/*
   ╔═══════════════════════════════════════════════════════════════════════════════════════════════════════════════════╗
   ║ MultiboxMultiremote for Itho - https://github.com/iappyx/MultiboxMultiremote                                      ║
   ╠═══════════════════════════════════════════════════════════════════════════════════════════════════════════════════╣
   ║                                                                                                                   ║
   ║ Remote control for multiple Itho fans using IDs of (multiple) own physical remotes.                               ║
   ║ Includes webinterface using Bootstrap and supports API, MQTT and a DHT22 sensor.                                  ║
   ║ Created by: Iappyx / Simonstar.                                                                                   ║
   ║                                                                                                                   ║
   ║ Using a modified version of the ITHORadio library (included) from https://github.com/philipsen/IthoRadio.         ║
   ║ The ITHORadio library is originally based on the library from:                                                    ║
   ║ https://github.com/supersjimmie/IthoEcoFanRFT/tree/master/Master/Itho made by 'supersjimmie' and 'klusjesman'.    ║
   ║ A CC1101 868Mhz transmitter is needed. The 433Mhz version also seems to work (range may be limited).              ║
   ║ For more info, background and history see: https://gathering.tweakers.net/forum/list_messages/1690945             ║
   ║                                                                                                                   ║
   ╚═══════════════════════════════════════════════════════════════════════════════════════════════════════════════════╝
   ╔════════╦═══════════╦══════════════════════════════════════════════════════════════════════════════════════════════╗
   ║ Name   ║ Date      ║ Description                                                                                  ║
   ╠════════╬═══════════╬══════════════════════════════════════════════════════════════════════════════════════════════╣
   ║ Iappyx ║ 31-5-2020 ║ Initial public release (v1.0)                                                                ║
   ╠════════╬═══════════╬══════════════════════════════════════════════════════════════════════════════════════════════╣
   ║ Iappyx ║  6-6-2020 ║ Minor updates (v1.01)                                                                        ║
   ╠════════╬═══════════╬══════════════════════════════════════════════════════════════════════════════════════════════╣
   ║ Iappyx ║  8-6-2020 ║ Bugs with some remotes and timers resolved (v1.02)                                           ║
   ╚════════╩═══════════╩══════════════════════════════════════════════════════════════════════════════════════════════╝
   ╔═══════════════════════════════════════════════════════════════════════════════════════════════════════════════════╗
   ║ Documentation available on https://github.com/iappyx/MultiboxMultiremote                                          ║
   ║ Please set configuration in config.h                                                                              ║
   ╚═══════════════════════════════════════════════════════════════════════════════════════════════════════════════════╝
*/

//External
#include "Arduino.h"
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>
#include <ESP8266HTTPUpdateServer.h>
#include <PubSubClient.h>
#include "IthoReceive.h"
#include "IthoSender.h"
#include "DHT.h"

//Internal
#include "config.h"
#include "config_boxes_and_remotes.h"
#include "returnFunctions.h"

void readDHT()
{
	dhtHumidity = dht.readHumidity();
	dhtTemperature = dht.readTemperature(false);

	if (isnan(dhtHumidity) || isnan(dhtHumidity))
	{
		return;
	}

	mqttClient.publish(mqttHumidityTopic, String(dhtHumidity).c_str());
	mqttClient.publish(mqttTemperatureTopic, String(dhtTemperature).c_str());
}

void showHomepage()
{
	delay(200);

	String Version = "1.02";
	String HTML = "<!DOCTYPE html><html lang='en'><head> <meta charset='utf-8'> <meta http-equiv='X-UA-Compatible' content='IE=edge'> <meta name='viewport' content='width=device-width, initial-scale=1'>";
	HTML += "<script>function convertFromMillis(milliseconds) { var day, hour, minute, seconds; seconds = Math.floor(milliseconds / 1000); minute = Math.floor(seconds / 60); seconds = seconds % 60; hour = Math.floor(minute / 60); minute = minute % 60; day = Math.floor(hour / 24); hour = hour % 24; return { day: day, hour: hour, minute: minute, seconds: seconds };}</script>";
	HTML += "<title>MultiboxMultiremote for Itho</title> <link rel='stylesheet' href='https://stackpath.bootstrapcdn.com/bootstrap/4.5.0/css/bootstrap.min.css' integrity='sha384-9aIt2nRpC12Uk9gS9baDl411NQApFmC26EwAOH8WgZl5MYYxFfc+NcPb1dKGj7Sk' crossorigin='anonymous'></head><body> <div class='container-fluid'> <div class='row'> <div class='col-md-2'> </div><div class='col-md-8'> <nav class='navbar navbar-expand-lg navbar-dark bg-dark'> <button class='navbar-toggler' type='button' data-toggle='collapse' data-target='#bs-example-navbar-collapse-1'> <span class='navbar-toggler-icon'></span> </button> <a class='navbar-brand' href='#'>MultiboxMultiremote for Itho</a>";
	HTML += "<script src='https://code.jquery.com/jquery-3.5.1.slim.min.js' integrity='sha384-DfXdz2htPH0lsSSs5nCTpuj/zy4C+OGpamoFVy38MVBnE+IbbVYUew+OrCXaRkfj' crossorigin='anonymous'></script> <script src='https://cdn.jsdelivr.net/npm/popper.js@1.16.0/dist/umd/popper.min.js' integrity='sha384-Q6E9RHvbIyZFJoft+2mJbHaEWldlvI9IOYy5n3zV9zzTtmI3UksdQRVvoxMfooAo' crossorigin='anonymous'></script> <script src='https://stackpath.bootstrapcdn.com/bootstrap/4.5.0/js/bootstrap.min.js' integrity='sha384-OgVRvuATP1z7JjHLkuOU7Xw704+h835Lr+6QL9UvYjZE3Ipu6Tp75j7Bh/kR0JKI' crossorigin='anonymous'></script>";
	HTML += "<div class='collapse navbar-collapse' id='bs-example-navbar-collapse-1'> <ul class='navbar-nav'> <li class='nav-item dropdown'> <a class='nav-link dropdown-toggle' href='#' id='navbarDropdownMenuLink' data-toggle='dropdown'>System</a> <div class='dropdown-menu' aria-labelledby='navbarDropdownMenuLink'>";

	if (loopLoop)
	{
		HTML += "<a class='dropdown-item' href='/api?action=toggleOnOff&value=true'>Switch off loops before OTA update</a><a class='dropdown-item' href='/api?action=reset&value=true'>Reboot device</a>";
	}
	else
	{
		HTML += "<a class='dropdown-item' href='/api?action=toggleOnOff&value=true'>Switch on loops</a><a class='dropdown-item' href='/api?action=reset&value=true'>Reboot device</a> <a class='dropdown-item' href='/update'>Update firmware OTA</a>";
	}

	HTML += "<a class='dropdown-item' href='https://github.com/iappyx/MultiboxMultiremote'>MultiboxMultiremote on GitHub</a>";
	HTML += "</div></li></ul> </div></nav> <nav aria-label='breadcrumb'> <ol class='breadcrumb'> <li class='breadcrumb-item' aria-current='page'>Home</li><li class='breadcrumb-item active' aria-current='page'>Status</li></ol> </nav> <div class='col-md-12'>&nbsp; </div><div class='row'> <div class='col-md-6'>";
	HTML += "<table class='table table-hover table-sm border'><tbody>";
	HTML += "<tr><th scope='row'> IP Address </th><td>" + WiFi.localIP().toString() + "</td></tr>";
	HTML += "<tr><th scope='row'> Client name </th><td>" + String(host) + "</td></tr>";
	HTML += "<tr><th scope='row'> Version </th><td>" + Version + "</td></tr>";

	// Let's format uptime on the client-side
	HTML += "<tr><th scope='row'> Uptime </th><td><script> var uptime = convertFromMillis(" + String(millis()) + "); document.write(uptime.day + ' day(s) ' + uptime.hour + ' hours ' + uptime.minute +' min ' + uptime.seconds + ' sec '); </script></td></tr>";

	if (dhtEnabled == 1)
	{
		HTML += "<tr><th scope='row'> Temperature </th><td>" + String(dhtTemperature) + "&#8451;</td></tr>";
		HTML += "<tr><th scope='row'> Humidity </th><td>" + String(dhtHumidity) + "%</td></tr>";
	}

	HTML += "</tbody></table>";
	HTML += "<div class='col-md-12'>&nbsp; </div></div><div class='col-md-6'>";

	if (loopLoop)
	{
		for (int i = 0; i < numberOfIthoBoxes; i++)
		{

			if (boxesAndRemotes[i].timer > 0)
			{
				HTML += "<div class='card'> <div class='card-body'> <h5 class='card-title'>" + String(boxesAndRemotes[i].friendly_name) + "</h5>";
				HTML += "<div class='progress'><div id='progress-bar-" + String(char('a' + (char)i)) + "' class='progress-bar' role='progressbar' style='width:" + String((float)boxesAndRemotes[i].timer / (float)boxesAndRemotes[i].timer_full * 100) + "%' aria-valuenow='" + String(boxesAndRemotes[i].timer) + "' aria-valuemin='0' aria-valuemax='" + String(boxesAndRemotes[i].timer_full) + "'>" + String(boxesAndRemotes[i].timer) + " sec. remaining</div></div><div class='col-md-12'>&nbsp; </div>";
				HTML += "<script type='text/javascript'>var " + String(char('a' + (char)i)) + " = " + String(boxesAndRemotes[i].timer) + "; var progressBar" + String(char('a' + (char)i)) + " = $('#progress-bar-" + String(char('a' + (char)i)) + "'); function reduce" + String(char('a' + (char)i)) + "Bar(){ if(" + String(char('a' + (char)i)) + " > 0){ " + String(char('a' + (char)i)) + " = " + String(char('a' + (char)i)) + " - 1; progressBar" + String(char('a' + (char)i)) + ".css('width', ((" + String(char('a' + (char)i)) + "/" + String(boxesAndRemotes[i].timer_full) + ")*100) + '%').text(" + String(char('a' + (char)i)) + "+ ' sec. remaining'); } setTimeout('reduce" + String(char('a' + (char)i)) + "Bar()', 1000);} reduce" + String(char('a' + (char)i)) + "Bar(); </script>";
			}
			else
			{
				HTML += "<div class='card'> <div class='card-body'> <h5 class='card-title'>" + String(boxesAndRemotes[i].friendly_name) + "</h5> <div class='col-md-12'>&nbsp; </div>";
			}

			for (int ii = 0; ii < boxesAndRemotes[i].numremotes; ii++)
			{

				HTML += "<h6 class='card-subtitle mb-2 text-muted'>" + String(boxesAndRemotes[i].remotes[ii].friendly_name) + "</h6> <nav class='nav nav-pills flex-column flex-sm-row'>";
				for (int iii = 0; iii < maxCommandsPerRemote; iii++)
				{
					if (!boxesAndRemotes[i].remotes[ii].remote_commands[iii].name == NULL)
					{
						if (String(boxesAndRemotes[i].remotes[ii].remote_commands[iii].name).equalsIgnoreCase(String(boxesAndRemotes[i].state)))
						{
							HTML += "<a class='flex-sm-fill text-sm-center nav-link active' href='/api?remote=" + String(boxesAndRemotes[i].remotes[ii].remote_id) + "&action=state&value=" + String(boxesAndRemotes[i].remotes[ii].remote_commands[iii].name) + "&redirect=button'>" + String(boxesAndRemotes[i].remotes[ii].remote_commands[iii].friendly_name) + "</a>";
						}
						else
						{
							HTML += "<a class='flex-sm-fill text-sm-center nav-link' href='/api?remote=" + String(boxesAndRemotes[i].remotes[ii].remote_id) + "&action=state&value=" + String(boxesAndRemotes[i].remotes[ii].remote_commands[iii].name) + "&redirect=button'>" + String(boxesAndRemotes[i].remotes[ii].remote_commands[iii].friendly_name) + "</a>";
						}
					}
				}
				HTML += "</nav><div class='col-md-12'>&nbsp; </div>";
			}
			HTML += "</div></div><div class='col-md-12'>&nbsp;</div>";
		}
	}

	HTML += "</div></div></div><div class='col-md-2'> </div></div></div>";
	HTML += "</body></html>";

	httpServer.send(200, "text/html", HTML);
}

void processAPI()
{
	String remote = httpServer.arg("remote");
	String action = httpServer.arg("action");
	String value = httpServer.arg("value");
	String redirect = httpServer.arg("redirect");

	if (action.equalsIgnoreCase("reset") && value.equalsIgnoreCase("true"))
	{
		httpServer.send(200, "text/html", "Reset ESP OK");
		delay(500);
		ESP.restart();
	}

	if (action.equalsIgnoreCase("toggleOnOff") && value.equalsIgnoreCase("true"))
	{
		if (loopLoop)
		{
			loopLoop = false;
		}
		else
		{
			loopLoop = true;
		}
		httpServer.sendHeader("Location", "/");
		httpServer.send(303);
	}

	if ((!returnBoxName(remote).equalsIgnoreCase("")) && (returnBoxIDByRemote(remote) > -1) && (returnRemoteIDByRemote(remote) > -1) && action.equalsIgnoreCase("state") && (loopLoop))
	{
		int boxId = returnBoxIDByRemote(remote);

		String boxName = String(returnBoxName(remote));
		String remoteId = String(returnRemoteIDByRemote(remote) + 1);
		String oldValue = String(boxesAndRemotes[returnBoxIDByRemote(remote)].state);

		int lastIDindex = (returnRemoteIDByRemote(remote) + 1);
		int remoteTimer = returnTimerByRemoteAndCommand(remote, value) * 60;
		int commandID = returnCommandID(remote, value);

		if (configLog)
		{
			if (serialLog)
			{
				Serial.println(String("Command '" + value + "' sent to box #" + String(boxId + 1) + " (" + boxName + ") with remote #" + remoteId + " (" + remote + ")."));
			}

			mqttClient.publish(mqttLogTopic, String("Command '" + value + "' sent to box #" + String(boxId + 1) + " (" + boxName + ") with remote #" + remoteId + " (" + remote + ").").c_str());
		}

		if (commandID > -1)
		{
			strcpy(boxesAndRemotes[returnBoxIDByRemote(remote)].oldstate, oldValue.c_str());
			strcpy(boxesAndRemotes[returnBoxIDByRemote(remote)].state, value.c_str());
			boxesAndRemotes[returnBoxIDByRemote(remote)].timer = remoteTimer;
			boxesAndRemotes[returnBoxIDByRemote(remote)].timer_full = remoteTimer;

			RemoteCommandExtended remoteCommandCustom = boxesAndRemotes[returnBoxIDByRemote(remote)].remotes[returnRemoteIDByRemote(remote)].remote_commands[commandID];

			IthoSender._send(String("web"), returnCustomRemoteId(remote), remoteCommandCustom);

			String newOldState = String(boxesAndRemotes[returnBoxIDByRemote(remote)].oldstate);
			String newState = String(boxesAndRemotes[returnBoxIDByRemote(remote)].state);

			if (configLog)
			{
				if (serialLog)
				{
					Serial.println("State is going from " + oldValue + " -> " + value);
				}
				mqttClient.publish(mqttLogTopic, String("State is going from " + oldValue + " -> " + value).c_str());
			}

			mqttClient.publish(mqttTopics[boxId].State, value.c_str());
			mqttClient.publish(mqttTopics[boxId].Timer, String(remoteTimer).c_str());
			mqttClient.publish(mqttTopics[boxId].LastIDindex, String(lastIDindex).c_str());
			mqttClient.publish(mqttStatusTopic, "online", true);

			if (!redirect.equalsIgnoreCase(F("button")))
			{
				httpServer.send(200, "text/html", "Command '" + value + "' sent to box #" + String(boxId + 1) + " (" + boxName + ") with remote #" + remoteId + " (" + remote + "). State is going from " + newOldState + " -> " + newState);
			}
		}
		else
		{
			httpServer.send(200, "text/html", "Command not valid");
		}
	}
	else
	{
		if ((configLog) && (serialLog))
		{
			Serial.println("ERROR: Unknown remote, values or action.");
		}
	}

	if (redirect.equalsIgnoreCase(F("button")))
	{
		httpServer.sendHeader("Location", "/");
		httpServer.send(303);
	}
}

void processReceivedCommand(const String &m)
{
	if (configLog)
	{
		if (serialLog)
		{
			Serial.println(m);
		}
		mqttClient.publish(mqttLogTopic, String(m).c_str());
	}

	if (m.startsWith("send/remote/"))
	{
		const String &receivedRemote = returnSubString(m, '/', 2);
		const String &receivedCommand = returnSubString(m, '/', 3);

		if (configLog)
		{
			if (serialLog)
			{
				Serial.println(String("Copy this command --> " + receivedCommand));
				Serial.println(String("Copy this remote  --> " + receivedRemote));
			}
			mqttClient.publish(mqttLogTopic, String("Copy this command --> " + receivedCommand).c_str());
			mqttClient.publish(mqttLogTopic, String("Copy this remote  --> " + receivedRemote).c_str());
		}
		int boxId = returnBoxIDByRemote(receivedRemote);

		if (boxId > -1)
		{
			String boxName = boxesAndRemotes[boxId].name;
			String commandName = returnCommandNameByBoxAndCommand(boxId, receivedCommand);
			String oldValue = String(boxesAndRemotes[boxId].state);
			int commandTimer = returnTimerByRemoteAndCommand(receivedRemote, commandName) * 60;

			if (configLog)
			{
				Serial.println(String("Received message for Box #" + String(boxId + 1) + " (" + boxName + ") with command '" + commandName + "' (" + receivedCommand + ") and timer " + commandTimer));
				mqttClient.publish(mqttLogTopic, String("Received message for Box #" + String(boxId + 1) + " (" + boxName + ") with command '" + commandName + "' (" + receivedCommand + ") and timer " + commandTimer).c_str());
			}
			strcpy(boxesAndRemotes[boxId].oldstate, oldValue.c_str());
			strcpy(boxesAndRemotes[boxId].state, commandName.c_str());

			boxesAndRemotes[boxId].timer = commandTimer;
			boxesAndRemotes[boxId].timer_full = commandTimer;

			if (configLog)
			{
				if (serialLog)
				{
					Serial.println("State is going from " + oldValue + " -> " + commandName);
				}
				mqttClient.publish(mqttLogTopic, String("State is going from " + oldValue + " -> " + commandName).c_str());
			}
			mqttClient.publish(mqttTopics[boxId].State, commandName.c_str());
			mqttClient.publish(mqttTopics[boxId].Timer, String(commandTimer).c_str());
			mqttClient.publish(mqttTopics[boxId].LastIDindex, String("0").c_str());
			mqttClient.publish(mqttStatusTopic, "online", true);
		}
		else
		{
			if (configLog)
			{
				if (serialLog)
				{
					Serial.println("This remote is not yet configured");
				}
				mqttClient.publish(mqttLogTopic, String("This remote is not yet configured").c_str());
			}
		}
	}
}

void mqttCallback(char *topic, byte *payload, unsigned int length)
{
	payload[length] = '\0';
	String strPayload = String((char *)payload);

	String strBoxId = returnSubString(String(topic), '/', 1);
	String strCommand = "/" + returnSubString(String(topic), '/', 3);

	int boxId = returnBoxIDByName(strBoxId);

	if ((boxId > -1) && (strCommand == mqttCommandTopic))
	{
		String remote = boxesAndRemotes[boxId].remotes[0].remote_id;
		String value = strPayload;
		String boxName = boxesAndRemotes[boxId].name;
		String remoteId = String(returnRemoteIDByRemote(remote) + 1);
		String oldValue = boxesAndRemotes[boxId].state;

		int lastIDindex = 0; // 0 means: it is sent by Mqtt-command (not via a real remote)
		int remoteTimer = returnTimerByRemoteAndCommand(remote, value) * 60;
		int commandID = returnCommandID(remote, value);

		if (commandID > -1)
		{

			if (configLog)
			{
				if (serialLog)
				{
					Serial.println(String("Command '" + value + "' sent to box #" + boxId + " (" + boxName + ") with remote #" + remoteId + " (" + remote + ")."));
				}
				mqttClient.publish(mqttLogTopic, String("Command '" + value + "' sent to box #" + boxId + " (" + boxName + ") with remote #" + remoteId + " (" + remote + ").").c_str());
			}

			strcpy(boxesAndRemotes[returnBoxIDByRemote(remote)].oldstate, oldValue.c_str());
			strcpy(boxesAndRemotes[returnBoxIDByRemote(remote)].state, value.c_str());
			boxesAndRemotes[returnBoxIDByRemote(remote)].timer = remoteTimer;
			boxesAndRemotes[returnBoxIDByRemote(remote)].timer_full = remoteTimer;

			if (configLog)
			{
				String newOldState = String(boxesAndRemotes[returnBoxIDByRemote(remote)].oldstate);
				String newState = String(boxesAndRemotes[returnBoxIDByRemote(remote)].state);
				if (serialLog)
				{
					Serial.println("State is going from " + newOldState + " -> " + newState);
				}
				mqttClient.publish(mqttLogTopic, String("State is going from " + newOldState + " -> " + newState).c_str());
			}

			RemoteCommandExtended remoteCommandCustom = boxesAndRemotes[returnBoxIDByRemote(remote)].remotes[returnRemoteIDByRemote(remote)].remote_commands[commandID];

			IthoSender._send(String("mqtt"), returnCustomRemoteId(remote), remoteCommandCustom);

			mqttClient.publish(mqttTopics[boxId].State, value.c_str());
			mqttClient.publish(mqttTopics[boxId].Timer, String(remoteTimer).c_str());
			mqttClient.publish(mqttTopics[boxId].LastIDindex, String(lastIDindex).c_str());
			mqttClient.publish(mqttStatusTopic, "online", true);
		}
	}
}

void mqttReconnect()
{

	while (mqttClient.connected() == 0)
	{
		if (mqttClient.connect(mqttClientId, mqttUsername, mqttPassword, mqttStatusTopic, mqttWillQoS, mqttWillRetain, mqttWillMessage))
		{

			for (int i = 0; i < numberOfIthoBoxes; i++)
			{
				mqttClient.subscribe(mqttTopics[i].Cmd);
			}

			mqttClient.publish(mqttStatusTopic, "online", true);
		}
		else
		{
			delay(4000);
		}
	}
}

void setup(void)
{
	if (serialLog)
	{
		Serial.begin(115200);
	}

	WiFi.mode(WIFI_OFF);

	WiFi.persistent(false);
	WiFi.setAutoReconnect(true);
	WiFi.begin(ssid, password);

	while (WiFi.waitForConnectResult() != WL_CONNECTED)
	{
		WiFi.begin(ssid, password);
	}
	MDNS.begin(host);

	httpUpdater.setup(&httpServer);
	httpServer.begin();
	httpServer.on("/", showHomepage);
	httpServer.on("/api", processAPI);

	MDNS.addService("http", "tcp", 80);

	IthoReceive.setInterruptPin(IRQ_pin);
	IthoReceive.printAllPacket = logAllPacket;
	IthoReceive.printNonRemote = logNonRemote;
	IthoReceive.logger(processReceivedCommand);
	IthoSender.logger(processReceivedCommand);
	IthoReceive.setup();

	mqttClient.setServer(mqttServer, 1883);
	mqttClient.setCallback(mqttCallback);
	dht.begin();

	delay(1500); //allow hardware to chill and relax a bit

	for (int i = 0; i < numberOfIthoBoxes; i++)
	{
		// create list with mqtt topics
		char timerTopic[255];
		char stateTopic[255];
		char cmdTopic[255];
		char lastidindexTopic[255];

		strcpy(stateTopic, mqttMainTopic);
		strcat(stateTopic, boxesAndRemotes[i].name);
		strcat(stateTopic, mqttSubTopic);

		strcpy(timerTopic, stateTopic);
		strcpy(cmdTopic, stateTopic);
		strcpy(lastidindexTopic, stateTopic);

		strcat(timerTopic, "/Timer");
		strcat(stateTopic, "/State");
		strcat(cmdTopic, "/Cmd");
		strcat(lastidindexTopic, "/LastIDindex");

		strcpy(mqttTopics[i].Timer, timerTopic);
		strcpy(mqttTopics[i].State, stateTopic);
		strcpy(mqttTopics[i].Cmd, cmdTopic);
		strcpy(mqttTopics[i].LastIDindex, lastidindexTopic);
	}
}

void loop(void)
{

	httpServer.handleClient();
	MDNS.update();

	if (loopLoop)
	{
		IthoReceive.loop();
		//delay(0);

		if (WiFi.status() == WL_CONNECTED)
		{

			if (millis() - lastRefreshTime >= 1000)
			{
				lastRefreshTime += 1000;

				if (!mqttClient.connected())
				{
					mqttReconnect();
				}

				//delay(0);

				for (int i = 0; i < numberOfIthoBoxes; i++)
				{
					if (boxesAndRemotes[i].timer > 0)
					{
						boxesAndRemotes[i].timer--;

						char timerTimer[5];
						itoa(boxesAndRemotes[i].timer, timerTimer, 10);

						mqttClient.publish(mqttTopics[i].Timer, timerTimer);
						mqttClient.publish(mqttStatusTopic, "online", true);

						if (boxesAndRemotes[i].timer < 2)
						{
							// if a timer was set refer back to setting of first command setting
							mqttClient.publish(mqttTopics[i].State, boxesAndRemotes[i].remotes[0].remote_commands[0].name);
						}
					}
				}
			}

			if ((millis() - lastRefreshTimeDHT >= 5000) && (dhtEnabled == 1))
			{
				lastRefreshTimeDHT += 5000;
				readDHT();
			}

			if (firstRun)
			{
				for (int i = 0; i < numberOfIthoBoxes; i++)
				{
					char timerTimer[5];
					itoa(boxesAndRemotes[i].timer, timerTimer, 10);
					mqttClient.publish(mqttTopics[i].Timer, timerTimer);
					mqttClient.publish(mqttTopics[i].State, boxesAndRemotes[i].state);
					mqttClient.publish(mqttStatusTopic, "online", true);
					firstRun = false;
				}
			}
		}
		mqttClient.loop();
	}
}
