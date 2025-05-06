#include <SDL3/SDL.h>
#include <SDL3/SDL_keyboard.h>
#include <SDL3/SDL_mouse.h>
#include <SDL3/SDL_video.h>
#include <array>
#include <bit>
#include <csignal>
#include <cstddef>
#include <cstdint>
#include <cstdio>
#include <dlfcn.h>
#include <link.h>
#include <sys/mman.h>
#include <unistd.h>

#include "MemoryManager/LinuxMemoryManager.hpp"

SDL_Window *window;
void *set_rela_mode = nullptr;

void *rela_mode_updater(void *thisptr, void *mouse, bool enabled) {
  printf("[wayland-fix] updating relative mode: %d\n", enabled);
  SDL_SetWindowRelativeMouseMode(window, enabled);
  return reinterpret_cast<decltype(rela_mode_updater) *>(set_rela_mode)(
      thisptr, mouse, enabled);
}

void __attribute__((constructor)) ctor() {
  printf("[wayland-fix] loading\n");
  window = SDL_GetWindowFromID(5); // bypass
  printf("[wayland-fix] window ptr: %p\n", window);

  bool worked = SDL_StartTextInput(window);
  printf("[wayland-fix] fixed text input: %s\n", worked ? "true" : "false");

  MemoryManager::LinuxMemoryManager<false, true, true> mem_mgr{};

  void* handle = dlmopen(LM_ID_BASE, "libinputsystem.so", RTLD_NOW);
  printf("[wayland-fix] libinputsystem handle: %p\n", handle);
  dlclose(handle);

  void *createInterface = dlsym(handle, "CreateInterface");
  printf("[wayland-fix] input system create interface: %p\n", createInterface);

  int lotto;
  void *inputstacksystem = ((void *(*)(const char *, int *))createInterface)(
      "InputStackSystemVersion001", &lotto);

  printf("[wayland-fix] input stack system: %p\n", inputstacksystem);

  void **table = *(void ***)inputstacksystem;

  void **orig = &table[(0x0013b068 - 0x0013afb0) / sizeof(void *)];
  printf("[wayland-fix] found rela mode updater at %p\n", orig);

  set_rela_mode = *orig;

  auto arr = std::bit_cast<const std::array<std::byte, sizeof(void *)>>(
      &rela_mode_updater);
  mem_mgr.write(uintptr_t(orig), arr.data(), sizeof(void *));

  printf("[wayland-fix] rela mode hooked\n");
}
