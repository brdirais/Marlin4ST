/**
  ******************************************************************************
  * @file    STM32F4xx-3dPrinter/Demonstrations/Src/stm32f4xx_hal_msp.c
  * @author  IPC Rennes
  * @version V1.0.0
  * @date    January 29, 2015
  * @brief   HAL MSP module.    
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT(c) 2014 STMicroelectronics</center></h2>
  *
  * Redistribution and use in source and binary forms, with or without modification,
  * are permitted provided that the following conditions are met:
  *   1. Redistributions of source code must retain the above copyright notice,
  *      this list of conditions and the following disclaimer.
  *   2. Redistributions in binary form must reproduce the above copyright notice,
  *      this list of conditions and the following disclaimer in the documentation
  *      and/or other materials provided with the distribution.
  *   3. Neither the name of STMicroelectronics nor the names of its contributors
  *      may be used to endorse or promote products derived from this software
  *      without specific prior written permission.
  *
  * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
  * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
  * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
  * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
  * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
  * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
  * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
  * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
  * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
  * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
  *
  ******************************************************************************  
  */ 

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "Marlin_export.h"

/** @defgroup MSP_module
  * @brief HAL MSP module.
  * @{
  */

/* Imported variables ---------------------------------------------------------*/
extern TIM_HandleTypeDef hTimPwmX;
extern TIM_HandleTypeDef hTimPwmY;
extern TIM_HandleTypeDef hTimPwmZ;
extern TIM_HandleTypeDef hTimPwmE1;
extern TIM_HandleTypeDef hTimPwmE2;
extern TIM_HandleTypeDef hTimPwmE3;

extern TIM_HandleTypeDef hTimPwmHeatBed;
extern TIM_HandleTypeDef hTimPwmHeatBed2;
extern TIM_HandleTypeDef hTimPwmHeatBed3;
extern TIM_HandleTypeDef hTimPwmHeatE1;
extern TIM_HandleTypeDef hTimPwmHeatE2;
extern TIM_HandleTypeDef hTimPwmHeatE3;

extern BspAdcDataType gBspAdcData;
extern BspWifiDataType gBspWifiData;

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
    
/* Private function prototypes -----------------------------------------------*/
extern void BSP_MotorControl_StepClockHandler(uint8_t deviceId); 
extern void BSP_MotorControl_FlagInterruptHandler(void);
/* Private functions ---------------------------------------------------------*/

/** @defgroup HAL_MSP_Private_Functions
  * @{
  */

/**
  * @brief SPI MSP Initialization 
  *        This function configures the hardware resources used in this example: 
  *           - Peripheral's clock enable
  *           - Peripheral's GPIO Configuration  
  * @param[in] hspi SPI handle pointer
  * @retval None
  */
void HAL_SPI_MspInit(SPI_HandleTypeDef *hspi)
{
  GPIO_InitTypeDef  GPIO_InitStruct;
  
  if(hspi->Instance == SPIx)
  {  
    /*##-1- Enable peripherals and GPIO Clocks #################################*/
    /* Enable GPIO TX/RX clock */
    SPIx_SCK_GPIO_CLK_ENABLE();
    SPIx_MISO_GPIO_CLK_ENABLE();
    SPIx_MOSI_GPIO_CLK_ENABLE();
    /* Enable SPI clock */
    SPIx_CLK_ENABLE(); 
    
    /*##-2- Configure peripheral GPIO ##########################################*/  
    /* SPI SCK GPIO pin configuration  */
    GPIO_InitStruct.Pin       = SPIx_SCK_PIN;
    GPIO_InitStruct.Mode      = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull      = GPIO_NOPULL;
    GPIO_InitStruct.Speed     = GPIO_SPEED_MEDIUM;
    GPIO_InitStruct.Alternate = SPIx_SCK_AF;
    
    HAL_GPIO_Init(SPIx_SCK_GPIO_PORT, &GPIO_InitStruct);
      
    /* SPI MISO GPIO pin configuration  */
    GPIO_InitStruct.Pin = SPIx_MISO_PIN;
    GPIO_InitStruct.Alternate = SPIx_MISO_AF;
    
    HAL_GPIO_Init(SPIx_MISO_GPIO_PORT, &GPIO_InitStruct);
    
    /* SPI MOSI GPIO pin configuration  */
    GPIO_InitStruct.Pin = SPIx_MOSI_PIN;
    GPIO_InitStruct.Alternate = SPIx_MOSI_AF;
      
    HAL_GPIO_Init(SPIx_MOSI_GPIO_PORT, &GPIO_InitStruct);   
  }
  else if (hspi->Instance == SPI_USER)
  {
    /*##-1- Enable peripherals and GPIO Clocks #################################*/
    /* Enable GPIO TX/RX clock */
    SPI_USER_SCK_GPIO_CLK_ENABLE();
    SPI_USER_MISO_GPIO_CLK_ENABLE();
    SPI_USER_MOSI_GPIO_CLK_ENABLE();
    /* Enable SPI clock */
    SPI_USER_CLK_ENABLE(); 
    
    /*##-2- Configure peripheral GPIO ##########################################*/  
    /* SPI SCK GPIO pin configuration  */
    GPIO_InitStruct.Pin       = SPI_USER_SCK_PIN;
    GPIO_InitStruct.Mode      = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull      = GPIO_NOPULL;
    GPIO_InitStruct.Speed     = GPIO_SPEED_MEDIUM;
    GPIO_InitStruct.Alternate = SPI_USER_SCK_AF;
    
    HAL_GPIO_Init(SPI_USER_SCK_GPIO_PORT, &GPIO_InitStruct);
      
    /* SPI MISO GPIO pin configuration  */
    GPIO_InitStruct.Pin = SPI_USER_MISO_PIN;
    GPIO_InitStruct.Alternate = SPI_USER_MISO_AF;
    
    HAL_GPIO_Init(SPI_USER_MISO_GPIO_PORT, &GPIO_InitStruct);
    
    /* SPI MOSI GPIO pin configuration  */
    GPIO_InitStruct.Pin = SPI_USER_MOSI_PIN;
    GPIO_InitStruct.Alternate = SPI_USER_MOSI_AF;
      
    HAL_GPIO_Init(SPI_USER_MOSI_GPIO_PORT, &GPIO_InitStruct);      
  }
}

/**
  * @brief SPI MSP De-Initialization 
  *        This function frees the hardware resources used in this example:
  *          - Disable the Peripheral's clock
  *          - Revert GPIO configuration to its default state
  * @param[in] hspi SPI handle pointer
  * @retval None
  */
