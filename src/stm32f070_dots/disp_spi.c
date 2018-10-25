#include "stdint.h"
#include "disp_spi.h"
#ifdef STDPERIPH
#include "stm32f0xx.h"
#else

#include <libopencm3/stm32/gpio.h>
#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/spi.h>
#endif

#ifdef STDPERIPH
#define SPIx                             SPI1
#define SPIx_CLK                         RCC_APB2Periph_SPI1
#define SPIx_IRQn                        SPI1_IRQn
#define SPIx_IRQHandler                  SPI1_IRQHandler

#define SPIx_SCK_PIN                     GPIO_Pin_3
#define SPIx_SCK_GPIO_PORT               GPIOB
#define SPIx_SCK_GPIO_CLK                RCC_AHBPeriph_GPIOB
#define SPIx_SCK_SOURCE                  GPIO_PinSource3
#define SPIx_SCK_AF                      GPIO_AF_0

#define SPIx_MISO_PIN                    GPIO_Pin_4
#define SPIx_MISO_GPIO_PORT              GPIOB
#define SPIx_MISO_GPIO_CLK               RCC_AHBPeriph_GPIOB
#define SPIx_MISO_SOURCE                 GPIO_PinSource4
#define SPIx_MISO_AF                     GPIO_AF_0

#define SPIx_MOSI_PIN                    GPIO_Pin_5
#define SPIx_MOSI_GPIO_PORT              GPIOB
#define SPIx_MOSI_GPIO_CLK               RCC_AHBPeriph_GPIOB
#define SPIx_MOSI_SOURCE                 GPIO_PinSource5
#define SPIx_MOSI_AF                     GPIO_AF_0

#define RXBUFFERSIZE                     TXBUFFERSIZE

#else
#endif


#ifdef STDPERIPH
static void my_spi_init(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;
  NVIC_InitTypeDef NVIC_InitStructure;
SPI_InitTypeDef  SPI_InitStructure;
  
  /* Enable the SPI periph */
  RCC_APB2PeriphClockCmd(SPIx_CLK, ENABLE);
  
  /* Enable SCK, MOSI, MISO and NSS GPIO clocks */
  RCC_AHBPeriphClockCmd(SPIx_SCK_GPIO_CLK | SPIx_MISO_GPIO_CLK | SPIx_MOSI_GPIO_CLK, ENABLE);
  
  GPIO_PinAFConfig(SPIx_SCK_GPIO_PORT, SPIx_SCK_SOURCE, SPIx_SCK_AF);
  GPIO_PinAFConfig(SPIx_MOSI_GPIO_PORT, SPIx_MOSI_SOURCE, SPIx_MOSI_AF);
  
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_DOWN;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_Level_3;

  /* SPI SCK pin configuration */
  GPIO_InitStructure.GPIO_Pin = SPIx_SCK_PIN;
  GPIO_Init(SPIx_SCK_GPIO_PORT, &GPIO_InitStructure);

  /* SPI  MOSI pin configuration */
  GPIO_InitStructure.GPIO_Pin =  SPIx_MOSI_PIN;
  GPIO_Init(SPIx_MOSI_GPIO_PORT, &GPIO_InitStructure);
  
  /* SPI configuration -------------------------------------------------------*/
  SPI_I2S_DeInit(SPIx);
  SPI_InitStructure.SPI_Direction = SPI_Direction_1Line_Tx;
  SPI_InitStructure.SPI_DataSize = 8;
  SPI_InitStructure.SPI_CPOL = SPI_CPOL_Low;
  SPI_InitStructure.SPI_CPHA = SPI_CPHA_1Edge;
  SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;
  SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_4;
  SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;
  SPI_InitStructure.SPI_CRCPolynomial = 7;
   
  /* Configure the SPI interrupt priority */
//  NVIC_InitStructure.NVIC_IRQChannel = SPIx_IRQn;
//  NVIC_InitStructure.NVIC_IRQChannelPriority = 1;
//  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
//  NVIC_Init(&NVIC_InitStructure);

  SPI_InitStructure.SPI_Mode = SPI_Mode_Master;
  SPI_Init(SPIx, &SPI_InitStructure);

  /* Enable the SPI peripheral */
  SPI_Cmd(SPIx, ENABLE);
}
#else

