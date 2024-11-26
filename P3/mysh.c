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
        strncpy(path, input, strlen(input) - strlen(tvTwo) + 1);
        path[strlen(input) - strlen(tvTwo)] = '\0';
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
            storage[counter - 1] = input;
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
        char *path = malloc(strlen(input) - strlen(start) + 1);
        strncpy(path, input, strlen(input) - strlen(start) - 1);
        path[strlen(input) - strlen(start) - 1] = '\0';
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
            storage[counter - 1] = input;
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
            storage[counter - 1] = input;
            return storage;
        }
        else
        {
            return storage;
        }
    }
}

int main(int argv, char **argc)
{
    char *command = malloc(1);
    int batch = 0;
    int check = 0;
    if (argv == 2 || isatty(STDIN_FILENO) == 0)
    {
        batch = 1;
    }
    else
    {
        printf("Welcome to my shell!");
    }
    do
    {
    restart:
        if (batch != 1)
        {
            printf("\nmysh> ");
            fflush(stdout);
        }
        int readFile = STDIN_FILENO;
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
            char *input;
            char *output;
            int fd[2];
            int checking = pipe(fd);
            if (checking < 0)
            {
                goto restart;
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
                input = malloc(i);
                strncpy(input, loop, i);
                for (int j = 0; j < i; j++)
                {
                    loop[j] = ' ';
                }
            }
            char **storage = malloc(0);
            char *file = malloc(strlen(processes) + 50);
            strcpy(file, processes);
            if (!strstr(file, "/"))
            {
                char *helper = malloc(strlen(processes) + 30);
                strcpy(helper, "/usr/local/bin/");
                strcat(helper, processes);
                if (access(helper, F_OK) >= 0)
                {
                    strcpy(file, helper);
                }
                else
                {
                    strcpy(helper, "/usr/bin/");
                    strcat(helper, processes);
                    if (access(helper, F_OK) >= 0)
                    {
                        strcpy(file, helper);
                    }
                    else
                    {
                        strcpy(helper, "/bin/");
                        strcat(helper, processes);
                        if (access(helper, F_OK) >= 0)
                        {
                            strcpy(file, helper);
                        }
                        else
                        {
                            printf("mysh: %s\n", strerror(errno));
                            goto restart;
                        }
                    }
                }
            }
            processes = __strtok_r(0, " ", &savePointTwo);
            int counter = 0;
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
                    for (int k = counter - length; k < counter; k++)
                    {
                        storage[k] = temp[counting];
                        counting++;
                    }
                }
                else
                {
                    counter++;
                    storage = realloc(storage, sizeof(char *) * counter);
                    storage[counter - 1] = processes;
                }
                processes = __strtok_r(0, " ", &savePointTwo);
            }
            pid_t pidOne = fork();
            if (pidOne == 0)
            {
                int fpTwo;
                if (input)
                {
                    fpTwo = open(input, O_RDONLY);
                    if (fpTwo > 0)
                    {
                        dup2(fpTwo, STDIN_FILENO);
                    }
                    else
                    {
                        goto restart;
                    }
                    free(input);
                }
                dup2(fd[1], STDOUT_FILENO);
                execv(file, storage);
                free(storage);
                free(file);
            }
            else
            {
                goto restart;
            }
            int status = 0;
            int signals;
            while ((signals = waitpid(pidOne, &status, 0)) > 0)
            {
                if (WIFSIGNALED(status))
                {
                    char *a;
                    psignal(WTERMSIG(status), &a);
                    printf("mysh: Terminated by signal: %s\n", a);
                    close(fd[0]);
                    close(fd[1]);
                    goto restart;
                }
            }
            if (status != 0)
            {
                printf("mysh: Command Failed: code %d\n", status);
                close(fd[0]);
                close(fd[1]);
                goto restart;
            }
            processes = __strtok_r(0, "|", &savePoint);
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
                strncpy(output, loop, i + 1);
                for (int j = 0; j < i; j++)
                {
                    loop[j] = ' ';
                }
            }
            storage = malloc(0);
            file = malloc(strlen(processes) + 50);
            strcpy(file, processes);
            if (!strstr(file, "/"))
            {
                char *helper = malloc(strlen(processes) + 30);
                strcpy(helper, "/usr/local/bin/");
                strcat(helper, processes);
                if (access(helper, F_OK) >= 0)
                {
                    strcpy(file, helper);
                }
                else
                {
                    strcpy(helper, "/usr/bin/");
                    strcat(helper, processes);
                    if (access(helper, F_OK) >= 0)
                    {
                        strcpy(file, helper);
                    }
                    else
                    {
                        strcpy(helper, "/bin/");
                        strcat(helper, processes);
                        if (access(helper, F_OK) >= 0)
                        {
                            strcpy(file, helper);
                        }
                        else
                        {
                            printf("mysh: %s", strerror(errno));
                            fflush(stdout);
                            goto restart;
                        }
                    }
                }
            }
            processes = __strtok_r(0, " ", &savePointTwo);
            counter = 0;
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
                    for (int k = counter - length; k < counter; k++)
                    {
                        storage[k] = temp[counting];
                        counting++;
                    }
                }
                else
                {
                    counter++;
                    storage = realloc(storage, sizeof(char *) * counter);
                    storage[counter - 1] = processes;
                }
                processes = __strtok_r(0, " ", &savePointTwo);
            }
            int fpTwo;
            pid_t pidTwo = fork();
            if (pidTwo == 0)
            {
                if (output)
                {
                    fpTwo = open(output, O_RDONLY);
                    if (fpTwo > 0)
                    {
                        dup2(fpTwo, STDOUT_FILENO);
                    }
                    else
                    {
                        goto restart;
                    }
                    free(output);
                }
                dup2(fd[0], STDIN_FILENO);
                execv(file, storage);
                free(storage);
                free(file);
                close(fpTwo);
            }
            int statusTwo = 0;
            int signalsTwo = 0;
            while ((signalsTwo = waitpid(pidTwo, &statusTwo, 0)) > 0)
            {
                if (WIFSIGNALED(status))
                {
                    char *a;
                    psignal(WTERMSIG(statusTwo), &a);
                    printf("mysh: Terminated by signal: %s", a);
                }
            }
            if (statusTwo != 0)
            {
                printf("mysh: Command Failed: code %d", statusTwo);
            }
            close(fd[0]);
            close(fd[1]);
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
                    goto restart;
                }
                char *temp = processes;
                processes = __strtok_r(0, " ", &savePoint);
                if (processes != NULL)
                {
                    goto restart;
                }
                if (chdir(temp) == -1)
                {
                    printf("cd: %s\n", strerror(errno));
                };
            }
            else if (strcmp(processes, "pwd") == 0)
            {
                printf("%s\n", getcwd(NULL, 0));
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
                            free(file);
                            goto restart;
                        }
                    }
                }
                free(file);
            }
            else if (strcmp(processes, "exit") == 0)
            {
                if (batch == 0)
                {
                    printf("\nExiting my shell\n");
                }
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
                    strncpy(output, loop, i + 1);
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
                    input = malloc(i);
                    strncpy(input, loop, i);
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
                            else
                            {
                                free(helper);
                                free(file);
                                printf("mysh: %s", strerror(errno));
                                goto restart;
                            }
                        }
                    }
                    free(helper);
                }
                char **storage = malloc(sizeof(char *));
                storage[0] = processes;
                processes = __strtok_r(0, " <>", &savePoint);
                int counter = 1;
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
                        for (int k = counter - length; k < counter; k++)
                        {
                            storage[k] = temp[counting];
                            counting++;
                        }
                    }
                    else
                    {
                        counter++;
                        storage = realloc(storage, sizeof(char *) * counter);
                        storage[counter - 1] = processes;
                    }
                    processes = __strtok_r(0, " <>", &savePoint);
                }
                int fpOne;
                int fpTwo;
                pid_t pid = fork();
                if (pid == 0)
                {
                    if (output)
                    {
                        fpOne = open(output, O_WRONLY | O_TRUNC | O_CREAT, 0644);
                        if (fpOne > 0)
                        {
                            dup2(fpOne, STDOUT_FILENO);
                        }
                        else
                        {
                            goto restart;
                        }
                    }
                    if (input)
                    {
                        fpTwo = open(input, O_RDONLY);
                        if (fpTwo > 0)
                        {
                            dup2(fpTwo, STDIN_FILENO);
                        }
                        else
                        {
                            goto restart;
                        }
                    }

                    execv(file, storage);
                }
                else if (pid < 0)
                {
                    goto restart;
                }
                free(file);
                free(input);
                free(output);
                free(storage);
                close(fpOne);
                close(fpTwo);
                int status = 0;
                int signals;
                while ((signals = waitpid(pid, &status, 0)) > 0)
                {
                    if (WIFSIGNALED(status))
                    {
                        char *a;
                        psignal(WTERMSIG(status), &a);
                        printf("mysh: Terminated by signal: %s", a);
                    }
                }
                if (status != 0)
                {
                    printf("mysh: Command Failed: code %d", status);
                }
            }
        }
        if (c == 0)
        {
            break;
        }
    } while (1);
    free(command);
}
