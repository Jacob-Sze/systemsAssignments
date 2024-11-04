#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <dirent.h>
#include <libgen.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>

typedef struct list
{
    char *key;
    int count;
    struct list *next;
} List;

List *allocateNode(char *key)
{
    List *node = malloc(sizeof(List));
    node->key = key;
    node->count = 1;
    node->next = NULL;
    return node;
}

List *node = NULL;

void directorySearch(char* dirName, DIR* dir){
    struct dirent *dp;
    while ((dp = readdir(dir)) != NULL)
            {
                struct stat st;
                char *name = malloc(strlen(dirName) + strlen(dp->d_name) + 5);
                strcpy(name, dirName);
                if (dirName[strlen(dirName) - 1] != '/')
                {
                    strcat(name, "/");
                }
                strcat(name, dp->d_name);
                strcat(name, "\0");
                stat(name, &st);
                if (S_ISREG(st.st_mode))
                {
                    char *a = name;
                    char buffer[10];
                    char *c = dp->d_name;
                    int b = strlen(c);
                    if (b > 4 && c[b - 1] == 't' && c[b - 2] == 'x' && c[b - 3] == 't' && c[b - 4] == '.' && c[0] != '.')
                    {
                        int fd = open(a, O_RDONLY);
                        int count = 0;
                        while (read(fd, buffer, 1) != 0)
                        {
                            count++;
                        }
                        close(fd);
                        char *fileData = malloc((sizeof(char) * count) + 1);
                        int fdTwo = open(a, O_RDONLY);
                        read(fdTwo, fileData, count);
                        fileData[count] = '\0';
                        for (int i = 0; i < count; i++)
                        {
                            if (fileData[i] == '-')
                            {
                                if (i == count - 1)
                                {
                                    fileData[i] = ' ';
                                }
                                else if (i == 0 || fileData[i + 1] == '-' || ((!(((int)fileData[i - 1]) >= 65 && ((int)fileData[i - 1]) <= 90)) && (!(((int)fileData[i - 1]) >= 97 && ((int)fileData[i - 1]) <= 122)) && !(fileData[i - 1] == '\'')) || ((!(((int)fileData[i + 1]) >= 65 && ((int)fileData[i + 1]) <= 90)) && (!(((int)fileData[i + 1]) >= 97 && ((int)fileData[i + 1]) <= 122)) && !(fileData[i + 1] == '\'')))
                                {
                                    while (fileData[i] == '-')
                                    {
                                        fileData[i] = ' ';
                                        i++;
                                    }
                                }
                            }
                        }
                        char *token = strtok(fileData, " !\"#$%&()*+,./0123456789:;<=>?@[\\]^_`{|}~\n");
                        while (token != NULL)
                        {
                            if (node == NULL)
                            {
                                char *temp = malloc((strlen(token) * sizeof(char)) + 1);
                                memcpy(temp, token, strlen(token));
                                temp[strlen(token)] = '\0';
                                node = allocateNode(temp);
                            }
                            else
                            {
                                List *cur = node;
                                int checker = 0;
                                while (cur->next != NULL)
                                {
                                    if (strcmp(token, cur->key) == 0)
                                    {
                                        checker = 1;
                                        cur->count++;
                                    }
                                    cur = cur->next;
                                }
                                if (strcmp(token, cur->key) == 0)
                                {
                                    checker = 1;
                                    cur->count++;
                                }
                                if (checker == 0)
                                {
                                    char *temp = malloc(strlen(token) * sizeof(char) + 1);
                                    memcpy(temp, token, strlen(token));
                                    temp[strlen(token)] = '\0';
                                    cur->next = allocateNode(temp);
                                }
                            }
                            token = strtok(0, " !\"#$%&()*+,./0123456789:;<=>?@[\\]^_`{|}~\n");
                        }
                        close(fdTwo);
                        free(fileData);
                    }
                }else if(S_ISDIR(st.st_mode) && dp->d_name[0] != '.'){
                    DIR *dirTwo = opendir(name);
                    directorySearch(name, dirTwo);
                    closedir(dirTwo);
                }
                free(name);
            }
}

