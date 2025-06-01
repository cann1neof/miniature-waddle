#include "commands.h"
#include "mymat.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <math.h>

/* Check if a string is a valid number (including decimals) */
int is_valid_real_number(const char* str) {
    char *endptr;
    double value;
    
    if (!str || *str == '\0') return 0;
    
    /* Skip leading whitespace */
    while (isspace(*str)) str++;
    if (*str == '\0') return 0;
    
    /* Try to convert to double */
    value = strtod(str, &endptr);
    
    /* Check for overflow/underflow */
    if (value == HUGE_VAL || value == -HUGE_VAL) {
        return 0; /* Will be caught as overflow error later */
    }
    
    /* Skip trailing whitespace */
    while (isspace(*endptr)) endptr++;
    
    /* Valid if we consumed the entire string */
    return *endptr == '\0';
}

/* Check if the command name is one we recognize */
int is_valid_command_name(const char* command) {
    if (!command) return 0;
    return (strcmp(command, "read_mat") == 0 || strcmp(command, "print_mat") == 0 ||
            strcmp(command, "add_mat") == 0 || strcmp(command, "sub_mat") == 0 ||
            strcmp(command, "mul_mat") == 0 || strcmp(command, "mul_scalar") == 0 ||
            strcmp(command, "trans_mat") == 0 || strcmp(command, "stop") == 0);
}

/* Count how many arguments are in the list */
int count_arguments(arg_list *args) {
    int count = 0;
    arg_node *current = get_first_argument(args);
    while (current) {
        count++;
        current = get_next_argument(current);
    }
    return count;
}

/* Make sure the arguments are valid for the given command */
int validate_command_arguments(const char* command_name, arg_list *args) {
    int arg_count = count_arguments(args);
    arg_node *current;
    char *arg_value;
    double test_value;
    char *endptr;
    int i;
    
    if (!command_name || !args) {
        printf("Missing argument\n");
        return 0;
    }
    
    if (strcmp(command_name, "read_mat") == 0) {
        if (arg_count < 1) {
            printf("Missing argument\n");
            return 0;
        }
        current = get_first_argument(args);
        arg_value = get_argument_value(current);
        if (get_matrix_index(arg_value) == -1) {
            printf("Undefined matrix name\n");
            return 0;
        }
        /* Check that all remaining arguments are valid real numbers */
        current = get_next_argument(current);
        while (current) {
            arg_value = get_argument_value(current);
            if (!is_valid_real_number(arg_value)) {
                printf("Argument is not a real number\n");
                return 0;
            }
            /* Check for numeric overflow */
            test_value = strtod(arg_value, &endptr);
            if (test_value == HUGE_VAL || test_value == -HUGE_VAL) {
                printf("Error: Numeric overflow in argument '%s'\n", arg_value);
                return 0;
            }
            current = get_next_argument(current);
        }
    }
    else if (strcmp(command_name, "print_mat") == 0) {
        if (arg_count < 1) {
            printf("Missing argument\n");
            return 0;
        }
        current = get_first_argument(args);
        while (current) {
            arg_value = get_argument_value(current);
            if (get_matrix_index(arg_value) == -1) {
                printf("Undefined matrix name\n");
                return 0;
            }
            current = get_next_argument(current);
        }
    }
    else if (strcmp(command_name, "add_mat") == 0 || strcmp(command_name, "sub_mat") == 0 || strcmp(command_name, "mul_mat") == 0) {
        if (arg_count < 3) {
            printf("Missing argument\n");
            return 0;
        }
        if (arg_count > 3) {
            printf("Extraneous text after end of command\n");
            return 0;
        }
        current = get_first_argument(args);
        for (i = 0; i < 3; i++) {
            arg_value = get_argument_value(current);
            if (get_matrix_index(arg_value) == -1) {
                printf("Undefined matrix name\n");
                return 0;
            }
            current = get_next_argument(current);
        }
    }
    else if (strcmp(command_name, "mul_scalar") == 0) {
        if (arg_count < 3) {
            printf("Missing argument\n");
            return 0;
        }
        if (arg_count > 3) {
            printf("Extraneous text after end of command\n");
            return 0;
        }
        current = get_first_argument(args);
        /* First argument: matrix name */
        arg_value = get_argument_value(current);
        if (get_matrix_index(arg_value) == -1) {
            printf("Undefined matrix name\n");
            return 0;
        }
        /* Second argument: scalar */
        current = get_next_argument(current);
        arg_value = get_argument_value(current);
        if (!is_valid_real_number(arg_value)) {
            printf("Argument is not a scalar\n");
            return 0;
        }
        /* Check for numeric overflow in scalar */
        test_value = strtod(arg_value, &endptr);
        if (test_value == HUGE_VAL || test_value == -HUGE_VAL) {
            printf("Error: Numeric overflow in scalar value '%s'\n", arg_value);
            return 0;
        }
        /* Third argument: target matrix */
        current = get_next_argument(current);
        arg_value = get_argument_value(current);
        if (get_matrix_index(arg_value) == -1) {
            printf("Undefined matrix name\n");
            return 0;
        }
    }
    else if (strcmp(command_name, "trans_mat") == 0) {
        if (arg_count < 2) {
            printf("Missing argument\n");
            return 0;
        }
        if (arg_count > 2) {
            printf("Extraneous text after end of command\n");
            return 0;
        }
        current = get_first_argument(args);
        for (i = 0; i < 2; i++) {
            arg_value = get_argument_value(current);
            if (get_matrix_index(arg_value) == -1) {
                printf("Undefined matrix name\n");
                return 0;
            }
            current = get_next_argument(current);
        }
    }
    else if (strcmp(command_name, "stop") == 0) {
        if (arg_count > 0) {
            printf("Extraneous text after end of command\n");
            return 0;
        }
    }
    
    return 1; /* All validations passed */
}

