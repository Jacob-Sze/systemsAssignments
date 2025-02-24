#include <signal.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <linux/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <errno.h>
#include <dirent.h>
#include <libgen.h>

char **wildCard(char *input)
{
    char *tv = input;
    char **storage = malloc(sizeof(char *));
    storage[0] = NULL;
    int counter = 1;
    while (strstr(tv, "/"))
    {
        tv = strstr(tv, "/") + 1;
    }
    char *tvTwo = malloc((strlen(tv) + 1) * sizeof(char));
    strncpy(tvTwo, tv, strlen(tv));
    tvTwo[strlen(tv)] = '\0';
    if (tvTwo[0] == '*')
    {
        tvTwo = tvTwo + 1;
        char *path = malloc(strlen(input) - strlen(tvTwo) + 10);
        strncpy(path, input, strlen(input) - strlen(tvTwo) - 1);
        path[strlen(input) - strlen(tvTwo) - 1] = '\0';
        if (strlen(input) - strlen(tvTwo) == 1)
        {
            strcpy(path, "./");
        }
        DIR *dir = opendir(path);
        struct dirent *dp;
        while ((dp = readdir(dir)) != NULL)
        {
            struct stat st;
            char *file = malloc(strlen(dp->d_name) + strlen(path) + 1);
            strcpy(file, path);
            strcat(file, dp->d_name);
            stat(file, &st);
            if (S_ISREG(st.st_mode))
            {
                if (strstr(dp->d_name, tvTwo) && dp->d_name[0] != '.' && strlen(strstr(dp->d_name, tvTwo)) == strlen(tvTwo))
                {
                    storage[counter - 1] = file;
                    counter++;
                    storage = realloc(storage, counter * sizeof(char *));
                    storage[counter - 1] = NULL;
                }
                else
                {
                    free(file);
                }
            }
            else
            {
                free(file);
            }
        }
        closedir(dir);
        free(path);
        tvTwo -= 1;
        free(tvTwo);
        if (counter == 1)
        {
            counter++;
            storage = realloc(storage, counter * sizeof(char *));
            storage[counter - 2] = malloc(strlen(input) + 1);
            strcpy(storage[counter - 2], input);
            storage[counter - 1] = NULL;
            return storage;
        }
        else
        {
            return storage;
        }
    }
    char *end = strtok(tvTwo, "*");
    char *start = malloc(strlen(tvTwo) + 1);
    strcpy(start, end);
    end = strtok(0, "*");
    if (end == NULL)
    {
        char *path = malloc(strlen(input) - strlen(start) + 5);
        strncpy(path, input, strlen(input) - strlen(start) + 1);
        path[strlen(input) - strlen(start) - 1] = '\0';
        if (strlen(input) - strlen(start) == 1)
        {
            strcpy(path, "./");
        }
        DIR *dir = opendir(path);
        struct dirent *dp;
        while ((dp = readdir(dir)) != NULL)
        {
            struct stat st;
            char *file = malloc(strlen(dp->d_name) + strlen(path) + 1);
            char *dpFile = dp->d_name;
            strcpy(file, path);
            strcat(file, dp->d_name);
            stat(file, &st);
            if (S_ISREG(st.st_mode))
            {
                if (strstr(dpFile, start) && strstr(dpFile, start) == dpFile)
                {
                    storage[counter - 1] = file;
                    counter++;
                    storage = realloc(storage, counter * sizeof(char *));
                    storage[counter - 1] = NULL;
                }
                else
                {
                    free(file);
                }
            }
            else
            {
                free(file);
            }
        }
        free(path);
        free(start);
        free(tvTwo);
        closedir(dir);
        if (counter == 1)
        {
            counter++;
            storage = realloc(storage, counter * sizeof(char *));
            storage[counter - 2] = malloc(strlen(input) + 1);
            strcpy(storage[counter - 2], input);
            storage[counter - 1] = NULL;
            return storage;
        }
        else
        {
            return storage;
        }
    }
    else
    {
        char *path = malloc(strlen(input) - strlen(start) - strlen(end) + 5);
        strncpy(path, input, strlen(input) - strlen(start) - strlen(end) - 1);
        path[strlen(input) - strlen(start) - strlen(end) - 1] = '\0';
        if (strlen(input) - strlen(start) - strlen(end) == 1)
        {
            strcpy(path, "./");
        }
        DIR *dir = opendir(path);
        struct dirent *dp;
        while ((dp = readdir(dir)) != NULL)
        {
            struct stat st;
            char *file = malloc(strlen(dp->d_name) + strlen(path) + 1);
            char *dpFile = dp->d_name;
            strcpy(file, path);
            strcat(file, dp->d_name);
            stat(file, &st);
            if (S_ISREG(st.st_mode))
            {
                if (strstr(dpFile, start) && strstr(dpFile, start) == dpFile && strstr(dpFile, end) && strlen(strstr(dpFile, end)) == strlen(end))
                {
                    storage[counter - 1] = file;
                    counter++;
                    storage = realloc(storage, counter * sizeof(char *));

                    storage[counter - 1] = NULL;
                }
                else
                {
                    free(file);
                }
            }
            else
            {
                free(file);
            }
        }
        closedir(dir);
        free(path);
        free(tvTwo);
        free(start);
        if (counter == 1)
        {
            counter++;
            storage = realloc(storage, counter * sizeof(char *));
            storage[counter - 2] = malloc(strlen(input) + 1);
            strcpy(storage[counter - 2], input);
            storage[counter - 1] = NULL;
            return storage;
        }
        else
        {
            return storage;
        }
    }
}

