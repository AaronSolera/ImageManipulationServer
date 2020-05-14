#ifndef _JSONHANDLER_H 
#define _JSONHANDLER_H  

char * find(char * string, char * text);
char * findBetween(char * start, char * end, char * text);
char * getResponseProperty(char * property, char * response);
char * getJSON(char * response);

#endif