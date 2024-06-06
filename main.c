/******************************************************************************/
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amdemuyn <amdemuyn@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/06 20:24:50 by amdemuyn          #+#    #+#             */
/*   Updated: 2024/06/06 21:10:43 by amdemuyn         ###   ########.fr       */
/*                                                                            */
/******************************************************************************/

/*compile with gcc main.c -lreadline*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <readline/readline.h>
#include <readline/history.h>

#define MAX_INPUT_SIZE 1024

int main() {
    char *input;

    while (1) {
        input = readline("$-> ");
        if (!input) {
            break; // Exit on EOF (Ctrl-D)
        }

        // Remove trailing newline
        input[strcspn(input, "\n")] = '\0';

        // Exit command
        if (strcmp(input, "exit") == 0) {
            free(input);
            break;
        }

        // Here you will add command execution
        printf("You entered: %s\n", input);

        // Add to history
        if (strlen(input) > 0) {
            add_history(input);
        }
        free(input);
    }

    return 0;
}
