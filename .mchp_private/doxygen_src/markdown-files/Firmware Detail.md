

<span id="start-doc"><a name="start-doc"></a></span>

<span id="firmware-overview"><a name="firmware-overview"> </a></span>

# Firmware Overview

An overview of the firmware is shown below.
The power controller state machine and fault handler are executed every 100us by the scheduler. So also are the GUI handler (every 1ms), and the HMI driver (every 100ms).

There are 5 interrupt sources.

- ISRADCCAN0: executed every 6th phase A PWM cycle. Measure phase A secondary current and Vout every pass, then cycle through sampling other ADC channels on the shared ADC core. Voltage loop is executed here, as well as SR state machine.
- ISRADCCAN1: executed every 6th phase B PWM cycle. Measure phase B secondary current.
- ISRSCCP1: triggered every 4th input capture event (SCCP peripheral), used to measure Vdd via a PWM signal from primary side whose duty cycle is proportional to the Vdd level.
- ISRCMP1: triggered if comparator 1 trips, which happens if there is a large output over current event on phase A.
- ISRCMP3: triggered if comparator 3 trips, which happens if there is a large output over current event on phase B.

<p>
  <center>
    <img src="images/illc-34.png" alt="firmware-0" width="1000">
    <br>
    Firmware overview.
  </center>
</p>

Microchip Code Configurator (MCC) is used to configure the peripherals. They are configured at run-time at the start of _main()_, before the background loop is initiated.

The main files are as follows:

- _driver/drv_adc.c_: this contains the 5 interrupt service routines.
- _power_controller/drv_pwrctrl_ILLC.c_: power controller state machine that is executed every 100us.
- _power_controller/drv_pwrctrl_ILLC_fault.c_: fault handlers.
- _power_controller/drv_pwrctrl_ILLC_ILPH_SRandControl.c_: power supply voltage loop and SR state machine and driver functions when running in interleaved mode.
- _power_controller/drv_pwrctrl_ILLC_PHA_SRandControl.c_: contains power supply voltage loop and SR state machine and driver functions when running phase A only.
- _misc/fault_common.c_: generic fault functions.

<p>
  <center>
    <img src="images/illc-36.png" alt="firmware-0" width="1000">
    <br>
    Firmware block diagram.
  </center>
</p>

<p>
  <center>
    <img src="images/illc-35.png" alt="firmware-0" width="1000">
    <br>
    Firmware block details.
  </center>
</p>

We now will go into more detail on certain parts of the firmware project that we deemed important and/or difficult to understand.

