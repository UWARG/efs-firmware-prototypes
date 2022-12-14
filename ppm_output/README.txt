PPM Output - Jack Greenwood

General Notes 
 - Period and auto-reload register (ARR) are the same and should be considered while reading this document. 
 - To test my code, I set up my project and continuously ran the code in the main while loop. Doing so allowed me to see multiple groups of pulses and verify that values
   were as expected. 
 - For testing, TIM2 was used to simulate a PWM wave we are sampling, and TIM5 was our PPM generation channel. (See configuration details below). 
 - TIM5 was configured to have a default period (ARR) of 64000 ticks. This might vary depending on which channels are available for PWM generation. 64000 was chosen simply because LOS timers were using 64000 at the time of writing.  
 - Both timers were up counters, 64000 period, 14 prescaler, 6560 pulse width, PWM mode one, enable the compare preload (not entirely sure how this helps), fast mode disabled, and clock 
   polarity high.
-  On the STM32F401RE, some timers can set a 32-bit value for the period, while others may only set the 16-bit maximum. Why this is the case, I am still determining. While testing, 
   my PPM output channel was chosen to be TIM5, given it could set a 32-bit value for the period.  
 - Some values will vary depending on the system used. For example, our ticks per millisecond will change depending on the clock speed of your project. This is important as
   our HAL_Delay call in particular situations depends on the value of our ticks per millisecond. See the math section for further information. 
 - The +/- 0.05 seen at the end of each time we set our period for TIM5 is for a bug where small changes in our ppm lead to inconsistent results. As a result, some captures might be 
   too large or too small by 0.05 milliseconds, but this is minimal as pulse widths are often off by +/- 0.05 ms.
 
 Math 
 - The project used to test had a system clock of 84MHz, or 84 000 000 Hz. The channel used to generate PPM pulses had a prescaler of 14. Using this information and knowledge from the boot camp, 
   we may compute the expected ticks per second as (84 000 000) / (14 + 1) = 5 600 000. 
 - Because we like working in milliseconds, we must convert our ticks per second to ticks per millisecond by multiplying by 0.001. Doing so results in a value of 5600.
 
 Limitations/Known Issues
 - The difference between your last and first pulse must be less than ~15000 ticks. This gives you a range of roughly 5.4ms between your starting and final pulse for a system running the 
   configurations described above. Undefined behavior happens when the difference is too high because the code gets stuck in a hal delay call before updating our period, subsequently missing a 
   pulse value. 
 - Issue where the code takes too long on the HAL_delay call, and a pulse value is sent twice before sending the next value. This issue occurs when our first pulse is less than the 
   second pulse, our period is 10ms ( or close to it), and the coming pulse is close to 10ms. I have not been able to replicate this issue recently, and I am 
   not sure that said conditions cause it, but I am sure it still exists. 
 - Due to the fact that our period is a 32 bit value, our system will round values that cannot be acheived . Ultimately, we lose points of precision, but this is inevitable.  
 
 Cases
 -  Pulse difference defaults to false when the difference in coming pulses is 0. In doing so, we see a negligible (0.05 millisecond) change in our pulse value.  
