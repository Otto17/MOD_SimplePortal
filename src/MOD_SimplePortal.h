/*
    ФОРК библиотеки "SimplePortal".
	Простой WiFi менеджер с возможностью задания Логина/Пароля для WEB интерфейса
		
    GitHub: https://github.com/Otto17/MOD_SimplePortal
    

    Версия:
    ver 0.1

*/

#define SP_AP_NAME "Wake-On-Line Connect"	// Название точки доступа
#define SP_AP_IP 192,168,4,1        		// IP точки доступа

#ifndef _MOD_SimplePortal_h
#define _MOD_SimplePortal_h
#include <DNSServer.h>
#ifdef ESP8266
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#else
#include <WiFi.h>
#include <WebServer.h>
#endif

#define SP_ERROR 0
#define SP_SUBMIT 1
#define SP_SWITCH_AP 2
#define SP_SWITCH_LOCAL 3
#define SP_EXIT 4
#define SP_TIMEOUT 5

//Структура для получения с WEB интерфейса данных
struct PortalCfg {
  char ssid[32];
  char pass[32];
  char loginUI[20];
  char passUI[20];
  uint8_t mode = WIFI_AP;    // (1 WIFI_STA, 2 WIFI_AP)
};
extern PortalCfg portalCfg;

void portalStart();     // Запустить портал
void portalStop();      // Остановить портал
bool portalTick();      // Вызывать в цикле
void portalRun(uint32_t prd = 60000);   // Блокирующий вызов
byte portalStatus();    // Статус: 1 connect, 2 ap, 3 local, 4 exit, 5 timeout

void SP_handleConnect();
void SP_handleAP();
void SP_handleLocal();
void SP_handleExit();
#endif