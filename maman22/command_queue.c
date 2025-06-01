#include "commands.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* Create a new empty command queue */
command_queue* create_command_queue(void) {
    command_queue *queue = (command_queue*)malloc(sizeof(command_queue));
    if (queue) {
        queue->head = NULL;
        queue->tail = NULL;
    }
    return queue;
}

/* Create a new empty argument list */
arg_list* create_arg_list(void) {
    arg_list *list = (arg_list*)malloc(sizeof(arg_list));
    if (list) {
        list->head = NULL;
        list->tail = NULL;
    }
    return list;
}

/* Add an argument to the end of the list */
int add_argument(arg_list *list, const char *argument) {
    arg_node *new_node;
    
    if (!list || !argument) return 0;
    
    new_node = (arg_node*)malloc(sizeof(arg_node));
    if (!new_node) return 0;
    
    new_node->argument = (char*)malloc(strlen(argument) + 1);
    if (!new_node->argument) {
        free(new_node);
        return 0;
    }
    
    strcpy(new_node->argument, argument);
    new_node->next = NULL;
    
    if (list->tail) {
        list->tail->next = new_node;
        list->tail = new_node;
    } else {
        list->head = list->tail = new_node;
    }
    
    return 1;
}

/* Add a command to the back of the queue */
int enqueue_command(command_queue *queue, const char *command_name, arg_list *arguments) {
    command_node *new_node;
    
    if (!queue || !command_name) return 0;
    
    new_node = (command_node*)malloc(sizeof(command_node));
    if (!new_node) return 0;
    
    new_node->command_name = (char*)malloc(strlen(command_name) + 1);
    if (!new_node->command_name) {
        free(new_node);
        return 0;
    }
    
    strcpy(new_node->command_name, command_name);
    new_node->arguments = arguments;
    new_node->next = NULL;
    
    if (queue->tail) {
        queue->tail->next = new_node;
        queue->tail = new_node;
    } else {
        queue->head = queue->tail = new_node;
    }
    
    return 1;
}

/* Remove and return the first command from the queue */
command_node* dequeue_command(command_queue *queue) {
    command_node *node_to_remove;
    
    if (!queue || !queue->head) return NULL;
    
    node_to_remove = queue->head;
    queue->head = queue->head->next;
    
    if (!queue->head) {
        queue->tail = NULL;
    }
    
    node_to_remove->next = NULL;
    return node_to_remove;
}

/* Check if the queue has no commands in it */
int is_queue_empty(command_queue *queue) {
    return (queue == NULL || queue->head == NULL);
}

/* Get the first argument from the list */
arg_node* get_first_argument(arg_list *list) {
    if (!list) return NULL;
    return list->head;
}

/* Get the next argument in the list */
arg_node* get_next_argument(arg_node *current) {
    if (!current) return NULL;
    return current->next;
}

/* Get the string value from an argument node */
char* get_argument_value(arg_node *node) {
    if (!node) return NULL;
    return node->argument;
}

/* Free up all memory used by an argument list */
void free_arg_list(arg_list *list) {
    arg_node *current;
    
    if (!list) return;
    
    current = list->head;
    while (current) {
        arg_node *next = current->next;
        free(current->argument);
        free(current);
        current = next;
    }
    
    free(list);
}

/* Free up memory used by a single command node */
void free_command_node(command_node *node) {
    if (!node) return;
    
    free(node->command_name);
    free_arg_list(node->arguments);
    free(node);
}

/* Free up all memory used by the command queue */
void free_command_queue(command_queue *queue) {
    if (!queue) return;
    
    while (!is_queue_empty(queue)) {
        command_node *node = dequeue_command(queue);
        free_command_node(node);
    }
    
    free(queue);
} 

