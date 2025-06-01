#ifndef COMMANDS_H
#define COMMANDS_H

#include "mymat.h"
#include "command_queue.h"

/**
 * @brief Main command processing function that reads and executes commands from stdin
 * @param matrices Array of matrices (MAT_A through MAT_F) for command operations
 * @note Processes commands line by line until "stop" command or EOF is encountered
 * @note Uses a command queue for sequential command execution
 * @warning Function will continue until explicit "stop" command is received
 */
void process_commands(mat matrices[MAT_COUNT]);

/**
 * @brief Dynamic parsing function that extracts command name and arguments from input line
 * @param line Input line containing command and arguments
 * @param args Argument list to be populated with parsed arguments
 * @return Dynamically allocated command name string, or NULL on parsing failure
 * @note Caller is responsible for freeing the returned command name
 * @note Handles comma-separated arguments and validates command syntax
 * @warning Prints specific error messages for various parsing failures
 */
char* parse_line(char *line, arg_list *args);

/**
 * @brief Validates if a string represents a valid real number
 * @param str String to be validated as a real number
 * @return 1 if string is a valid real number, 0 otherwise
 * @note Handles leading/trailing whitespace and uses strtod for validation
 * @warning Returns 0 for NULL or empty strings
 */
int is_valid_real_number(const char* str);

/**
 * @brief Validates if a command name is recognized by the system
 * @param command Command name string to be validated
 * @return 1 if command name is valid, 0 otherwise
 * @note Valid commands: read_mat, print_mat, add_mat, sub_mat, mul_mat, mul_scalar, trans_mat, stop
 * @warning Returns 0 for NULL command names
 */
int is_valid_command_name(const char* command);

/**
 * @brief Counts the total number of arguments in an argument list
 * @param args Argument list to count
 * @return Number of arguments in the list
 * @note Traverses the entire linked list to count arguments
 * @warning Returns 0 for NULL argument list
 */
int count_arguments(arg_list *args);

/**
 * @brief Validates command arguments against expected format for each command type
 * @param command_name Name of the command to validate arguments for
 * @param args Argument list containing the arguments to validate
 * @return 1 if arguments are valid for the command, 0 if validation fails
 * @note Checks argument count, matrix names, and numeric values as appropriate
 * @note Prints specific error messages for different validation failures
 * @warning Returns 0 for NULL command name or argument list
 */
int validate_command_arguments(const char* command_name, arg_list *args);

/**
 * @brief Executes all queued commands sequentially until queue is empty
 * @param queue Command queue containing commands to execute
 * @param matrices Array of matrices (MAT_A through MAT_F) for command operations
 * @note Validates each command before execution and skips invalid commands
 * @note Frees command nodes after execution to prevent memory leaks
 * @warning Stops execution immediately when "stop" command is encountered
 */
void execute_queued_commands(command_queue *queue, mat matrices[MAT_COUNT]);

#endif /* COMMANDS_H */
