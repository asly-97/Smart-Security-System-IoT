
class Control{
  public:
    //used to tell whether we must send a photo
    static bool sendPhoto;
    //flashlight's state
    static bool flashState;

    //The @IP of the broker
    static const char * mqtt_server;
};

bool Control::sendPhoto = false;
bool Control::flashState = false;
const char * Control::mqtt_server = "192.168.1.133";
