#include <stdlib.h>
#include <stdint.h>
#include "cartridge.h"
#include "bus.h"
#include "CPU.h"

Cart init_cartridge(FILE *file);
int main(int argc, char* argv[]){


  _Bool game_running;
  FILE *rom;
    FILE *log;
  int cpu_cycles = 7;
  rom = fopen(argv[1], "rb");
  if (rom == NULL)
  {
    printf("error: Couldn't open the rom\n");
    exit(1);
  }
  log = fopen("output.txt", "w");
  if (log == NULL)
  {
    printf("Could not open or create the file.\n");
    exit(1);
  }
  game_running = 1;
  Cart cart = init_cartridge(rom);
  bus_init(cart.mapper, &cart);
  CPU cpu = cpu_init();
  while(game_running){
    cpu_cycles += cpu_execute(&cpu);
  }
  return 0;

}