void HAL_SPI_MspDeInit(SPI_HandleTypeDef *hspi)
{
  if(hspi->Instance == SPIx)
  {  
    /*##-1- Reset peripherals ##################################################*/
    SPIx_FORCE_RESET();
    SPIx_RELEASE_RESET();

    /*##-2- Disable peripherals and GPIO Clocks ################################*/
    /* Configure SPI SCK as alternate function  */
    HAL_GPIO_DeInit(SPIx_SCK_GPIO_PORT, SPIx_SCK_PIN);
    /* Configure SPI MISO as alternate function  */
    HAL_GPIO_DeInit(SPIx_MISO_GPIO_PORT, SPIx_MISO_PIN);
    /* Configure SPI MOSI as alternate function  */
    HAL_GPIO_DeInit(SPIx_MOSI_GPIO_PORT, SPIx_MOSI_PIN);
  }
  else if (hspi->Instance == SPI_USER)
  {
    /*##-1- Reset peripherals ##################################################*/
    SPI_USER_FORCE_RESET();
    SPI_USER_RELEASE_RESET();

    /*##-2- Disable peripherals and GPIO Clocks ################################*/
    /* Configure SPI SCK as alternate function  */
    HAL_GPIO_DeInit(SPI_USER_SCK_GPIO_PORT, SPI_USER_SCK_PIN);
    /* Configure SPI MISO as alternate function  */
    HAL_GPIO_DeInit(SPI_USER_MISO_GPIO_PORT, SPI_USER_MISO_PIN);
    /* Configure SPI MOSI as alternate function  */
    HAL_GPIO_DeInit(SPI_USER_MOSI_GPIO_PORT, SPI_USER_MOSI_PIN);    
  }
}

/**
  * @brief UArt MSP Initialization 
  *        This function configures the hardware resources used in this example: 
  *           - Peripheral's clock enable
  *           - Peripheral's GPIO Configuration  
  * @param[in] huart UART handle pointer
  * @retval None
  */
void HAL_UART_MspInit(UART_HandleTypeDef* huart)
{
  GPIO_InitTypeDef GPIO_InitStruct;
  if(huart->Instance == BSP_UART_DEBUG)
  {
    /* Peripheral clock enable */
    __BSP_UART_DEBUG_CLK_ENABLE();
  
    /* Enable GPIO TX/RX clock */
    __BSP_UART_DEBUG_TX_GPIO_CLK_ENABLE();
    __BSP_UART_DEBUG_RX_GPIO_CLK_ENABLE();
    
    /* USART TX GPIO Configuration */
    GPIO_InitStruct.Pin = BSP_UART_DEBUG_TX_PIN;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_LOW;
    GPIO_InitStruct.Alternate = BSP_UART_DEBUG_TX_AF;
    HAL_GPIO_Init(BSP_UART_DEBUG_TX_PORT, &GPIO_InitStruct);

    /* USART RX GPIO Configuration     */    
    GPIO_InitStruct.Pin = BSP_UART_DEBUG_RX_PIN;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_LOW;
    GPIO_InitStruct.Alternate = BSP_UART_DEBUG_RX_AF;
    HAL_GPIO_Init(BSP_UART_DEBUG_RX_PORT, &GPIO_InitStruct);
    
    /* Configure the NVIC for UART */
    HAL_NVIC_SetPriority(BSP_UART_DEBUG_IRQn, 3, 0);
    HAL_NVIC_EnableIRQ(BSP_UART_DEBUG_IRQn);    
  }
  else  if(huart->Instance == BSP_WIFI_UART)
  {
    DMA_HandleTypeDef *pRxDmaHandle = &(gBspWifiData.rxDmaHandle);
    DMA_HandleTypeDef *pTxDmaHandle = &(gBspWifiData.txDmaHandle);
    
    /* Peripheral clock enable */
    __BSP_WIFI_UART_CLK_ENABLE();
  
    /* Enable GPIO TX/RX clock */
    __BSP_WIFI_UART_CTS_GPIO_CLK_ENABLE();
    __BSP_WIFI_UART_RTS_GPIO_CLK_ENABLE();
    __BSP_WIFI_UART_TX_GPIO_CLK_ENABLE();
    __BSP_WIFI_UART_RX_GPIO_CLK_ENABLE();
    __BSP_WIFI_UART_DMA_CLK_ENABLE();
    
    /* USART CTS GPIO Configuration */
    GPIO_InitStruct.Pin = BSP_WIFI_UART_CTS_PIN;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_LOW;
    GPIO_InitStruct.Alternate = BSP_WIFI_UART_CTS_AF;
    HAL_GPIO_Init(BSP_WIFI_UART_CTS_PORT, &GPIO_InitStruct);    
    
    /* USART RTS GPIO Configuration */
    GPIO_InitStruct.Pin = BSP_WIFI_UART_RTS_PIN;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_LOW;
    GPIO_InitStruct.Alternate = BSP_WIFI_UART_RTS_AF;
    HAL_GPIO_Init(BSP_WIFI_UART_RTS_PORT, &GPIO_InitStruct);    
    
    /* USART TX GPIO Configuration */
    GPIO_InitStruct.Pin = BSP_WIFI_UART_TX_PIN;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_LOW;
    GPIO_InitStruct.Alternate = BSP_WIFI_UART_TX_AF;
    HAL_GPIO_Init(BSP_WIFI_UART_TX_PORT, &GPIO_InitStruct);

    /* USART RX GPIO Configuration     */    
    GPIO_InitStruct.Pin = BSP_WIFI_UART_RX_PIN;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_LOW;
    GPIO_InitStruct.Alternate = BSP_WIFI_UART_RX_AF;
    HAL_GPIO_Init(BSP_WIFI_UART_RX_PORT, &GPIO_InitStruct);

    pRxDmaHandle->Instance = BSP_WIFI_UART_DMA_RX;
    pRxDmaHandle->Init.Channel = BSP_WIFI_UART_DMA_RX_CHANNEL;
    pRxDmaHandle->Init.Direction = DMA_PERIPH_TO_MEMORY;
    pRxDmaHandle->Init.PeriphInc = DMA_PINC_DISABLE;
    pRxDmaHandle->Init.MemInc = DMA_MINC_ENABLE;
    pRxDmaHandle->Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;
    pRxDmaHandle->Init.MemDataAlignment = DMA_MDATAALIGN_BYTE;
    pRxDmaHandle->Init.Mode = DMA_NORMAL;
    pRxDmaHandle->Init.Priority = DMA_PRIORITY_HIGH;
    pRxDmaHandle->Init.FIFOMode = DMA_FIFOMODE_DISABLE;
    pRxDmaHandle->Init.FIFOThreshold       = DMA_FIFO_THRESHOLD_FULL;
    pRxDmaHandle->Init.MemBurst            = DMA_MBURST_INC4;
    pRxDmaHandle->Init.PeriphBurst         = DMA_PBURST_INC4;     
    HAL_DMA_Init(pRxDmaHandle);

    __HAL_LINKDMA(huart, hdmarx, gBspWifiData.rxDmaHandle);
    
    pTxDmaHandle->Instance = BSP_WIFI_UART_DMA_TX;
    pTxDmaHandle->Init.Channel = BSP_WIFI_UART_DMA_TX_CHANNEL;
    pTxDmaHandle->Init.Direction = DMA_MEMORY_TO_PERIPH;
    pTxDmaHandle->Init.PeriphInc = DMA_PINC_DISABLE;
    pTxDmaHandle->Init.MemInc = DMA_MINC_ENABLE;
    pTxDmaHandle->Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;
    pTxDmaHandle->Init.MemDataAlignment = DMA_MDATAALIGN_BYTE;
    pTxDmaHandle->Init.Mode = DMA_NORMAL;
    pTxDmaHandle->Init.Priority = DMA_PRIORITY_LOW;
    pTxDmaHandle->Init.FIFOMode = DMA_FIFOMODE_DISABLE;
    pTxDmaHandle->Init.FIFOThreshold       = DMA_FIFO_THRESHOLD_FULL;
    pTxDmaHandle->Init.MemBurst            = DMA_MBURST_INC4;
    pTxDmaHandle->Init.PeriphBurst         = DMA_PBURST_INC4;
    HAL_DMA_Init(pTxDmaHandle);

    __HAL_LINKDMA(huart, hdmatx, gBspWifiData.txDmaHandle);

    HAL_NVIC_SetPriority(BSP_WIFI_UART_DMA_TX_IRQn, 4, 1);
    HAL_NVIC_EnableIRQ(BSP_WIFI_UART_DMA_TX_IRQn);
      
    /* NVIC configuration for DMA transfer complete interrupt (USARTx_RX) */
    HAL_NVIC_SetPriority(BSP_WIFI_UART_DMA_RX_IRQn, 4, 0);   
    HAL_NVIC_EnableIRQ(BSP_WIFI_UART_DMA_RX_IRQn);
    
    /* Configure the NVIC for UART */
    HAL_NVIC_SetPriority(BSP_WIFI_UART_IRQn, 4, 1);
    HAL_NVIC_EnableIRQ(BSP_WIFI_UART_IRQn);    
  }
}

