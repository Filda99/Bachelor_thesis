/***********************************************************************************************************************
 * This file was generated by the MCUXpresso Config Tools. Any manual edits made to this file
 * will be overwritten if the respective MCUXpresso Config Tools is used to update this file.
 **********************************************************************************************************************/

#ifndef _PIN_MUX_H_
#define _PIN_MUX_H_

/*!
 * @addtogroup pin_mux
 * @{
 */

/***********************************************************************************************************************
 * API
 **********************************************************************************************************************/

#if defined(__cplusplus)
extern "C" {
#endif

/*!
 * @brief Calls initialization functions.
 *
 */
void BOARD_InitBootPins(void);

#define SOPT4_TPM1CH0SRC_TPM1_CH0 0x00u /*!<@brief TPM1 channel 0 input capture source select: TPM1_CH0 signal */
#define SOPT5_UART0RXSRC_UART_RX 0x00u  /*!<@brief UART0 receive data source select: UART0_RX pin */
#define SOPT5_UART0TXSRC_UART_TX 0x00u  /*!<@brief UART0 transmit data source select: UART0_TX pin */

/*! @name PORTA2 (number 28), J1[4]/D1/UART0_TX
  @{ */

/* Symbols to be used with PORT driver */
#define BOARD_INITPINS_DEBUG_UART_TX_PORT PORTA               /*!<@brief PORT peripheral base pointer */
#define BOARD_INITPINS_DEBUG_UART_TX_PIN 2U                   /*!<@brief PORT pin number */
#define BOARD_INITPINS_DEBUG_UART_TX_PIN_MASK (1U << 2U)      /*!<@brief PORT pin mask */
                                                              /* @} */

/*! @name PORTA1 (number 27), J1[2]/D0/UART0_RX
  @{ */

/* Symbols to be used with PORT driver */
#define BOARD_INITPINS_DEBUG_UART_RX_PORT PORTA               /*!<@brief PORT peripheral base pointer */
#define BOARD_INITPINS_DEBUG_UART_RX_PIN 1U                   /*!<@brief PORT pin number */
#define BOARD_INITPINS_DEBUG_UART_RX_PIN_MASK (1U << 1U)      /*!<@brief PORT pin mask */
                                                              /* @} */

/*! @name PORTA12 (number 32), J1[8]/D3
  @{ */

/* Symbols to be used with PORT driver */
#define BOARD_INITPINS_LeftMotor_PORT PORTA                /*!<@brief PORT peripheral base pointer */
#define BOARD_INITPINS_LeftMotor_PIN 12U                   /*!<@brief PORT pin number */
#define BOARD_INITPINS_LeftMotor_PIN_MASK (1U << 12U)      /*!<@brief PORT pin mask */
                                                           /* @} */

/*! @name PORTA13 (number 33), J2[2]/D8
  @{ */

/* Symbols to be used with PORT driver */
#define BOARD_INITPINS_RightMotor_PORT PORTA                /*!<@brief PORT peripheral base pointer */
#define BOARD_INITPINS_RightMotor_PIN 13U                   /*!<@brief PORT pin number */
#define BOARD_INITPINS_RightMotor_PIN_MASK (1U << 13U)      /*!<@brief PORT pin mask */
                                                            /* @} */

/*! @name PORTD0 (number 73), J2[6]/D10
  @{ */

/* Symbols to be used with PORT driver */
#define BOARD_INITPINS_LEFTMAINSEN_PORT PORTD               /*!<@brief PORT peripheral base pointer */
#define BOARD_INITPINS_LEFTMAINSEN_PIN 0U                   /*!<@brief PORT pin number */
#define BOARD_INITPINS_LEFTMAINSEN_PIN_MASK (1U << 0U)      /*!<@brief PORT pin mask */
                                                            /* @} */

/*! @name PORTD3 (number 76), J2[10]/D12
  @{ */

/* Symbols to be used with PORT driver */
#define BOARD_INITPINS_CENTERMAINSEN_PORT PORTD               /*!<@brief PORT peripheral base pointer */
#define BOARD_INITPINS_CENTERMAINSEN_PIN 3U                   /*!<@brief PORT pin number */
#define BOARD_INITPINS_CENTERMAINSEN_PIN_MASK (1U << 3U)      /*!<@brief PORT pin mask */
                                                              /* @} */

/*! @name PORTD4 (number 77), J1[6]/D2
  @{ */

/* Symbols to be used with PORT driver */
#define BOARD_INITPINS_RIGHTMAINSEN_PORT PORTD               /*!<@brief PORT peripheral base pointer */
#define BOARD_INITPINS_RIGHTMAINSEN_PIN 4U                   /*!<@brief PORT pin number */
#define BOARD_INITPINS_RIGHTMAINSEN_PIN_MASK (1U << 4U)      /*!<@brief PORT pin mask */
                                                             /* @} */

/*! @name PORTD2 (number 75), J2[8]/D11
  @{ */

/* Symbols to be used with GPIO driver */
#define BOARD_INITPINS_RIGHTHALL_FGPIO FGPIOD             /*!<@brief FGPIO peripheral base pointer */
#define BOARD_INITPINS_RIGHTHALL_GPIO GPIOD               /*!<@brief GPIO peripheral base pointer */
#define BOARD_INITPINS_RIGHTHALL_GPIO_PIN_MASK (1U << 2U) /*!<@brief GPIO pin mask */

/* Symbols to be used with PORT driver */
#define BOARD_INITPINS_RIGHTHALL_PORT PORTD               /*!<@brief PORT peripheral base pointer */
#define BOARD_INITPINS_RIGHTHALL_PIN 2U                   /*!<@brief PORT pin number */
#define BOARD_INITPINS_RIGHTHALL_PIN_MASK (1U << 2U)      /*!<@brief PORT pin mask */
                                                          /* @} */

/*! @name PORTD5 (number 78), J2[4]/D9
  @{ */

/* Symbols to be used with PORT driver */
#define BOARD_INITPINS_ServoMotor_PORT PORTD               /*!<@brief PORT peripheral base pointer */
#define BOARD_INITPINS_ServoMotor_PIN 5U                   /*!<@brief PORT pin number */
#define BOARD_INITPINS_ServoMotor_PIN_MASK (1U << 5U)      /*!<@brief PORT pin mask */
                                                           /* @} */

/*! @name PORTC4 (number 61), J1[7]
  @{ */

/* Symbols to be used with PORT driver */
#define BOARD_INITPINS_SS_PORT PORTC               /*!<@brief PORT peripheral base pointer */
#define BOARD_INITPINS_SS_PIN 4U                   /*!<@brief PORT pin number */
#define BOARD_INITPINS_SS_PIN_MASK (1U << 4U)      /*!<@brief PORT pin mask */
                                                   /* @} */

/*! @name PORTC5 (number 62), J1[9]
  @{ */

/* Symbols to be used with PORT driver */
#define BOARD_INITPINS_SCK_PORT PORTC               /*!<@brief PORT peripheral base pointer */
#define BOARD_INITPINS_SCK_PIN 5U                   /*!<@brief PORT pin number */
#define BOARD_INITPINS_SCK_PIN_MASK (1U << 5U)      /*!<@brief PORT pin mask */
                                                    /* @} */

/*! @name PORTC6 (number 63), J1[11]
  @{ */

/* Symbols to be used with PORT driver */
#define BOARD_INITPINS_MISO_PORT PORTC               /*!<@brief PORT peripheral base pointer */
#define BOARD_INITPINS_MISO_PIN 6U                   /*!<@brief PORT pin number */
#define BOARD_INITPINS_MISO_PIN_MASK (1U << 6U)      /*!<@brief PORT pin mask */
                                                     /* @} */

/*! @name PORTC7 (number 64), J1[1]
  @{ */

/* Symbols to be used with PORT driver */
#define BOARD_INITPINS_MOSI_PORT PORTC               /*!<@brief PORT peripheral base pointer */
#define BOARD_INITPINS_MOSI_PIN 7U                   /*!<@brief PORT pin number */
#define BOARD_INITPINS_MOSI_PIN_MASK (1U << 7U)      /*!<@brief PORT pin mask */
                                                     /* @} */

/*!
 * @brief Configures pin routing and optionally pin electrical features.
 *
 */
void BOARD_InitPins(void);

#if defined(__cplusplus)
}
#endif

/*!
 * @}
 */
#endif /* _PIN_MUX_H_ */

/***********************************************************************************************************************
 * EOF
 **********************************************************************************************************************/
