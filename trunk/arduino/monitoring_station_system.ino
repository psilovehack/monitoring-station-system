#include <DHT.h>

//pin declarations
const int ACTPIN = 50;
const int DHTPIN = 52;

//libs declarations
DHT dht(DHTPIN, DHT11);

//######## action codes #######

//hardware acts
const int RESET_BOARD = 100;
const int FREE_MEMORY = 110;
const int UP_TIME = 120;

//activity acts
const int READ_ACTIVITY = 350;
const int ACTIVITY_NOTIFY_CHANGE = 351;
const int ACTIVITY_NOTIFY_STATUS = 352;

//evvironment acts
const int READ_TEMP = 360;
const int READ_HUMIDITY = 370;

const int LOG = 210;

//####### end actions declarations #########

// ##### actions properties #######

//flag to notify activity (or not) via serial 
int IS_NOTIFY_ACTIVITY = 1;

// ##### end actions properties #######

//others programs consts
#define ACTION_VALUE_SEPARATOR '='
#define END_LINE_CHAR ';'

//reset funcion
void (*reset) (void) = 0;

//interval for verify activity sensor. incremented by 5000 in function
long ACT_INTERVAL_THREAD = millis();

//local vars declarations
int ACTIVITY_DETECTED = 0;


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
  if (digitalRead(ACTPIN) && !ACTIVITY_DETECTED && ACT_INTERVAL_THREAD < millis()) {
    ACTIVITY_DETECTED = 1;
    ACT_INTERVAL_THREAD = millis() + 5000;
    if (IS_NOTIFY_ACTIVITY)
      printResult(READ_ACTIVITY, readActivity());
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

  case ACTIVITY_NOTIFY_STATUS:
    printResult(ACTIVITY_NOTIFY_STATUS, IS_NOTIFY_ACTIVITY);
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

  case UP_TIME:
    printResult(UP_TIME, millis()); 
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

  case ACTIVITY_NOTIFY_CHANGE:
    printResult(ACTIVITY_NOTIFY_CHANGE, changeActNotify(value));
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
  while ( (byteArray = (byte*) malloc(byteCounter * sizeof(byte))) != NULL) {
    byteCounter++; // if allocation was successful, then up the count for the next try
    free(byteArray); // free memory after allocating it
  }

  free(byteArray); // also free memory after the function finishes
  return byteCounter; // send back the highest number of bytes successfully allocated
}

/*
Return true if activity was detected
 */
int readActivity() {
  if (ACTIVITY_DETECTED) {
    ACTIVITY_DETECTED = 0;
    return 1;
  }
  return 0;
}

/*
Changes the notify act. action
 */
int changeActNotify(String value) {
  IS_NOTIFY_ACTIVITY = value.toInt();
  return IS_NOTIFY_ACTIVITY;
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
  print(actId + (String)ACTION_VALUE_SEPARATOR + value);
}