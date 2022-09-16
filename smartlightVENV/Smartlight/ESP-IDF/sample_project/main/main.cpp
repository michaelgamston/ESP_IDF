/*
                                                                  *****FILE HEADER*****
File Name - main.ino

Author/s - Michael Gamston - Joe Button

Description - Main function's for Controll of ESP32-WROOM

Hardware - A0.3 (ESP32-WROOM, 2xESP32-CAM)

Comments - Cubik intergration throwing up errors, work on getting it work. or implement own version. 

Libraries - N/A


Repo - michaelgamston/MVP
Branch - main

*/
#include "include/spi_comms.h"
#include "include/AWS_funcs.h"
#include <Arduino.h>
#include "include/dali.h"
#include "include/MySPIFFS.h"

static const BaseType_t pro_cpu = 0;
static const BaseType_t app_cpu = 1;

static TaskHandle_t message_handler;
static TaskHandle_t spi_loop; 
static SemaphoreHandle_t mutex;


void spi(void* parameters){
   //recieve data from both peripheral's and send to aws
  for (int i = 1; i <= 2; i++){
    spi_txn(i, 8192);
    send_image(spi_buf, SPI_BUFFER_SIZE);
    set_buf();
  }
}


void app_main()
{
  delay(2000);

  if(!SPIFFS.begin())Serial.println("SPIFFS failed to mount");
  Serial.begin(115200);

  mutex = xSemaphoreCreateMutex();
  connectAWS();
  init_spi();
  delay(10); // Allow time for peripherals to power up.

  xTaskCreatePinnedToCore(
    spi,
    "spi loop",
    10240,
    NULL,
    1,
    &spi_loop,
    app_cpu
  );

  vTaskDelete(NULL);
}

// void loop()
// {
//   Serial.print("Client loop");
//   client.loop();
// }
