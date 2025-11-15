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
    bool hasSeenPlayer; // Track if enemy has detected player (for alert sound)
    
    Enemy(double px, double py, EnemyType t = SOLDIER) : x(px), y(py), health(100), alive(true), 
          moveTimer(0), animFrame(0), type(t), shootTimer(0), shootCooldown(4.0), fallOffset(0), hasSeenPlayer(false) {}
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
enum WeaponType { PISTOL = 0, MACHINEGUN = 1, LASER = 2, SHOTGUN = 3, BAZOOKA = 4 };
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
    float rotation;  // Rotación para animación espiral
    int spawnTime;   // Tiempo de spawn para efectos
    
    LaserBeam() : x(0), y(0), dirX(0), dirY(0), lifetime(30), active(false), rotation(0), spawnTime(0) {}
};
std::vector<LaserBeam> laserBeams;

// Physical bullet system
struct Bullet {
    double x, y;
    double dirX, dirY;
    int lifetime;
    bool active;
    bool isEnemy; // true si es bala enemiga
    bool isBazooka; // true si es proyectil de bazooka
    double speed; // Velocidad de la bala (para bazooka más lenta)
    std::vector<std::pair<double, double>> trail; // Estela de posiciones
    
    Bullet() : x(0), y(0), dirX(0), dirY(0), lifetime(0), active(false), isEnemy(false), 
               isBazooka(false), speed(0.3) {}
};

// Explosion system
struct Explosion {
    double x, y;
    int lifetime;
    float radius;
    bool active;
    int frame; // Para animación
    
    Explosion() : x(0), y(0), lifetime(30), radius(2.0f), active(false), frame(0) {}
};

std::vector<Bullet> bullets;
std::vector<Bullet> enemyBullets;
std::vector<Explosion> explosions;

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
    bool hasSeenPlayer;
    
    EnemySnapshot() : x(0), y(0), health(0), alive(false), type(0), fallOffset(0), hasSeenPlayer(false) {}
    EnemySnapshot(const Enemy& e) : x(e.x), y(e.y), health(e.health), alive(e.alive), 
                                     type(e.type), fallOffset(e.fallOffset), hasSeenPlayer(e.hasSeenPlayer) {}
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
    STATE_REPLAY_VIEWER,
    STATE_MAP_EDITOR
};

GameState currentGameState = STATE_MAIN_MENU;
bool jsonFileLoaded = false;
bool jsonFileValid = false;
std::string loadedJsonFilename = "";
bool showFileDropZone = false;

// ============================================================================
// MAP EDITOR SYSTEM
// ============================================================================

enum EditorTool {
    TOOL_PAINT,
    TOOL_ERASE
};

enum EditorElement {
    ELEM_WALL_1,
    ELEM_WALL_2,
    ELEM_WALL_3,
    ELEM_WALL_4,
    ELEM_LIGHT,
    ELEM_ENEMY_DOG,
    ELEM_ENEMY_SOLDIER,
    ELEM_ITEM_AMMO,
    ELEM_ITEM_HEALTH,
    ELEM_SPAWN
};

// Editor state variables
int editorMapWidth = 24;
int editorMapHeight = 24;
int editorWorldMap[40][40];  // Maximum 40x40
float cameraRotation = 45.0f; // 0, 90, 180, 270 degrees
bool gridEnabled = true;
EditorTool currentTool = TOOL_PAINT;
EditorElement selectedElement = ELEM_WALL_1;
double spawnX = 2.0;
double spawnY = 2.0;

// Editor UI state
int hoveredTileX = -1;
int hoveredTileY = -1;

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

// Editor entities (temporary storage) - declared here after structs are defined
std::vector<LightSource> editorLights;
std::vector<Enemy> editorEnemies;
std::vector<Item> editorItems;

