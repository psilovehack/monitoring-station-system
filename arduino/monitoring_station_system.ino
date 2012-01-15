#include <EEPROM.h>
#include <DHT.h>
#include <SD.h>
#include <DS1302.h>
#include <Config.h>

//reset funcion
void (*reset) (void) = 0;


//######### pin declarations ######


//activity sensor
const int ACTPIN = 24;


//dth temp and humidity sensor
const int DHTPIN = 22;


//onboard led pin
const int LOADPIN = 13;


//RTC spi pins
int CE_PIN   = 6;
int IO_PIN   = 4;
int SCLK_PIN = 3;


//######### pin declarations ######


//libs declarations
DHT dht(DHTPIN, DHT11);
DS1302 rtc(CE_PIN, IO_PIN, SCLK_PIN);


//SD card lib
File file;
Time t;


//######## action codes #######


//system acts
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


//others actions
const int LOG = 210;


//####### end actions declarations #########


// ##### actions properties #######


//flag to notify activity (or not) via serial
int IS_NOTIFY_ACTIVITY = 0;


// ##### end actions properties #######


//###### others programs consts ########


#define ACTION_VALUE_SEPARATOR '='
#define END_LINE_CHAR ';'


//###### end others programs consts ########

//interval for verify activity sensor. incremented by 5000 in function
long ACT_INTERVAL_THREAD = millis();

long LOG_INTERVAL_THREAD = millis();

//local vars declarations
int ACTIVITY_DETECTED = 0;
const int EEPROM_SIZE = 4096;


void setup() {
  load();

  pinMode(ACTPIN, INPUT);
  pinMode(LOADPIN, OUTPUT);
  pinMode(53, OUTPUT);

  Serial.begin(9600);
  dht.begin();

  if (!SD.begin(5)) {
    Serial.println("sd card failed!");
    return;
  }

  delay(1000);

  Serial.println("Started");
  unload();
}

void loop() {

  action();
  verifyActivity();
  logging(0);


}//end loop

/*
verify activity wit 5 secs interval
 */
void verifyActivity(){
  //verify activity
  if (digitalRead(ACTPIN) && !ACTIVITY_DETECTED && ACT_INTERVAL_THREAD < millis()) {
    load();
    ACTIVITY_DETECTED = 1;
    ACT_INTERVAL_THREAD = millis() + 5000;
    logging(1);
    if (IS_NOTIFY_ACTIVITY)
      printResult(READ_ACTIVITY, readActivity());

    unload();
  }
}

void logging(int force) {

  if (LOG_INTERVAL_THREAD < millis() || force) {

    String out = getTime() + " H: " + readHumidity() + " T: " + readTemp() + " A: " + readActivity();

    Serial.println(out);

    log(out);
    LOG_INTERVAL_THREAD += 30000;
  }

}

/**
 * Return a string in bazillian format
 */
String getTime() {

  t = rtc.time();

  char out[20];
  snprintf(out, 20, "%02d/%02d/%04d %02d:%02d:%02d",
  t.date, t.mon, t.yr, t.hr, t.min, t.sec);

  return String(out);
}

/**
 * Returns a string representation of date (dd-MM-yy). Used for logging
 */
String getDate() {

  t = rtc.time();

  char out[20];
  snprintf(out, 20, "%02d%02d%d", t.date, t.mon, t.yr);

  return String(out);
}

/*
Function to read serial data.
 is recomended to use "if(Serial.available())" before call this funcion
 */
String readSerial() {

  String recData;

  while (Serial.available()) {
    recData += (char)Serial.read();
    delay(5);
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
    printResult(UP_TIME, millis()/1000);
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
  load();
  int byteCounter = 0; // initialize a counter
  byte *byteArray; // create a pointer to a byte array


    // use the malloc function to repeatedly attempt allocating a certain number of bytes to memory
  while ( (byteArray = (byte*) malloc(byteCounter * sizeof(byte))) != NULL) {
    byteCounter++; // if allocation was successful, then up the count for the next try
    free(byteArray); // free memory after allocating it
  }


  free(byteArray); // also free memory after the function finishes
  unload();
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
  load();
  int t = dht.readTemperature();
  unload();
  return t;


}


/*
read humidity
 */
int readHumidity() {
  load();
  int h = dht.readHumidity();
  unload();
  return h;
}

/*
Function to log data
 */
void log(String data) {
  load();
  
  String date = "";
  date += getDate();
  date += ".log";
  
  char fileName[15];
  date.toCharArray(fileName, 15);

  file = SD.open(fileName, FILE_WRITE);
  if (!file) {
    print("SD card file read error");
  } else {
    file.println(data);
    file.close();
  }

  unload();
}

/*
Prints a value in main serial port
 */
void print(String data) {
  load();
  Serial.println(data);
  unload();
}

void printResult(int actId, String value) {
  print(actId + ACTION_VALUE_SEPARATOR + value);
}


void printResult(int actId, int value) {
  print(actId + (String)ACTION_VALUE_SEPARATOR + value);
}

/*
Turn on the load led
 */
void load() {
  loading(1);
}

/*
Turn off the load led
 */
void unload() {
  loading(0);
}

/*
Turn on or off the load led
 */
void loading(int on) {
  digitalWrite(LOADPIN, on);
}
