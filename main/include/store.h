#pragma once

#include "esp_http_client.h"
#include "cJSON.h"
#include "esp_log.h"

#define TAG_STORE       "Store"
#define FIREBASE_KEY    ""
#define FIREBASE_URL    "https://firestore.googleapis.com/v1/projects/prj2-58979/databases/(default)/documents/users/"

typedef struct
{
    char plate[16];
    bool in;
} user_doc_t;

typedef struct
{
    char *data;
    size_t size;
} http_response_t;

typedef struct
{
    char *user_id;
    bool in_gate;
    cJSON *doc_root;
    cJSON *doc_fields;
    esp_http_client_handle_t client;
    http_response_t *response;
} user_connect_t;

static esp_err_t _http_event_handle(esp_http_client_event_t *);

esp_err_t fetch_firestore_document(user_connect_t *user_connect);
esp_err_t update_firestore_document(user_connect_t *user_connect);