/* Execute all the commands in the queue one by one */
void execute_queued_commands(command_queue *queue, mat matrices[MAT_COUNT]) {
    if (!queue || is_queue_empty(queue)) {
        return; /* No commands to execute */
    }
    
    while (!is_queue_empty(queue)) {
        command_node *cmd = dequeue_command(queue);
        arg_node *argument;
        char *matrix_name, *scalar_str;
        mat *first_matrix, *second_matrix, *target_matrix;
        double scalar;
        
        if (!cmd) break;
        
        /* Validate command arguments before execution */
        if (!validate_command_arguments(cmd->command_name, cmd->arguments)) {
            free_command_node(cmd);
            continue; /* Skip execution due to validation error */
        }
        
        if (strcmp(cmd->command_name, "read_mat") == 0) {
            argument = get_first_argument(cmd->arguments);
            if (argument) {
                matrix_name = get_argument_value(argument);
                target_matrix = get_matrix_by_name(matrix_name, matrices);
                
                if (target_matrix) {
                    read_mat(cmd->arguments, target_matrix);
                }
            }
        }
        else if (strcmp(cmd->command_name, "print_mat") == 0) {
            argument = get_first_argument(cmd->arguments);
            while (argument) {
                matrix_name = get_argument_value(argument);
                target_matrix = get_matrix_by_name(matrix_name, matrices);
                
                if (target_matrix) {
                    print_mat(target_matrix);
                }
                
                argument = get_next_argument(argument);
            }
        }
        else if (strcmp(cmd->command_name, "trans_mat") == 0) {
            argument = get_first_argument(cmd->arguments);
            first_matrix = get_matrix_by_name(get_argument_value(argument), matrices);
            
            argument = get_next_argument(argument);
            target_matrix = get_matrix_by_name(get_argument_value(argument), matrices);
            
            trans_mat(first_matrix, target_matrix);
        }
        else if (strcmp(cmd->command_name, "add_mat") == 0) {
            argument = get_first_argument(cmd->arguments);
            first_matrix = get_matrix_by_name(get_argument_value(argument), matrices);
            
            argument = get_next_argument(argument);
            second_matrix = get_matrix_by_name(get_argument_value(argument), matrices);

            argument = get_next_argument(argument);
            target_matrix = get_matrix_by_name(get_argument_value(argument), matrices);

            add_mat(first_matrix, second_matrix, target_matrix);
        }
        else if (strcmp(cmd->command_name, "sub_mat") == 0) {
            argument = get_first_argument(cmd->arguments);
            first_matrix = get_matrix_by_name(get_argument_value(argument), matrices);

            argument = get_next_argument(argument);
            second_matrix = get_matrix_by_name(get_argument_value(argument), matrices);

            argument = get_next_argument(argument);
            target_matrix = get_matrix_by_name(get_argument_value(argument), matrices);

            sub_mat(first_matrix, second_matrix, target_matrix);
        }
        else if (strcmp(cmd->command_name, "mul_mat") == 0) {
            argument = get_first_argument(cmd->arguments);
            first_matrix = get_matrix_by_name(get_argument_value(argument), matrices);

            argument = get_next_argument(argument);
            second_matrix = get_matrix_by_name(get_argument_value(argument), matrices);

            argument = get_next_argument(argument);
            target_matrix = get_matrix_by_name(get_argument_value(argument), matrices);    

            mul_mat(first_matrix, second_matrix, target_matrix);
        }
        else if (strcmp(cmd->command_name, "mul_scalar") == 0) {
            argument = get_first_argument(cmd->arguments);
            first_matrix = get_matrix_by_name(get_argument_value(argument), matrices);
            
            argument = get_next_argument(argument);
            scalar_str = get_argument_value(argument);
            scalar = strtod(scalar_str, NULL);

            argument = get_next_argument(argument);
            target_matrix = get_matrix_by_name(get_argument_value(argument), matrices);

            mul_scalar(first_matrix, scalar, target_matrix);
        }
        else if (strcmp(cmd->command_name, "stop") == 0) {
            free_command_node(cmd);
            return; /* Exit the loop */
        }
        
        free_command_node(cmd);
    }
}