void sig_handle(int sig){
    printf("mysh: Terminated by signal: %d\n", sig);
}

int main(int argv, char **argc)
{
    for (int i = 0; i < 32; i++)
    {
        if (i != 17)
        {
            signal(i, sig_handle);
        }
    }
    char *command = malloc(1);
    int batch = 0;
    int check = 0;
    if (argv == 2 || isatty(STDIN_FILENO) == 0)
    {
        batch = 1;
    }
    else
    {
        printf("Welcome to my shell!\n");
    }
    int readFile = STDIN_FILENO;

    do
    {
    restart:
        if (batch != 1)
        {
            printf("mysh> ");
            fflush(stdout);
        }
        if (argv == 2 && check == 0)
        {
            readFile = open(argc[1], O_RDONLY);
            check = 1;
        }
        int count = 1;
        command[0] = '\0';
        char buffer[1];
        int c;
        while ((c = read(readFile, buffer, 1)) > 0)
        {
            if (buffer[0] == '\n')
            {
                break;
            }

            count++;
            command = realloc(command, count);
            command[count - 2] = buffer[0];
            command[count - 1] = '\0';
        }
        if (strstr(command, "|"))
        {

            char *savePoint, savePointTwo;
            char *processes = __strtok_r(command, "|", &savePoint);
            char *input = NULL;
            char *output = NULL;
            int fd[2];
            if (pipe(fd) == -1)
            {
                perror("mysh");
                if(batch != 1){
                    printf("mysh: Command Failed: code 1\n");
                }
                goto restart;
            };
            if (strstr(processes, "<"))
            {
                char *loop = strstr(processes, "<") + 1;
                while (loop[0] == ' ')
                {
                    loop += 1;
                }
                int i = 0;
                while (loop[i] != '\0' && loop[i] != ' ')
                {
                    i++;
                }
                input = malloc(i + 1);
                strncpy(input, loop, i);
                input[i] = '\0';
                for (int j = 0; j < i; j++)
                {
                    loop[j] = ' ';
                }
            }
            char **storage = malloc(sizeof(char *) * 2);
            char *processesTwo;
            if (strstr(processes, " "))
            {
                processesTwo = __strtok_r(processes, " ", &savePointTwo);
            }
            else
            {
                processesTwo = processes;
            }
            char *file = malloc(strlen(processesTwo) + 50);
            strcpy(file, processesTwo);
            if (!strstr(file, "/"))
            {
                char *helper = malloc(strlen(processesTwo) + 30);
                strcpy(helper, "/usr/local/bin/");
                strcat(helper, processesTwo);
                if (access(helper, F_OK) >= 0)
                {
                    strcpy(file, helper);
                }
                else
                {
                    strcpy(helper, "/usr/bin/");
                    strcat(helper, processesTwo);
                    if (access(helper, F_OK) >= 0)
                    {
                        strcpy(file, helper);
                    }
                    else
                    {
                        strcpy(helper, "/bin/");
                        strcat(helper, processesTwo);
                        if (access(helper, F_OK) >= 0)
                        {
                            strcpy(file, helper);
                        }
                    }
                }
                free(helper);
            }
            storage[0] = file;
            storage[1] = NULL;
            int counter = 2;
            processesTwo = __strtok_r(0, " <>", &savePointTwo);
            while (processesTwo != NULL)
            {
                if (strstr(processesTwo, "*"))
                {
                    char **temp = wildCard(processesTwo);
                    int length = 0;
                    for (int i = 0; temp[i] != NULL; i++)
                    {
                        length++;
                    }
                    counter += length;
                    storage = realloc(storage, sizeof(char *) * counter);
                    int counting = 0;
                    for (int k = (counter - 1) - length; k < counter - 1; k++)
                    {
                        storage[k] = temp[counting];
                        counting++;
                    }
                    storage[counter - 1] = NULL;
                    free(temp);
                }
                else
                {
                    counter++;
                    storage = realloc(storage, sizeof(char *) * counter);
                    storage[counter - 2] = malloc(strlen(processesTwo) + 1);
                    strcpy(storage[counter - 2], processesTwo);
                    storage[counter - 1] = NULL;
                }
                processesTwo = __strtok_r(0, " <>", &savePointTwo);
            }
            int checker = 0;
            pid_t pidOne = fork();
            if (pidOne == 0)
            {
                setpgid(0, 0);
                close(fd[0]);
                int fpTwo;
                if (input)
                {
                    fpTwo = open(input, O_RDONLY);
                    if (fpTwo > 0)
                    {
                        dup2(fpTwo, STDIN_FILENO);
                        close(fpTwo);
                    }
                }
                dup2(fd[1], STDOUT_FILENO);
                close(fd[1]);
                if (execv(file, storage) == -1)
                {
                    perror("mysh");
                    exit(1);
                }
            }
            setpgid(pidOne, pidOne);
            for (int i = 1; i < counter - 1; i++)
            {
                free(storage[i]);
            }
            free(storage);
            free(file);
            processes = __strtok_r(0, "|", &savePoint);
            if (strstr(processes, ">"))
            {
                char *loop = strstr(processes, ">") + 1;
                while (loop[0] == ' ')
                {
                    loop += 1;
                }
                int i = 0;
                while (loop[i] != '\0' && loop[i] != ' ' && loop[i] != 2)
                {
                    i++;
                }
                output = malloc(i + 1);
                strncpy(output, loop, i);
                output[i] = '\0';
                for (int j = 0; j < i; j++)
                {
                    loop[j] = ' ';
                }
            }
            storage = malloc(sizeof(char *) * 2);
            if (strstr(processes, " "))
            {
                processesTwo = __strtok_r(processes, " ", &savePointTwo);
            }
            else
            {
                processesTwo = processes;
            }
            file = malloc(strlen(processesTwo) + 50);
            strcpy(file, processesTwo);
            if (!strstr(file, "/"))
            {
                char *helper = malloc(strlen(processesTwo) + 30);
                strcpy(helper, "/usr/local/bin/");
                strcat(helper, processesTwo);
                if (access(helper, F_OK) >= 0)
                {
                    strcpy(file, helper);
                }
                else
                {
                    strcpy(helper, "/usr/bin/");
                    strcat(helper, processesTwo);
                    if (access(helper, F_OK) >= 0)
                    {
                        strcpy(file, helper);
                    }
                    else
                    {
                        strcpy(helper, "/bin/");
                        strcat(helper, processesTwo);
                        if (access(helper, F_OK) >= 0)
                        {
                            strcpy(file, helper);
                        }
                    }
                }
                free(helper);
            }
            storage[0] = file;
            storage[1] = NULL;
            counter = 2;
            processesTwo = __strtok_r(0, " <>", &savePointTwo);
            while (processesTwo != NULL)
            {
                if (strstr(processesTwo, "*"))
                {
                    char **temp = wildCard(processesTwo);
                    int length = 0;
                    for (int i = 0; temp[i] != NULL; i++)
                    {
                        length++;
                    }
                    counter += length;
                    storage = realloc(storage, sizeof(char *) * counter);
                    int counting = 0;
                    for (int k = (counter - 1) - length; k < counter - 1; k++)
                    {
                        storage[k] = temp[counting];
                        counting++;
                    }
                    storage[counter - 1] = NULL;
                    free(temp);
                }
                else
                {
                    counter++;
                    storage = realloc(storage, sizeof(char *) * counter);
                    storage[counter - 2] = malloc(strlen(processesTwo) + 1);
                    strcpy(storage[counter - 2], processesTwo);
                    storage[counter - 1] = NULL;
                }
                processesTwo = __strtok_r(0, " <>", &savePointTwo);
            }
            checker = 0;
            int fpTwo;
            pid_t pidTwo = fork();
            if (pidTwo == 0)
            {
                setpgid(0, 0);
                close(fd[1]);
                if (output)
                {
                    fpTwo = open(output, O_WRONLY | O_TRUNC | O_CREAT, 0644);
                    if (fpTwo > 0)
                    {
                        dup2(fpTwo, STDOUT_FILENO);
                        close(fpTwo);
                    }
                }
                dup2(fd[0], STDIN_FILENO);
                close(fd[0]);
                if (execv(file, storage) == -1)
                {
                    perror("mysh");
                    exit(1);
                }
            }
            setpgid(pidTwo, pidTwo);
            close(fd[0]);
            close(fd[1]);
            int status = 0;
            int statusTwo = 0;
            waitpid(pidOne, &status, 0);
            if (WIFSIGNALED(status) && WTERMSIG(status) != 0)
            {
                printf("mysh: Terminated by signal: %d\n", WTERMSIG(status));
            }
            waitpid(pidTwo, &statusTwo, 0);
            if (WIFEXITED(statusTwo) && WEXITSTATUS(statusTwo) != 0 && batch != 1)
            {
                printf("mysh: Command Failed: code %d\n", WEXITSTATUS(statusTwo));
            }
            else if (WIFSIGNALED(status) && WTERMSIG(statusTwo) != 0)
            {
                printf("mysh: Terminated by signal: %d\n", WTERMSIG(statusTwo));
            }
            for (int i = 1; i < counter; i++)
            {
                free(storage[i]);
            }
            free(storage);
            free(file);
            if (input)
            {
                free(input);
            }
            if (output)
            {
                free(output);
            }
        }
        else
        {
            if (strlen(command) == 0)
            {
                goto restart;
            }
            char *savePoint;
            char *processes = __strtok_r(command, " ", &savePoint);
            if (strcmp(processes, "cd") == 0)
            {
                processes = __strtok_r(0, " ", &savePoint);
                if (processes == NULL)
                {
                    printf("cd: Wrong Number of Arguments\n");
                    goto restart;
                }
                char *temp = processes;
                processes = __strtok_r(0, " ", &savePoint);
                if (processes != NULL)
                {
                    printf("cd: Wrong Number of Arguments\n");
                    goto restart;
                }
                if (chdir(temp) == -1)
                {
                    printf("cd: %s\n", strerror(errno));
                };
            }
            else if (strcmp(processes, "pwd") == 0)
            {
                char *a = NULL;
                a = getcwd(a, 0);
                printf("%s\n", a);
                free(a);
            }
            else if (strcmp(processes, "which") == 0)
            {
                processes = __strtok_r(0, " ", &savePoint);
                if (processes == NULL)
                {
                    goto restart;
                }
                char *file = malloc(strlen(processes) + 2);
                strcpy(file, processes);
                processes = __strtok_r(0, " ", &savePoint);
                if (processes != NULL)
                {
                    goto restart;
                }
                char *helper = malloc(strlen(file) + 30);
                strcpy(helper, "/usr/local/bin/");
                strcat(helper, file);
                if (access(helper, F_OK) >= 0)
                {
                    printf("\n%s", helper);
                }
                else
                {
                    strcpy(helper, "/usr/bin/");
                    strcat(helper, file);
                    if (access(helper, F_OK) >= 0)
                    {
                        printf("\n%s", helper);
                    }
                    else
                    {
                        strcpy(helper, "/bin/");
                        strcat(helper, file);
                        if (access(helper, F_OK) >= 0)
                        {
                            printf("\n%s", helper);
                        }
                        else
                        {
                            perror("mysh");
                        }
                    }
                }
                free(helper);
            }
            else if (strcmp(processes, "exit") == 0)
            {
                printf("%s", savePoint);
                if (batch == 0)
                {
                    printf("\nExiting my shell\n");
                }
                free(command);
                return 0;
            }
            else
            {
                char *output = NULL;
                char *input = NULL;
                if (strstr(savePoint, ">"))
                {
                    char *loop = strstr(savePoint, ">") + 1;
                    while (loop[0] == ' ')
                    {
                        loop += 1;
                    }
                    int i = 0;
                    while (loop[i] != '\0' && loop[i] != ' ' && loop[i] != '<')
                    {
                        i++;
                    }
                    output = malloc(i + 1);
                    strncpy(output, loop, i);
                    output[i] = '\0';
                    for (int j = 0; j < i; j++)
                    {
                        loop[j] = ' ';
                    }
                }
                if (strstr(savePoint, "<"))
                {
                    char *loop = strstr(savePoint, "<") + 1;
                    while (loop[0] == ' ')
                    {
                        loop += 1;
                    }
                    int i = 0;
                    while (loop[i] != '\0' && loop[i] != ' ')
                    {
                        i++;
                    }
                    input = malloc(i + 1);
                    strncpy(input, loop, i);
                    input[i] = '\0';
                    for (int j = 0; j < i; j++)
                    {
                        loop[j] = ' ';
                    }
                }
                char *file = malloc(strlen(processes) + 50);
                strcpy(file, processes);

                if (!strstr(file, "/"))
                {
                    char *helper = malloc(strlen(file) + 30);
                    strcpy(helper, "/usr/local/bin/");
                    strcat(helper, file);
                    if (access(helper, F_OK) >= 0)
                    {
                        strcpy(file, helper);
                    }
                    else
                    {
                        strcpy(helper, "/usr/bin/");
                        strcat(helper, file);
                        if (access(helper, F_OK) >= 0)
                        {
                            strcpy(file, helper);
                        }
                        else
                        {
                            strcpy(helper, "/bin/");
                            strcat(helper, file);
                            if (access(helper, F_OK) >= 0)
                            {
                                strcpy(file, helper);
                            }
                        }
                    }
                    free(helper);
                }
                char **storage = malloc(sizeof(char *) * 2);
                storage[0] = processes;
                storage[1] = NULL;
                processes = __strtok_r(0, " <>", &savePoint);
                int counter = 2;
                while (processes != NULL)
                {
                    if (strstr(processes, "*"))
                    {
                        char **temp = wildCard(processes);
                        int length = 0;
                        for (int i = 0; temp[i] != NULL; i++)
                        {
                            length++;
                        }
                        counter += length;
                        storage = realloc(storage, sizeof(char *) * counter);
                        int counting = 0;
                        for (int k = (counter - 1) - length; k < counter - 1; k++)
                        {
                            storage[k] = temp[counting];
                            counting++;
                        }
                        storage[counter - 1] = NULL;
                        free(temp);
                    }
                    else
                    {
                        counter++;
                        storage = realloc(storage, sizeof(char *) * counter);
                        storage[counter - 2] = malloc(strlen(processes) + 1);
                        strcpy(storage[counter - 2], processes);
                        storage[counter - 1] = NULL;
                    }
                    processes = __strtok_r(0, " <>", &savePoint);
                }
                int fpOne;
                int fpTwo;
                pid_t pid = fork();
                if (pid == 0)
                {
                    setpgid(0, 0);
                    if (output)
                    {
                        fpOne = open(output, O_WRONLY | O_TRUNC | O_CREAT, 0644);
                        if (fpOne > 0)
                        {
                            dup2(fpOne, STDOUT_FILENO);
                            close(fpOne);
                        }
                    }
                    if (input)
                    {
                        fpTwo = open(input, O_RDONLY);
                        if (fpTwo > 0)
                        {
                            dup2(fpTwo, STDIN_FILENO);
                            close(fpTwo);
                        }
                    }
                    if (execv(file, storage) == -1)
                    {
                        perror("mysh");
                        exit(1);
                    }
                }
                else if (pid > 0)
                {
                    setpgid(pid, pid);
                }
                int statusTwo = 0;
                waitpid(pid, &statusTwo, 0);
                if (WIFEXITED(statusTwo) && WEXITSTATUS(statusTwo) != 0 && batch != 1)
                {
                    printf("mysh: Command Failed: code %d\n", WEXITSTATUS(statusTwo));
                }
                else if (WIFSIGNALED(statusTwo) && WTERMSIG(statusTwo) != 0)
                {
                    printf("mysh: Terminated by signal: %d\n", WTERMSIG(statusTwo));
                }
                free(file);
                if (input)
                {
                    free(input);
                }
                if (output)
                {
                    free(output);
                }
                for (int i = 1; i < counter; i++)
                {
                    free(storage[i]);
                }
                free(storage);
            }
        }
        if (c == 0)
        {
            break;
        }
    } while (1);
    free(command);
}
