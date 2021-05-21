# dbFS-meter by Moshe Stanylov

This is an implementation of a decibel Full Scale audio meter on the Arm Cortex-M4F microcontroller.
The system is designed to recieve an instrument level signal and amplify it using an LM741 op amp chip.
The amplified signal is sent to the ADC on the TM4C123 evaluation board to be sampled.
Software polling and interrupts are used to light the appropriate LEDs on an LED array.

a more detailed discussion of the design can be found in Design.pdf
