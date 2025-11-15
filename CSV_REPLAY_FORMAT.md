# Formato CSV de Replay - Wolfenstein 3D

## ¿Por qué CSV en vez de JSON?

El formato CSV es **mucho mejor** que JSON para este proyecto por varias razones:

1. **✅ Más simple de parsear en C++**: Usa `sscanf` en vez de parsing manual complejo
2. **✅ Ocupa menos espacio**: Un archivo CSV es ~10x más pequeño que JSON
3. **✅ Más rápido de cargar**: El parsing es casi instantáneo
4. **✅ Más fácil de debuggear**: Puedes abrir el CSV en Excel/LibreOffice
5. **✅ Formato perfecto para AI/ML**: Los modelos de ML trabajan nativamente con CSV

## Estructura del Archivo

```csv
# Wolfenstein 3D Replay v1.0
# Total Frames: 49
# Duration: 1.40 seconds
# Result: ongoing, Kills: 0, Final Health: 100
frame,time,posX,posY,dirX,dirY,planeX,planeY,health,ammo,kills,bullets,weapon,roll,vertPos,vertVel,fwd,back,left,right,rotL,rotR,shoot,jump,sprint,wpn
0,0.999,2.000,2.000,-1.000,0.000,0.000,0.660,100,50,0,0,0,0.000,0.000,0.000,0,0,0,0,0,0,0,0,0,0
1,1.012,2.000,2.000,-1.000,0.000,0.000,0.660,100,50,0,0,0,0.000,0.000,0.000,0,0,0,0,0,0,0,0,0,0
...
```

## Columnas del CSV

### Básicas
- `frame`: Número de frame
- `time`: Timestamp en segundos

### Posición y orientación del jugador
- `posX`, `posY`: Posición en el mapa
- `dirX`, `dirY`: Vector de dirección (adónde mira)
- `planeX`, `planeY`: Plano de cámara (para raycasting)

### Estado del jugador
- `health`: Salud (0-100)
- `ammo`: Munición disponible
- `kills`: Enemigos eliminados
- `bullets`: Balas disparadas
- `weapon`: Arma actual (0-7)

### Física avanzada
- `roll`: Inclinación de cámara
- `vertPos`: Posición vertical
- `vertVel`: Velocidad vertical

### Inputs (0 o 1 = false/true)
- `fwd`: Tecla adelante
- `back`: Tecla atrás
- `left`: Strafe izquierda
- `right`: Strafe derecha
- `rotL`: Rotar izquierda
- `rotR`: Rotar derecha
- `shoot`: Disparo
- `jump`: Salto
- `sprint`: Sprint
- `wpn`: Arma seleccionada

## Uso

### Exportar un replay (durante el juego)
1. Juega una partida
2. Presiona `U` para entrar en modo replay
3. Presiona `X` o click en el botón "Export"
4. Se descargará automáticamente un archivo `.csv`

### Importar un replay
1. En el menú principal, click en "VIEW REPLAY"
2. Selecciona un archivo `.csv`
3. Click en "PLAY" para ver el replay

## Ventajas para AI/ML

El formato CSV es ideal para entrenar modelos de AI porque:

```python
import pandas as pd

# Cargar datos
df = pd.read_csv('replay.csv', comment='#')

# Las columnas de input son perfectas para features
features = ['posX', 'posY', 'dirX', 'dirY', 'health', 'ammo']
actions = ['fwd', 'back', 'left', 'right', 'rotL', 'rotR', 'shoot']

X = df[features]
y = df[actions]

# ¡Listo para entrenar!
```

## Comparación de Tamaño

Para un replay de 1000 frames:
- **JSON**: ~500 KB
- **CSV**: ~50 KB (¡10x más pequeño!)

## Límites de Memoria

El juego está compilado con:
- **Memoria inicial**: 64 MB
- **Memoria máxima**: 2 GB
- **Capacidad**: ~40,000 frames (aprox. 20 minutos a 60 FPS)

Si necesitas replays más largos, puedes:
1. Exportar múltiples archivos CSV (el juego graba en circular buffer)
2. Reducir la tasa de muestreo (cada N frames)
3. Recompilar con más memoria

## Compatibilidad

El sistema detecta automáticamente el formato:
- ✅ CSV (recomendado)
- ⚠️  JSON (legacy, no soportado actualmente)

## Notas Técnicas

- El formato usa `,` como separador
- Las líneas que empiezan con `#` son comentarios
- Los valores booleanos son `0` (false) o `1` (true)
- Todos los números flotantes usan `.` como separador decimal
- La primera línea de datos es el header con nombres de columnas