[[back to top](#start-doc)]

- - -

<span id="llc-modes-of-operation-single-phase-or-interleaved"><a name="llc-modes-of-operation-single-phase-or-interleaved"> </a></span>

# LLC Modes of Operation: Single Phase or Interleaved

The LLC power board contains two phases, phase A and phase B. The firmware can be configured to run just a single phase (phase A), or run in interleaved mode (phase A and phase B both running).

To configure the mode of operation, open the header file "project_settings.h".

<p>
  <center>
    <img src="images/illc-37.png" alt="project-settings" width="400">
    <br>
    Location of project_settings.h.
  </center>
</p>

Scroll down to line 58 (see below).

To run in interleaved mode, POWER_STAGE_CONFIG_INTERLEAVED needs to be defined and POWER_STAGE_CONFIG_PHASE_A_ONLY should not be defined.

To run in phase A only, POWER_STAGE_CONFIG_PHASE_A_ONLY needs to be defined and POWER_STAGE_CONFIG_INTERLEAVED  should not be defined.

If neither or both are defined, you will get a compile error.

<p>
  <center>
    <img src="images/illc-38.png" alt="project-settings" width="900">
    <br>
    Configuration #defines.
  </center>
</p>

[[back to top](#start-doc)]
- - -

<span id="converter-state-machine"><a name="converter-state-machine"> </a></span>

# Converter State Machine

The main power controller state machine is illustrated below. It is executed every 100us. The code is located in _power_controller/drv_pwrctrl_ILLC.c_, see the function_Drv_PwrCtrl_ILLC_Task_100us()_. Most of the states are pretty standard for a digital DC/DC converter state machine (see state diagram below). Perhaps the only states worth describing in detail are the soft-start states, as these differ somewhat from other DC/DC converter state machines. Hence these are described in some detail below.

<p>
  <center>
    <img src="images/illc-19.png" alt="state-machine" width="500">
    <br>
    Power supply state machine.
  </center>
</p>

<span id="soft-starting-the-converter"><a name="soft-starting-the-converter"> </a></span>

[[back to top](#start-doc)]
- - -

## Soft-Starting the Converter

The soft-start ramp is split in 2 parts

(1) Run primary drive PWMs open-loop, at a fixed frequency of 1MHz (max frequency for our design)
- start at the minimum duty cycle (Ton = 50ns).
- ramp the duty cycle linearly in steps of 2.5ns every 100us until the duty reaches 45%.

(2) Run primary-drive PWMs closed-loop
- fixed duty cycle of 45%.
- compensator sets PWM frequency.
- ramp compensator reference linearly from pre-bias output voltage to target output voltage set-point.

Part 1 of the soft-start ramp is to prevent massive inrush current: if the duty cycle was set immediately to 50% the inrush current required to charge the output capacitor would trip the hardware over current protection. Thus part 1 is a duty cycle soft-start.

Note that the SRs drives (PWM2 for phase A and PWM4 for phase B) are switched off during soft-start, so any output current conduction is through the body diodes of the SRs during this time.

Also note that if we are running in interleaved mode, PG3 setup is identical to PG1, but PG3 lags PG1 by 90 degrees.

[[back to top](#start-doc)]
- - -

<span id="state-pcs_soft_start_pre1"><a name="state-pcs_soft_start_pre1"> </a></span>

### State PCS_SOFT_START_PRE1

In this state, the PWMs are running in open-loop mode (that is, they are not driven by the compensator).
The frequency is fixed at 1MHz. The on-time of the primary side half-bridge drive signals is set to 50ns.
Then every 100us, the on-time (and hence duty cycle) is increase by 2.5ns.
This continues until the duty cycle is 45% (as we allow for a dead-time of 50ns).

<p>
  <center>
    <img src="images/illc-20.png" alt="ss-pre1-00" width="1000">
    <br>
    Half bridge high and low switching signals at power-on.
  </center>
</p>

<p>
  <center>
    <img src="images/illc-21.png" alt="ss-pre1-01" width="1000">
    <br>
    Half bridge high and low switching signals during open-loop duty cycle ramp, with fixed Fsw = 1MHz.
  </center>

</p>

Once we reach the target primary drive on-time (equivalent to 45% duty cycle), we move to the state PCS_SOFT_START_PRE2.

[[back to top](#start-doc)]
- - -

<span id="state-pcs_soft_start_pre2"><a name="state-pcs_soft_start_pre2"> </a></span>

### State PCS_SOFT_START_PRE2

In this state, we enable frequency modulation of the PWM outputs by the voltage mode compensator (closed-loop mode of operation). The on-time of the primary side PWM drive signals is fixed at (PG1PER/2 - 50ns). The voltage loop reference is initialized based on the measured output voltage at this point. Once we complete this initialization, we move to the state PCS_SOFT_START_.

[[back to top](#start-doc)]
- - -

<span id="state-pcs_soft_start"><a name="state-pcs_soft_start"> </a></span>

### State PCS_SOFT_START

In this state, the reference to the voltage loop compensator is ramped linearly to the target set-point. The compensator controls the frequency of the primary side LLC half-bridge signals. The duty cycle is always set to (PG1PER/2 - 50ns).

Note that in this state the SRs are enabled.

A oscilloscope screenshot of the entire start up phase is shown below. The different parts are described also.

<p>
  <center>
    <img src="images/illc-22.png" alt="ss-00" width="1300">
    <br>
    Soft-start with different stages highlighted and described.
  </center>
</p>

[[back to top](#start-doc)]

- - -

<span id="fault-protection"><a name="fault-protection"> </a></span>

# Fault Protection

The fault protection code is executed every 100us at the start of the converter state machine in the function _Drv_PwrCtrl_ILLC_Fault_Check()_. The body of the fault code is located in the files _power_controller/drv_pwr_ctrl_ILLC_fault.c_and_misc/fault_common.c_.

There are two types of protection:

1. Firmware fault protection
2. Hardware fault protection

The firmware fault protection is implemented on the dsPIC on the DP-PIM. The hardware fault protection is implemented on the LLC power board. It's purpose is to prevent catastrophic board damage, particularly due to input and output over current events.

[[back to top](#start-doc)]
- - -

<span id="firmware-fault-protection"><a name="firmware-fault-protection"> </a></span>

## Firmware Fault Protection

All of our firmware fault protection has the same functionality. Each fault has a trigger threshold, a clear threshold, a fault blanking time and a fault clear time.

This is illustrated below, for a fault with a "max" threshold, which means that the fault is triggered when the fault source is above a threshold (output over voltage protection, for example).

Once the fault source breaches the trigger threshold, a timer is started. If the fault source stays above the trigger threshold for longer than the fault blanking time, then the fault becomes active, which means that the PWMs are switched off and the converter state machine is set to the "FAULT ACTIVE" state.

If the fault source drops back below the trigger threshold before the fault blanking time has expired, the timer is reset.  

When the fault is active, if the fault source stays below the fault clear threshold for the duration of the fault clear time, then the fault is cleared. When all fault sources are cleared, the converter will attempt to restart.

<p>
  <center>
    <img src="images/illc-29.png" alt="fault-protection" width="1000">
    <br>
    Firmware Fault protection.
  </center>
</p>

This is shown in more detail in a flowchart below. When "fault active == true", then the fault is active and the converter is disabled. When "fault active == false" the converter can attempt to start up.

<p>
  <center>
    <img src="images/illc-30.png" alt="fault-protection" width="1000">
    <br>
    Flowchart illustrating the firmware fault protection.
  </center>
</p>

All faults shown in the table below have firmware protection like this. In our firmware, this fault protection is run every 100us.

<p>
  <center>
    <img src="images/illc-31.png" alt="fault-protection" width="900">
    <br>
    ILLC faults with firmware protection.
  </center>
</p>

[[back to top](#start-doc)]
- - -

<span id="hardware-fault-protection"><a name="hardware-fault-protection"> </a></span>

## Hardware fault protection

The purpose of the hardware fault protection is to prevent catastrophic board damage, particularly from input or output over current.
Once triggered, it kicks in immediately (there is no fault blanking time). It sets all PWM drive signals to 0, which will turn off the converter. Note that this is completely independent of the dsPIC, so even if there are drive signals coming from the dsPIC when the hardware fault protection is tripped, the hardware protection will over-ride these signals (through AND gates on the hardware) before they get to the FET drivers.

<p>
  <center>
    <img src="images/illc-32.png" alt="fault-protection" width="400">
    <br>
    ILLC faults with hardware protection.
  </center>
</p>

If the hardware fault protection is triggered, the red LED LD700 will turn on. The protection is latched, meaning that once triggered it will not clear itself, it needs to be manually cleared.

If you want to re-run the board, you need to

- disable all PWMs first, either by holding down the RESET push button, or erasing the dsPIC firmware (we recommend the second option as it is safer)
- then short press the "RESET protection" push button on the HMI interface.

On the dsPIC, output over current protection using comparators and DACs is also implemented as follows:

- Current transformer phase A secondary sense tied to CMP1DAC (pin 22 of dsPIC)
- Current transformer phase B secondary sense tied to CMP3DAC (pin 18 of dsPIC)

Either of these comparators tripping will trigger the highest priority interrupt, which disables all PWM drive signals and puts the converter in the "FAULT ACTIVE" state.
Like the hardware fault protection, this fault protection is also latched, meaning that the dsPIC needs to be reset to restart the converter. If this fault protection is triggered, the RESET flag in the Power Board Visualizer GUI will be set, as shown below, indicating that the dsPIC needs to be reset to re-start the LLC converter.

<p>
  <center>
    <img src="images/illc-33.png" alt="fault-protection" width="250">
    <br>
    ILLC faults with hardware protection.
  </center>
</p>

[[back to top](#start-doc)]

- - -

<span id="pwm-setup"><a name="pwm-setup"> </a></span>

# PWM Setup

Most of the PWM setup is done by calling initialization functions generated by MCC at the top of _main()_. Some more custom configuration is also done at runtime as required.

- - -

<span id="pwm-routing"><a name="pwm-routing"> </a></span>

## PWM Routing

The two simplified schematics below show the routing of the PWM signals for phase A and phase B. PWM1 and PWM3 output are used for the primary drives for phase A and phase B respectively, while PWM2 and PWM4 are used for the SR drives.

The dsPIC is on the secondary side, so PWM1H, PWM1L, PWM3H and PWM3L have to pass through the isolation barrier. FET drivers are not shown here, please see the full schematic in the users guide for more detail.

<p>
  <center>
    <img src="images/illc-07.png" alt="PWM phase A" width="700">
    <br>
    PWM routing for phase A.
  </center>
</p>

<p>
  <center>
    <img src="images/illc-08.png" alt="PWM phase B" width="700">
    <br>
    PWM routing for phase B.
  </center>
</p>

The switching frequency range of our LLC solution is from 800kHz and 1MHz. To achieve robust operation in this frequency range with a PWM resolution of 250ps, we needed some special PWM module configuration, which will be describe in the following sections.

[[back to top](#start-doc)]
- - -

<span id="phase-a-pwm-setup"><a name="phase-a-pwm-setup"> </a></span>

## Phase A PWM Setup

For a single phase, the LLC primary drive signals should have a fixed duty cycle (50% minus some dead time) and variable frequency, as shown below. The voltage loop compensator output modulates the switching frequency of the converter. The drive signals to the high side and low side of the primary side half-bridge (before the resonant tank) need to be complementary, with a dead time between the falling edge on the high side drive and the rising edge of the low side drive, and visa-versa.

In our example firmware, the frequency can cary from 800kHz (max output voltage) to 1MHz (min output voltage).

To get this type of waveform from the PWM module, we configured the PWM peripherals in "Independent Edge, dual output mode".

In this mode of operation, each PWM edge is set independently via a register. Specifically,

- PWM1H rising edge occurs when PWM1 counter = PG1PHASE.
- PWM1H falling edge occurs when PWM1 counter = PG1DC.
- PW1L rising edge occurs when PWM1 counter = PG1TRIGA.
- PWM1L falling edge occurs when PWM1 counter = PG1TRIGB.

The primary drive signals for phase A (from PWM1 peripheral) are shown below.

<p>
  <center>
    <img src="images/illc-09.png" alt="PWM1 setup" width="1100">
    <br>
    PWM1 setup.
  </center>
</p>

<p>
  <center>
    <img src="images/illc-10.png" alt="PWM1 config" width="600">
    <br>
    PWM mode: independent edge, dual output.
  </center>
</p>

The SR drive signals for phase A come from the PWM2 module. PWM2H drives the SR on the high side of the half-bridge on the secondary side, and PWM2L drives the SR on the low-side. PWM2 is also configured in "independent edge, dual output" mode, but the SWAP bit is set, so that PWM2H and PWM2L are swapped.
The PWM2H and PWM2L setup is illustrated below.

<p>
  <center>
    <img src="images/illc-11.png" alt="PWM2 setup" width="1100">
    <br>
    PWM2 setup.
  </center>
</p>

[[back to top](#start-doc)]
- - -

<span id="phase-b-pwm-setup"><a name="phase-b-pwm-setup"> </a></span>

## Phase B PWM Setup

Phase B setup as follows:

- PWM3H drives primary side half-bridge, high side
- PWM3L drives primary side half-bridge, low side
- PWM4H drives secondary side (SR) half-bridge, high side
- PWM4L drives secondary side (SR) half-bridge, low side

Phase A and phase B run 90 degrees out of phase.
The synchronization scheme works as follows:

- PWM2 is synchronized to the EOC (End of Cycle) trigger of PWM1, so they run in phase.
- PWM3 is synchronized to PG2TRIGC via the PWM2 peripheral's PCI input and the PWM event A output. PG2TRIGC is set to (PG1PER / 4 ), so the PWM3 cycle starts 1/4 of a period after PWM1 and PWM2 cycle start.
- PWM4 is synchronized to the EOC (End of Cycle) trigger of PWM3, so PWM3 and PWM4 run in phase.
  
<p>
  <center>
    <img src="images/illc-15.png" alt="PWM2 setup" width="1100">
    <br>
    phase A and phase B sync.
  </center>
</p>

The ADC trigger for PWM1 is set to occur every 6th PWM1 period. It is set using PG1TRIGC (as PG1TRIGA and PG1TRIGB are already in use), and in the running firmware is set to be at the middle of the PG1H on-time so as to sample the average current.

The ADC trigger for PWM3 is set to occur every 6th PWM3 period. It is set using PG3TRIGC, and in the running firmware is set to be at the middle of the PG3H on-time so as to sample the average current.

The "easy setup" view for all 4 PWM modules is shown below.

<p>
  <center>
    <img src="images/illc-12.png" alt="PWM config MCC" width="1500">
    <br>
    PWM configuration in MCC (easy setup).
  </center>
</p>

Some registers in the "registers" view also need to be modified. The fields that need to be modified for phase A (related to PWM1 and PWM2) are highlighted below.

<p>
  <center>
    <img src="images/illc-13.png" alt="phase A config MCC" width="1500">
    <br>
    Phase A configuration in MCC (additional register settings).
  </center>
</p>

The fields that need to be modified for phase B (related to PWM3 and PWM4) are highlighted below.

<p>
  <center>
    <img src="images/illc-14.png" alt="phase B config MCC" width="1500">
    <br>
    Phase B configuration in MCC (additional register settings).
  </center>
</p>

[[back to top](#start-doc)]

- - -

<span id="regulating-the-output-voltage"><a name="regulating-the-output-voltage"> </a></span>

# Regulating the Output Voltage

In this section we describe  how to measure the open loop gain and phase of the plant, and show some results that we took from the LLC board.
We then discuss the compensator used to regulate the output voltage and show some open loop gain measurements of the closed loop system at different operating points.

- - -

<span id="plant-measurements"><a name="plant-measurements"> </a></span>

## Plant Measurements

It can be useful to measure the open loop frequency response of the plant, to allow the compensator to be designed appropriately.
In this section we describe a way to do this on this LLC demo board.

First please read section 1.4 of [[MA330048]](https://www.microchip.com/MA330048).
This describes how to measure the loop-gain of the plant using the dsPIC33CK256MP506 Digital Power Plug-In Module (DP-PIM) and a vector network analyzer such as the Bode 100 from Omnicron.

In this instance, we made some small modifications to the procedure described in the DP-PIM user's guide.

<p>
  <center>
    <img src="images/illc-43.png" alt="plant-04" width="600">
    <br>
    Plant measurement setup using DP-PIM.
  </center>
</p>

In the diagram above, circuitry inside the dsPIC is designated by an orange colour. Blue blocks are measurement circuitry (from the Bode 100 in our case). The plant is shown in green. The operational amplifier circuitry is on the DP-PIM. The Bode 100 generator output (which creates a small AC sinusoid that is swept over frequency as part of the open loop frequency response measurement) is connected across TP1 and TP2 of the DP-PIM. This signal then passes through an operational amplifier circuit on the DP-PIM.
The purpose of the operational amplifier circuit is to add an offset of 1.65V to the AC signal, as the ADC on the dsPIC can only digitize positive voltages and the output of the Bode 100 is AC.

This AC signal with a 1.65V DC offset is then digitized by the ADC on pin AN18 of the dsPIC. The ADC code equivalent to the 1.65V DC offset is subtracted in the firmware, and the result (which is a digitized AC signal) is added to the control input. Thus the control input is "disturbed" by this digitized AC sinusoid. For an LLC, the control input modulates the switching frequency. So as the control input moves up and down, so will the switching frequency.

The plant frequency response measurement includes the digital modulator and so is calculated as "control to output".

<p>
  <center>
    <img src="images/illc-45.png" alt="plant-06" width="300">
    <br>
    Plant "Control to Output".
  </center>
</p>

This is a measure of how much Vout varies in gain and phase as the control input is disturbed. Each measurement point corresponds to a frequency, this is the frequency of the AC signal disturbing the control input. The frequency of the AC disturbance is swept from almost DC to (usually) the ADC sampling frequency / 2.

This means that channel 1 of the Bode 100 should be connected as close as possible to where the control input is disturbed. Channel 2 of the Bode 100 should be connected to the output of the converter.

Channel 1 could be connected directly to the AN18 pin. In this case, the plant gain measurement would be accurate, but the measurement of the phase response would not be accurate at higher frequencies because of the ADC sampling delay (as the disturbance and Vout are sampled at a frequency of Fsw/6).  Hence, we take the ADC measurement on AN18 and convert it back to the analog domain by loading the digital value into a DAC on the dsPIC. This leads to a more accurate phase measurements at higher frequencies.

[[back to top](#start-doc)]
- - -

<span id="firmware-modifications"><a name="firmware-modifications"> </a></span>

### Firmware Modifications

Setup the firmware so that the converter is running in open loop mode, you can even remove the compensator from the code if you wish.

Configure the firmware to run in interleaved mode.

To setup the dsPIC to measure the disturbance on the AN18 pin, MCC can be used as shown below. For our measurement, we used the shared ADC core to measure both the output voltage and the disturbance.

<p>
  <center>
    <img src="images/illc-40.png" alt="plant-01" width="900">
    <br>
    Adding measurement of disturbance on AN18 with ADC using MCC.
  </center>
</p>

We use the shared ADC core to measure both the disturbance and Vout. Firstly, we measure Vout with the shared ADC core. Then the shared ADC core conversion channel is changed manually to measure the voltage on the AN18 pin. For example, the firmware below can be added to the function _ADCAN0Interrupt()_ in the file _driver/drv_adc.c_. It can be added just after the two secondary phase currents are sampled with the ADC.

<p>
  <center>
    <img src="images/illc-41.png" alt="plant-02" width="900">
    <br>
    Firmware added to ADCCAN0 interrupt for measuring both Vout and the disturbance signal.
  </center>
</p>

The measurement of the voltage on AN18 (with the 1.65V DC bias removed) is stored in _pwr_ctrl_adc_data.drv_adc_val_AN18_. Note that since the DC bias has been removed, this result can be negative or positive. This is correct as we want to control input to move in both directions.

We need to apply this disturbance to the control input.
In the function _Drv_PwrCtrl_ILLC_ILPHVoltageLoop()_, we added the following code for this purpose.  

<p>
  <center>
    <img src="images/illc-42.png" alt="plant-03" width="900">
    <br>
    Adding the disturbance to the control input.
  </center>
</p>

In the line highlighted above, the disturbance is added to the control input. Note that the disturbance at this point has been digitized and the DC bias has been removed, so this line adds the (digitized) small signal AC disturbance to the control input. It can cause the control input to both increase or decrease.

In the final line of the code snippet above, the control input is converted to a period for the PWM peripherals, as the switching period is modulated for the LLC topology.

Finally, we take the ADC measurement of the voltage on AN18 (that is, the disturbance input from bode measurement instrument) and load this digital value into DAC2 (which is connected to pin 17, and also to the test point on the top of the DP-PIM). To this end, this line should be added after the control input is updated. The output of DAC2 is measured with channel 1 of the Bode 100.

<p>
  <center>
    <img src="images/illc-44.png" alt="plant-05" width="600">
    <br>
    Routing the disturbance back out of the dsPIC on a DAC.  
  </center>
</p>

[[back to top](#start-doc)]
- - -

<span id="results"><a name="results"> </a></span>

### Results

Here we show the results at multiple operating points, running in interleaved mode. We use the potentiometer to change the switching frequency in open loop mode for these measurements. A resistive load is used.

Note that the gain and phase response moves quite a bit depending on the switching frequency and load. For an LLC converter, the plant is "dynamic" and thus pole and zero positions will move depending on the operating conditions.

For example, at 1A load, the SRs on the secondary side are disabled, this adds dynamic resistance in the conduction path and changes the measured plant frequency response. At 2A load, the SRs are enabled, meaning less dynamic resistance. This can be seen in the plots below.

Thus it is important to run these measurement at many different operating points to ensure that the compensator is designed for worst case conditions.

<p>
  <center>
    <img src="images/illc-47.png" alt="plant-00" width="900">
    <br>
    Plant measurement: Vin = 39V, Fs = 810kHz, Iload = 1A.
  </center>
</p>

<p>
  <center>
    <img src="images/illc-48.png" alt="plant-01" width="900">
    <br>
    Plant measurement: Vin = 39V, Fs = 810kHz, Iload = 2A.
  </center>
</p>

<p>
  <center>
    <img src="images/illc-49.png" alt="plant-02" width="900">
    <br>
    Plant measurement: Vin = 39V, Fs = 840kHz, Iload = 1A.
  </center>
</p>

<p>
  <center>
    <img src="images/illc-50.png" alt="plant-03" width="900">
    <br>
    Plant measurement: Vin = 39V, Fs = 840kHz, Iload = 2A.
  </center>
</p>

<p>
  <center>
    <img src="images/illc-51.png" alt="plant-04" width="900">
    <br>
    Plant measurement: Vin = 39V, Fs = 900kHz, Iload = 1A.
  </center>
</p>

<p>
  <center>
    <img src="images/illc-52.png" alt="plant-05" width="900">
    <br>
    Plant measurement: Vin = 39V, Fs = 900kHz, Iload = 2A.
  </center>
</p>

<p>
  <center>
    <img src="images/illc-53.png" alt="plant-06" width="900">
    <br>
    Plant measurement: Vin = 39V, Fs = 970kHz, Iload = 1A.
  </center>
</p>

<p>
  <center>
    <img src="images/illc-54.png" alt="plant-07" width="900">
    <br>
    Plant measurement: Vin = 39V, Fs = 970kHz, Iload = 2A.
  </center>
</p>

[[back to top](#start-doc)]
- - -

<span id="compensator-settings"><a name="compensator-settings"> </a></span>

## Compensator Settings

Since the plant frequency response is single pole system, it is sufficient to use voltage mode control to compensate for the plant, using a 2P2Z compensator.

As this is a frequency controlled converter, the control algorithm controls the PWM switching frequency to regulate the output voltage.

<p>
  <center>
    <img src="images/illc-55.png" alt="compensator-15" width="900">
    <br>
    Simplified block diagram of the control elements.
  </center>
</p>

[Digital Compensator Design Tool](https://www.microchip.com/developmenttools/ProductDetails/DCDT), abbreviated to DCDT, was used to design the 2P2Z compensator.

To download and install DCDT, please follow the instructions at the link above.

To open DCDT from MPLABx, click "Tools -> Embedded -> Digital Compensator Design Tool".

On the pop-up window that appears, click "Open", which will open the "VMC" DCDT project associated with the LLC MPLABx project.

On the next window that appears (shown below) click on the compensator block.
<p>
  <center>
    <img src="images/illc-56.png" alt="compensator-00" width="900">
    <br>
    Opening 2P2Z compensator in LLC DCDT project.
  </center>
</p>

The compensator settings in DCDT are shown below.

<p>
  <center>
    <img src="images/illc-57.png" alt="compensator-00" width="1000">
    <br>
    Compensator settings in DCDT.
  </center>
</p>

As can be seen above, for our 2P2Z compensator (which has 2 poles and 1 zero), we placed

- the pole at origin at 2kHz,
- the second pole at 200kHz,
- the zero at 1kHz.

Note that for this demo firmware, we tuned the compensator empirically.
For a 2P2Z compensator, typically the second pole is placed at high frequency (usually half of the ADC sampling frequency) to filter the effects of high frequency ripple and noise on the loop.
The zero is typically placed at a low frequency, perhaps somewhere between 200Hz and 500Hz to begin with. The purpose of this zero is to ensure that you have enough phase at the cross-over frequency.
The pole at origin is set to get as much DC gain as possible.

In this example, we started with a conservative coefficient set and moved the zero and pole at origin until our measured frequency response was deemed satisfactory.

[[back to top](#start-doc)]
- - -

<span id="open-loop-gain-measurements"><a name="open-loop-gain-measurements"> </a></span>

### Open Loop Gain Measurements

In this section we shown the open loop gain and phase response. To clarify, these are the "open loop" measurements of the closed loop system - so the measurements include the plant, the voltage feedback network and the 2P2Z compensator.

For these measurements, the Bode 100 output was connected across the 20R resistor R120, using the test points TP120 and TP121. This resistor was placed between the output terminal of the converter and the resistor divider used for output voltage sensing by the dsPIC.

<p>
  <center>
    <img src="images/illc-62.png" alt="olgain-4" width="800">
    <br>
    Measuring the open loop gain on the LLC demo board.
  </center>
</p>

Channel 2 of the Bode 100 was connected to TP120, and Channel 1 was connected to TP121.
See below for measurements taken across different load settings and input voltage settings.

Note that the crossover frequency, phase margin and slope of gain at the crossover frequency changes depending on the load and input voltage.
At 0.5A load, the SRs (and thus the current balancing scheme) are disabled, which slows down the loop response versus the measurements taken at 2.0A load.

<p>
  <center>
    <img src="images/illc-58.png" alt="olgain-0" width="900">
    <br>
    Open-loop gain/phase with Vin = 39V and Iload = 0.5A.
  </center>
</p>

<p>
  <center>
    <img src="images/illc-59.png" alt="olgain-1" width="900">
    <br>
    Open-loop gain/phase with Vin = 39V and Iload = 2.0A.
  </center>
</p>

<p>
  <center>
    <img src="images/illc-60.png" alt="olgain-2" width="900">
    <br>
    Open-loop gain/phase with Vin = 42V and Iload = 0.5A.
  </center>
</p>

<p>
  <center>
    <img src="images/illc-61.png" alt="olgain-3" width="900">
    <br>
    Open-loop gain/phase with Vin = 42V and Iload = 2.0A.
  </center>
</p>

[[back to top](#start-doc)]
- - -

<span id="phase-current-balancing"><a name="phase-current-balancing"> </a></span>

# Phase Current Balancing

When two or more identical half-bridge LLC converters are interleaved, any differences in their tank circuits will lead to unequal sharing of the load current between individual phases. Unequal load sharing is a major problem in interleaving resonant converters as it decreases thermal stability and can lead to high-circulating currents and even converter failure.

A innovative current balancing scheme has been implemented on the demo firmware accompanying this board. The goal is that both phases share the load current equally. This scheme is only run if the total load current is above 1.4A (see the macro _IOUT_SRONIL_in the firmware).
The code is located in the function_Drv_PwrCtrl_ILLC_ILPHVoltageLoop()_, which is in the file _power_controller/drv_pwrctrl_ILLC_SRandControl.c_. This function is called from the ADCAN0 interrupt, which is located in the file _driver/drv_adc.c_.

<p>
  <center>
    <img src="images/illc-18.png" alt="ILLC current balancing block dia" width="900">
    <br>
    Block diagram of LLC control loop with current balancing.
  </center>
</p>

The scheme works by changing the duty cycle of the SR drive signals on one phase until both phase currents are approximately the same, while keeping the duty cycle of the SR drives on the other phase fixed.

When the scheme is launched, the (filtered) currents of both phases are compared. The phase with the smaller current runs at a fixed (just under 50%) duty cycle, while the duty cycle of the SRs on the phase with the larger current is varied between a min value (100ns) and a max value (just under 50%) until the two currents are as close as possible.

Decreasing the duty cycle of the SR drives on one phase increases the dynamic resistance presented to the secondary current flowing through this phase. This results in less current flowing through the phase and more through the other phase. The total output current remains the same.

[[back to top](#start-doc)]
- - -

<span id="sr-drive-state-machine"><a name="sr-drive-state-machine"> </a></span>

## SR Drive State Machine

<p>
  <center>
    <img src="images/illc-01.png" alt="ILLC current balancing state machine" width="500">
    <br>
    Current balancing state machine.
  </center>
</p>
The state machine that runs the current balancing algorithm is illustrated above. It is run in the ADCAN0 interrupt, which is executed every 6th PWM period.

<br/>
<br/>

[[back to top](#start-doc)]
- - -

<span id="standby-state"><a name="standby-state"> </a></span>

### STANDBY State

Both SRs are disabled, so any conduction is through the body diodes of the SRs. We stay in this state until the output voltage is above 6V (see macro _VOUT_SR_ACTIVE_) and the total output current is above 1.4A (see macro _IOUT_SRONIL_). If both of these conditions are satisfied we go to the enable state.

<p>
  <center>
    <img src="images/illc-02.png" alt="standby-state-flowchart" width="450">
    <br>
    Flowchart for STANDBY state.
  </center>
</p>

[[back to top](#start-doc)]
- - -

<br/>
<span id="enable-state"><a name="enable-state"> </a></span>

### ENABLE State

Both SRs are still disabled. The two phase currents are compared. In our current balancing algorithm, the phase with the smaller current has a fixed SR duty cycle, and the SR duty of the other phase is varied to get the currents in balance. So the decision on which phase to fix and which to vary is made once at this point, and remains in force while the algorithm is active.

Note that the phase current measurements used for the comparison are filtered to reduce the effect of noise on the decision.

<p>
  <center>
    <img src="images/illc-03.png" alt="enable-state-flowchart" width="400">
    <br>
    Flowchart for ENABLE state.
  </center>
</p>

After the comparison is made, PWM2 and PWM4 (the PWMs used to drive the SRs for phase A and phase B respectively) are enabled and set at min duty cycle. The PWM setup is actually done in the STANDBY state, but the PWM outputs from the dsPIC are not enabled until the ENABLE state.

<p>
  <center>
    <img src="images/illc-04.png" alt="pwm2-setup" width="800">
    <br>
    PWM 2 setup.
  </center>
</p>

As discussed previously, the PWMs are configured in "Independent, dual output" mode. To recap, PG2 is used to drive the SRs for phase A, and PG4 is used to drive the SRs for phase B.

For phase A, the rising edge of PWM2H is set via the PG2TRIGA register (so the event is triggered when the internal period counter in PG2 reaches PG2TRIGA), and the falling edge set via PG2TRIGB. The rising edge of PWM2L is set by the PG2PHASE register, and the falling edge via the PG2DC register.

For phase B SR drive, PG4 is used, so PG4TRIGA, PG4TRIGB, PG4PHASE and PG4DC are used to set the rising and falling edges on PG4H and PG4L in the same way as for PG2.  

[[back to top](#start-doc)]
- - -

<br/>
<span id="softstart-state"><a name="softstart-state"> </a></span>

### SOFTSTART State

In this state, the duty cycles of the SR drives on both PG2 and PG4 are linearly ramped from the min duty (100ns pulse width) to the max duty (PGxPER/2*250ps - 124ns) in steps of 10ns.

<p>
  <center>
    <img src="images/illc-05.png" alt="enable-state-flowchart" width="800">
    <br>
    PWM2 signal during soft-start.
  </center>
</p>

The falling edges of the PWM2 and PWM4 outputs are fixed during the ramping of the duty cycle, and the rising edges are moved.
For PG2, PG2TRIGB is fixed at 50ns before the end of period, and PG2DC fixed at 50ns before the end of the half-period. For PG4, PG4TRIGB is fixed at 50ns before the end of period, and PG4DC fixed at 50ns before the half-period.  

During the soft-start ramp, PG2TRIGA, PG2PHASE, PG4TRIGA and PG4PHASE are decreased by steps equivalent to 10ns so that the duty cycle increases by 10ns each time.

The ramp is stopped when the rising edge of PG2PHASE is 74ns from the start of the period. Thus at the end of the ramp, all SR drive signals are running at just under 50% duty cycle (precisely, the on-time is [PG2PER/2*250ps] - 124ns).

At the end of the ramp, we change to the UP AND RUNNING state.

[[back to top](#start-doc)]
- - -

<br/>
<span id="up-and-running-state"><a name="up-and-running-state"> </a></span>

### UP AND RUNNING State

<p>
  <center>
    <img src="images/illc-06.png" alt="up-and-running-state-flowchart" width="1000">
    <br>
    Flowchart illustrating the UP AND RUNNING state.
  </center>
</p>

In this state, one of the SR drives of either phase A or phase B runs at a fixed duty cycle, and the other is moved. The decision on which phase to fix and which to vary is made in the ENABLE state and doesn't change thereafter.

If the output current through the phase being controlled is greater than the current through the other (fixed duty) phase, we decrease the duty cycle of the SR drives by 2ns. If the current is smaller, we increase the duty cycle on the SR drives by 2ns. The on-time of the SR drives is clamped at a max value of [PGxPER/2*250ps - 124ns], and at a min value of 100ns.

If the total output current drops below 1.0A (see macro _IOUT_SROFFIL_), all SR drives are disabled, and we go back to the STANDBY state.

[[back to top](#start-doc)]
- - -

<span id="results-1"><a name="results-1"> </a></span>

## Results

See results below. The two phase currents are out of balance until the total output current reaches 1.4A, at which point the current balancing algorithm kicks in, and the current is shared equally between the phases.

<p>
  <center>
    <img src="images/illc-16.png" alt="ibalance-0" width="1000">
    <br>
    Current Balancing.
  </center>
</p>

Below we show how it works with a load step from 0 to 3A. The time-base is 200us/div, so you can see that the currents are in balance within 1.5ms.

<p>
  <center>
    <img src="images/illc-17.png" alt="ibalance-1" width="1000">
    <br>
    Current Balancing with load step.
  </center>
</p>

[[back to top](#start-doc)]

- - -

<span id="plant-frequency-response-simulation-with-mplab-mindi"><a name="plant-frequency-response-simulation-with-mplab-mindi"> </a></span>

# Plant Frequency Response Simulation with MPLAB® Mindi™

Mindi™ is the Microchip-branded demo version of Simplis/SiMetrix. It supports the common features of the Simplis standard license but limits the number of circuit nodes.

We have included a model of the LLC converter in this deployment for educational purposes. See "mindi" folder.

<p>
  <center>
    <img src="images/illc-46.png" alt="mindi-00" width="1000">
    <br>
    Mindi Model of LLC converter.  
  </center>
</p>

[[back to top](#start-doc)]

---

&copy; 2021, Microchip Technology Inc.
