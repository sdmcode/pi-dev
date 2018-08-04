#include <kernel/gpu.h>
#include <kernel/framebuffer.h>
#include <kernel/mem.h>
#include <kernel/mailbox.h>
#include <kernel/kernelio.h>
#include <kernel/chars_pixels.h>

#include <common/stdlib.h>

void write_pixel(uint32_t x, uint32_t y, const pixel_t * pixel)
{
  uint8_t * location = fbinfo.buf + y * fbinfo.pitch + x * BYTES_PER_PIXEL;
  memcpy(location, pixel, BYTES_PER_PIXEL);
}

void gpu_putc(char c)
{
  static const pixel_t WHITE = {0xff, 0xff, 0xff};
  static const pixel_t BLACK = {0x00, 0x00, 0x00};

  uint8_t w, h;
  uint8_t mask;

  const uint8_t * bmp = font(c);

  uint32_t i, num_rows = fbinfo.height/CHAR_HEIGHT;

  if (fbinfo.chars_y >= num_rows) {
    for (i = 0; i < num_rows - 1; i++)
    {
      memcpy(fbinfo.buf + fbinfo.pitch * i * CHAR_HEIGHT, fbinfo.buf + fbinfo.pitch * (i + 1) * CHAR_HEIGHT, fbinfo.pitch * CHAR_HEIGHT);
    }

    bzero(fbinfo.buf + fbinfo.pitch * i * CHAR_HEIGHT, fbinfo.pitch * CHAR_HEIGHT);
    fbinfo.chars_y--;
  }

  if (c == '\n') {
    fbinfo.chars_x = 0;
    fbinfo.chars_y++;
    return;
  }

  if (c == 8 || c == 127) {
    if (fbinfo.chars_x > 0) {
      fbinfo.chars_x--;

      for (w = 0; w < CHAR_WIDTH; w++)
      {
        for (h = 0; h < CHAR_HEIGHT; h++)
        {
            write_pixel(fbinfo.chars_x * CHAR_WIDTH + w, fbinfo.chars_y * CHAR_HEIGHT + h, &BLACK);
        }
      }
    }
    return;
  }

  for (w = 0; w < CHAR_WIDTH; w++)
  {
    for (h = 0; h < CHAR_HEIGHT; h++)
    {
      mask = 1 << (w);

      if (bmp[h] & mask) {
        write_pixel(fbinfo.chars_x * CHAR_WIDTH + w, fbinfo.chars_y * CHAR_HEIGHT + h, &WHITE);
      } else {
        write_pixel(fbinfo.chars_x * CHAR_WIDTH + w, fbinfo.chars_y * CHAR_HEIGHT + h, &BLACK);
      }
    }
  }

  fbinfo.chars_x++;

  if (fbinfo.chars_x > fbinfo.chars_width) {
    fbinfo.chars_x = 0;
    fbinfo.chars_y++;
  }
}

void gpu_init(void)
{
  static const pixel_t BLK = {0x00, 0x00, 0x00};

  while (framebuffer_init());

  for (uint32_t j = 0; j < fbinfo.height; j++)
  {
    for (uint32_t i = 0; i < fbinfo.width; i++)
    {
      write_pixel(i, j, &BLK);
    }
  }
  puts("GPU Initialised..\n");
}
