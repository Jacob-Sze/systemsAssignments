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

int main(int argc, char **argv)
{
    DIR *dir;
    struct dirent *dp;
    int output = open("output.txt", O_WRONLY | O_CREAT, 0644);
    List *node = NULL;
    for (int i = 1; i < argc; i++)
    {
        if ((dir = opendir(argv[i])) != NULL)
        {

            while ((dp = readdir(dir)) != NULL)
            {
                struct stat st;
                stat(dp->d_name, &st);
                if (S_ISREG(st.st_mode))
                {
                    char *a = dp->d_name;
                    char buffer[10];

                    int b = strlen(a);
                    if (b > 4 && a[b - 1] == 't' && a[b - 2] == 'x' && a[b - 3] == 't' && a[b - 4] == '.' && a[0] != '.')
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
            }
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
        List* maxNode = node;
        List* prevMax = NULL;
        List* prev = node;
        List* cur = node -> next;
        while(cur != NULL){
            if((maxNode->count==cur->count && strcmp(maxNode->key, cur->key)>0) || maxNode->count<cur->count){
                maxNode = cur;
                prevMax = prev;   
            }
            cur = cur -> next;
            prev = prev ->next;
        }
        int length = snprintf(NULL, 0, "%s %d\n", maxNode->key, maxNode->count);
        char* a = malloc((length+1) * sizeof(char));
        snprintf(a, length+1, "%s %d\n", maxNode->key, maxNode->count);
        write(output, a, length);
        if(maxNode == node){
            node = node->next;
        }else{
            prevMax->next = prevMax->next->next;
        }
        free(maxNode->key);
        free(maxNode);
        free(a);
    }
    close(output);
    return EXIT_SUCCESS;
}
