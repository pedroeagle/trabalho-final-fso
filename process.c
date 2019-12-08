#define _XOPEN_SOURCE 500
#include <ftw.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <dirent.h>
#include <unistd.h>
#include <signal.h>
#include <sys/wait.h>
#include <math.h>


char DIRECTORY_NAME[1000000];
char ZIP_NAME[1000000];
char TEMP_NAME[1000000];
char *TO_CONSUME[1000000];
int DONE[1000000];
long long int POS_CONSUME = 0;
int T_FLAG[1000000];
int POS_TFLAG = 0;
char *FPATH[1000000];
int POS_FPATH = 0;
pid_t pid = 0, p1 = 0, p2 = 0, p3 = 0;
struct stat *SB;


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

void consume(char * new_path, char *fpath, int tflag){
    printf("%s\n", new_path);
    if(tflag == FTW_D){
        mkdir(new_path, 0700);
    }
    else if(tflag != FTW_D){
        FILE *original, *bziped;
        original = fopen(fpath, "rb");
        bziped = fopen(new_path, "wb");
        while(!original || !bziped){
            usleep(100000);
            original = fopen(fpath, "rb");
            bziped = fopen(new_path, "wb");
        }
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
}

static int display_info(const char *fpath, const struct stat *sb, int tflag, struct FTW *ftwbuf){
    int size = strlen(DIRECTORY_NAME);
    char *to_create;
    char *new_path;
    to_create = malloc(1000);
    new_path = malloc(1000);
    strcpy(to_create,size+1+fpath);
    strcpy(new_path, TEMP_NAME);
    strcat(new_path, "/");
    strcat(new_path, to_create);
    TO_CONSUME[POS_CONSUME] = malloc(1000000);
    strcpy(TO_CONSUME[POS_CONSUME], new_path);
    //printf("%s\n", TO_CONSUME[POS_CONSUME]);
    DONE[POS_CONSUME] = 0;
    POS_CONSUME++;
    T_FLAG[POS_TFLAG] = tflag;
    POS_TFLAG++;
    FPATH[POS_FPATH] = malloc(1000000);
    strcpy(FPATH[POS_FPATH], fpath);
    POS_FPATH++;
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

    pid = getpid();
    nftw((argc < 3) ? "." : argv[1], display_info, 20, 0);

    int qtd_process = ceil(POS_CONSUME/4);


    if(!(p1=fork())){
        for(int i = 0; i < POS_CONSUME; i+=4){
            consume(TO_CONSUME[i], FPATH[i], T_FLAG[i]);
        }
        //kill(pid, SIGUSR1);
        exit(0);
    }else{
        if(!(p2=fork())){
            //signal(SIGCONT, print_p2);
            for(int i = 1; i < POS_CONSUME; i+=4){
                consume(TO_CONSUME[i], FPATH[i], T_FLAG[i]);
            }
            //kill(pid, SIGUSR2);
            exit(0);
        }
        else{
            if(!(p3=fork())){
                //signal(SIGCONT, print_p2);
                for(int i = 2; i < POS_CONSUME; i+=4){
                    consume(TO_CONSUME[i], FPATH[i], T_FLAG[i]);
                }
                //kill(pid, SIGUSR2);
                exit(0);
            }
            else{
                //signal(SIGCONT, print_p2);
                for(int i = 3; i < POS_CONSUME; i+=4){
                    consume(TO_CONSUME[i], FPATH[i], T_FLAG[i]);
                }
                int status = 0;
                wait(NULL);
                wait(NULL);
                wait(NULL);
                char tar[255] = "tar cf ";
                strcat(tar, ZIP_NAME);
                strcat(tar, " ");
                strcat(tar, TEMP_NAME);
                FILE *terminal;
                terminal = popen(tar, "r");
                pclose(terminal);
                remove_dirs(TEMP_NAME);
                exit(0);
            }

        }
    }
    return 0;
}