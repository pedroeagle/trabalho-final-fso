#include <stdio.h>
#include <string.h>
#include <sys/dir.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>

#define PATH_MAX 4096
char DIRECTORIES[1000000][200];
char DIRECTORY_NAME[1000000];
char ZIP_NAME[1000000];
char TEMP_NAME[1000000];
long long int FILES_POS = 0;
FILE *terminal;

typedef struct Path{
    struct Path *prev;
    struct Path *next;
    char *dir;
}Path;

Path *FIRST_PATH;
Path *LAST_PATH;

char * GetPath(struct Path *first){
    char *path = malloc(1000000);
    Path *temp = first;
    while(temp){
        strcat(path, "/");
        strcat(path, temp->dir);
        temp = temp->next;
    }
    return path;
}

char * GetRPath(struct Path *file){
    char *path = malloc(1000000);
    Path *temp = file;
    while(temp){
        strcat(path, "/");
        strcat(path, temp->dir);
        temp = temp->prev;
    }
    return path;
}
typedef struct Files{
    Path path;
    Path *last;
    Path *first;
}Files;

Files files[1000000];
Path current;

void Insert(struct Path *path, struct Path *next){
    if(files[FILES_POS].last == NULL){
        files[FILES_POS].first = next;
        files[FILES_POS].last = next;
    }
    else{
        files[FILES_POS].last->next = next;
        next->prev = files[FILES_POS].last;
        files[FILES_POS].last = next;
    }
    /*Path *temp = path;
    while(temp->next){
        printf("%s\n", path->dir);
        temp = temp->next;
    }
    temp->next = next;
    next->prev = temp;
    next->next = NULL;*/
}

//utilize apenas para remover o último
void Remove(struct Path *path){
    while(path->next){
        path = path->next;
    }
    LAST_PATH = path->prev;
    LAST_PATH->next = NULL;
}




void find_files(char *dirname, Files *f){
    DIR *dir;
    struct dirent *dirp;
    dir=opendir(dirname);
    chdir(dirname);
    while((dirp=readdir(dir))!=NULL){
        if(dirp->d_type==4){
            if(strcmp(dirp->d_name, ".")==0 || strcmp(dirp->d_name, "..")==0){
                continue;
            }
            Path path;
            path.dir = malloc(sizeof(dirp->d_name));
            printf("%s %s\n", "FOLDER", dirp->d_name);
            strcpy(path.dir, dirp->d_name);
            Insert(&current, &path);
            printf("current: %s\n", GetPath(&current));
            find_files(dirp->d_name, f);
        }
        else{
            Path path;
            path.dir = malloc(sizeof(dirp->d_name));
            printf("%s %s\n", "FILE", dirp->d_name);
            strcpy(path.dir, dirp->d_name);
            /*files[i-1].path.dir = current.dir;
            files[i-1].path.next = current.next;
            files[i-1].path.prev = current.prev;*/
            //memcpy(&files[i-1].path, &current, sizeof(current));
            f[FILES_POS].path = current;
            f[FILES_POS].first = NULL;
            f[FILES_POS].last = NULL;
            memcpy(&f[FILES_POS].path, &current, sizeof(current));
            Insert(&f[FILES_POS].path, &path);
            printf("%s\n", GetPath(&f[FILES_POS].path));
            printf("---------------------\n");
            printf("%s\n", GetPath(&f[0].path));
            printf("%s\n", GetPath(&f[1].path));
            printf("---------------------\n");
            //printf("adress: %s\n", GetPath(&files[DIR_POS-1].path));
            FILES_POS++;
            
            //char zip[255] = "bzip2 ";
            //strcat(zip, dirp->d_name);
            //printf("%s\n", zip);
            //char path[PATH_MAX];
            //terminal = popen(zip, "r");
            //while(fgets(path, PATH_MAX, terminal) != NULL);
        }
        //strcpy(DIRECTORIES[DIR_POS], dirp->d_name);
        //DIR_POS++;
    }
    chdir("..");
    Remove(&current);
    closedir(dir);
}/*
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
            char zip[255] = "bzip2 -k";
            strcat(zip, dirp->d_name);
            char path[PATH_MAX];
            terminal = popen(zip, "r");
            while(fgets(path, PATH_MAX, terminal) != NULL);
            char file_name[256];
            strcpy(file_name, dirp->d_name);
            strcat(file_name, ".bz2");
            char append_file[256] = "tar --append ";
            strcat(append_file, file_name);
            strcat(append_file, " -f ");
            strcat(append_file, ZIP_NAME);
            terminal = popen(append_file, "r");
            while(fgets(path, PATH_MAX, terminal) != NULL);
            find_files(DIRECTORY_NAME);
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
}*/
struct stat st = {0};
int main( int argc, char *argv[ ]){
    strcpy(DIRECTORY_NAME, argv[1]); //Caminho atual
    strcpy(ZIP_NAME, argv[2]); //Nome do arquivo final
    int size = strlen(ZIP_NAME);  //Quantidade de caracteres do zip
    strcpy(TEMP_NAME, ZIP_NAME); // TEMP_NAME = ZIP_NAME
    TEMP_NAME[size-4]='\0'; //Termina a string no antes do .tar
    strcat(TEMP_NAME, ".bz2");
    mkdir(TEMP_NAME, 0700);
    char tar[255] = "tar cf ";
    strcat(tar, ZIP_NAME);
    strcat(tar, " ");
    strcat(tar, DIRECTORY_NAME);
    terminal = popen(tar, "r");
    char *path;
    while(fgets(path, PATH_MAX, terminal) != NULL);
    //Files files[1000000];
    current.prev = NULL;
    current.next = NULL;
    current.dir = malloc(sizeof(DIRECTORY_NAME));
    strcpy(current.dir, DIRECTORY_NAME);
    /*LAST_PATH = current;
    FIRST_PATH = current;*/
    files[0].path = current;
    FILES_POS++;
    find_files(DIRECTORY_NAME, files);
    
    /*
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