/**
  * @brief UART MSP De-Initialization 
  *        This function frees the hardware resources used in this example:
  *          - Disable the Peripheral's clock
  *          - Revert GPIO configuration to its default state
  * @param[in] huart UART handle pointer
  * @retval None
  */
void HAL_UART_MspDeInit(UART_HandleTypeDef* huart)
{
  if(huart->Instance == BSP_UART_DEBUG)
  {
   /* Reset peripherals */
    __BSP_UART_DEBUG_FORCE_RESET();
    __BSP_UART_DEBUG_RELEASE_RESET();

    /* Disable peripherals and GPIO Clocks */
    HAL_GPIO_DeInit(BSP_UART_DEBUG_TX_PORT, BSP_UART_DEBUG_TX_PIN);
    HAL_GPIO_DeInit(BSP_UART_DEBUG_RX_PORT, BSP_UART_DEBUG_RX_PIN);
    
    /* Disable the NVIC for UART */
    HAL_NVIC_DisableIRQ(BSP_UART_DEBUG_IRQn);

    /* Peripheral clock disable */
    __BSP_UART_DEBUG_CLK_DISABLE();
  }
  else  if(huart->Instance== BSP_WIFI_UART)
  {
   /* Reset peripherals */
    __BSP_WIFI_UART_FORCE_RESET();
    __BSP_WIFI_UART_RELEASE_RESET();

    /* Disable peripherals and GPIO Clocks */
    HAL_GPIO_DeInit(BSP_WIFI_UART_CTS_PORT, BSP_WIFI_UART_CTS_PIN);
    HAL_GPIO_DeInit(BSP_WIFI_UART_RTS_PORT, BSP_WIFI_UART_RTS_PIN);
    HAL_GPIO_DeInit(BSP_WIFI_UART_TX_PORT, BSP_WIFI_UART_TX_PIN);
    HAL_GPIO_DeInit(BSP_WIFI_UART_RX_PORT, BSP_WIFI_UART_RX_PIN);
    
    /* Disable the NVIC for UART */
    HAL_NVIC_DisableIRQ(BSP_WIFI_UART_IRQn);

    /* Peripheral clock disable */
    __BSP_WIFI_UART_CLK_DISABLE();
    
        /* Peripheral DMA DeInit*/
    HAL_DMA_DeInit(huart->hdmarx);
    HAL_DMA_DeInit(huart->hdmatx);
    
    HAL_NVIC_DisableIRQ(BSP_WIFI_UART_DMA_TX_IRQn);
    HAL_NVIC_DisableIRQ(BSP_WIFI_UART_DMA_RX_IRQn);
  }
}


/**
  * @brief PWM MSP Initialization 
  * @param[in] htim_pwm PWM handle pointer
  * @retval None
  */
