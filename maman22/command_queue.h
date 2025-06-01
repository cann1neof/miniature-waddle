#ifndef COMMAND_QUEUE_H
#define COMMAND_QUEUE_H

typedef struct arg_node {
    char *argument;               /* The string argument */
    struct arg_node *next;        /* Pointer to next argument */
} arg_node;

/* Structure for argument list */
typedef struct arg_list {
    arg_node *head;               /* Head of the argument list */
    arg_node *tail;               /* Tail of the argument list */
} arg_list;

/* Structure for command queue node */
typedef struct command_node {
    char *command_name;           /* Name of the command to execute */
    arg_list *arguments;          /* Pointer to list of arguments */
    struct command_node *next;    /* Pointer to next command in queue */
} command_node;

/* Structure for FIFO command queue */
typedef struct command_queue {
    command_node *head;           /* Head of the queue (first to dequeue) */
    command_node *tail;           /* Tail of the queue (last to enqueue) */
} command_queue;

/**
 * Creates and initializes a new command queue
 * Use case: Initialize an empty FIFO queue to store commands for sequential execution
 * @return Pointer to newly allocated command_queue, or NULL on allocation failure
 */
command_queue* create_command_queue(void);

/**
 * Adds a new command to the end of the queue (FIFO)
 * Use case: Queue up commands with their arguments for later execution in order
 * @param queue Pointer to the command queue
 * @param command_name Name of the command to be executed
 * @param arguments Pointer to argument list for the command (can be NULL for no args)
 * @return 1 on success, 0 on failure (allocation error or invalid parameters)
 */
int enqueue_command(command_queue *queue, const char *command_name, arg_list *arguments);

/**
 * Removes and returns the first command from the queue (FIFO)
 * Use case: Retrieve the next command to execute from the queue
 * @param queue Pointer to the command queue
 * @return Pointer to the dequeued command_node, or NULL if queue is empty
 * Note: Caller is responsible for freeing the returned command_node
 */
command_node* dequeue_command(command_queue *queue);

/**
 * Checks if the command queue is empty
 * Use case: Determine if there are any pending commands to execute
 * @param queue Pointer to the command queue
 * @return 1 if queue is empty, 0 if it contains commands
 */
int is_queue_empty(command_queue *queue);

/**
 * Frees all memory associated with the command queue
 * Use case: Clean up when done with the queue to prevent memory leaks
 * @param queue Pointer to the command queue to free
 * Note: Also frees all remaining command nodes and their arguments
 */
void free_command_queue(command_queue *queue);

/**
 * Frees memory associated with a single command node
 * Use case: Clean up individual command nodes after processing
 * @param node Pointer to the command node to free
 * Note: Also frees the command name string and argument list
 */
void free_command_node(command_node *node);

/* Argument list operations */

/**
 * Creates and initializes a new argument list
 * Use case: Initialize an empty list to store command arguments
 * @return Pointer to newly allocated arg_list, or NULL on allocation failure
 */
arg_list* create_arg_list(void);

/**
 * Adds a new argument to the end of the argument list
 * Use case: Build up a list of arguments for a command (e.g., "ls", "-l", "/home")
 * @param list Pointer to the argument list
 * @param argument String argument to add to the list
 * @return 1 on success, 0 on failure (allocation error or invalid parameters)
 */
int add_argument(arg_list *list, const char *argument);

/**
 * Gets the first argument node from the argument list
 * Use case: Start iterating through arguments or access the first argument
 * @param list Pointer to the argument list
 * @return Pointer to the first arg_node, or NULL if list is empty
 */
arg_node* get_first_argument(arg_list *list);

/**
 * Gets the next argument node in the list
 * Use case: Continue iterating through arguments after get_first_argument()
 * @param current Pointer to the current argument node
 * @return Pointer to the next arg_node, or NULL if at end of list
 */
arg_node* get_next_argument(arg_node *current);

/**
 * Extracts the string value from an argument node
 * Use case: Get the actual argument string for processing or execution
 * @param node Pointer to the argument node
 * @return Pointer to the argument string, or NULL if node is NULL
 */
char* get_argument_value(arg_node *node);

/**
 * Frees all memory associated with an argument list
 * Use case: Clean up argument lists to prevent memory leaks
 * @param list Pointer to the argument list to free
 * Note: Frees all argument nodes and their string values
 */
void free_arg_list(arg_list *list);

#endif /* COMMAND_QUEUE_H */


