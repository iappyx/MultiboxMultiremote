int returnBoxIDByName(String name)
{
	int returnInt = -1;
	for (int i = 0; i < numberOfIthoBoxes; i++)
	{

		if (name.startsWith((boxesAndRemotes[i].name)))
		{
			returnInt = i;
		}
	}

	return returnInt;
}

inline String returnBoxName(String remote)
{
	String returnString = "";
	for (int i = 0; i < numberOfIthoBoxes; i++)
	{

		for (int ii = 0; ii < boxesAndRemotes[i].numremotes; ii++)
		{
			if (!boxesAndRemotes[i].remotes[ii].remote_id == NULL)
			{

				if (String(boxesAndRemotes[i].remotes[ii].remote_id).equalsIgnoreCase(remote))
				{

					returnString = String(boxesAndRemotes[i].name);
				}
			}
		}
	}

	return returnString;
}

int returnBoxIDByRemote(String remote)
{
	int returnInt = -1;
	for (int i = 0; i < numberOfIthoBoxes; i++)
	{

		for (int ii = 0; ii < boxesAndRemotes[i].numremotes; ii++)
		{
			if (!boxesAndRemotes[i].remotes[ii].remote_id == NULL)
			{

				if (String(boxesAndRemotes[i].remotes[ii].remote_id).equalsIgnoreCase(remote))
				{

					returnInt = i;
					return returnInt;
				}
			}
		}
	}

	return returnInt;
}

int returnRemoteIDByRemote(String remote)
{
	int returnInt = -1;
	for (int i = 0; i < numberOfIthoBoxes; i++)
	{

		for (int ii = 0; ii < boxesAndRemotes[i].numremotes; ii++)
		{
			if (!boxesAndRemotes[i].remotes[ii].remote_id == NULL)
			{

				if (String(boxesAndRemotes[i].remotes[ii].remote_id).equalsIgnoreCase(remote))
				{

					returnInt = ii;
					return returnInt;
				}
			}
		}
	}

	return returnInt;
}

String RemoteCommandsToString(uint8_t *data, unsigned int length, bool ashex = true)
{
	String str = "";
	for (uint8_t i = 0; i < length; i++)
	{
		if (ashex)
		{
			if (data[i] == 0)
			{
				str += "0";
			}
			str += String(data[i], HEX);
		}
		else
		{
			str += String(data[i]);
		}
		if (i < length - 1)
			str += ":";
	}
	return str;
}

String returnCommandNameByBoxAndCommand(int box, String command)
{
	String returnString = "";

	for (int ii = 0; ii < boxesAndRemotes[box].numremotes; ii++)
	{

		for (int iii = 0; iii < maxCommandsPerRemote; iii++)
		{

			uint8_t *tempBytes = boxesAndRemotes[box].remotes[ii].remote_commands[iii].bytes;
			int tempLength = boxesAndRemotes[box].remotes[ii].remote_commands[iii].length;
			String tempString = String(RemoteCommandsToString(tempBytes, tempLength));
			if (tempString.equalsIgnoreCase(command))
			{
				returnString = boxesAndRemotes[box].remotes[ii].remote_commands[iii].name;
			}
		}
	}

	return returnString;
}

int returnTimerByRemoteAndCommand(String remote, String value)
{
	int returnInt = 0;
	for (int i = 0; i < numberOfIthoBoxes; i++)
	{

		for (int ii = 0; ii < boxesAndRemotes[i].numremotes; ii++)
		{
			if (!boxesAndRemotes[i].remotes[ii].remote_id == NULL)
			{

				if (String(boxesAndRemotes[i].remotes[ii].remote_id).equalsIgnoreCase(remote))
				{

					for (int iii = 0; iii < maxCommandsPerRemote; iii++)
					{
						if (String(boxesAndRemotes[i].remotes[ii].remote_commands[iii].name).equalsIgnoreCase(value))
						{
							returnInt = boxesAndRemotes[i].remotes[ii].remote_commands[iii].timer;
						}
					}

					return returnInt;
				}
			}
		}
	}

	return returnInt;
}

int returnCommandID(String remote, String value)
{
	int returnInt = -1;
	for (int i = 0; i < numberOfIthoBoxes; i++)
	{

		for (int ii = 0; ii < boxesAndRemotes[i].numremotes; ii++)
		{
			if (!boxesAndRemotes[i].remotes[ii].remote_id == NULL)
			{

				if (String(boxesAndRemotes[i].remotes[ii].remote_id).equalsIgnoreCase(remote))
				{

					for (int iii = 0; iii < maxCommandsPerRemote; iii++)
					{
						if (String(boxesAndRemotes[i].remotes[ii].remote_commands[iii].name).equalsIgnoreCase(value))
						{
							returnInt = iii;
						}
					}

					return returnInt;
				}
			}
		}
	}

	return returnInt;
}

uint8_t *returnCustomRemoteId(String remoteString)
{

	uint8_t remoteIdCustom[maxBitsRemoteId];

	char str[remoteString.length() + 1];
	remoteString.toCharArray(str, remoteString.length() + 1);
	char *pch;
	int i = 0;
	pch = strtok(str, ":");
	while (pch != NULL)
	{
		remoteIdCustom[i] = strtol(pch, 0, 16);
		pch = strtok(NULL, ":");
		i++;
	}

	return remoteIdCustom;
}

inline String returnSubString(const String &s, char separator, int cnt)
{
	int startIndex = 0;
	for (int i = 0; i < cnt; i++)
	{
		startIndex = s.indexOf(separator, startIndex);
		if (startIndex < 0)
			return "";
		startIndex++;
	}
	int lastIndex = s.indexOf(separator, startIndex);
	if (lastIndex < 0)
	{
		return s.substring(startIndex);
	}
	return s.substring(startIndex, lastIndex);
}
