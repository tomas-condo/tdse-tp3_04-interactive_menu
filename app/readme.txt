Example: tdse-tp3_04-interactive_menu

 Description:
 Bare Metal - Event-Triggered Systems (ETS)
 App - retarget_printf_to_Console
 Project for STM32 Project (STM32CubeIDE Version: 1.17.0)

  SystemCoreClock     => 64MHz (15.625nS)
  SysTick Rate Hertz  => 1000 ticks per second (1mS)

  app.c (app.h)
   Endless loops, which execute tasks with fixed computing time. This 
   sequential execution is only deviated from when an interrupt event occurs.
   Cyclic Executive (Update by Time Code, period = 1mS)

  task_sensor.c (task_sensor.h, task_sensor_attribute.h) 
   Non-Blocking & Update By Time Code -> Sensor Modeling
   
  task_menu.c (task_menu.h) 
   Non-Blocking & Update By Time Code -> Menu Code Integration
  
  display.c (display.h)
   Non-Blocking Code -> Display Code Library

  logger.h (logger.c)
   Utilities for Retarget "printf" to Console

  dwt.h
   Utilities for Mesure "clock cycle" and "execution time" of code
  
  systick.c (systick.h) 
   Utilities for delay "microseconds"

  Special connection requirements:
   There are no special connection requirements for this example.

Build procedures:
Visit the Getting started with STM32: STM32 step-by-step at 
"https://wiki.st.com/stm32mcu/wiki/STM32StepByStep:Getting_started_with_STM32_:_STM32_step_by_step"
to get started building STM32 Projects.