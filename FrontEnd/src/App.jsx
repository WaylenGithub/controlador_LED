import React, { useContext } from 'react';
import { WebSocketProvider, WebSocketContext } from './WebSocketContext';
import Button from './components/Button';
import Loader from './components/Loader'; // Importar el componente Loader
import './App.css';

function App() {
    const leds = Array.from({ length: 144 }, (_, i) => i); // Crear un array con la cantidad de LEDS a elegir

    /* EN CASO DE NECESITAR FILAS Y COLUMNAS ESPECIFICAS ELIMINA EL WRAP DEL CSS EN .ROW Y MODIFICA ESTO */
    /*     const rows = 6; // Número de filas
    const columns = 24; // Número de columnas (botones por fila)


    // Divido el array en grupos de 24 (6 filas de 24 botones)
    const ledRows = [];
    for (let i = 0; i < rows; i++) {
        const start = i * columns;
        const end = start + columns;
        const row = leds.slice(start, end); 


        ledRows.push(row);
    } */

    const { socket, isConnected, error } = useContext(WebSocketContext); // Obtener el estado de la conexión y el error

    // Función para cambiar el estado de todos los LEDs
    const toggleAllLeds = (newState) => {
        // Mostrar una ventana de confirmación
        const confirmMessage = newState === "verde" 
            ? "¿Estás seguro de que quieres poner en VERDE todos los LEDs?" 
            : "¿Estás seguro de que quieres poner en ROJO los LEDs?";

        const isConfirmed = window.confirm(confirmMessage);

        // Si el usuario confirma, cambiar el estado de todos los LEDs
        if (isConfirmed && socket) {
            socket.send(JSON.stringify({ action: "setAll", color: newState }));
        }
    };

    // Mostrar el Loader o el mensaje de error si no hay conexión (si quieren corregir algo sin necesidad de conectarse al ESP8266 comenta esta seccion)
        if (!isConnected) {
            return <Loader error={error} />;
        } 

    // Mostrar el contenido principal si hay conexión
    return (
        <div className='container'>
            <div className="section-title">
                <h1>LEDS A CONTROLAR</h1>
            </div>
            <div className="control-buttons">
                <button onClick={() => toggleAllLeds("verde")}>Todos en verde</button>
                <button onClick={() => toggleAllLeds("rojo")}>Todos en rojo</button>
            </div>
            {/* {leds  .map((row, rowIndex) => (
                */} <div /* key={rowIndex} */ className="row"> {/* Parte de la logica de filas y columnas estrictas, reemplaza leds.map por rows.map en caso de necesitarlo */}
                {leds.map((ledNumber) => (
                    <Button key={ledNumber} ledNumber={ledNumber} />
                ))}
            </div>
    {/*                ))} */}
        </div>
    );
}

export default App;