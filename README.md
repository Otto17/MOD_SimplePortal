# MOD_SimplePortal
ФОРК библиотеки "SimplePortal". Простой WiFi менеджер с возможностью задания Логина/Пароля для WEB интерфейса

### Совместимость
esp8266, esp32

## Содержание
- [Инициализация](#init)
- [Использование](#usage)
- [Пример](#example)
- [История изменений](#history)


<a id="init"></a>
## Инициализация
Нет

<a id="usage"></a>
## Использование
```cpp
void portalStart();     // запустить портал
void portalStop();      // остановить портал
bool portalTick();      // вызывать в цикле
void portalRun(uint32_t prd = 60000);   // блокирующий вызов
byte portalStatus();    // статус: 1 connect, 2 ap, 3 local, 4 exit, 5 timeout

// константы статуса
SP_ERROR            // ошибка
SP_SUBMIT           // отправлены логин-пароль
SP_SWITCH_AP        // сигнал на смену в AP
SP_SWITCH_LOCAL     // сигнал на смену в Local
SP_EXIT             // нажата кнопка выход
SP_TIMEOUT          // вышел таймаут
```

### Хранение настроек
Из скетча доступна переменная (структура) **portalCfg**:
- `char* ssid[32]` - логин
- `char* pass[32]` - пароль
- `char* loginUI[20]` - логин UI
- `char* passUI[32]` - пароль UI
- `byte mode` - (1 `WIFI_STA`, 2 `WIFI_AP`)
После отправки данных с портала можно забрать информацию из структуры и/или записать её в EEPROM.

### Как работает
![demo](/doc/demo.png)
- Библиотека запускает режим **SoftAP**, запускает **DNSServer** и **ESP8266WebServer**.
- Коннектимся к точке, по умолчанию называется **"ESP Config"**
- Откроется страница конфигурации (если не открылась - перейди по *192.168.4.1*). На странице можно 
настроить режим работы ESP, а также сконфигурировать подключение к роутеру (логин-пароль).
- Кнопка **"Сохранить ✓"** отправляет введённые данные на ESP. После нажатия **"Сохранить ✓"** режим в конфиге переключается в 1 (WIFI_STA).
- Библиотека не переключает итоговый режим работы esp, кнопки портала **"Режим AP"** / **"Режим STA"** просто дают сигналы "в скетч".
- Кнопка **"Закрыть портал"** просто завершает работу портала.
- После нажатия любой кнопки или выхода таймаута портал закрывается, сервер останавливается, SoftAP отключается.

<a id="example"></a>
## Примеры
### Блокирующий
```cpp
#include <ESP8266WiFi.h>
#include <SimplePortal.h>

void setup() {
  Serial.begin(9600);
  delay(3000);
  
  portalRun();  // запустить с таймаутом 60с
  //portalRun(30000); // запустить с кастомным таймаутом
  
  Serial.println(portalStatus());
  // статус: 0 error, 1 connect, 2 ap, 3 local, 4 exit, 5 timeout
  
  if (portalStatus() == SP_SUBMIT) {
    Serial.println(portalCfg.SSID);
    Serial.println(portalCfg.pass);
    // забираем логин-пароль
  }
}

void loop() {
}
```

### Асинхронный
```cpp
#include <ESP8266WiFi.h>
#include <SimplePortal.h>

void setup() {
  Serial.begin(9600);

  // запускаем портал
  portalStart();
}

void loop() {
  // вызываем в loop
  if (portalTick()) {
    Serial.println(portalStatus());
    if (portalStatus() == SP_SUBMIT) {
      Serial.println(portalCfg.SSID);
      Serial.println(portalCfg.pass);
      // забираем логин-пароль
    }

    // сработает однократно при действии
    // точка будет автоматически выключена
  }
}
```


### Получение данных WiFi и Логина/Пароля (Блокирующий)
```cpp
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

```

<a id="history"></a>
## История изменений находится в **"CHANGELOG"**

  Дата изменения: **01.03.2023г.**
  
  Версия: **0.1**
  
  Автор: **Otto**
