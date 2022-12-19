
#include "otafw.h"


static const char *TAG = "ota";


void Otafw::Init(const char* _url, const char* _cert)
{
    url=_url;
    cert=_cert;
}

void Otafw::Check()
{
    esp_http_client_config_t config = {
        .url = url,
        .cert_pem = (char *)cert,
    };
    esp_https_ota_config_t ota_config = {
        .http_config = &config,
    };
    esp_err_t ret = esp_https_ota(&ota_config);
    if (ret == ESP_OK) {
        esp_restart();
    } else {
        ESP_LOGI(TAG, "Otafw::Check err=%s",esp_err_to_name(ret));
    }
}

inline void otaSetup() {

    param.load("otaurl",&otaurl);
    if(otaurl) 
    {
        otafw.Init(otaurl,(const char*)ca_crt_start);
        xTaskCreate(&Ota, "ota_task", 8192, NULL, 5, NULL);
        ESP_LOGI(TAG,"Ota started");
    }
}
