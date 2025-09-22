// ESP32-S3 FreeRTOS demo: LED + Serial + Compute
// Board: ESP32-S3-DevKitC-1 v1.1  
// Baud: 115200
// Library required: Adafruit NeoPixel

#include <Arduino.h>
#include <Adafruit_NeoPixel.h>


#define LED_PIN     38      // ESP32-S3-DevKitC-1 onboard RGB data pin
#define NUM_LEDS    1
#define LED_BRIGHT  120     // 0..255

Adafruit_NeoPixel strip(NUM_LEDS, LED_PIN, NEO_GRB + NEO_KHZ800);


TaskHandle_t hLED = nullptr, hSER = nullptr, hCPU = nullptr;


void TaskLED(void *pv) {
  (void)pv;
  TickType_t last = xTaskGetTickCount();
  const TickType_t period = pdMS_TO_TICKS(400);  // 400 ms
  const TickType_t onTime = pdMS_TO_TICKS(150);  // LED ON window

  uint32_t colors[] = {
    Adafruit_NeoPixel::Color(255, 0, 0),   // Red
    Adafruit_NeoPixel::Color(0, 255, 0),   // Green
    Adafruit_NeoPixel::Color(0, 0, 255),   // Blue
    0                                      // Off
  };
  int i = 0;

  for (;;) {
    // ON
    strip.setPixelColor(0, colors[i]);
    strip.show();
    vTaskDelay(onTime);

    // OFF until next fixed period boundary
    strip.setPixelColor(0, 0);
    strip.show();
    vTaskDelayUntil(&last, period);

    i = (i + 1) % 4;  // R -> G -> B -> Off
  }
}


void TaskSerial(void *pv) {
  (void)pv;
  TickType_t last = xTaskGetTickCount();
  const TickType_t period = pdMS_TO_TICKS(1000); // 1 s

  for (;;) {
    size_t freeHeap = xPortGetFreeHeapSize();
    Serial.printf("[t=%lu ms] freeHeap=%u bytes\n",
                  (unsigned long)millis(),
                  (unsigned int)freeHeap);
    vTaskDelayUntil(&last, period);
  }
}


void TaskCompute(void *pv) {
  (void)pv;
  TickType_t last = xTaskGetTickCount();
  const TickType_t period = pdMS_TO_TICKS(200);  // 200 ms
  volatile uint32_t sink = 0;

  for (;;) {
    // Light CPU load
    for (int k = 0; k < 50000; ++k) {
      sink += (k ^ ((k << 1) + (k >> 1)));
    }
    vTaskDelayUntil(&last, period);
  }
}

void setup() {
  Serial.begin(115200);
  delay(200);
  Serial.println();
  Serial.println("ESP32-S3 FreeRTOS demo (LED + Serial + Compute)");

  
  strip.begin();
  strip.setBrightness(LED_BRIGHT);
  strip.clear();
  strip.show();

 
  xTaskCreate(TaskLED,    "TaskLED",    2048, NULL, 3, &hLED);
  xTaskCreate(TaskSerial, "TaskSerial", 3072, NULL, 2, &hSER);
  xTaskCreate(TaskCompute,"TaskCompute",2048, NULL, 1, &hCPU);
}

void loop() {
 
}
