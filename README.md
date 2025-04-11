# 🚀 Control de LEDs con ESP8266 + WebSocket

Un sistema de control remoto para tiras de LEDs NeoPixel (WS2812B) mediante una interfaz web usando WebSocket. El ESP8266 actúa como servidor web y WebSocket, permitiendo intercalar entre 2 colores de LEDs individualmente o en grupo, con persistencia de estado.

## 🌟 Características principales
- ✅ Control individual o grupal de LEDs
- ✅ Persistencia del estado (los LEDs mantienen su color después de reiniciar)
- ✅ Interfaz web responsive
- ✅ Comunicación en tiempo real con WebSocket
- ✅ Fácil configuración WiFi

## 📦 Hardware Requerido
- Placa ESP8266 (NodeMCU, Wemos D1 Mini, etc.)
- Tira de LEDs NeoPixel (WS2812B) - (en este ejemplo se usaron 144 LEDs)
- Fuente de alimentación adecuada
- Resistencia de 330-470Ω y capacitor de 1000µF (recomendados para protección)

## 🛠️ Configuración del ESP8266

### 📋 Requisitos
- Arduino IDE
- Board ESP8266 instalada (usar Boards Manager)
- Bibliotecas necesarias:
  - ESP8266WiFi
  - WebSockets
  - ArduinoJson
  - Adafruit NeoPixel
  - ESP8266mDNS

### ⚙️ Pasos de instalación
1. Clonar este repositorio
2. Abrir `ESP8266_LED_Controller/Microcontrolador/ESP8266.ino` en Arduino IDE
3. Modificar las variables en base a lo que uses en el código ESP8266.ino:
   ```cpp
   const char* ssid = "TU_SSID";
   const char* password = "TU_PASSWORD";
   #define LED_PIN "TU_PIN"
   #define LED_COUNT "NUMERO_LEDS"
   #define JSON_SIZE "TAMAÑO_JSON" (calcular alrededor de 20 bytes por LED, siempre redondear para arriba el resultado)
4. Subir el codigo a la placa ESP8266
5. Modificar las variables en base a lo que uses en el código:
   ```cpp
   FrontEnd/src/app.jsx
   const leds = Array.from({ length: "NUMERO_LEDS" }, (_, i) => i);
   FrontEnd\src\WebSocketContext.js
   details: [
    "Asegúrese de estar conectado al mismo router que el microcontrolador.",
    "Red: TP-LINK_FE90", //red que use en el momento
    "Contraseña: 67992980", //lo mismo
    "Asegúrese de que el microcontrolador esté encendido.",
    "Recargue la página para volver a intentar la conexión."
    ]

## 🛠️ Configuración del archivo del FrontEnd

### 📋 Requisitos
- Editor de Código de preferencia
- Node.js
- Un padre nuestro (opcional)

### ⚙️ Pasos de instalación
1. Descarga Node.js si no lo tenés
2. Descargar todas las dependencias necesarias:
   - react-scripts
   - react-dom
   - express
3. conecta tu pc al mismo internet que el microcontrolador
4. usa npm start o npm run build para empaquetarlo
5. opcional en caso de usar el npm run build: usa el inicio.bat como acceso directo para ejecutar el programa, cambiando la direccion en "serve "C:\users\usuario\documents\gestorled\build"" segun donde guardes el build

## 🚩 Cosas tener en cuenta
En base a mi experiencia programandolo es importante tener en cuenta los siguientes puntos:
  
  - Usar routers de 2.4Ghz de frecuencia, no funciona con aquellos de 5Ghz
  
  - Por alguna razón con algunas tiras de LED la señal de luz verde era luz roja y viceversa, actualmente no sé por qué ocurría pero si te sucede lo mismo puedes modificar la parte grafica de los botones para no tener que modificar el codigo del microcontrolador y subirlo devuelta en él
  
  - Originalmente este codigo estaba pensado para ESP32, y si cambian las dependencias a las necesarias para esta, en teoria deberia funcionar, sin embargo en la práctica nunca me funcionó, tal vez a ustedes si prueban les salga

### 🤝 Contribuciones

¡PRs y sugerencias son bienvenidas!  

📄 **Licencia**: [MIT](LICENSE.md)  

Creado por Liam Di Cerbo ([GitHub](https://github.com/Ezeliam))