static void my_spi_init(void)
{

	rcc_periph_clock_enable(RCC_SPI1);

	gpio_mode_setup(GPIOB, GPIO_MODE_AF, GPIO_PUPD_NONE,
			GPIO3 | GPIO4 |  GPIO5);
	gpio_set_af(GPIOB, GPIO_AF0, GPIO3 |GPIO4 | GPIO5);

	/* Setup SPI parameters. */
//	spi_init_master(SPI1, SPI_CR1_BAUDRATE_FPCLK_DIV_4, SPI_CR1_CPOL,
			//SPI_CR1_CPHA, SPI_CR1_MSBFIRST);
	/* FIXME read from datasheed if rising all falling edge ofclock is better
	 */
	spi_init_master(SPI1, SPI_CR1_BAUDRATE_FPCLK_DIV_4, 0,
			SPI_CR1_CPHA, SPI_CR1_MSBFIRST);
	spi_enable_ss_output(SPI1); /* Required, see NSS, 25.3.1 section. */

	/* Finally enable the SPI. */
	spi_enable(SPI1);

}


#endif


#ifdef STDPERIPH
void stcp_init(void) {
  GPIO_InitTypeDef GPIO_InitStructure;
    /* GPIOD Periph clock enable */
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOB, ENABLE);
    //RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA, ENABLE);

    /* Configure PA11  in output pushpull mode */
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;
    //  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;

    //GPIO_Init(GPIOA, &GPIO_InitStructure);
    GPIO_Init(GPIOB, &GPIO_InitStructure);

	/* and set off */

    GPIOB->BRR  = 0x1<<6;
}

static void show(void) {
volatile uint32_t   dly;
/*on */
    GPIOB->BSRR  = 0x1<<6;
//	for(dly = 0; dly < 500000; dly++);
/* and off */
    GPIOB->BRR  = 0x1<<6;
//	for(dly = 0; dly < 500000; dly++);

}
void bitbang_init() {
  GPIO_InitTypeDef GPIO_InitStructure;
    /* GPIOD Periph clock enable */
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOB, ENABLE);
    //RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA, ENABLE);

    /* Configure PA11  in output pushpull mode */
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5|GPIO_Pin_3;
    //  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;

    //GPIO_Init(GPIOA, &GPIO_InitStructure);
    GPIO_Init(GPIOB, &GPIO_InitStructure);

	
	/* and set off */

    GPIOB->BRR  = 0x1<<5;
    GPIOB->BRR  = 0x1<<3;
}

void bitbang_send(uint8_t data) {
volatile uint32_t   dly;
	unsigned int i;

	for (i=0; i<8;i++) {
		if (data & 0x1) {
			GPIOB->BSRR  = 0x1<<5;
		} else {
			GPIOB->BRR  = 0x1<<5;
		}
		data = data >> 1;

/*on */
    GPIOB->BSRR  = 0x1<<3;
/* and off */
    GPIOB->BRR  = 0x1<<3;
	}
}

void spi_send(uint8_t data);
void spi_send(uint8_t data)
{

//  SPI_I2S_ITConfig(SPIx, SPI_I2S_IT_ERR, ENABLE);


      /* Enable the Tx buffer empty interrupt */
    	SPI_SendData8(SPIx, data);
//      SPI_I2S_ITConfig(SPIx, SPI_I2S_IT_TXE, ENABLE);

//spi_send(0xCA);
 
    /* Waiting until TX FIFO is empty */
    //while (SPI_GetTransmissionFIFOStatus(SPIx) != SPI_TransmissionFIFOStatus_Empty);
//    while (SPI_GetTransmissionFIFOStatus(SPIx) != 
//SPI_TransmissionFIFOStatus_Full   	  );
    
#if 0
    /* Wait busy flag */
    while(SPI_I2S_GetFlagStatus(SPIx, SPI_I2S_FLAG_BSY) == SET)
    {}

    /* Disable the Tx buffer empty interrupt */
    SPI_I2S_ITConfig(SPIx, SPI_I2S_IT_TXE, DISABLE);
#endif
}

#else
static void show(void);
static void stcp_init(void)
{
	rcc_periph_clock_enable(RCC_GPIOA);
	rcc_periph_clock_enable(RCC_GPIOB);
	gpio_mode_setup(GPIOA, 
		GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, GPIO15);
	
			gpio_clear(GPIOA, GPIO15);
			show();
}

static void show(void) 
{
			gpio_set(GPIOA, GPIO15);
//	for(dly = 0; dly < 500000; dly++);
		__asm__("nop");
		__asm__("nop");
			gpio_clear(GPIOA, GPIO15);

}

#endif


void disp_spi_col_set(uint8_t data)
{
//bitbang_send(data);
//my_spi_send(data);

spi_send(SPI1, data);
/* wait for sending */
while (SPI_SR(SPI1) & SPI_SR_BSY && !(SPI_SR(SPI1) & SPI_SR_TXE));

/* FIXME make this with interrupt? */
show();
}

void disp_spi_init(void)
{

my_spi_init();
//bitbang_init();
stcp_init();
#if 0
#endif 
//bitbang_send(0xCA);
//show();
//	for(dly = 0; dly < 10000000; dly++);
//bitbang_send(0xAC);
//show();
}
