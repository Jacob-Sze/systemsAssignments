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

typedef struct list{
    char* key;
    int count;
    struct list* next;
} List;

List* allocateNode(char* key){
    List* node = malloc(sizeof(List));
    node -> key = key;
    node -> count = 1;
    node -> next = NULL;
    return node;
}

int main(int argc, char **argv)
{
    DIR *dir;
    struct dirent *dp;
    if ((dir = opendir(".")) == NULL)
    {
        perror("Cannot open .");
        exit(1);
    }
    int output = open("output.txt", O_WRONLY |O_CREAT, 0644);
    List* node = NULL;
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
                if (fd == -1)
                {
                    perror("open");
                    return 1;
                }
                int count = 0;
                while(read(fd, buffer, 1) != 0){
                    count++;
                }
                close(fd);
                char* fileData = malloc((sizeof(char)*count)+1);
                int fdTwo = open(a, O_RDONLY);
                read(fdTwo, fileData, count);
                fileData[count] = '\0';
                for(int i = 0; i<count;i++){
                    if(fileData[i] == '-'){
                        if(i == count-1){
                            fileData[i] = ' ';
                        }else if(i == 0 || fileData[i+1] == '-' || ((!(((int)fileData[i-1])>=65 && ((int)fileData[i-1])<=90)) && (!(((int)fileData[i-1])>=97 && ((int)fileData[i-1])<=122)) && !(fileData[i-1]=='\'')) || ((!(((int)fileData[i+1])>=65 && ((int)fileData[i+1])<=90)) && (!(((int)fileData[i+1])>=97 && ((int)fileData[i+1])<=122)) && !(fileData[i+1]=='\''))){
                            while(fileData[i] == '-'){
                                fileData[i] = ' ';
                                i++;
                            }
                        }
                    }
                }
                char* token = strtok(fileData, " !\"#$%&()*+,./0123456789:;<=>?@[\\]^_`{|}~\n");
                while(token != NULL){
                    if(node == NULL){
                        char* temp = malloc((strlen(token)*sizeof(char))+1);
                        memcpy(temp, token, strlen(token));
                        temp[strlen(token)] = '\0';
                        node = allocateNode(temp);
                    }else{
                        List* cur = node;
                        int checker = 0;
                        while(cur -> next != NULL){
                            if(strcmp(token, cur->key) == 0){
                                checker = 1;
                                cur->count++;
                            }
                            cur = cur->next;
                        }
                        if(checker == 0){
                            char* temp = malloc(strlen(token)*sizeof(char) + 1);
                            memcpy(temp, token, strlen(token));
                            temp[strlen(token)] = '\0';
                            cur -> next = allocateNode(temp);
                        }
                    }
                    token = strtok(0, " !\"#$%&()*+,./0123456789:;<=>?@[\\]^_`{|}~\n");
                }
                close(fdTwo);
                free(fileData);
            }
        }
    }
    List* cur = node;
    while(cur != NULL){
        write(output, cur->key, strlen(cur->key));
        write(output, " ", 1);
        char buffer[1000];
        snprintf(buffer, sizeof(buffer), "%d", cur->count);
        write(output, buffer, strlen(buffer));
        write(output, "\n", 1);
        cur = cur -> next;
    }
    while(node != NULL){
        List* prev = node;
        node = node->next;
        if(prev -> key){
            free(prev->key);
        }
        free(prev);
    }
    closedir(dir);
    close(output);
    return EXIT_SUCCESS;
}