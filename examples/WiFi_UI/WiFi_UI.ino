/* 
*
* Пример получения данных в скетч от MOD_SimplePortal
* 
*/


#include <ESP8266WiFi.h>
#include <MOD_SimplePortal.h>
#include <EEPROM.h>


void setup() {
  Serial.begin(115200);  // Инициализация Serial

  portalRun(300000);  // Pапустить с таймаутом 5 минут


  if (portalStatus() == SP_SUBMIT) {  // Если кнопка "Сохранить" с портала нажата, то получаем данные с формы через структуру
    EEPROM.begin(120);                // Инициализируем EEPROM
    EEPROM.put(0, portalCfg);         // Подготавливаем структуру к записи с нулевой ячейки (сохраняем её в ОЗУ)
    EEPROM.commit();                  // Записываем из ОЗУ в EEPROM
    EEPROM.end();                     // И очищаем данные из ОЗУ
    WiFi.softAPdisconnect();          // Отключаем режим AP
  }



  //Читаем логин пароль из памяти
  EEPROM.begin(120);         // Инициализируем EEPROM
  EEPROM.get(0, portalCfg);  // Считываем структуру начиная с нулевой ячейки


  Serial.print("SSID: ");
  Serial.println(portalCfg.ssid);
  Serial.print("Pass WiFi: ");
  Serial.println(portalCfg.pass);
  Serial.print("Login UI: ");
  Serial.println(portalCfg.loginUI);
  Serial.print("Pass UI: ");
  Serial.println(portalCfg.passUI);


  //Подключаемся к WiFi
  Serial.print("Connect to: ");
  Serial.println(portalCfg.ssid);
  WiFi.mode(WIFI_STA);                         // Запускаем WiFi в режиме "Станция"
  WiFi.hostname("ESP-TEST");                   // Задаём имя хоста
  WiFi.begin(portalCfg.ssid, portalCfg.pass);  // Берём данные из структуры
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println();
  Serial.print("Connected! Local IP: ");
  Serial.println(WiFi.localIP());
}


void loop() {
}