int main(int argc, char **argv)
{
    DIR *dir;
    int output = open("output.txt", O_WRONLY | O_CREAT, 0644);
    for (int i = 1; i < argc; i++)
    {
        if ((dir = opendir(argv[i])) != NULL)
        {
            directorySearch(argv[i], dir);
            closedir(dir);
        }
        struct stat st;
        stat(argv[i], &st);
        if (S_ISREG(st.st_mode))
        {
            char buffer[1];
            int fd = open(argv[i], O_RDONLY);
            int count = 0;
            while (read(fd, buffer, 1) != 0)
            {
                count++;
            }
            close(fd);
            char *fileData = malloc((sizeof(char) * count) + 1);
            int fdTwo = open(argv[i], O_RDONLY);
            read(fdTwo, fileData, count);
            fileData[count] = '\0';
            for (int i = 0; i < count; i++)
            {
                if (fileData[i] == '-')
                {
                    if (i == count - 1)
                    {
                        fileData[i] = ' ';
                    }
                    else if (i == 0 || fileData[i + 1] == '-' || ((!(((int)fileData[i - 1]) >= 65 && ((int)fileData[i - 1]) <= 90)) && (!(((int)fileData[i - 1]) >= 97 && ((int)fileData[i - 1]) <= 122)) && !(fileData[i - 1] == '\'')) || ((!(((int)fileData[i + 1]) >= 65 && ((int)fileData[i + 1]) <= 90)) && (!(((int)fileData[i + 1]) >= 97 && ((int)fileData[i + 1]) <= 122)) && !(fileData[i + 1] == '\'')))
                    {
                        while (fileData[i] == '-')
                        {
                            fileData[i] = ' ';
                            i++;
                        }
                    }
                }
            }
            char *token = strtok(fileData, " !\"#$%&()*+,./0123456789:;<=>?@[\\]^_`{|}~\n");
            while (token != NULL)
            {
                if (node == NULL)
                {
                    char *temp = malloc((strlen(token) * sizeof(char)) + 1);
                    memcpy(temp, token, strlen(token));
                    temp[strlen(token)] = '\0';
                    node = allocateNode(temp);
                }
                else
                {
                    List *cur = node;
                    int checker = 0;
                    while (cur->next != NULL)
                    {
                        if (strcmp(token, cur->key) == 0)
                        {
                            checker = 1;
                            cur->count++;
                        }
                        cur = cur->next;
                    }
                    if (strcmp(token, cur->key) == 0)
                    {
                        checker = 1;
                        cur->count++;
                    }
                    if (checker == 0)
                    {
                        char *temp = malloc(strlen(token) * sizeof(char) + 1);
                        memcpy(temp, token, strlen(token));
                        temp[strlen(token)] = '\0';
                        cur->next = allocateNode(temp);
                    }
                }
                token = strtok(0, " !\"#$%&()*+,./0123456789:;<=>?@[\\]^_`{|}~\n");
            }
            close(fdTwo);
            free(fileData);
        }
    }
    while (node != NULL)
    {
        List *maxNode = node;
        List *prevMax = NULL;
        List *prev = node;
        List *cur = node->next;
        while (cur != NULL)
        {
            if ((maxNode->count == cur->count && strcmp(maxNode->key, cur->key) > 0) || maxNode->count < cur->count)
            {
                maxNode = cur;
                prevMax = prev;
            }
            cur = cur->next;
            prev = prev->next;
        }
        int length = snprintf(NULL, 0, "%s %d\n", maxNode->key, maxNode->count);
        char *a = malloc((length + 1) * sizeof(char));
        snprintf(a, length + 1, "%s %d\n", maxNode->key, maxNode->count);
        write(output, a, length);
        if (maxNode == node)
        {
            node = node->next;
        }
        else
        {
            prevMax->next = prevMax->next->next;
        }
        free(maxNode->key);
        free(maxNode);
        free(a);
    }
    close(output);
    return EXIT_SUCCESS;
}
