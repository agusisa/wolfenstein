# Sistema de Menú Principal

## Descripción

El juego ahora incluye un menú principal con opciones para iniciar una nueva partida o visualizar replays previamente grabados desde archivos JSON.

## Estados del Juego

El motor ahora maneja tres estados principales:

```cpp
enum GameState {
    STATE_MAIN_MENU,      // Menú principal
    STATE_PLAYING,        // Jugando normalmente
    STATE_REPLAY_VIEWER   // Visualizando replay cargado desde JSON
};
```

## Características del Menú

### 1. Nueva Partida
- Botón verde en el menú principal
- Al clickearlo, inicia una nueva partida reseteando todos los valores del juego
- El juego comienza a grabar automáticamente para el sistema de replay

### 2. Visualizar Partida (View Replay)
- Botón azul en el menú principal
- Al clickearlo, se abre un file picker nativo del navegador
- Permite seleccionar archivos JSON exportados previamente

#### Flujo de Carga de Replay:

1. Click en "View Replay"
2. Se abre el file picker del navegador
3. Selecciona un archivo JSON de replay
4. El sistema valida el archivo:
   - Verifica que contenga las secciones "metadata" y "frames"
   - Cuenta el número de frames disponibles
5. Si es válido:
   - Aparece una zona con el nombre del archivo
   - Se muestra un botón "PLAY" púrpura
6. Click en "PLAY" inicia la reproducción del replay

### 3. Zona de Drop (Drag & Drop)
- Cuando se hace click en "View Replay", se expande una zona de drop
- Muestra instrucciones visuales con borde punteado
- Indica si el archivo cargado es válido (verde) o inválido (rojo)

## Integración con JavaScript

El sistema utiliza funciones exportadas de C++ que son accesibles desde JavaScript:

```cpp
extern "C" {
    void EMSCRIPTEN_KEEPALIVE loadReplayFromJSON(const char* jsonData);
    void EMSCRIPTEN_KEEPALIVE setJsonFilename(const char* filename);
}
```

Estas funciones permiten:
- Cargar el contenido del archivo JSON desde el navegador
- Validar el formato del replay
- Establecer el nombre del archivo para mostrar en la UI

## Navegación

### Desde el Menú:
- **Nueva Partida** → Estado `STATE_PLAYING`
- **View Replay** → File picker → PLAY → Estado `STATE_REPLAY_VIEWER`

### Desde el Juego (STATE_PLAYING):
- Presionar **ESC** durante "GAME OVER" o "VICTORY" → Vuelve al menú
- Presionar **U** → Inicia replay del juego actual (sin cambiar estado)
- Presionar **ESC** durante replay → Vuelve al juego

### Desde el Visor de Replay (STATE_REPLAY_VIEWER):
- Presionar **ESC** → Vuelve al menú principal

## Validación de JSON

El sistema realiza una validación básica del archivo JSON:

1. Verifica que el JSON contenga las secciones requeridas:
   - `"metadata"`: información sobre el replay
   - `"frames"`: array de frames grabados

2. Cuenta el número de frames disponibles

3. Si falla la validación, muestra un error en consola y marca el archivo como inválido

## Formato del JSON Esperado

```json
{
  "metadata": {
    "total_frames": 1000,
    "duration_seconds": 16.67,
    "result": "victory",
    "final_kills": 15,
    "final_health": 85,
    "accuracy": 0.7500
  },
  "frames": [
    {
      "frame": 0,
      "timestamp": 0.000,
      "player": {
        "pos": [2.0, 2.0],
        "dir": [-1.0, 0.0],
        "health": 100,
        "ammo": 50,
        "weapon": 1,
        "kills": 0
      },
      "input": {
        "forward": false,
        "backward": false,
        // ... más inputs
      },
      "enemies": [/* ... */],
      "bullets_count": 0,
      "enemy_bullets_count": 0
    }
    // ... más frames
  ]
}
```

## Futuras Mejoras

- Implementar parsing completo del JSON para cargar todos los frames
- Agregar preview del replay antes de reproducirlo
- Mostrar estadísticas del replay en el menú
- Agregar lista de replays guardados
- Implementar drag & drop visual real (actualmente solo hay file picker)

## Compilación

Para compilar con el nuevo sistema de menú:

```bash
em++ wolf3d_engine.cpp -o wolf3d.html \
  -s USE_SDL=2 \
  -s USE_SDL_MIXER=2 \
  -s ALLOW_MEMORY_GROWTH=1 \
  -s EXPORTED_FUNCTIONS='["_main", "_loadReplayFromJSON", "_setJsonFilename"]' \
  -s EXPORTED_RUNTIME_METHODS='["ccall", "cwrap"]' \
  -O2
```

Es importante incluir las funciones exportadas en `EXPORTED_FUNCTIONS` para que sean accesibles desde JavaScript.

## Notas Técnicas

- El sistema de menú renderiza usando SDL2 con primitivas gráficas (rectángulos)
- Los textos se simulan con rectángulos para mantener la simplicidad
- El menú se dibuja en el mismo canvas que el juego
- La transición entre estados es instantánea sin animaciones

