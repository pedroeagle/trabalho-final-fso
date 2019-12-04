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

void copy_files(char *dirname){
    DIR *dir;
    struct dirent *dirp;
    dir=opendir(dirname);
    chdir(dirname);
    
    while((dirp=readdir(dir))!=NULL){
        if(dirp->d_type==4){
            if(strcmp(dirp->d_name, ".")==0 || strcmp(dirp->d_name, "..")==0){
                continue;
            }
            //printf("%s %s\n", "FOLDER", dirp->d_name);
            find_files(dirp->d_name);
        }
        else{
            char zip[255] = "bzip2 ";
            strcat(zip, dirp->d_name);
            //printf("%s\n", zip);
            char path[PATH_MAX];
            terminal = popen(zip, "r");
            while(fgets(path, PATH_MAX, terminal) != NULL);
            //printf("%s %s\n", "FILE", dirp->d_name);
        }
        strcpy(DIRECTORIES[DIR_POS], dirp->d_name);
        DIR_POS++;
    }
    chdir("..");
    closedir(dir);
}
void find_files(char *dirname){
    DIR *dir;
    struct dirent *dirp;
    dir=opendir(dirname);
    chdir(dirname);
    
    while((dirp=readdir(dir))!=NULL){
        
        if(dirp->d_type==4){
            if(strcmp(dirp->d_name, ".")==0 || strcmp(dirp->d_name, "..")==0){
                continue;
            }
            //printf("%s %s\n", "FOLDER", dirp->d_name);
            find_files(dirp->d_name);
        }
        else{
            char zip[255] = "bzip2 ";
            strcat(zip, dirp->d_name);
            //printf("%s\n", zip);
            char path[PATH_MAX];
            terminal = popen(zip, "r");
            while(fgets(path, PATH_MAX, terminal) != NULL);
            //printf("%s %s\n", "FILE", dirp->d_name);
        }
        strcpy(DIRECTORIES[DIR_POS], dirp->d_name);
        DIR_POS++;
    }
    chdir("..");
    closedir(dir);
}

void remove_dirs(char *dirname){
    DIR *dir;
    struct dirent *dirp;
    dir=opendir(dirname);
    chdir(dirname);
    
    while((dirp=readdir(dir))!=NULL){
        
        if(dirp->d_type==4){
            if(strcmp(dirp->d_name, ".")==0 || strcmp(dirp->d_name, "..")==0){
                continue;
            }
            remove_dirs(dirp->d_name);
        }
        else{
            remove(dirp->d_name);
        }
        strcpy(DIRECTORIES[DIR_POS], dirp->d_name);
        DIR_POS++;
    }
    chdir("..");
    closedir(dir);
    rmdir(dirname);
}

int main( int argc, char *argv[ ]){
    strcpy(DIRECTORY_NAME, argv[1]); //Caminho atual
    strcpy(ZIP_NAME, argv[2]); //Nome do arquivo final
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
    remove_dirs(DIRECTORY_NAME);
    return 0;
}