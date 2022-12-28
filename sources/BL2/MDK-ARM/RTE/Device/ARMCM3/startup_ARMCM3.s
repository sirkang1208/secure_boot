;/**************************************************************************//**
; * @file     startup_ARMCM3.s
; * @brief    CMSIS Core Device Startup File for
; *           ARMCM3 Device Series
; * @version  V5.00
; * @date     02. March 2016
; ******************************************************************************/
;/*
; * Copyright (c) 2009-2016 ARM Limited. All rights reserved.
; *
; * SPDX-License-Identifier: Apache-2.0
; *
; * Licensed under the Apache License, Version 2.0 (the License); you may
; * not use this file except in compliance with the License.
; * You may obtain a copy of the License at
; *
; * www.apache.org/licenses/LICENSE-2.0
; *
; * Unless required by applicable law or agreed to in writing, software
; * distributed under the License is distributed on an AS IS BASIS, WITHOUT
; * WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
; * See the License for the specific language governing permissions and
; * limitations under the License.
; */

;/*
;//-------- <<< Use Configuration Wizard in Context Menu >>> ------------------
;*/


; <h> Stack Configuration
;   <o> Stack Size (in Bytes) <0x0-0xFFFFFFFF:8>
; </h>

Stack_Size      EQU     0x00001200

                AREA    STACK, NOINIT, READWRITE, ALIGN=3
Stack_Mem       SPACE   Stack_Size
__initial_sp


; <h> Heap Configuration
;   <o>  Heap Size (in Bytes) <0x0-0xFFFFFFFF:8>
; </h>

;Heap_Size       EQU     0x0001E000
;Heap_Size       EQU     0x00025000
Heap_Size       EQU     0x00012000
                
				AREA    HEAP, NOINIT, READWRITE, ALIGN=3
__heap_base
Heap_Mem        SPACE   Heap_Size
__heap_limit


                PRESERVE8
                THUMB


; Vector Table Mapped to Address 0 at Reset

                AREA    RESET, DATA, READONLY
                EXPORT  __Vectors
                EXPORT  __Vectors_End
                EXPORT  __Vectors_Size

__Vectors       DCD     __initial_sp              ; Top of Stack
                DCD     Reset_Handler             ; Reset Handler
                DCD     NMI_Handler               ; NMI Handler
                DCD     HardFault_Handler         ; Hard Fault Handler
                DCD     MemManage_Handler         ; MPU Fault Handler
                DCD     BusFault_Handler          ; Bus Fault Handler
                DCD     UsageFault_Handler        ; Usage Fault Handler
                DCD     0                         ; Reserved
                DCD     0                         ; Reserved
                DCD     0                         ; Reserved
                DCD     0                         ; Reserved
                DCD     SVC_Handler               ; SVCall Handler
                DCD     DebugMon_Handler          ; Debug Monitor Handler
                DCD     0                         ; Reserved
                DCD     PendSV_Handler            ; PendSV Handler
                DCD     SysTick_Handler           ; SysTick Handler

                ; External Interrupts
                DCD     INT_0_HANDLER            ;  0:  Watchdog Timer16
                DCD     INT_1_HANDLER            ;  1:  Real Time Clock17
                DCD     INT_2_HANDLER           ;  2:  Timer0 / Timer118
                DCD     INT_3_HANDLER           ;  3:  Timer2 / Timer319
                DCD     INT_4_HANDLER           ;  4:  MCIa20
                DCD     INT_5_HANDLER           ;  5:  MCIb21
                DCD     INT_6_HANDLER          ;  6:  UART0 - DUT FPGA22
                DCD     INT_7_HANDLER          ;  7:  UART1 - DUT FPGA23
                DCD     INT_8_HANDLER          ;  8:  UART2 - DUT FPGA24
                DCD     INT_9_HANDLER          ;  9:  UART4 - not connected25
                DCD     INT_10_HANDLER           ; 10: AACI / AC9726
                DCD     INT_11_HANDLER           ; 11: CLCD Combined Interrupt27
                DCD     INT_12_HANDLER           ; 12: Ethernet28
                DCD     INT_13_HANDLER          ; 13: USB Device29
                DCD     INT_14_HANDLER          ; 14: USB Host Controller30
                DCD     INT_15_HANDLER          ; 15: Character LCD31
                DCD     INT_16_HANDLER        ; 16: Flexray32
                DCD     INT_17_HANDLER            ; 17: CAN33	
