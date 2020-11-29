
#include<SoftwareSerial.h>//for bluethooth and wifi module
SoftwareSerial ESPserial(2,3);//(Rx,Tx)//wifi pins
String wifiname= "The Effortless";
String pass= "KandaMalli2893";
String data;
int ir_out1=6;//ir sensor-1
int ir_out2=7;//ir sensor-2
int res1,res2;
void setup() {
  // put your setup code here, to run once:
    ESPserial.begin(115200);
    Serial.begin(115200);
   pinMode(8,OUTPUT);
   pinMode(9,OUTPUT);
   pinMode(10,OUTPUT);
   pinMode(ir_out1,INPUT);
   pinMode(ir_out2,INPUT);
  espcmd("AT+RST");//attention command to reset the wifi module 
  espcmd("AT+CWMODE=1");//wifi module acts as a client
  espcmd("AT+CWJAP=\""+ wifiname +"\",\""+ pass +"\"");//attention command to connect to local wifi network
  while(!ESPserial.find("OK")){
    //becomes false and exits the loop only after the wifi module is connected
  }
  delay(1000);
  Serial.print("Successfull");
}


void loop() {
  // put your main code here, to run repeatedly:
    res1=digitalRead(ir_out1);//reads output from the first ir sensor
    res2=digitalRead(ir_out2);//reads output from the second ir sensor
    if(res1==0){
      Serial.println("output on");
      digitalWrite(8,HIGH);
      light_on();//user defined function to turn on lights
      }
    else if(res2==0){
      Serial.println("output off");
       digitalWrite(9,HIGH);
      light_off();//user defined function to turn off lights
       }
    else{
      Serial.println("blank");
      delay(1000);
       digitalWrite(10,HIGH);
      }
}
void light_on(){//user defined function to turn on lights
  espcmd("AT+CIPMUX=0");//attention command to specify number of devises to be connected
  espcmd("AT+CIPSTART=\"TCP\",\"api.thingspeak.com\",80");//adhress of the cloud we are using
   data="GET /update?api_key=EG23Y36D9LLC1KRJ&field1=1";//read api key given by the cloud
  espcmd("AT+CIPSEND="+String(data.length()+4));//attention comman to send data 
  ESPserial.find(">");
  ESPserial.println(data);
  espcmd("AT+CIPCLOSE=0");
  delay(1000);
}
void light_off(){//user defined function to turn off lights
  espcmd("AT+CIPMUX=0");
  espcmd("AT+CIPSTART=\"TCP\",\"api.thingspeak.com\",80");//adhress of the cloud we are using
  data="GET /update?api_key=EG23Y36D9LLC1KRJ&field2=1";//read api key given by the cloud
  espcmd("AT+CIPSEND="+String(data.length()+2));//attention comman to send data
  ESPserial.find(">");
  ESPserial.println(data);
  espcmd("AT+CIPCLOSE=0");
  delay(10000);
}
void espcmd(String cmd){//function to write in the esp monitor
  Serial.println(cmd);
  ESPserial.println(cmd);//used to print in the esp monitor
  delay(1000);
}
