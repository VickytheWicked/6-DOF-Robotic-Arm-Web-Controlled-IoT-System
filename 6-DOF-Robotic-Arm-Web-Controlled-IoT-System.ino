#include <WiFi.h>
#include <WebServer.h>
#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>

struct Servo
{
  int channel;
  int max;
  int min;
  int currentAngle;
};

const int servo_num = 6;

Servo all_servo[6] = {{0, 525, 110, 90}, {1, 550, 100, 54}, {2, 550, 250, 95}, {3, 420, 100, 90}, {4, 525, 100, 90}, {5, 365, 220,90}};
int targetAngle = -1;
int targetServo = -1;

/* ---------- WiFi credentials ---------- */
const char *ssid = "Lion";
const char *password = "vansh2004";

/* ---------- Web server ---------- */
WebServer server(80);

/* ---------- PCA9685 ---------- */
Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver();

/* ---------- Web page ---------- */
// void handleRoot() {
//   server.send(200, "text/html", R"rawliteral(
// <!DOCTYPE html>
// <html>
// <head>
//   <meta name="viewport" content="width=device-width, initial-scale=1">
// </head>
// <body>
//   <h2>Servo Caliberation</h2>
//   <form onsubmit="sendData(event)">
//   <label>Enter servo number:</label><br>
//   <input type="number" id="servo" min="1" max="6" value="1"
//          oninput="val1.innerText=this.value">
//   <p>Servo num: <span id="val1">1</span></p><br>

//   <label>Enter the tick value:</label><br>
//   <input type="number" id="tick" min="100" max="560" value="307"
//          oninput="val2.innerText=this.value">
//   <p>Tick value: <span id="val2">307</span> ticks</p>

//   <input type="submit" value="Update">
// </form>

// <script>
// function sendData(e) {
//   e.preventDefault(); // stops page reload

//   const servo = document.getElementById("servo").value-1;
//   const tick  = document.getElementById("tick").value;

//   fetch(`/set?servo=${servo}&tick=${tick}`);
// }
// </script>

// </body>
// </html>
// )rawliteral");
// }

void handleRoot()
{
  server.send(200, "text/html", R"rawliteral(
<!DOCTYPE html>
<html>
<head>
  <meta name="viewport" content="width=device-width, initial-scale=1">
  <title>6DOF Robotic Arm Control</title>

  <style>
    body {
      font-family: Arial, sans-serif;
      padding: 10px;
    }
    .servo {
      margin-bottom: 20px;
    }
    label {
      font-weight: bold;
    }
  </style>
</head>

<body>

<h2>6DOF Robotic Arm Control</h2>

<div class="servo">
  <label>Servo 1 (Base): <span id="v0">90</span>°</label><br>
  <input type="range" min="0" max="180" value="94"
         oninput="send(0,this.value); v0.innerText=this.value;">
</div>

<div class="servo">
  <label>Servo 2 (Shoulder): <span id="v1">180</span>°</label><br>
  <input type="range" min="0" max="180" value="54"
         oninput="send(1,this.value); v1.innerText=this.value;">
</div>

<div class="servo">
  <label>Servo 3 (Elbow): <span id="v2">0</span>°</label><br>
  <input type="range" min="0" max="180" value="95"
         oninput="send(2,this.value); v2.innerText=this.value;">
</div>

<div class="servo">
  <label>Servo 4 (Wrist Pitch): <span id="v3">90</span>°</label><br>
  <input type="range" min="0" max="180" value="90"
         oninput="send(3,this.value); v3.innerText=this.value;">
</div>

<div class="servo">
  <label>Servo 5 (Wrist Roll): <span id="v4">90</span>°</label><br>
  <input type="range" min="0" max="180" value="90"
         oninput="send(4,this.value); v4.innerText=this.value;">
</div>

<div class="servo">
  <label>Servo 6 (Gripper): <span id="v5">OPEN</span></label><br>

  <input type="button" value="OPEN"
         onclick="sendGripper(5, 90)">
         
  <input type="button" value="CLOSE"
         onclick="sendGripper(5, 180)">
</div>

<script>
function send(servo, angle) {
  fetch(`/set?servo=${servo}&angle=${angle}`);
}
function sendGripper(servo, angle) {
  document.getElementById("v5").innerText =angle === 90 ? "OPEN" : "CLOSE";
  fetch(`/set?servo=${servo}&angle=${angle}`);
}
</script>

</body>
</html>
)rawliteral");
}

/* ---------- Handler ---------- */
void handleMain()
{
  if (server.hasArg("angle"))
  {
    targetAngle = constrain(server.arg("angle").toInt(), 0, 180);
  }
  if (server.hasArg("servo"))
  {
    targetServo = constrain(server.arg("servo").toInt(), 0, 5);
  }
  server.send(200, "text/plain", "OK");
}



void angleSet()
{
  if (targetServo >= 0)
  {
    Servo &s = all_servo[targetServo];

    int step = 1;
    if (targetServo == 5)
      step = 2; // gripper
    if (targetServo == 4)
      step = 2; // wrist roll
    if (targetServo == 3)
      step = 1; // wrist pitch
    if (targetServo == 2)
      step = 1; // elbow
    if (targetServo == 1)
      step = 1; // shoulder
    if (targetServo == 0)
      step = 1; // base slow

    if (s.currentAngle < targetAngle)
      s.currentAngle += step;
    else if (s.currentAngle > targetAngle)
      s.currentAngle -= step;

    s.currentAngle = constrain(s.currentAngle, 0, 180);

    int safeMin = s.min + 5;
    int safeMax = s.max - 5;

    int pulse = map(s.currentAngle, 0, 180, safeMin, safeMax);
    pwm.setPWM(s.channel, 0, pulse);
  }
    delay(20);


}
/* ---------- Setup ---------- */
void setup()
{
  Serial.begin(115200);

  Wire.begin();
  pwm.begin();
  pwm.setPWMFreq(50); // 50 Hz for servo

  // Move to center safely
  // int centerPulse = map(90, 0, 180, SERVO_MIN, SERVO_MAX);
  
  // WiFi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
  }

  Serial.println(WiFi.localIP());

  for(int i=0;i<servo_num;i++)
  {
    targetAngle=all_servo[i].currentAngle;
    targetServo=i;
    angleSet();
    delay(50);
  }

  // Web routes
  server.on("/", handleRoot);
  server.on("/set", handleMain);
  server.begin();
}

void loop()
{
  // Serial.println(WiFi.localIP());

  server.handleClient();

  Serial.print("TargetAngle: ");
  Serial.println(targetAngle);
  Serial.print("TargetServo: ");
  Serial.println(targetServo);
  Serial.println("");

  Serial.print("Shoulder Angle: ");
  Serial.println(all_servo[1].currentAngle);
  Serial.println("");

  angleSet();

}
