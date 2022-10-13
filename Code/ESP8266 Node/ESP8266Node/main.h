//LED pin
#define LEDPin D1

//PIR motion sensor pin
#define PIRSensorPin D2

//Buzzer pin
#define BuzzerPin D3

class Control{
  public:
    //system's state
    static bool systemEnabled;
    //alarm's  state
    static bool alarmState;

    //The @IP of the broker
    static const char * mqtt_server;

    //toggles the alarm state
    static void alarm(const bool state){
      Control::alarmState = state;
      if(state){
        //Turn on the red LED
        digitalWrite(LEDPin,HIGH);
        //Turn on the buzzer
        digitalWrite(BuzzerPin,HIGH);
      }
      else{
        //Turn off the red LED
        digitalWrite(LEDPin,LOW);
        //Turn off the buzzer
        digitalWrite(BuzzerPin,LOW);
      }
    }
};

bool Control::alarmState = false;
bool Control::systemEnabled = true;

const char * Control::mqtt_server = "192.168.1.133";
