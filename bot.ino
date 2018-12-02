#include <ESP8266WiFi.h>

#include <WiFiUdp.h>
#include <Servo.h>

WiFiUDP Udp;
Servo myservo;

unsigned int port = 4210;
char incomingPacket[255];
char replyPacket[] = "Message Received";

int left_led = 0;
int right_led = 0;

int En1 = A0;          //L293D EN1 Correspond Development Board R3 A0 pin
int In1 = 16;          //L293D IN1 Correspond Development Board R3 A1 pin
int In2 = 14;          //L293D IN2 Correspond Development Board R3 A2 pin

int In4 = 12;          //L293D IN4 Correspond Development Board R3 A3 pin
int In3 = 13;          //L293D IN3 Correspond Development Board R3 A4 pin
int En2 = 15;          //L293D EN2 Correspond Development Board R3 A5 pin
int servo = 0;
int pos = 0;
const byte h3 = 4;
const byte h4 = 5;

volatile byte state = LOW;

unsigned char pwmval_left  = 0;           //definitions variable
unsigned char pwmval_right = 0;

unsigned char pwmval_left_init  = 10;     //Left motor Duty Cycle Adjust, adjust value between 0 to 20,
unsigned char pwmval_right_init = 10;     //Right motor Duty Cycle Adjust, adjust value between 0 to 20

unsigned char right_pwm = 1;              //Right motor PWM switch, turn on when 1 value
unsigned char left_pwm  = 1;              //left motor PWM switch, turn on when 1 value

const char* ssid = "OnePlus 6T-315c";
const char* password = "saqeeb98";


void forward(void)               //Car forward control function
{
  delay(1);
  Timer();
  pwmval_right = 0;
  pwmval_left  = 0;
  pwmval_left_init  = 10;
  pwmval_right_init = 10;

  digitalWrite(In1, HIGH);
  digitalWrite(In2, LOW);

  digitalWrite(In3, HIGH);
  digitalWrite(In4, LOW);
}

void right(void)                  //Turn right
{
  delay(1);
  Timer();
  pwmval_right = 0;
  pwmval_left  = 0;
  pwmval_left_init  = 10;
  pwmval_right_init = 10;

  digitalWrite(In1, HIGH);
  digitalWrite(In2, LOW);

  digitalWrite(In3, LOW);
  digitalWrite(In4, HIGH);
}

void left(void)                 //Turn left
{
  delay(1);
  Timer();
  pwmval_right = 0;
  pwmval_left  = 0;
  pwmval_left_init  = 10;
  pwmval_right_init = 10;

  digitalWrite(In1, LOW);
  digitalWrite(In2, HIGH);

  digitalWrite(In3, HIGH);
  digitalWrite(In4, LOW);
}

void stop_car(void)             //Stop
{
  digitalWrite(In1, LOW);
  digitalWrite(In2, LOW);

  digitalWrite(In3, LOW);
  digitalWrite(In4, LOW);
}

void back(void)                 //Backwards
{
  delay(1);
  Timer();
  pwmval_right = 0;
  pwmval_left  = 0;
  pwmval_left_init  = 10;
  pwmval_right_init = 10;

  digitalWrite(In1, LOW);
  digitalWrite(In2, HIGH);

  digitalWrite(In3, LOW);
  digitalWrite(In4, HIGH);
}

void left_moto(void)             //left-wheel speed control function
{
  if (left_pwm)
  {
    if (pwmval_left <= pwmval_left_init)
    {
      digitalWrite(En1, HIGH);
    }
    else
    {
      digitalWrite(En1, HIGH);
    }
    if (pwmval_left >= 20)
    {
      pwmval_left = 0;
    }
  }
  else
  {
    digitalWrite(En1, LOW);
  }
}

void right_moto(void)          //right-wheel speed control function
{
  if (right_pwm)
  {
    if (pwmval_right <= pwmval_right_init)
    {
      digitalWrite(En2, HIGH);
    }
    else if (pwmval_right > pwmval_right_init)
    {
      digitalWrite(En2, HIGH);
    }
    if (pwmval_right >= 20)
    {
      pwmval_right = 0;
    }
  }
  else
  {
    digitalWrite(En2, LOW);
  }
}


void Timer(void)       //T2 interrupt Processing function
{
  pwmval_left  = pwmval_left  + 1;
  pwmval_right = pwmval_right + 1;

  left_moto();
  right_moto();
}

void setup() 
{
  pinMode(h3, INPUT_PULLUP);
  pinMode(h4, INPUT_PULLUP);
  pinMode(En1, OUTPUT);  //Set the pin to output
  pinMode(In1, OUTPUT);  //Set the pin to output
  pinMode(In2, OUTPUT);  //Set the pin to output
  pinMode(En2, OUTPUT);  //Set the pin to output
  pinMode(In3, OUTPUT);  //Set the pin to output
  pinMode(In4, OUTPUT);  //Set the pin to output

  Serial.begin(115200);
  delay(10);

  Serial.println();
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) 
  {
    delay(500);
    Serial.print(".");
  }
  
  Serial.println("");
  Serial.println("WiFi connected");
  Udp.begin(port);
  Serial.printf("Now listening at IP %s, UDP port %d\n", WiFi.localIP().toString().c_str(), port);

  myservo.attach(servo);
  myservo.write(pos);
}

void loop() 
{
  int packetSize = Udp.parsePacket();
  if (packetSize)
  {
    Serial.printf("Received %d bytes from %s, port %d\n", packetSize, Udp.remoteIP().toString().c_str(), Udp.remotePort());
    int len = Udp.read(incomingPacket, 255);
    if (len > 0)
    {
      incomingPacket[len] = 0;
    }
    Serial.printf("UDP packet contents: %s\n", incomingPacket);
    if (String(incomingPacket) == "forward") 
    {
      forward();
      Serial.println("forward");
    }
    else if (String(incomingPacket) == "back") 
    {
      back();
      delay(333);
      stop_car();
      Serial.println("backward");
    }
    else if (String(incomingPacket) == "lt") 
    {
      left();
      Serial.println("left");
    }
    else if (String(incomingPacket) == "rt") 
    { 
      right();
      Serial.println("right");
    }
    else if (String(incomingPacket) == "left") 
    {
      left();
      delay(333);
      stop_car();
      Serial.println("left");
    }
    else if (String(incomingPacket) == "right") 
    { 
      right();
      delay(333);
      stop_car();
      Serial.println("right");
    }
    else if (String(incomingPacket) == "stop") 
    {
      stop_car();
      Serial.println("stop");
    }
    else if (String(incomingPacket) == "up")
    {
      for(pos = 0; pos <= 180; pos += 1)
      {
        myservo.write(pos);
        delay(15);
      }
      Serial.println("up");
    }
    else if (String(incomingPacket) == "down")
    {
     for(pos = 180; pos >= 0; pos -= 1)
      {
        myservo.write(pos);
        delay(15);
      }
    Serial.println("down");
    }
    else 
    {
      Serial.println("invalid request");
      return;
    }
    Udp.beginPacket(Udp.remoteIP(), Udp.remotePort());
    Udp.write(replyPacket);
    Udp.endPacket();
  }
  left_led = digitalRead(h3);
  right_led = digitalRead(h4);
  Timer();
  if(left_led == 0 || right_led == 0)
  {
    stop_car();
    delay(500);
  }

}
