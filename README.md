# 🎮 Wolfenstein 3D - WebAssembly Edition

A modern reimplementation of the classic Wolfenstein 3D using raycasting, built with C++, SDL2, and compiled to WebAssembly using Emscripten.

![Version](https://img.shields.io/badge/version-1.0-blue)
![License](https://img.shields.io/badge/license-MIT-green)
![Platform](https://img.shields.io/badge/platform-WebAssembly-purple)

## 🌟 Features

### Gameplay
- **4 Weapons System**: Pistol, Machine Gun, Laser, and Shotgun
- **Enemy AI**: Smart soldiers that shoot from distance and aggressive dogs that chase
- **Physics**: Jump mechanics with gravity
- **Item System**: Collectible ammo and health packs
- **Victory/Game Over Screens**: Complete with statistics

### Graphics
- **Raycasting Engine**: Classic 3D rendering technique
- **Procedural Textures**: Vintage brick walls and stone patterns
- **Z-Buffer**: Proper depth rendering
- **Sprite System**: 3D-looking enemies with shading
- **Sky Texture**: Animated ceiling
- **Weapon Trails**: Visual bullet trajectories

### Audio
- **Procedural Sound Generation**: All sounds generated in real-time
- **Sound Effects**: Shooting, enemy death, item pickup, player damage

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
  1-4            - Switch weapons

Game:
  R              - Restart (on Game Over/Victory)
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
emcc -O3 wolf3d_engine.cpp -o wolf3d.html \
  -s USE_SDL=2 \
  -s USE_SDL_MIXER=2 \
  -s SDL2_MIXER_FORMATS='["ogg"]' \
  -s ALLOW_MEMORY_GROWTH=1 \
  -s EXPORTED_FUNCTIONS='["_main"]' \
  -s EXPORTED_RUNTIME_METHODS='["ccall"]' \
  -std=c++11
```

## 📁 Project Structure

```
wolfenstein/
├── wolf3d_engine.cpp       # Main game engine source code
├── wolf3d.wasm            # Compiled WebAssembly binary
├── wolf3d.js              # Emscripten runtime
├── wolf3d.html            # Game HTML wrapper
├── index.html             # Project landing page
├── run_server.sh          # Helper script to start server
└── README.md              # This file
```

## 🎯 Game Objectives

### Victory Condition
Eliminate all 15 enemies (8 soldiers + 7 dogs) to win!

### Tips
- Use the Laser (weapon 3) for instant kills
- Collect ammo and health items scattered throughout the map
- Keep your distance from dogs (they're fast!)
- Strafe (A/D) to dodge enemy bullets
- Sprint (Shift) when retreating

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
- **Advanced AI**: Enemies with different behaviors
- **Physics System**: Jump mechanics with gravity

## 📊 Performance

- **Resolution**: 1080x720
- **FPS**: 60 (capped)
- **Map Size**: 40x40 tiles
- **WebAssembly Size**: ~1.0 MB

## 🔧 Development

### Architecture
- **Main Loop**: Emscripten event loop
- **Game State**: Global state management
- **Entity System**: Vectors for enemies, bullets, items
- **Rendering**: SDL2 renderer with hardware acceleration

### Code Organization
- ~1,915 lines of C++
- Modular function design
- Clear separation of concerns (input, update, render)

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
