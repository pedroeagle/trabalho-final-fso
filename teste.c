#include <stdio.h>
#include <string.h>
#include <sys/dir.h>
#include <unistd.h>

#define PATH_MAX 4096
char DIRECTORIES[1000000][200];
long long int DIR_POS = 0;
char DIRECTORY_NAME[1000000];
char ZIP_NAME[1000000];
char TEMP_NAME[1000000];
FILE *terminal;


int main( int argc, char *argv[ ]){
    strcpy(DIRECTORY_NAME, argv[1]); //Caminho atual
    char tar[255] = "tar cf ";
    strcpy(ZIP_NAME, DIRECTORY_NAME);
    strcat(ZIP_NAME, ".tar");
    strcat(tar, ZIP_NAME);
    strcat(tar, " ");
    strcat(tar, DIRECTORY_NAME);
    printf("%s\n", tar);
    terminal = popen(tar, "r");
    char *path;
    while(fgets(path, PATH_MAX, terminal) != NULL);
    /*//strcpy(ZIP_NAME, argv[2]); //Nome do arquivo final
    int size = strlen(ZIP_NAME);  //Quantidade de caracteres arquivo 
    strcpy(TEMP_NAME, ZIP_NAME); // TEMP_NAME == ZIP_NAME
    TEMP_NAME[size-4]='\0'; //Quebra linha no antes do .tar
    find_files(DIRECTORY_NAME);
    char tar[255] = "tar cf ";
    rename(DIRECTORY_NAME, TEMP_NAME); //Altera o nome do arquivo recebi para o arquivo de saida
    strcpy(DIRECTORY_NAME, TEMP_NAME);  
    //strcpy(DIRECTORY_NAME, ZIP_NAME);
    strcat(tar, ZIP_NAME);
    strcat(tar, " ");
    strcat(tar, DIRECTORY_NAME);
    terminal = popen(tar, "r");
    char *path;
    while(fgets(path, PATH_MAX, terminal) != NULL);
    remove_dirs(DIRECTORY_NAME);*/
    return 0;
}