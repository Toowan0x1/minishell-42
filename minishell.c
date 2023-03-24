#include "minishell.h"

#define STDIN   0
#define STDOUT  1
#define STDERR  2

// PIPEX-42 | write_heredoc function:
/*
void	write_heredoc(char *limiter, int fd[2])
{
	size_t	limiter_length;
	char	*line;

	limiter_length = ft_strlen(limiter);
	while (1)
	{
		ft_putstr_fd("pipex here_doc> ", 1);
		line = get_next_line(STDIN);
		if (!line)
		{
			close(fd[0]);
			close(fd[1]);
			exit(EXIT_FAILURE);
		}
		if (line[limiter_length] == '\n' && limiter && \
			!ft_strncmp(line, limiter, limiter_length))
		{
			close(fd[0]);
			close(fd[1]);
			exit(EXIT_SUCCESS);
		}
		ft_putstr_fd(line, fd[1]);
		free(line);
		line = NULL;
	}
}
*/

int    find_env_value(char *env, char *env_var)
{
    int     i;

    i = 0;
    while (env_var[i] != '\0')
    {
        if (env[i] == env_var[i]) //  "USER_42" == "USER"
            i++;
        else
            return (0);
    }
    //if (env[i] == "=")
    if (ft_strncmp(&env[i], "=", 1) == 0)
        return (i + 1);
    return (0);
}

char    *get_env_value(char *env_var)
{
    size_t  i;
    int     start;
    char    *val;
    //size_t  env_len;

    if (!env_var)
        return (NULL);
    i = 0;
    while (global_env[i])
    {
        if (find_env_value(global_env[i], env_var) != 0)
        {
            start = find_env_value(global_env[i], env_var);
            //val = malloc(sizeof(char) * (ft_strlen(global_env[i][start]) + 1)); // don't forget freeing this 
            val = ft_strdup(&global_env[i][start]);
            //printf("val = %s\n", val);
            return (val);
            //break ;
        }
        //else
            //printf("no\n");
        i++;
    }
    return (NULL);
}

int     find_env(char *env) // does that do same as find_env_value()
{
    size_t  i;
    int     len;

    len = ft_strlen(env);
    i = 0;
    while (global_env[i])
    {
        if (ft_strncmp(global_env[i], env, len) == 0)
        {
            if (ft_strncmp(&global_env[i][len + 1], "=", 0) == 0)
                return (i);
            i++;
        }
        else
            i++;
    }
    return (0);
}

void    set_env(char *env, char *new_env)
{
    size_t  i;
    int     len;
    char    *tmp;
    char    *new;

    if (!env || !new_env)
        return ;
    len = ft_strlen(env);
    i = find_env(env);
    if (i != 0)
    {
        tmp = ft_substr(global_env[i], 0, len + 1); // handle if malloc == null inside the substr function
        new = ft_strjoin(tmp, new_env); // this as well
        free(tmp);
        free(global_env[i]);
        global_env[i] = new;
        //printf("new = %s\n", new);
        //printf("global_env[%zu]= %s\n", i, global_env[i]);
    }

}

void    echo_cmd(char *line) // echo "salam w3alaykom"
{
    int i;

    // if "echo '" then i = 6;  

    i = 6;
    while (line[i])
    {
        write(1, &line[i], 1);
        i++;
    }
}

void    cd_cmd(char *line)
{
    //if (**char args && args[1] && args[2])
    // then print "error: too many arguments" + exit(1)
    int     ret;
    char    *path;
    char    *pwd;

    //ft_putstr_fd(path, 2);
    pwd = getcwd(NULL, 0);
    path = ft_strdup(&line[3]);
    ret = chdir(path);
    if (ret == -1) // if chdir() returns -1, it means that the directory could not be changed.
        perror("chdir"); // using perror to print the error message to the console.
    else
    {
        // update the PWD environment variable with the new directory path using the setenv()
        // we have also update the OLDPWD environment variable with the previous directory path.
        if (pwd)
        {
            set_env("OLDPWD", pwd);
            free(pwd);
        }
        if ((pwd = getcwd(NULL, 0)))
        {
            set_env("PWD", pwd);
            free(pwd);
        }
        printf("PWD=%s\n", get_env_value("PWD"));
        printf("OLDPWD=%s\n", get_env_value("OLDPWD"));
    }
    return ;
    // free pwd
}

