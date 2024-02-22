#include <WiFi.h>
#include <Servo.h>

//Signal pins for DRS
//Pin 2 on the connector is the left signal
#define DRS_Right_Signal_Pin 18
#define DRS_Left_Signal_Pin 21

//Define standard DRS mode rotations
#define DRS_Right_Mode1_Value 50
#define DRS_Left_Mode1_Value 130

#define DRS_Right_Mode2_Value 110
#define DRS_Left_Mode2_Value 70


//Linear potentiometer pins
#define LinPotPin_FR 32
#define LinPotPin_FL 33
#define LinPotPin_RR 25
#define LinPotPin_RL 26

//int FR_PotVal = 0;
//int FL_PotVal = 0;
//int RR_PotVal = 0;
//int RL_PotVal = 0;

//Local access point credentials
const char* ssid     = "DylanMoment";
const char* password = "DylanBad";

//Set web server port number to 80
WiFiServer server(80);

//Variable to store the HTTP request
String header;

//Store the DRS rotation
uint8_t DRS_Right_Current_Rotation = 0;
uint8_t DRS_Left_Current_Rotation = 0;
String TempVal;

//Servo variables
Servo ServoRight;
Servo ServoLeft;

//Task handles to allow for multi core support
TaskHandle_t Task1;
TaskHandle_t Task2;


void setup()
{
  Serial.begin(115200);

  //Set up DRS servos
  ServoRight.attach(DRS_Right_Signal_Pin);
  ServoLeft.attach(DRS_Left_Signal_Pin);

  pinMode(LinPotPin_FR, INPUT);
  pinMode(LinPotPin_FL, INPUT);
  pinMode(LinPotPin_RR, INPUT);
  pinMode(LinPotPin_RL, INPUT);

  // Connect to Wi-Fi network with SSID and password
  //Serial.print("Setting AP (Access Point)…");

  //Set up and start the access point
  WiFi.softAP(ssid, password);

  IPAddress IP = WiFi.softAPIP();
  //Serial.print("AP IP address: ");
  //Serial.println(IP);
  
  server.begin();

  delay(500);



  //create a task that will be executed in the Task1code() function, with priority 1 and executed on core 0
  xTaskCreatePinnedToCore(
                    Task1code,   /* Task function. */
                    "Task1",     /* name of task. */
                    10000,       /* Stack size of task */
                    NULL,        /* parameter of the task */
                    1,           /* priority of the task */
                    &Task1,      /* Task handle to keep track of created task */
                    0);          /* pin task to core 0 */                  
  delay(500); 

  //create a task that will be executed in the Task2code() function, with priority 1 and executed on core 1
  xTaskCreatePinnedToCore(
                    Task2code,   /* Task function. */
                    "Task2",     /* name of task. */
                    10000,       /* Stack size of task */
                    NULL,        /* parameter of the task */
                    1,           /* priority of the task */
                    &Task2,      /* Task handle to keep track of created task */
                    1);          /* pin task to core 1 */
    delay(500); 
}

void Task1code( void * pvParameters ){
//  Serial.print("Task1 running on core ");
//  Serial.println(xPortGetCoreID());

  for(;;){
//    FR_PotVal = analogRead(LinPotPin_FR);
//    FL_PotVal = analogRead(LinPotPin_FL);
//    RR_PotVal = analogRead(LinPotPin_RR);
//    RL_PotVal = analogRead(LinPotPin_RL);
//
//    Serial.print(",");
//    Serial.print(FR_PotVal);
//    Serial.print(",");
//    Serial.print(FL_PotVal);
//    Serial.print(",");
//    Serial.print(RR_PotVal);
//    Serial.print(",");
//    Serial.println(RL_PotVal);

    

    Serial.print(",");
    Serial.print(analogRead(LinPotPin_FR));
    Serial.print(",");
    Serial.print(analogRead(LinPotPin_FL));
    Serial.print(",");
    Serial.print(analogRead(LinPotPin_RR));
    Serial.print(",");
    Serial.println(analogRead(LinPotPin_RL));

    delay(1);
  } 
}