__Vectors_End

__Vectors_Size  EQU     __Vectors_End - __Vectors

                AREA    |.text|, CODE, READONLY


; Reset Handler

Reset_Handler   PROC
                EXPORT  Reset_Handler             [WEAK]
                IMPORT  SystemInit
                IMPORT  __main
                LDR     R0, =SystemInit
                BLX     R0
                LDR     R0, =__main
                BX      R0
                ENDP
INT_0_HANDLER PROC
                EXPORT  INT_0_HANDLER           [WEAK]
                IMPORT  Interrupt0

                LDR     R0, =Interrupt0
                BX      R0
                ENDP
INT_1_HANDLER PROC
                EXPORT  INT_1_HANDLER           [WEAK]
                IMPORT  Interrupt1

                LDR     R0, =Interrupt1
                BX      R0
                ENDP
INT_2_HANDLER PROC
                EXPORT  INT_2_HANDLER           [WEAK]
                IMPORT  Interrupt2

                LDR     R0, =Interrupt2
                BX      R0
                ENDP
INT_3_HANDLER PROC
                EXPORT  INT_3_HANDLER           [WEAK]
                IMPORT  Interrupt3

                LDR     R0, =Interrupt3
                BX      R0
                ENDP
INT_4_HANDLER PROC
                EXPORT  INT_4_HANDLER           [WEAK]
                IMPORT  Interrupt4

                LDR     R0, =Interrupt4
                BX      R0
                ENDP
INT_5_HANDLER PROC
                EXPORT  INT_5_HANDLER           [WEAK]
                IMPORT  Interrupt5

                LDR     R0, =Interrupt5
                BX      R0
                ENDP
INT_6_HANDLER PROC
                EXPORT  INT_6_HANDLER           [WEAK]
                IMPORT  Interrupt6

                LDR     R0, =Interrupt6
                BX      R0
                ENDP
INT_7_HANDLER PROC
                EXPORT  INT_7_HANDLER           [WEAK]
                IMPORT  Interrupt7

                LDR     R0, =Interrupt7
                BX      R0
                ENDP
INT_8_HANDLER PROC
                EXPORT  INT_8_HANDLER           [WEAK]
                IMPORT  Interrupt8

                LDR     R0, =Interrupt8
                BX      R0
                ENDP
INT_9_HANDLER PROC
                EXPORT  INT_9_HANDLER           [WEAK]
                IMPORT  Interrupt9

                LDR     R0, =Interrupt9
                BX      R0
                ENDP
INT_10_HANDLER PROC
                EXPORT  INT_10_HANDLER           [WEAK]
                IMPORT  Interrupt10

                LDR     R0, =Interrupt10
                BX      R0
                ENDP
INT_11_HANDLER PROC
                EXPORT  INT_11_HANDLER           [WEAK]
                IMPORT  Interrupt11

                LDR     R0, =Interrupt0
                BX      R0
                ENDP
INT_12_HANDLER PROC
                EXPORT  INT_12_HANDLER           [WEAK]
                IMPORT  Interrupt12

                LDR     R0, =Interrupt12
                BX      R0
                ENDP
INT_13_HANDLER PROC
                EXPORT  INT_13_HANDLER           [WEAK]
                IMPORT  Interrupt13

                LDR     R0, =Interrupt13
                BX      R0
                ENDP
INT_14_HANDLER PROC
                EXPORT  INT_14_HANDLER           [WEAK]
                IMPORT  Interrupt14

                LDR     R0, =Interrupt14
                BX      R0
                ENDP
INT_15_HANDLER PROC
                EXPORT  INT_15_HANDLER           [WEAK]
                IMPORT  Interrupt15

                LDR     R0, =Interrupt15
                BX      R0
                ENDP
INT_16_HANDLER PROC
                EXPORT  INT_16_HANDLER           [WEAK]
                IMPORT  Interrupt16

                LDR     R0, =Interrupt16
                BX      R0
                ENDP                                                                                                                                                                                                                                                                
