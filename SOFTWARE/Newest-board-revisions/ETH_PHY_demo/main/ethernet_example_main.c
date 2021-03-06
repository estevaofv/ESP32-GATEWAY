/* ethernet Example

   This example code is in the Public Domain (or CC0 licensed, at your option.)

   Unless required by applicable law or agreed to in writing, this
   software is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
   CONDITIONS OF ANY KIND, either express or implied.

   2018 March 26 - this example is based on the one for ESP32-EVB:
   2017 June 15 rudi ;-)
   change log
   edit ETH PHY Example for ESP32-GATEWAY REV B IoT LAN8710 Board with CAN
   - Board use LAN8710 without OSC Power Enable PIN
   - - TLK110 was removed
   - - Kconfig menuconfig TLK110 entry removed cause not need here
   - - we use LAN8720 config for the LAN8710 ( compatible )
   - - Power PIN was not used in shematic and board so not used in code
   - -


*/
#include <stdio.h>
#include <string.h>

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#include "esp_system.h"
#include "esp_err.h"
#include "esp_event_loop.h"
#include "esp_event.h"
#include "esp_attr.h"
#include "esp_log.h"
#include "esp_eth.h"

#include "rom/ets_sys.h"
#include "rom/gpio.h"

#include "soc/dport_reg.h"
#include "soc/io_mux_reg.h"
#include "soc/rtc_cntl_reg.h"
#include "soc/gpio_reg.h"
#include "soc/gpio_sig_map.h"

#include "tcpip_adapter.h"
#include "nvs_flash.h"
#include "driver/gpio.h"

#include "eth_phy/phy_lan8720.h"
#define DEFAULT_ETHERNET_PHY_CONFIG phy_lan8720_default_ethernet_config

static const char *TAG = "Olimex_ESP32_GATEWAY_REV_B_eth_example";

// not need here in this Board Configuration
// #define PIN_PHY_POWER CONFIG_PHY_POWER_PIN

#define PIN_SMI_MDC   23
#define PIN_SMI_MDIO  18
#define PIN_PHY_POWER 5

static void phy_device_power_enable_via_gpio(bool enable)
{
	if (!enable)
		phy_lan8720_default_ethernet_config.phy_power_enable(false);

	gpio_pad_select_gpio(PIN_PHY_POWER);
	gpio_set_direction(PIN_PHY_POWER,GPIO_MODE_OUTPUT);
	gpio_set_level(PIN_PHY_POWER, (int)enable);

	// Allow the power up/down to take effect, min 300us
	vTaskDelay(1);

	if (enable)
		phy_lan8720_default_ethernet_config.phy_power_enable(true);
}

static void eth_gpio_config_rmii(void)
{
    // RMII data pins are fixed:
    // TXD0 = GPIO19
    // TXD1 = GPIO22
    // TX_EN = GPIO21
    // RXD0 = GPIO25
    // RXD1 = GPIO26
    // CLK == GPIO0
    phy_rmii_configure_data_interface_pins();
    // MDC is GPIO 23, MDIO is GPIO 18
    phy_rmii_smi_configure_pins(PIN_SMI_MDC, PIN_SMI_MDIO);
}
void eth_task(void *pvParameter)
{
    tcpip_adapter_ip_info_t ip;
    memset(&ip, 0, sizeof(tcpip_adapter_ip_info_t));
    vTaskDelay(2000 / portTICK_PERIOD_MS);

    while (1) {

        vTaskDelay(2000 / portTICK_PERIOD_MS);

        if (tcpip_adapter_get_ip_info(ESP_IF_ETH, &ip) == 0) {
        	ESP_LOGI(TAG, "~~~~~~~~~~~");
            ESP_LOGI(TAG, "ETHIP:"IPSTR, IP2STR(&ip.ip));
            ESP_LOGI(TAG, "ETHPMASK:"IPSTR, IP2STR(&ip.netmask));
            ESP_LOGI(TAG, "ETHPGW:"IPSTR, IP2STR(&ip.gw));
            ESP_LOGI(TAG, "~~~~~~~~~~~");
        }
    }
}

uint8_t initEthernet()
{
	eth_config_t config = phy_lan8720_default_ethernet_config;
	esp_err_t ret = ESP_OK;

	/* Initialize adapter */
	tcpip_adapter_init();
	esp_event_loop_init(NULL, NULL);

	/* Set the PHY address in the example configuration */
	config.phy_addr = 0;
	config.gpio_config = eth_gpio_config_rmii;
	config.tcpip_input = tcpip_adapter_eth_input;
	config.phy_power_enable = phy_device_power_enable_via_gpio;

	/* Chanege clock mode */
	config.clock_mode = ETH_CLOCK_GPIO17_OUT;

	/* Initialize ethernet */
	ret = esp_eth_init(&config);
	if(ret != ESP_OK)
		return ret;

	ret = esp_eth_enable();
	vTaskDelay(100 / portTICK_PERIOD_MS);
	return ret;

}


void app_main()
{
    tcpip_adapter_ip_info_t ip;
    uint8_t ret;

    /* Clear ip info */
    memset(&ip, 0, sizeof(tcpip_adapter_ip_info_t));

    ret = initEthernet();
    
    if(ret == ESP_OK) {
         xTaskCreate(eth_task, "eth_task", 2048, NULL, (tskIDLE_PRIORITY + 2), NULL);
    }

    while(1) vTaskDelay(100);
	
}
