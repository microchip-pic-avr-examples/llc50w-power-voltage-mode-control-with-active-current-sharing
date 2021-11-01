


![image](images/microchip.png) 

---
# dsPIC33 Interleaved LLC Converter <!-- omit in toc --> 
### 2 phase Interleaved LLC converter with voltage mode control and phase current balancing. <!-- omit in toc --> 


<p><center><a target="_blank" href="https://www.microchip.com/en-us/development-tool/DV330102" rel="nofollow">
<img src="images/illc-00.png" alt="dsPIC33C Interleaved LLC Converter" width="700">
</a></center></p>

<p>
<center>
<a target="_blank" href="https://www.microchip.com/en-us/development-tool/DV330102" rel="nofollow">
dsPIC33 Interleaved LLC Converter Power Board
</a>
</center>
</p>

- - -



<span id="start-doc"><a name="start-doc"></a></span> 

## Table of Contents <!-- omit in toc --> 

- [__Summary__](#summary)
  - [__Highlights__](#highlights)
- [__Related Collateral__](#related-collateral)
  - [__Software Tools Used__](#software-tools-used)
  - [__Hardware Used__](#hardware-used)
- [__How to Use This Document__](#how-to-use-this-document)
- [__Quick Start Guide__](#quick-start-guide)
  - [__Human Machine Interface__](#human-machine-interface)
  - [__Testing the Board in Open-Loop Mode__](#testing-the-board-in-open-loop-mode)
  - [__Testing the Board in Closed-Loop Mode__](#testing-the-board-in-closed-loop-mode)
  - [__Setting Up and Connecting the Power Board Visualizer GUI__](#setting-up-and-connecting-the-power-board-visualizer-gui)
- [__Firmware Overview__](#firmware-overview)
- [__Firmware Deep Dive__](#firmware-deep-dive)
  - [__LLC Modes of Operation: Single Phase or Interleaved__](#llc-modes-of-operation-single-phase-or-interleaved)
  - [__Converter State Machine__](#converter-state-machine)
    - [__Soft-Starting the Converter__](#soft-starting-the-converter)
      - [__State PCS_SOFT_START_PRE1__](#state-pcs_soft_start_pre1)
      - [__State PCS_SOFT_START_PRE2__](#state-pcs_soft_start_pre2)
      - [__State PCS_SOFT_START__](#state-pcs_soft_start)
  - [__Fault Protection__](#fault-protection)
    - [__Firmware Fault Protection__](#firmware-fault-protection)
    - [__Hardware fault protection__](#hardware-fault-protection)
  - [__PWM Setup__](#pwm-setup)
    - [__PWM Routing__](#pwm-routing)
      - [__Phase A PWM Setup__](#phase-a-pwm-setup)
      - [__Phase B PWM Setup__](#phase-b-pwm-setup)
  - [__Control Method__](#control-method)
    - [__Plant Measurement__](#plant-measurement)
      - [__Firmware Modifications__](#firmware-modifications)
      - [__Results__](#results)
    - [__Loop Measurements__](#loop-measurements)
  - [__Phase Current Balancing__](#phase-current-balancing)
    - [__SR Drive State Machine__](#sr-drive-state-machine)
      - [__STANDBY State__](#standby-state)
      - [__ENABLE State__](#enable-state)
      - [__SOFTSTART State__](#softstart-state)
      - [__UP AND RUNNING State__](#up-and-running-state)
    - [__Results__](#results-1)
  - [__Plant Frequency Response Simulation with MPLAB® Mindi™__](#plant-frequency-response-simulation-with-mplab-mindi)

---
<span id="summary"><a name="summary"> </a></span>

## __Summary__

This solution demonstrates the implementation of an interleaved (2 phase) LLC converter using voltage mode control on the 50W Interleaved LLC Converter Development Board.

The 50W Interleaved LLC Converter Development Board is a generic development 
board for this topology that supports rapid prototyping and code development based on dsPIC33 devices. The board provides two identical half-bridge stages with LLC tank circuitry at the primary and voltage doubler rectification at the secondary. The board offers well organized building blocks that include an input filter, power stage, AUX supply, mating socket for Microchip’s newest Digital Power Plug-In Modules (DP PIMs), Human Machine Interface (HMI) and test points.
The electrical characteristics are prepared to allow safe voltage levels of up to 50 VDC in and up to 12 VDC out. Topology and design are scalable and can be easily turned into real industrial demands targeting 400 VDC or 800 VDC bus operating voltage. A mating socket for dsPIC33 plug-in modules allows the system to be evaluated with different controllers. The pinout is compatible for EP, CK and CH dsPIC® DSC DP PIMs. A Human-Machine-Interface (HMI) and test points allow for easy evaluation and debugging.

<span id="summary"><a name="summary"> </a></span>

### __Highlights__

What is special about this that can't easily be done in analog? What is the USP?
Showcases
* current sharing
* interleaved mode


[[back to top](#start-doc)]

- - -

<span id="related-collateral"><a name="related-collateral"> </a></span>


## __Related Collateral__

The related documentation can be found on the appropriate product website
- [Interleaved LLC Development Board](https://www.microchip.com/en-us/development-tool/DV330102)
- [dsPIC33CK256MP508 Family Data Sheet](https://www.microchip.com/70005349)
- [dsPIC33CK256MP508 Family Silicon Errata and Data Sheet Clarification](https://www.microchip.com/80000796)

Please always check for the latest data sheets on the respective product websites:
- [dsPIC33CK256MP508 Family](https://www.microchip.com/dsPIC33CK256MP508)
- [dsPIC33CH512MP508 Family](https://www.microchip.com/dsPIC33CH512MP508)

<span id="software-used"><a name="software-used"> </a></span>

### __Software Tools Used__
- [Power Board Visualizer GUI](https://www.microchip.com/SWLibraryWeb/product.aspx?product=POWER_BOARD_VISUALIZER)
- [MPLAB&reg; X IDE v5.50](https://www.microchip.com/mplabx-ide-windows-installer)
- [MPLAB&reg; XC16 Compiler v1.70](https://www.microchip.com/mplabxc16windows)
- [Microchip Code Configurator v4.2.1](https://www.microchip.com/mplab/mplab-code-configurator)
- [Digital Compensator Design Tool](https://www.microchip.com/developmenttools/ProductDetails/DCDT)
- [MPLAB&reg; Mindi™ Simulator](https://www.microchip.com/SWLibraryWeb/producttc.aspx?product=AnalogSimMPLABMindi)


<span id="hardware-used"><a name="hardware-used"> </a></span>

### __Hardware Used__
- Interleaved LLC Development Board, Part-No. [DV330102](https://www.microchip.com/en-us/development-tool/DV330102)
- dsPIC33CK256MP506 Digital Power PIM, Part-No. [MA330048](https://www.microchip.com/MA330048)

<span id="how-to-use-this-document"><a name="how-to-use-this-document"> </a></span>

[[back to top](#start-doc)]

- - -

## __How to Use This Document__
This document is intended as a supplement to the user's guide. We recommend that the user's guide is studied before reading this document. 
The user's guide can be found [here.](https://www.microchip.com/en-us/development-tool/DV330102).

[[back to top](#start-doc)]

- - -

<span id="quickstart"><a name="quickstart"> </a></span>

## __Quick Start Guide__

In this section we describe how to bring up the board both in open loop and closed loop modes, and with and without using the Power Board Visualizer GUI.

<span id="hmi"><a name="hmi"> </a></span>

### __Human Machine Interface__

The interleaved LLC board has a simple Human-Machine-Interface (HMI) with LEDs showing status, and push buttons for control. This is shown below.

<p>
  <center>
    <img src="images/illc-28.png" alt="hmi-control" width="1200">
    <br>
    HMI LEDs and push buttons on the ILLC power board
  </center>
</p>

The USER button control action is outlined in the table below.

<p>
  <center>
    <img src="images/illc-24.png" alt="hmi-control" width="900">
    <br>
    HMI control USER push button action
  </center>
</p>

The board status as shown by the LEDs on the ILLC power board and the DP-PIM is outlined below.

<p>
  <center>
    <img src="images/illc-25.png" alt="hmi-status" width="900">
    <br>
    HMI status LEDs
  </center>
</p>

<span id="open-loop-test"><a name="open-loop-test"> </a></span>

### __Testing the Board in Open-Loop Mode__
The hardware connections for running the board in open loop mode is shown below.

<p>
  <center>
    <img src="images/illc-23.png" alt="state-machine" width="900">
    <br>
    Hardware connections
  </center>
</p>

Note that we use the ICD4 in-circuit debugger to program the dsPIC, but any type of Microchip In-Circuit Debugger can be used here.

Please follow these steps to run the ILLC board in open loop mode.

1. Connect computer directly to DP-PIM via USB cable (USB Micro Type-B on DP-PIM).
2. Connect computer to ICD4 via USB cable, connect ICD4 to 6 pin header on DP-PIM via RJ11 cable and RJ11 to ICSP adapter.
3. Connect poti (5kR or 10kR) to 6 pin header on LLC power board as shown above.
4. Set poti wiper to max setting.
5. Connect input terminal of LLC power board to DC source capable of providing at least 40V. Leave DC source off for now.
6. Connect scope probe to TP100 (this is phase A high-side primary drive signal) ensuring to ground the scope at GND_S (not GND_P).
7. Connect a voltmeter to the output terminals of the LLC power board.
8. Connect output terminals of LLC power board to resistive or constant current load (set current load to around 0.5A or resistive load to around 20R), turn on the load if it is an e-load.
9. Open ILLC project in MPLABx. Set as the main project, then download the firmware to the dsPIC on the DP-PIM.
10. When the firmware is running, you should see 
   * the red LED on the DP-PIM slow blinking, indicating that the firmware is running.
   *  the green LED on the power board slow blinking, indicating that the power supply is not running.
   *  the red LED on the power board is on, indicating that a fault is active.
11. Set the DC source to 40V. Set current limit to 1.2A or lower. Turn on DC source.
12. Short press the USER button on the LLC power board. This turns on the converter. The status LEDs should behave as follows:
    * Green LED on power board constant on, indicating that the power supply is running.
    * Red LED on power board slow-blinking, indicating that open-loop mode is active.
13. Check the PWM signal on TP100 with an oscilloscope. The signal should have a frequency of around 1MHz, and a duty cycle of 45%.
14. Check the output voltage with the voltmeter: with 0.5A load, it should be around 7.4V.
15. Move the poti wiper slowly towards the min setting. While doing this, observe the signal on TP100 and the output voltage, you should see the frequency of the signal on TP100 decreasing, the duty staying (approximately) the same, and the output voltage increasing. 
16. With the poti wiper at the min setting, the signal on TP100 should have a frequency of 800kHz and a duty cycle of 45%. With a 0.5A load, the output voltage should be around 10.7V.

Note that at power-up, the firmware checks if a potentiometer is connected, and if so, the voltage on the potentiometer wiper sets the output voltage reference. If a potentiometer is not connected, the output voltage reference is fixed at 9V. The firmware will not detect that the poti is present if the poti is connected after power up, so you need to reset the dsPIC if this is the case.

<span id="closed-loop-test"><a name="closed-loop-test"> </a></span>

### __Testing the Board in Closed-Loop Mode__

1. Turn off the power supply if it is running by short pressing the USER button.
2.  Press and hold the USER button for at least two seconds to change the working mode from open loop to closed loop. The red LED on the power board should turn off.
3.  Short press the USER button to enable the power supply.
4.  Check for valid output voltage and switching signal on TP100.
5.  If the potentiometer is connected, moving the potentiometer wiper position will change the output voltage reference.
6.  If the potentiometer is not used, the output voltage should be around 9V (TP100 signal should have frequency of around 870kHz and duty of 45%).
7.  Using the board in closed-Loop mode creates a controlled stable output voltage. It must be independent from input voltage or load changes. The reference for the output voltage can be set with a potentiometer and/or with the Power Board Visualizer GUI. 
  
  We discuss using the Power Board Visualizer GUI in the next section.

[[back to top](#start-doc)]

- - - 

<span id="bpv-gui"><a name="bpv-gui"> </a></span>

### __Setting Up and Connecting the Power Board Visualizer GUI__

1.  Ensure that the firmware is running on the DP-PIM and that the DP-PIM is connected to your computer via a USB cable.
2.  Open the Power Board Visualizer application on your computer.
3.  On the window that appears, click "Open Project", then navigate to the "gui" folder in the ILLC firmware installation directory, and click on "ILLC_R1.xml". 
4.  The ILLC landing page will open. Click on the "COM?" button on the bottom of this page.
5.  Select the port which is used for the serial connection between your computer and the DP-PIM from the pull-down menu. Click "OK" and you will be brought back to the ILLC GUI landing page.
6.  Click on the "Enable" button on the bottom of the landing page. This will initiate comms between the GUI and the firmware running on the DP-PIM.
7.  When the comms is working, the "Communication status" indicator on the bottom of the LLC GUI landing page should flash alternating red and green.

With Microchips Power Board Visualizer GUI we can easily visualize values and states visible on the ILLC Demo Board. We can also change the output voltage reference (closed loop only) and switching frequency (open loop only) on the running power supply with the GUI.

On the "Status" tab we show an example below with the main values like voltages and currents as well as status bits from the power controller and Faults.

<p>
  <center>
    <img src="images/illc-26.png" alt="board-power-visualizer-00" width="1500">
    <br>
    Power Board Visualizer Landing Page
  </center>
</p>

On the "Schematic diagram" tab there is the power supply block diagram with online updates of the most important values for easy understanding the working principles. This is shown below.

<p>
  <center>
    <img src="images/illc-27.png" alt="board-power-visualizer-01" width="1500">
    <br>
    Power Board Visualizer Landing Page
  </center>
</p>


[[back to top](#start-doc)]


- - -

<span id="firmware-overview"><a name="firmware-overview"> </a></span>

## __Firmware Overview__

An overview of the firmware is shown below. 
The power controller state machine and fault handler are executed every 100us by the scheduler. So also are the GUI handler (every 1ms), and the HMI driver (every 100ms).

There are 5 interrupt sources.
* ISRADCCAN0: executed every 6th phase A PWM cycle. Measure phase A secondary current and Vout every pass, then cycle through sampling other ADC channels on the shared ADC core. Voltage loop is executed here, as well as SR state machine.
* ISRADCCAN1: executed every 6th phase B PWM cycle. Measure phase B secondary current.
* ISRSCCP1: triggered every 4th input capture event (SCCP peripheral), used to measure Vdd via a PWM signal from primary side whose duty cycle is proportional to the Vdd level.
* ISRCMP1: triggered if comparator 1 trips, which happens if there is a large output over current event on phase A.
* ISRCMP3: triggered if comparator 3 trips, which happens if there is a large output over current event on phase B.

<p>
  <center>
    <img src="images/illc-34.png" alt="firmware-0" width="1500">
    <br>
    Firmware overview
  </center>
</p>

[[back to top](#start-doc)]

Microchip Code Configurator (MCC) is used to configure the peripherals. They are configured at run-time at the start of _main()_, before the background loop is initiated.

The main files are as follows:
* _driver/drv_adc.c_: this contains the 5 interrupt service routines.
* _power_controller/drv_pwrctrl_ILLC.c_: power controller state machine that is executed every 100us.
* _power_controller/drv_pwrctrl_ILLC_fault.c_: fault handlers.
* _power_controller/drv_pwrctrl_ILLC_ILPH_SRandControl.c_: power supply voltage loop and SR state machine and driver functions when running in interleaved mode.
* _power_controller/drv_pwrctrl_ILLC_PHA_SRandControl.c_: contains power supply voltage loop and SR state machine and driver functions when running phase A only.
* _misc/fault_common.c_: generic fault functions.

<p>
  <center>
    <img src="images/illc-36.png" alt="firmware-0" width="1500">
    <br>
    Firmware block diagram
  </center>
</p>

<p>
  <center>
    <img src="images/illc-35.png" alt="firmware-0" width="1500">
    <br>
    Firmware block details
  </center>
</p>

[[back to top](#start-doc)]

- - - 

<span id="firmware-deep-dive"><a name="firmware-deep-dive"> </a></span>

## __Firmware Deep Dive__

We now will go into more detail on certain parts of the firmware project that we deemed important and/or difficult to understand.

- - - 

<span id="modes-of-operation"><a name="modes-of-operation"> </a></span>

### __LLC Modes of Operation: Single Phase or Interleaved__

The LLC power board contains two phases, phase A and phase B. The firmware can be configured to run just a single phase (phase A), or run in interleaved mode (phase A and phase B both running).

To configure the mode of operation, open the header file "project_settings.h".

<p>
  <center>
    <img src="images/illc-37.png" alt="project-settings" width="400">
    <br>
    Location of project_settings.h
  </center>
</p>

Scroll down to line 58 (see below).

To run in interleaved mode, POWER_STAGE_CONFIG_INTERLEAVED needs to be defined and POWER_STAGE_CONFIG_PHASE_A_ONLY should not be defined.

To run in phase A only, POWER_STAGE_CONFIG_PHASE_A_ONLY needs to be defined and POWER_STAGE_CONFIG_INTERLEAVED  should not be defined.

If neither or both are defined, you will get a compile error.

<p>
  <center>
    <img src="images/illc-38.png" alt="project-settings" width="1200">
    <br>
    Configuration #defines
  </center>
</p>

- - -


<span id="state-machine"><a name="state-machine"> </a></span>

### __Converter State Machine__

The main power controller state machine is illustrated below. It is executed every 100us. The code is located in _power_controller/drv_pwrctrl_ILLC.c_, see the function _Drv_PwrCtrl_ILLC_Task_100us()_. Most of the states are pretty standard for a digital DC/DC converter state machine (see state diagram below). Perhaps the only states worth describing in detail are the soft-start states, as these differ somewhat from other DC/DC converter state machines. Hence these are described in some detail below.

<p>
  <center>
    <img src="images/illc-19.png" alt="state-machine" width="500">
    <br>
    Power supply state machine
  </center>
</p>

<span id="soft-start-states"><a name="soft-start-states"> </a></span>

#### __Soft-Starting the Converter__

The soft-start ramp is split in 2 parts
1. Run primary drive PWMs open-loop, at a fixed frequency of 1MHz (max frequency for our design)
  - start at the minimum duty cycle (Ton = 50ns).
  - ramp the duty cycle linearly in steps of 2.5ns every 100us until the duty reaches 45%.
2. Run primary-drive PWMs closed-loop
  - fixed duty cycle of 45%. 
  - compensator sets PWM frequency.
  - ramp compensator reference linearly from pre-bias output voltage to target output voltage set-point.

Part 1 of the soft-start ramp is to prevent massive inrush current: if the duty cycle was set immediately to 50% the inrush current required to charge the output capacitor would trip the hardware over current protection. Thus part 1 is a duty cycle soft-start.

Note that the SRs drives (PWM2 for phase A and PWM4 for phase B) are switched off during soft-start, so any output current conduction is through the body diodes of the SRs during this time.

Also note that if we are running in interleaved mode, PG3 setup is identical to PG1, but PG3 lags PG1 by 90 degrees. 


<span id="soft-start-pre1"><a name="soft-start-pre1"> </a></span>

##### __State PCS_SOFT_START_PRE1__

In this state, the PWMs are running in open-loop mode (that is, they are not driven by the compensator).
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

Once we reach the target primary drive on-time (equivalent to 45% duty cycle), we move to the state PCS_SOFT_START_PRE2.

<span id="soft-start-pre2"><a name="soft-start-pre2"> </a></span>

##### __State PCS_SOFT_START_PRE2__

In this state, we enable frequency modulation of the PWM outputs by the voltage mode compensator (closed-loop mode of operation). The on-time of the primary side PWM drive signals is fixed at (PG1PER/2 - 50ns). The voltage loop reference is initialized based on the measured output voltage at this point. Once we complete this initialization, we move to the state PCS_SOFT_START_.

<span id="soft-start"><a name="soft-start"> </a></span>

##### __State PCS_SOFT_START__

In this state, the reference to the voltage loop compensator is ramped linearly to the target set-point. The compensator controls the frequency of the primary side LLC half-bridge signals. The duty cycle is always set to (PG1PER/2 - 50ns).

Note that in this state the SRs are enabled.

A oscilloscope screenshot of the entire start up phase is shown below. The different parts are described also.

<p>
  <center>
    <img src="images/illc-22.png" alt="ss-00" width="1300">
    <br>
    Soft-start with different stages highlighted and described
  </center>
</p>


[[back to top](#start-doc)]

- - -

<span id="fault-protection"><a name="fault-protection"> </a></span>

### __Fault Protection__

The fault protection code is executed every 100us at the start of the converter state machine in the function _Drv_PwrCtrl_ILLC_Fault_Check()_. The body of the fault code is located in the files _power_controller/drv_pwr_ctrl_ILLC_fault.c_ and _misc/fault_common.c_.

There are two types of protection:
1. Firmware fault protection
2. Hardware fault protection

The firmware fault protection is implemented on the dsPIC on the DP-PIM. The hardware fault protection is implemented on the LLC power board. It's purpose is to prevent catastrophic board damage, particularly due to input and output over current events.

<span id="firmware-fault-protection"><a name="firmware-fault-protection"> </a></span>

#### __Firmware Fault Protection__

All of our firmware fault protection has the same functionality. Each fault has a trigger threshold, a clear threshold, a fault blanking time and a fault clear time.

This is illustrated below, for a fault with a "max" threshold, which means that the fault is triggered when the fault source is above a threshold (output over voltage protection, for example). 

Once the fault source breaches the trigger threshold, a timer is started. If the fault source stays above the trigger threshold for longer than the fault blanking time, then the fault becomes active, which means that the PWMs are switched off and the converter state machine is set to the "FAULT ACTIVE" state. 

If the fault source drops back below the trigger threshold before the fault blanking time has expired, the timer is reset.  

When the fault is active, if the fault source stays below the fault clear threshold for the duration of the fault clear time, then the fault is cleared. When all fault sources are cleared, the converter will attempt to restart.

<p>
  <center>
    <img src="images/illc-29.png" alt="fault-protection" width="1500">
    <br>
    Firmware Fault protection
  </center>
</p>

This is shown in more detail in a flowchart below. When "fault active == true", then the fault is active and the converter is disabled. When "fault active == false" the converter can attempt to start up.

<p>
  <center>
    <img src="images/illc-30.png" alt="fault-protection" width="1500">
    <br>
    Flowchart illustrating the firmware fault protection
  </center>
</p>

All faults shown in the table below have firmware protection like this. In our firmware, this fault protection is run every 100us.

<p>
  <center>
    <img src="images/illc-31.png" alt="fault-protection" width="1500">
    <br>
    ILLC faults with firmware protection
  </center>
</p>

<span id="hardware-fault-protection"><a name="hardware-fault-protection"> </a></span>

#### __Hardware fault protection__

The purpose of the hardware fault protection is to prevent catastrophic board damage, particularly from input or output over current.
Once triggered, it kicks in immediately (there is no fault blanking time). It sets all PWM drive signals to 0, which will turn off the converter. Note that this is completely independent of the dsPIC, so even if there are drive signals coming from the dsPIC when the hardware fault protection is tripped, the hardware protection will over-ride these signals (through AND gates on the hardware) before they get to the FET drivers. 

<p>
  <center>
    <img src="images/illc-32.png" alt="fault-protection" width="400">
    <br>
    ILLC faults with hardware protection
  </center>
</p>

If the hardware fault protection is triggered, the red LED LD700 will turn on. The protection is latched, meaning that once triggered it will not clear itself, it needs to be manually cleared.

If you want to re-run the board, you need to 
* disable all PWMs first, either by holding down the RESET push button, or erasing the dsPIC firmware (we recommend the second option as it is safer)
* then short press the "RESET protection" push button on the HMI interface. 

On the dsPIC, output over current protection using comparators and DACs is also implemented as follows:
* Current transformer phase A secondary sense tied to CMP1DAC (pin 22 of dsPIC)
* Current transformer phase B secondary sense tied to CMP3DAC (pin 18 of dsPIC)

Either of these comparators tripping will trigger the highest priority interrupt, which disables all PWM drive signals and puts the converter in the "FAULT ACTIVE" state.
Like the hardware fault protection, this fault protection is also latched, meaning that the dsPIC needs to be reset to restart the converter. If this fault protection is triggered, the RESET flag in the Power Board Visualizer GUI will be set, as shown below, indicating that the dsPIC needs to be reset to re-start the LLC converter.

<p>
  <center>
    <img src="images/illc-33.png" alt="fault-protection" width="250">
    <br>
    ILLC faults with hardware protection
  </center>
</p>


[[back to top](#start-doc)]

- - -

<span id="pwm-setup"><a name="pwm-setup"> </a></span>

### __PWM Setup__

Most of the PWM setup is done by calling initialization functions generated by MCC at the top of _main()_. Some more custom configuration is also done at runtime as required.

<span id="pwm-routing"><a name="pwm-routing"> </a></span>

#### __PWM Routing__
The two simplified schematics below show the routing of the PWM signals for phase A and phase B. PWM1 and PWM3 output are used for the primary drives for phase A and phase B respectively, while PWM2 and PWM4 are used for the SR drives.

The dsPIC is on the secondary side, so PWM1H, PWM1L, PWM3H and PWM3L have to pass through the isolation barrier. FET drivers are not shown here, please see the full schematic in the users guide for more detail.

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

The switching frequency range of our LLC solution is from 800kHz and 1MHz. To achieve robust operation in this frequency range with a PWM resolution of 250ps, we needed some special PWM module configuration, which will be describe in the following sections.

<span id="phase-a-pwm"><a name="phase-a-pwm"> </a></span>

##### __Phase A PWM Setup__

For a single phase, the LLC primary drive signals should have a fixed duty cycle (50% minus some dead time) and variable frequency, as shown below. The voltage loop compensator output modulates the switching frequency of the converter. The drive signals to the high side and low side of the primary side half-bridge (before the resonant tank) need to be complementary, with a dead time between the falling edge on the high side drive and the rising edge of the low side drive, and visa-versa. 

In our example firmware, the frequency can cary from 800kHz (max output voltage) to 1MHz (min output voltage).

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
    <img src="images/illc-10.png" alt="PWM1 config" width="600">
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

##### __Phase B PWM Setup__

Phase B setup as follows:

* PWM3H drives primary side half-bridge, high side
* PWM3L drives primary side half-bridge, low side
* PWM4H drives secondary side (SR) half-bridge, high side
* PWM4L drives secondary side (SR) half-bridge, low side

Phase A and phase B run 90 degrees out of phase. 
The synchronization scheme works as follows:
* PWM2 is synchronized to the EOC (End of Cycle) trigger of PWM1, so they run in phase.
* PWM3 is synchronized to PG2TRIGC via the PWM2 peripheral's PCI input and the PWM event A output. PG2TRIGC is set to (PG1PER / 4 ), so the PWM3 cycle starts 1/4 of a period after PWM1 and PWM2 cycle start.
* PWM4 is synchronized to the EOC (End of Cycle) trigger of PWM3, so PWM3 and PWM4 run in phase.
  

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

### __Control Method__

Uses voltage mode control. Elaborate here!!

<p>
  <center>
    <img src="images/illc-18.png" alt="ILLC current balancing block dia" width="900">
    <br>
    Block diagram of LLC control loop with current balancing 
  </center>
</p>


<span id="plant-measurement"><a name="plant-measurement"> </a></span>

#### __Plant Measurement__

It can be useful to measure the open loop frequency response of the plant, to allow the compensator to be designed appropriately.
In this section we describe a way to do this. 

Firstly, please read section 1.4 of [[MA330048]](https://www.microchip.com/MA330048). 
This describes how to measure the loop-gain of the plant using the DP-PIM and a vector network analyzer such as the Bode 100.

For this design, we made some small modifications to this procedure, see below.

<p>
  <center>
    <img src="images/illc-43.png" alt="plant-04" width="600">
    <br>
    Plant measurement setup using DP-PIM.
  </center>
</p>

In the diagram above, circuitry inside the dsPIC is designated by an orange colour. Blue blocks are measurement circuitry (from the Bode 100 in our case). The plant is shown in green. The Bode 100 generator output (which creates a small AC sinusoid that is swept over frequency as part of the open loop frequency response measurement) is connected across TP1 and TP2 of the DP-PIM. This signal then passes through an operational amplifier circuit on the DP-PIM.
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

This means that channel 1 of the Bode 100 should be connected as close as possible to where the control input is disturbed, as this is the signal that is "disturbing" the control input. Channel 2 of the Bode 100 should be connected to the output of the converter.

Channel 1 could be connected directly to the AN18 pin. In this case, the plant gain measurement would be accurate, but the phase response would not be accurate at higher frequencies because of the ADC sampling delay (as the disturbance and Vout are sampled at a frequency of Fsw/7).  Hence, we take the ADC measurement on AN18 and convert it back to the analog domain by loading the digital value into a DAC on the dsPIC. This leads to a more accurate phase measurements at higher frequencies.

<span id="plant-firmware"><a name="plant-firmware"> </a></span>

##### __Firmware Modifications__

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
    Adding the disturbance to the control input
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

<span id="plant-results"><a name="plant-results"> </a></span>

##### __Results__

Here we show the results at a single operating point: Vin = 39V, Fsw = 840kHz, Iout = 1A, running in interleaved mode. Note that it is important to run this measurement at different operating points to ensure that the compensator is desginered for worst case conditions.

<p>
  <center>
    <img src="images/illc-39.png" alt="plant-00" width="900">
    <br>
    Plant measurement.
  </center>
</p>

* Why 840kHz?
* ask Milan to relate hardware to poles and zeros
* response at different operating points
* SRs on or off?

[[back to top](#start-doc)]

---

<span id="loop-measurements"><a name="loop-measurements"> </a></span>

#### __Loop Measurements__


[[back to top](#start-doc)]

---


<span id="current-balancing"><a name="current-balancing"> </a></span>

### __Phase Current Balancing__

A current balancing scheme has been implemented on the demo firmware accompanying this board. The goal is that both phases share the load current equally. This scheme is only run if the total load current is above 1.4A (see the macro _IOUT_SRONIL_ in the firmware). 
The code is located in the function _Drv_PwrCtrl_ILLC_ILPHVoltageLoop()_, which is in the file _power_controller/drv_pwrctrl_ILLC_SRandControl.c_. This function is called from the ADCAN0 interrupt, which is located in the file _driver/drv_adc.c_. 

<p>
  <center>
    <img src="images/illc-18.png" alt="ILLC current balancing block dia" width="900">
    <br>
    Block diagram of LLC control loop with current balancing 
  </center>
</p>

The scheme works by changing the duty cycle of the SR drive signals on one phase until both phase currents are approximately the same, while keeping the duty cycle of the SR drives on the other phase fixed. 

When the scheme is launched, the (filtered) currents of both phases are compared. The phase with the smaller current runs at a fixed (just under 50%) duty cycle, while the duty cycle of the SRs on the phase with the larger current is varied between a min value (100ns) and a max value (just under 50%) until the two currents are as close as possible.

Decreasing the duty cycle of the SR drives on one phase increases the dynamic resistance presented to the secondary current flowing through this phase. This results in less current flowing through the phase and more through the other phase. The total output current remains the same.

<span id="sr-drive-state-machine"><a name="sr-drive-state-machine"> </a></span>

#### __SR Drive State Machine__

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

##### __STANDBY State__
Both SRs are disabled, so any conduction is through the body diodes of the SRs. We stay in this state until the output voltage is above 6V (see macro _VOUT_SR_ACTIVE_) and the total output current is above 1.4A (see macro _IOUT_SRONIL_). If both of these conditions are satisfied we go to the enable state. 

<p>
  <center>
    <img src="images/illc-02.png" alt="standby-state-flowchart" width="450">
    <br>
    Flowchart for STANDBY state 
  </center>
</p>

<br/>
<span id="enable-state"><a name="enable-state"> </a></span>

##### __ENABLE State__

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

As discussed previously, the PWMs are configured in "Independent, dual output" mode. To recap, PG2 is used to drive the SRs for phase A, and PG4 is used to drive the SRs for phase B. 

For phase A, the rising edge of PWM2H is set via the PG2TRIGA register (so the event is triggered when the internal period counter in PG2 reaches PG2TRIGA), and the falling edge set via PG2TRIGB. The rising edge of PWM2L is set by the PG2PHASE register, and the falling edge via the PG2DC register. 

For phase B SR drive, PG4 is used, so PG4TRIGA, PG4TRIGB, PG4PHASE and PG4DC are used to set the rising and falling edges on PG4H and PG4L in the same way as for PG2.  

<br/>
<span id="softstart-state"><a name="softstart-state"> </a></span>

##### __SOFTSTART State__

In this state, the duty cycles of the SR drives on both PG2 and PG4 are linearly ramped from the min duty (100ns pulse width) to the max duty (PGxPER/2*250ps - 124ns) in steps of 10ns. 

<p>
  <center>
    <img src="images/illc-05.png" alt="enable-state-flowchart" width="800">
    <br>
    PWM2 signal during soft-start
  </center>
</p>

The falling edges of the PWM2 and PWM4 outputs are fixed during the ramping of the duty cycle, and the rising edges are moved.
For PG2, PG2TRIGB is fixed at 50ns before the end of period, and PG2DC fixed at 50ns before the end of the half-period. For PG4, PG4TRIGB is fixed at 50ns before the end of period, and PG4DC fixed at 50ns before the half-period.  

During the soft-start ramp, PG2TRIGA, PG2PHASE, PG4TRIGA and PG4PHASE are decreased by steps equivalent to 10ns so that the duty cycle increases by 10ns each time.

The ramp is stopped when the rising edge of PG2PHASE is 74ns from the start of the period. Thus at the end of the ramp, all SR drive signals are running at just under 50% duty cycle (precisely, the on-time is [PG2PER/2*250ps] - 124ns).

At the end of the ramp, we change to the UP AND RUNNING state.

<br/>
<span id="up-and-running-state"><a name="up-and-running-state"> </a></span>

##### __UP AND RUNNING State__

<p>
  <center>
    <img src="images/illc-06.png" alt="up-and-running-state-flowchart" width="1000">
    <br>
    Flowchart illustrating the UP AND RUNNING state 
  </center>
</p>

In this state, one of the SR drives of either phase A or phase B runs at a fixed duty cycle, and the other is moved. The decision on which phase to fix and which to vary is made in the ENABLE state and doesn't change thereafter. 

If the output current through the phase being controlled is greater than the current through the other (fixed duty) phase, we decrease the duty cycle of the SR drives by 2ns. If the current is smaller, we increase the duty cycle on the SR drives by 2ns. The on-time of the SR drives is clamped at a max value of [PGxPER/2*250ps - 124ns], and at a min value of 100ns.

If the total output current drops below 1.0A (see macro _IOUT_SROFFIL_), all SR drives are disabled, and we go back to the STANDBY state.

#### __Results__
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
    Current Balancing with load step.
  </center>
</p>


[[back to top](#start-doc)] 

- - -


<span id="plant-frequency-response-simulation-with-mplab-mindi"><a name="plant-frequency-response-simulation-with-mplab-mindi"> </a></span>

### __Plant Frequency Response Simulation with MPLAB® Mindi™__
Mindi™ is the Microchip-branded demo version of Simplis/SiMetrix. It supports the common features of the Simplis standard license but limits the number of circuit nodes. 

We hav inlcuded a model of the LLC converter in this deployment for educational purposes. See "mindi" folder.

<p>
  <center>
    <img src="images/illc-46.png" alt="mindi-00" width="1500">
    <br>
    Mindi Model of LLC converter.
  </center>
</p>

[[back to top](#start-doc)]

---

&copy; 2021, Microchip Technology Inc.
 