/* Parse a line of input and extract the command name and arguments */
char* parse_line(char *line, arg_list *args) {
    char *ptr = line;
    char *start;
    int len;
    char token_buffer[256];
    int expected_args;
    char *command_name = NULL;
    
    if (!line || !args) {
        return NULL;
    }
    
    /* Check for excessively long input line */
    if (strlen(line) >= 1023 && line[strlen(line)-1] != '\n') {
        printf("Error: Input line exceeds maximum length limit\n");
        return NULL;
    }
    
    /* Skip leading whitespace */
    for (; *ptr && (*ptr == ' ' || *ptr == '\t' || *ptr == '\n' || *ptr == '\r'); ptr++);
    
    /* Check for empty or whitespace-only input */
    if (!*ptr) {
        return NULL; /* Silently ignore empty lines */
    }
    
    /* Check for lines with only commas and whitespace */
    start = ptr;
    while (*start && (*start == ',' || *start == ' ' || *start == '\t')) start++;
    if (!*start || *start == '\n' || *start == '\r') {
        printf("Please enter a valid command\n");
        return NULL;
    }
    
    /* Extract command name (first word) */
    start = ptr;
    while (*ptr && *ptr != ' ' && *ptr != '\t' && *ptr != '\n' && *ptr != '\r' && *ptr != ',') {
        ptr++;
    }
    
    len = ptr - start;
    if (len > 0) {
        /* Check for token buffer overflow */
        if (len >= 255) {
            printf("Error: Command name exceeds maximum length limit\n");
            return NULL;
        }
        
        /* Dynamically allocate memory for command name */
        command_name = (char*)malloc(len + 1);
        if (!command_name) {
            printf("Error: Memory allocation failed for command name\n");
            return NULL;
        }
        strncpy(command_name, start, len);
        command_name[len] = '\0';
    } else {
        printf("Please enter a command\n");
        return NULL;
    }
    
    /* Check if command name is valid */
    if (!is_valid_command_name(command_name)) {
        printf("Undefined command name\n");
        free(command_name);
        return NULL;
    }
    
    /* Determine expected number of arguments for this command */
    if (strcmp(command_name, "stop") == 0) {
        expected_args = 0;
    } else if (strcmp(command_name, "print_mat") == 0) {
        expected_args = -1; /* Variable number of arguments */
    } else if (strcmp(command_name, "read_mat") == 0) {
        expected_args = -1; /* Variable number of arguments */
    } else if (strcmp(command_name, "trans_mat") == 0) {
        expected_args = 2;
    } else if (strcmp(command_name, "add_mat") == 0 || strcmp(command_name, "sub_mat") == 0 || 
               strcmp(command_name, "mul_mat") == 0 || strcmp(command_name, "mul_scalar") == 0) {
        expected_args = 3;
    } else {
        expected_args = -1;
    }
    
    /* Skip whitespace after command */
    for (; *ptr && (*ptr == ' ' || *ptr == '\t'); ptr++);
    
    /* Check for illegal comma immediately after command */
    if (*ptr == ',') {
        printf("Illegal comma\n");
        free(command_name);
        return NULL;
    }
    
    /* Parse arguments if they exist */
    while (*ptr && *ptr != '\n' && *ptr != '\r') {
        /* Skip leading whitespace for this argument */
        for (; *ptr && (*ptr == ' ' || *ptr == '\t'); ptr++);
        
        if (!*ptr || *ptr == '\n' || *ptr == '\r') break;
        
        /* Extract argument until comma, whitespace, or end of line */
        start = ptr;
        for (; *ptr && *ptr != ',' && *ptr != ' ' && *ptr != '\t' && *ptr != '\n' && *ptr != '\r'; ptr++);
        
        /* Get argument length and copy */
        len = ptr - start;
        if (len > 0 && len < 255) { /* Ensure buffer safety */
            strncpy(token_buffer, start, len);
            token_buffer[len] = '\0';
            
            if (!add_argument(args, token_buffer)) {
                printf("Error: Failed to add argument to list\n");
                free(command_name);
                return NULL;
            }
        } else if (len == 0) {
            /* Empty argument */
            printf("Multiple consecutive commas\n");
            free(command_name);
            return NULL;
        } else {
            /* Token too long */
            printf("Error: Argument exceeds maximum length limit\n");
            free(command_name);
            return NULL;
        }
        
        /* Skip whitespace after argument */
        for (; *ptr && (*ptr == ' ' || *ptr == '\t'); ptr++);
        
        /* Check if we have enough arguments and there's still more text */
        if (expected_args > 0 && count_arguments(args) >= expected_args && 
            (*ptr && *ptr != '\n' && *ptr != '\r')) {
            printf("Extraneous text after end of command\n");
            free(command_name);
            return NULL;
        }
        
        /* Handle comma separator */
        if (*ptr == ',') {
            ptr++; /* Skip comma */
            
            /* Skip whitespace after comma */
            for (; *ptr && (*ptr == ' ' || *ptr == '\t'); ptr++);
            
            /* Check for another comma (multiple consecutive commas) */
            if (*ptr == ',') {
                printf("Multiple consecutive commas\n");
                free(command_name);
                return NULL;
            }
            
            /* Check if line ends after comma (trailing comma) */
            if (!*ptr || *ptr == '\n' || *ptr == '\r') {
                printf("Extraneous text after end of command\n");
                free(command_name);
                return NULL;
            }
        } 
        /* If there's more content but no comma, check if it's valid */
        else if (*ptr && *ptr != '\n' && *ptr != '\r') {
            /* For commands that require commas between arguments, this is an error */
            if (strcmp(command_name, "read_mat") != 0) { /* read_mat can have space-separated numbers */
                printf("Missing comma\n");
                free(command_name);
                return NULL;
            }
            /* For read_mat, continue parsing space-separated numbers */
        }
    }
    
    return command_name; /* Success */
}

