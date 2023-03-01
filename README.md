# MOD_SimplePortal
ФОРК библиотеки "SimplePortal". Простой WiFi менеджер с возможностью задания Логина/Пароля для WEB интерфейса

### Совместимость
esp8266, esp32

## Содержание
- [Инициализация](#init)
- [Использование](#usage)
- [Пример](#example)
- [Отличия форка](#renew)
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
- `char* SSID[32]` - логин
- `char* pass[32]` - пароль
- `byte mode` - (1 `WIFI_STA`, 2 `WIFI_AP`)
После отправки данных с портала можно забрать информацию из структуры и/или записать её в EEPROM.

### Как работает
![demo](/doc/demo.png)
- Библиотека запускает режим **SoftAP**, запускает **DNSServer** и **ESP8266WebServer**.
- Коннектимся к точке, по умолчанию называется *"ESP Config"*
- Откроется страница конфигурации (если не открылась - перейди по *192.168.1.1*). На странице можно 
настроить режим работы ESP, а также сконфигурировать подключение к роутеру (логин-пароль).
- Кнопка *Submit* отправляет введённые данные на ESP. После нажатия *Submit* режим в конфиге переключается в 1 (WIFI_STA).
- Библиотека не переключает итоговый режим работы esp, кнопки портала *Switch AP* / *Switch Local* просто дают сигналы "в скетч".
- Кнопка Exit просто завершает работу портала.
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

<a id="renew"></a>
## Отличия от оригинального SimplePortal
- Добавлены поля Логин/Пароль (для UI или других целей) на WEB интерфейсе.
Получение данных из структуры такое же как в оригинале, только добавились ещё 2 поля с логином и паролем от UI.
- Немного украсил WEB страничку.


<a id="history"></a>
## История изменений находится в **"CHANGELOG"**

  Дата изменения: **01.03.2023г.**
  
  Версия: **0.1**
  
  Автор: **Otto**
