# osKernel
a simple os kernel

osKernel project is a small real time operating system that contains a scheduler using round robin mechanism.
This os can run three tasks in parallele.
In osKernel.h you will find all functions that support this small RTOS.
In main.c you will find a small application.
This os was tested on STM32L152RE, using ARM cortexM3, if you want to tested it and you are using another chip basing on other cortexM please refer to osKernel.c and change the address of SYSPRI3 and INTCTRL.

Fell free to use it!
