#include <stdint.h>
#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/gpio.h>
#include <libopencm3/stm32/usart.h>


int main(void) {
	rcc_clock_setup_in_hsi_out_48mhz();
	while(1)
        ;
}
