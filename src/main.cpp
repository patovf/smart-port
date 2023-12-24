#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <esp_log.h>

#include "driver/mpu6050.hpp"

static void mpu6050_task(void *pvParameters)
{
    MPU6050 mpu(GPIO_NUM_22, GPIO_NUM_21, I2C_NUM_0);

    if(!mpu.init()) 
    {
        ESP_LOGE("mpu6050", "falla al iniciar");
        vTaskDelete(0);
    }

    ESP_LOGI("mpu6050", "iniciado con exito");

    float ax, ay, az, gx, gy, gz;

    uint32_t lasttime = 0;
    int count = 0;

    while(1) 
    {
        ax = -mpu.getAccX();
        ay = -mpu.getAccY();
        az = -mpu.getAccZ();

        gx = mpu.getGyroX();
        gy = mpu.getGyroY();
        gz = mpu.getGyroZ();

        count++;

        if(esp_log_timestamp() / 1000 != lasttime)
        {
            lasttime = esp_log_timestamp() / 1000;
            ESP_LOGI("mpu6050", "cantidad: %d", count);
            count = 0;
            ESP_LOGI("mpu6050", "Acc: (%.3f, %.3f, %.3f)", ax, ay, az);
            ESP_LOGI("mpu6050", "Gyro: (%.3f, %.3f, %.3f)", gx, gy, gz);
        }
    }
}

extern "C" void app_main()
{
    xTaskCreatePinnedToCore(&mpu6050_task, "mpu6050_task", 2048, NULL, 5, NULL, 0);
}