/*
*****************************************************************************
**
 
**  File        : LinkerScript.ld
**
**  Abstract    : Linker script for STM32H743XIHx Device with
**                2048KByte FLASH, 128KByte RAM
**
**                Set heap size, stack size and stack location according
**                to application requirements.
**
**                Set memory bank area and size if external memory is used.
**
**  Target      : STMicroelectronics STM32
**
**
**  Distribution: The file is distributed as is, without any warranty
**                of any kind.
**
*****************************************************************************
** @attention
**
** Copyright (c) 2017 STMicroelectronics.
** All rights reserved.
**
** This software component is licensed by ST under BSD 3-Clause license,
** the "License"; You may not use this file except in compliance with the
** License. You may obtain a copy of the License at:
**                        opensource.org/licenses/BSD-3-ClauSTMMse
**
****************************************************************************
*/
 
/* Entry Point , 定义程序入口, 程序将从 Reset_Handler 开始执行*/
ENTRY(Reset_Handler)
 
/* Highest address of the user mode stack */
_estack = 0x20020000;    /* end of RAM ,定义RAM的地址空间是[0x20000000,0x20020000]*/
 
/* Generate a link error if heap and stack don't fit into RAM */
_Min_Heap_Size = 0x200;      /* required amount of heap ,定义堆的大小是512字节*/
_Min_Stack_Size = 0x400; /* required amount of stack ，定义栈的大小是1024字节*/
/* Specify the memory areas */
MEMORY
{
DTCMRAM (xrw)      : ORIGIN = 0x20000000, LENGTH = 128K
RAM_D1 (xrw)      : ORIGIN = 0x24000000, LENGTH = 1024K
RAM_D2 (xrw)      : ORIGIN = 0x30000000, LENGTH = 128K
RAM_D3 (xrw)      : ORIGIN = 0x38000000, LENGTH = 32K
ITCMRAM (xrw)      : ORIGIN = 0x00000000, LENGTH = 64K
FLASH (rx)      : ORIGIN = 0x8000000, LENGTH = 128K
}
/* Define output sections */
SECTIONS
{
  /* The startup code goes first into FLASH，中断向量表 */
  .isr_vector :
  {
    . = ALIGN(4); /* 4字节对齐 */
    KEEP(*(.isr_vector)) /* Startup code，KEEP 强制连接器保存中断向量表的字段，不能被编译器优化掉*/
    . = ALIGN(4); /* 4字节对齐 */
  } >FLASH
  /* The program code and other data goes into FLASH ，程序代码段*/
  .text :
  {
    . = ALIGN(4);
    *(.text)           /* .text sections (code) */
    *(.text*)          /* .text* sections (code) */
    *(.glue_7)         /* glue arm to thumb code */
    *(.glue_7t)        /* glue thumb to arm code */
    *(.eh_frame)
    KEEP (*(.init))     /* KEEP强制连接器保存.init 不能被编译器优化掉*/
    KEEP (*(.fini))     /* KEEP强制连接器保存.fini 不能被编译器优化掉*/
    . = ALIGN(4);
    _etext = .;        /* define a global symbols at end of code */
  } >FLASH
  /* Constant data goes into FLASH */
  .rodata :
  {
    . = ALIGN(4);
    *(.rodata)         /* .rodata sections (constants, strings, etc.) */
    *(.rodata*)        /* .rodata* sections (constants, strings, etc.) */
    . = ALIGN(4);
  } >FLASH
  .ARM.extab   : { *(.ARM.extab* .gnu.linkonce.armextab.*) } >FLASH
  .ARM : {
    __exidx_start = .;
    *(.ARM.exidx*)
    __exidx_end = .;
  } >FLASH
  /* .preinit_array和.init_array 保存程序或者共享对象加载时的初始化函数指针*/ 
  .preinit_array     :
  {
    PROVIDE_HIDDEN (__preinit_array_start = .);
    KEEP (*(.preinit_array*))
    PROVIDE_HIDDEN (__preinit_array_end = .);
  } >FLASH
  .init_array :
  {
    PROVIDE_HIDDEN (__init_array_start = .);
    KEEP (*(SORT(.init_array.*)))
    KEEP (*(.init_array*))
    PROVIDE_HIDDEN (__init_array_end = .);
  } >FLASH
  
  /* .fini_array 保存程序或者共享对象退出时的函数地址*/ 
  .fini_array :
  {
    PROVIDE_HIDDEN (__fini_array_start = .);
    KEEP (*(SORT(.fini_array.*)))
    KEEP (*(.fini_array*))
    PROVIDE_HIDDEN (__fini_array_end = .);
  } >FLASH
  /* used by the startup to initialize data */
  _sidata = LOADADDR(.data);
  /* Initialized data sections goes into RAM, load LMA copy after code */
  /*.data对应已初始化的全局变量, 编译后位于可执行文件中, 由启动代码加载到数据区中,由启动代码把这部分内容拷贝到RAM*/
  .data : 
  {
    . = ALIGN(4);
    _sdata = .;        /* create a global symbol at data start */
    *(.data)           /* .data sections */
    *(.data*)          /* .data* sections */
    . = ALIGN(4);
    _edata = .;        /* define a global symbol at data end */
  } >DTCMRAM AT> FLASH
  
  /* Uninitialized data section,没有初始值的全局变量 */
  . = ALIGN(4);
  .bss :
  {
    /* This is used by the startup in order to initialize the .bss secion */
    _sbss = .;         /* define a global symbol at bss start */
    __bss_start__ = _sbss;
    *(.bss)
    *(.bss*)
    *(COMMON)
    . = ALIGN(4);
    _ebss = .;         /* define a global symbol at bss end */
    __bss_end__ = _ebss;
  } >DTCMRAM
  /* User_heap_stack section, used to check that there is enough RAM left */
  ._user_heap_stack :
  {
    . = ALIGN(8);
    PROVIDE ( end = . );
    PROVIDE ( _end = . );
    . = . + _Min_Heap_Size;
    . = . + _Min_Stack_Size;
    . = ALIGN(8);
  } >DTCMRAM
  
  /* Remove information from the standard libraries ,把c库，数学库和gcc库过滤掉*/
  /DISCARD/ :
  {
    libc.a ( * )
    libm.a ( * )
    libgcc.a ( * )
  }
  .ARM.attributes 0 : { *(.ARM.attributes) }
}