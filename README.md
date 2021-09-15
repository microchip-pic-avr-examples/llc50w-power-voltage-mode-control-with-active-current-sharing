


![image](images/microchip.png) 

--- 
# dsPIC33 Interleaved LLC Converter <!-- omit in toc --> 
### 2 phase Interleaved LLC converter with voltage mode control and phase current balancing. <!-- omit in toc --> 


<p><center><a target="_blank" href="https://www.microchip.com/en-us/development-tool/DV330102" rel="nofollow">
<img src="images/illc-00.png" alt="dsPIC33C Interleaved LLC Converter" width="400">
</a></center></p>

<p>
<center>
<a target="_blank" href="https://www.microchip.com/en-us/development-tool/DV330102" rel="nofollow">
dsPIC33 Interleaved LLC Converter
</a>
</center>
</p>

- - -

<span id="start-doc"><a name="start-doc"></a></span> 

## Table of Contents <!-- omit in toc --> 

- [How to use this document](#how-to-use-this-document)
- [Quick-start guide](#quick-start-guide)
  - [Human Machine Interface](#human-machine-interface)
  - [Testing the board in open-loop mode](#testing-the-board-in-open-loop-mode)
  - [Testing the board in closed-loop mode](#testing-the-board-in-closed-loop-mode)
- [Setting up and connecting the Board Power Visualizer GUI](#setting-up-and-connecting-the-board-power-visualizer-gui)
- [State machine](#state-machine)
  - [States PCS_SOFT_START_PRE1, PCS_SOFT_START_PRE2 and PCS_SOFT_START_PR](#states-pcs_soft_start_pre1-pcs_soft_start_pre2-and-pcs_soft_start_pr)
    - [State PCS_SOFT_START_PRE1](#state-pcs_soft_start_pre1)
    - [State PCS_SOFT_START_PRE2](#state-pcs_soft_start_pre2)
    - [State PCS_SOFT_START](#state-pcs_soft_start)
- [Summary](#summary)
- [PWM setup](#pwm-setup)
  - [PWM routing](#pwm-routing)
    - [Phase A PWM setup](#phase-a-pwm-setup)
    - [Phase B PWM setup](#phase-b-pwm-setup)
- [Control Method](#control-method)
  - [Plant Measurement](#plant-measurement)
  - [Loop Measurements](#loop-measurements)
- [Phase Current Balancing](#phase-current-balancing)
  - [SR Drive State machine](#sr-drive-state-machine)
    - [STANDBY state](#standby-state)
    - [ENABLE state](#enable-state)
    - [SOFTSTART state](#softstart-state)
    - [UP AND RUNNING state](#up-and-running-state)
  - [Results](#results)
- [Related Collateral](#related-collateral)
  - [Software Used](#software-used)
  - [Hardware Used](#hardware-used)
- [Plant Frequency Response Simulation with MPLAB® Mindi™](#plant-frequency-response-simulation-with-mplab-mindi)
- [Code structure](#code-structure)
- [Fault Protection](#fault-protection)

---

<span id="how-to-use-this-document"><a name="how-to-use-this-document"> </a></span>

## How to use this document
This document is intended as a supplement to the user's guide. We recommend that the user's guide is studied before reading this document. 
The user's guide can be found [here.](https://www.microchip.com/developmenttools/ProductDetails/PartNO/EV44M28A).

[[back to top](#start-doc)]

- - -


<span id="quickstart"><a name="quickstart"> </a></span>

## Quick-start guide

<p>
  <center>
    <img src="images/illc-23.png" alt="state-machine" width="900">
    <br>
    Hardware connections
  </center>
</p>

<span id="hmi"><a name="hmi"> </a></span>

### Human Machine Interface

The ILLC board has a simple Human-Machine-Interface (HMI) with LEDs showing status and push buttons for control. This is shown below.

<p>
  <center>
    <img src="images/illc-28.png" alt="hmi-control" width="1200">
    <br>
    HMI LEDs and push buttons on the ILLC power board
  </center>
</p>

The USER button control is outlined in the table below.

<p>
  <center>
    <img src="images/illc-24.png" alt="hmi-control" width="900">
    <br>
    HMI control
  </center>
</p>

The board status as shown by the LEDs on the ILLC power board and the DP-PIM is outlined below.

<p>
  <center>
    <img src="images/illc-25.png" alt="hmi-status" width="900">
    <br>
    HMI status 
  </center>
</p>

<span id="open-loop-test"><a name="open-loop-test"> </a></span>

### Testing the board in open-loop mode

Note that we use the ICD4 in-circuit debugger to program the dsPIC, but any type of Microchip In-Circuit Debugger can be used here.

1. Connect computer to DP-PIM via USB cable
2. Connect computer to ICD4, connect ICD4 to 6 pin header on DP-PIM
3. Connect poti to 6 pin header on LLC power board as shown above
4. Set poti wiper to max setting
5. Connect input terminal of LLC power board to DC source capable of providing at least 40V. Leave DC source off for now.
6. Connect scope probe to TP100 (this is phase A high-side primary drive signal, ensuring to ground the scope at GND_S (not GND_P).
7. Connect a voltmeter to the output terminals of the LLC power board.
8. Connect output terminals of LLC power board to resistor or constant current load (set to around 0.5A or 20R), turn on the load if it is an e-load
9. Open illc project in MPLABx. Set as main project, then download the firmware to the dsPIC on the DP-PIM 
10. When the firmware is running, you should see 
   * the red LED on the DP-PIM slow blinking, indicating that the firmware is running
   *  the green LED on the power board slow blinking, indicating that the power supply is not running
   *  the red LED on the power board is on, indicating that a fault is active
11. Turn on the DC source, setting it to 40V. Set current limit to 1.2A or lower.
12. Short press the user button on the LLC power board. This turns on the converter. The status LED should be as follows
    * Green LED on power board on, indicating that the power supply is running
    * Red LED on power board slow-blinking, indicating that open-loop mode is active
13. Check the PWM signal on TP100 with a scope. It should have a frequency of around 1MHz, and a duty cycle of around 45%. 
14. Check the output voltage, with 0.5A load, it should be around 7.4V.
15. Move the poti wiper towards the min setting, observe the signal on TP100 and the output voltage, you should see the frequency of the signal on TP100 decreasing, the duty staying (approximately) the same, and the output voltage increasing. 
16. With the poti wiper at the min setting, the signal on TP100 should have a frequency of 800kHz and a duty cycle of 45%. With a 0.5A load, the output voltage should be around 10.7V.

Note that at power-up, the firmware checks if a potentiometer is connected, and if so, uses the voltage on the potentiometer wiper for the output voltage reference. If a potentiometer is not connected, the output voltage reference is set at 9V. 

<span id="closed-loop-test"><a name="closed-loop-test"> </a></span>

### Testing the board in closed-loop mode

1. Turn off the power supply if it is running by short pressing the USER button.
2.  Press and hold the USER button for at least two seconds to change the working mode from open loop to closed loop. The red LED on the power board should turn off.
3.  Short press the USER button to enable the power supply.
4.  Check for valid output voltage and switching signal on TP100.
5.  If the potentiometer is connected, moving the potentiometer wiper position will change the output voltage reference.
6.  If the potentiometer is not used, the output voltage should be around 9V (TP100 signal should have frequency of around 870kHz and duty of 45%).
7.  Using the board in closed-Loop mode creates a controlled stable output voltage. It must be independent from input voltage or load changes. The reference for the output voltage can be set with a potentiometer and/or with the GUI. We discuss using the Board Power Visualizer GUI in the next section.

[[back to top](#start-doc)]

- - - 

<span id="bpv-gui"><a name="bpv-gui"> </a></span>

## Setting up and connecting the Board Power Visualizer GUI

1.  Ensure that the firmware is running on the DP-PIM and that the DP-PIM is connected to your computer via a USB cable.
2.  Open the board power visualizer application on your computer.
3.  On the window that appears, click "Open Project", then navigate to the "gui" folder in the ILLC firmware installation directory, and click on "ILLC_R1.xml". 
4.  The ILLC landing page will open. Click on the "COM?" button on the bottom of this page.
5.  Select the port which is used for the serial connection between your computer and the DP-PIM from the pull-down menu. Click "OK" and you will be brought back to the ILLC GUI landing page.
6.  Click on the "Enable" button on the bottom of the landing page. This will initiate comms between the GUI and the firmware running on the DP-PIM.
7.  When the comms is working, the "Communication status" indicator on the bottom of the LLC GUI landing page should flash alternating red and green.

With Microchips Power Board Visualizer GUI we can easily visualize values and states visible on the ILLC Demo Board. We can also change the output voltage reference (closed loop) and switching frequency (open loop) on the running power supply with the GUI.
On the Status tab we have an example showing the main values like voltages and currents as well as status bits from the power controller and Faults.

<p>
  <center>
    <img src="images/illc-26.png" alt="board-power-visualizer-00" width="1500">
    <br>
    Board Power Visualizer Landing Page
  </center>
</p>

On the Schematic diagram tab there is the power supply block diagram with online updates of the most important values for easy understanding the working principles. This is shown below.

<p>
  <center>
    <img src="images/illc-27.png" alt="board-power-visualizer-01" width="1500">
    <br>
    Board Power Visualizer Landing Page
  </center>
</p>


[[back to top](#start-doc)]

- - - 

<span id="state-machine"><a name="state-machine"> </a></span>

## State machine

The main power controller state machine is shown below. It is run every 100us. Most of the states are pretty standard. Perhaps the only states worth describing in detail are the soft-start states. Hence we describe these below.

<p>
  <center>
    <img src="images/illc-19.png" alt="state-machine" width="500">
    <br>
    Power supply state machine
  </center>
</p>

<span id="soft-start-states"><a name="soft-start-states"> </a></span>

### States PCS_SOFT_START_PRE1, PCS_SOFT_START_PRE2 and PCS_SOFT_START_PR

The soft-start ramp is split in 2 parts
1. Open loop, fixed frequency of 1MHz (max frequency for our design), ramp duty cycle in steps of 2.5ns every 100us until duty reaches 50%.
2. Closed loop, fix duty cycle at 50%, ramp control loop input from pre-bias output voltage to target output voltage setpoint.

This first part is to prevent massive inrush current: this may happen if the duty cycle was set immediately to 50%. 

Note that the SRs drives (PWM2 for phase A and PWM4 for phase B) are switched off during soft-start, so any output current conduction is through the body diodes of the SRs during this time.

Also note that if we are running in interleaved mode, PG3 setup is identical to PG1, but PG3 lags PG1 by 90 degrees. 

<br>

<span id="soft-start-pre1"><a name="soft-start-pre1"> </a></span>

#### State PCS_SOFT_START_PRE1

In this state, the PWMs are running open loop (that is, they are not driven by the compensator).
The frequency is fixed at 1MHz. The on-time of the primary side half-bridge drive signals is set to 50ns. 
Then every 100us, the on-time (and hence duty cycle) is increase by 2.5ns.
This continues until the duty cycle is 45% (as we allow for a dead-time of 50ns).

<p>
  <center>
    <img src="images/illc-20.png" alt="ss-pre1-00" width="1000">
    <br>
    Half bridge high and low switching signals at power-on
  </center>
</p>

<p>
  <center>
    <img src="images/illc-21.png" alt="ss-pre1-01" width="1000">
    <br>
    Half bridge high and low switching signals during open-loop duty cycle ramp, with fixed Fsw = 1MHz
  </center>
</p>

Once we reach the target on-time, we move to the state PCS_SOFT_START_PRE2.

<br>

<span id="soft-start-pre2"><a name="soft-start-pre2"> </a></span>

#### State PCS_SOFT_START_PRE2

In this state, we enable frequency modulation of the PWM outputs by the voltage mode compensator. The duty cycle of the PWM drive signals is fixed at (PG1PER/2 - 50ns). The voltage loop reference is initialized based on the measured output voltage at this point. 

<br>

<span id="soft-start"><a name="soft-start"> </a></span>

#### State PCS_SOFT_START

In this state, the reference to the voltage loop compensator is ramped linearly to the target setpoint. The compensator controls the frequency of the primary side LLC half-bridge signals. The duty cycle is always set to (PG1PER/2 - 50ns).

Note that in this state the SRs are enabled.

A oscilloscope screenshot of the entire start up phase is shown above. The different parts are described also.

<p>
  <center>
    <img src="images/illc-22.png" alt="ss-00" width="1300">
    <br>
    Soft-start with different stages highlighted and described
  </center>
</p>


[[back to top](#start-doc)]


- - -
<span id="summary"><a name="summary"> </a></span>

## Summary

This solution demonstrates the implementation of an interleaved (2 phase) LLC converter using voltage mode control on the 50W Interleaved LLC Converter Development Board.

The 50W Interleaved LLC Converter Development Board is a generic development 
board for this topology that supports rapid prototyping and code development based on dsPIC33 devices. The board provides two identical half-bridge stages with LLC tank circuitry at the primary and voltage doubler rectification at the secondary. The board offers well organized building blocks that include an input filter, power stage, AUX supply, mating socket for Microchip’s newest Digital Power Plug-In Modules (DP PIMs), Human Machine Interface (HMI) and test points.
The electrical characteristics are prepared to allow safe voltage levels of up to 50 VDC in and up to 12 VDC out. Topology and design are scalable and can be easily turned into real industrial demands targeting 400 VDC or 800 VDC bus operating voltage. A mating socket for dsPIC33 plug-in modules allows the system to be evaluated with different controllers. The pinout is compatible for EP, CK and CH dsPIC® DSC DP PIMs. A Human-Machine-Interface (HMI) and test points allow for easy evaluation and debugging.




[[back to top](#start-doc)]

- - -

<span id="pwm-setup"><a name="pwm-setup"> </a></span>

## PWM setup

<span id="pwm-routing"><a name="pwm-routing"> </a></span>

### PWM routing
The two schematics below show the routing of the PWM signals for phase A and phase B. PWM1 and PWM3 output are used for the primary drives for phase A and phase B respectively, while PWM2 and PWM4 are used for the SR drives.

The dsPIC33C is on the secondary side, so PWM1H, PWM1L, PWM3H and PWM3L have to pass through the isolation barrier. FET drivers are not shown here, please see the schematic on the users guide for more detail.

<p>
  <center>
    <img src="images/illc-07.png" alt="PWM phase A" width="700">
    <br>
    PWM routing for phase A
  </center>
</p>


<p>
  <center>
    <img src="images/illc-08.png" alt="PWM phase B" width="700">
    <br>
    PWM routing for phase B
  </center>
</p>

The switching frequency range of our LLC solution is from 800kHz and 1Mhz. To achieve robust operation in this frequency range with a PWM resolution of 250ps, we needed some special PWM module configuration, which will be describe in the following sections.

<span id="phase-a-pwm"><a name="phase-a-pwm"> </a></span>

#### Phase A PWM setup

For a single phase, the LLC primary drive signals should have a fixed duty cycle (just below 50%) and variable frequency, as shown below. The drive signals to the high side and low side of the primary side half-bridge (before the resonant tank) need to be complementary, with a dead time between the falling edge on the high side drive and the rising edge of the low side drive, and visa-versa. 

In our example firmware, the frequency varies from 800kHz to 1MHz.

To get this type of waveform from the PWM module, we configured the PWM peripherals in "Independent Edge, dual output mode". 

In this mode of operation, each PWM edge is set independently via a register. Specifically,
* PWM1H rising edge occurs when PWM1 counter = PG1PHASE.
* PWM1H falling edge occurs when PWM1 counter = PG1DC.
* PW1L rising edge occurs when PWM1 counter = PG1TRIGA.
* PWM1L falling edge occurs when PWM1 counter = PG1TRIGB.

The primary drive signals for phase A (from PWM1 peripheral) are shown below.

<p>
  <center>
    <img src="images/illc-09.png" alt="PWM1 setup" width="1100">
    <br>
    PWM1 setup
  </center>
</p>

<p>
  <center>
    <img src="images/illc-10.png" alt="PWM1 config" width="400">
    <br>
    PWM mode: independent edge, dual output
  </center>
</p>

The SR drive signals for phase A come from the PWM2 module. PWM2H drives the SR on the high side of the half-bridge on the secondary side, and PWM2L drives the SR on the low-side. PWM2 is also configured in "independent edge, dual output" mode, but the SWAP bit is set, so that PWM2H and PWM2L are swapped.
The PWM2H and PWM2L setup is illustrated below.

<p>
  <center>
    <img src="images/illc-11.png" alt="PWM2 setup" width="1100">
    <br>
    PWM2 setup
  </center>
</p>

<span id="phase-b-pwm"><a name="phase-b-pwm"> </a></span>

#### Phase B PWM setup

Phase B setup as follows:

* PWM3H drives primary side half-bridge, high side
* PWM3L drives primary side half-bridge, low side
* PWM4H drives secondary side (SR) half-bridge, high side
* PWM4L drives secondary side (SR) half-bridge, low side

Phase A and phase B run 90 degrees out of phase. 
The synchronization scheme works as follows
* PWM2 is synchronized to the EOC (End of Cycle) trigger of PWM1, so they run in phase
* PWM3 is synchronized to PG2TRIGC via the PWM2 peripheral's PCI input and the PWM event A output. PG2TRIGC is set to (PG1PER / 4 ), so the PWM3 cycle starts 1/4 of a period after PWM1 and PWM2 cycle start.
* PWM4 is synchronized to the EOC (End of Cycle) trigger of PWM3, so they run in phase.


<p>
  <center>
    <img src="images/illc-15.png" alt="PWM2 setup" width="1100">
    <br>
    phase A and phase B sync
  </center>
</p>

The ADC trigger for PWM1 is set to occur every 6th PWM1 period. It is set using PG1TRIGC (as PG1TRIGA and PG1TRIGB are already in use), and in the running firmware is set to be at the middle of the PG1H on-time so as to sample the average current.

The ADC trigger for PWM3 is set to occur every 6th PWM3 period. It is set using PG3TRIGC, and in the running firmware is set to be at the middle of the PG3H on-time so as to sample the average current.

The "easy setup" view for all 4 PWM modules is shown below.

<p>
  <center>
    <img src="images/illc-12.png" alt="PWM config MCC" width="1500">
    <br>
    PWM configuration in MCC (easy setup)
  </center>
</p>

Some registers in the "registers" view also need to be modified. The fields that need to be modified for phase A (related to PWM1 and PWM2) are highlighted below.

<p>
  <center>
    <img src="images/illc-13.png" alt="phase A config MCC" width="1500">
    <br>
    Phase A configuration in MCC (additional register settings)
  </center>
</p>

The fields that need to be modified for phase B (related to PWM3 and PWM4) are highlighted below.

<p>
  <center>
    <img src="images/illc-14.png" alt="phase B config MCC" width="1500">
    <br>
    Phase B configuration in MCC (additional register settings)
  </center>
</p>


[[back to top](#start-doc)]

- - -

<span id="control-method"><a name="control-method"> </a></span>

## Control Method

Uses voltage mode control.

<p>
  <center>
    <img src="images/illc-18.png" alt="ILLC current balancing block dia" width="900">
    <br>
    Block diagram of LLC control loop with current balancing 
  </center>
</p>


<span id="plant-measurement"><a name="plant-measurement"> </a></span>

### Plant Measurement

Discuss how to do this on DP-PIM

[[back to top](#start-doc)]

---

<span id="loop-measurements"><a name="loop-measurements"> </a></span>

### Loop Measurements


[[back to top](#start-doc)]

---


<span id="current-balancing"><a name="current-balancing"> </a></span>

## Phase Current Balancing

A current balancing scheme has been implemented on the demo firmware accompanying this board. The goal is that both phases share the load current equally. This scheme is only run if the total load current is above 1.4A (see the macro "IOUT_SRONIL" in the firmware), so the thermal stress is shared equally between the phases when the load is high enough that thermal management is warranted.

<p>
  <center>
    <img src="images/illc-18.png" alt="ILLC current balancing block dia" width="900">
    <br>
    Block diagram of LLC control loop with current balancing 
  </center>
</p>

The scheme works by changing the duty cycle of the SR drive signals on one phase until the currents are approximately the same, while keeping the duty cycle of the SR drives on the other phase fixed. When the scheme is launched, the (filtered) currents of both phases are compared. The phase with the smaller current runs at a fixed (just under 50%) duty cycle, while the duty cycle of the SRs on the phase with the larger current is varied between a min value (100ns) and a max value (just under 50%) until the two currents are as close as possible.

Decreasing the duty cycle of the SR drives on a phase will decrease the amount of current flowing through this phase and increase the amount of current flowing through the other phase. The total output current remains the same.

<span id="sr-drive-state-machine"><a name="sr-drive-state-machine"> </a></span>

### SR Drive State machine

<p>
  <center>
    <img src="images/illc-01.png" alt="ILLC current balancing state machine" width="500">
    <br>
    Current balancing state machine
  </center>
</p>
The state machine that runs the current balancing algorithm is illustrated above. It is run in the ADCAN0 interrupt, which is executed every 6th PWM period. 

<br/>
<br/>

<span id="standby-state"><a name="standby-state"> </a></span>

#### STANDBY state
Both SRs are disabled, so any conduction is through the body diodes of the SRs. We stay in this state until the output voltage is above 6V (see macro "VOUT_SR_ACTIVE") and the total output current is above 1.4A (see macro "IOUT_SRONIL"). If both of these conditions are satisfied we go to the enable state. 

<p>
  <center>
    <img src="images/illc-02.png" alt="standby-state-flowchart" width="450">
    <br>
    Flowchart for STANDBY state
  </center>
</p>

<br/>
<span id="enable-state"><a name="enable-state"> </a></span>

#### ENABLE state

Both SRs are still disabled. The two phase currents are compared. In our current balancing algorithm, the phase with the smaller current has a fixed SR duty cycle, and the SR duty of the other phase is varied to get the currents in balance. So the decision on which phase to fix and which to vary is made once at this point, and remains in force while the algorithm is active.

Note that the phase current measurements used for the comparison are filtered to reduce the effect of noise on the decision.


<p>
  <center>
    <img src="images/illc-03.png" alt="enable-state-flowchart" width="400">
    <br>
    Flowchart for ENABLE state
  </center>
</p>

After the comparison is made, PWM2 and PWM4 (the PWMs used to drive the SRs for phase A and phase B respectively) are enabled and set at min duty cycle. The PWM setup is actually done in the STANDBY state, but the PWM outputs from the dsPIC are not enabled until the ENABLE state.

<p>
  <center>
    <img src="images/illc-04.png" alt="pwm2-setup" width="800">
    <br>
    PWM 2 setup
  </center>
</p>

As discussed in section XXX, the PWMs are configured in "Independent, dual output" mode. To recap, PG2 is used to drive the SRs for phase A, and PG4 is used to drive the SRs for phase B. 

For phase A, the rising edge of PWM2H is set via the PG2TRIGA register (so the event is triggered when the internal period counter in PG2 reaches PG2TRIGA), and the falling edge set via PG2TRIGB. The rising edge of PWM2L is set by the PG2PHASE register, and the falling edge via the PG2DC register. 

For phase B SR drive, PG4 is used, so PG4TRIGA, PG4TRIGB, PG4PHASE and PG4DC are used to set the rising and falling edges on PG4H and PG4L in the same way as for PG2. 

<br/>
<span id="softstart-state"><a name="softstart-state"> </a></span>

#### SOFTSTART state

In this state, the duty cycles of the SR drives on both PG2 and PG4 are linearly ramped from the min duty (100ns pulse width) to the max duty (PGxPER/2*250ps - 124ns) in steps of 10ns.

<p>
  <center>
    <img src="images/illc-05.png" alt="enable-state-flowchart" width="800">
    <br>
    PWM 2 setup
  </center>
</p>

The falling edges of the PWM2 and PWM4 outputs are fixed during the ramping of the duty cycle, and the rising edges are moved.
For PG2, PG2TRIGB is fixed at 50ns before the end of period, and PG2DC fixed at 50ns before the end of the half-period. For PG4, PG4TRIGB is fixed at 50ns before the end of period, and PG4DC fixed at 50ns before the half-period.  

During the soft-start ramp, PG2TRIGA, PG2PHASE, PG4TRIGA and PG4PHASE are decreased by steps equivalent to 10ns so that the duty cycle increases by 10ns each time.

The ramp is stopped when the rising edge of PG2PHASE is 74ns from the start of the period. Thus at the end of the ramp, all SR drive signals are running at just under 50% duty cycle (precisely, this is [PG2PER/2*250ps] - 124ns).

At the end of the ramp, we change to the UP AND RUNNING state.

<br/>
<span id="up-and-running-state"><a name="up-and-running-state"> </a></span>

#### UP AND RUNNING state

<p>
  <center>
    <img src="images/illc-06.png" alt="up-and-running-state-flowchart" width="1000">
    <br>
    Flowchart illustrating the UP AND RUNNING state
  </center>
</p>

In this state, one of the SR drives of either phase A or phase B runs at a fixed duty cycle, and the other is moved. The decision on which phase to fix and which to vary is made in the ENABLE state and doesn't change thereafter. 

If the output current through the phase being controlled is greater than the current through the other (fixed duty) phase, we decrease the duty cycle of the SR drives by 2ns. If the current is smaller, we increase the duty cycle on the SR drives by 2ns. The on-time of the SR drives is clamped at a max value of [PGxPER/2*250ps - 124ns], and at a min value of 100ns.

If the total output current drops below 1.0A (see macro "IOUT_SROFFIL"), all SR drives are disabled, and we go back to the STANDBY state.

### Results
See results below. The two phase currents are out of balance until the total output current reaches 1.4A, at which point the current balancing algorithm kicks in, and the current is shared equally between the phases.


<p>
  <center>
    <img src="images/illc-16.png" alt="ibalance-0" width="1500">
    <br>
    Current Balancing
  </center>
</p>

Below we show how it works with a load step from 0 to 3A. The time-base is 200us/div, so you can see that the currents are in balance within 1.5ms.

<p>
  <center>
    <img src="images/illc-17.png" alt="ibalance-1" width="1500">
    <br>
    Current Balancing with load step
  </center>
</p>


[[back to top](#start-doc)]

- - -


<span id="related-collateral"><a name="related-collateral"> </a></span>


## Related Collateral

The related documentation can be found on the appropriate product website
- [Interleaved LLC Development Board](https://www.microchip.com/en-us/development-tool/DV330102)
- [dsPIC33CK256MP508 Family Data Sheet](https://www.microchip.com/70005349)
- [dsPIC33CK256MP508 Family Silicon Errata and Data Sheet Clarification](https://www.microchip.com/80000796)

Please always check for the latest data sheets on the respective product websites:
- [dsPIC33CK256MP508 Family](https://www.microchip.com/dsPIC33CK256MP508)
- [dsPIC33CH512MP508 Family](https://www.microchip.com/dsPIC33CH512MP508)

<span id="software-used"><a name="software-used"> </a></span>

### Software Used
- [Power Board Visualizer GUI](https://www.microchip.com/SWLibraryWeb/product.aspx?product=POWER_BOARD_VISUALIZER)
- [MPLAB&reg; X IDE v5.45](https://www.microchip.com/mplabx-ide-windows-installer)
- [MPLAB&reg; XC16 Compiler v1.61](https://www.microchip.com/mplabxc16windows)
- [Microchip Code Configurator v4.0.2](https://www.microchip.com/mplab/mplab-code-configurator)
- [Digital Compensator Design Tool](https://www.microchip.com/developmenttools/ProductDetails/DCDT)
- [MPLAB&reg; Mindi™ Simulator](https://www.microchip.com/SWLibraryWeb/producttc.aspx?product=AnalogSimMPLABMindi)


<span id="hardware-used"><a name="hardware-used"> </a></span>

### Hardware Used
- Interleaved LLC Development Board, Part-No. [DV330102](https://www.microchip.com/en-us/development-tool/DV330102)
- dsPIC33CK256MP506 Digital Power PIM, Part-No. [MA330048](https://www.microchip.com/MA330048)

[[back to top](#start-doc)]

---

<span id="plant-frequency-response-simulation-with-mplab-mindi"><a name="plant-frequency-response-simulation-with-mplab-mindi"> </a></span>

## Plant Frequency Response Simulation with MPLAB® Mindi™
Mindi™ is the Microchip-branded demo version of Simplis/SiMetrix. It supports the common features of the Simplis standard license but limits the number of circuit nodes. 

[[back to top](#start-doc)]

---

<span id="code-structure"><a name="code-structure"> </a></span>

## Code structure

[[back to top](#start-doc)]

---

<span id="fault-protection"><a name="fault-protection"> </a></span>

## Fault Protection

Vdd: over voltage and under voltage
Vout: over voltage
Input over current for phase A
Input over current  for phase B
Output over current for phase A
Output over current for phase B
Auxillary out of range

[[back to top](#start-doc)]

---

&copy; 2021, Microchip Technology Inc.
