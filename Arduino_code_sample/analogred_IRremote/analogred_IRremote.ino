
//G030k8Tx
#include <Adafruit_NeoPixel.h>

#include <HardwareSerial.h>
HardwareSerial Serial1(PA10, PA9);
#define DECODE_NEC
#include "PinDefinitionsAndMore.h"
#include <IRremote.hpp> // include the library


#include "STM32TimerInterrupt.h"
STM32Timer analogRead_timer(TIM14); //

#define led_right PB7 //ال ای دی راست
#define led_left PB4  //ال ای دی 

#define num_led 4     // number of leds
const int num_yarn = 2;
int rec_sens[num_yarn] = {PA5, PA7};
int IRreceive = 0;
int IRreceivePre;
#define output PB6   //Out Put
#define in_key PB8  // Input Key (input is not used in master sensor)
//#define remote PB9 (is defined in PinDefinitionsAndMore.h)
#define led_regl PB3

Adafruit_NeoPixel strip1 = Adafruit_NeoPixel(num_led, led_right, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel strip2 = Adafruit_NeoPixel(num_led, led_left, NEO_GRB + NEO_KHZ800);

bool input = 1;
bool boolInd;  // to manage blinking in start_led
bool outCircuit;  // once is set to 1, the sensor output is low
bool outHigh = 0;  // outHigh =1 means out put is high
int pink;   // is used to mange pink blinking (see start_led)
//
unsigned long axtim;

int check_cnt;
int analogRead_tim = 600L; // microsrcond: read timer
int res = 12;
int int_calib = 600, calib_tim = 5000;  //  Interval to check yarn
unsigned long axtim_yarn_state, axtim_calib;
unsigned long tim_now, calib_end;   //Present time (msec)

const bool out_command = 1;  // خروجی 24 ولت است

int ledstate, ledstate_pre; // 0: led off, 1: dirt1, 2:dirt2, 3:flaw, 4:torn, 5:outCircuit

int stat = 0;
const int num_ledstate = 6;
bool torn_stat;
// Define class yarnStates
class yarnStates {
  public:
    bool torn ;
    bool dirt1 ;
    bool dirt2 ;
    bool flaw ;
    int per, count_total, count_yarn, maxper, yarn_chance;
    int maxim;
    int minim = pow(2, res) - 1;
    int read_sens;
    int threshold, thresh;

    void calibration(int);
    void resetCalibration(void);
    void read_thresh_perturb(int);
    void yarn_state(void);
};

class ledStates {
  public:
    int led_intrv, command_delay;
    int ind;    // define index of led
    String direc; //"backw", "forw"  direction of led getting on
    unsigned long axtim, axtim_command_delay;
    int red, green, blue;
    ledStates (void) {
      direc = "backw";
    }
    void led(int) ;
    void stateChange(void) ;
    void start_led(int) ;
    void setup_param(int, byte, byte, byte, int);//  blink_interval, red, green, blue, command_delay
};
//Define object from the class containing led state which depends on yarn states: torn , dirt1, dirt2
yarnStates yarn[num_yarn];
ledStates led_samp[num_ledstate];

//Define functons
void rainbow(uint8_t);
void perturb_func(void);
void checkState(void);
void ledstate_define(void);

unsigned long lastPressTime = 0; // Variable to track the last press time (debounce)
const unsigned long debounceDelay = 100; // Debounce time in milliseconds
void irCheck_func() {
  if (IrReceiver.decode()) {
    IrReceiver.resume();

    // Get the current time
    unsigned long currentTime = millis();
    // Check if enough time has passed since the last press
    if (currentTime - lastPressTime > debounceDelay) {
      if (IrReceiver.decodedIRData.command == 18) {  // the key to change in circuit mode to outcircuit and vice versa
        IRreceive = 1;
        if ( IRreceive != IRreceivePre)
          outCircuit = !outCircuit;

      }
      else
        IRreceive = 0;  // if some junk value is received by pressing the keys, set IRreceive to 0
    }

    // Update the last press time
    lastPressTime = currentTime;
  }
  else
    IRreceive = 0;

  IRreceivePre = IRreceive;
  IrReceiver.decodedIRData.command = 0;
}

void analogRead_func() {  //sensors are read in case of no IR receive
  if (IRreceive == 0) {  // main computation stops in case of IR receive for stability
    perturb_func();
    check_cnt++;
  }
  if (check_cnt == 840) {  // gather data for 840 *.6ms=504ms then decide the state
    check_cnt = 0;
    if (IRreceive == 0)
      checkState();
    ledstate_define();
  }
}
void perturb_func() {
  for (int i = 0; i < num_yarn; i++)
    yarn[i].read_thresh_perturb(i); // Analoog read, Computing threshold and perturbation: difference between min and max

}
void checkState() {
  for (int i = 0 ; i < num_yarn; i++)
    yarn[i].yarn_state();
}
void ledstate_define() {
  // Define led state :0, 1, 2, 3, 4, 5
  ledstate_pre = ledstate;

  torn_stat = 1;
  for (int i = 0 ; i < num_yarn; i++)
    torn_stat = torn_stat && (yarn[i].torn == 1 && yarn[i].flaw == 0);

  if (torn_stat == 0)
    ledstate = 0;

  for (int i = 0 ; i < num_yarn; i++) {
    if (yarn[i].dirt1 == 1 && torn_stat == 0)
      ledstate = 1;
  }
  //--

  if (torn_stat == 1 )
    ledstate = 4;

  //--
  if  (outCircuit == 1 )
    ledstate = 5;

  for (int i = 0 ; i < num_yarn; i++) {
    if (yarn[i].dirt2 == 1)
      ledstate = 2;
  }
  //--
  for (int i = 0 ; i < num_yarn; i++) {
    if (yarn[i].flaw == 1 && torn_stat == 0)
      ledstate = 3;
  }

  if ( input == 0 || ledstate == 4 )
    outHigh = 1;
  else
    outHigh = 0;

}

void setup() {
  Serial1.begin(115200);
  strip1.begin();
  strip1.show(); // Initialize all pixels to 'off'
  strip2.begin();
  strip2.show(); // Initialize all pixels to 'off'
  strip1.clear();
  strip2.clear();
  strip1.setBrightness(200);
  strip2.setBrightness(200);

  pinMode(output, OUTPUT);  //خروجی مثبت 24 ولت در پارگی
  pinMode(led_regl, OUTPUT);   // خروجی برای  روشن کردن ال ای دی
  pinMode(IR_RECEIVE_PIN, INPUT);// Configure the IR pin as input
  pinMode(in_key, INPUT);

  digitalWrite(output, 0);
  // defining priority is vital since IR receive library use the same pin that is connected to receive sensor
  // the first priority goes to IR receive so no sensor analog read when IR remote is activated
  NVIC_SetPriority(EXTI4_15_IRQn, 0); // Priority 0: External interrupt for IR
  NVIC_SetPriority(TIM3_IRQn, 1);
  NVIC_SetPriority(TIM14_IRQn, 2); // Lowest priority: Analog Read
  analogReadResolution(res);
  analogWriteResolution(res);

  led_samp[0].setup_param(0, 0, 0, 0, 0); //led off
  led_samp[1].setup_param(50, 249, 114, 111, 2000);// dirt state 1 /pink (blink)
  led_samp[2].setup_param(200, 250, 0, 0, 1000); //dirt state 2     /red (blink)
  led_samp[3].setup_param(300, 0, 0, 250, 1000); //flaw mode           /blue (blink)
  led_samp[4].setup_param(110, 0, 230, 80, 0); // torn mode         /cyan (back and forth)
  led_samp[5].setup_param(100, 0, 250, 0 , 0); // outCircuit mode   /green (blink)

  //Launching Sensor
  digitalWrite (led_regl, HIGH);
  delay(20);
  for (int i = 0; i <= num_led - 1; i++)
    rainbow(3);

  strip1.clear();
  strip2.clear();
  strip1.show();
  strip2.show();
  digitalWrite (led_regl, HIGH);
  delay(20);
  
  //  Calibration
  tim_now = millis();
  calib_end = tim_now + calib_tim;
  while (tim_now <= calib_end) {
    for (int i = 0 ; i < num_yarn; i++)
      yarn[i].calibration(i);

    if (millis() - axtim_calib > int_calib) {
      axtim_calib = millis();
      for (int i = 0 ; i < num_yarn; i++)
        yarn[i].resetCalibration();
    }
    tim_now = millis();

//    for (int i = 0 ; i < num_yarn; i++) {
//      Serial1.print(yarn[i].thresh);
//      Serial1.print(",");
//    }
//    Serial1.println();
  }

  //------------
  // Calibration end alarm


  digitalWrite (led_regl, HIGH);
  delay(20);
  int n = 0;
  for (int i = 0; i < 512; i++) {
    if (i >= 256) {
      strip1.setBrightness(i - n - 1);
      strip2.setBrightness(i - n - 1);
      n += 2;
    }
    else {
      strip1.setBrightness(i);
      strip2.setBrightness(i);
    }

    delay(3);
    for (int j = num_led - 1; j >= 0; j--) {
      strip1.setPixelColor(j, strip1.Color(200, 200 , 0));
      strip2.setPixelColor(j, strip2.Color(200, 200 , 0));
      strip1.show();
      strip2.show();
    }
  }
  strip1.setBrightness(100);
  strip2.setBrightness(100);
  digitalWrite (led_regl, LOW);

  analogRead_timer.attachInterruptInterval(analogRead_tim, analogRead_func);
  attachInterrupt(digitalPinToInterrupt(IR_RECEIVE_PIN), irCheck_func, FALLING); //IR is chekhed only if a falling edge happens

  IrReceiver.begin(IR_RECEIVE_PIN, ENABLE_LED_FEEDBACK);
}


//
void loop() {


//  for (int i = 0; i < num_yarn; i++) {
    Serial1.print(yarn[1].read_sens);
//    Serial1.print(", ");
//    Serial1.print(yarn[i].per);
//    Serial1.print(", ");
//    Serial1.print(yarn[i].threshold);
    //    Serial1.print(", ");
    //    Serial1.print(yarn[i].yarn_chance);
    //    Serial1.print(" ,");
    //    Serial1.print(yarn[i].dirt1);
    //    Serial1.print("     ");
    //    Serial1.print(yarn[i].dirt2);
//    Serial1.print("     ");
//  }
  Serial1.println( );
  input = digitalRead(in_key);
  tim_now = millis();

  //i stateChange some aux values are reset
  if (ledstate_pre != ledstate)
    led_samp[ledstate].stateChange();


  //_______________________LED commands

  led_samp[ledstate].start_led(ledstate);



  //OutPut Command
  // 0: led off, 1: dirt1, 2:dirt2, 3:flaw, 4:torn, 5:outCircuit
  if  (outHigh == 1  ) {
    pinMode(output, OUTPUT);
    digitalWrite(output,  0);
  }
  else {
    pinMode(output, OUTPUT);
    digitalWrite(output, 1);
  }
}

void rainbow(uint8_t wait) {
  uint16_t i, j;

  for (j = 0; j < 256; j++) {
    for (i = 0; i < strip1.numPixels(); i++) {
      strip1.setPixelColor(i, Wheel((i + j) & 255));
      strip2.setPixelColor(i, Wheel((i + j) & 255));
    }
    strip1.show();
    strip2.show();
    delay(wait);
  }
}
uint32_t Wheel(byte WheelPos) {
  if (WheelPos < 85) {
    return strip1.Color(WheelPos * 3, 255 - WheelPos * 3, 0);
    return strip2.Color(WheelPos * 3, 255 - WheelPos * 3, 0);
  } else if (WheelPos < 170) {
    WheelPos -= 85;
    return strip1.Color(255 - WheelPos * 3, 0, WheelPos * 3);
    return strip2.Color(255 - WheelPos * 3, 0, WheelPos * 3);
  } else {
    WheelPos -= 170;
    return strip1.Color(0, WheelPos * 3, 255 - WheelPos * 3);
    return strip2.Color(0, WheelPos * 3, 255 - WheelPos * 3);
  }
}