void HAL_TIM_PWM_MspInit(TIM_HandleTypeDef* htim_pwm)
{
  GPIO_InitTypeDef GPIO_InitStruct;
  if(htim_pwm  == &hTimPwmX)
  {
    /* Peripheral clock enable */
    __BSP_MOTOR_CONTROL_BOARD_TIMER_PWM_X_CLCK_ENABLE();
  
    /* GPIO configuration */
    GPIO_InitStruct.Pin = BSP_MOTOR_CONTROL_BOARD_PWM_X_PIN;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_LOW;
    GPIO_InitStruct.Alternate = BSP_MOTOR_CONTROL_BOARD_AFx_TIMx_PWM_X;
    HAL_GPIO_Init(BSP_MOTOR_CONTROL_BOARD_PWM_X_PORT, &GPIO_InitStruct);

    /* Set Interrupt Group Priority of Timer Interrupt*/ 
    HAL_NVIC_SetPriority(BSP_MOTOR_CONTROL_BOARD_PWM_X_IRQn, 6, 0);
    
    /* Enable the timer global Interrupt */
    HAL_NVIC_EnableIRQ(BSP_MOTOR_CONTROL_BOARD_PWM_X_IRQn);  
  }
  else if(htim_pwm  == &hTimPwmY)
  {
    /* Peripheral clock enable */
    __BSP_MOTOR_CONTROL_BOARD_TIMER_PWM_Y_CLCK_ENABLE();
  
    /* GPIO configuration */ 
    GPIO_InitStruct.Pin = BSP_MOTOR_CONTROL_BOARD_PWM_Y_PIN;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_LOW;
    GPIO_InitStruct.Alternate = BSP_MOTOR_CONTROL_BOARD_AFx_TIMx_PWM_Y;
    HAL_GPIO_Init(BSP_MOTOR_CONTROL_BOARD_PWM_Y_PORT, &GPIO_InitStruct);
    
    /* Set Interrupt Group Priority of Timer Interrupt*/ 
    HAL_NVIC_SetPriority(BSP_MOTOR_CONTROL_BOARD_PWM_Y_IRQn, 6, 0);
    
    /* Enable the timer2 global Interrupt */
    HAL_NVIC_EnableIRQ(BSP_MOTOR_CONTROL_BOARD_PWM_Y_IRQn);    

  }
  else if(htim_pwm == &hTimPwmZ)
  {
    /* Peripheral clock enable */
    __BSP_MOTOR_CONTROL_BOARD_TIMER_PWM_Z_CLCK_ENABLE();
  
    /* GPIO configuration */
    GPIO_InitStruct.Pin = BSP_MOTOR_CONTROL_BOARD_PWM_Z_PIN;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_LOW;
    GPIO_InitStruct.Alternate = BSP_MOTOR_CONTROL_BOARD_AFx_TIMx_PWM_Z;
    HAL_GPIO_Init(BSP_MOTOR_CONTROL_BOARD_PWM_Z_PORT, &GPIO_InitStruct);    
    
    /* Set Interrupt Group Priority of Timer Interrupt*/ 
    HAL_NVIC_SetPriority(BSP_MOTOR_CONTROL_BOARD_PWM_Z_IRQn, 5, 0);
    
    /* Enable the timer global Interrupt */
    HAL_NVIC_EnableIRQ(BSP_MOTOR_CONTROL_BOARD_PWM_Z_IRQn);  
  }
  else if(htim_pwm == &hTimPwmE1)
  {
    /* Peripheral clock enable */
    __BSP_MOTOR_CONTROL_BOARD_TIMER_PWM_E1_CLCK_ENABLE();
  
    /* GPIO configuration */
    GPIO_InitStruct.Pin = BSP_MOTOR_CONTROL_BOARD_PWM_E1_PIN;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_LOW;
    GPIO_InitStruct.Alternate = BSP_MOTOR_CONTROL_BOARD_AFx_TIMx_PWM_E1;
    HAL_GPIO_Init(BSP_MOTOR_CONTROL_BOARD_PWM_E1_PORT, &GPIO_InitStruct);    
    
    /* Set Interrupt Group Priority of Timer Interrupt*/ 
    HAL_NVIC_SetPriority(BSP_MOTOR_CONTROL_BOARD_PWM_E1_IRQn, 5, 0);
    
    /* Enable the timer global Interrupt */
    HAL_NVIC_EnableIRQ(BSP_MOTOR_CONTROL_BOARD_PWM_E1_IRQn);  
  }
  else if(htim_pwm == &hTimPwmE2)
  {
    /* Peripheral clock enable */
    __BSP_MOTOR_CONTROL_BOARD_TIMER_PWM_E2_CLCK_ENABLE();
  
    /* GPIO configuration */
    GPIO_InitStruct.Pin = BSP_MOTOR_CONTROL_BOARD_PWM_E2_PIN;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_LOW;
    GPIO_InitStruct.Alternate = BSP_MOTOR_CONTROL_BOARD_AFx_TIMx_PWM_E2;
    HAL_GPIO_Init(BSP_MOTOR_CONTROL_BOARD_PWM_E2_PORT, &GPIO_InitStruct);    
    
    /* Set Interrupt Group Priority of Timer Interrupt*/ 
    HAL_NVIC_SetPriority(BSP_MOTOR_CONTROL_BOARD_PWM_E2_IRQn, 5, 0);
    
    /* Enable the timer global Interrupt */
    HAL_NVIC_EnableIRQ(BSP_MOTOR_CONTROL_BOARD_PWM_E2_IRQn);  
  }
  else if(htim_pwm == &hTimPwmE3)
  {
    /* Peripheral clock enable */
    __BSP_MOTOR_CONTROL_BOARD_TIMER_PWM_E3_CLCK_ENABLE();
  
    /* GPIO configuration */
    GPIO_InitStruct.Pin = BSP_MOTOR_CONTROL_BOARD_PWM_E3_PIN;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_LOW;
    GPIO_InitStruct.Alternate = BSP_MOTOR_CONTROL_BOARD_AFx_TIMx_PWM_E3;
    HAL_GPIO_Init(BSP_MOTOR_CONTROL_BOARD_PWM_E3_PORT, &GPIO_InitStruct);    
    
    /* Set Interrupt Group Priority of Timer Interrupt*/ 
    HAL_NVIC_SetPriority(BSP_MOTOR_CONTROL_BOARD_PWM_E3_IRQn, 5, 0);
    
    /* Enable the timer global Interrupt */
    HAL_NVIC_EnableIRQ(BSP_MOTOR_CONTROL_BOARD_PWM_E3_IRQn);  
  }
  else if(htim_pwm == &hTimPwmHeatBed)
  {
    /* Peripheral clock enable */
    __BSP_MISC_TIMER_PWM_HEAT_BED_CLCK_ENABLE();
  
    /* GPIO configuration */
    GPIO_InitStruct.Pin = BSP_HEAT_BED1_PIN;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_LOW;
    GPIO_InitStruct.Alternate = BSP_MISC_AFx_TIMx_PWM_HEAT_BED;
    HAL_GPIO_Init(BSP_HEAT_BED1_PORT, &GPIO_InitStruct);    
  }
  else if(htim_pwm == &hTimPwmHeatBed2)
  {
    /* Peripheral clock enable */
    __BSP_MISC_TIMER_PWM_HEAT_BED2_CLCK_ENABLE();
  
    /* GPIO configuration */
    GPIO_InitStruct.Pin = BSP_HEAT_BED2_PIN;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_LOW;
    GPIO_InitStruct.Alternate = BSP_MISC_AFx_TIMx_PWM_HEAT_BED2;
    HAL_GPIO_Init(BSP_HEAT_BED2_PORT, &GPIO_InitStruct);    
  }
  else if(htim_pwm == &hTimPwmHeatBed3)
  {
    /* Peripheral clock enable */
    __BSP_MISC_TIMER_PWM_HEAT_BED3_CLCK_ENABLE();
  
    /* GPIO configuration */
    GPIO_InitStruct.Pin = BSP_HEAT_BED3_PIN;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_LOW;
    GPIO_InitStruct.Alternate = BSP_MISC_AFx_TIMx_PWM_HEAT_BED3;
    HAL_GPIO_Init(BSP_HEAT_BED3_PORT, &GPIO_InitStruct);    
  }
  else if(htim_pwm == &hTimPwmHeatE1)
  {
    /* Peripheral clock enable */
    __BSP_MISC_TIMER_PWM_HEAT_E1_CLCK_ENABLE();
  
    /* GPIO configuration */
    GPIO_InitStruct.Pin = BSP_HEAT_E1_PIN;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_LOW;
    GPIO_InitStruct.Alternate = BSP_MISC_AFx_TIMx_PWM_HEAT_E1;
    HAL_GPIO_Init(BSP_HEAT_E1_PORT, &GPIO_InitStruct);    
  }  
  else if(htim_pwm == &hTimPwmHeatE2)
  {
    /* Peripheral clock enable */
    __BSP_MISC_TIMER_PWM_HEAT_E2_CLCK_ENABLE();
  
    /* GPIO configuration */
    GPIO_InitStruct.Pin = BSP_HEAT_E2_PIN;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_LOW;
    GPIO_InitStruct.Alternate = BSP_MISC_AFx_TIMx_PWM_HEAT_E2;
    HAL_GPIO_Init(BSP_HEAT_E2_PORT, &GPIO_InitStruct);    
  }  
    else if(htim_pwm == &hTimPwmHeatE3)
  {
    /* Peripheral clock enable */
    __BSP_MISC_TIMER_PWM_HEAT_E3_CLCK_ENABLE();
  
    /* GPIO configuration */
    GPIO_InitStruct.Pin = BSP_HEAT_E3_PIN;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_LOW;
    GPIO_InitStruct.Alternate = BSP_MISC_AFx_TIMx_PWM_HEAT_E3;
    HAL_GPIO_Init(BSP_HEAT_E3_PORT, &GPIO_InitStruct);    

  }
}