void Task2code( void * pvParameters ){
//  Serial.print("Task2 running on core ");
//  Serial.println(xPortGetCoreID());

  for(;;){
    //Listen for incomming clients
  WiFiClient client = server.available();

  if (client) {                             // If a new client connects,
    //Serial.println("New Client.");          // print a message out in the serial port
    String currentLine = "";                // make a String to hold incoming data from the client
    while(client.connected()) {            // loop while the client's connected
      if (client.available()) {             // if there's bytes to read from the client,
        char c = client.read();             // read a byte, then
        //Serial.write(c);                    // print it out the serial monitor
        header += c;
        if (c == '\n') {                    // if the byte is a newline character
          // if the current line is blank, you got two newline characters in a row.
          // that's the end of the client HTTP request, so send a response:
          if (currentLine.length() == 0) {
            // HTTP headers always start with a response code (e.g. HTTP/1.1 200 OK)
            // and a content-type so the client knows what's coming, then a blank line:
            client.println("HTTP/1.1 200 OK");
            client.println("Content-type:text/html");
            client.println("Connection: close");
            client.println();
            
            if (header.indexOf("GET /DRSMode1") >= 0)
            {
              //Serial.println("Set DRS mode 1");
              DRS_Right_Current_Rotation = DRS_Right_Mode1_Value;
              DRS_Left_Current_Rotation = DRS_Left_Mode1_Value;
              ServoRight.write(DRS_Right_Current_Rotation);
              ServoLeft.write(DRS_Left_Current_Rotation);
            }
            else if (header.indexOf("GET /DRSMode2") >= 0)
            {
              //Serial.println("Set DRS mode 2");
              DRS_Right_Current_Rotation = DRS_Right_Mode2_Value;
              DRS_Left_Current_Rotation = DRS_Left_Mode2_Value;
              ServoRight.write(DRS_Right_Current_Rotation);
              ServoLeft.write(DRS_Left_Current_Rotation);
            }
            
            // Display the HTML web page
            client.println("<!DOCTYPE html><html>");
            client.println("<head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">");
            client.println("<link rel=\"icon\" href=\"data:,\">");
            // CSS to style the on/off buttons 
            // Feel free to change the background-color and font-size attributes to fit your preferences
            client.println("<style>html { font-family: Helvetica; display: inline-block; margin: 0px auto; text-align: center;}");
            client.println(".button { background-color: #4CAF50; border: none; color: white; padding: 16px 40px;");
            client.println("text-decoration: none; font-size: 30px; margin: 2px; cursor: pointer;}");
            client.println(".button2 {background-color: #555555;}</style></head>");
            
            //Web Page Heading
            client.println("<body><h1>S&T Wind Tunnel 2023</h1>");
            client.println("<h1></h1>");
            
            //Display the current DRS rotation value
            TempVal = DRS_Right_Current_Rotation;
            client.println("<p>DRS Right - " + TempVal + "</p>");
            TempVal = DRS_Left_Current_Rotation;
            client.println("<p>DRS Left - " + TempVal + "</p>");
            
            //Check if DRS is not in mode 1     
            if((DRS_Right_Current_Rotation != DRS_Right_Mode1_Value) && (DRS_Left_Current_Rotation != DRS_Left_Mode1_Value))
            {
              client.println("<p><a href=\"/DRSMode1\"><button class=\"button\">SET MODE 1</button></a></p>");
            }
            else //DRS is in mode 1
            {
              client.println("<p><a href=\"/DRSMode2\"><button class=\"button button2\">SET MODE 2</button></a></p>");
            }

            
            client.println("</body></html>");
            
            // The HTTP response ends with another blank line
            client.println();
            // Break out of the while loop
            break;
          }
          else
          {                       // if you got a newline, then clear currentLine
            currentLine = "";
          }
        }
        else if (c != '\r')
        {                         // if you got anything else but a carriage return character,
          currentLine += c;       // add it to the end of the currentLine
        }
      }
    }
    
    //Clear the header variable
    header = "";
    
    //Close the connection
    client.stop();
    //Serial.println("Client disconnected.");
    //Serial.println("");
  }
  }
}

void loop()
{
  
}
