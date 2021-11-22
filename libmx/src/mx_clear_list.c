#include "../inc/libmx.h"

void mx_clear_list(t_list **list) {

    t_list* current = *list;
    t_list* next = NULL;
    while (current != NULL) {

        next = current->next;
        free(current);
        current = next;

    }
    *list = NULL;

}
