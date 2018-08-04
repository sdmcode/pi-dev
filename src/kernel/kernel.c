#include <stddef.h>
#include <stdint.h>

#include <kernel/uart.h>
#include <kernel/mem.h>
#include <kernel/atag.h>
#include <kernel/gpu.h>

#include <kernel/kernelio.h>

#include <common/stdlib.h>

void kernel_main(uint32_t r0, uint32_t r1, uint32_t atags)
{
  char buf[256];

  (void) r0;
  (void) r1;
  (void) atags;

  std_puts("Initialising memory...\n");
  mem_init((atag_t *)atags);

  std_puts("Initialising GPU...\n");
  gpu_init();

  puts("\nHello console...\n");

  while (1)
  {
    bzero(buf, 256);
    gets(buf, 256);
  }
}
