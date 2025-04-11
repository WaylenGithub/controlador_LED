import React, { createContext, useState, useEffect } from 'react';

export const WebSocketContext = createContext(null);

export const WebSocketProvider = ({ children }) => {
    const [socket, setSocket] = useState(null);
    const [ledStates, setLedStates] = useState({});
    const [isConnected, setIsConnected] = useState(false); // Estado para rastrear la conexión
    const [error, setError] = useState(null); // Estado para rastrear errores

    // Función para actualizar el estado de un LED
    const updateLedState = (ledNumber, color) => {
        setLedStates((prevStates) => ({
            ...prevStates,
            [ledNumber]: color,
        }));
    };

    useEffect(() => {
        // Crear la conexión WebSocket
        const ws = new WebSocket("ws://esp8266.local:81");

        // Manejar la apertura de la conexión
        ws.onopen = () => {
            console.log("Conectado a ESP8266");
            setSocket(ws);
            setIsConnected(true); // Marcar la conexión como activa
            setError(null); // Limpiar el estado de error

            // Solicitar el estado actual de los LEDs al conectarse
            ws.send(JSON.stringify({ action: "getState" }));
        };

        // Manejar mensajes entrantes
        ws.onmessage = (event) => {
            const message = JSON.parse(event.data);

            console.log("📩 Mensaje recibido:", event.data);

            if (message.action === "updateState") {
                // Transformar el array de LEDs en un objeto
                const ledMap = {};
                message.leds.forEach((led) => {
                    ledMap[led.led] = led.color === "null" ? "rojo" : led.color; // Convertir "null" a "rojo"
                });

                // Actualizar el estado de los LEDs
                setLedStates(ledMap);
                console.log("Estado actualizado:", ledMap);
            } else if (message.action === "updateLed") {
                // Actualizar el estado de un LED específico
                updateLedState(message.led, message.color);
            }
        };

        // Manejar errores de conexión
        ws.onerror = (error) => {
            console.error("Error en WebSocket:", error);
            setError({
                message: "Error de conexión",
                details: [
                    "Asegúrese de estar conectado al mismo router que el microcontrolador.",
                    "Red: TP-LINK_FE90", // Reemplazar con la red de su router
                    "Contraseña: 67992980",
                    "Asegúrese de que el microcontrolador esté encendido.",
                    "Recargue la página para volver a intentar la conexión."
                ]
            });
            setIsConnected(false); // Marcar la conexión como inactiva
        };

        // Manejar el cierre de la conexión
        ws.onclose = () => {
            console.log("Conexión WebSocket cerrada");
            setSocket(null);
            setIsConnected(false); // Marcar la conexión como inactiva
        };

        // Cerrar la conexión al desmontar el componente
        return () => {
            ws.close();
        };
    }, []);

    return (
        <WebSocketContext.Provider value={{ socket, ledStates, updateLedState, isConnected, error }}>
            {children}
        </WebSocketContext.Provider>
    );
};