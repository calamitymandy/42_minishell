// ADD HEADER

#include "minishell.h"

bool    set_pwd_n_oldpwd(t_minishell *mini)
{
    char    buff[1024];
    char    *working_dir;

    working_dir = getcwd(buff, 1024);
    mini->pwd = ft_strdup(working_dir);
    if (!mini->pwd)
        return (false);
    if (srch_env_i(mini->env, "OLDPWD") != -1)
    {
        mini->old_pwd = ft_strdup(get_env_value(mini->env,
                    "OLDPWD"));
        if (!mini->old_pwd)
            return (false);
    }
    else
    {
        mini->old_pwd = ft_strdup(working_dir);
        if (!mini->old_pwd)
            return (false);
    }
    return (true);
}