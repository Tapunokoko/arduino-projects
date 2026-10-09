# Arduino Projects

Arduino sketches from my electronics coursework and hands-on projects. Each folder is one sketch that opens directly in the Arduino IDE.

| Sketch | What it does | Parts used |
|---|---|---|
| [`stepper_tone`](stepper_tone/stepper_tone.ino) | "Singing stepper" tone tester for ENGR 346. Drives a stepper motor at a set step rate so the motor hums a pitch. Supports wave, full-step and half-step drive, plus an optional speed ramp, all controlled from the Serial Monitor. | Arduino Uno, ULN2003 driver, 28BYJ-48 stepper |
| [`Happy_birthday_with_piezo`](Happy_birthday_with_piezo/Happy_birthday_with_piezo.ino) | Press a button to play "Happy Birthday" on a piezo buzzer while 5 LEDs light up in time with the notes. | Piezo buzzer, 5 LEDs, push button |
| [`motor_control`](motor_control/motor_control.ino) | Turns a potentiometer reading (0–1023) into a servo angle (0–179°) using `map()`. | Servo motor, potentiometer |
| [`temperature_sensor`](temperature_sensor/temperature_sensor.ino) | Reads a TMP36 sensor, converts the ADC value to volts and then °C, and lights 0–3 LEDs as the temperature rises above a baseline. | TMP36, 3 LEDs |
| [`photo_transitor_light_sensor_project`](photo_transitor_light_sensor_project/photo_transitor_light_sensor_project.ino) | Reads three phototransistors (red/green/blue filters) and drives an RGB LED with PWM to mix the color it sees. | 3 phototransistors, RGB LED |
| [`spaceship`](spaceship/spaceship.ino) | "Spaceship interface": a green LED stays on until a button is pressed, then two red LEDs flash back and forth. | 3 LEDs, push button |

## Skills shown
- Digital and analog I/O (`digitalRead`, `analogRead`, `analogWrite` / PWM)
- Sensor signal conversion (ADC counts → voltage → temperature)
- Motor control: servo positioning and stepper coil sequencing through a driver board
- Serial Monitor command parsing and debugging output
- Timing with `delay()` and `micros()`-based step timing

## How to run
1. Open the `.ino` file in the [Arduino IDE](https://www.arduino.cc/en/software).
2. Wire the parts listed above (pin numbers are defined at the top of each sketch).
3. Select your board (Arduino Uno) and port, then click **Upload**.
