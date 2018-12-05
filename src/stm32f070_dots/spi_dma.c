#include <stdint.h>

#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/gpio.h>
#include <libopencm3/stm32/dma.h>
#include <libopencm3/stm32/usart.h>
#include <libopencm3/stm32/spi.h>
#include <libopencm3/cm3/nvic.h>
#include <libopencm3/cm3/systick.h>

#include "spi_dma.h"
#include "disp.h"
#include "hw_defs.h"

void spi_dma_init(void)
{
    // Enable DMA clock
    rcc_periph_clock_enable(RCC_DMA1);
    // In order to use SPI1_TX, we need DMA 1 Channel 3
    dma_channel_reset(DMA1, DMA_CHANNEL3);
    // SPI1 data register as output
    dma_set_peripheral_address(DMA1, DMA_CHANNEL3, (uint32_t)&SPI1_DR);
    // We will be using system memory as the source data
    dma_set_read_from_memory(DMA1, DMA_CHANNEL3);
    // Memory increment mode needs to be turned on, so that if we're sending
    // multiple bytes the DMA controller actually sends a series of bytes,
    // instead of the same byte multiple times.
    dma_enable_memory_increment_mode(DMA1, DMA_CHANNEL3);
    // Contrarily, the peripheral does not need to be incremented - the SPI
    // data register doesn't move around as we write to it.
    dma_disable_peripheral_increment_mode(DMA1, DMA_CHANNEL3);
    // We want to use 16 bit transfers
    dma_set_peripheral_size(DMA1, DMA_CHANNEL3, DMA_CCR_PSIZE_16BIT);
    dma_set_memory_size(DMA1, DMA_CHANNEL3, DMA_CCR_MSIZE_16BIT);
    // We don't have any other DMA transfers going, but if we did we can use
    // priorities to try to ensure time-critical transfers are not interrupted
    // by others. In this case, it is alone.
    dma_set_priority(DMA1, DMA_CHANNEL3, DMA_CCR_PL_LOW);
    dma_enable_circular_mode(DMA1, DMA_CHANNEL3);
}

void spi_dma_start(void *data, int data_size)
{
    // Note - manipulating the memory address/size of the DMA controller cannot
    // be done while the channel is enabled. Ensure any previous transfer has
    // completed and the channel is disabled before you start another transfer.
    // Tell the DMA controller to start reading memory data from this address
    dma_set_memory_address(DMA1, DMA_CHANNEL3, (uint32_t)data);
    // Configure the number of bytes to transfer
    dma_set_number_of_data(DMA1, DMA_CHANNEL3, data_size);
    // Enable the DMA channel.
    dma_enable_channel(DMA1, DMA_CHANNEL3);

    // Finally, enable SPI DMA transmit. This call is what actually starts the
    // DMA transfer.
    spi_enable_tx_dma(DISP_COLS_SPI);
}
