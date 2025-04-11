const express = require('express');
const path = require('path');

const app = express();
const port = 3000;

// Servir la carpeta build
app.use(express.static(path.join(__dirname, 'build')));

// Redirigir todas las rutas a index.html
app.get('*', (req, res) => {
    res.sendFile(path.join(__dirname, 'build', 'index.html'));
});

app.listen(port, () => {
    console.log(`Servidor corriendo en http://localhost:${port}`);
});