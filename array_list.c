//
// Created by Russ Sobti on 5/2/23.
//

#include "array_list.h"
#include "tree.h" // for the entry struct
#include "stdlib.h"

void array_list_free(array_list *arr) {
    free(arr->array);
    arr->array = NULL;
    arr->capacity = 0;
    arr->size = 0;
}
array_list *array_list_new(void) {
    array_list *list = calloc(1,sizeof(array_list));
    if (list == NULL) {
        return NULL;
    }
    list->size = 0;
    list->capacity = 10;
    list->array = calloc(list->capacity, sizeof(entry*));
    if (list->array == NULL) {
        free(list);
        return NULL;
    }
    return list;
}

void array_list_add_to_end(array_list *list, entry* value) {
    if (list->size == list->capacity) {
        list->capacity *= 2;
        entry** new_array = realloc(list->array, list->capacity * sizeof(entry*));
        if (new_array == NULL) {
            return;
        }
        list->array = new_array;
    }
    list->array[list->size] = value;
    list->size++;
}
entry* array_list_stack_pop(array_list *list) {
    if (list->size == 0) {
        return NULL;
    }
    entry* value = list->array[list->size - 1];
    list->size--;
    list->array[list->size] = NULL;
    return value;
}

entry* array_list_queue_pop(array_list *list) {
    if (list->size == 0) {
        return NULL;
    }
    entry* value = list->array[0];
    for (int i = 0; i < list->size - 1; i++) {
        list->array[i] = list->array[i + 1];
    }
    list->size--;
    list->array[list->size] = NULL;
    return value;
}

void array_list_alphabetize(array_list *list){
    // alphabetize the array list by entry name
    for (int i = 0; i < list->size; i++) {
        for (int j = i + 1; j < list->size; j++) {
            if (strcmp(list->array[i]->name, list->array[j]->name) > 0) {
                entry* temp = list->array[i];
                list->array[i] = list->array[j];
                list->array[j] = temp;
            }
        }
    }
}
