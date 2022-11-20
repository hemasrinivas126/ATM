#ifdef ESP32
#include <WiFi.h>
#else
#include <ESP8266WiFi.h>
#endif
#include <WiFiClientSecure.h>
#include <UniversalTelegramBot.h>  
#include <ArduinoJson.h>
#include <HTTPClient.h>
const char* ssid = "Jai Balayya";
const char* password = "tcdp3983";
HTTPClient http;
String GOOGLE_SCRIPT_ID = "AKfycby_v-Vgi9-RPJR-Tg54Sr4Jy-uTNCB0yncA_O9tqOoMz7QbeHw0s3V9_tTdWK41Ycft";
String text;
int httpCode;
#define BOTtoken "5984324473:AAHfJrg9zyYowM7usCI7FF0mYrcTZoHa1Mg"
#define CHAT_ID "1545327246"
#ifdef ESP8266
X509List cert(TELEGRAM_CERTIFICATE_ROOT);
#endif
WiFiClientSecure client;
UniversalTelegramBot bot(BOTtoken, client);
int botRequestDelay = 1000;
unsigned long lastTimeBotRan;
const int ledPin = 2;
bool ledState = LOW;
int numNewMessages;
String for_balance_c;
String for_balance_d;

void handleNewMessages(int numNewMessages) {
  numNewMessages=numNewMessages;
  Serial.println("handleNewMessages");
  Serial.println(String(numNewMessages));
  for (int i = 0; i < numNewMessages; i++) {
    String chat_id = String(bot.messages[i].chat_id);
    if (chat_id != CHAT_ID) {
      bot.sendMessage(chat_id, "Unauthorized user", "");
      continue;
    }
  
    text = bot.messages[i].text;
    String from_name = bot.messages[i].from_name;

    if (text == "/start") {
      String welcome = "Welcome, " + from_name + ".\n";
      welcome += "Use the following commands to control your outputs.\n\n";
      welcome += "/login to proceed further \n";
      welcome += "/register if you want to create new account  \n";
      bot.sendMessage(chat_id, welcome, "");
    }

    if (text == "/login") {
      String unit = "1";
      String check_acc_pass;
      String acc_recieve;
      bot.sendMessage(chat_id, "Enter your account number", "");
      delay(12000);
      Serial.println(text);
      Serial.println("Before the func");
      func();
      text = bot.messages[i].text;
      func();
      Serial.println("After the func");
      Serial.println(text);
      String acc_num=text;
      bot.sendMessage(chat_id, "Enter your password", "");
      delay(12000);
      func();
      text = bot.messages[i].text;
      func();
      
      Serial.println("inside password");
      Serial.println(text);
      Serial.println("Request the url");
      HTTPClient http;
      String urlFinal2 = "https://script.google.com/macros/s/" + GOOGLE_SCRIPT_ID + "/exec?" + "password=" + text +"&account_no=" + String(acc_num) +"&acc_pass=1" ;
      http.begin(urlFinal2.c_str());
      http.setFollowRedirects(HTTPC_STRICT_FOLLOW_REDIRECTS);
      int httpCode = http.GET();
      if (httpCode > 0) {
        check_acc_pass = http.getString();
      } else {
        Serial.println("no httpcode");
      }
      Serial.println(check_acc_pass);
      if(acc_num.length()!=12){
        check_acc_pass="TRUE1";
        
      }
      if (check_acc_pass == "TRUE") {
        String welcome = "Welcome, " + from_name + ".\n";
        welcome += "Use the following commands to control your outputs.\n\n";
        welcome += "/CREDIT to credit the amount \n";
        // welcome += "/led_off to turn GPIO OFF \n";
        welcome += "/DEBIT to debit the amount \n";
        bot.sendMessage(chat_id, welcome, "");
      }
      if (check_acc_pass == "TRUE1"){
        String welcome = "Sorry " + from_name + ".\n";
        welcome += "The account number must contains of 12 digits.\n\n";
        welcome += "/start to login again \n";
        bot.sendMessage(chat_id, welcome, "");
      }
      else if(check_acc_pass == "FALSE"){
        String welcome = "Sorry " + from_name + ".\n";
        welcome += "login creditials are Wrong.\n\n";
        welcome += "/start to enter creditials again \n";
        bot.sendMessage(chat_id, welcome, "");
      }
    }
    if (text == "/CREDIT") {
      String opt1 = "CREDIT";
      bot.sendMessage(chat_id, "Enter how many 100 notes to credit", "");
      delay(12000);
      func();
      text = bot.messages[i].text;
      func();
      Serial.println(text);
      String for_balance_c = "0";
      HTTPClient http;
      String urlFinal4 = "https://script.google.com/macros/s/" + GOOGLE_SCRIPT_ID + "/exec?" + "notes_credit=" + text + "&opt_credit=" + opt1 ;
      http.begin(urlFinal4.c_str());
      http.setFollowRedirects(HTTPC_STRICT_FOLLOW_REDIRECTS);
      Serial.println("Request the url");
      int httpCode1 = http.GET();
      if (httpCode1 > 0) {
        for_balance_c = http.getString();
      } else {
        Serial.println("no httpcode");
      }
      Serial.println(httpCode1);
      Serial.println(for_balance_c);
      http.end();
      int int_for_balance_c=for_balance_c.toInt();
      if (int_for_balance_c >0) {
        String welcome = "Welcome, " + from_name + ".\n";
        welcome += "Balance: "+for_balance_c+"rs.\n";
        welcome += "\n\n /exit To exit \n";
        bot.sendMessage(chat_id, welcome, "");
      }
    } 
    if (text == "/DEBIT") {
      String opt2 = "DEBIT";
      bot.sendMessage(chat_id, "Enter how many 100 notes to debit", "");
      delay(12000);
      func();
      text = bot.messages[i].text;
      func();
      String for_balance_d;
      HTTPClient http;
      String urlFinal6= "https://script.google.com/macros/s/" + GOOGLE_SCRIPT_ID + "/exec?" + "notes_debit=" + text + "&opt_debit=" + opt2;
      http.begin(urlFinal6.c_str());
      http.setFollowRedirects(HTTPC_STRICT_FOLLOW_REDIRECTS);
      Serial.println("Request the url");
      int httpCode1 = http.GET();
      if (httpCode1 > 0) {
        for_balance_d = http.getString();
      } else {
        Serial.println("no httpcode");
      }
      Serial.println(httpCode1);
      Serial.println(for_balance_d);
      int int_for_balance_d=for_balance_d.toInt();
      if (int_for_balance_d>0) {
        String welcome = "Welcome, " + from_name + ".\n";
        welcome += "Balance: "+ for_balance_d+ "rs.\n";
        welcome += "\n\n /exit To exit \n";
        bot.sendMessage(chat_id, welcome, "");
      }

    }
    if (text == "/exit") {
      String welcome = "Thank you" + from_name + ".\n";
      welcome += "\n\n /start to enter the account number \n";
      bot.sendMessage(chat_id, welcome, "");
    }
    else if (text == "/register") {
      String new_acc_num;
      String create;
      String new_check_acc_pass;
      bot.sendMessage(chat_id, "Enter your account number", "");
      delay(12000);
      Serial.println(text);
      Serial.println("Before the func");
      func();
      text = bot.messages[i].text;
      func();
      Serial.println("After the func");
      Serial.println(text);
      new_acc_num=text;
      bot.sendMessage(chat_id, "Enter your password", "");
      delay(12000);
      func();
      text = bot.messages[i].text;
      func();
      Serial.println("inside password");
      Serial.println(text);
      Serial.println("Request the url");
      if(new_acc_num.length()!=12){
        new_check_acc_pass="FALSE2";
      }
      else{
        new_check_acc_pass="TRUE2";
      }
      if (new_check_acc_pass == "FALSE2"){
        String welcome = "Sorry " + from_name + ".\n";
        welcome += "The account number must contains of 12 digits.\n\n";
        welcome += "/register to register again \n";
        bot.sendMessage(chat_id, welcome, "");
      }
      else if(new_check_acc_pass="TRUE2"){
        HTTPClient http;
        String urlFinal8 = "https://script.google.com/macros/s/" + GOOGLE_SCRIPT_ID + "/exec?" + "newpassword=" + text +"&newaccount_no=" + String(new_acc_num) +"&newacc_pass=1" ;
        http.begin(urlFinal8.c_str());
        http.setFollowRedirects(HTTPC_STRICT_FOLLOW_REDIRECTS);
        int httpCode = http.GET();
        if (httpCode > 0) {
          create = http.getString();
        } else {
          Serial.println("no httpcode");
        }
        Serial.println(create);
        if(create=="CREATED"){
          String welcome = "congratulations" + from_name + ".\n";
          welcome += "Account created\n\n";
          welcome += "/start click start to login  \n";
          bot.sendMessage(chat_id, welcome, "");
        }
        
      }
    }

    // else{

    //   String welcome = "Thank you" + from_name + ".\n";
    //   welcome += "Wrong Input";
    //   welcome += "\n\n /start to enter the account number \n";
    //   bot.sendMessage(chat_id, welcome, "");
    // }
  }
}