void    pwd_cmd(char *line)
{
    (void)line;
    char    *cwd;//pwd

    cwd = getcwd(NULL, 0);
    if (!cwd)
        perror("getcwd");
    ft_putendl_fd(cwd, 1);
    free(cwd);
}

void    export_cmd(char *line)
{
    (void)line;
    // ...
    return ;
}

void    unset_cmd(char *line)
{
    //size_t  i;
    //ssize_t index;

    global_status = 0; // ??
    //if (!line[1])
    //    return (1);
    (void)line;
    ft_putstr_fd("unset\n", 2);
    //
    return ;
}

void    env_cmd(char *line)
{
    (void)line;
    ft_putstr_fd("env\n", 2);
    return ;
}
void    exit_cmd(char *line)
{
    (void)line;
    ft_putstr_fd("exit\n", 2);
    return ;
}

void    parse_builtins(char *line)
{
    /* ECHO */
    if (ft_strncmp("echo ", line, 4) == 0)
        echo_cmd(line);
    /* CD */
    else if (ft_strncmp("cd ", line, 2) == 0)
        cd_cmd(line);
    /* PWD */
    // unset path
    else if (ft_strncmp("pwd ", line, 3) == 0)
        pwd_cmd(line);
    /* EXPORT */
    else if (ft_strncmp("export ", line, 6) == 0)
        export_cmd(line);
    /* UNSET */
    else if (ft_strncmp("unset ", line, 5) == 0)
        unset_cmd(line);
    /* ENV */
    else if (ft_strncmp("env ", line, 3) == 0)
        env_cmd(line);
    /* EXIT */
    else if (ft_strncmp("exit ", line, 4) == 0)
        exit_cmd(line);
    else
        ft_putstr_fd("WRONG\n", 2);
}



int main(int ac, char **av, char **env)
{
    //dup2(fd, 0);
    (void)ac;
    (void)av;
    size_t  i; // int is enough
    char    *line;

    i = 0;
    while (env[i])
        i++;
    global_env = (char **)malloc(sizeof(char) * (i + 1)); //env
    if (!global_env)
        return (0);
    i = 0;
    while (env[i])
    {
        global_env[i] = ft_strdup(env[i]);
        i++;
    }
    global_env[i] = 0;
    //get_env_value("USER");
    //get_env_value("USERR");
    while (1)
    {
        ft_putstr_fd("\e[32m", 1);
        ft_putstr_fd(get_env_value("USER"), 1);
        ft_putstr_fd("@\e[0m:", 1);
        ft_putstr_fd("\e[34m", 1);
        //ft_putstr_fd(get_env_value("PWD"), 1);
        ft_putstr_fd(getcwd(NULL, 0), 1);
        ft_putstr_fd("$\e[0m ", 1);
        line  = get_next_line(STDIN);
        if (!line)
        {
            /**/
            exit(0);
        }
        parse_builtins(line);
        free(line);
        line = NULL;
    }
    exit(0);
}

 
// some notes here :
/* --------------- */
// function called loop => while (get_next_line(0, &line) > 0)
// int the last of this while there is a function called ft_exit()
// the ft_exit() frees global vars and free the allocated vars, struct and lists then it will exit
   // depending on the g_status like this: exit(global_status)
// free the line in loop_prompt()
// handle syntax error when taking inputs from stdin
// handle signals as well (SIGINT)

// i changed a simple trick in my ft_strdup function, i have to check it later
//getenv
//setenv