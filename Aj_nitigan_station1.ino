// Station1
#include "sps30.h"
#include "ClosedCube_HDC1080.h"

#include <Arduino.h>
#include <ArduinoJson.h>
#include <Stream.h>
#include <HardwareSerial.h>

#define TX2_pin  17
#define RX2_pin  16
#define DIR_pin  26
#define PWR_pin  27

Stream *_Serial;
HardwareSerial myserial(2);

#define SP30_COMMS SERIALPORT1

#define TX_PIN 13
#define RX_PIN 14

#define DEBUG 0

#define sensor_NO2  35
#define sensor_CO  32
#define sensor_S02  33
#define sensor_O3  25

const int wind_speed_Pin = 36;  // Analog input pin that the potentiometer is attached to
const int wind_dir_Pin = 39;  // Analog input pin that the potentiometer is attached to

ClosedCube_HDC1080 hdc1080;

unsigned long previousMillis = 0;        // will store last time LED was updated

// constants won't change:
const long interval = 30000;           // interval at which to blink (milliseconds)

struct _RES
{
  unsigned char status;
  String data;
  String temp;
};

int cmm_state = 0;
int count = 0;

int wind_speed_adc = 0; 
double wind_speed_voltage = 0;
double wind_speed = 0; //m/s.

int wind_dir_adc = 0; 
String wind_dir = "NA";

// function prototypes (sometimes the pre-processor does not create prototypes themself on ESPxx)
void ErrtoMess(char *mess, uint8_t r);
void Errorloop(char *mess, uint8_t r);
void GetDeviceInfo();
bool read_all();

// create constructor
SPS30 sps30;

void setup() {
  
  Serial.begin(115200);
  
  hdc1080.begin(0x40);

  Serial.println(F("Trying to connect"));

  // set driver debug level
  sps30.EnableDebugging(DEBUG);

  // set pins to use for softserial and Serial1 on ESP32
  if (TX_PIN != 0 && RX_PIN != 0) sps30.SetSerialPin(RX_PIN, TX_PIN);

  // Begin communication channel;
  if (! sps30.begin(SP30_COMMS))
    Errorloop((char *) "could not initialize communication channel.", 0);

  // check for SPS30 connection
  if (! sps30.probe()) Errorloop((char *) "could not probe / connect with SPS30.", 0);
  else  Serial.println(F("Detected SPS30."));

  // reset SPS30 connection
  if (! sps30.reset()) Errorloop((char *) "could not reset.", 0);

  // read device info
  GetDeviceInfo();

  // start measurement
  if (sps30.start()) Serial.println(F("Measurement started"));
  else Errorloop((char *) "Could NOT start measurement", 0);

  if (SP30_COMMS == I2C_COMMS) {
    if (sps30.I2C_expect() == 4)
      Serial.println(F(" !!! Due to I2C buffersize only the SPS30 MASS concentration is available !!! \n"));
  }

  myserial.begin(115200, SERIAL_8N1, RX2_pin, TX2_pin);
  _Serial = &myserial;

  pinMode(26, OUTPUT);
  pinMode(27, OUTPUT);
  digitalWrite(26, HIGH);
  delay(800);
  digitalWrite(26, LOW);
  delay(800);
  digitalWrite(26, HIGH);
  delay(800);

  delay(5000);

  Serial.println("START.......");
  Send_command("AT");
  Serial.println("Command Status : " + String(cmm_state));
  Send_command("AT+CCID");
  Serial.println("Command Status : " + String(cmm_state));
  Send_command("AT+CIMI");
  Serial.println("Command Status : " + String(cmm_state));
  Send_command("AT+CPIN?");
  Serial.println("Command Status : " + String(cmm_state));
  Send_command("AT+CFUN=1");
  Serial.println("Command Status : " + String(cmm_state));
  Send_command("AT*MCGDEFCONT=\"IPV4V6\",\"");
  Serial.println("Command Status : " + String(cmm_state));
  Send_command("AT+CSQ");
  Serial.println("Command Status : " + String(cmm_state));
  Send_command("AT+CGREG=1");
  Serial.println("Command Status : " + String(cmm_state));
  Send_command("AT+CGATT=1");
  Serial.println("Command Status : " + String(cmm_state));
  Send_command("AT+CGCONTRDP");
  Serial.println("Command Status : " + String(cmm_state));


  //  String str1 = "Hello My name is wivach fbgvaosdighaoeihgpWEG[ehg9o*GEPIgoihidfgPAWOIfpawe0ogyhosragjxtfkxdftkzfkxjzsejhdjmztjrgljwbelp  ti98hrfpaeogya[owe4h[a0we9hg[09WH=0ihgap[0erhgap-9we8hgpoawie";
  //  String Payload = "{\"data\":\"" + str1 + "\"}";
  //  Payload = str2HexStr(Payload);
  //
  //
  //  Send_command("AT+CHTTPCREATE=\"http://pladaw.3bbddns.com:12820/\"");
  //  Serial.println("Command Status : " + String(cmm_state));
  //  Send_command("AT+CHTTPCON=0");
  //  Serial.println("Command Status : " + String(cmm_state));
  //  Send_command("AT+CHTTPSEND=0,1,\"/index.php\",4163636570743a202a2f2a0d0a436f6e6e656374696f6e3a204b6565702d416c6976650d0a557365722d4167656e743a2053494d434f4d5f4d4f44554c450d0a,\"application/json\"," + Payload);
  //  Serial.println("Command Status : " + String(cmm_state));
  //
  //  Send_command("AT+CHTTPDISCON=0");
  //  Serial.println("Command Status : " + String(cmm_state));
  //  Send_command("AT+CHTTPDESTROY=0");
  //  Serial.println("Command Status : " + String(cmm_state));
}