/**
  * @brief PWM MSP De-Initialization
  * @param[in] htim_pwm PWM handle pointer
  * @retval None
  */
void HAL_TIM_PWM_MspDeInit(TIM_HandleTypeDef* htim_pwm)
{
  if (htim_pwm == &hTimPwmX)
  {
    //Do not disable clock as timer can be used by other functionality
    //__BSP_MOTOR_CONTROL_BOARD_TIMER_PWM_X_CLCK_DISABLE();
  
    /* GPIO Deconfiguration */
    HAL_GPIO_DeInit(BSP_MOTOR_CONTROL_BOARD_PWM_X_PORT, BSP_MOTOR_CONTROL_BOARD_PWM_X_PIN);
    
    /* Disable the timer global Interrupt */
    HAL_NVIC_DisableIRQ(BSP_MOTOR_CONTROL_BOARD_PWM_X_IRQn);

  }
  else if (htim_pwm == &hTimPwmY)
  {
     //Do not disable clock as timer can be used by other functionality
    //__BSP_MOTOR_CONTROL_BOARD_TIMER_PWM_Y_CLCK_DISABLE();
  
    /* GPIO Deconfiguration */
    HAL_GPIO_DeInit(BSP_MOTOR_CONTROL_BOARD_PWM_Y_PORT, BSP_MOTOR_CONTROL_BOARD_PWM_Y_PIN);
    
    /* Disable the timer global Interrupt */
    HAL_NVIC_DisableIRQ(BSP_MOTOR_CONTROL_BOARD_PWM_Y_IRQn);

  }
  else if (htim_pwm == &hTimPwmZ)
  {
    //Do not disable clock as timer can be used by other functionality
    //__BSP_MOTOR_CONTROL_BOARD_TIMER_PWM_Z_CLCK_DISABLE();
    
    /* GPIO Deconfiguration */
    HAL_GPIO_DeInit(BSP_MOTOR_CONTROL_BOARD_PWM_Z_PORT, BSP_MOTOR_CONTROL_BOARD_PWM_Z_PIN);
    
    /* Disable the timer global Interrupt */
    HAL_NVIC_DisableIRQ(BSP_MOTOR_CONTROL_BOARD_PWM_Z_IRQn);
  }
  else if (htim_pwm == &hTimPwmE1)
  {
    //Do not disable clock as timer can be used by other functionality    
    ////__BSP_MOTOR_CONTROL_BOARD_TIMER_PWM_E1_CLCK_DISABLE();
    
    /* GPIO Deconfiguration */
    HAL_GPIO_DeInit(BSP_MOTOR_CONTROL_BOARD_PWM_E1_PORT, BSP_MOTOR_CONTROL_BOARD_PWM_E1_PIN);
    
    /* Disable the timer global Interrupt */
    HAL_NVIC_DisableIRQ(BSP_MOTOR_CONTROL_BOARD_PWM_E1_IRQn);    
  }
  else if (htim_pwm == &hTimPwmE2)
  {
    //Do not disable clock as timer can be used by other functionality    
    //__BSP_MOTOR_CONTROL_BOARD_TIMER_PWM_E2_CLCK_DISABLE();
    
    /* GPIO Deconfiguration */
    HAL_GPIO_DeInit(BSP_MOTOR_CONTROL_BOARD_PWM_E2_PORT, BSP_MOTOR_CONTROL_BOARD_PWM_E2_PIN);
    
    /* Disable the timer global Interrupt */
    HAL_NVIC_DisableIRQ(BSP_MOTOR_CONTROL_BOARD_PWM_E2_IRQn);       
  }
  else if (htim_pwm == &hTimPwmE3)
  {
    //Do not disable clock as timer can be used by other functionality    
    //__BSP_MOTOR_CONTROL_BOARD_TIMER_PWM_E3_CLCK_DISABLE();
    
    /* GPIO Deconfiguration */
    HAL_GPIO_DeInit(BSP_MOTOR_CONTROL_BOARD_PWM_E3_PORT, BSP_MOTOR_CONTROL_BOARD_PWM_E3_PIN);
    
    /* Disable the timer global Interrupt */
    HAL_NVIC_DisableIRQ(BSP_MOTOR_CONTROL_BOARD_PWM_E3_IRQn);       
  }
  else if(htim_pwm == &hTimPwmHeatBed)
  {
    //Do not disable clock as timer can be used by other functionality
    //__BSP_MISC_TIMER_PWM_HEAT_BED_CLCK_DISABLE();

    /* GPIO Deconfiguration */
    HAL_GPIO_DeInit(BSP_HEAT_BED1_PORT, BSP_HEAT_BED1_PIN);
  }
  else if(htim_pwm == &hTimPwmHeatBed2)
  {
    //Do not disable clock as timer can be used by other functionality
    //__BSP_MISC_TIMER_PWM_HEAT_BED2_CLCK_DISABLE();
    
    /* GPIO Deconfiguration */
    HAL_GPIO_DeInit(BSP_HEAT_BED2_PORT, BSP_HEAT_BED2_PIN);
  }
  else if(htim_pwm == &hTimPwmHeatBed3)
  {
    //Do not disable clock as timer can be used by other functionality
    //__BSP_MISC_TIMER_PWM_HEAT_BED3_CLCK_DISABLE();
    
    /* GPIO Deconfiguration */
    HAL_GPIO_DeInit(BSP_HEAT_BED3_PORT, BSP_HEAT_BED3_PIN);
  }
  else if(htim_pwm == &hTimPwmHeatE1)
  {
    //Do not disable clock as timer can be used by other functionality 
    //__BSP_MISC_TIMER_PWM_HEAT_E1_CLCK_DISABLE();
  
    /* GPIO Deconfiguration */
    HAL_GPIO_DeInit(BSP_HEAT_E1_PORT, BSP_HEAT_E1_PIN);    
  }
  else if(htim_pwm == &hTimPwmHeatE2)
  {
    //Do not disable clock as timer can be used by other functionality 
    //__BSP_MISC_TIMER_PWM_HEAT_E2_CLCK_DISABLE();
  
    /* GPIO Deconfiguration */
    HAL_GPIO_DeInit(BSP_HEAT_E2_PORT, BSP_HEAT_E2_PIN);    
  }
  else if(htim_pwm == &hTimPwmHeatE3)
  {
    //Do not disable clock as timer can be used by other functionality 
    //__BSP_MISC_TIMER_PWM_HEAT_E3_CLCK_DISABLE();
  
    /* GPIO Deconfiguration */
    HAL_GPIO_DeInit(BSP_HEAT_E3_PORT, BSP_HEAT_E3_PIN);    
  }
}

