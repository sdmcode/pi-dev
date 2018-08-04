#include <stddef.h>
#include <stdint.h>

#include <kernel/uart.h>
#include <common/stdlib.h>

void mmio_write(uint32_t reg, uint32_t data)
{
  *(volatile uint32_t*)reg = data;
}

uint32_t mmio_read(uint32_t reg)
{
  return *(volatile uint32_t*)reg;
}

void delay(uint32_t count)
{
  asm volatile("__delay_%=: subs %[count], %[count], #1; bne __delay_%=\n"
          : "=r"(count): [count]"0"(count) : "cc");
}

void uart_init()
{
  uart_control_t control;

  bzero(&control, 4);
  mmio_write(UART0_CR, control.as_int);

  mmio_write(GPPUD, 0x00000000);
  delay(150);

  mmio_write(GPPUDCLK0, (1 << 14) | (1 << 15));
  delay(150);

  mmio_write(GPPUDCLK0, 0x00000000);

  mmio_write(UART0_ICR, 0x7FF);

  mmio_write(UART0_IBRD, 1);
  mmio_write(UART0_FBRD, 40);

  mmio_write(UART0_LCRH, (1 << 4) | (1 << 5) | (1 << 6));

  mmio_write(UART0_IMSC, (1 << 1) | (1 << 4) | (1 << 5) | (1 << 6) | (1 << 7) | (1 << 8) | (1 << 9) | (1 << 10));

  control.uart_enabled = 1;
  control.transmit_enabled = 1;
  control.recieve_enabled = 1;

  mmio_write(UART0_CR, control.as_int);
}

uart_flags_t read_flags(void)
{
  uart_flags_t flags;
  flags.as_int = mmio_read(UART0_FR);

  return flags;
}

void uart_putc(unsigned char c)
{
  uart_flags_t flags;

  do {
    flags = read_flags();
  } while (flags.transmit_queue_full);

  mmio_write(UART0_DR, c);
}

unsigned char uart_getc()
{
  uart_flags_t flags;

  do {
    flags = read_flags();
  } while (flags.recieve_queue_empty);

  return mmio_read(UART0_DR);
}