void loop() {
  //read_all();
 
  unsigned long currentMillis = millis();

  if (currentMillis - previousMillis >= interval) {
    // save the last time you blinked the LED

     Serial.println(F("Station 1"));
    
    previousMillis = currentMillis;

    read_all();

  }
}

/**
   @brief : read and display device info
*/
void GetDeviceInfo()
{
  char buf[32];
  uint8_t ret;
  SPS30_version v;

  //try to read serial number
  ret = sps30.GetSerialNumber(buf, 32);
  if (ret == ERR_OK) {
    Serial.print(F("Serial number : "));
    if (strlen(buf) > 0)  Serial.println(buf);
    else Serial.println(F("not available"));
  }
  else
    ErrtoMess((char *) "could not get serial number", ret);

  // try to get product name
  ret = sps30.GetProductName(buf, 32);
  if (ret == ERR_OK)  {
    Serial.print(F("Product name  : "));

    if (strlen(buf) > 0)  Serial.println(buf);
    else Serial.println(F("not available"));
  }
  else
    ErrtoMess((char *) "could not get product name.", ret);

  // try to get version info
  ret = sps30.GetVersion(&v);
  if (ret != ERR_OK) {
    Serial.println(F("Can not read version info"));
    return;
  }

  Serial.print(F("Firmware level: "));  Serial.print(v.major);
  Serial.print("."); Serial.println(v.minor);

  if (SP30_COMMS != I2C_COMMS) {
    Serial.print(F("Hardware level: ")); Serial.println(v.HW_version);

    Serial.print(F("SHDLC protocol: ")); Serial.print(v.SHDLC_major);
    Serial.print("."); Serial.println(v.SHDLC_minor);
  }

  Serial.print(F("Library level : "));  Serial.print(v.DRV_major);
  Serial.print(".");  Serial.println(v.DRV_minor);
}

