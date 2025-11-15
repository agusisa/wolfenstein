# 🎮 Wolfenstein 3D - WebAssembly Edition

A modern reimplementation of the classic Wolfenstein 3D using raycasting, built with C++, SDL2, and compiled to WebAssembly using Emscripten.

![Version](https://img.shields.io/badge/version-2.0-blue)
![License](https://img.shields.io/badge/license-MIT-green)
![Platform](https://img.shields.io/badge/platform-WebAssembly-purple)

## 🌟 Features

### Gameplay
- **5 Weapons System**: Pistol, Machine Gun, Laser, Shotgun, and Bazooka with explosive projectiles
- **Enemy AI**: Smart soldiers that shoot from distance and aggressive German Shepherd dogs that chase and alert
- **Physics**: Jump mechanics with gravity
- **Item System**: Collectible ammo and health packs with dynamic lighting
- **Victory/Game Over Screens**: Complete with statistics and replay/menu options
- **Death Animations**: Enemies fall and stay visible as corpses with blood effects

### Advanced Features
- **Replay System**: Record and playback gameplay sessions with full controls (pause, speed, timeline)
- **Map Editor**: Isometric editor with rotation, multiple tools, and save/load functionality
- **Main Menu**: Professional UI with New Game, View Replay (drag & drop JSON), and Map Editor options
- **Lighting System**: Dynamic lighting with ambient light, static sources, player flashlight, and sector-based lighting
- **Shadow System**: Distance-based fog and projected shadows from light sources

### Graphics
- **Raycasting Engine**: Classic 3D rendering technique with modern enhancements
- **Dynamic Lighting**: Multiple light sources, shadows, and adjustable global luminosity
- **Procedural Textures**: Vintage brick walls and stone patterns
- **Z-Buffer**: Proper depth rendering
- **3D Weapon Models**: Half-Life style detailed weapons with realistic shading and effects
- **Enhanced Sprites**: Detailed enemy sprites (improved dog model with German Shepherd features)
- **Visual Effects**: Explosions, bullet trails, laser beams, muzzle flashes, and weapon-specific lighting
- **Sky Texture**: Animated ceiling
- **Death Effects**: Corpses flatten, darken, and show blood tint

### Audio
- **Procedural Sound Generation**: All sounds generated in real-time
- **Sound Effects**: Shooting, enemy death, item pickup, player damage, enemy alert sounds

### Controls
```
Movement:
  W/S or ↑/↓     - Move forward/backward
  A/D            - Strafe left/right
  ←/→            - Rotate camera
  Shift          - Sprint
  Space          - Jump

Combat:
  B or Ctrl      - Shoot
  Left Click     - Shoot
  1-5            - Switch weapons (1=Pistol, 2=Machinegun, 3=Laser, 4=Shotgun, 5=Bazooka)

Utilities:
  F              - Toggle flashlight
  L              - Adjust global light multiplier (in-game slider)
  ESC            - Return to main menu

Replay Mode:
  U              - Start replay mode (during gameplay)
  Space          - Pause/Resume
  ←/→            - Change playback speed
  Timeline Click - Jump to specific frame

Map Editor:
  Paint/Erase    - Click buttons to select tool
  Click on Grid  - Place/Remove elements
  Arrow Keys     - Rotate camera view
  Test           - Test the current map
  Save/Load      - Export/Import map as JSON

Main Menu:
  N              - New Game (keyboard shortcut)
  Click Buttons  - Navigate menu options
```

## 🚀 Quick Start

### Playing Online
Simply open `wolf3d.html` in a modern web browser with WebAssembly support.

### Running Locally

1. **Start a local server**:
```bash
python3 -m http.server 8000
```

2. **Open in browser**:
```
http://localhost:8000/wolf3d.html
```

## 🛠️ Building from Source

### Prerequisites
- Emscripten SDK
- SDL2 (provided by Emscripten)
- C++11 compiler

### Build Steps

1. **Install Emscripten**:
```bash
git clone https://github.com/emscripten-core/emsdk.git
cd emsdk
./emsdk install latest
./emsdk activate latest
source ./emsdk_env.sh
```

2. **Compile the project**:
```bash
em++ wolf3d_engine.cpp -o wolf3d.html \
  -s USE_SDL=2 \
  -s USE_SDL_MIXER=2 \
  -s WASM=1 \
  -O3 \
  -s ALLOW_MEMORY_GROWTH=1 \
  -s INITIAL_MEMORY=64MB \
  -s MAXIMUM_MEMORY=2GB \
  -s EXPORTED_FUNCTIONS='["_main","_loadReplayFromJSON","_setJsonFilename","_loadMapFromJSON"]' \
  -s EXPORTED_RUNTIME_METHODS='["ccall","cwrap"]' \
  -std=c++11
```

## 📁 Project Structure

```
wolfenstein/
├── wolf3d_engine.cpp       # Main game engine source code (~6,100+ lines)
├── wolf3d.wasm            # Compiled WebAssembly binary
├── wolf3d.js              # Emscripten runtime
├── wolf3d.html            # Game HTML wrapper
├── index.html             # Project landing page
├── run_server.sh          # Helper script to start server
├── README.md              # This file
├── REPLAY_SYSTEM_*.md     # Replay system documentation
├── MENU_SYSTEM.md         # Menu system documentation
└── CSV_REPLAY_FORMAT.md   # CSV replay format specification
```

## 🎯 Game Objectives

### Victory Condition
Eliminate all enemies to win! (Default: 1 soldier + 1 dog for testing, customizable via Map Editor)

### Tips
- **Weapon Selection**: 
  - Pistol (1): Basic weapon, unlimited ammo
  - Machinegun (2): Rapid fire, high ammo consumption
  - Laser (3): Instant beam weapon with red visual effects
  - Shotgun (4): Close-range powerful blasts
  - Bazooka (5): Explosive projectiles with area damage (can damage yourself!)
- Use the flashlight (F) in dark areas to see better
- Collect ammo and health items scattered throughout the map (they glow!)
- Keep your distance from dogs (they're fast and aggressive!)
- Strafe (A/D) to dodge enemy bullets
- Sprint (Shift) when retreating
- Adjust lighting with the slider (L) if it's too dark or bright
- Record your gameplay and analyze it with the replay system (U)

## 🏗️ Technical Details

### Raycasting Engine
The game uses a classic raycasting technique similar to the original Wolfenstein 3D:
- 2D grid-based map
- DDA algorithm for wall detection
- Vertical scanline rendering
- Sprite billboarding for enemies and items

### Modern Enhancements
- **SDL2 Integration**: Cross-platform graphics and audio
- **WebAssembly**: Runs natively in web browsers
- **Procedural Generation**: Textures and sounds generated at runtime
- **Advanced AI**: Enemies with different behaviors and alert sounds
- **Physics System**: Jump mechanics with gravity and vertical camera effects
- **Dynamic Lighting**: Multiple light types with shadows and fog
- **Replay Recording**: Frame-by-frame state capture for AI training
- **Map Editor**: Visual isometric editor with real-time testing
- **State Management**: Menu system with multiple game states

## 📊 Performance

- **Resolution**: 1080x720
- **FPS**: 60 (capped)
- **Map Size**: 40x40 tiles (customizable in editor)
- **WebAssembly Size**: ~1.2 MB
- **Memory**: Up to 2GB allocated for large replays
- **Lighting**: Real-time calculation for all surfaces with optimized algorithms

## 🔧 Development

### Architecture
- **Main Loop**: Emscripten event loop with game states
- **Game States**: Main Menu, Playing, Replay Viewer, Map Editor
- **Entity System**: Vectors for enemies, bullets, items, lights, explosions
- **Rendering**: SDL2 renderer with hardware acceleration
- **Lighting Engine**: Dynamic light calculation with shadows and fog
- **Replay System**: CSV-based recording with player state capture

### Code Organization
- **~6,100+ lines of C++**: Well-structured and modular
- **Game Systems**:
  - Raycasting engine with lighting
  - AI and pathfinding
  - Physics and collision
  - Weapon and combat systems
  - Replay recording/playback
  - Map editor with isometric rendering
  - UI system with custom bitmap fonts
- **Clear separation of concerns**: Input, update, render, and state management

## 📋 Version History

### Version 2.0 (Current) - Advanced Features Update
- ✨ **New**: Bazooka weapon with explosive projectiles and area damage
- ✨ **New**: Enemy death animations with corpses that stay visible
- ✨ **New**: Improved dog sprite (detailed German Shepherd model)
- ✨ **New**: Blood effects and darkening for dead enemies
- 🎨 **Enhanced**: 3D weapon models (Half-Life style)
- 🎨 **Enhanced**: Blue energy bullets with glow effects
- 🎨 **Enhanced**: Animated rotating laser beam with red trail
- 💡 **Enhanced**: Dynamic lighting for bullets, laser, and explosions
- 🔧 **Fixed**: Death animation flattening effect (enemies lie down)

### Version 1.5 - Lighting & Map Editor Update
- ✨ **New**: Dynamic lighting system with ambient, static, and flashlight sources
- ✨ **New**: Shadow system with distance fog and projected shadows
- ✨ **New**: Map editor with isometric view and rotation
- ✨ **New**: Global light multiplier control (UI slider)
- 💡 Item-specific lighting (health and ammo glow)
- 🎮 Improved jump effect consistency across all elements

### Version 1.0 - Replay System Update
- ✨ **New**: Replay recording and playback system
- ✨ **New**: Professional main menu with multiple options
- ✨ **New**: CSV-based replay format for AI training
- 🎮 Enhanced UI with custom bitmap fonts
- 🎮 Victory/Game Over screens with replay options

### Version 0.5 - Initial Release
- 🎮 Basic raycasting engine
- 🔫 4 weapons (Pistol, Machinegun, Laser, Shotgun)
- 👾 2 enemy types (Soldier and Dog)
- 🎯 Physics and collision system

## 🎨 Asset Attribution

This is a reimplementation inspired by the classic Wolfenstein 3D by id Software. All code is written from scratch using modern C++ and SDL2.

Original Wolfenstein 3D © 1992 id Software

## 📝 License

MIT License - See LICENSE file for details

## 🤝 Contributing

Contributions are welcome! Feel free to:
- Report bugs
- Suggest new features
- Submit pull requests
- Improve documentation

## 🙏 Acknowledgments

- id Software for the original Wolfenstein 3D
- Lode Vandevenne for raycasting tutorials
- Emscripten team for the amazing toolchain
- SDL2 team for the multimedia library

## 📧 Contact

For questions or feedback, please open an issue on GitHub.

---

**Made with ❤️ and lots of raycasting math**
