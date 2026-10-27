#pragma once
#include <stdbool.h>
#include <stdint.h>

enum { ZW_ROOM_COUNT=128, ZW_TILE_COUNT=704, ZW_ATTR_COUNT=48 };
typedef struct {
    uint8_t tiles[ZW_TILE_COUNT], attrs[ZW_ATTR_COUNT];
    uint8_t palette[32], ram[2048];
    uint64_t ticks;
    bool valid, owned, simulation_ready;
} ZeldaWorldRoom;
extern ZeldaWorldRoom zelda_world_rooms[ZW_ROOM_COUNT];
bool zelda_world_decode_room(unsigned room);
void zelda_world_refresh(void);
bool zelda_world_live(void);
bool zelda_world_supported(void);
void zelda_world_rooms_reset(void);
void zelda_world_rooms_step(void);
void zelda_world_rooms_loaded(void);
void zelda_world_rooms_tcp_setup(void);
void zelda_world_sync_doors(unsigned room,unsigned entering);
int zelda_world_neighbor(unsigned room,unsigned direction);
bool zelda_world_portal(unsigned room);
void zelda_world_enable_hooks(bool enabled);
uint64_t zelda_world_handoffs(void);
uint64_t zelda_world_sim_failures(void);