/**
   @brief : read and display all values
*/
bool read_all()
{
  static bool header = true;
  uint8_t ret, error_cnt = 0;
  struct sps_values val;

  // loop to get data
  do {

    ret = sps30.GetValues(&val);

    // data might not have been ready
    if (ret == ERR_DATALENGTH) {

      if (error_cnt++ > 3) {
        ErrtoMess((char *) "Error during reading values: ", ret);
        return (false);
      }
      delay(1000);
    }

    // if other error
    else if (ret != ERR_OK) {
      ErrtoMess((char *) "Error during reading values: ", ret);
      return (false);
    }

  } while (ret != ERR_OK);

  // only print header first time
  if (header) {
    Serial.println(F("-------------Mass -----------    ------------- Number --------------   -Average-"));
    Serial.println(F("     Concentration [μg/m3]             Concentration [#/cm3]             [μm]"));
    Serial.println(F("P1.0\tP2.5\tP4.0\tP10\tP0.5\tP1.0\tP2.5\tP4.0\tP10\tPartSize\n"));
    header = false;
  }
  //String data_out = String(val.MassPM1) + ("\t") + String(val.MassPM2) + ("\t") + String(val.MassPM4) + ("\t") + String(val.MassPM10) + ("\t");
  //data_out += String(val.NumPM0) + ("\t") + String(val.NumPM1) + ("\t") + String(val.NumPM2) + ("\t") + String(val.NumPM4) + ("\t") + String(val.NumPM10) + ("\t") + String(val.PartSize) + ("\r\n");

  //String str1 = data_out;

  /*Serial.print("PM1 : ");
  Serial.println(String(val.MassPM1));
  Serial.print("PM2 : ");
  Serial.println(String(val.MassPM2));
  Serial.print("PM10 : ");
  Serial.println(String(val.MassPM10));
  Serial.print("CO : ");
  Serial.println(readCO());
  Serial.print("NO2 : ");
  Serial.println(readNO2());
  Serial.print("SO2 : ");
  Serial.println(readSO2());
  Serial.print("O3 : ");
  Serial.println(readO3());
  Serial.print("Temp : ");
  Serial.println(readTemp());
  Serial.print("Humi : ");
  Serial.println(readHum());
  Serial.print("Wind Speed : ");
  Serial.println(readWindSpeed());
  Serial.print("Wid Dir : ");
  Serial.println(readWindDir());*/
  
  DynamicJsonDocument doc(1024);
  doc["Station"] = "1";
  doc["PM1"] = String(val.MassPM1);
  doc["PM2"] = String(val.MassPM2);
  doc["PM10"] = String(val.MassPM10);
  doc["CO"] = readCO();
  doc["NO2"] = readNO2();
  doc["SO2"] = readSO2();
  doc["O3"] = readO3();
  doc["Temp"] = readTemp();
  doc["Humi"] = readHum();
  doc["WindSp"] = readWindSpeed();
  doc["WindDir"] = readWindDir();
  
  String output;
  serializeJson(doc, output);

  String Payload = output;
  Serial.println(wind_speed_adc);
  Serial.println(Payload);
  Payload = str2HexStr(Payload);
  
  Send_command("AT+CHTTPCREATE=\"http://pladaw.3bbddns.com:12820/Nitigan\"");
  Serial.println("Command Status : " + String(cmm_state));
  Send_command("AT+CHTTPCON=0");
  Serial.println("Command Status : " + String(cmm_state));
  //Send_command("AT+CHTTPSEND=0,1,\"/pmdata/index.php\",4163636570743a202a2f2a0d0a436f6e6e656374696f6e3a204b6565702d416c6976650d0a557365722d4167656e743a2053494d434f4d5f4d4f44554c450d0a,\"application/json\"," + Payload);
  //Send_command("AT+CHTTPSEND=0,1,\"/aj_nitigan.php\",,\"application/json\"," + Payload);
  Send_command("AT+CHTTPSEND=0,1,\"/aj_nitigan_1.php\",,\"application/json\"," + Payload);
  Serial.println("Command Status : " + String(cmm_state));

  Send_command("AT+CHTTPDISCON=0");
  Serial.println("Command Status : " + String(cmm_state));
  Send_command("AT+CHTTPDESTROY=0");
  Serial.println("Command Status : " + String(cmm_state));

  //Serial.print(data_out);*/

  return (true);
}

/**
    @brief : continued loop after fatal error
    @param mess : message to display
    @param r : error code

    if r is zero, it will only display the message
*/
void Errorloop(char *mess, uint8_t r)
{
  if (r) ErrtoMess(mess, r);
  else Serial.println(mess);
  Serial.println(F("Program on hold"));
  for (;;) delay(100000);
}

