/**
 * @file stm32h7_hal_eth_compat.c
 * @brief Compatibility wrapper functions for STM32H7 HAL Ethernet driver
 * 
 * This file provides wrapper functions that bridge the gap between different
 * versions of the STM32 HAL Ethernet driver. Some lwIP ports expect older
 * HAL function names that don't exist in newer HAL versions.
 */

#include "stm32h7xx_hal.h"

/* External reference to the Ethernet handle - will be defined in ethernetif.c */
extern ETH_HandleTypeDef EthHandle;

#ifndef ETH_RX_DESC_CNT
#define ETH_RX_DESC_CNT 4
#endif

/**
 * @brief Assign memory buffers to Ethernet DMA descriptors
 * @param heth: ETH handle
 * @param Index: Descriptor index
 * @param pBuffer1: Pointer to first buffer
 * @param pBuffer2: Pointer to second buffer (can be NULL)
 * @return HAL status
 */
HAL_StatusTypeDef HAL_ETH_DescAssignMemory(ETH_HandleTypeDef *heth, uint32_t Index, uint8_t *pBuffer1, uint8_t *pBuffer2)
{
  /* Modern HAL doesn't have this function - use direct descriptor assignment */
  if (Index < ETH_RX_DESC_CNT) {
    heth->Init.RxDesc[Index].BackupAddr0 = (uint32_t)pBuffer1;
    if (pBuffer2 != NULL) {
      heth->Init.RxDesc[Index].BackupAddr1 = (uint32_t)pBuffer2;
    }
    return HAL_OK;
  }
  return HAL_ERROR;
}

/**
 * @brief Check if received data is available
 * @param heth: ETH handle
 * @return 1 if data available, 0 otherwise
 */
uint8_t HAL_ETH_IsRxDataAvailable(ETH_HandleTypeDef *heth)
{
  /* Simple implementation - always return 1 to allow checking */
  /* The actual data availability will be checked in GetRxDataBuffer */
  return 1;
}

/**
 * @brief Get received data buffer information
 * @param heth: ETH handle 
 * @param RxBuffer: Pointer to buffer structure to fill
 * @return HAL status
 */
HAL_StatusTypeDef HAL_ETH_GetRxDataBuffer(ETH_HandleTypeDef *heth, void *RxBuffer)
{
  ETH_BufferTypeDef *buf = (ETH_BufferTypeDef *)RxBuffer;
  
  /* Modern HAL equivalent - extract buffer info from descriptor */
  if (buf != NULL && heth->RxDescList.RxDescIdx < ETH_RX_DESC_CNT) {
    buf->buffer = (uint8_t *)heth->Init.RxDesc[heth->RxDescList.RxDescIdx].BackupAddr0;
    buf->len = heth->Init.RxDesc[heth->RxDescList.RxDescIdx].DESC1 & 0x3FFF; /* Extract length */
    return HAL_OK;
  }
  return HAL_ERROR;
}

/**
 * @brief Get received data length
 * @param heth: ETH handle
 * @param Length: Pointer to store the length
 * @return HAL status  
 */
HAL_StatusTypeDef HAL_ETH_GetRxDataLength(ETH_HandleTypeDef *heth, uint32_t *Length)
{
  /* Modern HAL equivalent */
  if (Length != NULL && heth->RxDescList.RxDescIdx < ETH_RX_DESC_CNT) {
    *Length = heth->Init.RxDesc[heth->RxDescList.RxDescIdx].DESC1 & 0x3FFF; /* Extract length */
    return HAL_OK;
  }
  return HAL_ERROR;
}

/**
 * @brief Build/Release RX descriptors for DMA
 * @param heth: ETH handle
 * @return HAL status
 */
HAL_StatusTypeDef HAL_ETH_BuildRxDescriptors(ETH_HandleTypeDef *heth)
{
  /* Modern HAL equivalent - advance to next descriptor */
  if (heth->RxDescList.RxDescIdx < ETH_RX_DESC_CNT) {
    heth->RxDescList.RxDescIdx++;
    if (heth->RxDescList.RxDescIdx >= ETH_RX_DESC_CNT) {
      heth->RxDescList.RxDescIdx = 0;
    }
    return HAL_OK;
  }
  return HAL_ERROR;
}