/**
  * @brief PWM Callback
  * @param[in] htim PWM handle pointer
  * @retval None
  */
void HAL_TIM_PWM_PulseFinishedCallback(TIM_HandleTypeDef *htim)
{
  if ((htim->Instance == BSP_MOTOR_CONTROL_BOARD_TIMER_PWM_X)&& (htim->Channel == BSP_MOTOR_CONTROL_BOARD_HAL_ACT_CHAN_TIMER_PWM_X))
  {
    if (BSP_MotorControl_GetDeviceState(0) != INACTIVE)
    {
      BSP_MotorControl_StepClockHandler(0);
    }
  }
  if ((htim->Instance == BSP_MOTOR_CONTROL_BOARD_TIMER_PWM_Y)&& (htim->Channel == BSP_MOTOR_CONTROL_BOARD_HAL_ACT_CHAN_TIMER_PWM_Y))
  {
    if (BSP_MotorControl_GetDeviceState(1) != INACTIVE)
    { 
      BSP_MotorControl_StepClockHandler(1);
    }
  }
  if ((htim->Instance == BSP_MOTOR_CONTROL_BOARD_TIMER_PWM_Z)&& (htim->Channel == BSP_MOTOR_CONTROL_BOARD_HAL_ACT_CHAN_TIMER_PWM_Z))
  {
    HAL_GPIO_TogglePin(BSP_MOTOR_CONTROL_BOARD_PWM_Z_PORT, BSP_MOTOR_CONTROL_BOARD_PWM_Z_PIN);
    if ((BSP_MotorControl_GetDeviceState(2) != INACTIVE)&& 
        (HAL_GPIO_ReadPin(BSP_MOTOR_CONTROL_BOARD_PWM_Z_PORT, BSP_MOTOR_CONTROL_BOARD_PWM_Z_PIN) == GPIO_PIN_SET))
    {
      BSP_MotorControl_StepClockHandler(2);
    }
  }
  if ((htim->Instance == BSP_MOTOR_CONTROL_BOARD_TIMER_PWM_E1)&& (htim->Channel == BSP_MOTOR_CONTROL_BOARD_HAL_ACT_CHAN_TIMER_PWM_E1))
  {
    HAL_GPIO_TogglePin(BSP_MOTOR_CONTROL_BOARD_PWM_E1_PORT, BSP_MOTOR_CONTROL_BOARD_PWM_E1_PIN);
    if ((BSP_MotorControl_GetDeviceState(3) != INACTIVE)&& 
        (HAL_GPIO_ReadPin(BSP_MOTOR_CONTROL_BOARD_PWM_E1_PORT, BSP_MOTOR_CONTROL_BOARD_PWM_E1_PIN) == GPIO_PIN_SET))
    {
      BSP_MotorControl_StepClockHandler(3);
    }
  }
  if ((htim->Instance == BSP_MOTOR_CONTROL_BOARD_TIMER_PWM_E2)&& (htim->Channel == BSP_MOTOR_CONTROL_BOARD_HAL_ACT_CHAN_TIMER_PWM_E2))
  {
    HAL_GPIO_TogglePin(BSP_MOTOR_CONTROL_BOARD_PWM_E2_PORT, BSP_MOTOR_CONTROL_BOARD_PWM_E2_PIN);
    if ((BSP_MotorControl_GetDeviceState(4) != INACTIVE)&& 
        (HAL_GPIO_ReadPin(BSP_MOTOR_CONTROL_BOARD_PWM_E2_PORT, BSP_MOTOR_CONTROL_BOARD_PWM_E2_PIN) == GPIO_PIN_SET))
    {
      BSP_MotorControl_StepClockHandler(4);
    }
  }
#ifndef MARLIN
  if ((htim->Instance == BSP_MOTOR_CONTROL_BOARD_TIMER_PWM_E3)&& (htim->Channel == BSP_MOTOR_CONTROL_BOARD_HAL_ACT_CHAN_TIMER_PWM_E3))
  {
    HAL_GPIO_TogglePin(BSP_MOTOR_CONTROL_BOARD_PWM_E3_PORT, BSP_MOTOR_CONTROL_BOARD_PWM_E3_PIN);
    if ((BSP_MotorControl_GetDeviceState(5) != INACTIVE)&& 
        (HAL_GPIO_ReadPin(BSP_MOTOR_CONTROL_BOARD_PWM_E3_PORT, BSP_MOTOR_CONTROL_BOARD_PWM_E3_PIN) == GPIO_PIN_SET))
    {
      BSP_MotorControl_StepClockHandler(5);
    }
  }
#endif  
  if ((htim->Instance == BSP_MISC_TIMER_TICK)&& (htim->Channel == BSP_MISC_HAL_ACT_CHAN_TIMER_TICK))
  {
#ifdef MARLIN
    IsrStepperHandler();
#else    
    TC3_Handler();
#endif    
    
  }
#ifdef MARLIN
  if ((htim->Instance == BSP_MISC_TIMER_SERVO)&& (htim->Channel == BSP_MISC_HAL_ACT_CHAN_TIMER_SERVO))
  {
    TimerStService();
  }    
  
  if ((htim->Instance == BSP_MISC_TIMER_TICK2)&& (htim->Channel == BSP_MISC_HAL_ACT_CHAN_TIMER_TICK2))
  {
    IsrTemperatureHandler();
  }  
#endif      
}

