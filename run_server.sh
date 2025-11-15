#!/bin/bash
# Servidor web simple para ejecutar Wolfenstein 3D WebAssembly

echo "======================================"
echo "  Wolfenstein 3D - WebAssembly Port"
echo "======================================"
echo ""
echo "Iniciando servidor web en el puerto 8000..."
echo "Abre tu navegador en: http://localhost:8000/index.html"
echo ""
echo "Presiona Ctrl+C para detener el servidor"
echo ""

# Intentar con Python 3
if command -v python3 &> /dev/null; then
    python3 -m http.server 8000
# Intentar con Python 2
elif command -v python &> /dev/null; then
    python -m SimpleHTTPServer 8000
# Intentar con Node.js
elif command -v npx &> /dev/null; then
    npx http-server -p 8000
else
    echo "Error: No se encontró Python o Node.js"
    echo "Por favor instala uno de estos:"
    echo "  - Python 3: https://www.python.org/"
    echo "  - Node.js: https://nodejs.org/"
    exit 1
fi

