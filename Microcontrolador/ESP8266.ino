#include <ESP8266WiFi.h>       // Usar ESP8266WiFi en lugar de WiFi.h
#include <WebSocketsServer.h>  // Compatible con ESP8266
#include <ArduinoJson.h>       // Compatible con ESP8266
#include <ESP8266mDNS.h>       // Usar ESP8266mDNS en lugar de ESPmDNS
#include <FS.h>                // Usar FS en lugar de SPIFFS (SPIFFS está incluido en FS)
#include <Adafruit_NeoPixel.h> // Usar Adafruit NeoPixel

const char* ssid = "TP-LINK_FE90"; //Acá va el nombre del Router al que se conectan ambos, tanto el microcontrolador como la computadora donde se ejecuta el programa para controlar los LEDs
const char* password = "67992980"; //Contraseña del Router

#define LED_PIN D4         // Pin donde están conectados los LEDs (D4 en mi caso con el ESP8266)
#define LED_COUNT 144      // Número total de LEDs (debería funcionar con cualquier cantidad que soporte el pin del ESP8266
#define JSON_SIZE 3000     // Valor por bytes del JSON, se debe calcular aprox 20 bytes por LED, por lo que para 144 LEDs sería 2880 bytes, pero para evitar problemas uso 3000, mejor redondear para arriba

// Inicializar la tira de LEDs con Adafruit NeoPixel (en mi caso me funcionó con esta biblioteca)
Adafruit_NeoPixel strip(LED_COUNT, LED_PIN, NEO_GRB + NEO_KHZ800);

WebSocketsServer webSocket = WebSocketsServer(81);

struct LedState {
    String color;
} ledStates[LED_COUNT];

void createDefaultJson() {
    if (SPIFFS.exists("/leds.json")) {
        Serial.println("✅ El archivo leds.json ya existe.");
        return;
    }

    Serial.println("Creando archivo leds.json con valores predeterminados...");
    File file = SPIFFS.open("/leds.json", "w");
    if (!file) {
        Serial.println("❌ Error al crear el archivo.");
        return;
    }

    StaticJsonDocument<JSON_SIZE> doc;
    JsonArray array = doc.createNestedArray("leds");

    for (int i = 0; i < LED_COUNT; i++) {
        JsonObject obj = array.createNestedObject();
        obj["color"] = "rojo"; // Estado inicial: todos en rojo solamente la primera vez que se ejecuta el programa
    }

    if (serializeJson(doc, file) == 0) {
        Serial.println("❌ Error al escribir en el archivo.");
    } else {
        Serial.println("✅ Archivo leds.json creado con éxito.");
    }

    file.close();
}

void saveState() {
    File file = SPIFFS.open("/leds.json", "w");
    if (!file) {
        Serial.println("❌ Error al abrir el archivo para escribir.");
        return;
    }

    StaticJsonDocument<JSON_SIZE> doc; // Tamaño del JSON
    JsonArray array = doc.createNestedArray("leds");
    for (int i = 0; i < LED_COUNT; i++) {
        JsonObject obj = array.createNestedObject();
        obj["color"] = ledStates[i].color;
    }

    // Imprimir el JSON antes de guardarlo para corroborar
    Serial.println("JSON a guardar:");
    serializeJsonPretty(doc, Serial);
    Serial.println();

    if (serializeJson(doc, file) == 0) {
        Serial.println("❌ Error al escribir en el archivo.");
    }

    file.close();

    // Verificar que el archivo no esté vacío
    File checkFile = SPIFFS.open("/leds.json", "r");
    if (checkFile.size() == 0) {
        Serial.println("❌ El archivo está vacío después de escribirlo.");
    } else {
        Serial.println("✅ Archivo escrito correctamente.");
    }
    checkFile.close();
}

