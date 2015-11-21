#ifndef GLOBAL
#define GLOBAL

constexpr int TILE_SIZE = 32;
constexpr int MAP_WIDTH = 13;
constexpr int MAP_HEIGHT = 11;
constexpr int SCENE_WIDTH = MAP_WIDTH * TILE_SIZE;
constexpr int SCENE_HEIGHT = MAP_HEIGHT * TILE_SIZE;
constexpr int KEY_EVENT_MAX_LATENCY = 200;
constexpr int MS_PER_FILL_UNIT = 20;

constexpr const char* BLOWTHENALL_WINDOW_TITLE = "BlowThemAll"; // POR QUE O VINICIUS É UM VIADO
#endif // GLOBAL