void HAL_TIM_OC_MspInit(TIM_HandleTypeDef* htim_oc)
{
  if(htim_oc->Instance == BSP_MISC_TIMER_TICK)
  {
    /* Peripheral clock enable */
    __BSP_MISC_TIMER_TICK_CLCK_ENABLE();

    /* Peripheral interrupt init*/
    /* Sets the priority grouping field */
    //HAL_NVIC_SetPriorityGrouping(NVIC_PRIORITYGROUP_3);   BDI : not needed. Done once at startup
    HAL_NVIC_SetPriority(BSP_MISC_TICK_IRQn, 0, 0);
    HAL_NVIC_EnableIRQ(BSP_MISC_TICK_IRQn);
  }
#ifdef MARLIN
  if(htim_oc->Instance == BSP_MISC_TIMER_TICK2)
  {
    /* Peripheral clock enable */
    __BSP_MISC_TIMER_TICK2_CLCK_ENABLE();

    /* Peripheral interrupt init*/
    /* Sets the priority grouping field */
    //HAL_NVIC_SetPriorityGrouping(NVIC_PRIORITYGROUP_0);  BDI
    //HAL_NVIC_SetPriorityGrouping(NVIC_PRIORITYGROUP_3);  BDI : not needed. Done once at startup
    HAL_NVIC_SetPriority(BSP_MISC_TICK2_IRQn, 5, 0);
    HAL_NVIC_EnableIRQ(BSP_MISC_TICK2_IRQn);
  }  
  if(htim_oc->Instance == BSP_MISC_TIMER_SERVO)
  {
    /* Peripheral clock enable */
    __BSP_MISC_TIMER_SERVO_CLCK_ENABLE();

    /* Peripheral interrupt init*/
    /* Sets the priority grouping field */
    // HAL_NVIC_SetPriorityGrouping(NVIC_PRIORITYGROUP_0);  BDI
    //HAL_NVIC_SetPriorityGrouping(NVIC_PRIORITYGROUP_3);  BDI : not needed. Done once at startup

    HAL_NVIC_SetPriority(BSP_MISC_SERVO_IRQn, 1, 0);
    HAL_NVIC_EnableIRQ(BSP_MISC_SERVO_IRQn);
  } 
#endif  
}

void HAL_TIM_OC_MspDeInit(TIM_HandleTypeDef* htim_oc)
{

  if(htim_oc->Instance == BSP_MISC_TIMER_TICK)
  {
    /* Peripheral clock disable */
    __BSP_MISC_TIMER_TICK_CLCK_DISABLE();

    /* Peripheral interrupt Deinit*/
    HAL_NVIC_DisableIRQ(BSP_MISC_TICK_IRQn);
  }
#ifdef MARLIN
  if(htim_oc->Instance == BSP_MISC_TIMER_TICK2)
  {
    /* Peripheral clock disable */
    __BSP_MISC_TIMER_TICK2_CLCK_DISABLE();

    /* Peripheral interrupt Deinit*/
    HAL_NVIC_DisableIRQ(BSP_MISC_TICK2_IRQn);
  }  
  if(htim_oc->Instance == BSP_MISC_TIMER_SERVO)
  {
    /* Peripheral clock disable */
    __BSP_MISC_TIMER_SERVO_CLCK_DISABLE();

    /* Peripheral interrupt Deinit*/
    HAL_NVIC_DisableIRQ(BSP_MISC_SERVO_IRQn);
  }    
#endif  
}

/**
  * @brief External Line Callback 
  * @param[in] GPIO_Pin pin number
  * @retval None
  */
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
  if (GPIO_Pin == BSP_MOTOR_CONTROL_BOARD_FLAG_PIN)
  {
    BSP_MotorControl_FlagInterruptHandler();
  }
  if (GPIO_Pin == BSP_SD_DETECT_PIN)
  {
    BSP_SD_DetectIT();
  }
 }