INT_17_HANDLER PROC
                EXPORT  INT_17_HANDLER           [WEAK]
                IMPORT  Interrupt17

                LDR     R0, =Interrupt17
                BX      R0
                ENDP


; Dummy Exception Handlers (infinite loops which can be modified)

NMI_Handler     PROC
                EXPORT  NMI_Handler               [WEAK]
                B       .
                ENDP
HardFault_Handler\
                PROC
                EXPORT  HardFault_Handler         [WEAK]
                B       .
                ENDP
MemManage_Handler\
                PROC
                EXPORT  MemManage_Handler         [WEAK]
                B       .
                ENDP
BusFault_Handler\
                PROC
                EXPORT  BusFault_Handler          [WEAK]
                B       .
                ENDP
UsageFault_Handler\
                PROC
                EXPORT  UsageFault_Handler        [WEAK]
                B       .
                ENDP
SVC_Handler     PROC
                EXPORT  SVC_Handler               [WEAK]
                B       .
                ENDP
DebugMon_Handler\
                PROC
                EXPORT  DebugMon_Handler          [WEAK]
                B       .
                ENDP
PendSV_Handler  PROC
                EXPORT  PendSV_Handler            [WEAK]
                B       .
                ENDP
SysTick_Handler PROC
                EXPORT  SysTick_Handler           [WEAK]
                B       .
                ENDP

Default_Handler PROC

                EXPORT  WDT_IRQHandler            [WEAK]
                EXPORT  RTC_IRQHandler            [WEAK]
                EXPORT  TIM0_IRQHandler           [WEAK]
                EXPORT  TIM2_IRQHandler           [WEAK]
                EXPORT  MCIA_IRQHandler           [WEAK]
                EXPORT  MCIB_IRQHandler           [WEAK]
                EXPORT  UART0_IRQHandler          [WEAK]
                EXPORT  UART1_IRQHandler          [WEAK]
                EXPORT  UART2_IRQHandler          [WEAK]
                EXPORT  UART3_IRQHandler          [WEAK]
                EXPORT  UART4_IRQHandler          [WEAK]
                EXPORT  AACI_IRQHandler           [WEAK]
                EXPORT  CLCD_IRQHandler           [WEAK]
                EXPORT  ENET_IRQHandler           [WEAK]
                EXPORT  USBDC_IRQHandler          [WEAK]
                EXPORT  USBHC_IRQHandler          [WEAK]
                EXPORT  CHLCD_IRQHandler          [WEAK]
                EXPORT  FLEXRAY_IRQHandler        [WEAK]
                EXPORT  CAN_IRQHandler            [WEAK]
                EXPORT  LIN_IRQHandler            [WEAK]
                EXPORT  I2C_IRQHandler            [WEAK]
                EXPORT  CPU_CLCD_IRQHandler       [WEAK]
                EXPORT  SPI_IRQHandler            [WEAK]

WDT_IRQHandler
RTC_IRQHandler
TIM0_IRQHandler
TIM2_IRQHandler
MCIA_IRQHandler
MCIB_IRQHandler
UART0_IRQHandler
UART1_IRQHandler
UART2_IRQHandler
UART3_IRQHandler
UART4_IRQHandler
AACI_IRQHandler
CLCD_IRQHandler
ENET_IRQHandler
USBDC_IRQHandler
USBHC_IRQHandler
CHLCD_IRQHandler
FLEXRAY_IRQHandler
CAN_IRQHandler
LIN_IRQHandler
I2C_IRQHandler
CPU_CLCD_IRQHandler
SPI_IRQHandler
                B       .

                ENDP


                ALIGN


; User Initial Stack & Heap

                IF      :DEF:__MICROLIB

                EXPORT  __initial_sp
                EXPORT  __heap_base
                EXPORT  __heap_limit

                ELSE

                IMPORT  __use_two_region_memory
                EXPORT  __user_initial_stackheap

__user_initial_stackheap PROC
                LDR     R0, =  Heap_Mem
                LDR     R1, =(Stack_Mem + Stack_Size)
                LDR     R2, = (Heap_Mem +  Heap_Size)
                LDR     R3, = Stack_Mem
                BX      LR
                ENDP

                ALIGN

                ENDIF


                END
