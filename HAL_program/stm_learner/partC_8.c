/*

8: Independent Watchdog (IWDG) Timer Refresh
    Problem: Embedded systems require watchdog timers to recover from lockups. 
    Configure the STM32 Independent Watchdog using its register base (0x40003000) with a ~1-second timeout, 
    and write a refresh function using key register (IWDG_KR) pointer writes.

*/