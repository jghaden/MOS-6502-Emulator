/**
  ******************************************************************************
  * @file    Memory.hpp
  * @author  Josh Haden
  * @version V0.0.1
  * @date    05 NOV 2024
  * @brief   Header for Memory.cpp
  ******************************************************************************
  * @attention
  *
  *
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MEMORY_HPP
#define __MEMORY_HPP

/* Includes ------------------------------------------------------------------*/
#include <array>

#include "Types.hpp"

/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
const uint32_t MEMORY_MAX_SIZE = 65536;

/* Exported macro ------------------------------------------------------------*/
/* Exported variables --------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */
/* Class definition --------------------------------------------------------- */
class Memory
{
private:
    std::array<BYTE_T, MEMORY_MAX_SIZE> memory;
public:
    Memory();

    BYTE_T  operator[](ADDRESS_T addr) const;
    BYTE_T& operator[](ADDRESS_T addr);
};



#endif /* __MEMORY_HPP */

/************************END OF FILE************************/
