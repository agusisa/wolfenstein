// Wolfenstein 3D Engine with Weapons, Shooting, and Enemies
// Enhanced version with gameplay mechanics and sound
#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>
#include <emscripten.h>
#include <cmath>
#include <cstdio>
#include <vector>
#include <cstdlib>
#include <ctime>

const int SCREEN_WIDTH = 1080;
const int SCREEN_HEIGHT = 720;
const int MAP_WIDTH = 40;
const int MAP_HEIGHT = 40;

// World map - LARGER 40x40 map with more rooms
int worldMap[MAP_WIDTH][MAP_HEIGHT] = {
    {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
    {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
    {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
    {1,0,0,0,0,0,2,2,2,2,2,0,0,0,0,3,0,3,0,3,0,0,0,0,0,4,4,4,4,0,0,0,0,0,0,0,0,0,0,1},
    {1,0,0,0,0,0,2,0,0,0,2,0,0,0,0,0,0,0,0,0,0,0,0,0,0,4,0,0,4,0,0,0,0,0,0,0,0,0,0,1},
    {1,0,0,0,0,0,2,0,0,0,2,0,0,0,0,3,0,0,0,3,0,0,0,0,0,4,0,0,4,0,0,0,0,0,0,0,0,0,0,1},
    {1,0,0,0,0,0,2,0,0,0,2,0,0,0,0,0,0,0,0,0,0,0,0,0,0,4,0,0,4,0,0,0,0,0,0,0,0,0,0,1},
    {1,0,0,0,0,0,2,2,0,2,2,0,0,0,0,3,0,3,0,3,0,0,0,0,0,4,4,4,4,0,0,0,0,0,0,0,0,0,0,1},
    {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
    {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
    {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,2,2,2,2,2,0,0,0,0,0,0,0,0,0,0,0,0,1},
    {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,2,0,0,0,2,0,0,0,0,0,0,0,0,0,0,0,0,1},
    {1,0,0,0,0,0,0,0,0,0,0,0,3,3,3,3,3,0,0,0,0,0,2,0,0,0,2,0,0,0,0,0,0,0,0,0,0,0,0,1},
    {1,0,0,0,0,0,0,0,0,0,0,0,3,0,0,0,3,0,0,0,0,0,2,0,0,0,2,0,0,0,0,0,0,0,0,0,0,0,0,1},
    {1,0,0,0,0,0,0,0,0,0,0,0,3,0,0,0,3,0,0,0,0,0,2,2,0,2,2,0,0,0,0,0,0,0,0,0,0,0,0,1},
    {1,0,0,0,0,0,0,0,0,0,0,0,3,3,0,3,3,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
    {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
    {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
    {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
    {1,4,4,4,4,4,4,4,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
    {1,4,0,0,0,0,0,0,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
    {1,4,0,0,0,0,0,0,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,4,4,4,4,4,0,0,0,0,1},
    {1,4,0,0,0,0,0,0,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,4,0,0,0,4,0,0,0,0,1},
    {1,4,4,4,0,4,4,4,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,4,0,0,0,4,0,0,0,0,1},
    {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,4,0,0,0,4,0,0,0,0,1},
    {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,3,3,3,0,0,0,0,0,4,4,0,4,4,0,0,0,0,1},
    {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,3,0,3,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
    {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,3,0,3,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
    {1,0,0,0,0,0,0,0,0,0,0,2,2,2,2,2,0,0,0,0,0,0,3,0,3,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
    {1,0,0,0,0,0,0,0,0,0,0,2,0,0,0,2,0,0,0,0,0,0,3,3,3,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
    {1,0,0,0,0,0,0,0,0,0,0,2,0,0,0,2,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
    {1,0,0,0,0,0,0,0,0,0,0,2,0,0,0,2,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
    {1,0,0,0,0,0,0,0,0,0,0,2,2,0,2,2,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
    {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
    {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
    {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
    {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
    {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
    {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1}
};

// Enemy structure
enum EnemyType { SOLDIER = 0, DOG = 1 };
struct Enemy {
    double x, y;
    int health;
    bool alive;
    double moveTimer;
    int animFrame;
    EnemyType type;
    double shootTimer;
    double shootCooldown;
    double fallOffset; // Para animación de caída al morir
    
    Enemy(double px, double py, EnemyType t = SOLDIER) : x(px), y(py), health(100), alive(true), 
          moveTimer(0), animFrame(0), type(t), shootTimer(0), shootCooldown(4.0), fallOffset(0) {}
};

// Player and game state
double posX = 20, posY = 20;
double dirX = -1, dirY = 0;
double planeX = 0, planeY = 0.66;
int playerHealth = 100;
int ammo = 500;
int kills = 0;
int totalEnemies = 0; // Se inicializa en initEnemies()
int bulletsFired = 0; // Contador de balas disparadas
bool gameOver = false;
bool gameWon = false;

// Weapon system
enum WeaponType { PISTOL = 0, MACHINEGUN = 1, LASER = 2, SHOTGUN = 3 };
int currentWeapon = PISTOL;
int weaponState = 0; // 0 = idle, 1 = shooting
int weaponFrame = 0;
int weaponTimer = 0;
bool laserActive = false;
double cameraRoll = 0.0; // Inclinación de cámara al moverse lateralmente

// Sistema de salto
double verticalPosition = 0.0; // Posición vertical (para el salto)
double verticalVelocity = 0.0; // Velocidad vertical
bool isJumping = false;
const double GRAVITY = 0.008;  // Gravedad más lenta
const double JUMP_FORCE = 0.20; // Salto más lento y bajo

// Control del mouse
bool mouseLocked = false;
int lastMouseX = SCREEN_WIDTH / 2;
int lastMouseY = SCREEN_HEIGHT / 2;

// Láser con estela
struct LaserBeam {
    double x, y;
    double dirX, dirY;
    int lifetime;
    bool active;
    std::vector<std::pair<double, double>> trail;
    
    LaserBeam() : x(0), y(0), dirX(0), dirY(0), lifetime(30), active(false) {}
};
std::vector<LaserBeam> laserBeams;

// Physical bullet system
struct Bullet {
    double x, y;
    double dirX, dirY;
    int lifetime;
    bool active;
    bool isEnemy; // true si es bala enemiga
    std::vector<std::pair<double, double>> trail; // Estela de posiciones
    
    Bullet() : x(0), y(0), dirX(0), dirY(0), lifetime(0), active(false), isEnemy(false) {}
};

std::vector<Bullet> bullets;
std::vector<Bullet> enemyBullets;

// Enemies
std::vector<Enemy> enemies;

// Item system
enum ItemType { ITEM_AMMO = 0, ITEM_HEALTH = 1 };
struct Item {
    double x, y;
    ItemType type;
    bool active;
    int lightIndex; // Index of the light source for this item
    
    Item(double px, double py, ItemType t) : x(px), y(py), type(t), active(true), lightIndex(-1) {}
};

std::vector<Item> items;

// ============================================================================
// REPLAY SYSTEM - Frame-by-Frame Recording for AI Training
// ============================================================================

// Input state structure (critical for AI training)
struct InputState {
    bool forward, backward, strafeLeft, strafeRight;
    bool rotateLeft, rotateRight;
    bool shoot, jump, sprint;
    int weaponSelected; // 0-3
    
    InputState() : forward(false), backward(false), strafeLeft(false), strafeRight(false),
                   rotateLeft(false), rotateRight(false), shoot(false), jump(false), 
                   sprint(false), weaponSelected(0) {}
};

// Snapshot structures
struct EnemySnapshot {
    double x, y;
    int health;
    bool alive;
    int type; // SOLDIER or DOG
    double fallOffset;
    
    EnemySnapshot() : x(0), y(0), health(0), alive(false), type(0), fallOffset(0) {}
    EnemySnapshot(const Enemy& e) : x(e.x), y(e.y), health(e.health), alive(e.alive), 
                                     type(e.type), fallOffset(e.fallOffset) {}
};

struct BulletSnapshot {
    double x, y, dirX, dirY;
    int lifetime;
    bool isEnemy;
    
    BulletSnapshot() : x(0), y(0), dirX(0), dirY(0), lifetime(0), isEnemy(false) {}
    BulletSnapshot(const Bullet& b) : x(b.x), y(b.y), dirX(b.dirX), dirY(b.dirY), 
                                       lifetime(b.lifetime), isEnemy(b.isEnemy) {}
};

struct ItemSnapshot {
    double x, y;
    int type; // ITEM_AMMO or ITEM_HEALTH
    bool active;
    
    ItemSnapshot() : x(0), y(0), type(0), active(false) {}
    ItemSnapshot(const Item& i) : x(i.x), y(i.y), type(i.type), active(i.active) {}
};

struct LaserBeamSnapshot {
    double x, y, dirX, dirY;
    int lifetime;
    
    LaserBeamSnapshot() : x(0), y(0), dirX(0), dirY(0), lifetime(0) {}
    LaserBeamSnapshot(const LaserBeam& l) : x(l.x), y(l.y), dirX(l.dirX), dirY(l.dirY), 
                                             lifetime(l.lifetime) {}
};

// Complete game state snapshot
struct GameStateSnapshot {
    // Player state
    double posX, posY, dirX, dirY, planeX, planeY;
    int playerHealth, ammo, kills, bulletsFired;
    int currentWeapon;
    double cameraRoll, verticalPosition, verticalVelocity;
    bool isJumping, laserActive;
    
    // Input state (critical for AI)
    InputState input;
    
    // Game entities
    std::vector<EnemySnapshot> enemies;
    std::vector<BulletSnapshot> bullets;
    std::vector<BulletSnapshot> enemyBullets;
    std::vector<ItemSnapshot> items;
    std::vector<LaserBeamSnapshot> laserBeams;
    
    // Game state
    bool gameOver, gameWon;
    double timestamp;
    int frameNumber;
    
    GameStateSnapshot() : posX(0), posY(0), dirX(0), dirY(0), planeX(0), planeY(0),
                          playerHealth(0), ammo(0), kills(0), bulletsFired(0),
                          currentWeapon(0), cameraRoll(0), verticalPosition(0), verticalVelocity(0),
                          isJumping(false), laserActive(false), gameOver(false), gameWon(false),
                          timestamp(0), frameNumber(0) {}
};

// Replay system variables
std::vector<GameStateSnapshot> recordedFrames;
bool isRecording = true; // Always recording during gameplay
int currentFrameNumber = 0;
const int MAX_RECORDED_FRAMES = 36000; // 10 minutes @ 60 FPS
InputState currentInput; // Current input state

// Replay playback variables
bool isReplaying = false;
int replayCurrentFrame = 0;
float replaySpeed = 1.0f; // 1.0 = normal, 0.5 = slow, 2.0 = fast
bool replayPaused = false;
float replayFrameAccumulator = 0.0f; // For variable speed playback

// Menu system
enum GameState {
    STATE_MAIN_MENU,
    STATE_PLAYING,
    STATE_REPLAY_VIEWER
};

GameState currentGameState = STATE_MAIN_MENU;
bool jsonFileLoaded = false;
bool jsonFileValid = false;
std::string loadedJsonFilename = "";
bool showFileDropZone = false;

SDL_Window* window = nullptr;
SDL_Renderer* renderer = nullptr;
bool running = true;

// Z-buffer for depth testing (para que enemigos y balas no se vean detrás de paredes)
double zBuffer[SCREEN_WIDTH];

// ============================================================================
// DYNAMIC LIGHTING SYSTEM
// ============================================================================

struct LightSource {
    double x, y;           // Position in map
    float radius;          // Light range
    int r, g, b;           // Light color
    float intensity;       // 0.0 to 1.0
    bool active;
};

struct SectorLight {
    int minX, minY, maxX, maxY;  // Sector bounds
    float brightness;             // 0.0 (dark) to 1.0 (bright)
};

// Global lighting variables
std::vector<LightSource> lightSources;
std::vector<SectorLight> sectors;
float ambientLight = 0.15f;        // Base darkness (15% illumination)
bool flashlightEnabled = false;
float flashlightRadius = 8.0f;
float flashlightIntensity = 1.0f;
float globalLightMultiplier = 1.0f; // Slider control for all lights (0.0 to 2.0)
bool sliderDragging = false;       // Track if user is dragging the light slider

// Sound system
Mix_Chunk* shootSound = nullptr;
Mix_Chunk* enemyDeathSound = nullptr;
Mix_Chunk* playerHitSound = nullptr;
Mix_Chunk* pickupAmmoSound = nullptr;
Mix_Chunk* pickupHealthSound = nullptr;

// Generar sonidos procedurales simples
void generateSounds() {
    // Sonido de disparo (beep corto)
    const int shootFreq = 22050;
    const int shootSamples = shootFreq / 20; // 50ms
    Sint16* shootBuffer = new Sint16[shootSamples];
    for(int i = 0; i < shootSamples; i++) {
        double t = (double)i / shootFreq;
        shootBuffer[i] = (Sint16)(sin(2 * M_PI * 800 * t) * 8000 * exp(-t * 10));
    }
    shootSound = Mix_QuickLoad_RAW((Uint8*)shootBuffer, shootSamples * 2);
    
    // Sonido de muerte del enemigo (explosión)
    const int deathSamples = shootFreq / 5; // 200ms
    Sint16* deathBuffer = new Sint16[deathSamples];
    for(int i = 0; i < deathSamples; i++) {
        double t = (double)i / shootFreq;
        deathBuffer[i] = (Sint16)((rand() % 16000 - 8000) * exp(-t * 5));
    }
    enemyDeathSound = Mix_QuickLoad_RAW((Uint8*)deathBuffer, deathSamples * 2);
    
    // Sonido de golpe al jugador (impacto)
    const int hitSamples = shootFreq / 15; // ~67ms
    Sint16* hitBuffer = new Sint16[hitSamples];
    for(int i = 0; i < hitSamples; i++) {
        double t = (double)i / shootFreq;
        hitBuffer[i] = (Sint16)(sin(2 * M_PI * 200 * t) * 12000 * exp(-t * 15));
    }
    playerHitSound = Mix_QuickLoad_RAW((Uint8*)hitBuffer, hitSamples * 2);
    
    // Sonido de recoger munición (ding alto)
    const int pickupAmmoSamples = shootFreq / 10; // 100ms
    Sint16* pickupAmmoBuffer = new Sint16[pickupAmmoSamples];
    for(int i = 0; i < pickupAmmoSamples; i++) {
        double t = (double)i / shootFreq;
        pickupAmmoBuffer[i] = (Sint16)(sin(2 * M_PI * 1200 * t) * 10000 * exp(-t * 8));
    }
    pickupAmmoSound = Mix_QuickLoad_RAW((Uint8*)pickupAmmoBuffer, pickupAmmoSamples * 2);
    
    // Sonido de recoger salud (tono agradable)
    const int pickupHealthSamples = shootFreq / 8; // 125ms
    Sint16* pickupHealthBuffer = new Sint16[pickupHealthSamples];
    for(int i = 0; i < pickupHealthSamples; i++) {
        double t = (double)i / shootFreq;
        double freq = 600 + sin(t * 20) * 100; // Vibrato
        pickupHealthBuffer[i] = (Sint16)(sin(2 * M_PI * freq * t) * 8000 * exp(-t * 6));
    }
    pickupHealthSound = Mix_QuickLoad_RAW((Uint8*)pickupHealthBuffer, pickupHealthSamples * 2);
}

// ============================================================================
// REPLAY SYSTEM FUNCTIONS
// ============================================================================

// Capture current game state into a snapshot
GameStateSnapshot captureCurrentState(double timestamp) {
    GameStateSnapshot snapshot;
    
    // Player state
    snapshot.posX = posX;
    snapshot.posY = posY;
    snapshot.dirX = dirX;
    snapshot.dirY = dirY;
    snapshot.planeX = planeX;
    snapshot.planeY = planeY;
    snapshot.playerHealth = playerHealth;
    snapshot.ammo = ammo;
    snapshot.kills = kills;
    snapshot.bulletsFired = bulletsFired;
    snapshot.currentWeapon = currentWeapon;
    snapshot.cameraRoll = cameraRoll;
    snapshot.verticalPosition = verticalPosition;
    snapshot.verticalVelocity = verticalVelocity;
    snapshot.isJumping = isJumping;
    snapshot.laserActive = laserActive;
    
    // Input state (current inputs)
    snapshot.input = currentInput;
    
    // Enemies
    snapshot.enemies.clear();
    for(const auto& enemy : enemies) {
        snapshot.enemies.push_back(EnemySnapshot(enemy));
    }
    
    // Bullets
    snapshot.bullets.clear();
    for(const auto& bullet : bullets) {
        if(bullet.active) {
            snapshot.bullets.push_back(BulletSnapshot(bullet));
        }
    }
    
    snapshot.enemyBullets.clear();
    for(const auto& bullet : enemyBullets) {
        if(bullet.active) {
            snapshot.enemyBullets.push_back(BulletSnapshot(bullet));
        }
    }
    
    // Items
    snapshot.items.clear();
    for(const auto& item : items) {
        snapshot.items.push_back(ItemSnapshot(item));
    }
    
    // Laser beams
    snapshot.laserBeams.clear();
    for(const auto& beam : laserBeams) {
        if(beam.active) {
            snapshot.laserBeams.push_back(LaserBeamSnapshot(beam));
        }
    }
    
    // Game state
    snapshot.gameOver = gameOver;
    snapshot.gameWon = gameWon;
    snapshot.timestamp = timestamp;
    snapshot.frameNumber = currentFrameNumber;
    
    return snapshot;
}

// Record current frame
void recordFrame(double timestamp) {
    if(!isRecording || isReplaying) return;
    
    // Check if we've reached the maximum
    if(recordedFrames.size() >= MAX_RECORDED_FRAMES) {
        // Remove oldest frame (circular buffer)
        recordedFrames.erase(recordedFrames.begin());
    }
    
    // Capture and store current state
    GameStateSnapshot snapshot = captureCurrentState(timestamp);
    recordedFrames.push_back(snapshot);
    currentFrameNumber++;
    
    // Print first frame recorded
    if(currentFrameNumber == 1) {
        printf(">>> First frame recorded! Recording active.\n");
    }
    
    // Optional: Print progress every 600 frames (10 seconds @ 60 FPS)
    if(currentFrameNumber % 600 == 0) {
        printf("Recording: %d frames (%.1f seconds)\n", currentFrameNumber, timestamp);
    }
}

// Clear recording (when restarting game)
void clearRecording() {
    recordedFrames.clear();
    currentFrameNumber = 0;
    isRecording = true;
    printf("Recording cleared. Ready to record new session.\n");
}

// Apply a snapshot to restore game state
void applyGameState(const GameStateSnapshot& snapshot) {
    // Player state
    posX = snapshot.posX;
    posY = snapshot.posY;
    dirX = snapshot.dirX;
    dirY = snapshot.dirY;
    planeX = snapshot.planeX;
    planeY = snapshot.planeY;
    playerHealth = snapshot.playerHealth;
    ammo = snapshot.ammo;
    kills = snapshot.kills;
    bulletsFired = snapshot.bulletsFired;
    currentWeapon = snapshot.currentWeapon;
    cameraRoll = snapshot.cameraRoll;
    verticalPosition = snapshot.verticalPosition;
    verticalVelocity = snapshot.verticalVelocity;
    isJumping = snapshot.isJumping;
    laserActive = snapshot.laserActive;
    
    // Enemies
    enemies.clear();
    for(const auto& enemySnap : snapshot.enemies) {
        Enemy enemy(enemySnap.x, enemySnap.y, (EnemyType)enemySnap.type);
        enemy.health = enemySnap.health;
        enemy.alive = enemySnap.alive;
        enemy.fallOffset = enemySnap.fallOffset;
        enemies.push_back(enemy);
    }
    
    // Bullets
    bullets.clear();
    for(const auto& bulletSnap : snapshot.bullets) {
        Bullet bullet;
        bullet.x = bulletSnap.x;
        bullet.y = bulletSnap.y;
        bullet.dirX = bulletSnap.dirX;
        bullet.dirY = bulletSnap.dirY;
        bullet.lifetime = bulletSnap.lifetime;
        bullet.active = true;
        bullet.isEnemy = bulletSnap.isEnemy;
        bullets.push_back(bullet);
    }
    
    enemyBullets.clear();
    for(const auto& bulletSnap : snapshot.enemyBullets) {
        Bullet bullet;
        bullet.x = bulletSnap.x;
        bullet.y = bulletSnap.y;
        bullet.dirX = bulletSnap.dirX;
        bullet.dirY = bulletSnap.dirY;
        bullet.lifetime = bulletSnap.lifetime;
        bullet.active = true;
        bullet.isEnemy = bulletSnap.isEnemy;
        enemyBullets.push_back(bullet);
    }
    
    // Items
    items.clear();
    for(const auto& itemSnap : snapshot.items) {
        Item item(itemSnap.x, itemSnap.y, (ItemType)itemSnap.type);
        item.active = itemSnap.active;
        items.push_back(item);
    }
    
    // Laser beams
    laserBeams.clear();
    for(const auto& beamSnap : snapshot.laserBeams) {
        LaserBeam beam;
        beam.x = beamSnap.x;
        beam.y = beamSnap.y;
        beam.dirX = beamSnap.dirX;
        beam.dirY = beamSnap.dirY;
        beam.lifetime = beamSnap.lifetime;
        beam.active = true;
        laserBeams.push_back(beam);
    }
    
    // Game state
    gameOver = snapshot.gameOver;
    gameWon = snapshot.gameWon;
}

// Start replay mode
void startReplay() {
    printf(">>> startReplay() called. Frames: %d, isRecording: %d\n", (int)recordedFrames.size(), isRecording);
    
    if(recordedFrames.empty()) {
        printf(">>> ERROR: No frames recorded yet!\n");
        return;
    }
    
    isReplaying = true;
    replayCurrentFrame = 0;
    replayPaused = false;
    replaySpeed = 1.0f;
    replayFrameAccumulator = 0.0f;
    
    // Apply first frame
    applyGameState(recordedFrames[0]);
    
    printf(">>> REPLAY MODE: Started (%d frames recorded)\n", (int)recordedFrames.size());
    printf(">>> Controls: SPACE=pause, ←/→=frame step, Q/E=skip, -/+=speed, R=restart, X=export, ESC=exit\n");
}

// Exit replay mode and return to gameplay
void exitReplay() {
    if(!isReplaying) return;
    
    isReplaying = false;
    replayPaused = false;
    
    // If viewing a loaded replay, return to menu
    if(currentGameState == STATE_REPLAY_VIEWER) {
        currentGameState = STATE_MAIN_MENU;
        printf("REPLAY MODE: Exited. Returning to main menu.\n");
        return;
    }
    
    // Otherwise, restore to last recorded frame (continue gameplay)
    if(!recordedFrames.empty()) {
        applyGameState(recordedFrames.back());
    }
    
    printf("REPLAY MODE: Exited. Returning to gameplay.\n");
}

// Export replay to CSV (much simpler and faster than JSON!)
void exportReplayToCSV() {
    if(recordedFrames.empty()) {
        printf("ERROR: No frames to export!\n");
        return;
    }
    
    printf("Exporting replay to CSV... (%d frames)\n", (int)recordedFrames.size());
    
    std::string csv = "";
    char buffer[512];
    
    // Header with metadata
    csv += "# Wolfenstein 3D Replay - Simple format (player data only)\n";
    sprintf(buffer, "# Total Frames: %d\n", (int)recordedFrames.size());
    csv += buffer;
    sprintf(buffer, "# Duration: %.2f seconds\n", recordedFrames.back().timestamp);
    csv += buffer;
    const char* result = gameWon ? "victory" : (gameOver ? "defeat" : "ongoing");
    sprintf(buffer, "# Result: %s, Kills: %d, Final Health: %d\n", 
            result, recordedFrames.back().kills, recordedFrames.back().playerHealth);
    csv += buffer;
    
    // Column headers
    csv += "frame,time,posX,posY,dirX,dirY,planeX,planeY,health,ammo,kills,bullets,weapon,roll,vertPos,vertVel,fwd,back,left,right,rotL,rotR,shoot,jump,sprint,wpn\n";
    
    // Export player data only (one line per frame)
    for(size_t i = 0; i < recordedFrames.size(); i++) {
        const GameStateSnapshot& f = recordedFrames[i];
        
        sprintf(buffer, "%d,%.3f,%.3f,%.3f,%.3f,%.3f,%.3f,%.3f,%d,%d,%d,%d,%d,%.3f,%.3f,%.3f,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d\n",
                f.frameNumber, f.timestamp,
                f.posX, f.posY, f.dirX, f.dirY, f.planeX, f.planeY,
                f.playerHealth, f.ammo, f.kills, f.bulletsFired, f.currentWeapon,
                f.cameraRoll, f.verticalPosition, f.verticalVelocity,
                f.input.forward, f.input.backward, f.input.strafeLeft, f.input.strafeRight,
                f.input.rotateLeft, f.input.rotateRight, f.input.shoot, f.input.jump,
                f.input.sprint, f.input.weaponSelected);
        csv += buffer;
        
        // Progress feedback
        if((i + 1) % 1000 == 0) {
            printf("Exported %d/%d frames...\n", (int)(i + 1), (int)recordedFrames.size());
        }
    }
    
    printf("CSV export complete! Size: %d bytes, Frames: %d\n", 
           (int)csv.length(), (int)recordedFrames.size());
    
    // Download CSV file
    EM_ASM({
        var csvStr = UTF8ToString($0);
        var blob = new Blob([csvStr], {type: 'text/csv'});
        var url = URL.createObjectURL(blob);
        var a = document.createElement('a');
        a.href = url;
        a.download = 'wolfenstein_replay_' + Date.now() + '.csv';
        document.body.appendChild(a);
        a.click();
        document.body.removeChild(a);
        URL.revokeObjectURL(url);
        console.log('CSV Replay exported successfully!');
    }, csv.c_str());
}

// Legacy function - keep for compatibility
void exportReplayToJSON() {
    printf("JSON export is deprecated. Use CSV instead for better performance.\n");
    exportReplayToCSV();
}

// Parse a single CSV line - can be Player, Enemy, Bullet, Item, or Laser
// ParsedLine struct removed - using simple direct parsing now

// Simple CSV parser - player data only (STABLE VERSION)
bool parseCSVLine(const char* line, GameStateSnapshot& snapshot) {
    // Skip comments, empty lines, and header
    if(!line || line[0] == '#' || line[0] == '\n' || line[0] == '\0' || line[0] == 'f') {
        return false;
    }
    
    // Initialize with safe defaults
    snapshot.frameNumber = 0;
    snapshot.timestamp = 0.0;
    snapshot.posX = 5.0;
    snapshot.posY = 5.0;
    snapshot.dirX = -1.0;
    snapshot.dirY = 0.0;
    snapshot.planeX = 0.0;
    snapshot.planeY = 0.66;
    snapshot.playerHealth = 100;
    snapshot.ammo = 50;
    snapshot.kills = 0;
    snapshot.bulletsFired = 0;
    snapshot.currentWeapon = 1;
    snapshot.cameraRoll = 0.0;
    snapshot.verticalPosition = 0.0;
    snapshot.verticalVelocity = 0.0;
    
    // Parse the CSV line
    int fwd, back, left, right, rotL, rotR, shoot, jump, sprint;
    int numParsed = sscanf(line, "%d,%lf,%lf,%lf,%lf,%lf,%lf,%lf,%d,%d,%d,%d,%d,%lf,%lf,%lf,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d",
                       &snapshot.frameNumber, &snapshot.timestamp,
                       &snapshot.posX, &snapshot.posY,
                       &snapshot.dirX, &snapshot.dirY,
                       &snapshot.planeX, &snapshot.planeY,
                       &snapshot.playerHealth, &snapshot.ammo,
                       &snapshot.kills, &snapshot.bulletsFired,
                       &snapshot.currentWeapon,
                       &snapshot.cameraRoll, &snapshot.verticalPosition, &snapshot.verticalVelocity,
                       &fwd, &back, &left, &right,
                       &rotL, &rotR, &shoot, &jump,
                       &sprint, &snapshot.input.weaponSelected);
    
    // Convert int inputs to bools
    snapshot.input.forward = (fwd != 0);
    snapshot.input.backward = (back != 0);
    snapshot.input.strafeLeft = (left != 0);
    snapshot.input.strafeRight = (right != 0);
    snapshot.input.rotateLeft = (rotL != 0);
    snapshot.input.rotateRight = (rotR != 0);
    snapshot.input.shoot = (shoot != 0);
    snapshot.input.jump = (jump != 0);
    snapshot.input.sprint = (sprint != 0);
    
    // Clear entity vectors (not in simple format)
    snapshot.enemies.clear();
    snapshot.bullets.clear();
    snapshot.items.clear();
    snapshot.laserBeams.clear();
    
    // Valid if we parsed at least the core fields (frame, time, pos, dir)
    return (numParsed >= 8);
}

// createEmptySnapshot removed - initialization happens directly in parseCSVLine now

// Load replay from CSV - simple format (STABLE VERSION)
void loadReplayFromCSV(const char* csvData) {
    printf(">>> loadReplayFromCSV called (simple format - player only)\n");
    
    // Clear current recording
    recordedFrames.clear();
    currentFrameNumber = 0;
    
    // Reserve space
    recordedFrames.reserve(10000);
    
    // Parse line by line
    const char* lineStart = csvData;
    const char* lineEnd = csvData;
    char lineBuffer[512];
    int framesLoaded = 0;
    
    while(*lineStart != '\0') {
        // Find end of line
        lineEnd = lineStart;
        while(*lineEnd != '\n' && *lineEnd != '\0') lineEnd++;
        
        // Copy line to buffer
        int lineLen = lineEnd - lineStart;
        if(lineLen > 0 && lineLen < 512) {
            memcpy(lineBuffer, lineStart, lineLen);
            lineBuffer[lineLen] = '\0';
            
            // Parse this line
            GameStateSnapshot snapshot;
            if(parseCSVLine(lineBuffer, snapshot)) {
                recordedFrames.push_back(snapshot);
                framesLoaded++;
                
                if(framesLoaded % 1000 == 0) {
                    printf("Loaded %d frames...\n", framesLoaded);
                }
            }
        }
        
        // Move to next line
        lineStart = (*lineEnd == '\n') ? lineEnd + 1 : lineEnd;
    }
    
    if(!recordedFrames.empty()) {
        printf("✓ CSV loaded successfully: %d frames\n", (int)recordedFrames.size());
        printf("  Duration: %.2f seconds\n", recordedFrames.back().timestamp);
        printf("  Final health: %d, Kills: %d\n", 
               recordedFrames.back().playerHealth, recordedFrames.back().kills);
        jsonFileValid = true;
        jsonFileLoaded = true;
    } else {
        printf("ERROR: Failed to load CSV - no valid frames found\n");
        jsonFileValid = false;
        jsonFileLoaded = true;
    }
}

// Exported functions for JavaScript
extern "C" {
    // Main load function - detects format automatically
    void EMSCRIPTEN_KEEPALIVE loadReplayFromJSON(const char* data) {
        if(data == nullptr || strlen(data) < 10) {
            printf("ERROR: Invalid replay data\n");
            jsonFileValid = false;
            jsonFileLoaded = true;
            return;
        }
        
        // Auto-detect format
        std::string str(data, std::min(strlen(data), (size_t)100));
        
        if(str.find("# Wolfenstein") != std::string::npos || str.find("frame,time") != std::string::npos) {
            printf(">>> Detected CSV format\n");
            loadReplayFromCSV(data);
        } else {
            printf("ERROR: Unknown replay format\n");
            printf("Please use CSV format (.csv files only)\n");
            jsonFileValid = false;
            jsonFileLoaded = true;
        }
    }
    
    void EMSCRIPTEN_KEEPALIVE setJsonFilename(const char* filename) {
        loadedJsonFilename = filename;
        printf(">>> JSON file selected: %s\n", filename);
    }
}

// Update replay (advance frames according to speed)
void updateReplay() {
    if(!isReplaying || recordedFrames.empty()) return;
    if(replayPaused) return;
    
    // Variable speed playback with better precision
    replayFrameAccumulator += replaySpeed;
    
    while(replayFrameAccumulator >= 1.0f) {
        replayFrameAccumulator -= 1.0f;
        replayCurrentFrame++;
        
        // Loop back to start
        if(replayCurrentFrame >= (int)recordedFrames.size()) {
            replayCurrentFrame = 0;
            printf("REPLAY: Looping back to start\n");
        }
        
        // Apply the frame
        applyGameState(recordedFrames[replayCurrentFrame]);
    }
}

// Handle replay controls
void handleReplayInput() {
    SDL_Event event;
    while(SDL_PollEvent(&event)) {
        if(event.type == SDL_QUIT) {
            running = false;
            return;
        }
        
        // Handle mouse click on timeline and buttons
        if(event.type == SDL_MOUSEBUTTONDOWN) {
            int mouseX = event.button.x;
            int mouseY = event.button.y;
            
            // Button definitions (matching drawReplayUI)
            int buttonY = SCREEN_HEIGHT - 75;
            int buttonSize = 30;
            int buttonSpacing = 40;
            
            // Play/Pause button (x=20)
            if(mouseX >= 20 && mouseX <= 20 + buttonSize && 
               mouseY >= buttonY && mouseY <= buttonY + buttonSize) {
                replayPaused = !replayPaused;
                printf("REPLAY: %s\n", replayPaused ? "PAUSED" : "PLAYING");
            }
            // Speed - button (x=60)
            else if(mouseX >= 60 && mouseX <= 60 + buttonSize && 
                    mouseY >= buttonY && mouseY <= buttonY + buttonSize) {
                if(replaySpeed > 0.25f) {
                    if(replaySpeed == 1.0f) replaySpeed = 0.5f;
                    else if(replaySpeed == 0.5f) replaySpeed = 0.25f;
                    else if(replaySpeed > 1.0f) replaySpeed /= 2.0f;
                    printf("REPLAY: Speed %.2fx\n", replaySpeed);
                }
            }
            // Speed + button (x=100)
            else if(mouseX >= 100 && mouseX <= 100 + buttonSize && 
                    mouseY >= buttonY && mouseY <= buttonY + buttonSize) {
                if(replaySpeed < 8.0f) {
                    if(replaySpeed == 0.25f) replaySpeed = 0.5f;
                    else if(replaySpeed == 0.5f) replaySpeed = 1.0f;
                    else replaySpeed *= 2.0f;
                    printf("REPLAY: Speed %.2fx\n", replaySpeed);
                }
            }
            // Restart button (x=140)
            else if(mouseX >= 140 && mouseX <= 140 + buttonSize && 
                    mouseY >= buttonY && mouseY <= buttonY + buttonSize) {
                replayCurrentFrame = 0;
                replayPaused = false;
                applyGameState(recordedFrames[0]);
                printf("REPLAY: Restarted from beginning\n");
            }
            // Export button (x=180)
            else if(mouseX >= 180 && mouseX <= 180 + buttonSize && 
                    mouseY >= buttonY && mouseY <= buttonY + buttonSize) {
                exportReplayToCSV();
            }
            
            // Timeline bar area (at bottom - NEW POSITION)
            int timelineX = 20;
            int timelineY = SCREEN_HEIGHT - 35;
            int timelineWidth = SCREEN_WIDTH - 40;
            int timelineHeight = 20;
            
            if(mouseX >= timelineX && mouseX <= timelineX + timelineWidth &&
               mouseY >= timelineY && mouseY <= timelineY + timelineHeight) {
                // Calculate which frame to jump to
                float clickPercent = (float)(mouseX - timelineX) / timelineWidth;
                int targetFrame = (int)(clickPercent * recordedFrames.size());
                
                if(targetFrame >= 0 && targetFrame < (int)recordedFrames.size()) {
                    replayCurrentFrame = targetFrame;
                    applyGameState(recordedFrames[replayCurrentFrame]);
                    printf("REPLAY: Jumped to frame %d (%.1f%%)\n", replayCurrentFrame, clickPercent * 100);
                }
            }
        }
        
        if(event.type == SDL_KEYDOWN) {
            switch(event.key.keysym.sym) {
                case SDLK_ESCAPE:
                    exitReplay();
                    break;
                    
                case SDLK_SPACE:
                    replayPaused = !replayPaused;
                    printf("REPLAY: %s\n", replayPaused ? "PAUSED" : "PLAYING");
                    break;
                    
                case SDLK_LEFT:
                    if(replayPaused && replayCurrentFrame > 0) {
                        replayCurrentFrame--;
                        applyGameState(recordedFrames[replayCurrentFrame]);
                        printf("REPLAY: Frame %d / %d\n", replayCurrentFrame, (int)recordedFrames.size());
                    }
                    break;
                    
                case SDLK_RIGHT:
                    if(replayPaused && replayCurrentFrame < (int)recordedFrames.size() - 1) {
                        replayCurrentFrame++;
                        applyGameState(recordedFrames[replayCurrentFrame]);
                        printf("REPLAY: Frame %d / %d\n", replayCurrentFrame, (int)recordedFrames.size());
                    }
                    break;
                    
                case SDLK_q:
                    // Skip backward 10 frames
                    replayCurrentFrame -= 10;
                    if(replayCurrentFrame < 0) replayCurrentFrame = 0;
                    applyGameState(recordedFrames[replayCurrentFrame]);
                    printf("REPLAY: Skipped to frame %d\n", replayCurrentFrame);
                    break;
                    
                case SDLK_e:
                    // Skip forward 10 frames
                    replayCurrentFrame += 10;
                    if(replayCurrentFrame >= (int)recordedFrames.size()) {
                        replayCurrentFrame = (int)recordedFrames.size() - 1;
                    }
                    applyGameState(recordedFrames[replayCurrentFrame]);
                    printf("REPLAY: Skipped to frame %d\n", replayCurrentFrame);
                    break;
                    
                case SDLK_MINUS:
                case SDLK_KP_MINUS:
                    // Decrease speed
                    if(replaySpeed > 0.25f) {
                        if(replaySpeed == 1.0f) replaySpeed = 0.5f;
                        else if(replaySpeed == 0.5f) replaySpeed = 0.25f;
                        else if(replaySpeed > 1.0f) replaySpeed /= 2.0f;
                        printf("REPLAY: Speed %.2fx\n", replaySpeed);
                    }
                    break;
                    
                case SDLK_EQUALS:
                case SDLK_PLUS:
                case SDLK_KP_PLUS:
                    // Increase speed
                    if(replaySpeed < 8.0f) {
                        if(replaySpeed == 0.25f) replaySpeed = 0.5f;
                        else if(replaySpeed == 0.5f) replaySpeed = 1.0f;
                        else replaySpeed *= 2.0f;
                        printf("REPLAY: Speed %.2fx\n", replaySpeed);
                    }
                    break;
                    
                case SDLK_r:
                    // Restart replay
                    replayCurrentFrame = 0;
                    replayPaused = false;
                    applyGameState(recordedFrames[0]);
                    printf("REPLAY: Restarted from beginning\n");
                    break;
                    
                case SDLK_x:
                    // Export replay to CSV
                    exportReplayToCSV();
                    break;
            }
        }
    }
}

void initEnemies() {
    // Solo 2 enemigos en el centro del mapa
    enemies.push_back(Enemy(19, 20, DOG));      // Perro
    enemies.push_back(Enemy(21, 20, SOLDIER));  // Soldado
    
    totalEnemies = enemies.size(); // Guardar el total de enemigos
    printf("Enemigos inicializados: %d (1 perro, 1 soldado)\n", totalEnemies);
}

void initItems() {
    // Spawn items throughout the map
    items.push_back(Item(10, 5, ITEM_AMMO));
    items.push_back(Item(30, 5, ITEM_HEALTH));
    items.push_back(Item(5, 12, ITEM_AMMO));
    items.push_back(Item(15, 18, ITEM_HEALTH));
    items.push_back(Item(25, 12, ITEM_AMMO));
    items.push_back(Item(33, 20, ITEM_HEALTH));
    items.push_back(Item(12, 28, ITEM_AMMO));
    items.push_back(Item(28, 35, ITEM_HEALTH));
    items.push_back(Item(18, 8, ITEM_AMMO));
    items.push_back(Item(22, 32, ITEM_HEALTH));
}

// ============================================================================
// LIGHTING SYSTEM FUNCTIONS
// ============================================================================

void initLighting() {
    lightSources.clear();
    sectors.clear();
    
    // Add torches in corners and hallways
    lightSources.push_back({5.5, 5.5, 4.0f, 255, 200, 100, 0.8f, true});
    lightSources.push_back({15.5, 5.5, 4.0f, 255, 200, 100, 0.8f, true});
    lightSources.push_back({5.5, 15.5, 4.0f, 255, 200, 100, 0.8f, true});
    lightSources.push_back({15.5, 15.5, 4.0f, 255, 200, 100, 0.8f, true});
    lightSources.push_back({30.5, 10.5, 4.0f, 255, 200, 100, 0.8f, true});
    lightSources.push_back({20.5, 20.5, 5.0f, 255, 220, 150, 1.0f, true}); // Brighter center light
    lightSources.push_back({10.5, 30.5, 4.0f, 255, 200, 100, 0.8f, true});
    lightSources.push_back({30.5, 30.5, 4.0f, 255, 200, 100, 0.8f, true});
    
    // Add lights for items (health packs = red, ammo = yellow/orange)
    for(auto& item : items) {
        item.lightIndex = lightSources.size(); // Store the index of this item's light
        
        if(item.type == ITEM_HEALTH) {
            // Red glow for health packs
            lightSources.push_back({item.x + 0.5, item.y + 0.5, 3.0f, 255, 50, 50, 0.6f, true});
        } else if(item.type == ITEM_AMMO) {
            // Yellow/orange glow for ammo
            lightSources.push_back({item.x + 0.5, item.y + 0.5, 3.0f, 255, 200, 50, 0.6f, true});
        }
    }
    
    // Central room bright, corridors darker
    sectors.push_back({18, 18, 23, 23, 0.4f});    // Central room
    sectors.push_back({0, 0, 10, MAP_HEIGHT, 0.1f});  // Left corridor
    sectors.push_back({30, 0, MAP_WIDTH, MAP_HEIGHT, 0.1f}); // Right corridor
    
    printf("Lighting system initialized: %d light sources (%d items with lights), %d sectors\n", 
           (int)lightSources.size(), (int)items.size(), (int)sectors.size());
}

float calculateLightingAt(double x, double y, double z = 0) {
    float totalLight = ambientLight;
    
    // Sector-based lighting
    for(const auto& sector : sectors) {
        if(x >= sector.minX && x <= sector.maxX && 
           y >= sector.minY && y <= sector.maxY) {
            totalLight += sector.brightness;
        }
    }
    
    // Static light sources
    for(const auto& light : lightSources) {
        if(!light.active) continue;
        
        double dx = x - light.x;
        double dy = y - light.y;
        double dist = sqrt(dx*dx + dy*dy);
        
        if(dist < light.radius) {
            float attenuation = 1.0f - (dist / light.radius);
            attenuation = attenuation * attenuation; // Quadratic falloff
            totalLight += light.intensity * attenuation;
        }
    }
    
    // Player flashlight
    if(flashlightEnabled) {
        double dx = x - posX;
        double dy = y - posY;
        double dist = sqrt(dx*dx + dy*dy);
        
        // Check if point is in front of player (cone check)
        double dotProduct = (dx * dirX + dy * dirY) / (dist + 0.001);
        if(dotProduct > 0.5 && dist < flashlightRadius) {
            float attenuation = 1.0f - (dist / flashlightRadius);
            float coneEffect = (dotProduct - 0.5f) * 2.0f; // 0.0 to 1.0
            totalLight += flashlightIntensity * attenuation * coneEffect;
        }
    }
    
    // Apply global light multiplier (from UI slider)
    totalLight *= globalLightMultiplier;
    
    // Clamp to [0, 1]
    if(totalLight > 1.0f) totalLight = 1.0f;
    return totalLight;
}

bool isInShadow(double pointX, double pointY, const LightSource& light) {
    double dx = pointX - light.x;
    double dy = pointY - light.y;
    double dist = sqrt(dx*dx + dy*dy);
    
    // Check for blocking entities
    for(const auto& enemy : enemies) {
        if(!enemy.alive) continue;
        
        // Simple circle-based shadow check
        double edx = enemy.x - light.x;
        double edy = enemy.y - light.y;
        double enemyDist = sqrt(edx*edx + edy*edy);
        
        // If enemy is between light and point
        if(enemyDist < dist) {
            double px = enemy.x - pointX;
            double py = enemy.y - pointY;
            double pointToEnemyDist = sqrt(px*px + py*py);
            
            // Check if point is in shadow cone
            if(pointToEnemyDist < 0.5) { // Enemy radius
                return true;
            }
        }
    }
    
    return false;
}

void applyLighting(int& r, int& g, int& b, float lightLevel) {
    r = int(r * lightLevel);
    g = int(g * lightLevel);
    b = int(b * lightLevel);
    if(r > 255) r = 255;
    if(g > 255) g = 255;
    if(b > 255) b = 255;
}

void drawWeapon() {
    // Draw weapon at bottom center of screen
    int weaponWidth = 120;
    int weaponHeight = 180;
    int weaponX = SCREEN_WIDTH / 2 - weaponWidth / 2;
    int weaponY = SCREEN_HEIGHT - weaponHeight;
    
    // Weapon recoil when shooting
    if(weaponState == 1) {
        weaponY -= 20;
    }
    
    // Draw weapon based on type - CADA ARMA DIFERENTE
    if(currentWeapon == PISTOL) {
        // Pistola - gris, compacta
        SDL_SetRenderDrawColor(renderer, 60, 60, 60, 255);
        SDL_Rect barrel = {weaponX + 50, weaponY, 20, 60};
        SDL_RenderFillRect(renderer, &barrel);
        
        SDL_SetRenderDrawColor(renderer, 80, 80, 80, 255);
        SDL_Rect grip = {weaponX + 45, weaponY + 60, 30, 80};
        SDL_RenderFillRect(renderer, &grip);
        
        // Detalles plateados
        SDL_SetRenderDrawColor(renderer, 150, 150, 150, 255);
        SDL_Rect detail = {weaponX + 52, weaponY + 10, 16, 4};
        SDL_RenderFillRect(renderer, &detail);
    } 
    else if(currentWeapon == MACHINEGUN) {
        // Ametralladora - más larga, doble cañón
        SDL_SetRenderDrawColor(renderer, 40, 40, 40, 255);
        SDL_Rect barrel1 = {weaponX + 45, weaponY, 12, 80};
        SDL_RenderFillRect(renderer, &barrel1);
        SDL_Rect barrel2 = {weaponX + 63, weaponY, 12, 80};
        SDL_RenderFillRect(renderer, &barrel2);
        
        SDL_SetRenderDrawColor(renderer, 70, 70, 70, 255);
        SDL_Rect grip = {weaponX + 45, weaponY + 80, 30, 60};
        SDL_RenderFillRect(renderer, &grip);
        
        // Cargador largo
        SDL_SetRenderDrawColor(renderer, 30, 30, 30, 255);
        SDL_Rect mag = {weaponX + 52, weaponY + 100, 16, 40};
        SDL_RenderFillRect(renderer, &mag);
    }
    else if(currentWeapon == LASER) {
        // Laser - forma futurista, color azulado
        SDL_SetRenderDrawColor(renderer, 30, 50, 100, 255);
        SDL_Rect barrel = {weaponX + 48, weaponY, 24, 70};
        SDL_RenderFillRect(renderer, &barrel);
        
        // Luces LED
        SDL_SetRenderDrawColor(renderer, 0, 200, 255, 255);
        for(int i = 0; i < 4; i++) {
            SDL_Rect led = {weaponX + 54, weaponY + 15 + i*12, 4, 4};
            SDL_RenderFillRect(renderer, &led);
            SDL_Rect led2 = {weaponX + 62, weaponY + 15 + i*12, 4, 4};
            SDL_RenderFillRect(renderer, &led2);
        }
        
        SDL_SetRenderDrawColor(renderer, 50, 80, 120, 255);
        SDL_Rect grip = {weaponX + 45, weaponY + 70, 30, 70};
        SDL_RenderFillRect(renderer, &grip);
        
        // Cristal del láser
        SDL_SetRenderDrawColor(renderer, 255, 50, 50, 200);
        SDL_Rect crystal = {weaponX + 55, weaponY - 5, 10, 10};
        SDL_RenderFillRect(renderer, &crystal);
    }
    else if(currentWeapon == SHOTGUN) {
        // Escopeta - más ancha, color madera
        SDL_SetRenderDrawColor(renderer, 70, 50, 30, 255);
        SDL_Rect barrel = {weaponX + 40, weaponY, 40, 70};
        SDL_RenderFillRect(renderer, &barrel);
        
        // Doble cañón
        SDL_SetRenderDrawColor(renderer, 30, 30, 30, 255);
        SDL_Rect bore1 = {weaponX + 50, weaponY - 2, 8, 8};
        SDL_RenderFillRect(renderer, &bore1);
        SDL_Rect bore2 = {weaponX + 62, weaponY - 2, 8, 8};
        SDL_RenderFillRect(renderer, &bore2);
        
        SDL_SetRenderDrawColor(renderer, 90, 60, 30, 255);
        SDL_Rect grip = {weaponX + 45, weaponY + 70, 30, 70};
        SDL_RenderFillRect(renderer, &grip);
    }
    
    // Muzzle flash when shooting (not in replay mode)
    if(weaponState == 1 && weaponFrame < 2 && !isReplaying) {
        SDL_SetRenderDrawColor(renderer, 255, 255, 0, 255);
        SDL_Rect flash = {weaponX + 45, weaponY - 15, 30, 20};
        SDL_RenderFillRect(renderer, &flash);
        
        SDL_SetRenderDrawColor(renderer, 255, 200, 0, 200);
        SDL_Rect flash2 = {weaponX + 35, weaponY - 25, 50, 30};
        SDL_RenderFillRect(renderer, &flash2);
    }
}

// Bitmap font: 5x7 pixels per character
void drawChar(char c, int x, int y, int r, int g, int b) {
    SDL_SetRenderDrawColor(renderer, r, g, b, 255);
    
    // Convert to uppercase
    if(c >= 'a' && c <= 'z') c = c - 32;
    
    // 5x7 bitmap patterns for each character
    static const bool patterns[][7][5] = {
        // A
        {{0,1,1,1,0},{1,0,0,0,1},{1,0,0,0,1},{1,1,1,1,1},{1,0,0,0,1},{1,0,0,0,1},{1,0,0,0,1}},
        // B
        {{1,1,1,1,0},{1,0,0,0,1},{1,1,1,1,0},{1,0,0,0,1},{1,0,0,0,1},{1,0,0,0,1},{1,1,1,1,0}},
        // C
        {{0,1,1,1,0},{1,0,0,0,1},{1,0,0,0,0},{1,0,0,0,0},{1,0,0,0,0},{1,0,0,0,1},{0,1,1,1,0}},
        // D
        {{1,1,1,0,0},{1,0,0,1,0},{1,0,0,0,1},{1,0,0,0,1},{1,0,0,0,1},{1,0,0,1,0},{1,1,1,0,0}},
        // E
        {{1,1,1,1,1},{1,0,0,0,0},{1,1,1,1,0},{1,0,0,0,0},{1,0,0,0,0},{1,0,0,0,0},{1,1,1,1,1}},
        // F
        {{1,1,1,1,1},{1,0,0,0,0},{1,1,1,1,0},{1,0,0,0,0},{1,0,0,0,0},{1,0,0,0,0},{1,0,0,0,0}},
        // G
        {{0,1,1,1,0},{1,0,0,0,1},{1,0,0,0,0},{1,0,1,1,1},{1,0,0,0,1},{1,0,0,0,1},{0,1,1,1,0}},
        // H
        {{1,0,0,0,1},{1,0,0,0,1},{1,1,1,1,1},{1,0,0,0,1},{1,0,0,0,1},{1,0,0,0,1},{1,0,0,0,1}},
        // I
        {{1,1,1,1,1},{0,0,1,0,0},{0,0,1,0,0},{0,0,1,0,0},{0,0,1,0,0},{0,0,1,0,0},{1,1,1,1,1}},
        // J
        {{0,0,1,1,1},{0,0,0,1,0},{0,0,0,1,0},{0,0,0,1,0},{0,0,0,1,0},{1,0,0,1,0},{0,1,1,0,0}},
        // K
        {{1,0,0,0,1},{1,0,0,1,0},{1,1,1,0,0},{1,0,1,0,0},{1,0,0,1,0},{1,0,0,0,1},{1,0,0,0,1}},
        // L
        {{1,0,0,0,0},{1,0,0,0,0},{1,0,0,0,0},{1,0,0,0,0},{1,0,0,0,0},{1,0,0,0,0},{1,1,1,1,1}},
        // M
        {{1,0,0,0,1},{1,1,0,1,1},{1,0,1,0,1},{1,0,0,0,1},{1,0,0,0,1},{1,0,0,0,1},{1,0,0,0,1}},
        // N
        {{1,0,0,0,1},{1,1,0,0,1},{1,0,1,0,1},{1,0,0,1,1},{1,0,0,0,1},{1,0,0,0,1},{1,0,0,0,1}},
        // O
        {{0,1,1,1,0},{1,0,0,0,1},{1,0,0,0,1},{1,0,0,0,1},{1,0,0,0,1},{1,0,0,0,1},{0,1,1,1,0}},
        // P
        {{1,1,1,1,0},{1,0,0,0,1},{1,0,0,0,1},{1,1,1,1,0},{1,0,0,0,0},{1,0,0,0,0},{1,0,0,0,0}},
        // Q
        {{0,1,1,1,0},{1,0,0,0,1},{1,0,0,0,1},{1,0,0,0,1},{1,0,1,0,1},{1,0,0,1,0},{0,1,1,0,1}},
        // R
        {{1,1,1,1,0},{1,0,0,0,1},{1,0,0,0,1},{1,1,1,1,0},{1,0,1,0,0},{1,0,0,1,0},{1,0,0,0,1}},
        // S
        {{0,1,1,1,1},{1,0,0,0,0},{1,0,0,0,0},{0,1,1,1,0},{0,0,0,0,1},{0,0,0,0,1},{1,1,1,1,0}},
        // T
        {{1,1,1,1,1},{0,0,1,0,0},{0,0,1,0,0},{0,0,1,0,0},{0,0,1,0,0},{0,0,1,0,0},{0,0,1,0,0}},
        // U
        {{1,0,0,0,1},{1,0,0,0,1},{1,0,0,0,1},{1,0,0,0,1},{1,0,0,0,1},{1,0,0,0,1},{0,1,1,1,0}},
        // V
        {{1,0,0,0,1},{1,0,0,0,1},{1,0,0,0,1},{1,0,0,0,1},{1,0,0,0,1},{0,1,0,1,0},{0,0,1,0,0}},
        // W
        {{1,0,0,0,1},{1,0,0,0,1},{1,0,0,0,1},{1,0,1,0,1},{1,0,1,0,1},{1,0,1,0,1},{0,1,0,1,0}},
        // X
        {{1,0,0,0,1},{1,0,0,0,1},{0,1,0,1,0},{0,0,1,0,0},{0,1,0,1,0},{1,0,0,0,1},{1,0,0,0,1}},
        // Y
        {{1,0,0,0,1},{1,0,0,0,1},{0,1,0,1,0},{0,0,1,0,0},{0,0,1,0,0},{0,0,1,0,0},{0,0,1,0,0}},
        // Z
        {{1,1,1,1,1},{0,0,0,0,1},{0,0,0,1,0},{0,0,1,0,0},{0,1,0,0,0},{1,0,0,0,0},{1,1,1,1,1}},
        // 0
        {{0,1,1,1,0},{1,0,0,0,1},{1,0,0,1,1},{1,0,1,0,1},{1,1,0,0,1},{1,0,0,0,1},{0,1,1,1,0}},
        // 1
        {{0,0,1,0,0},{0,1,1,0,0},{0,0,1,0,0},{0,0,1,0,0},{0,0,1,0,0},{0,0,1,0,0},{0,1,1,1,0}},
        // 2
        {{0,1,1,1,0},{1,0,0,0,1},{0,0,0,0,1},{0,0,0,1,0},{0,0,1,0,0},{0,1,0,0,0},{1,1,1,1,1}},
        // 3
        {{1,1,1,1,0},{0,0,0,0,1},{0,0,0,0,1},{0,1,1,1,0},{0,0,0,0,1},{0,0,0,0,1},{1,1,1,1,0}},
        // 4
        {{0,0,0,1,0},{0,0,1,1,0},{0,1,0,1,0},{1,0,0,1,0},{1,1,1,1,1},{0,0,0,1,0},{0,0,0,1,0}},
        // 5
        {{1,1,1,1,1},{1,0,0,0,0},{1,1,1,1,0},{0,0,0,0,1},{0,0,0,0,1},{1,0,0,0,1},{0,1,1,1,0}},
        // 6
        {{0,0,1,1,0},{0,1,0,0,0},{1,0,0,0,0},{1,1,1,1,0},{1,0,0,0,1},{1,0,0,0,1},{0,1,1,1,0}},
        // 7
        {{1,1,1,1,1},{0,0,0,0,1},{0,0,0,1,0},{0,0,1,0,0},{0,1,0,0,0},{0,1,0,0,0},{0,1,0,0,0}},
        // 8
        {{0,1,1,1,0},{1,0,0,0,1},{1,0,0,0,1},{0,1,1,1,0},{1,0,0,0,1},{1,0,0,0,1},{0,1,1,1,0}},
        // 9
        {{0,1,1,1,0},{1,0,0,0,1},{1,0,0,0,1},{0,1,1,1,1},{0,0,0,0,1},{0,0,0,1,0},{0,1,1,0,0}},
        // : (colon)
        {{0,0,0,0,0},{0,0,1,0,0},{0,0,0,0,0},{0,0,0,0,0},{0,0,0,0,0},{0,0,1,0,0},{0,0,0,0,0}},
        // Space (always empty)
        {{0,0,0,0,0},{0,0,0,0,0},{0,0,0,0,0},{0,0,0,0,0},{0,0,0,0,0},{0,0,0,0,0},{0,0,0,0,0}}
    };
    
    int patternIndex = -1;
    if(c >= 'A' && c <= 'Z') patternIndex = c - 'A';
    else if(c >= '0' && c <= '9') patternIndex = 26 + (c - '0');
    else if(c == ':') patternIndex = 36;
    else if(c == ' ') patternIndex = 37;
    else return; // Unknown character
    
    // Draw the pattern
    for(int row = 0; row < 7; row++) {
        for(int col = 0; col < 5; col++) {
            if(patterns[patternIndex][row][col]) {
                SDL_Rect pixel = {x + col, y + row, 1, 1};
                SDL_RenderFillRect(renderer, &pixel);
            }
        }
    }
}

// Draw text string
void drawText(const char* text, int x, int y, int r, int g, int b, int scale = 1) {
    int cursorX = x;
    for(int i = 0; text[i] != '\0'; i++) {
        drawChar(text[i], cursorX, y, r, g, b);
        cursorX += (6 * scale); // Spacing increases with scale
    }
}

// Draw enemy sprite - DOG
void drawDogSprite(int x, int y, int size) {
    // Body (brown)
    SDL_SetRenderDrawColor(renderer, 120, 80, 40, 255);
    SDL_Rect body = {x + size/4, y + size/2, size/2, size/3};
    SDL_RenderFillRect(renderer, &body);
    
    // Head
    SDL_Rect head = {x + size/3, y + size/3, size/3, size/4};
    SDL_RenderFillRect(renderer, &head);
    
    // Ears
    SDL_SetRenderDrawColor(renderer, 100, 60, 30, 255);
    SDL_Rect ear1 = {x + size/3, y + size/4, size/8, size/6};
    SDL_Rect ear2 = {x + size/2, y + size/4, size/8, size/6};
    SDL_RenderFillRect(renderer, &ear1);
    SDL_RenderFillRect(renderer, &ear2);
    
    // Eyes (red - aggressive)
    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
    SDL_Rect eye1 = {x + size/3 + 2, y + size/3 + 2, 3, 3};
    SDL_Rect eye2 = {x + size/2 - 2, y + size/3 + 2, 3, 3};
    SDL_RenderFillRect(renderer, &eye1);
    SDL_RenderFillRect(renderer, &eye2);
    
    // Legs
    SDL_SetRenderDrawColor(renderer, 100, 60, 30, 255);
    for(int i = 0; i < 4; i++) {
        SDL_Rect leg = {x + size/4 + i*size/8, y + size*3/4, size/12, size/4};
        SDL_RenderFillRect(renderer, &leg);
    }
}

// Draw enemy sprite - SOLDIER
void drawSoldierSprite(int x, int y, int size) {
    // Helmet (gray)
    SDL_SetRenderDrawColor(renderer, 80, 80, 80, 255);
    SDL_Rect helmet = {x + size/3, y + size/6, size/3, size/5};
    SDL_RenderFillRect(renderer, &helmet);
    
    // Face (skin)
    SDL_SetRenderDrawColor(renderer, 200, 150, 120, 255);
    SDL_Rect face = {x + size/3, y + size/3, size/3, size/4};
    SDL_RenderFillRect(renderer, &face);
    
    // Uniform (blue/gray)
    SDL_SetRenderDrawColor(renderer, 60, 80, 120, 255);
    SDL_Rect body = {x + size/4, y + size*7/12, size/2, size*5/12};
    SDL_RenderFillRect(renderer, &body);
    
    // Gun (black)
    SDL_SetRenderDrawColor(renderer, 40, 40, 40, 255);
    SDL_Rect gun = {x + size*3/4, y + size*2/3, size/4, size/12};
    SDL_RenderFillRect(renderer, &gun);
    
    // Eyes
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_Rect eye1 = {x + size/3 + 3, y + size/3 + 5, 2, 2};
    SDL_Rect eye2 = {x + size/2 - 3, y + size/3 + 5, 2, 2};
    SDL_RenderFillRect(renderer, &eye1);
    SDL_RenderFillRect(renderer, &eye2);
    
    // Legs
    SDL_SetRenderDrawColor(renderer, 40, 60, 80, 255);
    SDL_Rect leg1 = {x + size/3, y + size - size/6, size/6, size/6};
    SDL_Rect leg2 = {x + size/2, y + size - size/6, size/6, size/6};
    SDL_RenderFillRect(renderer, &leg1);
    SDL_RenderFillRect(renderer, &leg2);
}

// Simple digit drawing function (7-segment style)
void drawDigit(int digit, int x, int y, int r, int g, int b) {
    int w = 8, h = 14;
    SDL_SetRenderDrawColor(renderer, r, g, b, 255);
    
    // Segments: top, top-right, bottom-right, bottom, bottom-left, top-left, middle
    bool segs[10][7] = {
        {1,1,1,1,1,1,0}, // 0
        {0,1,1,0,0,0,0}, // 1
        {1,1,0,1,1,0,1}, // 2
        {1,1,1,1,0,0,1}, // 3
        {0,1,1,0,0,1,1}, // 4
        {1,0,1,1,0,1,1}, // 5
        {1,0,1,1,1,1,1}, // 6
        {1,1,1,0,0,0,0}, // 7
        {1,1,1,1,1,1,1}, // 8
        {1,1,1,1,0,1,1}  // 9
    };
    
    if(digit < 0 || digit > 9) return;
    
    SDL_Rect seg;
    if(segs[digit][0]) { seg = {x+2, y, w-4, 2}; SDL_RenderFillRect(renderer, &seg); } // top
    if(segs[digit][1]) { seg = {x+w-2, y+2, 2, h/2-2}; SDL_RenderFillRect(renderer, &seg); } // top-right
    if(segs[digit][2]) { seg = {x+w-2, y+h/2+1, 2, h/2-2}; SDL_RenderFillRect(renderer, &seg); } // bottom-right
    if(segs[digit][3]) { seg = {x+2, y+h-2, w-4, 2}; SDL_RenderFillRect(renderer, &seg); } // bottom
    if(segs[digit][4]) { seg = {x, y+h/2+1, 2, h/2-2}; SDL_RenderFillRect(renderer, &seg); } // bottom-left
    if(segs[digit][5]) { seg = {x, y+2, 2, h/2-2}; SDL_RenderFillRect(renderer, &seg); } // top-left
    if(segs[digit][6]) { seg = {x+2, y+h/2-1, w-4, 2}; SDL_RenderFillRect(renderer, &seg); } // middle
}

void drawNumber(int number, int x, int y, int r, int g, int b) {
    if(number == 0) {
        drawDigit(0, x, y, r, g, b);
        return;
    }
    
    char buffer[16];
    sprintf(buffer, "%d", number);
    int len = strlen(buffer);
    for(int i = 0; i < len; i++) {
        drawDigit(buffer[i] - '0', x + i * 10, y, r, g, b);
    }
}

void drawHUD() {
    // Health bar con número
    SDL_SetRenderDrawColor(renderer, 200, 0, 0, 255);
    SDL_Rect healthBg = {10, SCREEN_HEIGHT - 30, 200, 20};
    SDL_RenderFillRect(renderer, &healthBg);
    
    SDL_SetRenderDrawColor(renderer, 0, 200, 0, 255);
    SDL_Rect health = {10, SCREEN_HEIGHT - 30, (playerHealth * 2), 20};
    SDL_RenderFillRect(renderer, &health);
    
    // Número de salud dentro de la barra
    drawNumber(playerHealth, 90, SCREEN_HEIGHT - 27, 255, 255, 255);
    
    // Icono de munición
    SDL_SetRenderDrawColor(renderer, 200, 180, 0, 255);
    SDL_Rect ammoIcon = {220, SCREEN_HEIGHT - 28, 12, 16};
    SDL_RenderFillRect(renderer, &ammoIcon);
    SDL_SetRenderDrawColor(renderer, 100, 90, 0, 255);
    SDL_RenderDrawRect(renderer, &ammoIcon);
    
    // Número de munición
    drawNumber(ammo, 240, SCREEN_HEIGHT - 27, 255, 255, 0);
    
    // Kills counter con número
    SDL_SetRenderDrawColor(renderer, 200, 0, 0, 255);
    SDL_Rect skullIcon = {SCREEN_WIDTH - 80, 10, 12, 12};
    SDL_RenderFillRect(renderer, &skullIcon);
    drawNumber(kills, SCREEN_WIDTH - 60, 10, 255, 0, 0);
    
    // Weapon indicator
    const char* weaponNames[] = {"PISTOL", "M-GUN", "LASER", "SHOTGUN"};
    // Dibujar nombre del arma actual en la esquina
    
    // Flashlight indicator
    if(flashlightEnabled) {
        drawText("LIGHT", 10, 50, 255, 255, 100);
    }
    
    // GLOBAL LIGHT INTENSITY SLIDER (arriba centro)
    int sliderX = SCREEN_WIDTH / 2 - 100;
    int sliderY = 10;
    int sliderWidth = 200;
    int sliderHeight = 20;
    
    // Label
    drawText("LIGHT", sliderX - 60, sliderY + 5, 255, 200, 100);
    
    // Slider background (dark)
    SDL_SetRenderDrawColor(renderer, 40, 40, 40, 200);
    SDL_Rect sliderBg = {sliderX, sliderY, sliderWidth, sliderHeight};
    SDL_RenderFillRect(renderer, &sliderBg);
    
    // Slider border
    SDL_SetRenderDrawColor(renderer, 200, 200, 200, 255);
    SDL_RenderDrawRect(renderer, &sliderBg);
    
    // Slider fill (indicates current level, 0.0-2.0 range mapped to slider width)
    float fillPercent = globalLightMultiplier / 2.0f; // 0.0 to 1.0
    int fillWidth = int(fillPercent * sliderWidth);
    
    // Color gradient: red (low) -> yellow (mid) -> white (high)
    int r, g, b;
    if(globalLightMultiplier < 1.0f) {
        // Red to Yellow (0.0 to 1.0)
        r = 255;
        g = int(globalLightMultiplier * 255);
        b = 0;
    } else {
        // Yellow to White (1.0 to 2.0)
        r = 255;
        g = 255;
        b = int((globalLightMultiplier - 1.0f) * 255);
    }
    
    SDL_SetRenderDrawColor(renderer, r, g, b, 255);
    SDL_Rect sliderFill = {sliderX, sliderY, fillWidth, sliderHeight};
    SDL_RenderFillRect(renderer, &sliderFill);
    
    // Percentage indicator (0-200%)
    int percentage = int(globalLightMultiplier * 100);
    drawNumber(percentage, sliderX + sliderWidth + 10, sliderY + 5, 255, 255, 255);
    
    // Crosshair
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderDrawLine(renderer, SCREEN_WIDTH/2 - 10, SCREEN_HEIGHT/2, SCREEN_WIDTH/2 - 3, SCREEN_HEIGHT/2);
    SDL_RenderDrawLine(renderer, SCREEN_WIDTH/2 + 3, SCREEN_HEIGHT/2, SCREEN_WIDTH/2 + 10, SCREEN_HEIGHT/2);
    SDL_RenderDrawLine(renderer, SCREEN_WIDTH/2, SCREEN_HEIGHT/2 - 10, SCREEN_WIDTH/2, SCREEN_HEIGHT/2 - 3);
    SDL_RenderDrawLine(renderer, SCREEN_WIDTH/2, SCREEN_HEIGHT/2 + 3, SCREEN_WIDTH/2, SCREEN_HEIGHT/2 + 10);
}

void shoot() {
    if(ammo <= 0) return;
    
    // Ametralladora puede disparar mientras mantienes presionado
    if(currentWeapon != MACHINEGUN && currentWeapon != SHOTGUN && currentWeapon != LASER && weaponState != 0) return;
    
    weaponState = 1;
    weaponFrame = 0;
    
    if(currentWeapon == PISTOL) {
        ammo--;
        bulletsFired++;
        if(shootSound) Mix_PlayChannel(-1, shootSound, 0);
        
        Bullet bullet;
        bullet.x = posX;
        bullet.y = posY;
        bullet.dirX = dirX;
        bullet.dirY = dirY;
        bullet.lifetime = 60;
        bullet.active = true;
        bullets.push_back(bullet);
    }
    else if(currentWeapon == MACHINEGUN) {
        // Disparo rápido
        if(weaponTimer == 0) {
            ammo--;
            bulletsFired++;
            if(shootSound) Mix_PlayChannel(-1, shootSound, 0);
            
            Bullet bullet;
            bullet.x = posX;
            bullet.y = posY;
            bullet.dirX = dirX;
            bullet.dirY = dirY;
            bullet.lifetime = 60;
            bullet.active = true;
            bullets.push_back(bullet);
        }
    }
    else if(currentWeapon == LASER) {
        // LÁSER AUTOMÁTICO - Mata instantáneamente con raycast
        laserActive = true;
        if(weaponTimer % 5 == 0) {
            ammo--; // Consume munición más lento
            bulletsFired++;
            
            // Raycast para encontrar enemigo en la mira
            double rayX = posX;
            double rayY = posY;
            double rayDirX = dirX;
            double rayDirY = dirY;
            
            // Crear beam visual
            LaserBeam beam;
            beam.x = posX;
            beam.y = posY;
            beam.dirX = dirX;
            beam.dirY = dirY;
            beam.lifetime = 20;
            beam.active = true;
            laserBeams.push_back(beam);
            
            // Raycast hasta encontrar enemigo o pared
            for(int step = 0; step < 50; step++) {
                rayX += rayDirX * 0.5;
                rayY += rayDirY * 0.5;
                
                // Check wall collision
                if(worldMap[int(rayX)][int(rayY)] != 0) break;
                
                // Check enemy collision
                for(auto& enemy : enemies) {
                    if(!enemy.alive) continue;
                    
                    double dx = enemy.x - rayX;
                    double dy = enemy.y - rayY;
                    double dist = sqrt(dx * dx + dy * dy);
                    
                    if(dist < 0.5) { // Hit!
                        enemy.health = 0; // ¡KILL INSTANTÁNEO!
                        enemy.alive = false;
                        kills++;
                        if(enemyDeathSound) Mix_PlayChannel(-1, enemyDeathSound, 0);
                        printf("Láser kill! Total kills: %d\n", kills);
                        return; // Solo mata un enemigo por disparo
                    }
                }
            }
        }
    }
    else if(currentWeapon == SHOTGUN) {
        // ESCOPETA - dispara múltiples perdigones en patrón de dispersión
        if(weaponTimer == 0) {
            ammo -= 5; // Consume más munición
            bulletsFired += 7;
            if(shootSound) Mix_PlayChannel(-1, shootSound, 0);
            
            // Crear 7 perdigones con dispersión
            for(int i = -3; i <= 3; i++) {
                Bullet bullet;
                bullet.x = posX;
                bullet.y = posY;
                
                // Calcular dirección con dispersión
                double spread = i * 0.15; // Ángulo de dispersión
                double cos_s = cos(spread);
                double sin_s = sin(spread);
                bullet.dirX = dirX * cos_s - dirY * sin_s;
                bullet.dirY = dirX * sin_s + dirY * cos_s;
                
                bullet.lifetime = 40; // Menos duración que pistola
                bullet.active = true;
                bullets.push_back(bullet);
            }
        }
    }
}

void updateEnemies(double deltaTime) {
    // Actualizar animación de caída para enemigos muertos
    for(auto& enemy : enemies) {
        if(!enemy.alive) {
            // Los enemigos caen lentamente al morir
            if(enemy.fallOffset < 1.0) {
                enemy.fallOffset += deltaTime * 1.5; // Caída gradual
                if(enemy.fallOffset > 1.0) enemy.fallOffset = 1.0;
            }
            continue;
        }
    }
    
    for(auto& enemy : enemies) {
        if(!enemy.alive) continue;
        
        double dx = posX - enemy.x;
        double dy = posY - enemy.y;
        double dist = sqrt(dx * dx + dy * dy);
        
        // AI diferente según el tipo
        if(enemy.type == DOG) {
            // PERROS: corren más rápido y atacan de cerca
            enemy.moveTimer += deltaTime;
            if(enemy.moveTimer > 0.2) { // ¡Más rápidos!
                enemy.moveTimer = 0;
                
                if(dist > 1.0 && dist < 25) {
                    dx /= dist;
                    dy /= dist;
                    
                    double newX = enemy.x + dx * 0.6; // ¡Doble velocidad!
                    double newY = enemy.y + dy * 0.6;
                    
                    if(worldMap[int(newX)][int(newY)] == 0) {
                        enemy.x = newX;
                        enemy.y = newY;
                    }
                }
                
                // Perros atacan de cerca
                if(dist < 1.0) {
                    playerHealth -= 8;
                    if(playerHealth < 0) playerHealth = 0;
                    if(playerHitSound) Mix_PlayChannel(-1, playerHitSound, 0);
                }
            }
        } 
        else { // SOLDIER
            // SOLDADOS: se mueven lento pero disparan a distancia
            enemy.moveTimer += deltaTime;
            enemy.shootTimer += deltaTime;
            
            if(enemy.moveTimer > 0.5) {
                enemy.moveTimer = 0;
                
                // Mantener distancia del jugador
                if(dist > 3.0 && dist < 15) {
                    dx /= dist;
                    dy /= dist;
                    
                    double newX = enemy.x + dx * 0.25;
                    double newY = enemy.y + dy * 0.25;
                    
                    if(worldMap[int(newX)][int(newY)] == 0) {
                        enemy.x = newX;
                        enemy.y = newY;
                    }
                }
                else if(dist < 2.0) {
                    // Retroceder si está muy cerca
                    dx /= dist;
                    dy /= dist;
                    double newX = enemy.x - dx * 0.2;
                    double newY = enemy.y - dy * 0.2;
                    if(worldMap[int(newX)][int(newY)] == 0) {
                        enemy.x = newX;
                        enemy.y = newY;
                    }
                }
            }
            
            // DISPARO: Soldados disparan al jugador
            if(enemy.shootTimer > enemy.shootCooldown && dist < 15 && dist > 2.0) {
                enemy.shootTimer = 0;
                enemy.shootCooldown = 3.5 + (rand() % 15) / 10.0; // Cooldown más largo (3.5-5.0s)
                
                // Crear bala enemiga
                Bullet enemyBullet;
                enemyBullet.x = enemy.x;
                enemyBullet.y = enemy.y;
                // Dirección hacia el jugador
                enemyBullet.dirX = dx / dist;
                enemyBullet.dirY = dy / dist;
                enemyBullet.lifetime = 120;
                enemyBullet.active = true;
                enemyBullet.isEnemy = true;
                enemyBullets.push_back(enemyBullet);
                
                if(shootSound) Mix_PlayChannel(-1, shootSound, 0);
            }
        }
        
        enemy.animFrame = (enemy.animFrame + 1) % 4;
    }
    
    // Update enemy bullets
    for(auto& bullet : enemyBullets) {
        if(!bullet.active) continue;
        
        double bulletSpeed = 0.3;
        double newX = bullet.x + bullet.dirX * bulletSpeed;
        double newY = bullet.y + bullet.dirY * bulletSpeed;
        
        // Check wall collision
        if(worldMap[int(newX)][int(newY)] != 0) {
            bullet.active = false;
            continue;
        }
        
        // Check player collision
        double dx = posX - newX;
        double dy = posY - newY;
        double dist = sqrt(dx * dx + dy * dy);
        
        if(dist < 0.5) {
            playerHealth -= 15; // Daño por bala enemiga
            if(playerHealth < 0) playerHealth = 0;
            if(playerHitSound) Mix_PlayChannel(-1, playerHitSound, 0);
            bullet.active = false;
            continue;
        }
        
        bullet.x = newX;
        bullet.y = newY;
        bullet.lifetime--;
        if(bullet.lifetime <= 0) {
            bullet.active = false;
        }
    }
    
    // Update physical bullets
    for(auto& bullet : bullets) {
        if(!bullet.active) continue;
        
        // Agregar posición actual a la estela
        if(bullet.trail.size() < 15) { // Mantener hasta 15 posiciones
            bullet.trail.push_back(std::make_pair(bullet.x, bullet.y));
        } else {
            bullet.trail.erase(bullet.trail.begin()); // Remover la más antigua
            bullet.trail.push_back(std::make_pair(bullet.x, bullet.y));
        }
        
        // Move bullet (velocidad rápida)
        double bulletSpeed = 0.5;
        double newX = bullet.x + bullet.dirX * bulletSpeed;
        double newY = bullet.y + bullet.dirY * bulletSpeed;
        
        // Check wall collision
        if(worldMap[int(newX)][int(newY)] != 0) {
            bullet.active = false;
            continue;
        }
        
        // Check enemy collision
        bool hitEnemy = false;
        for(auto& enemy : enemies) {
            if(!enemy.alive) continue;
            
            double dx = enemy.x - newX;
            double dy = enemy.y - newY;
            double dist = sqrt(dx * dx + dy * dy);
            
            if(dist < 0.3) { // Radio de colisión
                enemy.health -= 50;
                if(enemy.health <= 0) {
                    enemy.alive = false;
                    kills++;
                    if(enemyDeathSound) Mix_PlayChannel(-1, enemyDeathSound, 0);
                    printf("Enemy killed! Total kills: %d\n", kills);
                }
                bullet.active = false;
                hitEnemy = true;
                break;
            }
        }
        
        if(!hitEnemy) {
            bullet.x = newX;
            bullet.y = newY;
            bullet.lifetime--;
            if(bullet.lifetime <= 0) {
                bullet.active = false;
            }
        }
    }
    
    // Update laser beams
    for(auto& beam : laserBeams) {
        if(!beam.active) continue;
        
        // Agregar estela al láser (más lenta y duradera)
        if(beam.trail.size() < 30) {
            beam.trail.push_back(std::make_pair(beam.x, beam.y));
        }
        
        beam.lifetime--;
        if(beam.lifetime <= 0) {
            beam.active = false;
        }
    }
}

void drawEnemies() {
    // Sort enemies by distance for proper rendering order
    std::vector<std::pair<double, int>> enemyOrder;
    
    for(size_t i = 0; i < enemies.size(); i++) {
        if(!enemies[i].alive) continue;
        
        double dx = enemies[i].x - posX;
        double dy = enemies[i].y - posY;
        double dist = dx * dx + dy * dy;
        enemyOrder.push_back(std::make_pair(dist, i));
    }
    
    std::sort(enemyOrder.begin(), enemyOrder.end());
    std::reverse(enemyOrder.begin(), enemyOrder.end());
    
    // Draw enemies from far to near
    for(auto& pair : enemyOrder) {
        Enemy& enemy = enemies[pair.second];
        
        double spriteX = enemy.x - posX;
        double spriteY = enemy.y - posY;
        
        // Transform to camera space
        double invDet = 1.0 / (planeX * dirY - dirX * planeY);
        double transformX = invDet * (dirY * spriteX - dirX * spriteY);
        double transformY = invDet * (-planeY * spriteX + planeX * spriteY);
        
        if(transformY <= 0) continue; // Behind player
        
        int spriteScreenX = int((SCREEN_WIDTH / 2) * (1 + transformX / transformY));
        
        int spriteHeight = abs(int(SCREEN_HEIGHT / transformY));
        
        // Aplicar efecto de caída al morir
        int fallOffsetPixels = int(enemy.fallOffset * SCREEN_HEIGHT * 0.5);
        
        // Aplicar efecto de salto (igual que las paredes y el piso)
        int jumpOffset = int(verticalPosition * SCREEN_HEIGHT * 0.02);
        
        int drawStartY = -spriteHeight / 2 + SCREEN_HEIGHT / 2 + fallOffsetPixels + jumpOffset;
        if(drawStartY < 0) drawStartY = 0;
        int drawEndY = spriteHeight / 2 + SCREEN_HEIGHT / 2 + fallOffsetPixels + jumpOffset;
        if(drawEndY >= SCREEN_HEIGHT) drawEndY = SCREEN_HEIGHT - 1;
        
        // Enemigos más delgados (30% del tamaño original)
        int spriteWidth = abs(int(SCREEN_HEIGHT / transformY * 0.3));
        int drawStartX = -spriteWidth / 2 + spriteScreenX;
        if(drawStartX < 0) drawStartX = 0;
        int drawEndX = spriteWidth / 2 + spriteScreenX;
        if(drawEndX >= SCREEN_WIDTH) drawEndX = SCREEN_WIDTH - 1;
        
        // Draw enemy sprite 3D con sombras
        for(int stripe = drawStartX; stripe < drawEndX; stripe++) {
            // Z-buffer check
            if(transformY > 0 && stripe > 0 && stripe < SCREEN_WIDTH && transformY < zBuffer[stripe]) {
                int color = 255 - int(transformY * 10);
                if(color < 50) color = 50;
                
                // Calculate lighting at enemy position
                float enemyLight = calculateLightingAt(enemy.x, enemy.y);
                
                // Check shadows from light sources
                for(const auto& light : lightSources) {
                    if(light.active && isInShadow(enemy.x, enemy.y, light)) {
                        enemyLight *= 0.3f; // Heavy shadow
                        break;
                    }
                }
                
                // Apply distance fog
                float fogFactor = 1.0f - (transformY / 20.0f);
                if(fogFactor < 0.0f) fogFactor = 0.0f;
                enemyLight *= fogFactor;
                
                // Apply lighting to base color
                int litColor = int(color * enemyLight);
                if(litColor < 0) litColor = 0;
                
                // Calcular posición relativa en el sprite (para efecto 3D)
                int relativeX = stripe - drawStartX;
                int spriteCenter = spriteWidth / 2;
                int distFromCenter = abs(relativeX - spriteCenter);
                float shadeFactor = 1.0 - (distFromCenter / (float)spriteCenter) * 0.3;
                
                if(enemy.type == SOLDIER) {
                    // SOLDADO - Estilo 3D con sombras
                    
                    // Piernas
                    SDL_SetRenderDrawColor(renderer, 
                        litColor/4 * shadeFactor, litColor/4 * shadeFactor, litColor/4 * shadeFactor, 255);
                    SDL_Rect legs = {stripe, drawStartY + spriteHeight*2/3, 1, spriteHeight/3};
                    SDL_RenderFillRect(renderer, &legs);
                    
                    // Torso (uniforme gris)
                    SDL_SetRenderDrawColor(renderer, 
                        litColor/3 * shadeFactor, litColor/3 * shadeFactor, litColor/3 * shadeFactor, 255);
                    SDL_Rect torso = {stripe, drawStartY + spriteHeight/3, 1, spriteHeight/3};
                    SDL_RenderFillRect(renderer, &torso);
                    
                    // Brazos (más oscuros a los lados)
                    if(distFromCenter > spriteWidth * 0.3) {
                        SDL_SetRenderDrawColor(renderer, 
                            litColor/4 * shadeFactor, litColor/4 * shadeFactor, litColor/4 * shadeFactor, 255);
                        SDL_Rect arm = {stripe, drawStartY + spriteHeight/2, 1, spriteHeight/4};
                        SDL_RenderFillRect(renderer, &arm);
                    }
                    
                    // Cabeza (piel)
                    SDL_SetRenderDrawColor(renderer, 
                        litColor * shadeFactor, litColor*0.8 * shadeFactor, litColor*0.6 * shadeFactor, 255);
                    SDL_Rect head = {stripe, drawStartY + spriteHeight/8, 1, spriteHeight/5};
                    SDL_RenderFillRect(renderer, &head);
                    
                    // Casco (verde oscuro)
                    SDL_SetRenderDrawColor(renderer, 
                        30 * shadeFactor * enemyLight, 60 * shadeFactor * enemyLight, 30 * shadeFactor * enemyLight, 255);
                    SDL_Rect helmet = {stripe, drawStartY, 1, spriteHeight/10};
                    SDL_RenderFillRect(renderer, &helmet);
                    
                    // Banda roja (Nazi)
                    if(distFromCenter < spriteWidth * 0.2) {
                        SDL_SetRenderDrawColor(renderer, 180 * shadeFactor * enemyLight, 0, 0, 255);
                        SDL_Rect band = {stripe, drawStartY + spriteHeight/2, 1, spriteHeight/12};
                        SDL_RenderFillRect(renderer, &band);
                    }
                }
                else { // DOG
                    // PERRO PASTOR ALEMÁN - Más detallado y realista
                    int dogHeight = spriteHeight * 0.65;
                    int dogStartY = drawStartY + spriteHeight * 0.35;
                    
                    // Patas traseras
                    if(relativeX > spriteWidth * 0.6 || relativeX < spriteWidth * 0.4) {
                        SDL_SetRenderDrawColor(renderer, 
                            50 * shadeFactor * enemyLight, 35 * shadeFactor * enemyLight, 15 * shadeFactor * enemyLight, 255);
                        SDL_Rect leg = {stripe, int(dogStartY + dogHeight * 0.5), 1, int(dogHeight * 0.5)};
                        SDL_RenderFillRect(renderer, &leg);
                    }
                    
                    // Cuerpo (pelaje marrón oscuro con gradiente)
                    int bodyColor = 90 + (relativeX % 5);
                    SDL_SetRenderDrawColor(renderer, 
                        bodyColor * shadeFactor * enemyLight, (bodyColor * 0.6) * shadeFactor * enemyLight, 20 * shadeFactor * enemyLight, 255);
                    SDL_Rect body = {stripe, int(dogStartY + dogHeight * 0.2), 1, int(dogHeight * 0.4)};
                    SDL_RenderFillRect(renderer, &body);
                    
                    // Pecho/vientre (más claro)
                    if(distFromCenter < spriteWidth * 0.25) {
                        SDL_SetRenderDrawColor(renderer, 
                            120 * shadeFactor * enemyLight, 90 * shadeFactor * enemyLight, 50 * shadeFactor * enemyLight, 255);
                        SDL_Rect chest = {stripe, int(dogStartY + dogHeight * 0.35), 1, int(dogHeight * 0.25)};
                        SDL_RenderFillRect(renderer, &chest);
                    }
                    
                    // Hombros/espalda (negro)
                    SDL_SetRenderDrawColor(renderer, 
                        30 * shadeFactor * enemyLight, 25 * shadeFactor * enemyLight, 20 * shadeFactor * enemyLight, 255);
                    SDL_Rect back = {stripe, int(dogStartY + dogHeight * 0.15), 1, int(dogHeight * 0.2)};
                    SDL_RenderFillRect(renderer, &back);
                    
                    // Cabeza (hocico prominente)
                    SDL_SetRenderDrawColor(renderer, 
                        85 * shadeFactor * enemyLight, 60 * shadeFactor * enemyLight, 30 * shadeFactor * enemyLight, 255);
                    SDL_Rect head = {stripe, int(dogStartY), 1, int(dogHeight * 0.35)};
                    SDL_RenderFillRect(renderer, &head);
                    
                    // Hocico (más oscuro, puntiagudo)
                    if(distFromCenter < spriteWidth * 0.2) {
                        SDL_SetRenderDrawColor(renderer, 
                            40 * shadeFactor * enemyLight, 30 * shadeFactor * enemyLight, 20 * shadeFactor * enemyLight, 255);
                        SDL_Rect snout = {stripe, int(dogStartY + dogHeight * 0.15), 1, int(dogHeight * 0.15)};
                        SDL_RenderFillRect(renderer, &snout);
                    }
                    
                    // Orejas puntiagudas (hacia arriba)
                    if(distFromCenter < spriteWidth * 0.15 && relativeX % 7 < 3) {
                        SDL_SetRenderDrawColor(renderer, 
                            60 * shadeFactor * enemyLight, 45 * shadeFactor * enemyLight, 25 * shadeFactor * enemyLight, 255);
                        SDL_Rect ear = {stripe, int(dogStartY - dogHeight * 0.1), 1, int(dogHeight * 0.15)};
                        SDL_RenderFillRect(renderer, &ear);
                    }
                    
                    // Ojos amarillos brillantes (lupinos) - keep bright
                    if(distFromCenter < spriteWidth * 0.18 && relativeX % 5 < 2) {
                        SDL_SetRenderDrawColor(renderer, 255 * enemyLight, 200 * enemyLight, 50 * enemyLight, 255);
                        SDL_Rect eye = {stripe, int(dogStartY + dogHeight * 0.08), 1, 2};
                        SDL_RenderFillRect(renderer, &eye);
                    }
                    
                    // Dientes visibles (agresivo) - keep bright
                    if(distFromCenter < spriteWidth * 0.12 && relativeX % 4 == 0) {
                        SDL_SetRenderDrawColor(renderer, 255 * enemyLight, 255 * enemyLight, 255 * enemyLight, 255);
                        SDL_Rect tooth = {stripe, int(dogStartY + dogHeight * 0.22), 1, 2};
                        SDL_RenderFillRect(renderer, &tooth);
                    }
                    
                    // Patas delanteras
                    if(relativeX % 6 < 3) {
                        SDL_SetRenderDrawColor(renderer, 
                            55 * shadeFactor * enemyLight, 40 * shadeFactor * enemyLight, 20 * shadeFactor * enemyLight, 255);
                        SDL_Rect frontLeg = {stripe, int(dogStartY + dogHeight * 0.5), 1, int(dogHeight * 0.5)};
                        SDL_RenderFillRect(renderer, &frontLeg);
                    }
                    
                    // Cola (detrás, levantada)
                    if(distFromCenter > spriteWidth * 0.3) {
                        SDL_SetRenderDrawColor(renderer, 
                            70 * shadeFactor * enemyLight, 50 * shadeFactor * enemyLight, 25 * shadeFactor * enemyLight, 255);
                        SDL_Rect tail = {stripe, int(dogStartY + dogHeight * 0.1), 1, int(dogHeight * 0.3)};
                        SDL_RenderFillRect(renderer, &tail);
                    }
                }
            }
        }
    }
}

void drawItems() {
    // Draw items in the world
    for(auto& item : items) {
        if(!item.active) continue;
        
        double spriteX = item.x - posX;
        double spriteY = item.y - posY;
        
        // Transform to camera space
        double invDet = 1.0 / (planeX * dirY - dirX * planeY);
        double transformX = invDet * (dirY * spriteX - dirX * spriteY);
        double transformY = invDet * (-planeY * spriteX + planeX * spriteY);
        
        if(transformY <= 0) continue; // Behind player
        
        int spriteScreenX = int((SCREEN_WIDTH / 2) * (1 + transformX / transformY));
        
        int spriteSize = abs(int(SCREEN_HEIGHT / transformY * 0.4));
        
        // Aplicar efecto de salto (igual que las paredes, piso y enemigos)
        int jumpOffset = int(verticalPosition * SCREEN_HEIGHT * 0.02);
        
        int drawStartY = -spriteSize / 2 + SCREEN_HEIGHT / 2 + 30 + jumpOffset; // On ground
        int drawStartX = -spriteSize / 2 + spriteScreenX;
        
        if(drawStartX < 0 || drawStartX >= SCREEN_WIDTH) continue;
        if(drawStartY < 0 || drawStartY >= SCREEN_HEIGHT) continue;
        
        // Z-buffer check: solo dibujar si está delante de paredes
        if(spriteScreenX >= 0 && spriteScreenX < SCREEN_WIDTH && transformY >= zBuffer[spriteScreenX]) continue;
        
        // Calculate lighting at item position
        float itemLight = calculateLightingAt(item.x, item.y);
        
        // Apply distance fog
        float fogFactor = 1.0f - (transformY / 20.0f);
        if(fogFactor < 0.0f) fogFactor = 0.0f;
        itemLight *= fogFactor;
        
        // Draw item sprite
        if(item.type == ITEM_AMMO) {
            // Yellow ammo box
            int r = 200, g = 200, b = 0;
            applyLighting(r, g, b, itemLight);
            SDL_SetRenderDrawColor(renderer, r, g, b, 255);
            SDL_Rect itemRect = {drawStartX, drawStartY, spriteSize, spriteSize/2};
            SDL_RenderFillRect(renderer, &itemRect);
            
            r = 100; g = 100; b = 0;
            applyLighting(r, g, b, itemLight);
            SDL_SetRenderDrawColor(renderer, r, g, b, 255);
            SDL_RenderDrawRect(renderer, &itemRect);
        } else {
            // Red health pack
            int r = 200, g = 0, b = 0;
            applyLighting(r, g, b, itemLight);
            SDL_SetRenderDrawColor(renderer, r, g, b, 255);
            SDL_Rect itemRect = {drawStartX, drawStartY, spriteSize, spriteSize/2};
            SDL_RenderFillRect(renderer, &itemRect);
            
            // White cross
            r = 255; g = 255; b = 255;
            applyLighting(r, g, b, itemLight);
            SDL_SetRenderDrawColor(renderer, r, g, b, 255);
            SDL_Rect cross1 = {drawStartX + spriteSize/2 - 2, drawStartY + 4, 4, spriteSize/2 - 8};
            SDL_Rect cross2 = {drawStartX + 4, drawStartY + spriteSize/4 - 2, spriteSize - 8, 4};
            SDL_RenderFillRect(renderer, &cross1);
            SDL_RenderFillRect(renderer, &cross2);
        }
    }
}

void checkItemPickup() {
    for(auto& item : items) {
        if(!item.active) continue;
        
        double dx = item.x - posX;
        double dy = item.y - posY;
        double dist = sqrt(dx * dx + dy * dy);
        
        if(dist < 0.8) {
            if(item.type == ITEM_AMMO) {
                ammo += 30;
                printf("Picked up ammo! Now: %d\n", ammo);
                if(pickupAmmoSound) Mix_PlayChannel(-1, pickupAmmoSound, 0);
            } else {
                playerHealth += 25;
                if(playerHealth > 100) playerHealth = 100;
                printf("Picked up health! Now: %d\n", playerHealth);
                if(pickupHealthSound) Mix_PlayChannel(-1, pickupHealthSound, 0);
            }
            item.active = false;
            
            // Deactivate the item's light
            if(item.lightIndex >= 0 && item.lightIndex < (int)lightSources.size()) {
                lightSources[item.lightIndex].active = false;
            }
        }
    }
}

void handleInput() {
    // Si está muerto o ganó, permitir reiniciar, ver replay o volver al menú
    if(gameOver || gameWon) {
        SDL_Event event;
        while(SDL_PollEvent(&event)) {
            if(event.type == SDL_QUIT) {
                running = false;
            }
            if(event.type == SDL_KEYDOWN) {
                // R: Reiniciar juego
                if(event.key.keysym.sym == SDLK_r) {
                    // Reiniciar todas las variables del juego
                    playerHealth = 100;
                    ammo = 50;
                    kills = 0;
                    bulletsFired = 0;
                    gameOver = false;
                    gameWon = false;
                    posX = 2.0;
                    posY = 2.0;
                    dirX = -1.0;
                    dirY = 0.0;
                    planeX = 0.0;
                    planeY = 0.66;
                    currentWeapon = PISTOL;
                    cameraRoll = 0;
                    verticalPosition = 0;
                    verticalVelocity = 0;
                    isJumping = false;
                    
                    // Limpiar balas y beams
                    bullets.clear();
                    enemyBullets.clear();
                    laserBeams.clear();
                    
                    // Reiniciar enemigos
                    enemies.clear();
                    initEnemies();
                    
                    // Reiniciar items
                    items.clear();
                    initItems();
                    
                    // REPLAY: Clear recording for new session
                    clearRecording();
                    
                    printf("Juego reiniciado! Posición inicial: (%.1f, %.1f), Enemigos: %d\n", posX, posY, totalEnemies);
                }
                // U: Ver replay
                else if(event.key.keysym.sym == SDLK_u) {
                    printf(">>> Iniciando replay desde pantalla de Game Over/Victory...\n");
                    startReplay();
                }
                // ESC: Volver al menú principal
                else if(event.key.keysym.sym == SDLK_ESCAPE) {
                    printf(">>> Volviendo al menú principal...\n");
                    currentGameState = STATE_MAIN_MENU;
                    gameOver = false;
                    gameWon = false;
                    
                    // Resetear juego para la próxima partida
                    playerHealth = 100;
                    ammo = 50;
                    kills = 0;
                    bulletsFired = 0;
                    posX = 2.0;
                    posY = 2.0;
                    dirX = -1.0;
                    dirY = 0.0;
                    planeX = 0.0;
                    planeY = 0.66;
                    currentWeapon = PISTOL;
                    cameraRoll = 0;
                    verticalPosition = 0;
                    verticalVelocity = 0;
                    isJumping = false;
                    
                    bullets.clear();
                    enemyBullets.clear();
                    laserBeams.clear();
                    enemies.clear();
                    items.clear();
                }
            }
        }
        return; // No procesar más input si está muerto/ganó
    }
    
    // Reset input state each frame
    currentInput = InputState();
    
    SDL_Event event;
    while(SDL_PollEvent(&event)) {
        if(event.type == SDL_QUIT) {
            running = false;
        }
        if(event.type == SDL_KEYDOWN) {
            // Cambiar arma con números
            if(event.key.keysym.sym == SDLK_1) {
                currentWeapon = PISTOL;
                printf("Arma: Pistola\n");
            }
            else if(event.key.keysym.sym == SDLK_2) {
                currentWeapon = MACHINEGUN;
                printf("Arma: Ametralladora\n");
            }
            else if(event.key.keysym.sym == SDLK_3) {
                currentWeapon = LASER;
                printf("Arma: Laser\n");
            }
            else if(event.key.keysym.sym == SDLK_4) {
                currentWeapon = SHOTGUN;
                printf("Arma: Escopeta\n");
            }
            // Toggle flashlight with F
            else if(event.key.keysym.sym == SDLK_f) {
                flashlightEnabled = !flashlightEnabled;
                printf("Flashlight: %s\n", flashlightEnabled ? "ON" : "OFF");
            }
            // REPLAY: Enter replay mode with 'U'
            else if(event.key.keysym.sym == SDLK_u) {
                printf(">>> U key pressed! Recorded frames: %d\n", (int)recordedFrames.size());
                startReplay();
            }
            // Disparar con B (A y D son para strafe)
            else if(event.key.keysym.sym == SDLK_b) {
                shoot();
            }
            // Saltar con barra espaciadora
            else if(event.key.keysym.sym == SDLK_SPACE) {
                if(!isJumping && verticalPosition == 0) {
                    verticalVelocity = JUMP_FORCE;
                    isJumping = true;
                    printf("¡Salto!\n");
                }
            }
            else if(event.key.keysym.sym == SDLK_LCTRL) {
                shoot();
            }
        }
        if(event.type == SDL_KEYUP) {
            if(event.key.keysym.sym == SDLK_b || event.key.keysym.sym == SDLK_LCTRL) {
                laserActive = false;
            }
        }
        if(event.type == SDL_MOUSEBUTTONDOWN) {
            if(event.button.button == SDL_BUTTON_LEFT) {
                int mouseX = event.button.x;
                int mouseY = event.button.y;
                
                // Check if clicking on light slider
                int sliderX = SCREEN_WIDTH / 2 - 100;
                int sliderY = 10;
                int sliderWidth = 200;
                int sliderHeight = 20;
                
                if(mouseX >= sliderX && mouseX <= sliderX + sliderWidth &&
                   mouseY >= sliderY && mouseY <= sliderY + sliderHeight) {
                    // Clicked on slider - start dragging
                    sliderDragging = true;
                    
                    // Update value immediately
                    float normalizedX = (mouseX - sliderX) / (float)sliderWidth;
                    globalLightMultiplier = normalizedX * 2.0f; // Map to 0.0-2.0 range
                    if(globalLightMultiplier < 0.0f) globalLightMultiplier = 0.0f;
                    if(globalLightMultiplier > 2.0f) globalLightMultiplier = 2.0f;
                    
                    printf("Light intensity: %.2f\n", globalLightMultiplier);
                } else {
                    // Not on slider - shoot
                    shoot();
                }
            }
        }
        if(event.type == SDL_MOUSEBUTTONUP) {
            if(event.button.button == SDL_BUTTON_LEFT) {
                sliderDragging = false;
                laserActive = false;
            }
        }
        if(event.type == SDL_MOUSEMOTION && sliderDragging) {
            // Update slider while dragging
            int mouseX = event.motion.x;
            int sliderX = SCREEN_WIDTH / 2 - 100;
            int sliderWidth = 200;
            
            float normalizedX = (mouseX - sliderX) / (float)sliderWidth;
            globalLightMultiplier = normalizedX * 2.0f; // Map to 0.0-2.0 range
            if(globalLightMultiplier < 0.0f) globalLightMultiplier = 0.0f;
            if(globalLightMultiplier > 2.0f) globalLightMultiplier = 2.0f;
        }
    }
    
    const Uint8* keystate = SDL_GetKeyboardState(NULL);
    double moveSpeed = 0.05;
    
    // REPLAY: Capture inputs for AI training
    currentInput.forward = keystate[SDL_SCANCODE_W] || keystate[SDL_SCANCODE_UP];
    currentInput.backward = keystate[SDL_SCANCODE_S] || keystate[SDL_SCANCODE_DOWN];
    currentInput.strafeLeft = keystate[SDL_SCANCODE_A];
    currentInput.strafeRight = keystate[SDL_SCANCODE_D];
    currentInput.rotateLeft = keystate[SDL_SCANCODE_LEFT];
    currentInput.rotateRight = keystate[SDL_SCANCODE_RIGHT];
    currentInput.shoot = keystate[SDL_SCANCODE_B] || keystate[SDL_SCANCODE_LCTRL];
    currentInput.jump = keystate[SDL_SCANCODE_SPACE];
    currentInput.sprint = keystate[SDL_SCANCODE_LSHIFT];
    currentInput.weaponSelected = currentWeapon;
    
    // Disparo continuo para ametralladora, láser y escopeta con B o Ctrl
    if(keystate[SDL_SCANCODE_B] || keystate[SDL_SCANCODE_LCTRL]) {
        if(currentWeapon == MACHINEGUN || currentWeapon == LASER || currentWeapon == SHOTGUN) {
            shoot();
        }
    } else {
        laserActive = false;
    }
    
    // Sprint
    if(keystate[SDL_SCANCODE_LSHIFT]) {
        moveSpeed *= 1.5;
    }
    
    // Forward/backward con W/S
    if(keystate[SDL_SCANCODE_W] || keystate[SDL_SCANCODE_UP]) {
        if(worldMap[int(posX + dirX * moveSpeed)][int(posY)] == 0) posX += dirX * moveSpeed;
        if(worldMap[int(posX)][int(posY + dirY * moveSpeed)] == 0) posY += dirY * moveSpeed;
    }
    if(keystate[SDL_SCANCODE_S] || keystate[SDL_SCANCODE_DOWN]) {
        if(worldMap[int(posX - dirX * moveSpeed)][int(posY)] == 0) posX -= dirX * moveSpeed;
        if(worldMap[int(posX)][int(posY - dirY * moveSpeed)] == 0) posY -= dirY * moveSpeed;
    }
    
    // Strafe left/right con A/D (movimiento lateral)
    if(keystate[SDL_SCANCODE_A]) {
        // Strafe izquierda (perpendicular a la dirección)
        double strafeX = -dirY;
        double strafeY = dirX;
        if(worldMap[int(posX + strafeX * moveSpeed)][int(posY)] == 0) posX += strafeX * moveSpeed;
        if(worldMap[int(posX)][int(posY + strafeY * moveSpeed)] == 0) posY += strafeY * moveSpeed;
    }
    if(keystate[SDL_SCANCODE_D]) {
        // Strafe derecha (perpendicular a la dirección)
        double strafeX = dirY;
        double strafeY = -dirX;
        if(worldMap[int(posX + strafeX * moveSpeed)][int(posY)] == 0) posX += strafeX * moveSpeed;
        if(worldMap[int(posX)][int(posY + strafeY * moveSpeed)] == 0) posY += strafeY * moveSpeed;
    }
    
    // Rotar con flechas izquierda/derecha
    double rotSpeed = 0.03;
    if(keystate[SDL_SCANCODE_LEFT]) {
        double oldDirX = dirX;
        dirX = dirX * cos(rotSpeed) - dirY * sin(rotSpeed);
        dirY = oldDirX * sin(rotSpeed) + dirY * cos(rotSpeed);
        double oldPlaneX = planeX;
        planeX = planeX * cos(rotSpeed) - planeY * sin(rotSpeed);
        planeY = oldPlaneX * sin(rotSpeed) + planeY * cos(rotSpeed);
    }
    if(keystate[SDL_SCANCODE_RIGHT]) {
        double oldDirX = dirX;
        dirX = dirX * cos(-rotSpeed) - dirY * sin(-rotSpeed);
        dirY = oldDirX * sin(-rotSpeed) + dirY * cos(-rotSpeed);
        double oldPlaneX = planeX;
        planeX = planeX * cos(-rotSpeed) - planeY * sin(-rotSpeed);
        planeY = oldPlaneX * sin(-rotSpeed) + planeY * cos(-rotSpeed);
    }
    
    // Física del salto
    if(isJumping || verticalPosition > 0) {
        verticalVelocity -= GRAVITY;
        verticalPosition += verticalVelocity;
        
        // Aterrizar
        if(verticalPosition <= 0) {
            verticalPosition = 0;
            verticalVelocity = 0;
            isJumping = false;
        }
    }
    
    // Update weapon animation
    if(weaponState == 1) {
        weaponTimer++;
        // Velocidad de disparo más lenta (valores más altos = más lento)
        int animSpeed = (currentWeapon == MACHINEGUN) ? 5 : 
                       (currentWeapon == SHOTGUN) ? 8 : 
                       (currentWeapon == LASER) ? 3 : 6;
        if(weaponTimer > animSpeed) {
            weaponTimer = 0;
            weaponFrame++;
            int maxFrames = (currentWeapon == LASER) ? 2 : 4;
            if(weaponFrame > maxFrames && currentWeapon != MACHINEGUN && !laserActive) {
                weaponState = 0;
                weaponFrame = 0;
            }
        }
    }
    
    // Check for item pickup
    checkItemPickup();
}

// Draw main menu
void drawMainMenu() {
    // Animated gradient background
    for(int y = 0; y < SCREEN_HEIGHT; y++) {
        int r = 10 + (y * 20) / SCREEN_HEIGHT;
        int g = 10 + (y * 30) / SCREEN_HEIGHT;
        int b = 40 + (y * 40) / SCREEN_HEIGHT;
        SDL_SetRenderDrawColor(renderer, r, g, b, 255);
        SDL_RenderDrawLine(renderer, 0, y, SCREEN_WIDTH, y);
    }
    
    // Decorative top bar
    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 100);
    SDL_Rect topBar = {0, 0, SCREEN_WIDTH, 8};
    SDL_RenderFillRect(renderer, &topBar);
    SDL_SetRenderDrawColor(renderer, 255, 100, 0, 100);
    SDL_Rect topBar2 = {0, 8, SCREEN_WIDTH, 4};
    SDL_RenderFillRect(renderer, &topBar2);
    
    // Title: "WOLFENSTEIN 3D" with 3D effect (MUCH bigger!)
    // Shadow layers
    drawText("WOLFENSTEIN 3D", SCREEN_WIDTH / 2 - 162, 53, 0, 0, 0, 3);
    drawText("WOLFENSTEIN 3D", SCREEN_WIDTH / 2 - 159, 50, 50, 0, 0, 3);
    drawText("WOLFENSTEIN 3D", SCREEN_WIDTH / 2 - 156, 47, 100, 0, 0, 3);
    // Main title
    drawText("WOLFENSTEIN 3D", SCREEN_WIDTH / 2 - 153, 44, 255, 50, 50, 3);
    
    // Subtitle
    drawText("RAYCASTING ENGINE", SCREEN_WIDTH / 2 - 102, 72, 150, 150, 200, 2);
    
    // Decorative border around menu area
    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
    SDL_SetRenderDrawColor(renderer, 100, 100, 150, 60);
    SDL_Rect menuFrame = {SCREEN_WIDTH/2 - 200, 240, 400, 300};
    SDL_RenderFillRect(renderer, &menuFrame);
    SDL_SetRenderDrawColor(renderer, 150, 150, 200, 150);
    SDL_RenderDrawRect(renderer, &menuFrame);
    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_NONE);
    
    // Menu buttons
    int buttonWidth = 320;
    int buttonHeight = 70;
    int buttonX = SCREEN_WIDTH / 2 - buttonWidth / 2;
    int buttonY = 270;
    
    // Nueva Partida button with gradient effect
    // Button shadow
    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 100);
    SDL_Rect newGameShadow = {buttonX + 4, buttonY + 4, buttonWidth, buttonHeight};
    SDL_RenderFillRect(renderer, &newGameShadow);
    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_NONE);
    
    // Button gradient
    for(int i = 0; i < buttonHeight; i++) {
        int r = 50 + (i * 40) / buttonHeight;
        int g = 150 - (i * 30) / buttonHeight;
        SDL_SetRenderDrawColor(renderer, r, g, 50, 255);
        SDL_RenderDrawLine(renderer, buttonX, buttonY + i, buttonX + buttonWidth, buttonY + i);
    }
    
    // Button border
    SDL_SetRenderDrawColor(renderer, 150, 255, 100, 255);
    SDL_Rect newGameBorder = {buttonX - 3, buttonY - 3, buttonWidth + 6, buttonHeight + 6};
    SDL_RenderDrawRect(renderer, &newGameBorder);
    SDL_Rect newGameBorder2 = {buttonX - 2, buttonY - 2, buttonWidth + 4, buttonHeight + 4};
    SDL_RenderDrawRect(renderer, &newGameBorder2);
    
    // Draw "NEW GAME" text with shadow (MUCH bigger!)
    drawText("NEW GAME", buttonX + 51, buttonY + 22, 0, 0, 0, 3);
    drawText("NEW GAME", buttonX + 50, buttonY + 21, 255, 255, 255, 3);
    
    // Keyboard shortcut hint
    drawText("(PRESS N)", buttonX + 100, buttonY + 46, 200, 255, 200, 2);
    
    // Visualizar Partida button
    buttonY += 80;
    
    if(showFileDropZone) {
        // Show file drop zone
        SDL_SetRenderDrawColor(renderer, 40, 40, 80, 255);
        SDL_Rect dropZone = {buttonX, buttonY, buttonWidth, buttonHeight * 2 + 20};
        SDL_RenderFillRect(renderer, &dropZone);
        
        // Dashed border
        SDL_SetRenderDrawColor(renderer, 150, 150, 200, 255);
        for(int i = 0; i < buttonWidth; i += 10) {
            SDL_RenderDrawPoint(renderer, buttonX + i, buttonY);
            SDL_RenderDrawPoint(renderer, buttonX + i, buttonY + buttonHeight * 2 + 20);
        }
        for(int i = 0; i < buttonHeight * 2 + 20; i += 10) {
            SDL_RenderDrawPoint(renderer, buttonX, buttonY + i);
            SDL_RenderDrawPoint(renderer, buttonX + buttonWidth, buttonY + i);
        }
        
        // Instructions text (BIGGER!)
        drawText("DROP FILE HERE", buttonX + 45, buttonY + 25, 200, 200, 255, 2);
        drawText("OR CLICK TO BROWSE", buttonX + 25, buttonY + 45, 200, 200, 255, 2);
        
        // File info if loaded
        if(jsonFileLoaded) {
            if(jsonFileValid) {
                char buffer[128];
                sprintf(buffer, "LOADED %d FRAMES", (int)recordedFrames.size());
                drawText(buffer, buttonX + 40, buttonY + 80, 100, 255, 100, 2);
            } else {
                drawText("INVALID FILE", buttonX + 60, buttonY + 80, 255, 100, 100, 2);
            }
        }
        
        // Play button if valid JSON
        if(jsonFileValid) {
            buttonY += buttonHeight * 2 + 40;
            SDL_SetRenderDrawColor(renderer, 120, 60, 120, 255);
            SDL_Rect playButton = {buttonX, buttonY, buttonWidth, buttonHeight};
            SDL_RenderFillRect(renderer, &playButton);
            SDL_SetRenderDrawColor(renderer, 255, 120, 255, 255);
            SDL_Rect playBorder = {buttonX - 2, buttonY - 2, buttonWidth + 4, buttonHeight + 4};
            SDL_RenderDrawRect(renderer, &playBorder);
            
            // "PLAY" text (BIGGER!)
            drawText("PLAY", buttonX + 115, buttonY + 22, 255, 255, 255, 3);
        }
    } else {
        // VIEW REPLAY button with gradient effect
        // Button shadow
        SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 100);
        SDL_Rect replayShadow = {buttonX + 4, buttonY + 4, buttonWidth, buttonHeight};
        SDL_RenderFillRect(renderer, &replayShadow);
        SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_NONE);
        
        // Button gradient (purple/blue theme)
        for(int i = 0; i < buttonHeight; i++) {
            int r = 60 + (i * 60) / buttonHeight;
            int b = 150 - (i * 30) / buttonHeight;
            SDL_SetRenderDrawColor(renderer, r, 80, b, 255);
            SDL_RenderDrawLine(renderer, buttonX, buttonY + i, buttonX + buttonWidth, buttonY + i);
        }
        
        // Button border
        SDL_SetRenderDrawColor(renderer, 150, 150, 255, 255);
        SDL_Rect replayBorder = {buttonX - 3, buttonY - 3, buttonWidth + 6, buttonHeight + 6};
        SDL_RenderDrawRect(renderer, &replayBorder);
        SDL_Rect replayBorder2 = {buttonX - 2, buttonY - 2, buttonWidth + 4, buttonHeight + 4};
        SDL_RenderDrawRect(renderer, &replayBorder2);
        
        // Draw "VIEW REPLAY" text with shadow (MUCH bigger!)
        drawText("VIEW REPLAY", buttonX + 15, buttonY + 22, 0, 0, 0, 3);
        drawText("VIEW REPLAY", buttonX + 14, buttonY + 21, 255, 255, 255, 3);
        
        // Hint text
        drawText("(LOAD CSV/JSON)", buttonX + 65, buttonY + 46, 200, 200, 255, 2);
    }
    
    // Draw enemy sprites on the sides of the menu
    // Left side - DOG (bigger sprite!)
    drawDogSprite(70, 300, 100);
    drawText("DOG", 95, 410, 200, 150, 100, 2);
    
    // Right side - SOLDIER (bigger sprite!)
    drawSoldierSprite(SCREEN_WIDTH - 170, 300, 100);
    drawText("SOLDIER", SCREEN_WIDTH - 197, 410, 150, 180, 200, 2);
    
    // Footer decorative line
    SDL_SetRenderDrawColor(renderer, 100, 100, 150, 255);
    SDL_RenderDrawLine(renderer, 50, SCREEN_HEIGHT - 65, SCREEN_WIDTH - 50, SCREEN_HEIGHT - 65);
    
    // Footer text with icons (MUCH bigger!)
    drawText("PRESS 'U' DURING GAMEPLAY TO RECORD", SCREEN_WIDTH / 2 - 204, SCREEN_HEIGHT - 50, 150, 200, 255, 2);
    
    // Version/credit
    drawText("RAYCASTING v1.0", 10, SCREEN_HEIGHT - 20, 80, 80, 120, 2);
}

// Handle main menu input
void handleMainMenuInput() {
    SDL_Event event;
    while(SDL_PollEvent(&event)) {
        if(event.type == SDL_QUIT) {
            running = false;
            return;
        }
        
        // Keyboard shortcuts
        if(event.type == SDL_KEYDOWN) {
            if(event.key.keysym.sym == SDLK_n) {
                // Press 'N' for New Game
                printf(">>> NEW GAME (keyboard shortcut)!\n");
                currentGameState = STATE_PLAYING;
                // Reset game completely
                posX = 2.0; posY = 2.0;
                dirX = -1.0; dirY = 0.0;
                planeX = 0.0; planeY = 0.66;
                playerHealth = 100;
                ammo = 50;
                kills = 0;
                bulletsFired = 0;
                gameOver = false;
                gameWon = false;
                currentWeapon = PISTOL;
                cameraRoll = 0;
                verticalPosition = 0;
                verticalVelocity = 0;
                isJumping = false;
                
                // Limpiar entidades
                bullets.clear();
                enemyBullets.clear();
                laserBeams.clear();
                enemies.clear();
                items.clear();
                
                // Reinicializar enemigos e items
                initEnemies();
                initItems();
                
                clearRecording();
                printf("Juego iniciado con %d enemigos\n", totalEnemies);
                return;
            }
        }
        
        if(event.type == SDL_MOUSEBUTTONDOWN && event.button.button == SDL_BUTTON_LEFT) {
            int mouseX = event.button.x;
            int mouseY = event.button.y;
            
            int buttonWidth = 320;
            int buttonHeight = 70;
            int buttonX = SCREEN_WIDTH / 2 - buttonWidth / 2;
            int buttonY = 270;
            int buttonSpacing = 80;
            
            // Check New Game button
            if(mouseX >= buttonX && mouseX <= buttonX + buttonWidth &&
               mouseY >= buttonY && mouseY <= buttonY + buttonHeight) {
                printf(">>> NEW GAME clicked!\n");
                currentGameState = STATE_PLAYING;
                // Reset game completely
                posX = 2.0; posY = 2.0;
                dirX = -1.0; dirY = 0.0;
                planeX = 0.0; planeY = 0.66;
                playerHealth = 100;
                ammo = 50;
                kills = 0;
                bulletsFired = 0;
                gameOver = false;
                gameWon = false;
                currentWeapon = PISTOL;
                cameraRoll = 0;
                verticalPosition = 0;
                verticalVelocity = 0;
                isJumping = false;
                
                // Limpiar entidades
                bullets.clear();
                enemyBullets.clear();
                laserBeams.clear();
                enemies.clear();
                items.clear();
                
                // Reinicializar enemigos e items
                initEnemies();
                initItems();
                
                clearRecording();
                printf("Juego iniciado con %d enemigos\n", totalEnemies);
                return;
            }
            
            // Check View Replay button
            buttonY += buttonSpacing;
            if(!showFileDropZone) {
                if(mouseX >= buttonX && mouseX <= buttonX + buttonWidth &&
                   mouseY >= buttonY && mouseY <= buttonY + buttonHeight) {
                    printf(">>> VIEW REPLAY clicked!\n");
                    showFileDropZone = true;
                    // Trigger file picker
                    EM_ASM({
                        var input = document.createElement('input');
                        input.type = 'file';
                        input.accept = '.csv,.json';  // Accept both CSV (preferred) and JSON (legacy)
                        input.onchange = function(e) {
                            var file = e.target.files[0];
                            if(file) {
                                Module.ccall('setJsonFilename', null, ['string'], [file.name]);
                                var reader = new FileReader();
                                reader.onload = function(event) {
                                    var dataStr = event.target.result;
                                    Module.ccall('loadReplayFromJSON', null, ['string'], [dataStr]);
                                };
                                reader.readAsText(file);
                            }
                        };
                        input.click();
                    });
                    return;
                }
            } else {
                // Check Play button (if JSON is valid)
                if(jsonFileValid) {
                    buttonY += buttonHeight * 2 + 40;
                    if(mouseX >= buttonX && mouseX <= buttonX + buttonWidth &&
                       mouseY >= buttonY && mouseY <= buttonY + buttonHeight) {
                        printf(">>> PLAY REPLAY clicked!\n");
                        // Start replay from loaded JSON
                        if(!recordedFrames.empty()) {
                            currentGameState = STATE_REPLAY_VIEWER;
                            startReplay();
                        }
                        return;
                    }
                }
            }
        }
    }
}

// Draw replay UI overlay
void drawReplayUI() {
    if(!isReplaying) return;
    
    // Semi-transparent overlay at top
    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 180);
    SDL_Rect topOverlay = {0, 0, SCREEN_WIDTH, 80};
    SDL_RenderFillRect(renderer, &topOverlay);
    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_NONE);
    
    // "REPLAY MODE" indicator (red circle + text)
    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
    // Red circle (recording indicator style)
    for(int i = 0; i < 20; i++) {
        for(int j = 0; j < 20; j++) {
            int dx = i - 10;
            int dy = j - 10;
            if(dx*dx + dy*dy <= 100) {
                SDL_RenderDrawPoint(renderer, 20 + i, 20 + j);
            }
        }
    }
    
    // "REPLAY" text (simple block letters)
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    // R
    SDL_Rect r1 = {50, 15, 3, 20};
    SDL_RenderFillRect(renderer, &r1);
    SDL_Rect r2 = {50, 15, 10, 3};
    SDL_RenderFillRect(renderer, &r2);
    SDL_Rect r3 = {50, 24, 10, 3};
    SDL_RenderFillRect(renderer, &r3);
    SDL_Rect r4 = {57, 15, 3, 10};
    SDL_RenderFillRect(renderer, &r4);
    SDL_Rect r5 = {57, 27, 3, 8};
    SDL_RenderFillRect(renderer, &r5);
    
    // E
    SDL_Rect e1 = {65, 15, 3, 20};
    SDL_RenderFillRect(renderer, &e1);
    SDL_Rect e2 = {65, 15, 10, 3};
    SDL_RenderFillRect(renderer, &e2);
    SDL_Rect e3 = {65, 24, 8, 3};
    SDL_RenderFillRect(renderer, &e3);
    SDL_Rect e4 = {65, 32, 10, 3};
    SDL_RenderFillRect(renderer, &e4);
    
    // P
    SDL_Rect p1 = {80, 15, 3, 20};
    SDL_RenderFillRect(renderer, &p1);
    SDL_Rect p2 = {80, 15, 10, 3};
    SDL_RenderFillRect(renderer, &p2);
    SDL_Rect p3 = {80, 24, 10, 3};
    SDL_RenderFillRect(renderer, &p3);
    SDL_Rect p4 = {87, 15, 3, 12};
    SDL_RenderFillRect(renderer, &p4);
    
    // L
    SDL_Rect l1 = {95, 15, 3, 20};
    SDL_RenderFillRect(renderer, &l1);
    SDL_Rect l2 = {95, 32, 10, 3};
    SDL_RenderFillRect(renderer, &l2);
    
    // A
    SDL_Rect a1 = {110, 15, 3, 20};
    SDL_RenderFillRect(renderer, &a1);
    SDL_Rect a2 = {110, 15, 10, 3};
    SDL_RenderFillRect(renderer, &a2);
    SDL_Rect a3 = {110, 24, 10, 3};
    SDL_RenderFillRect(renderer, &a3);
    SDL_Rect a4 = {117, 15, 3, 20};
    SDL_RenderFillRect(renderer, &a4);
    
    // Y
    SDL_Rect y1 = {125, 15, 3, 10};
    SDL_RenderFillRect(renderer, &y1);
    SDL_Rect y2 = {132, 15, 3, 10};
    SDL_RenderFillRect(renderer, &y2);
    SDL_Rect y3 = {128, 25, 3, 10};
    SDL_RenderFillRect(renderer, &y3);
    
    // INFO PANEL - Clean design with colored bars
    int totalFrames = (int)recordedFrames.size();
    int infoX = SCREEN_WIDTH - 200;
    
    // Box background for better readability
    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 150);
    SDL_Rect infoBox = {infoX - 5, 10, 195, 65};
    SDL_RenderFillRect(renderer, &infoBox);
    SDL_SetRenderDrawColor(renderer, 100, 100, 100, 255);
    SDL_RenderDrawRect(renderer, &infoBox);
    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_NONE);
    
    // FRAME - Yellow bar indicator
    SDL_SetRenderDrawColor(renderer, 255, 255, 0, 255);
    SDL_Rect frameBar = {infoX, 15, 3, 12};
    SDL_RenderFillRect(renderer, &frameBar);
    
    char frameText[64];
    sprintf(frameText, "%d/%d", replayCurrentFrame, totalFrames);
    int xPos = infoX + 10;
    for(size_t i = 0; i < strlen(frameText); i++) {
        if(frameText[i] >= '0' && frameText[i] <= '9') {
            drawDigit(frameText[i] - '0', xPos, 15, 255, 255, 100);
            xPos += 14;
        } else {
            xPos += 10; // Space for '/'
        }
    }
    
    // SPEED - Cyan bar indicator  
    SDL_SetRenderDrawColor(renderer, 0, 255, 255, 255);
    SDL_Rect speedBar = {infoX, 35, 3, 12};
    SDL_RenderFillRect(renderer, &speedBar);
    
    char speedText[32];
    sprintf(speedText, "%.2fx", replaySpeed);
    xPos = infoX + 10;
    for(size_t i = 0; i < strlen(speedText); i++) {
        if(speedText[i] >= '0' && speedText[i] <= '9') {
            drawDigit(speedText[i] - '0', xPos, 35, 100, 255, 255);
            xPos += 14;
        } else if(speedText[i] == '.') {
            SDL_Rect dot = {xPos + 5, 45, 3, 3};
            SDL_SetRenderDrawColor(renderer, 100, 255, 255, 255);
            SDL_RenderFillRect(renderer, &dot);
            xPos += 10;
        } else {
            xPos += 12; // Space for 'x'
        }
    }
    
    // TIME - Orange bar indicator
    if(!recordedFrames.empty()) {
        SDL_SetRenderDrawColor(renderer, 255, 180, 0, 255);
        SDL_Rect timeBar = {infoX, 55, 3, 12};
        SDL_RenderFillRect(renderer, &timeBar);
        
        double currentTime = recordedFrames[replayCurrentFrame].timestamp;
        double totalTime = recordedFrames.back().timestamp;
        
        char timeText[32];
        sprintf(timeText, "%.1f/%.1f", currentTime, totalTime);
        xPos = infoX + 10;
        for(size_t i = 0; i < strlen(timeText); i++) {
            if(timeText[i] >= '0' && timeText[i] <= '9') {
                drawDigit(timeText[i] - '0', xPos, 55, 255, 200, 100);
                xPos += 14;
            } else if(timeText[i] == '.') {
                SDL_Rect dot = {xPos + 5, 65, 3, 3};
                SDL_SetRenderDrawColor(renderer, 255, 200, 100, 255);
                SDL_RenderFillRect(renderer, &dot);
                xPos += 10;
            } else {
                xPos += 10; // Space for '/'
            }
        }
    }
    
    // Paused indicator
    if(replayPaused) {
        SDL_SetRenderDrawColor(renderer, 255, 50, 50, 255);
        // Draw "PAUSED"
        SDL_Rect pause1 = {SCREEN_WIDTH/2 - 30, 15, 8, 20};
        SDL_RenderFillRect(renderer, &pause1);
        SDL_Rect pause2 = {SCREEN_WIDTH/2 - 15, 15, 8, 20};
        SDL_RenderFillRect(renderer, &pause2);
    }
    
    // Timeline bar at bottom
    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 180);
    SDL_Rect bottomOverlay = {0, SCREEN_HEIGHT - 90, SCREEN_WIDTH, 90};
    SDL_RenderFillRect(renderer, &bottomOverlay);
    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_NONE);
    
    // CONTROL BUTTONS
    int buttonY = SCREEN_HEIGHT - 75;
    int buttonSize = 30;
    int buttonSpacing = 40;
    int startX = 20;
    
    // Play/Pause button
    SDL_SetRenderDrawColor(renderer, replayPaused ? 100 : 0, replayPaused ? 200 : 255, 100, 255);
    SDL_Rect playPauseBtn = {startX, buttonY, buttonSize, buttonSize};
    SDL_RenderFillRect(renderer, &playPauseBtn);
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderDrawRect(renderer, &playPauseBtn);
    // Draw icon
    if(replayPaused) {
        // Play triangle
        for(int i = 0; i < buttonSize/2; i++) {
            SDL_RenderDrawLine(renderer, startX + 8 + i/2, buttonY + 8 + i, 
                             startX + 8 + i/2, buttonY + 22 - i);
        }
    } else {
        // Pause bars
        SDL_Rect bar1 = {startX + 9, buttonY + 8, 4, 14};
        SDL_Rect bar2 = {startX + 17, buttonY + 8, 4, 14};
        SDL_RenderFillRect(renderer, &bar1);
        SDL_RenderFillRect(renderer, &bar2);
    }
    
    // Speed - button
    startX += buttonSpacing;
    SDL_SetRenderDrawColor(renderer, 255, 150, 0, 255);
    SDL_Rect speedMinusBtn = {startX, buttonY, buttonSize, buttonSize};
    SDL_RenderFillRect(renderer, &speedMinusBtn);
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderDrawRect(renderer, &speedMinusBtn);
    SDL_Rect minus = {startX + 7, buttonY + 13, 16, 4};
    SDL_RenderFillRect(renderer, &minus);
    
    // Speed + button
    startX += buttonSpacing;
    SDL_SetRenderDrawColor(renderer, 255, 150, 0, 255);
    SDL_Rect speedPlusBtn = {startX, buttonY, buttonSize, buttonSize};
    SDL_RenderFillRect(renderer, &speedPlusBtn);
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderDrawRect(renderer, &speedPlusBtn);
    SDL_Rect plusH = {startX + 7, buttonY + 13, 16, 4};
    SDL_Rect plusV = {startX + 13, buttonY + 7, 4, 16};
    SDL_RenderFillRect(renderer, &plusH);
    SDL_RenderFillRect(renderer, &plusV);
    
    // Restart button
    startX += buttonSpacing;
    SDL_SetRenderDrawColor(renderer, 100, 150, 255, 255);
    SDL_Rect restartBtn = {startX, buttonY, buttonSize, buttonSize};
    SDL_RenderFillRect(renderer, &restartBtn);
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderDrawRect(renderer, &restartBtn);
    // R icon
    SDL_Rect rBar1 = {startX + 10, buttonY + 8, 3, 14};
    SDL_Rect rBar2 = {startX + 10, buttonY + 8, 8, 3};
    SDL_Rect rBar3 = {startX + 10, buttonY + 13, 8, 3};
    SDL_RenderFillRect(renderer, &rBar1);
    SDL_RenderFillRect(renderer, &rBar2);
    SDL_RenderFillRect(renderer, &rBar3);
    
    // Export button
    startX += buttonSpacing;
    SDL_SetRenderDrawColor(renderer, 200, 100, 200, 255);
    SDL_Rect exportBtn = {startX, buttonY, buttonSize, buttonSize};
    SDL_RenderFillRect(renderer, &exportBtn);
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderDrawRect(renderer, &exportBtn);
    // X icon
    SDL_Rect xBar1 = {startX + 8, buttonY + 8, 14, 3};
    SDL_Rect xBar2 = {startX + 8, buttonY + 19, 14, 3};
    SDL_RenderFillRect(renderer, &xBar1);
    SDL_RenderFillRect(renderer, &xBar2);
    
    // Timeline background (below buttons)
    int timelineX = 20;
    int timelineY = SCREEN_HEIGHT - 35;
    int timelineWidth = SCREEN_WIDTH - 40;
    int timelineHeight = 20;
    
    SDL_SetRenderDrawColor(renderer, 60, 60, 60, 255);
    SDL_Rect timelineBg = {timelineX, timelineY, timelineWidth, timelineHeight};
    SDL_RenderFillRect(renderer, &timelineBg);
    
    // Timeline progress
    if(totalFrames > 0) {
        int progressWidth = (replayCurrentFrame * timelineWidth) / totalFrames;
        SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
        SDL_Rect timelineProgress = {timelineX, timelineY, progressWidth, timelineHeight};
        SDL_RenderFillRect(renderer, &timelineProgress);
        
        // Current position marker
        int markerX = timelineX + progressWidth;
        SDL_SetRenderDrawColor(renderer, 255, 255, 0, 255);
        SDL_Rect marker = {markerX - 2, timelineY - 5, 4, timelineHeight + 10};
        SDL_RenderFillRect(renderer, &marker);
    }
    
    // Timeline border
    SDL_SetRenderDrawColor(renderer, 200, 200, 200, 255);
    SDL_RenderDrawRect(renderer, &timelineBg);
    
    // Re-draw HUD on top of overlay so it's visible
    // Health bar
    SDL_SetRenderDrawColor(renderer, 100, 0, 0, 200);
    SDL_Rect healthBg = {SCREEN_WIDTH - 250, SCREEN_HEIGHT - 80, 230, 25};
    SDL_RenderFillRect(renderer, &healthBg);
    SDL_SetRenderDrawColor(renderer, 200, 200, 200, 255);
    SDL_RenderDrawRect(renderer, &healthBg);
    
    // Get current health from replay frame
    int currentHealth = playerHealth;
    int currentAmmo = ammo;
    if(!recordedFrames.empty() && replayCurrentFrame < recordedFrames.size()) {
        currentHealth = recordedFrames[replayCurrentFrame].playerHealth;
        currentAmmo = recordedFrames[replayCurrentFrame].ammo;
    }
    
    SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
    int healthWidth = (currentHealth * 230) / 100;
    SDL_Rect health = {SCREEN_WIDTH - 250, SCREEN_HEIGHT - 80, healthWidth, 25};
    SDL_RenderFillRect(renderer, &health);
    
    // Health number
    drawNumber(currentHealth, SCREEN_WIDTH - 150, SCREEN_HEIGHT - 77, 255, 255, 255);
    
    // Ammo indicator
    drawText("AMMO:", SCREEN_WIDTH - 90, SCREEN_HEIGHT - 77, 200, 200, 0);
    drawNumber(currentAmmo, SCREEN_WIDTH - 45, SCREEN_HEIGHT - 77, 255, 255, 0);
}

void render() {
    // Clear screen
    SDL_SetRenderDrawColor(renderer, 50, 50, 50, 255);
    SDL_RenderClear(renderer);
    
    // Draw ceiling with lighting (simplified - darker ambient)
    for(int y = 0; y < SCREEN_HEIGHT / 2; y++) {
        // Keep sky gradient but apply darkness
        int skyBlue = 135 + (y * 30) / (SCREEN_HEIGHT / 2);
        int skyGreen = 206 - (y * 50) / (SCREEN_HEIGHT / 2);
        int skyRed = 235 - (y * 100) / (SCREEN_HEIGHT / 2);
        
        // Apply ambient darkness to ceiling
        float ceilingLight = ambientLight * 0.6f; // Ceiling darker
        applyLighting(skyRed, skyGreen, skyBlue, ceilingLight);
        
        for(int x = 0; x < SCREEN_WIDTH; x++) {
            SDL_SetRenderDrawColor(renderer, skyRed, skyGreen, skyBlue, 255);
            SDL_RenderDrawPoint(renderer, x, y);
        }
    }
    
    // Draw floor with lighting
    // Aplicar efecto de salto al piso
    int jumpOffset = int(verticalPosition * SCREEN_HEIGHT * 0.02);
    
    for(int y = SCREEN_HEIGHT / 2; y < SCREEN_HEIGHT; y++) {
        float rayDirX0 = dirX - planeX;
        float rayDirY0 = dirY - planeY;
        float rayDirX1 = dirX + planeX;
        float rayDirY1 = dirY + planeY;
        
        int p = y - SCREEN_HEIGHT / 2;
        float posZ = 0.5 * SCREEN_HEIGHT;
        float rowDistance = posZ / p;
        
        float floorStepX = rowDistance * (rayDirX1 - rayDirX0) / SCREEN_WIDTH;
        float floorStepY = rowDistance * (rayDirY1 - rayDirY0) / SCREEN_WIDTH;
        
        float floorX = posX + rowDistance * rayDirX0;
        float floorY = posY + rowDistance * rayDirY0;
        
        // Aplicar offset de salto a la posición Y del piso
        int floorScreenY = y + jumpOffset;
        
        // Solo dibujar si está dentro de los límites de la pantalla
        if(floorScreenY >= SCREEN_HEIGHT / 2 && floorScreenY < SCREEN_HEIGHT) {
            for(int x = 0; x < SCREEN_WIDTH; x++) {
                float lightLevel = calculateLightingAt(floorX, floorY);
                
                // Distance fog
                float fogFactor = 1.0f - (rowDistance / 20.0f);
                if(fogFactor < 0.0f) fogFactor = 0.0f;
                lightLevel *= fogFactor;
                
                int r = 70, g = 60, b = 50;
                applyLighting(r, g, b, lightLevel);
                
                SDL_SetRenderDrawColor(renderer, r, g, b, 255);
                SDL_RenderDrawPoint(renderer, x, floorScreenY);
                
                floorX += floorStepX;
                floorY += floorStepY;
            }
        }
    }
    
    // Raycasting for walls with textures
    for(int x = 0; x < SCREEN_WIDTH; x++) {
        double cameraX = 2 * x / double(SCREEN_WIDTH) - 1;
        double rayDirX = dirX + planeX * cameraX;
        double rayDirY = dirY + planeY * cameraX;
        
        int mapX = int(posX);
        int mapY = int(posY);
        
        double deltaDistX = (rayDirX == 0) ? 1e30 : std::abs(1 / rayDirX);
        double deltaDistY = (rayDirY == 0) ? 1e30 : std::abs(1 / rayDirY);
        
        double sideDistX, sideDistY;
        int stepX, stepY;
        
        if(rayDirX < 0) {
            stepX = -1;
            sideDistX = (posX - mapX) * deltaDistX;
        } else {
            stepX = 1;
            sideDistX = (mapX + 1.0 - posX) * deltaDistX;
        }
        if(rayDirY < 0) {
            stepY = -1;
            sideDistY = (posY - mapY) * deltaDistY;
        } else {
            stepY = 1;
            sideDistY = (mapY + 1.0 - posY) * deltaDistY;
        }
        
        int hit = 0;
        int side;
        
        while(hit == 0) {
            if(sideDistX < sideDistY) {
                sideDistX += deltaDistX;
                mapX += stepX;
                side = 0;
            } else {
                sideDistY += deltaDistY;
                mapY += stepY;
                side = 1;
            }
            
            if(worldMap[mapX][mapY] > 0) hit = 1;
        }
        
        double perpWallDist;
        if(side == 0) perpWallDist = (mapX - posX + (1 - stepX) / 2) / rayDirX;
        else perpWallDist = (mapY - posY + (1 - stepY) / 2) / rayDirY;
        
        int lineHeight = (int)(SCREEN_HEIGHT / perpWallDist);
        
        // Aplicar inclinación de cámara (camera roll) y salto
        int rollOffset = int(cameraRoll * (x - SCREEN_WIDTH / 2));
        int jumpOffset = int(verticalPosition * SCREEN_HEIGHT * 0.02); // Efecto sutil (10%)
        
        // INVERTIDO: Sumar el offset hace que la vista baje (el suelo sube)
        int drawStart = -lineHeight / 2 + SCREEN_HEIGHT / 2 + rollOffset + jumpOffset;
        if(drawStart < 0) drawStart = 0;
        int drawEnd = lineHeight / 2 + SCREEN_HEIGHT / 2 + rollOffset + jumpOffset;
        if(drawEnd >= SCREEN_HEIGHT) drawEnd = SCREEN_HEIGHT - 1;
        
        // Calculate wall X position for texturing
        double wallX;
        if(side == 0) wallX = posY + perpWallDist * rayDirY;
        else wallX = posX + perpWallDist * rayDirX;
        wallX -= std::floor(wallX);
        
        // Wall type determines base color
        int wallType = worldMap[mapX][mapY];
        int baseR, baseG, baseB;
        switch(wallType) {
            case 1: baseR = 180; baseG = 20; baseB = 20; break;   // Red brick
            case 2: baseR = 20; baseG = 180; baseB = 20; break;   // Green stone
            case 3: baseR = 20; baseG = 20; baseB = 180; break;   // Blue metal
            case 4: baseR = 180; baseG = 180; baseB = 20; break;  // Yellow stone
            default: baseR = 120; baseG = 120; baseB = 120; break;
        }
        
        // Draw textured wall column - SUPER OPTIMIZED con líneas
        int texX = int(wallX * 8) % 8;
        
        // Simplificar texturas aún más para velocidad
        int r = baseR, g = baseG, b = baseB;
        
        // Textura simple basada en tipo
        if(wallType == 1 && texX % 4 == 0) r *= 0.7; // Ladrillos
        else if(wallType == 2 && (texX + 1) % 2 == 0) r *= 0.85; // Piedra
        else if(wallType == 3 && (texX == 0 || texX == 7)) r *= 0.6; // Metal
        else if(wallType == 4 && texX == 0) { r = 60; g = 60; b = 60; } // Mortero
        
        // Calculate lighting at wall position
        double wallWorldX = mapX + 0.5;
        double wallWorldY = mapY + 0.5;
        float lightLevel = calculateLightingAt(wallWorldX, wallWorldY);
        
        // Apply distance fog
        float fogFactor = 1.0f - (perpWallDist / 20.0f); // Fog at 20 units
        if(fogFactor < 0.0f) fogFactor = 0.0f;
        lightLevel *= fogFactor;
        
        // Apply lighting to color
        applyLighting(r, g, b, lightLevel);
        
        // Darken for side
        if(side == 1) { r *= 0.8; g *= 0.8; b *= 0.8; }
        
        // Dibujar línea completa en vez de píxel por píxel (MUY RÁPIDO)
        SDL_SetRenderDrawColor(renderer, r, g, b, 255);
        SDL_RenderDrawLine(renderer, x, drawStart, x, drawEnd);
        
        // Guardar distancia en z-buffer para depth testing
        zBuffer[x] = perpWallDist;
    }
    
    // Draw items first (behind enemies)
    drawItems();
    
    // Draw enemies on top of walls
    drawEnemies();
    
    // Draw laser beam (rayo láser continuo)
    if(laserActive && currentWeapon == LASER) {
        // Raycast para el láser
        double laserDirX = dirX;
        double laserDirY = dirY;
        
        int mapX = int(posX);
        int mapY = int(posY);
        
        double deltaDistX = (laserDirX == 0) ? 1e30 : std::abs(1 / laserDirX);
        double deltaDistY = (laserDirY == 0) ? 1e30 : std::abs(1 / laserDirY);
        
        double sideDistX, sideDistY;
        int stepX, stepY;
        
        if(laserDirX < 0) {
            stepX = -1;
            sideDistX = (posX - mapX) * deltaDistX;
        } else {
            stepX = 1;
            sideDistX = (mapX + 1.0 - posX) * deltaDistX;
        }
        if(laserDirY < 0) {
            stepY = -1;
            sideDistY = (posY - mapY) * deltaDistY;
        } else {
            stepY = 1;
            sideDistY = (mapY + 1.0 - posY) * deltaDistY;
        }
        
        // Encontrar punto de impacto
        int hit = 0;
        int side;
        while(hit == 0) {
            if(sideDistX < sideDistY) {
                sideDistX += deltaDistX;
                mapX += stepX;
                side = 0;
            } else {
                sideDistY += deltaDistY;
                mapY += stepY;
                side = 1;
            }
            if(worldMap[mapX][mapY] > 0) hit = 1;
        }
        
        double laserDist;
        if(side == 0) laserDist = (mapX - posX + (1 - stepX) / 2) / laserDirX;
        else laserDist = (mapY - posY + (1 - stepY) / 2) / laserDirY;
        
        // Dibujar rayo láser rojo en el centro de la pantalla
        int centerY = SCREEN_HEIGHT / 2;
        SDL_SetRenderDrawColor(renderer, 255, 0, 0, 200);
        
        // Línea gruesa para el láser
        for(int i = -2; i <= 2; i++) {
            SDL_RenderDrawLine(renderer, SCREEN_WIDTH/2 - 100, centerY + i, SCREEN_WIDTH/2 + 100, centerY + i);
        }
        
        // Núcleo brillante
        SDL_SetRenderDrawColor(renderer, 255, 150, 150, 255);
        SDL_RenderDrawLine(renderer, SCREEN_WIDTH/2 - 100, centerY, SCREEN_WIDTH/2 + 100, centerY);
        
        // Dañar enemigos en el láser
        for(auto& enemy : enemies) {
            if(!enemy.alive) continue;
            
            double dx = enemy.x - posX;
            double dy = enemy.y - posY;
            double dist = sqrt(dx * dx + dy * dy);
            
            double angle = atan2(dy, dx) - atan2(laserDirY, laserDirX);
            if(std::abs(angle) < 0.1 && dist < laserDist) {
                enemy.health -= 1; // Daño continuo
                if(enemy.health <= 0) {
                    enemy.alive = false;
                    kills++;
                    if(enemyDeathSound) Mix_PlayChannel(-1, enemyDeathSound, 0);
                }
            }
        }
    }
    
    // Draw physical bullets (balas que se mueven por el espacio)
    for(const auto& bullet : bullets) {
        if(!bullet.active) continue;
        
        // Dibujar estela PRIMERO (más tenue y lenta)
        for(size_t i = 0; i < bullet.trail.size(); i++) {
            double trailX = bullet.trail[i].first;
            double trailY = bullet.trail[i].second;
            
            double relX = trailX - posX;
            double relY = trailY - posY;
            
            double invDet = 1.0 / (planeX * dirY - dirX * planeY);
            double transformX = invDet * (dirY * relX - dirX * relY);
            double transformY = invDet * (-planeY * relX + planeX * relY);
            
            if(transformY > 0.1) {
                int screenX = int((SCREEN_WIDTH / 2) * (1 + transformX / transformY));
                int screenY = SCREEN_HEIGHT / 2;
                
                if(screenX >= 0 && screenX < SCREEN_WIDTH && transformY < zBuffer[screenX]) {
                    // Alpha decrece con la edad de la estela
                    int alpha = (i * 255) / bullet.trail.size(); // Más vieja = más transparente
                    int trailSize = std::max(1, int(15 / transformY));
                    
                    // Estela amarilla tenue
                    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
                    SDL_SetRenderDrawColor(renderer, 255, 255, 0, alpha);
                    SDL_Rect trailRect = {screenX - trailSize/2, screenY - trailSize/2, trailSize, trailSize};
                    SDL_RenderFillRect(renderer, &trailRect);
                    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_NONE);
                }
            }
        }
        
        // Transform bullet position to screen coordinates
        double relX = bullet.x - posX;
        double relY = bullet.y - posY;
        
        double invDet = 1.0 / (planeX * dirY - dirX * planeY);
        double transformX = invDet * (dirY * relX - dirX * relY);
        double transformY = invDet * (-planeY * relX + planeX * relY);
        
        if(transformY > 0.1) {
            int screenX = int((SCREEN_WIDTH / 2) * (1 + transformX / transformY));
            int screenY = SCREEN_HEIGHT / 2;
            
            // Z-buffer check: solo dibujar si la bala está delante de las paredes
            if(screenX >= 0 && screenX < SCREEN_WIDTH && transformY < zBuffer[screenX]) {
                // Bala MÁS PEQUEÑA
                int bulletSize = std::max(2, int(40 / transformY));
                
                // Dibujar bala brillante (amarilla)
                SDL_SetRenderDrawColor(renderer, 255, 255, 0, 255);
                SDL_Rect bulletRect = {screenX - bulletSize/2, screenY - bulletSize/2, bulletSize, bulletSize};
                SDL_RenderFillRect(renderer, &bulletRect);
                
                // Borde blanco brillante (solo si es grande)
                if(bulletSize > 3) {
                    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
                    SDL_RenderDrawRect(renderer, &bulletRect);
                }
            }
        }
    }
    
    // Draw laser beams con estela lenta y roja
    for(const auto& beam : laserBeams) {
        if(!beam.active) continue;
        
        // Dibujar estela del láser (MUY lenta y duradera)
        for(size_t i = 0; i < beam.trail.size(); i++) {
            double trailX = beam.trail[i].first;
            double trailY = beam.trail[i].second;
            
            double relX = trailX - posX;
            double relY = trailY - posY;
            
            double invDet = 1.0 / (planeX * dirY - dirX * planeY);
            double transformX = invDet * (dirY * relX - dirX * relY);
            double transformY = invDet * (-planeY * relX + planeX * relY);
            
            if(transformY > 0.1) {
                int screenX = int((SCREEN_WIDTH / 2) * (1 + transformX / transformY));
                int screenY = SCREEN_HEIGHT / 2;
                
                if(screenX >= 0 && screenX < SCREEN_WIDTH && transformY < zBuffer[screenX]) {
                    // Alpha decrece más lento
                    int alpha = 100 + ((i * 155) / std::max((size_t)1, beam.trail.size()));
                    int beamSize = std::max(2, int(60 / transformY));
                    
                    // Estela roja del láser
                    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
                    SDL_SetRenderDrawColor(renderer, 255, 50, 50, alpha);
                    SDL_Rect beamRect = {screenX - beamSize/2, screenY - beamSize/2, beamSize, beamSize};
                    SDL_RenderFillRect(renderer, &beamRect);
                    
                    // Brillo central
                    if(i > beam.trail.size() * 0.7) {
                        SDL_SetRenderDrawColor(renderer, 255, 150, 150, alpha);
                        SDL_Rect glowRect = {screenX - beamSize/4, screenY - beamSize/4, beamSize/2, beamSize/2};
                        SDL_RenderFillRect(renderer, &glowRect);
                    }
                    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_NONE);
                }
            }
        }
    }
    
    // Draw enemy bullets (balas enemigas rojas viniendo hacia ti)
    for(const auto& bullet : enemyBullets) {
        if(!bullet.active) continue;
        
        // Transform bullet position to screen coordinates
        double relX = bullet.x - posX;
        double relY = bullet.y - posY;
        
        double invDet = 1.0 / (planeX * dirY - dirX * planeY);
        double transformX = invDet * (dirY * relX - dirX * relY);
        double transformY = invDet * (-planeY * relX + planeX * relY);
        
        if(transformY > 0.1) {
            int screenX = int((SCREEN_WIDTH / 2) * (1 + transformX / transformY));
            int screenY = SCREEN_HEIGHT / 2;
            
            // Z-buffer check
            if(screenX >= 0 && screenX < SCREEN_WIDTH && transformY < zBuffer[screenX]) {
                // Bala enemiga ROJA (más grande para ser visible)
                int bulletSize = std::max(3, int(50 / transformY));
                
                // Bala roja brillante con efecto de peligro
                SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
                SDL_Rect bulletRect = {screenX - bulletSize/2, screenY - bulletSize/2, bulletSize, bulletSize};
                SDL_RenderFillRect(renderer, &bulletRect);
                
                // Borde naranja brillante
                SDL_SetRenderDrawColor(renderer, 255, 150, 0, 255);
                SDL_RenderDrawRect(renderer, &bulletRect);
                
                // Estela roja detrás
                if(bulletSize > 4) {
                    SDL_SetRenderDrawColor(renderer, 200, 0, 0, 150);
                    SDL_Rect trail = {screenX - bulletSize/2 - 2, screenY - bulletSize/2 - 2, 
                                     bulletSize + 4, bulletSize + 4};
                    SDL_RenderDrawRect(renderer, &trail);
                }
            }
        }
    }
    
    // Draw weapon
    drawWeapon();
    
    // Draw HUD
    drawHUD();
    
    // Game over check
    if(playerHealth <= 0 && !gameOver) {
        gameOver = true;
    }
    
    // Victory check - Todos los enemigos muertos
    if(kills >= totalEnemies && !gameWon && !gameOver) {
        gameWon = true;
        printf("¡VICTORIA! Todos los enemigos eliminados\n");
    }
    
    // Game Over Screen con estadísticas
    if(gameOver) {
        // Overlay oscuro
        SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 200);
        SDL_Rect overlay = {0, 0, SCREEN_WIDTH, SCREEN_HEIGHT};
        SDL_RenderFillRect(renderer, &overlay);
        SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_NONE);
        
        // "GAME OVER" en rojo grande
        SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
        // G
        SDL_Rect gVertL = {150, 100, 15, 80};
        SDL_RenderFillRect(renderer, &gVertL);
        SDL_Rect gTop = {150, 100, 50, 15};
        SDL_RenderFillRect(renderer, &gTop);
        SDL_Rect gBot = {150, 165, 50, 15};
        SDL_RenderFillRect(renderer, &gBot);
        SDL_Rect gMid = {175, 140, 25, 10};
        SDL_RenderFillRect(renderer, &gMid);
        SDL_Rect gVertR = {185, 140, 15, 40};
        SDL_RenderFillRect(renderer, &gVertR);
        
        // A
        SDL_Rect aTop = {220, 100, 50, 15};
        SDL_RenderFillRect(renderer, &aTop);
        SDL_Rect aVertL = {220, 100, 15, 80};
        SDL_RenderFillRect(renderer, &aVertL);
        SDL_Rect aVertR = {255, 100, 15, 80};
        SDL_RenderFillRect(renderer, &aVertR);
        SDL_Rect aMid = {220, 140, 50, 10};
        SDL_RenderFillRect(renderer, &aMid);
        
        // M
        SDL_Rect mVertL = {290, 100, 15, 80};
        SDL_RenderFillRect(renderer, &mVertL);
        SDL_Rect mVertR = {345, 100, 15, 80};
        SDL_RenderFillRect(renderer, &mVertR);
        SDL_Rect mDiagL = {305, 110, 15, 20};
        SDL_RenderFillRect(renderer, &mDiagL);
        SDL_Rect mDiagR = {320, 110, 15, 20};
        SDL_RenderFillRect(renderer, &mDiagR);
        
        // E
        SDL_Rect eVertL = {380, 100, 15, 80};
        SDL_RenderFillRect(renderer, &eVertL);
        SDL_Rect eTop = {380, 100, 50, 15};
        SDL_RenderFillRect(renderer, &eTop);
        SDL_Rect eMid = {380, 137, 40, 10};
        SDL_RenderFillRect(renderer, &eMid);
        SDL_Rect eBot = {380, 165, 50, 15};
        SDL_RenderFillRect(renderer, &eBot);
        
        // OVER
        SDL_Rect oVertL1 = {160, 200, 15, 60};
        SDL_RenderFillRect(renderer, &oVertL1);
        SDL_Rect oVertR1 = {205, 200, 15, 60};
        SDL_RenderFillRect(renderer, &oVertR1);
        SDL_Rect oTop1 = {160, 200, 60, 15};
        SDL_RenderFillRect(renderer, &oTop1);
        SDL_Rect oBot1 = {160, 245, 60, 15};
        SDL_RenderFillRect(renderer, &oBot1);
        
        SDL_Rect vVertL = {240, 200, 15, 50};
        SDL_RenderFillRect(renderer, &vVertL);
        SDL_Rect vVertR = {275, 200, 15, 50};
        SDL_RenderFillRect(renderer, &vVertR);
        SDL_Rect vBot = {250, 250, 15, 10};
        SDL_RenderFillRect(renderer, &vBot);
        
        SDL_Rect eVertL2 = {310, 200, 15, 60};
        SDL_RenderFillRect(renderer, &eVertL2);
        SDL_Rect eTop2 = {310, 200, 50, 15};
        SDL_RenderFillRect(renderer, &eTop2);
        SDL_Rect eMid2 = {310, 227, 40, 10};
        SDL_RenderFillRect(renderer, &eMid2);
        SDL_Rect eBot2 = {310, 245, 50, 15};
        SDL_RenderFillRect(renderer, &eBot2);
        
        SDL_Rect rVertL2 = {380, 200, 15, 60};
        SDL_RenderFillRect(renderer, &rVertL2);
        SDL_Rect rTop2 = {380, 200, 40, 15};
        SDL_RenderFillRect(renderer, &rTop2);
        SDL_Rect rVertR2 = {405, 200, 15, 30};
        SDL_RenderFillRect(renderer, &rVertR2);
        SDL_Rect rMid2 = {380, 227, 40, 10};
        SDL_RenderFillRect(renderer, &rMid2);
        SDL_Rect rDiag = {395, 240, 20, 20};
        SDL_RenderFillRect(renderer, &rDiag);
        
        // Estadísticas
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        drawNumber(kills, 200, 290, 255, 255, 0);
        drawNumber(bulletsFired, 200, 320, 255, 255, 0);
        
        // Controles disponibles
        drawText("PRESS 'R' TO RESTART", 160, 370, 200, 200, 200);
        drawText("PRESS 'U' TO VIEW REPLAY", 140, 395, 100, 200, 255);
        drawText("PRESS 'ESC' TO MAIN MENU", 135, 420, 255, 200, 100);
    }
    
    // Victory Screen
    if(gameWon) {
        // Overlay oscuro
        SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 200);
        SDL_Rect overlay = {0, 0, SCREEN_WIDTH, SCREEN_HEIGHT};
        SDL_RenderFillRect(renderer, &overlay);
        SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_NONE);
        
        // "YOU WIN!" en verde/amarillo grande
        SDL_SetRenderDrawColor(renderer, 50, 255, 50, 255);
        
        // Y
        SDL_Rect yVertL = {120, 100, 15, 40};
        SDL_RenderFillRect(renderer, &yVertL);
        SDL_Rect yVertR = {155, 100, 15, 40};
        SDL_RenderFillRect(renderer, &yVertR);
        SDL_Rect yBot = {135, 140, 5, 40};
        SDL_RenderFillRect(renderer, &yBot);
        
        // O
        SDL_Rect oVertL = {190, 100, 15, 80};
        SDL_RenderFillRect(renderer, &oVertL);
        SDL_Rect oVertR = {235, 100, 15, 80};
        SDL_RenderFillRect(renderer, &oVertR);
        SDL_Rect oTop = {190, 100, 60, 15};
        SDL_RenderFillRect(renderer, &oTop);
        SDL_Rect oBot = {190, 165, 60, 15};
        SDL_RenderFillRect(renderer, &oBot);
        
        // U
        SDL_Rect uVertL = {270, 100, 15, 80};
        SDL_RenderFillRect(renderer, &uVertL);
        SDL_Rect uVertR = {315, 100, 15, 80};
        SDL_RenderFillRect(renderer, &uVertR);
        SDL_Rect uBot = {270, 165, 60, 15};
        SDL_RenderFillRect(renderer, &uBot);
        
        // W (segunda línea)
        SDL_SetRenderDrawColor(renderer, 255, 255, 50, 255);
        SDL_Rect wVertL = {150, 200, 15, 70};
        SDL_RenderFillRect(renderer, &wVertL);
        SDL_Rect wVertR = {230, 200, 15, 70};
        SDL_RenderFillRect(renderer, &wVertR);
        SDL_Rect wBot1 = {165, 255, 15, 15};
        SDL_RenderFillRect(renderer, &wBot1);
        SDL_Rect wBot2 = {200, 255, 15, 15};
        SDL_RenderFillRect(renderer, &wBot2);
        SDL_Rect wMid = {185, 240, 15, 30};
        SDL_RenderFillRect(renderer, &wMid);
        
        // I
        SDL_Rect iVert = {265, 200, 15, 70};
        SDL_RenderFillRect(renderer, &iVert);
        SDL_Rect iTop = {255, 200, 35, 15};
        SDL_RenderFillRect(renderer, &iTop);
        SDL_Rect iBot = {255, 255, 35, 15};
        SDL_RenderFillRect(renderer, &iBot);
        
        // N
        SDL_Rect nVertL = {310, 200, 15, 70};
        SDL_RenderFillRect(renderer, &nVertL);
        SDL_Rect nVertR = {360, 200, 15, 70};
        SDL_RenderFillRect(renderer, &nVertR);
        SDL_Rect nDiag1 = {325, 215, 15, 20};
        SDL_RenderFillRect(renderer, &nDiag1);
        SDL_Rect nDiag2 = {340, 235, 15, 20};
        SDL_RenderFillRect(renderer, &nDiag2);
        
        // ! (exclamación)
        SDL_SetRenderDrawColor(renderer, 255, 50, 50, 255);
        SDL_Rect excTop = {395, 200, 20, 50};
        SDL_RenderFillRect(renderer, &excTop);
        SDL_Rect excBot = {395, 260, 20, 10};
        SDL_RenderFillRect(renderer, &excBot);
        
        // Estadísticas
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        drawNumber(kills, 200, 310, 50, 255, 50);
        drawNumber(bulletsFired, 200, 340, 255, 255, 50);
        drawNumber(playerHealth, 200, 370, 255, 50, 50);
        
        // Controles disponibles
        drawText("PRESS 'R' TO RESTART", 160, 410, 200, 200, 200);
        drawText("PRESS 'U' TO VIEW REPLAY", 140, 435, 100, 200, 255);
        drawText("PRESS 'ESC' TO MAIN MENU", 135, 460, 255, 200, 100);
    }
    
    // Draw replay UI overlay (if in replay mode)
    drawReplayUI();
    
    SDL_RenderPresent(renderer);
}

void mainLoop() {
    static double lastTime = 0;
    double currentTime = SDL_GetTicks() / 1000.0;
    double deltaTime = currentTime - lastTime;
    lastTime = currentTime;
    
    // Handle different game states
    switch(currentGameState) {
        case STATE_MAIN_MENU:
            handleMainMenuInput();
            drawMainMenu();
            SDL_RenderPresent(renderer);
            break;
            
        case STATE_PLAYING:
            if(!isReplaying) {
                // Normal gameplay mode
                handleInput();
                updateEnemies(deltaTime);
                
                // Record frame automatically
                if(isRecording && !gameOver && !gameWon) {
                    recordFrame(currentTime);
                }
            } else {
                // Replay mode (triggered by U key during gameplay)
                handleReplayInput();
                updateReplay();
            }
            render();
            break;
            
        case STATE_REPLAY_VIEWER:
            // Viewing loaded replay from JSON
            handleReplayInput();
            updateReplay();
            render();
            break;
    }
}

int main(int argc, char* argv[]) {
    printf("🔥🔥🔥 WOLFENSTEIN WITH REPLAY - BUILD 2025-11-15 00:43 🔥🔥🔥\n");
    
    srand(time(NULL));
    
    if(SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) < 0) {
        printf("SDL Init Error: %s\n", SDL_GetError());
        return 1;
    }
    
    // Inicializar SDL_mixer
    if(Mix_OpenAudio(22050, AUDIO_S16SYS, 1, 512) < 0) {
        printf("SDL_mixer Init Error: %s\n", Mix_GetError());
    }
    
    Mix_AllocateChannels(8);
    
    window = SDL_CreateWindow(
        "Wolfenstein 3D - WebAssembly (ENHANCED!)",
        SDL_WINDOWPOS_UNDEFINED,
        SDL_WINDOWPOS_UNDEFINED,
        SCREEN_WIDTH,
        SCREEN_HEIGHT,
        SDL_WINDOW_SHOWN
    );
    
    if(!window) {
        printf("Window Error: %s\n", SDL_GetError());
        return 1;
    }
    
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if(!renderer) {
        printf("Renderer Error: %s\n", SDL_GetError());
        return 1;
    }
    
    initEnemies();
    initItems();
    initLighting();
    generateSounds();
    
    printf("===========================================\n");
    printf(">>> REPLAY SYSTEM INITIALIZED\n");
    printf(">>> Recording: %s, Max frames: %d\n", isRecording ? "ACTIVE" : "INACTIVE", MAX_RECORDED_FRAMES);
    printf(">>> Press 'U' to start replay mode\n");
    printf("===========================================\n");
    printf("  Wolfenstein 3D - MEGA Enhanced Edition!\n");
    printf("===========================================\n");
    printf("Controls:\n");
    printf("  WASD / Arrow Keys - Move and turn\n");
    printf("  SPACE / Left Click - Shoot\n");
    printf("  1-4 - Switch Weapons\n");
    printf("    1: Pistol\n");
    printf("    2: Machine Gun\n");
    printf("    3: Laser\n");
    printf("    4: Shotgun\n");
    printf("  Left Shift - Sprint\n");
    printf("\n");
    printf("Map Size: %dx%d\n", MAP_WIDTH, MAP_HEIGHT);
    printf("Enemies: %zu\n", enemies.size());
    printf("Items: %zu\n", items.size());
    printf("Health: %d\n", playerHealth);
    printf("Ammo: %d\n", ammo);
    printf("\n");
    printf("¡A cazar nazis en el mapa gigante!\n");
    printf("===========================================\n");
    
    emscripten_set_main_loop(mainLoop, 0, 1);
    
    // Limpiar
    if(shootSound) Mix_FreeChunk(shootSound);
    if(enemyDeathSound) Mix_FreeChunk(enemyDeathSound);
    if(playerHitSound) Mix_FreeChunk(playerHitSound);
    if(pickupAmmoSound) Mix_FreeChunk(pickupAmmoSound);
    if(pickupHealthSound) Mix_FreeChunk(pickupHealthSound);
    Mix_CloseAudio();
    
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    
    return 0;
}
