#include <DHT.h>


//pin declarations
const int ACTPIN =  50;
const int DHTPIN = 52;

//action codes
const int READ_ACTIVITY = 350;
const int READ_TEMP = 360;
const int READ_HUMIDITY = 370;

const int LOG = 210;

const int FREE_MEMORY = 110;
const int RESET_BOARD = 100;

//others programs consts
const String ACTION_VALUE_SEPARATOR = "=";

//libs declarations
DHT dht(DHTPIN, DHT11);

//reset funcion
void (*reset) (void) = 0;

long ACT_INTERVAL = millis();

//local vars declarations
boolean ACTIVITY_DETECTED = false;


void setup() {

  Serial.begin(9600);
  dht.begin();

  pinMode(ACTPIN, INPUT);
  analogReference(DEFAULT);

  delay(1000);

  Serial.println("Started");

}

void loop() {

  action();
  verifyActivity();

}//end loop

/*
verify activity wit 5 secs interval
 */
void verifyActivity(){
  //verify activity
  if (digitalRead(ACTPIN) && !ACTIVITY_DETECTED && ACT_INTERVAL < millis()) {
    ACTIVITY_DETECTED = true;
    printResult(READ_ACTIVITY, readActivity());
    ACT_INTERVAL = millis() + 5000;
  }
}


/*
Function to read serial data.
 is recomended to use "if(Serial.available())" before call this funcion
 */
String readSerial() {

  String recData;

  while (Serial.available()) {
    recData += (char)Serial.read();
    delay(10);
  }
  return recData; 
}

/*
Verify and execute actions
 Actions code was declared in head of file;
 */
void action() {

  //if serial data available
  if (Serial.available()) {

    String action = readSerial();

    //action without value
    if (action.indexOf(ACTION_VALUE_SEPARATOR) == -1) {
      executeAction(action.toInt());

    } 
    else {
      executeAction(getActionId(action), getActionValue(action));
    }

  }

}

/*
Execute a simple action
 */
void executeAction(int action) {

  switch (action) {

  case READ_ACTIVITY:
    printResult(READ_ACTIVITY, readActivity());
    break;

  case READ_TEMP:
    printResult(READ_TEMP, readTemp());
    break;

  case READ_HUMIDITY:
    printResult(READ_HUMIDITY, readHumidity());
    break;

  case FREE_MEMORY:
    printResult(FREE_MEMORY, memoryTest());
    break;

  case RESET_BOARD:
    reset();
    break;

  }

}

/*
Execute a action with a value
 */
void executeAction(int action, String value){

  switch (action) {

  case LOG:
    log(value);
    break;

  }

}

/*
Return the id of a action with value
 */
int getActionId(String action) {
  return action.substring(0, action.indexOf(ACTION_VALUE_SEPARATOR)).toInt();
}

/*
Return the value of a action
 */
String getActionValue(String action) {
  return action.substring(action.indexOf(ACTION_VALUE_SEPARATOR) + 1);
}

/*
This function will return the number of bytes currently free in RAM
 */
int memoryTest() {
  int byteCounter = 0; // initialize a counter
  byte *byteArray; // create a pointer to a byte array
  // More on pointers here: http://en.wikipedia.org/wiki/Pointer#C_pointers

  // use the malloc function to repeatedly attempt allocating a certain number of bytes to memory
  while ( (byteArray = (byte*) malloc (byteCounter * sizeof(byte))) != NULL ) {
    byteCounter++; // if allocation was successful, then up the count for the next try
    free(byteArray); // free memory after allocating it
  }

  free(byteArray); // also free memory after the function finishes
  return byteCounter; // send back the highest number of bytes successfully allocated
}

/*
Return true if activity was detected
 */
String readActivity() {
  if (ACTIVITY_DETECTED) {
    ACTIVITY_DETECTED = false;
    return "true";
  }
  return "false";
}


/*
Read temperature
 */
int readTemp() {
  return (int)dht.readTemperature();
}

/*
read humidity
 */
int readHumidity() {
  return (int)dht.readHumidity();
}

/*
Function to log data
 */
void log(String data) {

  //por enquanto somente sera printado
  print(data);

}

/*
Prints a value in main serial port
 */
void print(String data) {
  Serial.println(data); 
}

void printResult(int actId, String value) {
  print(actId + ACTION_VALUE_SEPARATOR + value);
}

void printResult(int actId, int value) {
  print(actId + ACTION_VALUE_SEPARATOR + value);
}
