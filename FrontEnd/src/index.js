import React from 'react';
import ReactDOM from 'react-dom/client';
import App from './App';
import './App.css';
import { WebSocketProvider } from './WebSocketContext';

const root = ReactDOM.createRoot(document.getElementById('root'));

root.render(
    <WebSocketProvider>
        <App />
    </WebSocketProvider>
);