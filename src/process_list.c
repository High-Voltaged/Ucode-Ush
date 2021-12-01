#include "../inc/ush.h"

t_process *mx_create_process(t_cmd_utils* utils)
{
    t_process *new_node = malloc(sizeof(t_process));
    
    char** paths = mx_get_exec_paths(utils->args[0], NULL, true);
    new_node->stopped = new_node->completed = false;
    new_node->path = paths[0] ? mx_strdup(paths[0]) : mx_strdup(utils->args[0]);
    mx_del_strarr(&paths);
    
    new_node->next = NULL;
    return new_node;
}

void mx_process_push_back(t_process **list, t_cmd_utils* utils) {

    t_process* new_node = mx_create_process(utils);
    if (list != NULL && *list == NULL) {
        *list = new_node;
        return;
    }

    t_process* last = *list;
    while (last->next != NULL) {
        last = last->next;
    } 

    new_node->next = last->next;
    last->next = new_node;

}

void mx_process_pop_front(t_process **head) {

    if (head == NULL || *head == NULL) return; 

    if ((*head)->next == NULL) {
        free(*head);
        *head = NULL;
        return;
    }

    t_process* temp = *head;
    *head = (*head)->next;
    free(temp);

}

void mx_clear_process(t_process** p) {

    mx_strdel(&(*p)->path);
    free(*p);
    *p = NULL;

}

void mx_process_pop_back(t_process **head) {

    if (head == NULL || *head == NULL) return;

    if ((*head)->next == NULL) {
        mx_clear_process(head);
        return;
    }

    t_process* prelast = *head;
    while (prelast->next->next != NULL) {

        prelast = prelast->next;

    }
    mx_clear_process(&prelast->next);
    prelast->next = NULL;

}

void mx_process_pop_index(t_process **list, int index) {

    int size = 0;
    t_process* head = *list;
    while (head != NULL) {
        head = head->next;
        ++size;
    }

    if (index <= 0) {
        mx_process_pop_front(list);
        return;
    } else if (index >= size) {
        mx_process_pop_back(list);
        return;
    }

    t_process* current = *list;
    for (int i = 0; current != NULL && i < (index - 1); ++i) {
        current = current->next;
    }
    t_process* next = current->next->next;
    free(current->next);
    current->next = next;

}


void mx_clear_process_list(t_process **list)
{
    if (list == NULL || *list == NULL)
        return;

    t_process *node = *list;
    t_process *next = NULL;

    while (node != NULL)
    {
        next = node->next;
        mx_clear_process(&node);
        node = next;
    }
    
    *list = NULL;
}

int mx_process_list_size(t_process* list) {

    if (list == NULL) return 0;

    t_process* head = list;
    int size = 0;
    while (head != NULL) {

        head = head->next;
        ++size;

    }
    return size;

}


t_process* mx_get_process_by_pid(t_process* list, pid_t pid, int* index) {

    t_process* current = list;
    int i = 0;
    while (current) {

        if (index != NULL)
            *index = i++;

        if (current->pid == pid)   
            return current;
        current = current->next;

    }
    return NULL;

}

t_process* mx_top_process(t_process* list) {

    t_process* current = list;
    while (current->next) {

        current = current->next;

    }
    return current;

}

void mx_print_process_list(t_process* list) {

    while (list) {

        mx_printstr(list->path);
        mx_printstr("\n");
        list = list->next;

    }

}
