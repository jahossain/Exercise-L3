ESP32-S3 FreeRTOS Lab
Serial Monitor Check

Uploaded a minimal sketch to test serial output.
At 115200 baud, output shows:

https://github.com/jahossain/Exercise-L3/blob/main/Screenshot%202025-09-22%20202407.png





Experiment Results

Priority swap (Serial = 1, Compute = 2):
The serial output timing becomes less regular (more jitter) because the compute task can delay the serial task.

Heavier CPU load (inner loop 200000):
LED still blinks at the same rhythm. Reason: TaskLED has the highest priority, so it always runs on time even if CPU load increases.

TaskLED with period = 200 ms, ON time = 150 ms:
Duty cycle = 150/200 = 75%.
If ON time gets close to the period, the LED stays ON almost continuously, and timing margins for OFF shrink, causing the blink pattern to blur.
