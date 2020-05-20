#ifndef _DATAHANDLER_H 
#define _DATAHANDLER_H  

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

void   checkPath(char * path);
char * getProperty(char * property, char * data);
void   setConfigurationFileData(conf * info, char* configFilePath);
char * getJSON(char * response);

#endif