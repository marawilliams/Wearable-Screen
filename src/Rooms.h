#pragma once

struct Room {
    char code;
    const char* name;
};

constexpr Room ROOMS[] = {
    {'k', "kitchen"},
    {'b', "bedroom"},
    {'l', "living room"},
    {'g', "garage"}
};

constexpr size_t ROOM_COUNT = sizeof(ROOMS) / sizeof(ROOMS[0]);

inline const Room* findRoom(char code) {
  for (size_t i = 0; i < ROOM_COUNT; i++) {
    if (ROOMS[i].code == code) return &ROOMS[i];
  }
  return nullptr;
}