void loadState() {
    if (!SPIFFS.exists("/leds.json")) {
        Serial.println("❌ El archivo leds.json no existe.");
        return;
    }

    File file = SPIFFS.open("/leds.json", "r");
    if (!file) {
        Serial.println("No se encontró archivo de estado previo.");
        return;
    }


    StaticJsonDocument<JSON_SIZE> doc; // Ajusta el tamaño según sea necesario
    DeserializationError error = deserializeJson(doc, file);
    file.close();

    if (error) {
        Serial.print("❌ Error al cargar JSON desde archivo: ");
        Serial.println(error.c_str()); // Imprimir el tipo de error
        return;
    }

    // Imprimir el JSON cargado
    Serial.println("JSON cargado:");
    serializeJsonPretty(doc, Serial);
    Serial.println();

    JsonArray array = doc["leds"].as<JsonArray>();
    for (int i = 0; i < LED_COUNT && i < array.size(); i++) {
        ledStates[i].color = array[i]["color"].as<String>();
        if (ledStates[i].color == "rojo") {
            strip.setPixelColor(i, strip.Color(255, 0, 0)); // Rojo
        } else if (ledStates[i].color == "verde") {
            strip.setPixelColor(i, strip.Color(0, 255, 0)); // Verde
        } else {
            strip.setPixelColor(i, strip.Color(0, 0, 0)); // Apagar el LED
        }
    }
    strip.show(); // Actualizar la tira de LEDs
}


void printFileContent() {
    File file = SPIFFS.open("/leds.json", "r");
    if (!file) {
        Serial.println("❌ No se pudo abrir el archivo para lectura.");
        return;
    }

    Serial.println("Contenido de leds.json:");
    while (file.available()) {
        Serial.write(file.read());
    }
    Serial.println();
    file.close();
}

void handleWebSocketMessage(uint8_t num, uint8_t *payload, size_t length) {
    StaticJsonDocument<200> doc;
    DeserializationError error = deserializeJson(doc, payload, length);

    // Imprimir el JSON recibido
    Serial.println("JSON recibido:");
    Serial.println((char*)payload); // Imprimir el payload como cadena
    Serial.println();

    if (!error) {
        String action = doc["action"]; // Obtener la acción del Front-End

        if (action == "setAll") {
            // Cambiar el estado de todos los LEDs
            String color = doc["color"].as<String>();

            for (int i = 0; i < LED_COUNT; i++) {
                ledStates[i].color = color;

                if (color == "rojo") {
                    strip.setPixelColor(i, strip.Color(255, 0, 0)); // Rojo
                } else if (color == "verde") {
                    strip.setPixelColor(i, strip.Color(0, 255, 0)); // Verde
                } else {
                    strip.setPixelColor(i, strip.Color(0, 0, 0)); // Apagar el LED
                }
            }

            strip.show(); // Actualizar la tira de LEDs
            saveState(); // Guardar el estado en SPIFFS

            // Enviar confirmación al cliente
            String response;
            StaticJsonDocument<200> responseDoc;
            responseDoc["action"] = "updateState"; // Confirmar que todos los LEDs se actualizaron
            JsonArray array = responseDoc.createNestedArray("leds");
            for (int i = 0; i < LED_COUNT; i++) {
                JsonObject obj = array.createNestedObject();
                obj["led"] = i;
                obj["color"] = ledStates[i].color;
            }
            serializeJson(responseDoc, response);
            webSocket.sendTXT(num, response);
            Serial.println("📤 Enviando estado actualizado de todos los LEDs al cliente...");
            Serial.println(response);
        } else if (action == "updateLed") {
            // Cambiar el estado de un LED específico
            int led = doc["led"]; // Obtener el índice del LED
            String color = doc["color"].as<String>(); // Obtener el color

            // Verificar que el índice del LED esté dentro del rango válido
            if (led >= 0 && led < LED_COUNT) {
                ledStates[led].color = color; // Actualizar el estado del LED

                // Cambiar el color del LED específico
                if (color == "rojo") {
                    strip.setPixelColor(led, strip.Color(255, 0, 0)); // Rojo
                } else if (color == "verde") {
                    strip.setPixelColor(led, strip.Color(0, 255, 0)); // Verde
                } else {
                    Serial.println("Color no válido.");
                }

                strip.show(); // Actualizar la tira de LEDs
                saveState(); // Guardar el estado en SPIFFS

                // Enviar confirmación al cliente
                String response;
                StaticJsonDocument<200> responseDoc;
                responseDoc["action"] = "updateLed"; // Confirmar que el LED se actualizó
                responseDoc["led"] = led;
                responseDoc["color"] = color;
                serializeJson(responseDoc, response);
                webSocket.sendTXT(num, response);
                Serial.print("Actualizando LED ");
                Serial.print(led);
                Serial.print(" a color ");
                Serial.println(color);
                Serial.println(response);
            } else {
                Serial.println("Índice de LED fuera de rango.");
            }
        } else {
            Serial.println("Acción no válida.");
        }
    } else {
        Serial.println("Error al deserializar el mensaje JSON.");
    }
}

