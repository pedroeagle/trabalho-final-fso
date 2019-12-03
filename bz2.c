#include <stdio.h>
#include <string.h>
#include <sys/dir.h>
#include <unistd.h>

#define PATH_MAX 4096
char DIRECTORIES[1000000][200];
long long int DIR_POS = 0;
char DIRECTORY_NAME[200];
FILE *terminal;

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
    strcpy(DIRECTORY_NAME, argv[1]);
    char destino_tar_name[255];
    char destino_temp_name[255];
    strcpy(destino_tar_name, DIRECTORY_NAME);
    strcpy(destino_temp_name, DIRECTORY_NAME);
    strcat(destino_tar_name, ".bz2.tar");
    strcat(destino_temp_name, ".bz2");
    rename(DIRECTORY_NAME, destino_temp_name);
    find_files(destino_temp_name);
    char tar[255] = "tar cf ";
    strcat(tar, destino_tar_name);
    strcat(tar, " ");
    strcat(tar, destino_temp_name);
    printf("%s\n", tar);
    terminal = popen(tar, "r");
    char *path;
    while(fgets(path, PATH_MAX, terminal) != NULL);
    remove_dirs(destino_temp_name);
    return 0;
}