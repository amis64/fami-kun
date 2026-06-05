#include <stdlib.h>
#include <stdint.h>
#include "cartridge.h"
#include "bus.h"
#include "CPU.h"

Cart init_cartridge(FILE *file);
int main(int argc, char* argv[]){



  FILE *rom;
  rom = fopen(argv[1], "rb");
  if (rom == NULL)
  {
    printf("error: Couldn't open the rom\n");
    exit(1);
  }

  Cart cart = init_cartridge(rom);
  bus_init(cart.mapper);
  CPU cpu = cpu_init();

  return 0;

}



