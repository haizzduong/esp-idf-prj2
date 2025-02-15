#include "store.h"


static esp_err_t _http_event_handle(esp_http_client_event_t *evt)
{
    http_response_t *response = evt->user_data;

    switch(evt->event_id) {
        case HTTP_EVENT_ERROR:
            ESP_LOGI(TAG_STORE, "HTTP_EVENT_ERROR");
            break;
        case HTTP_EVENT_ON_CONNECTED:
            ESP_LOGI(TAG_STORE, "HTTP_EVENT_ON_CONNECTED");
            break;
        case HTTP_EVENT_HEADER_SENT:
            ESP_LOGI(TAG_STORE, "HTTP_EVENT_HEADER_SENT");
            break;
        case HTTP_EVENT_ON_HEADER:
            ESP_LOGI(TAG_STORE, "HTTP_EVENT_ON_HEADER");
            printf("%.*s", evt->data_len, (char*)evt->data);
            break;
        case HTTP_EVENT_ON_DATA: {
            ESP_LOGI(TAG_STORE, "HTTP_EVENT_ON_DATA");
            if (response != NULL)
            {
                char *new_data = realloc(response->data, response->size + evt->data_len + 1);
                if (!new_data) {
                    ESP_LOGE(TAG_STORE, "Memory allocation failed!");
                    return ESP_FAIL;
                }

                response->data = new_data;
                memcpy(response->data + response->size, evt->data, evt->data_len);
                response->size += evt->data_len;
                response->data[response->size] = '\0';
            }
            break;
        }
        case HTTP_EVENT_ON_FINISH:
            ESP_LOGI(TAG_STORE, "HTTP_EVENT_ON_FINISH");
            break;
        case HTTP_EVENT_DISCONNECTED:
            ESP_LOGI(TAG_STORE, "HTTP_EVENT_DISCONNECTED");
            break;
        case HTTP_EVENT_REDIRECT:
            ESP_LOGI(TAG_STORE, "HTTP_EVENT_REDIRECT");
            break;
    }
    return ESP_OK;
}

esp_err_t fetch_firestore_document(user_connect_t *user_connect)
{
    user_doc_t user_doc = {
        .plate = "",
        .in = false,
    };

    char url[256];


    snprintf(url, sizeof(FIREBASE_URL) + strlen(user_connect->user_id) + sizeof(FIREBASE_KEY), "%s%s%s", FIREBASE_URL,user_connect->user_id, FIREBASE_KEY);

    esp_http_client_config_t config = {
        .url = url,
        .event_handler = _http_event_handle,
        .user_data = user_connect->response,
    };

    user_connect->client = esp_http_client_init(&config);
    esp_err_t err = esp_http_client_perform(user_connect->client);

    if (err != ESP_OK)
    {
        ESP_LOGE(TAG_STORE, "HTTP request failed: %s", esp_err_to_name(err));
        goto CLEANUP;
    }
    
    if (user_connect->response->size == 0 || !user_connect->response->data)
    {
        ESP_LOGE(TAG_STORE, "No data received!");
        err = ESP_FAIL;
        goto CLEANUP;
    }

    user_connect->doc_root = cJSON_Parse(user_connect->response->data);
    if (!user_connect->doc_root)
    {
        ESP_LOGE(TAG_STORE, "JSON Parse Error: %s", cJSON_GetErrorPtr());
        err = ESP_FAIL;
        goto CLEANUP;
    }

    user_connect->doc_fields = cJSON_GetObjectItem(user_connect->doc_root, "fields");
    if (user_connect->doc_fields) {
        cJSON *plate_field = cJSON_GetObjectItem(user_connect->doc_fields, "plate");
        if (plate_field)
        {
            cJSON *plateValue = cJSON_GetObjectItem(plate_field, "stringValue");
            if (cJSON_IsString(plateValue))
            {
                strncpy(user_doc.plate, plateValue->valuestring, sizeof(user_doc.plate) - 1);
            }
        }

        cJSON *in_field = cJSON_GetObjectItem(user_connect->doc_fields, "in");
        if (in_field) 
        {
            cJSON *inValue = cJSON_GetObjectItem(in_field, "booleanValue");
            if (cJSON_IsBool(inValue)) 
            {
                user_doc.in = cJSON_IsTrue(inValue);
            }
        }
    }
    ESP_LOGI(TAG_STORE, "Received document:");
    ESP_LOGI(TAG_STORE, "Plate: %s", user_doc.plate);
    ESP_LOGI(TAG_STORE, "State: %s", user_doc.in ? "in" : "out");
CLEANUP:
    return err;
}

esp_err_t update_firestore_document(user_connect_t *user_connect)
{
    char *update_payload = NULL;
    cJSON_SetBoolValue(cJSON_GetObjectItem(cJSON_GetObjectItem(user_connect->doc_fields, "in"), "booleanValue"), !(user_connect->in_gate));
    update_payload = cJSON_PrintUnformatted(user_connect->doc_root);

    esp_http_client_set_method(user_connect->client, HTTP_METHOD_PATCH);
    esp_http_client_set_header(user_connect->client, "Content-Type", "application/json");
    esp_http_client_set_post_field(user_connect->client, update_payload, strlen(update_payload));

    esp_err_t err = esp_http_client_perform(user_connect->client);

    if (err == ESP_OK) 
    {
        ESP_LOGI(TAG_STORE, "Data sent successfully, HTTP Status = %d", esp_http_client_get_status_code(user_connect->client));
    }
    else 
    {
        ESP_LOGE(TAG_STORE, "HTTP PATCH request failed: %s", esp_err_to_name(err));
    }
    free(update_payload);
    return err;

}

