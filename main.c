/******************************************************************************/
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amdemuyn <amdemuyn@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/06 20:24:50 by amdemuyn          #+#    #+#             */
/*   Updated: 2024/06/12 19:51:21 by amdemuyn         ###   ########.fr       */
/*                                                                            */
/******************************************************************************/

#include "minishell.h"
/*compile with gcc main.c -lreadline*/

int g_status;

void    exit_mini(t_minishell *mini, int exit_code)
{
    //TODO
    exit(exit_code);
}

bool    init_main_struct(t_minishell *mini, char **env)
{
    //TODO set_env_var & set_pwd_and aldpwd
    mini->env = env;
    mini->line = NULL;
    //mini->ctrlc_heredoc = false;
    //mini->token = NULL;
    //mini->cmd = NULL;
    mini->pid = -1;
    g_status = 0;
    return (true);
}

void    main_loop(t_minishell *mini)
{
    char    *input;

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
}

int main(int argc, char **argv, char **env)
{
    t_minishell mini;

    (void)argv;
    (void)argc;
    if (!init_main_struct(&mini, env))
        exit_mini(NULL, EXIT_FAILURE);
    main_loop(&mini);
    return 0;
}
