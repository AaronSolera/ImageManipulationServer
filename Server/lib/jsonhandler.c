#include <string.h>
#include <stdlib.h>

char * find(char * string, char * text){
    int string_counter = 0;
    for(int text_counter = 0; text_counter < strlen(text); text_counter ++){
        if(*(string + string_counter) == *(text + text_counter)){
            string_counter ++;
            if(string_counter == strlen(string)){
                return (text + text_counter + 1);
            }
        }else{
            string_counter = 0;
        }       
    }
    return NULL;
}

char * findUntil(char * string, char * text){
    char * text_end = find(string, text);
    char * split_string = (char *) calloc(text_end - text, sizeof(char));
    for(int c = 0; c < (text_end - text) - 1; c++) {
        *(split_string + c) = *(text + c);
    }
    return split_string;
}

char * findBetween(char * start, char * end, char * text){
    char * start_text = find(start, text);
    if(start_text != NULL){
        char * end_text = find(end, start_text) - strlen(end);
        if(end_text != NULL){
            int size = (end_text - start_text);
            char * new_text = (char *) calloc(size, sizeof(char));
            while(start_text < end_text){
                * (new_text ++) = * (start_text ++);
            }
            return new_text - size;
        }
        return NULL;
    }
    return NULL;
}

char * getResponseProperty(char * property, char * response){
    return findBetween(property, "\n", response);
}

char * getJSON(char * response){
    return find(response, "\n\n");
}