void HAL_ADC_MspInit(ADC_HandleTypeDef* hadc)
{

  GPIO_InitTypeDef GPIO_InitStruct;
  if (hadc->Instance == BSP_ADC)
  {
    DMA_HandleTypeDef *pDmaHandle = &(gBspAdcData.dmaHandle);
    
    /* ADC clock enable */
    __BSP_ADC_CLK_ENABLE();
  
    /* DMA controller clock enable */
    __BSP_DMA_CLK_ENABLE();
    
    /* ADC1 GPIO Configuration    */
    GPIO_InitStruct.Pin = BSP_THERM_BED1_PIN;
    GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(BSP_THERM_BED1_PORT, &GPIO_InitStruct);

    GPIO_InitStruct.Pin = BSP_THERM_BED2_PIN;
    GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(BSP_THERM_BED2_PORT, &GPIO_InitStruct);
    
    GPIO_InitStruct.Pin = BSP_THERM_E1_PIN;
    GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(BSP_THERM_E1_PORT, &GPIO_InitStruct);

    GPIO_InitStruct.Pin = BSP_THERM_E2_PIN;
    GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(BSP_THERM_E2_PORT, &GPIO_InitStruct);
    
    GPIO_InitStruct.Pin = BSP_THERM_E3_PIN;
    GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(BSP_THERM_E3_PORT, &GPIO_InitStruct);

    GPIO_InitStruct.Pin = BSP_THERM_BED3_PIN;
    GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(BSP_THERM_BED3_PORT, &GPIO_InitStruct);    
    
    GPIO_InitStruct.Pin = BSP_IR_OUT_PIN;
    GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(BSP_IR_OUT_PORT, &GPIO_InitStruct);    

    /* Peripheral DMA init*/
    pDmaHandle->Instance = BSP_DMA;
    pDmaHandle->Init.Channel = BSP_DMA_CHANNEL;
    pDmaHandle->Init.Direction = DMA_PERIPH_TO_MEMORY;
    pDmaHandle->Init.PeriphInc = DMA_PINC_DISABLE;
    pDmaHandle->Init.MemInc = DMA_MINC_ENABLE;
    pDmaHandle->Init.PeriphDataAlignment = DMA_PDATAALIGN_HALFWORD;
    pDmaHandle->Init.MemDataAlignment = DMA_MDATAALIGN_HALFWORD;
    pDmaHandle->Init.Mode = DMA_CIRCULAR;
    pDmaHandle->Init.Priority = DMA_PRIORITY_LOW;
    pDmaHandle->Init.FIFOMode = DMA_FIFOMODE_DISABLE;
    HAL_DMA_DeInit(pDmaHandle);  
    HAL_DMA_Init(pDmaHandle);

    __HAL_LINKDMA(hadc,DMA_Handle,gBspAdcData.dmaHandle);

    HAL_NVIC_SetPriority(BSP_DMA_IRQn, 7, 1);
    HAL_NVIC_EnableIRQ(BSP_DMA_IRQn);
    
    /* Peripheral interrupt init*/
    /* Sets the priority grouping field */
    HAL_NVIC_SetPriority(BSP_ADC_IRQn, 7, 0);
    HAL_NVIC_EnableIRQ(BSP_ADC_IRQn);
  }

}

void HAL_ADC_MspDeInit(ADC_HandleTypeDef* hadc)
{

  if(hadc->Instance==ADC1)
  {
    __BSP_ADC_FORCE_RESET();
    __BSP_ADC_RELEASE_RESET();   
    
    /* Peripheral clock disable */
    __BSP_ADC_CLK_DISABLE();
    
    /* ADC GPIO Deconfiguration    */
    HAL_GPIO_DeInit(BSP_THERM_BED1_PORT, BSP_THERM_BED1_PIN);
    HAL_GPIO_DeInit(BSP_THERM_BED2_PORT, BSP_THERM_BED2_PIN);
    HAL_GPIO_DeInit(BSP_THERM_BED3_PORT, BSP_THERM_BED3_PIN);
    HAL_GPIO_DeInit(BSP_THERM_E1_PORT, BSP_THERM_E1_PIN);
    HAL_GPIO_DeInit(BSP_THERM_E2_PORT, BSP_THERM_E2_PIN);
    HAL_GPIO_DeInit(BSP_THERM_E3_PORT, BSP_THERM_E3_PIN);
    HAL_GPIO_DeInit(BSP_IR_OUT_PORT, BSP_IR_OUT_PIN);
    
    /* Peripheral DMA DeInit*/
    if(hadc->DMA_Handle != NULL)
    {
      HAL_DMA_DeInit(hadc->DMA_Handle);
    }

    /* Peripheral interrupt Deinit*/
    HAL_NVIC_DisableIRQ(BSP_DMA_IRQn);
    HAL_NVIC_DisableIRQ(BSP_ADC_IRQn);
  }

}

/**
  * @brief I2C MSP Initialization 
  *        This function configures the hardware resources used in this example: 
  *           - Peripheral's clock enable
  *           - Peripheral's GPIO Configuration  
  * @param[in] hi2c I2C handle pointer
  * @retval None
  */
void HAL_I2C_MspInit(I2C_HandleTypeDef *hi2c)
{
  GPIO_InitTypeDef GPIO_InitStruct;
  if (hi2c->Instance == BSP_MISC_I2C)
  {
    /* I2C and GPIOs clock enable */
    __BSP_MISC_I2C_CLK_ENABLE();
    __BSP_MISC_I2C_SCL_GPIO_CLK_ENABLE();
    __BSP_MISC_I2C_SDA_GPIO_CLK_ENABLE();
    
    /* I2C GPIO configuration */
    GPIO_InitStruct.Pin = BSP_MISC_I2C_SCL_PIN;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_OD;
    GPIO_InitStruct.Pull = GPIO_PULLUP;
    GPIO_InitStruct.Alternate = BSP_MISC_AFx_I2C;
    HAL_GPIO_Init(BSP_MISC_I2C_SCL_PORT, &GPIO_InitStruct);
    
    GPIO_InitStruct.Pin = BSP_MISC_I2C_SDA_PIN;
    HAL_GPIO_Init(BSP_MISC_I2C_SDA_PORT, &GPIO_InitStruct);
    
    /* I2C settings */
    hi2c->Init.ClockSpeed = BSP_MISC_I2C_CLOCK_SPEED;
    hi2c->Init.DutyCycle = I2C_DUTYCYCLE_2;
    hi2c->Init.OwnAddress1 = 0;
    hi2c->Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
    hi2c->Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
    hi2c->Init.OwnAddress2 = 0;
    hi2c->Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
    hi2c->Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;
    
    /* I2C filtering */
    HAL_I2CEx_ConfigAnalogFilter(hi2c, I2C_ANALOGFILTER_ENABLE);
  }
}

/**
  * @brief I2C MSP De-Initialization 
  *        This function frees the hardware resources used in this example:
  *          - Disable the Peripheral's clock
  *          - Revert GPIO to their default state
  * @param hi2c: I2C handle pointer
  * @retval None
  */
void HAL_I2C_MspDeInit(I2C_HandleTypeDef *hi2c)
{
  /* Reset peripherals */
  __BSP_MISC_I2C_FORCE_RESET();
  __BSP_MISC_I2C_RELEASE_RESET();

  /* Disable I2C clock */
  /* GPIO clocks are left enabled as they can be used by other peripherals */
  __BSP_MISC_I2C_CLK_DISABLE();
  
  /* I2C GPIOs deconfiguration */
  HAL_GPIO_DeInit(BSP_MISC_I2C_SCL_PORT, BSP_MISC_I2C_SCL_PIN);
  HAL_GPIO_DeInit(BSP_MISC_I2C_SDA_PORT, BSP_MISC_I2C_SDA_PIN);  
}

/**
  * @}
  */

/**
  * @}
  */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