void setup() {
  Serial.begin(115200);
#ifdef ESP8266
  configTime(0, 0, "pool.ntp.org");  
  client.setTrustAnchors(&cert);  
#endif
  pinMode(ledPin, OUTPUT);
  digitalWrite(ledPin, ledState);
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
#ifdef ESP32
  client.setCACert(TELEGRAM_CERTIFICATE_ROOT);  
#endif
  while (WiFi.status() != WL_CONNECTED) {
    delay(100);
    Serial.print(".");
  }
  Serial.println(WiFi.localIP());
  text = "1";
}

void loop() {
  if (millis() > lastTimeBotRan + botRequestDelay) {
    int numNewMessages = bot.getUpdates(bot.last_message_received + 1);
    while (numNewMessages) {
      Serial.println("got response");
      handleNewMessages(numNewMessages);
      numNewMessages = bot.getUpdates(bot.last_message_received + 1);
    }
    lastTimeBotRan = millis();
  }
}
void func()
{
  if (millis() > lastTimeBotRan + botRequestDelay) {
    int numNewMessages = bot.getUpdates(bot.last_message_received + 1);
    while (numNewMessages) {
      Serial.println("got response");
      for (int i = 0; i < numNewMessages; i++) {
        String chat_id = String(bot.messages[i].chat_id);
        if (chat_id != CHAT_ID) {
          bot.sendMessage(chat_id, "Unauthorized user", "");
          continue;
        }
      }
      numNewMessages = bot.getUpdates(bot.last_message_received + 1);
    }
    lastTimeBotRan = millis();
  }
}
