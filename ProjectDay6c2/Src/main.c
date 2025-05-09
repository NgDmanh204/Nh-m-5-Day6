/**
 ****************************************************************************
 * @file           : main.c
 * @author         : Auto-generated by STM32CubeIDE
 * @brief          : Main program body
 ****************************************************************************
 * @attention
 *
 * <h2><center>&copy; Copyright (c) 2025 STMicroelectronics.
 * All rights reserved.</center></h2>
 *
 * This software component is licensed by ST under BSD 3-Clause license,
 * the "License"; You may not use this file except in compliance with the
 * License. You may obtain a copy of the License at:
 *                        opensource.org/licenses/BSD-3-Clause
 *
 ****************************************************************************
 */

#include <stdint.h>

// ==== Bước 2: Định nghĩa địa chỉ thanh ghi ====
#define RCC_BASE_ADDR              0x40023800UL
#define RCC_CFGR_REG_OFFSET        0x08UL
#define RCC_CR_REG_OFFSET          0x00UL

#define RCC_CFGR_REG_ADDR          (RCC_BASE_ADDR + RCC_CFGR_REG_OFFSET)
#define RCC_CR_REG_ADDR            (RCC_BASE_ADDR + RCC_CR_REG_OFFSET)
#define GPIOA_BASE_ADDR            0x40020000UL

int main(void) {
    // ==== Bước 3: Tạo con trỏ tới thanh ghi RCC ====
    uint32_t *pRccCrReg     = (uint32_t*) RCC_CR_REG_ADDR;
    uint32_t *pRccCfgrReg   = (uint32_t*) RCC_CFGR_REG_ADDR;

    // ==== Bước 4: Bật HSE bằng cách set bit HSEON (bit 16) ====
    *pRccCrReg |= (1 << 16);

    // ==== Bước 5: Chờ cho HSE sẵn sàng bằng cách đợi HSERDY (bit 17) được set ====
    while (!(*pRccCrReg & (1 << 17)));

    // ==== Bước 6: Chọn HSE làm nguồn clock chính cho hệ thống (System Clock) ====
    *pRccCfgrReg |= (1 << 0);  // SW[1:0] = 01 -> HSE

    // ==== Bước 7: Cấu hình MCO1 để chọn HSE làm nguồn xuất clock ====
    *pRccCfgrReg &= ~(0x3 << 21);  // Clear bit 21 và 22
    *pRccCfgrReg |=  (1 << 22);    // Set bit 22 -> chọn HSE (10)

    // ==== Bước 8: Chọn hệ số chia prescaler cho MCO1 là 4 ====
    *pRccCfgrReg |= (1 << 25);     // Bit 25 = 1
    *pRccCfgrReg |= (1 << 26);     // Bit 26 = 1 -> MCO1PRE = 110 (chia 4)

    // ==== Bước 9: Cấu hình PA8 làm Alternate Function (AF0 = MCO1) ====

    // 9.1: Bật clock cho GPIOA
    uint32_t *pRCCAhb1Enr = (uint32_t*)(RCC_BASE_ADDR + 0x30);
    *pRCCAhb1Enr |= (1 << 0);  // Enable GPIOA clock

    // 9.2: Cấu hình PA8 là alternate function mode
    uint32_t *pGPIOAModeReg = (uint32_t*)(GPIOA_BASE_ADDR + 0x00);
    *pGPIOAModeReg &= ~(0x3 << 16);  // Clear MODER8
    *pGPIOAModeReg |=  (0x2 << 16);  // Set MODER8 = 10 (AF mode)

    // 9.3: Gán Alternate Function 0 cho PA8 trong AFRH
    uint32_t *pGPIOAAltFunHighReg = (uint32_t*)(GPIOA_BASE_ADDR + 0x24);
    *pGPIOAAltFunHighReg &= ~(0xF << 0);  // Clear AFRH0 = AF0 (MCO1)

    // ==== DONE: PA8 đang xuất xung clock HSE chia 4 ====
    while (1) {
        // Oscilloscope/Logic analyzer có thể đo được tín hiệu tại PA8.
    }

    return 0;
}

