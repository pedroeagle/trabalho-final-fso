#define _XOPEN_SOURCE 500
#include <ftw.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <dirent.h>
#include <unistd.h>

char DIRECTORY_NAME[1000000];
char ZIP_NAME[1000000];
char TEMP_NAME[1000000];

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
    }
    chdir("..");
    closedir(dir);
    rmdir(dirname);
}

static int display_info(const char *fpath, const struct stat *sb, int tflag, struct FTW *ftwbuf){
    int size = strlen(DIRECTORY_NAME);
    char *to_create;
    char *new_path;
    to_create = malloc(1000000);
    new_path = malloc(1000000);
    strcpy(to_create,size+1+fpath);
    strcpy(new_path, TEMP_NAME);
    strcat(new_path, "/");
    strcat(new_path, to_create);
    if(tflag == FTW_D && ftwbuf->base != 0){
        mkdir(new_path, 0700);
    }
    else if(tflag != FTW_D){
        FILE *original, *bziped;
        original = fopen(fpath, "rb");
        bziped = fopen(new_path, "wb");
        
        int r;
        while (feof(original)==0){
            char buffer[100000];
            r=fread(buffer,1,100000,original);
            fwrite(buffer,1,r,bziped);
        }
        fclose(original);
        fclose(bziped);
        char bzip2[1000000];
        strcpy(bzip2, "bzip2 ");
        strcat(bzip2, new_path);
        FILE *terminal;
        terminal = popen(bzip2, "r");
        strcat(new_path, ".bz2");
        pclose(terminal);
    }

        
    return 0;           
}

int remove_func(const char *fpath, const struct stat *stat, int tflags, struct FTW *ftwbuf)
{
    int rmv = remove(fpath);
    return rmv;
}

void remove_files(char path[])
{
    int fd_limit = 5;
    int flags = FTW_DEPTH;
    int rm;

    rm = nftw(path, remove_func, fd_limit, flags);
}

int main(int argc, char *argv[]){
    strcpy(DIRECTORY_NAME, argv[1]);
    strcpy(ZIP_NAME, argv[2]);
    int size = strlen(ZIP_NAME);
    strcpy(TEMP_NAME, ZIP_NAME); 
    TEMP_NAME[size-4]='\0';
    mkdir(TEMP_NAME, 0700);

    nftw((argc < 3) ? "." : argv[1], display_info, 20, 0);

    char tar[255] = "tar cf ";
    strcat(tar, ZIP_NAME);
    strcat(tar, " ");
    strcat(tar, TEMP_NAME);
    FILE *terminal;
    terminal = popen(tar, "r");
    pclose(terminal);
    remove_dirs(TEMP_NAME);
    remove_files(TEMP_NAME);

    return 0;
}