/* Main function that reads user input and processes matrix commands */
void process_commands(mat matrices[MAT_COUNT]) {
    /* Variable declarations - all at the beginning for C90 compliance */
    char line[1024];
    char *command_name;
    command_queue *queue;
    arg_list *current_args = NULL;
    int queue_size = 0;
    const int MAX_QUEUE_SIZE = 1000; /* Prevent queue overflow */

    /* Initialize command queue */
    queue = create_command_queue();

    if (!queue) {
        printf("Error: Failed to create command queue\n");
        return;
    }

    while (fgets(line, sizeof(line), stdin)) {
        /* Check for queue overflow */
        if (queue_size >= MAX_QUEUE_SIZE) {
            printf("Error: Command queue overflow - too many pending commands\n");
            continue;
        }
        
        /* Create argument list for this command */
        current_args = create_arg_list();
        if (!current_args) {
            printf("Error: Failed to create argument list\n");
            continue;
        }
        
        /* Parse the line dynamically - extracts command and populates args directly */
        command_name = parse_line(line, current_args);
        if (command_name) {
            /* Check if command is "stop" */
            if (strcmp(command_name, "stop") == 0) {
                free_arg_list(current_args);
                free(command_name);
                break;
            }
            
            /* Enqueue the command with its arguments */
            if (enqueue_command(queue, command_name, current_args)) {
                /* Queue now owns the arg_list, so we don't free it */
                current_args = NULL;
                queue_size++;
                
                /* Execute immediately */
                execute_queued_commands(queue, matrices);
                queue_size = 0; /* Reset after execution */
            } else {
                printf("Error: Failed to enqueue command '%s'\n", command_name);
                free_arg_list(current_args);
                current_args = NULL;
            }
            free(command_name);
        } else {
            /* Parsing failed - error message already printed by parse_line */
            free_arg_list(current_args);
            current_args = NULL;
        }
    }

    /* Clean up */
    free_command_queue(queue);
}