void onWebSocketEvent(uint8_t num, WStype_t type, uint8_t *payload, size_t length) {
    if (type == WStype_CONNECTED) {
        Serial.println("Cliente conectado");

        // Enviar el estado actual de los LEDs al cliente
        String json;
        StaticJsonDocument<JSON_SIZE> doc;
        doc["action"] = "updateState"; // Agregar el campo "action"
        JsonArray array = doc.createNestedArray("leds");
        for (int i = 0; i < LED_COUNT; i++) {
            JsonObject obj = array.createNestedObject();
            obj["led"] = i;
            obj["color"] = ledStates[i].color;
        }
        serializeJson(doc, json);
        webSocket.sendTXT(num, json);
    } else if (type == WStype_TEXT) {
        handleWebSocketMessage(num, payload, length);
    }
}

void initSPIFFS() {
    if (!SPIFFS.begin()) {  // Iniciar SPIFFS sin formatear
        Serial.println("❌ Error iniciando SPIFFS. Formateando...");
        SPIFFS.format();    // Formatear SPIFFS si no se puede montar
        if (SPIFFS.begin()) {
            Serial.println("✅ SPIFFS formateado y montado correctamente.");
        } else {
            Serial.println("❌ Error crítico: No se pudo montar SPIFFS.");
        }
    } else {
        Serial.println("✅ SPIFFS montado correctamente.");
    }
}

void recreateFile() {
    if (SPIFFS.exists("/leds.json")) {
        SPIFFS.remove("/leds.json");
        Serial.println("Archivo leds.json eliminado.");
    }

    createDefaultJson(); // Crear un nuevo archivo con valores predeterminados
}

bool isFileCorrupt() {
    if (!SPIFFS.exists("/leds.json")) {
        Serial.println("❌ El archivo leds.json no existe.");
        return true; // Si no existe, se considera corrupto
    }

    File file = SPIFFS.open("/leds.json", "r");
    if (!file) {
        Serial.println("No se pudo abrir el archivo para lectura.");
        return true; // Si no se puede abrir, se considera corrupto
    }

    // Intentar parsear el JSON
    StaticJsonDocument<JSON_SIZE> doc;
    DeserializationError error = deserializeJson(doc, file);
    file.close();

    if (error) {
        Serial.print("❌ Error al cargar JSON desde archivo: ");
        Serial.println(error.c_str());
        return true; // Si hay un error de parseo, el archivo está corrupto
    }

    // Verificar que el JSON tenga la estructura esperada
    if (!doc.containsKey("leds")) {
        Serial.println("❌ El archivo no tiene la clave 'leds'.");
        return true; // Si no tiene la clave esperada, está corrupto
    }

    Serial.println("✅ El archivo no está corrupto.");
    return false; // El archivo está bien
}

void setup() {
    Serial.begin(115200);
    delay(1000); // Espera para que el monitor serie se abra
    Serial.println("ESP8266 Iniciando...");

    // Conectar a WiFi
    WiFi.mode(WIFI_STA); // Modo estación (cliente)
    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED) {
        delay(1000);
        Serial.println("Conectando...");
    }
    Serial.println("Conectado a WiFi");

    // Iniciar mDNS
    if (!MDNS.begin("esp8266")) {
        Serial.println("Error iniciando mDNS");
    }

    // Iniciar SPIFFS
    initSPIFFS();

    if (!SPIFFS.exists("/leds.json") || isFileCorrupt()) {
        recreateFile();
    }

    // Inicializar la tira de LEDs con Adafruit NeoPixel
    strip.begin();
    strip.setBrightness(50); // Ajustar el brillo (0-255)
    strip.show(); // Inicializar todos los LEDs apagados

    // Cargar el estado inicial de los LEDs
    loadState();

    // Mostrar la dirección IP
    Serial.println(WiFi.localIP());

    // Iniciar WebSocket
    webSocket.begin();
    webSocket.onEvent(onWebSocketEvent);
    Serial.println("WebSocket iniciado en esp8266.local");
}

void loop() {
    webSocket.loop();
    MDNS.update(); // Actualizar mDNS
}