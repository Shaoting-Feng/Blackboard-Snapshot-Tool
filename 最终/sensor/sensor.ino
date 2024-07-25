String comdata = "";

//
// Detect colors using TCS230.
//

// Arduino uno pins for control of TCS230
#define TCS320_OE 27
#define TCS320_S0 30
#define TCS320_S1 31
#define TCS320_S2 22
#define TCS320_S3 23
#define TCS320_OUT 24

#define SEL_RED  \
   digitalWrite(TCS320_S2,LOW);digitalWrite(TCS320_S3,LOW)
#define SEL_GREEN \
   digitalWrite(TCS320_S2,HIGH);digitalWrite(TCS320_S3,HIGH)
#define SEL_BLUE \
   digitalWrite(TCS320_S2,LOW);digitalWrite(TCS320_S3,HIGH)
#define SEL_CLEAR \
   digitalWrite(TCS320_S2,HIGH);digitalWrite(TCS320_S3,LOW)

#define TWO_PER \
   digitalWrite(TCS320_S0,LOW);digitalWrite(TCS320_S1,HIGH);

#define debug(a) Serial.println((a));

////////////////////////////////////////////////////////////////
void setup() {

   pinMode(TCS320_OE,OUTPUT);
   pinMode(TCS320_S0,OUTPUT);
   pinMode(TCS320_S1,OUTPUT);
   pinMode(TCS320_S2,OUTPUT);
   pinMode(TCS320_S3,OUTPUT);
   pinMode(TCS320_OUT,INPUT);

   TWO_PER;

   digitalWrite(TCS320_OE,LOW); // On always.

   Serial.begin(115200);
}

////////////////////////////////////////////////////////////////
unsigned long get_TCS230_reading(void) {
  unsigned long val;
  noInterrupts();
  val = pulseIn(TCS320_OUT,HIGH,20000); // 2000us=2ms  2Hz min.
  interrupts();
  return val;
}

static int clr,red,green,blue;
static int preredval = 0, pregreenval = 0, preblueval = 0;

////////////////////////////////////////////////////////////////
uint16_t detect(void) {
    unsigned long val;

    SEL_RED;
    red = val = get_TCS230_reading();
    
    SEL_GREEN;
    green = val = get_TCS230_reading();
    
    SEL_BLUE;
    blue = val = get_TCS230_reading();
}

////////////////////////////////////////////////////////////////
void loop() {
  
      detect();
      if(red + green + blue - preredval - pregreenval - preblueval > 400 || red + green + blue - preredval - pregreenval - preblueval < -400) {
          comdata = "";
          Serial.println("TCS230 color detector has detected a change.");
      }
      preredval = red;
      pregreenval = green;
      preblueval = blue;
      delay(2000);
}
