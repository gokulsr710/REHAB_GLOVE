# REHAB_GLOVE
This rehab glove is a lightweight, wearable device designed to help stroke patients regain finger and hand mobility through gentle, timed inflation–deflation cycles. At its core sits an Arduino Nano that orchestrates three main subsystems:

1. Actuation (Pump + Solenoid Valve):

   * A small air pump (or motor) pushes airflow into the glove, causing the fingers to bend into a “grip” position.
   * A solenoid valve vents the air when it’s time for the glove to relax.
   * Both pump and valve are switched on and off by a two‐channel relay module; when the relay lines go LOW, the pump and valve activate.

2. Pressure Feedback (HX710B Sensor):

   * A pressure transducer inside the glove measures internal pneumatic pressure. Its analog output is converted by the HX710B 24-bit ADC.
   * During each inflation phase, the Arduino continuously reads raw counts from the HX710B. If this value exceeds a calibrated threshold (representing a safe maximum pressure), the Arduino cuts off the pump early—preventing over-inflation and ensuring patient safety.

3. User Interface (Button + I²C LCD):

   * A single pushbutton toggles the entire therapy session on or off. The Nano uses its internal pull-up resistor so that pressing the button pulls the line to ground, which flips a software flag.
   * A 16×2 I²C LCD reports real-time status:

     * Top row shows either “Therapy: ON,” “Therapy: OFF,” or “Cycle: N.”
     * Bottom row displays “Bending…” during inflation, “Pressure HIGH!” if the sensor limit is reached, or “Relaxing…” during deflation.
   * Each complete inflate-deflate sequence increments an on-screen cycle counter, so patients or therapists can track progress.



 How a Single Cycle Unfolds

1. Start/Stop Control:

   * When powered up, the LCD reads “Rehab Glove Ready.”
   * The user (patient or caregiver) presses the button once → LCD updates to “Therapy: ON,” and the first cycle begins.

2. Inflation (“Bending”) Phase (2 seconds max)

   * The Nano drives both relay channels LOW, energizing the pump and simultaneously opening the solenoid valve’s coil circuit (but the valve body remains closed until the Arduino switches it off later).
   * As air is forced into the glove, fingers flex around an object or into a neutral gripping posture.
   * Inside this 2-second window, the Nano polls the HX710B. If raw sensor counts climb above the preset threshold, the Nano immediately de-energizes the pump, preventing excessive pressure. The LCD switches to “Pressure HIGH!” to indicate this early cutoff.

3. Deflation (“Relaxing”) Phase (3 seconds):

   * After the inflation timer expires (or the pressure limit is reached), both relay outputs go HIGH, turning off the pump and energizing the valve coil so that air vents out of the glove.
   * Fingers return to a relaxed, extended position. The LCD reads “Relaxing….”
   * This 3-second pause gives muscles a brief rest before the next cycle.

4.Cycle Count & Repeat:

   * Once the deflation timer elapses, the Nano increments a cycle counter and updates the top LCD line to “Cycle: X” (where X is the number of completed bend–relax sequences).
   * Unless the button is pressed again to stop therapy, the Nano immediately begins the next inflation phase.



### Safety and Calibration Details

* Flyback Diodes on Inductive Loads: Both the pump’s motor terminals and the solenoid valve coil have 1N4007 diodes wired across them (cathode to positive supply, anode to the side that switches to ground). These diodes clamp voltage spikes when each device is turned off, preventing high-voltage transients from damaging the Nano or relay contacts.
* Pressure Threshold Tuning: Before clinical use, you must calibrate the HX710B’s raw output. With the glove fully donned, run a quick sketch that prints raw pressure counts to the serial monitor. Have a therapist or clinician observe finger resistance and note the count corresponding to a comfortable flex. Set the code’s `MAX_PRESSURE` to just below that value so the glove stops inflating before any discomfort or risk of joint strain.
* Adjustable Timing: Although the default “bend” time is 2 seconds and “relax” time is 3 seconds, these constants can be tuned in code to match a particular patient’s rehabilitation protocol—longer holds, shorter rests, or vice versa.
* Simple On/Off Control: The single pushbutton allows the patient or caregiver to immediately halt the cycle at any point, and the LCD changes to “Therapy: OFF.” If the button is pressed mid-inflation or mid-deflation, the Nano simply stops all outputs, so there’s no risk of the glove continuing unwanted movement.

---

### Intended Benefits

* Repetitive Motion Training: By automating a consistent bend-relax cycle, the glove encourages repeated finger flexion and extension, which is critical for neural plasticity after stroke.
* Built-In Safety Net: Pressure monitoring ensures the glove never over-inflates beyond a comfortable threshold, protecting fragile joints and soft tissue.
* Immediate Feedback: The LCD informs both patient and therapist of cycle count and pressure events, allowing on-the-fly adjustments.
* Ease of Use: A single start/stop button makes the system accessible even to users with limited hand dexterity.

By integrating timed actuation, pressure sensing, and clear visual feedback, this rehab glove empowers stroke survivors to perform controlled, repetitive hand exercises at home or in a clinic—reducing dependence on manual assistance and promoting more consistent therapy sessions.
