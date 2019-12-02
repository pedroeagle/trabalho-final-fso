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

int main( int argc, char *argv[ ]){
    strcpy(DIRECTORY_NAME, argv[1]);
    char cp[255] = "cp -ax ";
    char destino_dir_name[255];
    strcat(cp, DIRECTORY_NAME);
    strcat(destino_dir_name, DIRECTORY_NAME);
    strcat(destino_dir_name, ".bz2");
    strcat(cp, " ");
    strcat(cp, destino_dir_name);
    terminal = popen(cp, "r");
    char path[PATH_MAX];
    while(fgets(path, PATH_MAX, terminal) != NULL);
    find_files(destino_dir_name);
    char tar[255] = "tar cf ";
    char destino_tar_name[255];
    strcpy(destino_tar_name, DIRECTORY_NAME);
    strcat(destino_tar_name, ".bz2.tar");
    strcat(tar, destino_tar_name);
    strcat(tar, " ");
    strcat(tar, destino_dir_name);
    //printf("%s\n", tar);
    terminal = popen(tar, "r");
    while(fgets(path, PATH_MAX, terminal) != NULL);
    char rm[255] = "rm -r ";
    strcat(rm, destino_dir_name);
    terminal = popen(rm, "r");
    while(fgets(path, PATH_MAX, terminal) != NULL);
    return 0;
}