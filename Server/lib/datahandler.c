#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>

#define MAX_FILE_SIZE 10485760

struct info {
   char * red_path;
   char * green_path;
   char * blue_path;
   char * mean_path;
   char * median_path;
   char * record_path;
   int port;
};

typedef struct info conf;

char * findRight(char * string, char * text, int occurrence){
    int string_counter = 0;
    for(int text_counter = 0; text_counter < strlen(text); text_counter ++){
        if(*(string + string_counter) == *(text + text_counter)){
            string_counter ++;
            if(string_counter == strlen(string)){
                if((occurrence--) == 0){
                    return (text + text_counter + 1);
                }
            }
        }else{
            string_counter = 0;
        }       
    }
    return NULL;
}

char * findLeft(char * string, char * text, int occurrence){
    char * text_end = findRight(string, text, occurrence);
    if(text_end != NULL){
        char * split_string = (char *) calloc(text_end - text, sizeof(char));
        for(int c = 0; c < (text_end - text) - 1; c++) {
            *(split_string + c) = *(text + c);
        }
        return split_string;
    }
    return NULL;
}

char * findBetween(char * start, char * end, char * text, int occurrence){
    char * start_text, * end_text;
    if((start_text = findRight(start, text, occurrence)) != NULL){
        if((end_text = findLeft(end, start_text, occurrence)) != NULL){
            return end_text;
        } else {
            return start_text;
        }
    } else {
        return NULL;
    }
}

char * mergeString(char * string1, char * string2){
    int len1 = strlen(string1), len2 = strlen(string2);
    char * merge_string = (char *) calloc(len1 + len2, sizeof(char));
    for(int c1 = 0; c1 < len1; c1++)
    {
        * (merge_string + c1) = * (string1 + c1);
    }
    for(int c2 = 0; c2 < len2; c2++)
    {
        * ((merge_string + len1) + c2) = * (string2 + c2);
    }
    return merge_string;
}

void createDirectory(char * path){
    printf("The directory %s does not exist. Creating it.\n", path);
    if(mkdir(path, 0777) == -1) {
        printf("Error creating %s dir.\n", path);
        exit(EXIT_FAILURE);
    }
}

/*  
*   C faster way to check if a directory exists
*   This code is a modification obtained at the website https://stackoverflow.com/questions/9314586/c-faster-way-to-check-if-a-directory-exists/9314627
*   Creator: Johannes Weiss, Freb 16, 2012.
*/
void checkDirectory(char * path){
    // Using stat system call for getting directory information
    struct stat s;
    // If stat system call returns -1 the directory does not exists. If it is not -1 the directory exists. 
    if(stat(path, &s) == -1) {
        createDirectory(path);
    } else {
        if S_ISDIR(s.st_mode)
            printf("The directory already exists.\n");
        else
            createDirectory(path);
    }
}

void checkPath(char * path){
    int occurrence_counter = 0;
    char * temp_path;
    while((temp_path = findLeft("/", path, occurrence_counter++)) != NULL){
        checkDirectory(temp_path);
    }
    checkDirectory(path);
}

char * readFile(char * name){
    // Reserving memory to read the file.
    char * file_memory = (char *) calloc(MAX_FILE_SIZE, sizeof(char));
    // Openning the html document in read-only mode.
    int fd = open(name, O_RDONLY); 
    // Reding the whole file.
    read(fd, file_memory, MAX_FILE_SIZE); 
    // Closing read file.
    close(fd);
    // Getting file lenght.
    int file_size = strlen(file_memory);
    // Reserving the necessary memory.
    char * file_string = (char *) calloc(file_size, sizeof(char));
    // Coping file_memory to file_string.
    for (int c = 0; c < file_size; c++){
        * (file_string + c) = * (file_memory + c);
    }
    // Setting free the unnecessary memory.
    free(file_memory);

    return file_string;
}

char * writeFile(char* name, char * data){
    // Openning file in read-only mode. If file does not exist, it will be generated.
    int fd = open(name, O_CREAT | O_WRONLY, 0644);
    // Writing the data.
    write(fd, data, strlen(data));
    // Closing written file.
    close(fd);
    printf("------------------------------------------------------------\n%s\n------------------------------------------------------------\nThis has been witten at %s\n------------------------------------------------------------\n", data, name);
}

char * getProperty(char * property, char * data){
    char * temp_property = mergeString(property, ": ");
    return findBetween(temp_property, "\n", data, 0);
}

void setConfigurationFileData(conf * info){
    char * config_file;
    // Using access to know the file existance
    if (access("config.conf", F_OK) == -1){
        config_file = "PORT: 1717\nCOLOR_DIR: color\nRECORD_DIR: record\nFILTER_DIR: filter";
        printf("Configuration file does not exist. Creating a generic one.\n");
        writeFile("config.conf", config_file);
    }
    else{
        config_file = readFile("config.conf");
        printf("Configuration file successsfuly read. Everything has been configured.\n");
    }
    // Setting all conf struct values for being used by the server.
    info->port = atoi(getProperty("PORT",config_file));
    info->red_path = mergeString(getProperty("COLOR_DIR", config_file),"/red");
    info->green_path = mergeString(getProperty("COLOR_DIR", config_file),"/green");
    info->blue_path = mergeString(getProperty("COLOR_DIR", config_file),"/blue");
    info->mean_path = mergeString(getProperty("FILTER_DIR", config_file),"/mean");
    info->median_path = mergeString(getProperty("FILTER_DIR", config_file),"/median");
    info->record_path = getProperty("RECORD_DIR", config_file);
    // Checking all paths. If they don't exists, they will be generated.
    checkPath(info->red_path);
    checkPath(info->green_path);
    checkPath(info->blue_path);
    checkPath(info->mean_path);
    checkPath(info->median_path);
    checkPath(info->record_path);
}

char * getJSON(char * response){
    return findRight(response, "\n\n", 0);
}