/**
    @brief : display error message
    @param mess : message to display
    @param r : error code

*/
void ErrtoMess(char *mess, uint8_t r)
{
  char buf[80];

  Serial.print(mess);

  sps30.GetErrDescription(r, buf, 80);
  Serial.println(buf);
}

String  str2HexStr(String strin)
{
  int lenuse = strin.length();
  char charBuf[lenuse * 2 - 1];
  char strBuf[lenuse * 2 - 1];
  String strout = "";
  strin.toCharArray(charBuf, lenuse * 2) ;
  for (int i = 0; i < lenuse; i++)
  {
    sprintf(strBuf, "%02X", charBuf[i]);

    if (String(strBuf) != F("00") )
    {
      strout += strBuf;
    }
  }

  return strout;
}

void Send_command(String cmd)
{
  String Sim_res = "";
  int fail_cunt = 0;
  do
  {
    cmm_state = -1;
    _Serial->println(cmd);
    Sim_res = Wait_module_res(500, "OK");
    fail_cunt ++;
    if (fail_cunt > 5) break;
    Serial.print(".");
    delay(300);
  } while (cmm_state != 1);

  //Sim_res.replace("OK", "");
  Serial.println(Sim_res);
}
String Wait_module_res(long tout, String str_wait)
{
  unsigned long pv_ok = millis();
  unsigned long current_ok = millis();
  String input;
  unsigned char flag_out = 1;
  unsigned char res = -1;
  _RES res_;
  res_.temp = "";
  res_.data = "";

  while (flag_out)
  {
    if (_Serial->available())
    {
      input = _Serial->readStringUntil('\n');
      res_.temp += input;
      if (input.indexOf(str_wait) != -1)
      {
        res = 1;
        cmm_state = 1;
        flag_out = 0;
      }
      else if (input.indexOf(F("ERROR")) != -1)
      {
        res = 0;
        cmm_state = 0;
        flag_out = 0;
      }
    }
    current_ok = millis();
    if (current_ok - pv_ok >= tout)
    {
      flag_out = 0;
      res = 0;
      pv_ok = current_ok;
    }
  }

  res_.status = res;
  res_.data = input;
  return (res_.temp);
}

double mapf(double val, double in_min, double in_max, double out_min, double out_max) 
{
    return (val - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

String readWindSpeed(){
  wind_speed_adc = analogRead(wind_speed_Pin);
  wind_speed_voltage = (wind_speed_adc * 3.3 ) / (4095);
  wind_speed = mapf(wind_speed_voltage, 0.17, 3.3, 1, 20); 
  if(wind_speed < 0)wind_speed = 0;
  
  return String(wind_speed);
}

String readWindDir(){
  wind_speed_adc = analogRead(wind_dir_Pin) * 2;
  if(wind_speed_adc >= 0 and wind_speed_adc < 580)wind_dir = "N";
  else if(wind_speed_adc >= 580 and wind_speed_adc < 1470)wind_dir = "NE";
  else if(wind_speed_adc >= 1470 and wind_speed_adc < 2330)wind_dir = "E";
  else if(wind_speed_adc >= 2330 and wind_speed_adc < 3240)wind_dir = "SE";
  else if(wind_speed_adc >= 3240 and wind_speed_adc < 4176)wind_dir = "S";
  else if(wind_speed_adc >= 4176 and wind_speed_adc < 4976)wind_dir = "SW";
  else if(wind_speed_adc >= 4976 and wind_speed_adc < 5870)wind_dir = "W";
  else if(wind_speed_adc >= 5870 and wind_speed_adc < 6040)wind_dir = "NW";
  else wind_dir = "UN";
  return String(wind_dir);
}

String readTemp(){
  
  return String(hdc1080.readTemperature());
  
}

String readHum(){
  
  return String(hdc1080.readHumidity());
  
}

String readNO2(){
  
  return String(analogRead(sensor_NO2));
}

String readCO(){
  
  return String(analogRead(sensor_CO));
}

String readSO2(){
  
  return String(analogRead(sensor_S02));
}

String readO3(){
  
  return String(analogRead(sensor_O3));
}
