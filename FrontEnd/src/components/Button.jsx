import React, { useContext, useState, useEffect } from 'react';
import { WebSocketContext } from '../WebSocketContext';

const Button = ({ ledNumber }) => {
    const [isHovered, setIsHovered] = useState(false); // Estado para controlar el hover
    const { socket, ledStates, updateLedState } = useContext(WebSocketContext); // Obtener la conexión WebSocket y el estado de los LEDs

    // Obtener el estado del LED desde el contexto
    const ledState = ledStates[ledNumber] || "rojo"; // Estado inicial: rojo

    // Depurar el re-renderizado del botón
    useEffect(() => {
        console.log(`Botón ${ledNumber} re-renderizado con estado:`, ledState);
    }, [ledState, ledNumber]);

    // Función para cambiar el estado del LED
    const toggleLED = () => {
        if (!socket) {
            console.error("WebSocket no está conectado.");
            return;
        }
    
        const newState = ledState === "rojo" ? "verde" : "rojo";
        socket.send(JSON.stringify({ action: "updateLed", led: ledNumber, color: newState })); // Incluir el campo "action"
        updateLedState(ledNumber, newState); // Actualizar el estado en el contexto
    };

    // Estilos dinámicos para el botón
    const buttonStyle = {
        width: "40px",
        height: "40px",
        fontSize: "10px",
        cursor: "pointer",
        borderRadius: "50%",
        color: "white",
        backgroundColor: ledState === "verde" ? "green" : "red",
        transition: 'transform 0.2s ease-in-out, box-shadow 0.2s ease-in-out',
        transform: isHovered ? 'scale(1.1)' : 'scale(1)',
        border: '2px solid rgba(255, 255, 255, 0.3)', // Borde más sutil
        boxShadow: isHovered ? '0 4px 10px rgba(0, 0, 0, 0.3)' : '0 2px 5px rgba(0, 0, 0, 0.2)', // Sombra al hacer hover
    };

    return (
        <button
            style={buttonStyle}
            onClick={toggleLED}
            onMouseEnter={() => setIsHovered(true)} // Cambiar el estado al entrar el mouse
            onMouseLeave={() => setIsHovered(false)} // Cambiar el estado al salir el mouse
        >
            {ledNumber + 1}
        </button>
    );
};

export default Button;