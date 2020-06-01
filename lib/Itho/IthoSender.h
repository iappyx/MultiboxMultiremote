#include "IthoCC1101.h"

class RemoteCommandExtended;

class IthoSenderClass
{
  public:
//    void turnOff(const String& sender) { sendCommand(sender, "eco"); }
//    void turnOn(const String& sender) { sendCommand(sender, "cook1"); }

//  void sendCommand(const String& sender, const String &c);
//  void sendCommandRoom(const String &c);
//  void sendCommand(const String& sender, const String &remote, const String& remoteCommand);
//  void remoteId(const uint8_t *);
//  void remoteIdRoom(const uint8_t *);

    void logger(void (*)(const String &));

  private:
    static void _convertToPacket(const ByteArray &, CC1101Packet &);
    void (*_logger)(const String &) = NULL;
    public:
    void _log(const String &s);

    RemoteCommandExtended *_lookupByName(const String &n, const RemoteCommandExtended commands[]);
    void _send(const String& sender, uint8_t id[], RemoteCommandExtended remoteCommand);
    void _send(const String& sender, ByteArray id, ByteArray cc);

    uint8_t _counter = 87;
//  uint8_t _remoteId[3] = {0x52, 0x50, 0xb9};
//  uint8_t _remoteIdRoom[3] = {0x00, 0x00, 0x00}; // We overwrite this later
    uint8_t _remoteByte0 = 0x16;
};

extern IthoSenderClass IthoSender;
