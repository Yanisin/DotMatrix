#ifndef HW_DEFS_60X60_REV_0_5_H
#define HW_DEFS_60X60_REV_0_5_H


#ifndef HW_60x60_rev_0_5
#error The matching HW selection macro not defined
#endif

/**************************************
 *	Debug LED Definitions
 **************************************/
#define LED_RCC RCC_GPIOC
#define LED_PORT GPIOC
#define LED_PIN GPIO13

/**************************************
 *	Buttons Definitions
 **************************************/
#define BTN0_RCC RCC_GPIOA
#define BTN0_PORT GPIOA
#define BTN0_PIN GPIO6
#define BTN0_EXTI EXTI6
#define BTN_IRQ NVIC_EXTI4_15_IRQ

#define BTN1_RCC RCC_GPIOA
#define BTN1_PORT GPIOA
#define BTN1_PIN GPIO7
#define BTN1_EXTI EXTI7

/**************************************
 *	I2C Definitions
 **************************************/

#define I2C1_GPIO_PORT GPIOB
#define I2C1_GPIO_RCC RCC_GPIOB
#define I2C1_GPIO_SCL GPIO9
#define I2C1_GPIO_SDA GPIO8
#define I2C1_GPIO_AF GPIO_AF1

#define I2C2_GPIO_PORT GPIOB
#define I2C2_GPIO_RCC RCC_I2C2
#define I2C2_GPIO_SCL GPIO13
#define I2C2_GPIO_SDA GPIO14
#define I2C2_GPIO_AF GPIO_AF5

/**************************************
 *	Display Definitions
 **************************************/

#define DISP_ROWS_NUM 8
#define DISP_COLS_NUM 8

#define DISP_GPIOS_RCC (RCC_GPIOA | RCC_GPIOB)

/*rows anodes*/
/*cols cathodes*/
#define DISP_nOE_ROW_GPIO GPIOB
#define DISP_nOE_COL_GPIO GPIOB
#define DISP_nOE_ROW_GPIO_PIN GPIO2
#define DISP_nOE_COL_GPIO_PIN GPIO4

#define DISP_COLS_SPI SPI1
#define DISP_COLS_GPIO_RCC (RCC_GPIOB | RCC_GPIOA)
#define DISP_COLS_SPI_RCC RCC_SPI1

#define DISP_COLS_CLK_GPIO GPIOB
#define DISP_COLS_CLK_GPIO_PIN GPIO3
#define DISP_COLS_CLK_AF_NUM GPIO_AF0

#define DISP_COLS_DATA_GPIO GPIOB
#define DISP_COLS_DATA_GPIO_PIN GPIO5
#define DISP_COLS_DATA_AF_NUM GPIO_AF0

#define DISP_COLS_STROBE_GPIO GPIOA
#define DISP_COLS_STROBE_GPIO_PIN GPIO15
#define DISP_COLS_STROBE_AF_NUM GPIO_AF0


/**************************************
 *	USARTs Definitions
 **************************************/

#define USART_BAUDRATE 115200

#define USART_A 0
#define USART_A_REG USART1
#define USART_A_RCC RCC_USART1
#define USART_A_IRQ NVIC_USART1_IRQ
#define USART_A_RX_RCC RCC_GPIOA
#define USART_A_RX_PORT GPIOA
#define USART_A_RX_PIN GPIO10
#define USART_A_RX_AF_NUM GPIO_AF1
#define USART_A_TX_RCC RCC_GPIOA
#define USART_A_TX_PORT GPIOA
#define USART_A_TX_PIN GPIO9
#define USART_A_TX_AF_NUM GPIO_AF1

#define USART_B 1
#define USART_B_REG USART2
#define USART_B_RCC RCC_USART2
#define USART_B_IRQ NVIC_USART2_IRQ
#define USART_B_RX_RCC RCC_GPIOA
#define USART_B_RX_PORT GPIOA
#define USART_B_RX_PIN GPIO3
#define USART_B_RX_AF_NUM GPIO_AF1
#define USART_B_TX_RCC RCC_GPIOA
#define USART_B_TX_PORT GPIOA
#define USART_B_TX_PIN GPIO2
#define USART_B_TX_AF_NUM GPIO_AF1

#define USART_C 2
#define USART_C_REG USART3
#define USART_C_RCC RCC_USART3
#define USART_C_IRQ NVIC_USART3_4_IRQ
#define USART_C_RX_RCC RCC_GPIOB
#define USART_C_RX_PORT GPIOB
#define USART_C_RX_PIN GPIO11
#define USART_C_RX_AF_NUM GPIO_AF4
#define USART_C_TX_RCC RCC_GPIOB
#define USART_C_TX_PORT GPIOB
#define USART_C_TX_PIN GPIO10
#define USART_C_TX_AF_NUM GPIO_AF4

#define USART_D 3
#define USART_D_REG USART4
#define USART_D_RCC RCC_USART4
#define USART_D_IRQ NVIC_USART3_4_IRQ
#define USART_D_RX_RCC RCC_GPIOA
#define USART_D_RX_PORT GPIOA
#define USART_D_RX_PIN GPIO1
#define USART_D_RX_AF_NUM GPIO_AF4
#define USART_D_TX_RCC RCC_GPIOA
#define USART_D_TX_PORT GPIOA
#define USART_D_TX_PIN GPIO0
#define USART_D_TX_AF_NUM GPIO_AF4

#define USART_DIR_UP USART_D
#define USART_DIR_DOWN USART_B
#define USART_DIR_LEFT USART_C
#define USART_DIR_RIGHT USART_A

#endif