// Sound system
Mix_Chunk* shootSound = nullptr;
Mix_Chunk* enemyDeathSound = nullptr;
Mix_Chunk* playerHitSound = nullptr;
Mix_Chunk* pickupAmmoSound = nullptr;
Mix_Chunk* pickupHealthSound = nullptr;
Mix_Chunk* enemyAlertSound = nullptr;

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
    
    // Sonido de alerta de enemigo (beep rápido ascendente)
    const int alertSamples = shootFreq / 12; // ~83ms
    Sint16* alertBuffer = new Sint16[alertSamples];
    for(int i = 0; i < alertSamples; i++) {
        double t = (double)i / shootFreq;
        double freq = 600 + (t * 800); // Frecuencia ascendente de 600 a 1400 Hz
        alertBuffer[i] = (Sint16)(sin(2 * M_PI * freq * t) * 9000 * exp(-t * 12));
    }
    enemyAlertSound = Mix_QuickLoad_RAW((Uint8*)alertBuffer, alertSamples * 2);
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
    // Map editor - load map from JSON
    void EMSCRIPTEN_KEEPALIVE loadMapFromJSON(const char* jsonData);
    
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
    
    // Player flashlight - projected forward from player
    if(flashlightEnabled) {
        // Project flashlight origin 1.5 units in front of player
        double flashOriginX = posX + dirX * 1.5;
        double flashOriginY = posY + dirY * 1.5;
        
        double dx = x - flashOriginX;
        double dy = y - flashOriginY;
        double dist = sqrt(dx*dx + dy*dy);
        
        // Check if point is in front of player (cone check with tighter angle)
        double dotProduct = (dx * dirX + dy * dirY) / (dist + 0.001);
        if(dotProduct > 0.7 && dist < flashlightRadius) {
            float attenuation = 1.0f - (dist / flashlightRadius);
            float coneEffect = (dotProduct - 0.7f) * 3.33f; // 0.0 to 1.0 (adjusted for new threshold)
            totalLight += flashlightIntensity * attenuation * coneEffect;
        }
    }
    
    // Bullet lighting - balas iluminan su trayectoria
    for(const auto& bullet : bullets) {
        if(!bullet.active) continue;
        
        double dx = x - bullet.x;
        double dy = y - bullet.y;
        double dist = sqrt(dx*dx + dy*dy);
        
        // Bazooka tiene mayor radio e intensidad
        const float bulletRadius = bullet.isBazooka ? 5.0f : 4.0f;
        const float bulletIntensity = bullet.isBazooka ? 0.9f : 0.7f;
        
        if(dist < bulletRadius) {
            float attenuation = 1.0f - (dist / bulletRadius);
            attenuation = attenuation * attenuation; // Quadratic falloff
            
            // Balas azules o violetas (bazooka) iluminan el entorno
            totalLight += bulletIntensity * attenuation;
        }
    }
    
    // Explosion lighting - explosiones iluminan intensamente
    for(const auto& explosion : explosions) {
        if(!explosion.active) continue;
        
        double dx = x - explosion.x;
        double dy = y - explosion.y;
        double dist = sqrt(dx*dx + dy*dy);
        
        const float explosionRadius = 6.0f;  // Radio grande de iluminación
        float explosionIntensity = 1.5f * (1.0f - (float)explosion.frame / 30.0f); // Decrece con el tiempo
        
        if(dist < explosionRadius) {
            float attenuation = 1.0f - (dist / explosionRadius);
            attenuation = attenuation * attenuation;
            totalLight += explosionIntensity * attenuation;
        }
    }
    
    // Enemy bullet lighting (rojo para balas enemigas)
    for(const auto& bullet : enemyBullets) {
        if(!bullet.active) continue;
        
        double dx = x - bullet.x;
        double dy = y - bullet.y;
        double dist = sqrt(dx*dx + dy*dy);
        
        const float bulletRadius = 2.0f;
        const float bulletIntensity = 0.3f;
        
        if(dist < bulletRadius) {
            float attenuation = 1.0f - (dist / bulletRadius);
            attenuation = attenuation * attenuation;
            totalLight += bulletIntensity * attenuation;
        }
    }
    
    // Laser beam lighting - el láser ilumina su recorrido con LUZ ROJA
    if(laserActive && currentWeapon == LASER) {
        // Crear iluminación ROJA a lo largo del rayo láser
        double laserEndX = posX + dirX * 20.0;  // Proyectar 20 unidades adelante
        double laserEndY = posY + dirY * 20.0;
        
        // Calcular distancia del punto al segmento del láser
        double laserDx = laserEndX - posX;
        double laserDy = laserEndY - posY;
        double laserLength = sqrt(laserDx*laserDx + laserDy*laserDy);
        
        // Normalizar dirección del láser
        double laserNormX = laserDx / laserLength;
        double laserNormY = laserDy / laserLength;
        
        // Proyección del punto en el láser
        double dx = x - posX;
        double dy = y - posY;
        double projection = dx * laserNormX + dy * laserNormY;
        
        if(projection >= 0 && projection <= laserLength) {
            // Punto más cercano en el láser
            double closestX = posX + laserNormX * projection;
            double closestY = posY + laserNormY * projection;
            
            double distToLaser = sqrt((x - closestX)*(x - closestX) + (y - closestY)*(y - closestY));
            
            const float laserLightRadius = 4.0f;  // Radio aumentado para más iluminación roja
            const float laserLightIntensity = 0.8f;  // Intensidad aumentada
            
            if(distToLaser < laserLightRadius) {
                float attenuation = 1.0f - (distToLaser / laserLightRadius);
                attenuation = attenuation * attenuation;
                totalLight += laserLightIntensity * attenuation;
            }
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
    // Draw weapon at bottom center of screen - ESTILO HALF-LIFE 3D
    int weaponWidth = 160;  // Más ancho para mejor presencia
    int weaponHeight = 220;
    int weaponX = SCREEN_WIDTH / 2 - weaponWidth / 2;
    int weaponY = SCREEN_HEIGHT - weaponHeight;
    
    // Weapon recoil when shooting (más dramático)
    if(weaponState == 1) {
        weaponY -= 30;
        weaponX += (weaponFrame % 2 == 0 ? -2 : 2);  // Shake horizontal
    }
    
    // Draw weapon based on type - RENDERIZADO 3D MEJORADO
    if(currentWeapon == PISTOL) {
        // === PISTOLA 3D - Estilo Half-Life ===
        
        // Barrel (cañón) con perspectiva y sombreado
        // Sombra del cañón (lado izquierdo)
        SDL_SetRenderDrawColor(renderer, 35, 35, 35, 255);
        SDL_Rect barrelShadow = {weaponX + 55, weaponY + 20, 8, 70};
        SDL_RenderFillRect(renderer, &barrelShadow);
        
        // Cañón principal
        SDL_SetRenderDrawColor(renderer, 60, 60, 60, 255);
        SDL_Rect barrel = {weaponX + 63, weaponY + 20, 20, 70};
        SDL_RenderFillRect(renderer, &barrel);
        
        // Highlight del cañón (lado derecho)
        SDL_SetRenderDrawColor(renderer, 90, 90, 90, 255);
        SDL_Rect barrelHighlight = {weaponX + 78, weaponY + 22, 5, 66};
        SDL_RenderFillRect(renderer, &barrelHighlight);
        
        // Boca del cañón (en perspectiva)
        SDL_SetRenderDrawColor(renderer, 20, 20, 20, 255);
        SDL_Rect bore = {weaponX + 66, weaponY + 15, 14, 8};
        SDL_RenderFillRect(renderer, &bore);
        SDL_SetRenderDrawColor(renderer, 40, 40, 40, 255);
        SDL_Rect boreInner = {weaponX + 68, weaponY + 17, 10, 4};
        SDL_RenderFillRect(renderer, &boreInner);
        
        // Slide (corredera) con detalles
        SDL_SetRenderDrawColor(renderer, 70, 70, 70, 255);
        SDL_Rect slide = {weaponX + 58, weaponY + 35, 30, 35};
        SDL_RenderFillRect(renderer, &slide);
        
        // Detalles de la corredera
        SDL_SetRenderDrawColor(renderer, 50, 50, 50, 255);
        for(int i = 0; i < 5; i++) {
            SDL_Rect groove = {weaponX + 60, weaponY + 40 + i*6, 2, 4};
            SDL_RenderFillRect(renderer, &groove);
        }
        
        // Grip (empuñadura) con textura
        SDL_SetRenderDrawColor(renderer, 45, 45, 45, 255);
        SDL_Rect grip = {weaponX + 60, weaponY + 75, 28, 100};
        SDL_RenderFillRect(renderer, &grip);
        
        // Textura de grip (puntos de agarre)
        SDL_SetRenderDrawColor(renderer, 30, 30, 30, 255);
        for(int y = 0; y < 10; y++) {
            for(int x = 0; x < 3; x++) {
                SDL_Rect dot = {weaponX + 64 + x*8, weaponY + 80 + y*9, 3, 3};
                SDL_RenderFillRect(renderer, &dot);
            }
        }
        
        // Highlight del grip
        SDL_SetRenderDrawColor(renderer, 65, 65, 65, 255);
        SDL_Rect gripHighlight = {weaponX + 82, weaponY + 77, 4, 96};
        SDL_RenderFillRect(renderer, &gripHighlight);
        
        // Detalles metálicos plateados
        SDL_SetRenderDrawColor(renderer, 180, 180, 180, 255);
        SDL_Rect detail1 = {weaponX + 65, weaponY + 45, 18, 3};
        SDL_RenderFillRect(renderer, &detail1);
        SDL_Rect detail2 = {weaponX + 67, weaponY + 55, 14, 2};
        SDL_RenderFillRect(renderer, &detail2);
        
        // Tornillos
        SDL_SetRenderDrawColor(renderer, 100, 100, 100, 255);
        SDL_Rect screw1 = {weaponX + 65, weaponY + 90, 3, 3};
        SDL_RenderFillRect(renderer, &screw1);
        SDL_Rect screw2 = {weaponX + 80, weaponY + 90, 3, 3};
        SDL_RenderFillRect(renderer, &screw2);
    } 
    else if(currentWeapon == MACHINEGUN) {
        // === AMETRALLADORA 3D ===
        
        // Cuerpo principal con volumen
        SDL_SetRenderDrawColor(renderer, 35, 35, 35, 255);
        SDL_Rect bodyShadow = {weaponX + 50, weaponY + 60, 50, 80};
        SDL_RenderFillRect(renderer, &bodyShadow);
        
        SDL_SetRenderDrawColor(renderer, 50, 50, 50, 255);
        SDL_Rect body = {weaponX + 55, weaponY + 60, 50, 80};
        SDL_RenderFillRect(renderer, &body);
        
        SDL_SetRenderDrawColor(renderer, 70, 70, 70, 255);
        SDL_Rect bodyHighlight = {weaponX + 95, weaponY + 62, 8, 76};
        SDL_RenderFillRect(renderer, &bodyHighlight);
        
        // Doble cañón con perspectiva
        // Cañón izquierdo
        SDL_SetRenderDrawColor(renderer, 30, 30, 30, 255);
        SDL_Rect barrel1 = {weaponX + 60, weaponY + 10, 15, 90};
        SDL_RenderFillRect(renderer, &barrel1);
        SDL_SetRenderDrawColor(renderer, 50, 50, 50, 255);
        SDL_Rect barrel1Highlight = {weaponX + 72, weaponY + 12, 3, 86};
        SDL_RenderFillRect(renderer, &barrel1Highlight);
        
        // Cañón derecho
        SDL_SetRenderDrawColor(renderer, 30, 30, 30, 255);
        SDL_Rect barrel2 = {weaponX + 80, weaponY + 10, 15, 90};
        SDL_RenderFillRect(renderer, &barrel2);
        SDL_SetRenderDrawColor(renderer, 50, 50, 50, 255);
        SDL_Rect barrel2Highlight = {weaponX + 92, weaponY + 12, 3, 86};
        SDL_RenderFillRect(renderer, &barrel2Highlight);
        
        // Bocas de los cañones
        SDL_SetRenderDrawColor(renderer, 15, 15, 15, 255);
        SDL_Rect bore1 = {weaponX + 62, weaponY + 5, 11, 8};
        SDL_RenderFillRect(renderer, &bore1);
        SDL_Rect bore2 = {weaponX + 82, weaponY + 5, 11, 8};
        SDL_RenderFillRect(renderer, &bore2);
        
        // Cargador grande con detalles
        SDL_SetRenderDrawColor(renderer, 25, 25, 25, 255);
        SDL_Rect mag = {weaponX + 65, weaponY + 110, 25, 60};
        SDL_RenderFillRect(renderer, &mag);
        
        // Detalle del cargador (balas visibles)
        SDL_SetRenderDrawColor(renderer, 200, 180, 50, 255);
        for(int i = 0; i < 6; i++) {
            SDL_Rect bullet = {weaponX + 68, weaponY + 115 + i*9, 4, 6};
            SDL_RenderFillRect(renderer, &bullet);
            SDL_Rect bullet2 = {weaponX + 82, weaponY + 115 + i*9, 4, 6};
            SDL_RenderFillRect(renderer, &bullet2);
        }
        
        // Grip con textura antideslizante
        SDL_SetRenderDrawColor(renderer, 40, 40, 40, 255);
        SDL_Rect grip = {weaponX + 70, weaponY + 140, 20, 50};
        SDL_RenderFillRect(renderer, &grip);
        
        // Textura del grip
        for(int i = 0; i < 12; i++) {
            SDL_SetRenderDrawColor(renderer, 25, 25, 25, 255);
            SDL_Rect line = {weaponX + 71, weaponY + 142 + i*4, 18, 2};
            SDL_RenderFillRect(renderer, &line);
        }
    }
    else if(currentWeapon == LASER) {
        // === ARMA LÁSER 3D FUTURISTA ===
        
        // Cuerpo principal futurista con gradiente
        SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
        
        // Sombra base
        SDL_SetRenderDrawColor(renderer, 20, 30, 60, 255);
        SDL_Rect bodyShadow = {weaponX + 50, weaponY + 50, 60, 100};
        SDL_RenderFillRect(renderer, &bodyShadow);
        
        // Cuerpo principal (azul oscuro metálico)
        SDL_SetRenderDrawColor(renderer, 40, 60, 110, 255);
        SDL_Rect body = {weaponX + 55, weaponY + 50, 50, 100};
        SDL_RenderFillRect(renderer, &body);
        
        // Highlights metálicos azules
        SDL_SetRenderDrawColor(renderer, 70, 100, 150, 255);
        SDL_Rect highlight1 = {weaponX + 95, weaponY + 52, 8, 96};
        SDL_RenderFillRect(renderer, &highlight1);
        SDL_Rect highlight2 = {weaponX + 57, weaponY + 52, 3, 96};
        SDL_RenderFillRect(renderer, &highlight2);
        
        // Cámara de energía (cristal brillante)
        SDL_SetRenderDrawColor(renderer, 0, 150, 255, 180);
        SDL_Rect energyChamber = {weaponX + 63, weaponY + 70, 30, 50};
        SDL_RenderFillRect(renderer, &energyChamber);
        
        // Energía interna pulsante (efecto de carga)
        int pulseAlpha = 100 + (weaponFrame * 30) % 155;
        SDL_SetRenderDrawColor(renderer, 50, 200, 255, pulseAlpha);
        SDL_Rect energyCore = {weaponX + 68, weaponY + 80, 20, 30};
        SDL_RenderFillRect(renderer, &energyCore);
        
        SDL_SetRenderDrawColor(renderer, 150, 230, 255, pulseAlpha + 50);
        SDL_Rect energyCenter = {weaponX + 73, weaponY + 90, 10, 10};
        SDL_RenderFillRect(renderer, &energyCenter);
        
        // Barril del láser (cañón de enfoque)
        SDL_SetRenderDrawColor(renderer, 35, 50, 90, 255);
        SDL_Rect barrel = {weaponX + 65, weaponY + 10, 26, 60};
        SDL_RenderFillRect(renderer, &barrel);
        
        // Anillos del enfocador
        SDL_SetRenderDrawColor(renderer, 100, 150, 200, 255);
        for(int i = 0; i < 4; i++) {
            SDL_Rect ring = {weaponX + 66, weaponY + 20 + i*12, 24, 4};
            SDL_RenderFillRect(renderer, &ring);
        }
        
        // LEDs de estado (parpadeantes)
        for(int i = 0; i < 4; i++) {
            int ledAlpha = ((weaponFrame + i*2) % 20 < 10) ? 255 : 100;
            SDL_SetRenderDrawColor(renderer, 0, 255, 200, ledAlpha);
            SDL_Rect led1 = {weaponX + 60, weaponY + 75 + i*10, 4, 4};
            SDL_RenderFillRect(renderer, &led1);
            SDL_Rect led2 = {weaponX + 92, weaponY + 75 + i*10, 4, 4};
            SDL_RenderFillRect(renderer, &led2);
        }
        
        // Emisor láser (cristal rojo brillante)
        if(laserActive) {
            // Resplandor rojo cuando está activo
            SDL_SetRenderDrawColor(renderer, 255, 0, 0, 150);
            SDL_Rect glowOuter = {weaponX + 68, weaponY - 5, 20, 20};
            SDL_RenderFillRect(renderer, &glowOuter);
        }
        
        SDL_SetRenderDrawColor(renderer, 255, 50, 50, 255);
        SDL_Rect emitter = {weaponX + 73, weaponY, 10, 15};
        SDL_RenderFillRect(renderer, &emitter);
        
        SDL_SetRenderDrawColor(renderer, 255, 150, 150, 255);
        SDL_Rect emitterCore = {weaponX + 76, weaponY + 3, 4, 9};
        SDL_RenderFillRect(renderer, &emitterCore);
        
        // Grip ergonómico
        SDL_SetRenderDrawColor(renderer, 50, 70, 100, 255);
        SDL_Rect grip = {weaponX + 65, weaponY + 130, 26, 70};
        SDL_RenderFillRect(renderer, &grip);
        
        // Detalles del grip
        SDL_SetRenderDrawColor(renderer, 30, 50, 80, 255);
        for(int i = 0; i < 8; i++) {
            SDL_Rect detail = {weaponX + 67, weaponY + 135 + i*8, 22, 3};
            SDL_RenderFillRect(renderer, &detail);
        }
        
        SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_NONE);
    }
    else if(currentWeapon == SHOTGUN) {
        // === ESCOPETA 3D CON MADERA ===
        
        // Cañones dobles con perspectiva
        // Cañón izquierdo
        SDL_SetRenderDrawColor(renderer, 25, 25, 25, 255);
        SDL_Rect barrel1 = {weaponX + 55, weaponY + 10, 18, 80};
        SDL_RenderFillRect(renderer, &barrel1);
        SDL_SetRenderDrawColor(renderer, 45, 45, 45, 255);
        SDL_Rect barrel1Highlight = {weaponX + 70, weaponY + 12, 3, 76};
        SDL_RenderFillRect(renderer, &barrel1Highlight);
        
        // Cañón derecho
        SDL_SetRenderDrawColor(renderer, 25, 25, 25, 255);
        SDL_Rect barrel2 = {weaponX + 77, weaponY + 10, 18, 80};
        SDL_RenderFillRect(renderer, &barrel2);
        SDL_SetRenderDrawColor(renderer, 45, 45, 45, 255);
        SDL_Rect barrel2Highlight = {weaponX + 92, weaponY + 12, 3, 76};
        SDL_RenderFillRect(renderer, &barrel2Highlight);
        
        // Bocas de los cañones
        SDL_SetRenderDrawColor(renderer, 15, 15, 15, 255);
        SDL_Rect bore1 = {weaponX + 58, weaponY + 5, 12, 9};
        SDL_RenderFillRect(renderer, &bore1);
        SDL_Rect bore2 = {weaponX + 80, weaponY + 5, 12, 9};
        SDL_RenderFillRect(renderer, &bore2);
        
        // Mecanismo de disparo (metal)
        SDL_SetRenderDrawColor(renderer, 60, 60, 60, 255);
        SDL_Rect action = {weaponX + 60, weaponY + 70, 35, 25};
        SDL_RenderFillRect(renderer, &action);
        
        SDL_SetRenderDrawColor(renderer, 90, 90, 90, 255);
        SDL_Rect actionHighlight = {weaponX + 88, weaponY + 72, 5, 21};
        SDL_RenderFillRect(renderer, &actionHighlight);
        
        // Culata de madera con vetas
        SDL_SetRenderDrawColor(renderer, 70, 45, 25, 255);
        SDL_Rect stock = {weaponX + 60, weaponY + 100, 35, 90};
        SDL_RenderFillRect(renderer, &stock);
        
        // Vetas de madera (textura)
        SDL_SetRenderDrawColor(renderer, 50, 30, 15, 255);
        for(int i = 0; i < 15; i++) {
            int offset = (i % 3) * 2;
            SDL_Rect grain = {weaponX + 62 + offset, weaponY + 105 + i*6, 28 - offset, 2};
            SDL_RenderFillRect(renderer, &grain);
        }
        
        // Highlight de la madera (barniz)
        SDL_SetRenderDrawColor(renderer, 100, 70, 40, 255);
        SDL_Rect woodHighlight = {weaponX + 87, weaponY + 102, 6, 86};
        SDL_RenderFillRect(renderer, &woodHighlight);
        
        // Gatillo
        SDL_SetRenderDrawColor(renderer, 180, 150, 50, 255);
        SDL_Rect trigger = {weaponX + 72, weaponY + 110, 8, 15};
        SDL_RenderFillRect(renderer, &trigger);
    }
    else if(currentWeapon == BAZOOKA) {
        // === BAZOOKA 3D - LANZACOHETES ESTILO HALF-LIFE ===
        
        SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
        
        // Tubo lanzador principal (grande y grueso)
        SDL_SetRenderDrawColor(renderer, 50, 50, 50, 255);
        SDL_Rect tube = {weaponX + 45, weaponY + 20, 65, 60};
        SDL_RenderFillRect(renderer, &tube);
        
        // Sombra del tubo
        SDL_SetRenderDrawColor(renderer, 30, 30, 30, 255);
        SDL_Rect tubeShadow = {weaponX + 45, weaponY + 22, 10, 56};
        SDL_RenderFillRect(renderer, &tubeShadow);
        
        // Highlight del tubo
        SDL_SetRenderDrawColor(renderer, 80, 80, 80, 255);
        SDL_Rect tubeHighlight = {weaponX + 100, weaponY + 22, 8, 56};
        SDL_RenderFillRect(renderer, &tubeHighlight);
        
        // Boca del lanzacohetes (círculo grande)
        SDL_SetRenderDrawColor(renderer, 15, 15, 15, 255);
        SDL_Rect bore = {weaponX + 52, weaponY + 10, 50, 15};
        SDL_RenderFillRect(renderer, &bore);
        
        // Interior de la boca (más oscuro)
        SDL_SetRenderDrawColor(renderer, 5, 5, 5, 255);
        SDL_Rect boreInner = {weaponX + 56, weaponY + 13, 42, 9};
        SDL_RenderFillRect(renderer, &boreInner);
        
        // Anillos metálicos del cañón (textura)
        SDL_SetRenderDrawColor(renderer, 70, 70, 70, 255);
        for(int i = 0; i < 3; i++) {
            SDL_Rect ring = {weaponX + 47, weaponY + 30 + i*18, 61, 5};
            SDL_RenderFillRect(renderer, &ring);
        }
        
        // Mecanismo de disparo lateral
        SDL_SetRenderDrawColor(renderer, 60, 60, 60, 255);
        SDL_Rect trigger = {weaponX + 70, weaponY + 90, 20, 35};
        SDL_RenderFillRect(renderer, &trigger);
        
        // Gatillo
        SDL_SetRenderDrawColor(renderer, 200, 50, 50, 255);
        SDL_Rect triggerButton = {weaponX + 75, weaponY + 100, 10, 15};
        SDL_RenderFillRect(renderer, &triggerButton);
        
        // Visor óptico en la parte superior
        SDL_SetRenderDrawColor(renderer, 40, 40, 40, 255);
        SDL_Rect scope = {weaponX + 65, weaponY + 18, 25, 10};
        SDL_RenderFillRect(renderer, &scope);
        
        // Lente del visor
        SDL_SetRenderDrawColor(renderer, 0, 150, 200, 180);
        SDL_Rect scopeLens = {weaponX + 70, weaponY + 20, 5, 6};
        SDL_RenderFillRect(renderer, &scopeLens);
        SDL_Rect scopeLens2 = {weaponX + 82, weaponY + 20, 5, 6};
        SDL_RenderFillRect(renderer, &scopeLens2);
        
        // Empuñadura ergonómica
        SDL_SetRenderDrawColor(renderer, 45, 45, 45, 255);
        SDL_Rect grip = {weaponX + 65, weaponY + 130, 30, 60};
        SDL_RenderFillRect(renderer, &grip);
        
        // Textura del grip
        SDL_SetRenderDrawColor(renderer, 30, 30, 30, 255);
        for(int i = 0; i < 8; i++) {
            SDL_Rect gripLine = {weaponX + 67, weaponY + 135 + i*7, 26, 3};
            SDL_RenderFillRect(renderer, &gripLine);
        }
        
        // Detalles metálicos (tornillos y placas)
        SDL_SetRenderDrawColor(renderer, 100, 100, 100, 255);
        SDL_Rect screw1 = {weaponX + 52, weaponY + 35, 4, 4};
        SDL_RenderFillRect(renderer, &screw1);
        SDL_Rect screw2 = {weaponX + 100, weaponY + 35, 4, 4};
        SDL_RenderFillRect(renderer, &screw2);
        SDL_Rect screw3 = {weaponX + 52, weaponY + 70, 4, 4};
        SDL_RenderFillRect(renderer, &screw3);
        SDL_Rect screw4 = {weaponX + 100, weaponY + 70, 4, 4};
        SDL_RenderFillRect(renderer, &screw4);
        
        // Placa de advertencia (naranja)
        SDL_SetRenderDrawColor(renderer, 255, 150, 0, 255);
        SDL_Rect warningPlate = {weaponX + 70, weaponY + 50, 20, 8};
        SDL_RenderFillRect(renderer, &warningPlate);
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_Rect warningLine = {weaponX + 72, weaponY + 52, 16, 4};
        SDL_RenderFillRect(renderer, &warningLine);
        
        // Indicador LED (violeta cuando activo)
        if(weaponFrame % 6 < 3) {
            SDL_SetRenderDrawColor(renderer, 200, 50, 255, 255);
            SDL_Rect led = {weaponX + 96, weaponY + 55, 5, 5};
            SDL_RenderFillRect(renderer, &led);
        }
        
        // Soporte lateral del tubo
        SDL_SetRenderDrawColor(renderer, 55, 55, 55, 255);
        SDL_Rect support1 = {weaponX + 55, weaponY + 80, 8, 30};
        SDL_RenderFillRect(renderer, &support1);
        SDL_Rect support2 = {weaponX + 92, weaponY + 80, 8, 30};
        SDL_RenderFillRect(renderer, &support2);
        
        SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_NONE);
    }
    
    // Muzzle flash MEJORADO when shooting (not in replay mode)
    if(weaponState == 1 && weaponFrame < 3 && !isReplaying) {
        SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
        
        int flashX = weaponX + 60;
        int flashY = weaponY - 20;
        
        // Color según el arma
        if(currentWeapon == LASER) {
            // Flash rojo para láser
            SDL_SetRenderDrawColor(renderer, 255, 0, 0, 100);
            SDL_Rect flash3 = {flashX - 15, flashY - 15, 70, 50};
            SDL_RenderFillRect(renderer, &flash3);
            
            SDL_SetRenderDrawColor(renderer, 255, 50, 50, 200);
            SDL_Rect flash2 = {flashX - 5, flashY - 5, 50, 30};
            SDL_RenderFillRect(renderer, &flash2);
            
            SDL_SetRenderDrawColor(renderer, 255, 150, 150, 255);
            SDL_Rect flash1 = {flashX + 5, flashY + 5, 30, 15};
            SDL_RenderFillRect(renderer, &flash1);
        } else if(currentWeapon == BAZOOKA) {
            // Flash violeta para bazooka (más grande)
            SDL_SetRenderDrawColor(renderer, 150, 0, 200, 100);
            SDL_Rect flash4 = {flashX - 25, flashY - 25, 90, 70};
            SDL_RenderFillRect(renderer, &flash4);
            
            SDL_SetRenderDrawColor(renderer, 180, 50, 230, 150);
            SDL_Rect flash3 = {flashX - 18, flashY - 18, 76, 56};
            SDL_RenderFillRect(renderer, &flash3);
            
            SDL_SetRenderDrawColor(renderer, 200, 100, 255, 200);
            SDL_Rect flash2 = {flashX - 10, flashY - 10, 60, 40};
            SDL_RenderFillRect(renderer, &flash2);
            
            SDL_SetRenderDrawColor(renderer, 230, 180, 255, 255);
            SDL_Rect flash1 = {flashX, flashY, 40, 25};
            SDL_RenderFillRect(renderer, &flash1);
        } else {
            // Flash amarillo-azul para balas normales
            SDL_SetRenderDrawColor(renderer, 100, 150, 255, 80);
            SDL_Rect flash3 = {flashX - 15, flashY - 15, 70, 50};
            SDL_RenderFillRect(renderer, &flash3);
            
            SDL_SetRenderDrawColor(renderer, 150, 200, 255, 180);
            SDL_Rect flash2 = {flashX - 5, flashY - 5, 50, 30};
            SDL_RenderFillRect(renderer, &flash2);
            
            SDL_SetRenderDrawColor(renderer, 200, 230, 255, 255);
            SDL_Rect flash1 = {flashX + 5, flashY + 5, 30, 15};
            SDL_RenderFillRect(renderer, &flash1);
        }
        
        SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_NONE);
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
    
    // Ametralladora, shotgun, láser y bazooka pueden disparar mientras mantienes presionado
    if(currentWeapon != MACHINEGUN && currentWeapon != SHOTGUN && currentWeapon != LASER && currentWeapon != BAZOOKA && weaponState != 0) return;
    
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
    else if(currentWeapon == BAZOOKA) {
        // BAZOOKA - dispara proyectil explosivo lento
        if(weaponTimer == 0) {
            ammo -= 10; // Consume mucha munición
            bulletsFired++;
            if(shootSound) Mix_PlayChannel(-1, shootSound, 0);
            
            Bullet rocket;
            rocket.x = posX;
            rocket.y = posY;
            rocket.dirX = dirX;
            rocket.dirY = dirY;
            rocket.lifetime = 120; // Dura más en el aire
            rocket.active = true;
            rocket.isBazooka = true;
            rocket.speed = 0.15; // MÁS LENTA que balas normales (0.3)
            bullets.push_back(rocket);
            
            printf("Bazooka fired! Rocket speed: %.2f\n", rocket.speed);
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
        
        // Check if enemy can see player (first time detection for alert sound)
        if(!enemy.hasSeenPlayer && dist < 15.0) {
            // Check for line of sight (no walls between enemy and player)
            bool hasLineOfSight = true;
            int steps = (int)(dist * 2);
            for(int i = 1; i < steps; i++) {
                double checkX = enemy.x + (dx / dist) * (i * 0.5);
                double checkY = enemy.y + (dy / dist) * (i * 0.5);
                if(worldMap[(int)checkX][(int)checkY] != 0) {
                    hasLineOfSight = false;
                    break;
                }
            }
            
            if(hasLineOfSight) {
                enemy.hasSeenPlayer = true;
                if(enemyAlertSound) Mix_PlayChannel(-1, enemyAlertSound, 0);
                printf("Enemy detected player! Alert sound played.\n");
            }
        }
        
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
        int maxTrail = bullet.isBazooka ? 25 : 15; // Bazooka deja estela más larga
        if(bullet.trail.size() < (size_t)maxTrail) {
            bullet.trail.push_back(std::make_pair(bullet.x, bullet.y));
        } else {
            bullet.trail.erase(bullet.trail.begin()); // Remover la más antigua
            bullet.trail.push_back(std::make_pair(bullet.x, bullet.y));
        }
        
        // Move bullet usando la velocidad configurada
        double bulletSpeed = bullet.isBazooka ? bullet.speed : 0.5; // Bazooka es más lenta
        double newX = bullet.x + bullet.dirX * bulletSpeed;
        double newY = bullet.y + bullet.dirY * bulletSpeed;
        
        // Check wall collision
        if(worldMap[int(newX)][int(newY)] != 0) {
            // Si es bazooka, crear explosión
            if(bullet.isBazooka) {
                Explosion explosion;
                explosion.x = bullet.x;
                explosion.y = bullet.y;
                explosion.radius = 2.0f; // Radio de 2x2 cuadrados
                explosion.lifetime = 30;
                explosion.active = true;
                explosion.frame = 0;
                explosions.push_back(explosion);
                
                // Daño al jugador si está cerca
                double distToPlayer = sqrt((posX - bullet.x)*(posX - bullet.x) + 
                                          (posY - bullet.y)*(posY - bullet.y));
                if(distToPlayer < 2.5) { // Radio de daño al jugador
                    int damage = (int)(80.0 * (1.0 - distToPlayer / 2.5)); // Daño decrece con distancia
                    playerHealth -= damage;
                    if(playerHealth < 0) playerHealth = 0;
                    if(playerHitSound) Mix_PlayChannel(-1, playerHitSound, 0);
                    printf("Explosion damaged player! Damage: %d, Distance: %.2f\n", damage, distToPlayer);
                }
                
                printf("Rocket hit wall! Explosion created at (%.1f, %.1f)\n", bullet.x, bullet.y);
            }
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
            
            double collisionRadius = bullet.isBazooka ? 0.5 : 0.3; // Bazooka tiene colisión más grande
            
            if(dist < collisionRadius) {
                if(bullet.isBazooka) {
                    // Crear explosión al impactar enemigo
                    Explosion explosion;
                    explosion.x = newX;
                    explosion.y = newY;
                    explosion.radius = 2.0f;
                    explosion.lifetime = 30;
                    explosion.active = true;
                    explosion.frame = 0;
                    explosions.push_back(explosion);
                    
                    // Daño al jugador si está cerca del enemigo impactado
                    double distToPlayer = sqrt((posX - newX)*(posX - newX) + 
                                              (posY - newY)*(posY - newY));
                    if(distToPlayer < 2.5) {
                        int damage = (int)(80.0 * (1.0 - distToPlayer / 2.5));
                        playerHealth -= damage;
                        if(playerHealth < 0) playerHealth = 0;
                        if(playerHitSound) Mix_PlayChannel(-1, playerHitSound, 0);
                        printf("Explosion damaged player! Damage: %d, Distance: %.2f\n", damage, distToPlayer);
                    }
                    
                    printf("Rocket hit enemy! Explosion created\n");
                } else {
                    // Bala normal
                    enemy.health -= 50;
                    if(enemy.health <= 0) {
                        enemy.alive = false;
                        kills++;
                        if(enemyDeathSound) Mix_PlayChannel(-1, enemyDeathSound, 0);
                        printf("Enemy killed! Total kills: %d\n", kills);
                    }
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
    
    // Update explosions and apply area damage
    for(auto& explosion : explosions) {
        if(!explosion.active) continue;
        
        explosion.frame++;
        explosion.lifetime--;
        
        // Aplicar daño a enemigos en el área (solo en el primer frame)
        if(explosion.frame == 1) {
            for(auto& enemy : enemies) {
                if(!enemy.alive) continue;
                
                double dx = enemy.x - explosion.x;
                double dy = enemy.y - explosion.y;
                double dist = sqrt(dx*dx + dy*dy);
                
                if(dist < explosion.radius) {
                    // Daño decrece con la distancia
                    int damage = (int)(150.0 * (1.0 - dist / explosion.radius));
                    enemy.health -= damage;
                    if(enemy.health <= 0) {
                        enemy.alive = false;
                        kills++;
                        if(enemyDeathSound) Mix_PlayChannel(-1, enemyDeathSound, 0);
                        printf("Enemy killed by explosion! Total kills: %d\n", kills);
                    }
                    printf("Explosion damaged enemy at distance %.2f, damage: %d\n", dist, damage);
                }
            }
        }
        
        if(explosion.lifetime <= 0) {
            explosion.active = false;
        }
    }
    
    // Update laser beams
    for(auto& beam : laserBeams) {
        if(!beam.active) continue;
        
        // Agregar estela al láser (más lenta y duradera, MÁS LARGA)
        if(beam.trail.size() < 50) {  // Aumentado de 30 a 50 para estela más larga
            beam.trail.push_back(std::make_pair(beam.x, beam.y));
        }
        
        // Actualizar rotación para animación espiral (gira rápidamente)
        beam.rotation += 15.0f;  // Grados por frame (muy rápido)
        if(beam.rotation >= 360.0f) beam.rotation -= 360.0f;
        
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

// Forward declarations for editor functions
void exportMapToJSON();
void loadMapFromJSON(const char* jsonData);
void testMap();

void handleMapEditorInput() {
    SDL_Event event;
    while(SDL_PollEvent(&event)) {
        if(event.type == SDL_QUIT) {
            running = false;
            return;
        }
        
        // Arrow keys: Rotate camera
        if(event.type == SDL_KEYDOWN) {
            if(event.key.keysym.sym == SDLK_LEFT) {
                cameraRotation -= 90.0f;
                if(cameraRotation < 0) cameraRotation = 270.0f;
                printf("Camera rotation: %.0f\n", cameraRotation);
            }
            else if(event.key.keysym.sym == SDLK_RIGHT) {
                cameraRotation += 90.0f;
                if(cameraRotation >= 360.0f) cameraRotation = 0.0f;
                printf("Camera rotation: %.0f\n", cameraRotation);
            }
            else if(event.key.keysym.sym == SDLK_UP) {
                cameraRotation = 45.0f; // Reset to default
                printf("Camera rotation: %.0f\n", cameraRotation);
            }
            else if(event.key.keysym.sym == SDLK_ESCAPE) {
                currentGameState = STATE_MAIN_MENU;
                return;
            }
        }
        
        // Mouse click
        if(event.type == SDL_MOUSEBUTTONDOWN && event.button.button == SDL_BUTTON_LEFT) {
            int mouseX = event.button.x;
            int mouseY = event.button.y;
            
            // Check UI buttons first
            
            // LEFT PANEL: Tools
            int leftPanelX = 10;
            int leftPanelY = 80;
            int buttonWidth = 120;
            int buttonHeight = 40;
            int buttonSpacing = 50;
            
            // Paint/Erase button
            if(mouseX >= leftPanelX && mouseX <= leftPanelX + buttonWidth &&
               mouseY >= leftPanelY && mouseY <= leftPanelY + buttonHeight) {
                currentTool = (currentTool == TOOL_PAINT) ? TOOL_ERASE : TOOL_PAINT;
                printf("Tool: %s\n", currentTool == TOOL_PAINT ? "PAINT" : "ERASE");
                continue;
            }
            
            // Grid button
            leftPanelY += buttonSpacing;
            if(mouseX >= leftPanelX && mouseX <= leftPanelX + buttonWidth &&
               mouseY >= leftPanelY && mouseY <= leftPanelY + buttonHeight) {
                gridEnabled = !gridEnabled;
                printf("Grid: %s\n", gridEnabled ? "ON" : "OFF");
                continue;
            }
            
            // Size buttons
            leftPanelY += buttonSpacing;
            int sizeValues[] = {16, 24, 32};
            for(int i = 0; i < 3; i++) {
                if(mouseX >= leftPanelX && mouseX <= leftPanelX + buttonWidth &&
                   mouseY >= leftPanelY + i * 35 && mouseY <= leftPanelY + i * 35 + 30) {
                    // Change map size
                    editorMapWidth = sizeValues[i];
                    editorMapHeight = sizeValues[i];
                    
                    // Clear map
                    for(int y = 0; y < 40; y++) {
                        for(int x = 0; x < 40; x++) {
                            editorWorldMap[x][y] = 0;
                        }
                    }
                    editorLights.clear();
                    editorEnemies.clear();
                    editorItems.clear();
                    
                    printf("Map resized to %dx%d\n", editorMapWidth, editorMapHeight);
                    continue;
                }
            }
            
            // RIGHT PANEL: Elements
            int rightPanelX = SCREEN_WIDTH - 140;
            int rightPanelY = 80;
            int elemButtonSize = 50;
            
            // Wall buttons (2x2 grid)
            for(int i = 0; i < 4; i++) {
                int row = i / 2;
                int col = i % 2;
                SDL_Rect wallButton = {rightPanelX + col * 60, rightPanelY + row * 55, elemButtonSize, elemButtonSize};
                if(mouseX >= wallButton.x && mouseX <= wallButton.x + wallButton.w &&
                   mouseY >= wallButton.y && mouseY <= wallButton.y + wallButton.h) {
                    selectedElement = (EditorElement)(ELEM_WALL_1 + i);
                    printf("Selected: Wall %d\n", i + 1);
                    continue;
                }
            }
            
            // Light, Dog buttons
            rightPanelY += 110;
            SDL_Rect lightButton = {rightPanelX, rightPanelY, elemButtonSize, elemButtonSize};
            if(mouseX >= lightButton.x && mouseX <= lightButton.x + lightButton.w &&
               mouseY >= lightButton.y && mouseY <= lightButton.y + lightButton.h) {
                selectedElement = ELEM_LIGHT;
                printf("Selected: Light\n");
                continue;
            }
            
            SDL_Rect dogButton = {rightPanelX + 60, rightPanelY, elemButtonSize, elemButtonSize};
            if(mouseX >= dogButton.x && mouseX <= dogButton.x + dogButton.w &&
               mouseY >= dogButton.y && mouseY <= dogButton.y + dogButton.h) {
                selectedElement = ELEM_ENEMY_DOG;
                printf("Selected: Dog\n");
                continue;
            }
            
            // Soldier, Ammo buttons
            rightPanelY += 55;
            SDL_Rect soldierButton = {rightPanelX, rightPanelY, elemButtonSize, elemButtonSize};
            if(mouseX >= soldierButton.x && mouseX <= soldierButton.x + soldierButton.w &&
               mouseY >= soldierButton.y && mouseY <= soldierButton.y + soldierButton.h) {
                selectedElement = ELEM_ENEMY_SOLDIER;
                printf("Selected: Soldier\n");
                continue;
            }
            
            SDL_Rect ammoButton = {rightPanelX + 60, rightPanelY, elemButtonSize, elemButtonSize};
            if(mouseX >= ammoButton.x && mouseX <= ammoButton.x + ammoButton.w &&
               mouseY >= ammoButton.y && mouseY <= ammoButton.y + ammoButton.h) {
                selectedElement = ELEM_ITEM_AMMO;
                printf("Selected: Ammo\n");
                continue;
            }
            
            // Health, Spawn buttons
            rightPanelY += 55;
            SDL_Rect healthButton = {rightPanelX, rightPanelY, elemButtonSize, elemButtonSize};
            if(mouseX >= healthButton.x && mouseX <= healthButton.x + healthButton.w &&
               mouseY >= healthButton.y && mouseY <= healthButton.y + healthButton.h) {
                selectedElement = ELEM_ITEM_HEALTH;
                printf("Selected: Health\n");
                continue;
            }
            
            SDL_Rect spawnButton = {rightPanelX + 60, rightPanelY, elemButtonSize, elemButtonSize};
            if(mouseX >= spawnButton.x && mouseX <= spawnButton.x + spawnButton.w &&
               mouseY >= spawnButton.y && mouseY <= spawnButton.y + spawnButton.h) {
                selectedElement = ELEM_SPAWN;
                printf("Selected: Spawn\n");
                continue;
            }
            
            // BOTTOM PANEL: Actions
            int bottomY = SCREEN_HEIGHT - 70;
            int bottomButtonWidth = 140;
            int bottomButtonHeight = 50;
            int bottomSpacing = 160;
            int startX = SCREEN_WIDTH / 2 - bottomSpacing * 2;
            
            for(int i = 0; i < 4; i++) {
                SDL_Rect actionButton = {startX + i * bottomSpacing, bottomY, bottomButtonWidth, bottomButtonHeight};
                if(mouseX >= actionButton.x && mouseX <= actionButton.x + actionButton.w &&
                   mouseY >= actionButton.y && mouseY <= actionButton.y + actionButton.h) {
                    if(i == 0) {
                        // SAVE
                        exportMapToJSON();
                    } else if(i == 1) {
                        // LOAD
                        EM_ASM({
                            var input = document.createElement('input');
                            input.type = 'file';
                            input.accept = '.json';
                            input.onchange = function(e) {
                                var file = e.target.files[0];
                                if(file) {
                                    var reader = new FileReader();
                                    reader.onload = function(event) {
                                        var jsonData = event.target.result;
                                        var lengthBytes = lengthBytesUTF8(jsonData) + 1;
                                        var jsonPtr = _malloc(lengthBytes);
                                        stringToUTF8(jsonData, jsonPtr, lengthBytes);
                                        _loadMapFromJSON(jsonPtr);
                                        _free(jsonPtr);
                                    };
                                    reader.readAsText(file);
                                }
                            };
                            input.click();
                        });
                    } else if(i == 2) {
                        // TEST
                        testMap();
                        return;
                    } else if(i == 3) {
                        // BACK TO MENU
                        currentGameState = STATE_MAIN_MENU;
                        return;
                    }
                    continue;
                }
            }
            
            // If no UI button was clicked, try to place/erase on map
            if(hoveredTileX >= 0 && hoveredTileY >= 0 && 
               hoveredTileX < editorMapWidth && hoveredTileY < editorMapHeight) {
                
                if(currentTool == TOOL_PAINT) {
                    // PAINT mode: place selected element
                    if(selectedElement >= ELEM_WALL_1 && selectedElement <= ELEM_WALL_4) {
                        // Place wall
                        int wallType = selectedElement - ELEM_WALL_1 + 1;
                        editorWorldMap[hoveredTileX][hoveredTileY] = wallType;
                        printf("Placed wall %d at (%d, %d)\n", wallType, hoveredTileX, hoveredTileY);
                    }
                    else if(selectedElement == ELEM_LIGHT) {
                        // Place light
                        LightSource newLight;
                        newLight.x = hoveredTileX + 0.5;
                        newLight.y = hoveredTileY + 0.5;
                        newLight.radius = 5.0f;
                        newLight.r = 255;
                        newLight.g = 200;
                        newLight.b = 100;
                        newLight.intensity = 0.7f;
                        newLight.active = true;
                        editorLights.push_back(newLight);
                        printf("Placed light at (%d, %d)\n", hoveredTileX, hoveredTileY);
                    }
                    else if(selectedElement == ELEM_ENEMY_DOG || selectedElement == ELEM_ENEMY_SOLDIER) {
                        // Place enemy
                        EnemyType enemyType = (selectedElement == ELEM_ENEMY_DOG) ? DOG : SOLDIER;
                        Enemy newEnemy(hoveredTileX + 0.5, hoveredTileY + 0.5, enemyType);
                        editorEnemies.push_back(newEnemy);
                        printf("Placed %s at (%d, %d)\n", 
                               selectedElement == ELEM_ENEMY_DOG ? "dog" : "soldier",
                               hoveredTileX, hoveredTileY);
                    }
                    else if(selectedElement == ELEM_ITEM_AMMO || selectedElement == ELEM_ITEM_HEALTH) {
                        // Place item
                        ItemType itemType = (selectedElement == ELEM_ITEM_AMMO) ? ITEM_AMMO : ITEM_HEALTH;
                        Item newItem(hoveredTileX + 0.5, hoveredTileY + 0.5, itemType);
                        editorItems.push_back(newItem);
                        printf("Placed %s at (%d, %d)\n", 
                               selectedElement == ELEM_ITEM_AMMO ? "ammo" : "health",
                               hoveredTileX, hoveredTileY);
                    }
                    else if(selectedElement == ELEM_SPAWN) {
                        // Set spawn point
                        spawnX = hoveredTileX + 0.5;
                        spawnY = hoveredTileY + 0.5;
                        printf("Spawn point set to (%.1f, %.1f)\n", spawnX, spawnY);
                    }
                }
                else {
                    // ERASE mode: remove everything at this tile
                    editorWorldMap[hoveredTileX][hoveredTileY] = 0;
                    
                    // Remove lights at this position
                    editorLights.erase(
                        std::remove_if(editorLights.begin(), editorLights.end(),
                            [&](const LightSource& l) {
                                return (int)l.x == hoveredTileX && (int)l.y == hoveredTileY;
                            }),
                        editorLights.end()
                    );
                    
                    // Remove enemies at this position
                    editorEnemies.erase(
                        std::remove_if(editorEnemies.begin(), editorEnemies.end(),
                            [&](const Enemy& e) {
                                return (int)e.x == hoveredTileX && (int)e.y == hoveredTileY;
                            }),
                        editorEnemies.end()
                    );
                    
                    // Remove items at this position
                    editorItems.erase(
                        std::remove_if(editorItems.begin(), editorItems.end(),
                            [&](const Item& i) {
                                return (int)i.x == hoveredTileX && (int)i.y == hoveredTileY;
                            }),
                        editorItems.end()
                    );
                    
                    printf("Erased tile at (%d, %d)\n", hoveredTileX, hoveredTileY);
                }
            }
        }
    }
}

void exportMapToJSON() {
    printf("Exporting map to JSON...\n");
    
    std::string json = "{\n";
    char buffer[512];
    
    // Map dimensions
    sprintf(buffer, "  \"width\": %d,\n", editorMapWidth);
    json += buffer;
    sprintf(buffer, "  \"height\": %d,\n", editorMapHeight);
    json += buffer;
    
    // Walls (2D array)
    json += "  \"walls\": [\n";
    for(int y = 0; y < editorMapHeight; y++) {
        json += "    [";
        for(int x = 0; x < editorMapWidth; x++) {
            sprintf(buffer, "%d", editorWorldMap[x][y]);
            json += buffer;
            if(x < editorMapWidth - 1) json += ",";
        }
        json += "]";
        if(y < editorMapHeight - 1) json += ",";
        json += "\n";
    }
    json += "  ],\n";
    
    // Lights
    json += "  \"lights\": [\n";
    for(size_t i = 0; i < editorLights.size(); i++) {
        const auto& light = editorLights[i];
        sprintf(buffer, "    {\"x\": %.2f, \"y\": %.2f, \"radius\": %.2f, \"r\": %d, \"g\": %d, \"b\": %d, \"intensity\": %.2f}",
                light.x, light.y, light.radius, light.r, light.g, light.b, light.intensity);
        json += buffer;
        if(i < editorLights.size() - 1) json += ",";
        json += "\n";
    }
    json += "  ],\n";
    
    // Enemies
    json += "  \"enemies\": [\n";
    for(size_t i = 0; i < editorEnemies.size(); i++) {
        const auto& enemy = editorEnemies[i];
        sprintf(buffer, "    {\"x\": %.2f, \"y\": %.2f, \"type\": \"%s\"}",
                enemy.x, enemy.y, enemy.type == DOG ? "DOG" : "SOLDIER");
        json += buffer;
        if(i < editorEnemies.size() - 1) json += ",";
        json += "\n";
    }
    json += "  ],\n";
    
    // Items
    json += "  \"items\": [\n";
    for(size_t i = 0; i < editorItems.size(); i++) {
        const auto& item = editorItems[i];
        sprintf(buffer, "    {\"x\": %.2f, \"y\": %.2f, \"type\": \"%s\"}",
                item.x, item.y, item.type == ITEM_AMMO ? "AMMO" : "HEALTH");
        json += buffer;
        if(i < editorItems.size() - 1) json += ",";
        json += "\n";
    }
    json += "  ],\n";
    
    // Spawn point
    sprintf(buffer, "  \"spawn\": {\"x\": %.2f, \"y\": %.2f}\n", spawnX, spawnY);
    json += buffer;
    
    json += "}\n";
    
    printf("JSON export complete! Size: %d bytes\n", (int)json.length());
    
    // Download JSON file
    EM_ASM({
        var jsonStr = UTF8ToString($0);
        var blob = new Blob([jsonStr], { type: 'application/json' });
        var url = URL.createObjectURL(blob);
        var a = document.createElement('a');
        a.href = url;
        a.download = 'wolfenstein_map_' + Date.now() + '.json';
        document.body.appendChild(a);
        a.click();
        document.body.removeChild(a);
        URL.revokeObjectURL(url);
        console.log('Map JSON file download initiated');
    }, json.c_str());
}

void loadMapFromJSON(const char* jsonData) {
    printf("Loading map from JSON...\n");
    
    // Parse JSON manually (simple parser for our specific format)
    std::string json(jsonData);
    
    // Clear current map
    for(int y = 0; y < 40; y++) {
        for(int x = 0; x < 40; x++) {
            editorWorldMap[x][y] = 0;
        }
    }
    editorLights.clear();
    editorEnemies.clear();
    editorItems.clear();
    
    // Parse width and height
    size_t pos = json.find("\"width\":");
    if(pos != std::string::npos) {
        sscanf(json.c_str() + pos + 8, "%d", &editorMapWidth);
    }
    pos = json.find("\"height\":");
    if(pos != std::string::npos) {
        sscanf(json.c_str() + pos + 9, "%d", &editorMapHeight);
    }
    
    printf("Map dimensions: %dx%d\n", editorMapWidth, editorMapHeight);
    
    // Parse walls array
    pos = json.find("\"walls\":");
    if(pos != std::string::npos) {
        const char* wallData = json.c_str() + pos + 8;
        // Find the opening bracket of the array
        while(*wallData && *wallData != '[') wallData++;
        wallData++; // Skip opening bracket
        
        for(int y = 0; y < editorMapHeight && *wallData; y++) {
            // Find opening bracket for this row
            while(*wallData && *wallData != '[') wallData++;
            wallData++;
            
            for(int x = 0; x < editorMapWidth && *wallData; x++) {
                while(*wallData && (*wallData == ' ' || *wallData == '\n')) wallData++;
                editorWorldMap[x][y] = atoi(wallData);
                // Skip to next number or end
                while(*wallData && *wallData != ',' && *wallData != ']') wallData++;
                if(*wallData == ',') wallData++;
            }
        }
    }
    
    // Parse lights
    pos = json.find("\"lights\":");
    if(pos != std::string::npos) {
        const char* lightData = json.c_str() + pos + 9;
        while(*lightData && *lightData != '[') lightData++;
        lightData++;
        
        while(*lightData) {
            // Find next light object
            while(*lightData && *lightData != '{') {
                if(*lightData == ']') break;
                lightData++;
            }
            if(*lightData != '{') break;
            
            LightSource light;
            float x, y, radius, intensity;
            int r, g, b;
            if(sscanf(lightData, "{\"x\": %f, \"y\": %f, \"radius\": %f, \"r\": %d, \"g\": %d, \"b\": %d, \"intensity\": %f",
                     &x, &y, &radius, &r, &g, &b, &intensity) == 7) {
                light.x = x;
                light.y = y;
                light.radius = radius;
                light.r = r;
                light.g = g;
                light.b = b;
                light.intensity = intensity;
                light.active = true;
                editorLights.push_back(light);
            }
            
            // Skip to end of this object
            while(*lightData && *lightData != '}') lightData++;
            if(*lightData == '}') lightData++;
        }
    }
    
    // Parse enemies
    pos = json.find("\"enemies\":");
    if(pos != std::string::npos) {
        const char* enemyData = json.c_str() + pos + 10;
        while(*enemyData && *enemyData != '[') enemyData++;
        enemyData++;
        
        while(*enemyData) {
            while(*enemyData && *enemyData != '{') {
                if(*enemyData == ']') break;
                enemyData++;
            }
            if(*enemyData != '{') break;
            
            float x, y;
            char type[16];
            if(sscanf(enemyData, "{\"x\": %f, \"y\": %f, \"type\": \"%[^\"]\"", &x, &y, type) == 3) {
                EnemyType enemyType = (strcmp(type, "DOG") == 0) ? DOG : SOLDIER;
                Enemy enemy(x, y, enemyType);
                editorEnemies.push_back(enemy);
            }
            
            while(*enemyData && *enemyData != '}') enemyData++;
            if(*enemyData == '}') enemyData++;
        }
    }
    
    // Parse items
    pos = json.find("\"items\":");
    if(pos != std::string::npos) {
        const char* itemData = json.c_str() + pos + 8;
        while(*itemData && *itemData != '[') itemData++;
        itemData++;
        
        while(*itemData) {
            while(*itemData && *itemData != '{') {
                if(*itemData == ']') break;
                itemData++;
            }
            if(*itemData != '{') break;
            
            float x, y;
            char type[16];
            if(sscanf(itemData, "{\"x\": %f, \"y\": %f, \"type\": \"%[^\"]\"", &x, &y, type) == 3) {
                ItemType itemType = (strcmp(type, "AMMO") == 0) ? ITEM_AMMO : ITEM_HEALTH;
                Item item(x, y, itemType);
                editorItems.push_back(item);
            }
            
            while(*itemData && *itemData != '}') itemData++;
            if(*itemData == '}') itemData++;
        }
    }
    
    // Parse spawn
    pos = json.find("\"spawn\":");
    if(pos != std::string::npos) {
        float x, y;
        if(sscanf(json.c_str() + pos + 8, "{\"x\": %f, \"y\": %f", &x, &y) == 2) {
            spawnX = x;
            spawnY = y;
        }
    }
    
    printf("Map loaded: %d walls, %d lights, %d enemies, %d items, spawn at (%.1f, %.1f)\n",
           editorMapWidth * editorMapHeight, (int)editorLights.size(), (int)editorEnemies.size(),
           (int)editorItems.size(), spawnX, spawnY);
}

void testMap() {
    printf("Testing map...\n");
    
    // Copy editor map to game map
    for(int y = 0; y < editorMapHeight; y++) {
        for(int x = 0; x < editorMapWidth; x++) {
            if(x < MAP_WIDTH && y < MAP_HEIGHT) {
                worldMap[x][y] = editorWorldMap[x][y];
            }
        }
    }
    
    // Clear remaining areas if editor map is smaller
    for(int y = editorMapHeight; y < MAP_HEIGHT; y++) {
        for(int x = 0; x < MAP_WIDTH; x++) {
            worldMap[x][y] = 0;
        }
    }
    for(int y = 0; y < MAP_HEIGHT; y++) {
        for(int x = editorMapWidth; x < MAP_WIDTH; x++) {
            worldMap[x][y] = 0;
        }
    }
    
    // Copy lights
    lightSources.clear();
    for(const auto& light : editorLights) {
        lightSources.push_back(light);
    }
    
    // Copy enemies
    enemies.clear();
    totalEnemies = 0;
    for(const auto& editorEnemy : editorEnemies) {
        enemies.push_back(editorEnemy);
        totalEnemies++;
    }
    
    // Copy items
    items.clear();
    for(const auto& editorItem : editorItems) {
        items.push_back(editorItem);
    }
    
    // Initialize item lights
    for(auto& item : items) {
        item.lightIndex = lightSources.size();
        if(item.type == ITEM_HEALTH) {
            lightSources.push_back({item.x, item.y, 3.0f, 255, 50, 50, 0.6f, true});
        } else if(item.type == ITEM_AMMO) {
            lightSources.push_back({item.x, item.y, 3.0f, 255, 200, 50, 0.6f, true});
        }
    }
    
    // Set player spawn position
    posX = spawnX;
    posY = spawnY;
    
    // Reset player state
    playerHealth = 100;
    ammo = 50;
    kills = 0;
    gameOver = false;
    gameWon = false;
    dirX = -1.0;
    dirY = 0.0;
    planeX = 0.0;
    planeY = 0.66;
    
    // Reset replay state
    isReplaying = false;
    recordedFrames.clear();
    replayCurrentFrame = 0;
    replayPaused = false;
    
    printf("Map test: Player spawned at (%.1f, %.1f), %d enemies, %d items\n",
           posX, posY, totalEnemies, (int)items.size());
    
    // Switch to gameplay
    currentGameState = STATE_PLAYING;
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
            else if(event.key.keysym.sym == SDLK_5) {
                currentWeapon = BAZOOKA;
                printf("Arma: Bazooka\n");
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
            // ESC: Return to main menu
            else if(event.key.keysym.sym == SDLK_ESCAPE) {
                printf(">>> Returning to main menu...\n");
                currentGameState = STATE_MAIN_MENU;
                return;
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
    
    // MAP EDITOR button
    buttonY += 80;
    
    // Button shadow
    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 100);
    SDL_Rect editorShadow = {buttonX + 4, buttonY + 4, buttonWidth, buttonHeight};
    SDL_RenderFillRect(renderer, &editorShadow);
    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_NONE);
    
    // Button gradient (orange theme)
    for(int i = 0; i < buttonHeight; i++) {
        int r = 150 + (i * 30) / buttonHeight;
        int g = 80 - (i * 20) / buttonHeight;
        SDL_SetRenderDrawColor(renderer, r, g, 30, 255);
        SDL_RenderDrawLine(renderer, buttonX, buttonY + i, buttonX + buttonWidth, buttonY + i);
    }
    
    // Button border
    SDL_SetRenderDrawColor(renderer, 255, 150, 50, 255);
    SDL_Rect editorBorder = {buttonX - 3, buttonY - 3, buttonWidth + 6, buttonHeight + 6};
    SDL_RenderDrawRect(renderer, &editorBorder);
    SDL_Rect editorBorder2 = {buttonX - 2, buttonY - 2, buttonWidth + 4, buttonHeight + 4};
    SDL_RenderDrawRect(renderer, &editorBorder2);
    
    // Draw "MAP EDITOR" text with shadow
    drawText("MAP EDITOR", buttonX + 25, buttonY + 22, 0, 0, 0, 3);
    drawText("MAP EDITOR", buttonX + 24, buttonY + 21, 255, 255, 255, 3);
    
    // Hint text
    drawText("(CREATE LEVELS)", buttonX + 75, buttonY + 46, 255, 200, 100, 2);
    
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
            
            // Check MAP EDITOR button (third button)
            buttonY += buttonSpacing;
            if(mouseX >= buttonX && mouseX <= buttonX + buttonWidth &&
               mouseY >= buttonY && mouseY <= buttonY + buttonHeight) {
                printf(">>> MAP EDITOR clicked!\n");
                
                // Copy current game map to editor
                for(int y = 0; y < MAP_HEIGHT; y++) {
                    for(int x = 0; x < MAP_WIDTH; x++) {
                        editorWorldMap[x][y] = worldMap[x][y];
                    }
                }
                
                // Copy entities to editor
                editorLights.clear();
                for(const auto& light : lightSources) {
                    if(light.active) editorLights.push_back(light);
                }
                editorEnemies = enemies;
                editorItems = items;
                
                // Set editor dimensions to current map size
                editorMapWidth = 24;  // Default, can be changed in editor
                editorMapHeight = 24;
                
                // Switch to editor
                currentGameState = STATE_MAP_EDITOR;
                return;
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

// ============================================================================
// MAP EDITOR RENDERING (ISOMETRIC VIEW)
// ============================================================================

void renderIsometricMap() {
    // Clear screen with dark gray
    SDL_SetRenderDrawColor(renderer, 40, 40, 40, 255);
    SDL_RenderClear(renderer);
    
    const int tileWidth = 32;
    const int tileHeight = 16;
    const int wallHeight = 24;
    const int centerX = SCREEN_WIDTH / 2;
    const int centerY = SCREEN_HEIGHT / 2 - 100;
    
    // Update hovered tile based on mouse position
    int mouseX, mouseY;
    SDL_GetMouseState(&mouseX, &mouseY);
    hoveredTileX = -1;
    hoveredTileY = -1;
    
    // Helper lambda to convert map coords to isometric screen coords with rotation
    auto mapToIso = [&](int mapX, int mapY) -> std::pair<int, int> {
        // Apply rotation to map coordinates
        int rotX = mapX;
        int rotY = mapY;
        
        if(cameraRotation == 90.0f) {
            rotX = mapY;
            rotY = editorMapWidth - 1 - mapX;
        } else if(cameraRotation == 180.0f) {
            rotX = editorMapWidth - 1 - mapX;
            rotY = editorMapHeight - 1 - mapY;
        } else if(cameraRotation == 270.0f) {
            rotX = editorMapHeight - 1 - mapY;
            rotY = mapX;
        }
        
        // Isometric projection
        int isoX = centerX + (rotX - rotY) * tileWidth / 2;
        int isoY = centerY + (rotX + rotY) * tileHeight / 2;
        
        return {isoX, isoY};
    };
    
    // Render in back-to-front order for proper depth
    for(int y = 0; y < editorMapHeight; y++) {
        for(int x = 0; x < editorMapWidth; x++) {
            std::pair<int, int> iso = mapToIso(x, y);
            int isoX = iso.first;
            int isoY = iso.second;
            
            int wallType = editorWorldMap[x][y];
            
            // 1. Draw floor tile (light gray diamond)
            SDL_Point floorPoints[5] = {
                {isoX, isoY},
                {isoX + tileWidth/2, isoY + tileHeight/2},
                {isoX, isoY + tileHeight},
                {isoX - tileWidth/2, isoY + tileHeight/2},
                {isoX, isoY}
            };
            SDL_SetRenderDrawColor(renderer, 80, 80, 80, 255);
            for(int i = 0; i < 4; i++) {
                SDL_RenderDrawLine(renderer, floorPoints[i].x, floorPoints[i].y, 
                                 floorPoints[i+1].x, floorPoints[i+1].y);
            }
            
            // Fill floor
            for(int dy = 0; dy < tileHeight; dy++) {
                int width = (dy < tileHeight/2) ? dy * 2 : (tileHeight - dy) * 2;
                SDL_SetRenderDrawColor(renderer, 70, 70, 70, 255);
                SDL_RenderDrawLine(renderer, isoX - width/2, isoY + dy, isoX + width/2, isoY + dy);
            }
            
            // 2. Draw grid if enabled
            if(gridEnabled) {
                SDL_SetRenderDrawColor(renderer, 100, 100, 100, 255);
                for(int i = 0; i < 4; i++) {
                    SDL_RenderDrawLine(renderer, floorPoints[i].x, floorPoints[i].y, 
                                     floorPoints[i+1].x, floorPoints[i+1].y);
                }
            }
            
            // 3. Draw walls with 3D height
            if(wallType > 0) {
                int r, g, b;
                if(wallType == 1) { r = 180; g = 60; b = 60; }       // Red brick
                else if(wallType == 2) { r = 120; g = 120; b = 120; } // Gray stone
                else if(wallType == 3) { r = 60; g = 100; b = 180; }  // Blue metal
                else { r = 100; g = 140; b = 80; }                    // Green mortar
                
                // Top face (diamond)
                SDL_Point topPoints[5] = {
                    {isoX, isoY - wallHeight},
                    {isoX + tileWidth/2, isoY + tileHeight/2 - wallHeight},
                    {isoX, isoY + tileHeight - wallHeight},
                    {isoX - tileWidth/2, isoY + tileHeight/2 - wallHeight},
                    {isoX, isoY - wallHeight}
                };
                SDL_SetRenderDrawColor(renderer, r, g, b, 255);
                for(int dy = 0; dy < tileHeight; dy++) {
                    int width = (dy < tileHeight/2) ? dy * 2 : (tileHeight - dy) * 2;
                    SDL_RenderDrawLine(renderer, isoX - width/2, isoY + dy - wallHeight, 
                                     isoX + width/2, isoY + dy - wallHeight);
                }
                
                // Left face
                SDL_SetRenderDrawColor(renderer, r * 0.7, g * 0.7, b * 0.7, 255);
                SDL_Point leftFace[4] = {
                    {isoX - tileWidth/2, isoY + tileHeight/2},
                    {isoX, isoY + tileHeight},
                    {isoX, isoY + tileHeight - wallHeight},
                    {isoX - tileWidth/2, isoY + tileHeight/2 - wallHeight}
                };
                for(int i = 0; i < 4; i++) {
                    SDL_RenderDrawLine(renderer, leftFace[i].x, leftFace[i].y, 
                                     leftFace[(i+1)%4].x, leftFace[(i+1)%4].y);
                }
                for(int dy = 0; dy < wallHeight; dy++) {
                    SDL_RenderDrawLine(renderer, isoX - tileWidth/2, isoY + tileHeight/2 - dy,
                                     isoX, isoY + tileHeight - dy);
                }
                
                // Right face
                SDL_SetRenderDrawColor(renderer, r * 0.5, g * 0.5, b * 0.5, 255);
                SDL_Point rightFace[4] = {
                    {isoX, isoY + tileHeight},
                    {isoX + tileWidth/2, isoY + tileHeight/2},
                    {isoX + tileWidth/2, isoY + tileHeight/2 - wallHeight},
                    {isoX, isoY + tileHeight - wallHeight}
                };
                for(int dy = 0; dy < wallHeight; dy++) {
                    SDL_RenderDrawLine(renderer, isoX, isoY + tileHeight - dy,
                                     isoX + tileWidth/2, isoY + tileHeight/2 - dy);
                }
            }
            
            // Check if mouse is hovering this tile (simple bounds check)
            if(mouseX >= isoX - tileWidth/2 && mouseX <= isoX + tileWidth/2 &&
               mouseY >= isoY - wallHeight && mouseY <= isoY + tileHeight) {
                hoveredTileX = x;
                hoveredTileY = y;
            }
        }
    }
    
    // 4. Draw lights
    for(const auto& light : editorLights) {
        int lx = (int)light.x;
        int ly = (int)light.y;
        if(lx >= 0 && lx < editorMapWidth && ly >= 0 && ly < editorMapHeight) {
            std::pair<int, int> iso = mapToIso(lx, ly);
            int isoX = iso.first;
            int isoY = iso.second;
            
            // Yellow circle
            SDL_SetRenderDrawColor(renderer, 255, 255, 100, 255);
            for(int r = 0; r < 8; r++) {
                for(int a = 0; a < 360; a += 10) {
                    int px = isoX + r * cos(a * 3.14159 / 180.0);
                    int py = isoY + r * sin(a * 3.14159 / 180.0) - 10;
                    SDL_RenderDrawPoint(renderer, px, py);
                }
            }
        }
    }
    
    // 5. Draw enemies
    for(const auto& enemy : editorEnemies) {
        int ex = (int)enemy.x;
        int ey = (int)enemy.y;
        if(ex >= 0 && ex < editorMapWidth && ey >= 0 && ey < editorMapHeight) {
            std::pair<int, int> iso = mapToIso(ex, ey);
            int isoX = iso.first;
            int isoY = iso.second;
            
            if(enemy.type == DOG) {
                // Brown dog
                SDL_SetRenderDrawColor(renderer, 120, 80, 40, 255);
            } else {
                // Gray soldier
                SDL_SetRenderDrawColor(renderer, 150, 150, 150, 255);
            }
            
            SDL_Rect enemyRect = {isoX - 6, isoY - 16, 12, 16};
            SDL_RenderFillRect(renderer, &enemyRect);
            SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
            SDL_RenderDrawRect(renderer, &enemyRect);
        }
    }
    
    // 6. Draw items
    for(const auto& item : editorItems) {
        int ix = (int)item.x;
        int iy = (int)item.y;
        if(ix >= 0 && ix < editorMapWidth && iy >= 0 && iy < editorMapHeight) {
            std::pair<int, int> iso = mapToIso(ix, iy);
            int isoX = iso.first;
            int isoY = iso.second;
            
            if(item.type == ITEM_AMMO) {
                // Yellow ammo box
                SDL_SetRenderDrawColor(renderer, 200, 200, 0, 255);
                SDL_Rect ammoRect = {isoX - 5, isoY - 8, 10, 8};
                SDL_RenderFillRect(renderer, &ammoRect);
                SDL_SetRenderDrawColor(renderer, 100, 100, 0, 255);
                SDL_RenderDrawRect(renderer, &ammoRect);
            } else {
                // Red health pack with white cross
                SDL_SetRenderDrawColor(renderer, 200, 0, 0, 255);
                SDL_Rect healthRect = {isoX - 5, isoY - 8, 10, 8};
                SDL_RenderFillRect(renderer, &healthRect);
                
                SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
                SDL_RenderDrawLine(renderer, isoX - 3, isoY - 4, isoX + 3, isoY - 4);
                SDL_RenderDrawLine(renderer, isoX, isoY - 7, isoX, isoY - 1);
            }
        }
    }
    
    // 7. Draw spawn point
    {
        int sx = (int)spawnX;
        int sy = (int)spawnY;
        if(sx >= 0 && sx < editorMapWidth && sy >= 0 && sy < editorMapHeight) {
            std::pair<int, int> iso = mapToIso(sx, sy);
            int isoX = iso.first;
            int isoY = iso.second;
            
            // Green circle
            SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
            for(int r = 0; r < 10; r++) {
                for(int a = 0; a < 360; a += 10) {
                    int px = isoX + r * cos(a * 3.14159 / 180.0);
                    int py = isoY + r * sin(a * 3.14159 / 180.0) - 15;
                    SDL_RenderDrawPoint(renderer, px, py);
                }
            }
            
            // Draw "S"
            drawText("S", isoX - 4, isoY - 20, 255, 255, 255, 1);
        }
    }
    
    // 8. Highlight hovered tile
    if(hoveredTileX >= 0 && hoveredTileY >= 0) {
        std::pair<int, int> iso = mapToIso(hoveredTileX, hoveredTileY);
        int isoX = iso.first;
        int isoY = iso.second;
        
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        SDL_Point highlightPoints[5] = {
            {isoX, isoY},
            {isoX + tileWidth/2, isoY + tileHeight/2},
            {isoX, isoY + tileHeight},
            {isoX - tileWidth/2, isoY + tileHeight/2},
            {isoX, isoY}
        };
        for(int i = 0; i < 4; i++) {
            SDL_RenderDrawLine(renderer, highlightPoints[i].x, highlightPoints[i].y, 
                             highlightPoints[i+1].x, highlightPoints[i+1].y);
        }
    }
}

void drawMapEditorUI() {
    // Title
    drawText("MAP EDITOR", SCREEN_WIDTH / 2 - 100, 10, 255, 255, 100, 3);
    
    // ===== LEFT PANEL: TOOLS =====
    int leftPanelX = 10;
    int leftPanelY = 80;
    int buttonWidth = 120;
    int buttonHeight = 40;
    int buttonSpacing = 50;
    
    // Panel background
    SDL_SetRenderDrawColor(renderer, 30, 30, 30, 200);
    SDL_Rect leftPanel = {leftPanelX - 5, leftPanelY - 5, buttonWidth + 10, buttonHeight * 3 + buttonSpacing * 2 + 10};
    SDL_RenderFillRect(renderer, &leftPanel);
    SDL_SetRenderDrawColor(renderer, 100, 100, 100, 255);
    SDL_RenderDrawRect(renderer, &leftPanel);
    
    // PAINT / ERASE button
    SDL_Rect paintButton = {leftPanelX, leftPanelY, buttonWidth, buttonHeight};
    if(currentTool == TOOL_PAINT) {
        SDL_SetRenderDrawColor(renderer, 0, 150, 0, 255);
    } else {
        SDL_SetRenderDrawColor(renderer, 60, 60, 60, 255);
    }
    SDL_RenderFillRect(renderer, &paintButton);
    SDL_SetRenderDrawColor(renderer, 200, 200, 200, 255);
    SDL_RenderDrawRect(renderer, &paintButton);
    drawText(currentTool == TOOL_PAINT ? "PAINT" : "ERASE", leftPanelX + 20, leftPanelY + 13, 255, 255, 255, 2);
    
    // GRID toggle button
    leftPanelY += buttonSpacing;
    SDL_Rect gridButton = {leftPanelX, leftPanelY, buttonWidth, buttonHeight};
    if(gridEnabled) {
        SDL_SetRenderDrawColor(renderer, 0, 100, 150, 255);
    } else {
        SDL_SetRenderDrawColor(renderer, 60, 60, 60, 255);
    }
    SDL_RenderFillRect(renderer, &gridButton);
    SDL_SetRenderDrawColor(renderer, 200, 200, 200, 255);
    SDL_RenderDrawRect(renderer, &gridButton);
    drawText(gridEnabled ? "GRID:ON" : "GRID:OFF", leftPanelX + 15, leftPanelY + 13, 255, 255, 255, 2);
    
    // Size selector buttons
    leftPanelY += buttonSpacing;
    const char* sizes[] = {"16x16", "24x24", "32x32"};
    int sizeValues[] = {16, 24, 32};
    for(int i = 0; i < 3; i++) {
        SDL_Rect sizeButton = {leftPanelX, leftPanelY + i * 35, buttonWidth, 30};
        if(editorMapWidth == sizeValues[i]) {
            SDL_SetRenderDrawColor(renderer, 150, 100, 0, 255);
        } else {
            SDL_SetRenderDrawColor(renderer, 60, 60, 60, 255);
        }
        SDL_RenderFillRect(renderer, &sizeButton);
        SDL_SetRenderDrawColor(renderer, 200, 200, 200, 255);
        SDL_RenderDrawRect(renderer, &sizeButton);
        drawText(sizes[i], leftPanelX + 30, leftPanelY + i * 35 + 8, 255, 255, 255, 2);
    }
    
    // ===== RIGHT PANEL: ELEMENTS =====
    int rightPanelX = SCREEN_WIDTH - 140;
    int rightPanelY = 80;
    int elemButtonSize = 50;
    int elemSpacing = 55;
    
    // Panel background
    SDL_SetRenderDrawColor(renderer, 30, 30, 30, 200);
    SDL_Rect rightPanel = {rightPanelX - 5, rightPanelY - 5, 130, elemSpacing * 3 + 10};
    SDL_RenderFillRect(renderer, &rightPanel);
    SDL_SetRenderDrawColor(renderer, 100, 100, 100, 255);
    SDL_RenderDrawRect(renderer, &rightPanel);
    
    // Wall buttons (2x2 grid)
    const char* wallNames[] = {"W1", "W2", "W3", "W4"};
    int wallColors[][3] = {{180, 60, 60}, {120, 120, 120}, {60, 100, 180}, {100, 140, 80}};
    for(int i = 0; i < 4; i++) {
        int row = i / 2;
        int col = i % 2;
        SDL_Rect wallButton = {rightPanelX + col * 60, rightPanelY + row * 55, elemButtonSize, elemButtonSize};
        
        if(selectedElement == (EditorElement)(ELEM_WALL_1 + i)) {
            SDL_SetRenderDrawColor(renderer, 255, 255, 0, 255);
            SDL_Rect highlight = {wallButton.x - 2, wallButton.y - 2, wallButton.w + 4, wallButton.h + 4};
            SDL_RenderFillRect(renderer, &highlight);
        }
        
        SDL_SetRenderDrawColor(renderer, wallColors[i][0], wallColors[i][1], wallColors[i][2], 255);
        SDL_RenderFillRect(renderer, &wallButton);
        SDL_SetRenderDrawColor(renderer, 200, 200, 200, 255);
        SDL_RenderDrawRect(renderer, &wallButton);
        drawText(wallNames[i], wallButton.x + 12, wallButton.y + 18, 255, 255, 255, 2);
    }
    
    // Light button
    rightPanelY += 110;
    SDL_Rect lightButton = {rightPanelX, rightPanelY, elemButtonSize, elemButtonSize};
    if(selectedElement == ELEM_LIGHT) {
        SDL_SetRenderDrawColor(renderer, 255, 255, 0, 255);
        SDL_Rect highlight = {lightButton.x - 2, lightButton.y - 2, lightButton.w + 4, lightButton.h + 4};
        SDL_RenderFillRect(renderer, &highlight);
    }
    SDL_SetRenderDrawColor(renderer, 255, 255, 100, 255);
    SDL_RenderFillRect(renderer, &lightButton);
    SDL_SetRenderDrawColor(renderer, 200, 200, 200, 255);
    SDL_RenderDrawRect(renderer, &lightButton);
    drawText("LIGHT", lightButton.x + 5, lightButton.y + 18, 0, 0, 0, 2);
    
    // Dog button
    SDL_Rect dogButton = {rightPanelX + 60, rightPanelY, elemButtonSize, elemButtonSize};
    if(selectedElement == ELEM_ENEMY_DOG) {
        SDL_SetRenderDrawColor(renderer, 255, 255, 0, 255);
        SDL_Rect highlight = {dogButton.x - 2, dogButton.y - 2, dogButton.w + 4, dogButton.h + 4};
        SDL_RenderFillRect(renderer, &highlight);
    }
    SDL_SetRenderDrawColor(renderer, 120, 80, 40, 255);
    SDL_RenderFillRect(renderer, &dogButton);
    SDL_SetRenderDrawColor(renderer, 200, 200, 200, 255);
    SDL_RenderDrawRect(renderer, &dogButton);
    drawText("DOG", dogButton.x + 10, dogButton.y + 18, 255, 255, 255, 2);
    
    // Soldier button
    rightPanelY += elemSpacing;
    SDL_Rect soldierButton = {rightPanelX, rightPanelY, elemButtonSize, elemButtonSize};
    if(selectedElement == ELEM_ENEMY_SOLDIER) {
        SDL_SetRenderDrawColor(renderer, 255, 255, 0, 255);
        SDL_Rect highlight = {soldierButton.x - 2, soldierButton.y - 2, soldierButton.w + 4, soldierButton.h + 4};
        SDL_RenderFillRect(renderer, &highlight);
    }
    SDL_SetRenderDrawColor(renderer, 150, 150, 150, 255);
    SDL_RenderFillRect(renderer, &soldierButton);
    SDL_SetRenderDrawColor(renderer, 200, 200, 200, 255);
    SDL_RenderDrawRect(renderer, &soldierButton);
    drawText("SOLD", soldierButton.x + 8, soldierButton.y + 18, 0, 0, 0, 2);
    
    // Ammo button
    SDL_Rect ammoButton = {rightPanelX + 60, rightPanelY, elemButtonSize, elemButtonSize};
    if(selectedElement == ELEM_ITEM_AMMO) {
        SDL_SetRenderDrawColor(renderer, 255, 255, 0, 255);
        SDL_Rect highlight = {ammoButton.x - 2, ammoButton.y - 2, ammoButton.w + 4, ammoButton.h + 4};
        SDL_RenderFillRect(renderer, &highlight);
    }
    SDL_SetRenderDrawColor(renderer, 200, 200, 0, 255);
    SDL_RenderFillRect(renderer, &ammoButton);
    SDL_SetRenderDrawColor(renderer, 200, 200, 200, 255);
    SDL_RenderDrawRect(renderer, &ammoButton);
    drawText("AMMO", ammoButton.x + 6, ammoButton.y + 18, 0, 0, 0, 2);
    
    // Health button
    rightPanelY += elemSpacing;
    SDL_Rect healthButton = {rightPanelX, rightPanelY, elemButtonSize, elemButtonSize};
    if(selectedElement == ELEM_ITEM_HEALTH) {
        SDL_SetRenderDrawColor(renderer, 255, 255, 0, 255);
        SDL_Rect highlight = {healthButton.x - 2, healthButton.y - 2, healthButton.w + 4, healthButton.h + 4};
        SDL_RenderFillRect(renderer, &highlight);
    }
    SDL_SetRenderDrawColor(renderer, 200, 0, 0, 255);
    SDL_RenderFillRect(renderer, &healthButton);
    SDL_SetRenderDrawColor(renderer, 200, 200, 200, 255);
    SDL_RenderDrawRect(renderer, &healthButton);
    // Draw cross
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderDrawLine(renderer, healthButton.x + 15, healthButton.y + 25, healthButton.x + 35, healthButton.y + 25);
    SDL_RenderDrawLine(renderer, healthButton.x + 25, healthButton.y + 15, healthButton.x + 25, healthButton.y + 35);
    
    // Spawn button
    SDL_Rect spawnButton = {rightPanelX + 60, rightPanelY, elemButtonSize, elemButtonSize};
    if(selectedElement == ELEM_SPAWN) {
        SDL_SetRenderDrawColor(renderer, 255, 255, 0, 255);
        SDL_Rect highlight = {spawnButton.x - 2, spawnButton.y - 2, spawnButton.w + 4, spawnButton.h + 4};
        SDL_RenderFillRect(renderer, &highlight);
    }
    SDL_SetRenderDrawColor(renderer, 0, 200, 0, 255);
    SDL_RenderFillRect(renderer, &spawnButton);
    SDL_SetRenderDrawColor(renderer, 200, 200, 200, 255);
    SDL_RenderDrawRect(renderer, &spawnButton);
    drawText("SPAWN", spawnButton.x + 5, spawnButton.y + 18, 255, 255, 255, 2);
    
    // ===== BOTTOM PANEL: ACTIONS =====
    int bottomY = SCREEN_HEIGHT - 70;
    int bottomButtonWidth = 140;
    int bottomButtonHeight = 50;
    int bottomSpacing = 160;
    int startX = SCREEN_WIDTH / 2 - bottomSpacing * 2;
    
    // Panel background
    SDL_SetRenderDrawColor(renderer, 30, 30, 30, 200);
    SDL_Rect bottomPanel = {startX - 10, bottomY - 5, bottomSpacing * 4 + 20, bottomButtonHeight + 10};
    SDL_RenderFillRect(renderer, &bottomPanel);
    SDL_SetRenderDrawColor(renderer, 100, 100, 100, 255);
    SDL_RenderDrawRect(renderer, &bottomPanel);
    
    const char* actionLabels[] = {"SAVE", "LOAD", "TEST", "BACK"};
    int actionColors[][3] = {{0, 120, 180}, {180, 120, 0}, {0, 180, 60}, {180, 60, 60}};
    
    for(int i = 0; i < 4; i++) {
        SDL_Rect actionButton = {startX + i * bottomSpacing, bottomY, bottomButtonWidth, bottomButtonHeight};
        SDL_SetRenderDrawColor(renderer, actionColors[i][0], actionColors[i][1], actionColors[i][2], 255);
        SDL_RenderFillRect(renderer, &actionButton);
        SDL_SetRenderDrawColor(renderer, 220, 220, 220, 255);
        SDL_RenderDrawRect(renderer, &actionButton);
        drawText(actionLabels[i], actionButton.x + 35, actionButton.y + 18, 255, 255, 255, 2);
    }
    
    // ===== INFO DISPLAY =====
    // Rotation indicator
    char rotText[32];
    snprintf(rotText, sizeof(rotText), "ROT: %.0f", cameraRotation);
    drawText(rotText, 10, SCREEN_HEIGHT - 100, 255, 255, 100, 2);
    
    // Map size indicator
    char sizeText[32];
    snprintf(sizeText, sizeof(sizeText), "SIZE: %dx%d", editorMapWidth, editorMapHeight);
    drawText(sizeText, 10, SCREEN_HEIGHT - 130, 200, 200, 200, 2);
    
    // Instructions
    drawText("ARROWS: Rotate | CLICK: Place/Erase", SCREEN_WIDTH / 2 - 180, 50, 200, 200, 200, 1);
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
        
        // Dibujar rayo láser rojo desde el arma hasta el centro (vertical)
        int centerX = SCREEN_WIDTH / 2;
        int centerY = SCREEN_HEIGHT / 2;
        int weaponY = SCREEN_HEIGHT - 100; // Posición del arma (abajo)
        
        SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
        
        // Resplandor exterior rojo grande
        for(int i = -12; i <= 12; i++) {
            SDL_SetRenderDrawColor(renderer, 255, 0, 0, 30);
            SDL_RenderDrawLine(renderer, centerX + i, weaponY, centerX + i, centerY);
        }
        
        // Resplandor medio rojo
        for(int i = -6; i <= 6; i++) {
            SDL_SetRenderDrawColor(renderer, 255, 50, 50, 100);
            SDL_RenderDrawLine(renderer, centerX + i, weaponY, centerX + i, centerY);
        }
        
        // Núcleo láser rojo intenso
        for(int i = -3; i <= 3; i++) {
            SDL_SetRenderDrawColor(renderer, 255, 100, 100, 200);
            SDL_RenderDrawLine(renderer, centerX + i, weaponY, centerX + i, centerY);
        }
        
        // Centro del láser (blanco-rojo brillante)
        SDL_SetRenderDrawColor(renderer, 255, 200, 200, 255);
        SDL_RenderDrawLine(renderer, centerX, weaponY, centerX, centerY);
        
        SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_NONE);
        
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
                    int trailSize = bullet.isBazooka ? std::max(2, int(35 / transformY)) : std::max(1, int(20 / transformY));
                    
                    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
                    
                    if(bullet.isBazooka) {
                        // Estela violeta para bazooka
                        SDL_SetRenderDrawColor(renderer, 180, 80, 230, alpha);
                        SDL_Rect trailRect = {screenX - trailSize/2, screenY - trailSize/2, trailSize, trailSize};
                        SDL_RenderFillRect(renderer, &trailRect);
                    } else {
                        // Estela azul brillante para balas normales
                        SDL_SetRenderDrawColor(renderer, 100, 150, 255, alpha);
                        SDL_Rect trailRect = {screenX - trailSize/2, screenY - trailSize/2, trailSize, trailSize};
                        SDL_RenderFillRect(renderer, &trailRect);
                    }
                    
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
                SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
                
                if(bullet.isBazooka) {
                    // === BALA DE BAZOOKA VIOLETA - MÁS GRANDE ===
                    int bulletSize = std::max(6, int(80 / transformY)); // Más grande que normal
                    
                    // Resplandor exterior violeta gigante
                    int glowSize = bulletSize + 15;
                    SDL_SetRenderDrawColor(renderer, 150, 0, 200, 80);
                    SDL_Rect outerGlow = {screenX - glowSize/2, screenY - glowSize/2, glowSize, glowSize};
                    SDL_RenderFillRect(renderer, &outerGlow);
                    
                    // Resplandor medio violeta
                    int midGlow = bulletSize + 8;
                    SDL_SetRenderDrawColor(renderer, 180, 50, 230, 150);
                    SDL_Rect midGlowRect = {screenX - midGlow/2, screenY - midGlow/2, midGlow, midGlow};
                    SDL_RenderFillRect(renderer, &midGlowRect);
                    
                    // Núcleo violeta brillante
                    SDL_SetRenderDrawColor(renderer, 200, 100, 255, 255);
                    SDL_Rect bulletRect = {screenX - bulletSize/2, screenY - bulletSize/2, bulletSize, bulletSize};
                    SDL_RenderFillRect(renderer, &bulletRect);
                    
                    // Centro blanco-violeta (core)
                    int coreSize = std::max(3, bulletSize / 2);
                    SDL_SetRenderDrawColor(renderer, 230, 180, 255, 255);
                    SDL_Rect coreRect = {screenX - coreSize/2, screenY - coreSize/2, coreSize, coreSize};
                    SDL_RenderFillRect(renderer, &coreRect);
                    
                    // Anillo exterior para efecto de cohete
                    SDL_SetRenderDrawColor(renderer, 255, 150, 255, 200);
                    for(int i = 0; i < 4; i++) {
                        SDL_Rect ring = {screenX - bulletSize/2 + i, screenY - bulletSize/2 + i, 
                                        bulletSize - i*2, bulletSize - i*2};
                        SDL_RenderDrawRect(renderer, &ring);
                    }
                } else {
                    // === BALA NORMAL AZUL ===
                    int bulletSize = std::max(3, int(50 / transformY));
                    
                    // Resplandor exterior azul grande
                    int glowSize = bulletSize + 8;
                    SDL_SetRenderDrawColor(renderer, 50, 100, 255, 100);
                    SDL_Rect outerGlow = {screenX - glowSize/2, screenY - glowSize/2, glowSize, glowSize};
                    SDL_RenderFillRect(renderer, &outerGlow);
                    
                    // Resplandor medio azul
                    int midGlow = bulletSize + 4;
                    SDL_SetRenderDrawColor(renderer, 100, 150, 255, 180);
                    SDL_Rect midGlowRect = {screenX - midGlow/2, screenY - midGlow/2, midGlow, midGlow};
                    SDL_RenderFillRect(renderer, &midGlowRect);
                    
                    // Núcleo azul brillante
                    SDL_SetRenderDrawColor(renderer, 150, 200, 255, 255);
                    SDL_Rect bulletRect = {screenX - bulletSize/2, screenY - bulletSize/2, bulletSize, bulletSize};
                    SDL_RenderFillRect(renderer, &bulletRect);
                    
                    // Centro blanco-azul (core)
                    int coreSize = std::max(2, bulletSize / 2);
                    SDL_SetRenderDrawColor(renderer, 200, 230, 255, 255);
                    SDL_Rect coreRect = {screenX - coreSize/2, screenY - coreSize/2, coreSize, coreSize};
                    SDL_RenderFillRect(renderer, &coreRect);
                }
                
                SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_NONE);
            }
        }
    }
    
    // Draw laser beams con estela MEJORADA - animación giratoria y efectos geniales
    for(const auto& beam : laserBeams) {
        if(!beam.active) continue;
        
        // Dibujar estela del láser con efectos mejorados
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
                    // Alpha decrece más lento para estela duradera
                    float ageRatio = (float)i / std::max((size_t)1, beam.trail.size());
                    int alpha = 80 + (int)(ageRatio * 175); // 80 a 255
                    int beamSize = std::max(3, int(70 / transformY));
                    
                    // Efecto de rotación espiral basado en edad y tiempo
                    float spiralRotation = beam.rotation + (ageRatio * 360.0f * 2); // 2 vueltas completas
                    
                    // Calcular offset de la espiral (órbita alrededor del centro)
                    float spiralRadius = beamSize * 0.3f * (1.0f - ageRatio); // Radio decrece
                    int offsetX = (int)(cos(spiralRotation * M_PI / 180.0f) * spiralRadius);
                    int offsetY = (int)(sin(spiralRotation * M_PI / 180.0f) * spiralRadius);
                    
                    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
                    
                    // Capa 1: Resplandor exterior rojo oscuro (más grande)
                    int outerSize = beamSize + 6;
                    SDL_SetRenderDrawColor(renderer, 180, 20, 20, alpha / 3);
                    SDL_Rect outerGlow = {screenX + offsetX - outerSize/2, screenY + offsetY - outerSize/2, 
                                          outerSize, outerSize};
                    SDL_RenderFillRect(renderer, &outerGlow);
                    
                    // Capa 2: Resplandor medio rojo brillante
                    int midSize = beamSize + 3;
                    SDL_SetRenderDrawColor(renderer, 255, 50, 50, alpha / 2);
                    SDL_Rect midGlow = {screenX + offsetX - midSize/2, screenY + offsetY - midSize/2, 
                                        midSize, midSize};
                    SDL_RenderFillRect(renderer, &midGlow);
                    
                    // Capa 3: Núcleo central rojo intenso
                    SDL_SetRenderDrawColor(renderer, 255, 80, 80, alpha);
                    SDL_Rect beamCore = {screenX + offsetX - beamSize/2, screenY + offsetY - beamSize/2, 
                                         beamSize, beamSize};
                    SDL_RenderFillRect(renderer, &beamCore);
                    
                    // Capa 4: Brillo central blanco-rosa (solo en la parte más nueva)
                    if(ageRatio > 0.6) {
                        int coreSize = std::max(2, beamSize / 2);
                        int coreAlpha = (int)(alpha * (ageRatio - 0.6f) / 0.4f); // Fade in
                        SDL_SetRenderDrawColor(renderer, 255, 200, 200, coreAlpha);
                        SDL_Rect hotCore = {screenX + offsetX - coreSize/2, screenY + offsetY - coreSize/2, 
                                           coreSize, coreSize};
                        SDL_RenderFillRect(renderer, &hotCore);
                    }
                    
                    // Partículas extras alrededor (efecto de energía)
                    if(i % 3 == 0 && ageRatio > 0.5) {
                        for(int p = 0; p < 4; p++) {
                            float particleAngle = spiralRotation + (p * 90.0f);
                            float particleRadius = beamSize * 0.8f;
                            int particleX = (int)(cos(particleAngle * M_PI / 180.0f) * particleRadius);
                            int particleY = (int)(sin(particleAngle * M_PI / 180.0f) * particleRadius);
                            
                            int particleSize = std::max(1, beamSize / 4);
                            int particleAlpha = alpha / 2;
                            SDL_SetRenderDrawColor(renderer, 255, 100, 100, particleAlpha);
                            SDL_Rect particle = {screenX + offsetX + particleX - particleSize/2, 
                                                screenY + offsetY + particleY - particleSize/2, 
                                                particleSize, particleSize};
                            SDL_RenderFillRect(renderer, &particle);
                        }
                    }
                    
                    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_NONE);
                }
            }
        }
    }
    
    // Draw explosions (explosiones animadas)
    for(const auto& explosion : explosions) {
        if(!explosion.active) continue;
        
        double relX = explosion.x - posX;
        double relY = explosion.y - posY;
        
        double invDet = 1.0 / (planeX * dirY - dirX * planeY);
        double transformX = invDet * (dirY * relX - dirX * relY);
        double transformY = invDet * (-planeY * relX + planeX * relY);
        
        if(transformY > 0.1) {
            int screenX = int((SCREEN_WIDTH / 2) * (1 + transformX / transformY));
            int screenY = SCREEN_HEIGHT / 2;
            
            if(screenX >= 0 && screenX < SCREEN_WIDTH && transformY < zBuffer[screenX]) {
                // Tamaño de explosión crece y luego decrece
                float progress = (float)explosion.frame / 30.0f;
                float sizeFactor = progress < 0.3f ? (progress / 0.3f) : (1.0f - (progress - 0.3f) / 0.7f);
                int explosionSize = std::max(10, int(sizeFactor * 150 / transformY));
                
                SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
                
                // Capas de explosión (naranja y amarillo)
                // Capa externa (roja oscura)
                int outerSize = explosionSize;
                int outerAlpha = (int)(180 * sizeFactor);
                SDL_SetRenderDrawColor(renderer, 255, 50, 0, outerAlpha);
                SDL_Rect outerExplosion = {screenX - outerSize/2, screenY - outerSize/2, outerSize, outerSize};
                SDL_RenderFillRect(renderer, &outerExplosion);
                
                // Capa media (naranja brillante)
                int midSize = explosionSize * 0.7f;
                int midAlpha = (int)(220 * sizeFactor);
                SDL_SetRenderDrawColor(renderer, 255, 150, 0, midAlpha);
                SDL_Rect midExplosion = {screenX - midSize/2, screenY - midSize/2, midSize, midSize};
                SDL_RenderFillRect(renderer, &midExplosion);
                
                // Capa interna (amarillo brillante)
                int innerSize = explosionSize * 0.4f;
                int innerAlpha = (int)(255 * sizeFactor);
                SDL_SetRenderDrawColor(renderer, 255, 255, 100, innerAlpha);
                SDL_Rect innerExplosion = {screenX - innerSize/2, screenY - innerSize/2, innerSize, innerSize};
                SDL_RenderFillRect(renderer, &innerExplosion);
                
                // Centro blanco brillante
                int coreSize = explosionSize * 0.2f;
                if(coreSize > 2) {
                    int coreAlpha = (int)(255 * (1.0f - progress) * sizeFactor);
                    SDL_SetRenderDrawColor(renderer, 255, 255, 255, coreAlpha);
                    SDL_Rect coreExplosion = {screenX - coreSize/2, screenY - coreSize/2, coreSize, coreSize};
                    SDL_RenderFillRect(renderer, &coreExplosion);
                }
                
                // Partículas de humo (4 direcciones)
                if(progress > 0.2f) {
                    for(int p = 0; p < 4; p++) {
                        float angle = p * 90.0f * M_PI / 180.0f;
                        float particleDist = explosionSize * 0.8f * (progress - 0.2f) / 0.8f;
                        int particleX = screenX + (int)(cos(angle) * particleDist);
                        int particleY = screenY + (int)(sin(angle) * particleDist);
                        int particleSize = std::max(3, explosionSize / 8);
                        int particleAlpha = (int)(150 * (1.0f - progress));
                        
                        SDL_SetRenderDrawColor(renderer, 100, 100, 100, particleAlpha);
                        SDL_Rect particle = {particleX - particleSize/2, particleY - particleSize/2, 
                                            particleSize, particleSize};
                        SDL_RenderFillRect(renderer, &particle);
                    }
                }
                
                SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_NONE);
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
            
        case STATE_MAP_EDITOR:
            // Map editor mode
            handleMapEditorInput();
            renderIsometricMap();
            drawMapEditorUI();
            SDL_RenderPresent(renderer);
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
