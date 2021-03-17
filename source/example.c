#include <stdio.h>
#include "jansson.h"

int main()
{
    json_error_t err;
    // json_t* foo = json_loads("{\"hello\": false}", 0, &err);
    json_t* foo = json_load_file("C:/Users/sasoun/Downloads/0_sideways_2_cam.json", 0, &err);
    if (!foo) {
        printf("Parse failed!  %s \n", err.text);
        return 1;
    } 
    json_t* hello_value = json_object_get(foo, "origin");
    if (hello_value) {
        printf("Hello: %s", json_boolean_value(hello_value) ? "true" : "false");
    }
    return 0;
}