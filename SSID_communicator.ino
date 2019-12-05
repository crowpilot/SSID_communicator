#include <M5Stack.h>
#include "WiFi.h"
#include "esp_wifi.h"
#include <WebServer.h>

const IPAddress IP(10, 0, 0, 1);
const IPAddress SUBNET(255, 255, 255, 0);

wifi_sta_list_t stationList;

char WIFIKEY[] = "";
char SSID[30] = "Hello Crow";

//alphabet[setchar] to SSID_set[cursor_pos]

char alphabet[] = " ABCDEFGHIJKLMNOPQRSTUVWXYZ!? ()";
int cursor_pos = 0;
int selectchar = 0;

WebServer server(80);

void setup() {
  // put your setup code here, to run once:
  esp_wifi_set_promiscuous(true);
  M5.begin();
}

int btn_flag = 0;

void loop() {
  M5.Lcd.setCursor(0, 0);
  M5.Lcd.setTextSize(3);
  M5.Lcd.print("SSID communicator");
  M5.Lcd.setTextSize(2);

  M5.Lcd.setCursor(0, 65);
  M5.Lcd.printf("SSID:");q
  M5.Lcd.setCursor(0, 90);
  M5.Lcd.printf("Connect:%2d ", WiFi.softAPgetStationNum());

  M5.Lcd.setCursor(0, 180);
  M5.Lcd.print("HLD BAK    CLR");
  M5.Lcd.setCursor(0, 210);
  M5.Lcd.setTextSize(3);
  M5.Lcd.print("  SEL  ENT  SEND");
  M5.Lcd.setTextSize(2);
  M5.Lcd.setCursor(0, 30);
  M5.Lcd.print(SSID);
  M5.Lcd.setCursor(0, 45);
  for (int i = 0; i < cursor_pos; i++) {
    M5.Lcd.print(" ");
  }
  M5.Lcd.print("-");
  // put your main code here, to run repeatedly:
  //hold bak
  if (M5.BtnA.pressedFor(1000)) {
    btn_flag = 1;
  }
  if (M5.BtnA.wasReleased()) {
    if (btn_flag == 1) {
      btn_flag = 0;
      if (cursor_pos > 0) {
        cursor_pos--;
        M5.Lcd.setCursor(0, 45);
        M5.Lcd.print("                          ");
      }
    } else {
      if (selectchar < 32) {
        selectchar++;
      } else {
        selectchar = 0;
      }
      SSID[cursor_pos] = alphabet[selectchar];
      M5.Lcd.setCursor(0, 30);
      M5.Lcd.print("                   ");
      M5.Lcd.setCursor(0, 30);
      M5.Lcd.print(SSID);
    }
  }
  if (M5.BtnB.wasPressed()) {
    cursor_pos += 1;
    selectchar = 0;
  }
  //reset
  if (M5.BtnB.pressedFor(1000)) {
    for (int i = 0; i < 30; i++) {
      SSID[i] = ' ';
    }
    M5.Lcd.setCursor(0, 45);
    M5.Lcd.print("                          ");
    cursor_pos = 0;
    selectchar = 0;
  }
  if (M5.BtnC.wasPressed()) {

    WiFi.softAP(SSID, WIFIKEY);
    delay(100);
    WiFi.softAPConfig(IP, IP, SUBNET);
    delay(100);
    M5.Lcd.setCursor(0, 65);
    M5.Lcd.printf("SSID:%s", SSID);
//    M5.Lcd.setCursor(0, 100);
//    M5.Lcd.printf("Connect:%2d ", WiFi.softAPgetStationNum());

  }

  esp_wifi_ap_get_sta_list(&stationList);
  wifi_sta_info_t station = stationList.sta[stationList.num-1];
  
  M5.Lcd.setCursor(0,110);
  M5.Lcd.print("MAC:");
  for(int i=0;i<6;i++){
    M5.Lcd.printf("%02x:",station.mac[i]);
  }

  
  M5.update();
}
