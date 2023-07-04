//
// Created by Russ Sobti on 5/2/23.
//

#ifndef CSC357_LAB3_REALRUSSSOBTI_ARRAYLIST_H
#define CSC357_LAB3_REALRUSSSOBTI_ARRAYLIST_H
typedef struct entry entry;
typedef struct array_list array_list;

struct entry {
    char type; // 'd' for directory, 'f' for file
    int hidden; // 1 if hidden, 0 if not
    char *name; // name of file or directory
    int visited;  // 1 if visited, 0 if not
    array_list *children; // Pointer to an array_list of children
    int size; // size of file or directory, in bytes
};

void array_list_free(array_list* arr);
struct array_list {
    int size;
    int capacity;
    entry** array;
};
// entry: a struct that represents a file or directory, and its children


array_list *array_list_new(void);
void array_list_add_to_end(array_list* list, entry* value);
entry* array_list_stack_pop(array_list* list);
entry* array_list_queue_pop(array_list* list);
void array_list_alphabetize(array_list* list);
#endif //CSC357_LAB3_REALRUSSSOBTI_ARRAYLIST_H
