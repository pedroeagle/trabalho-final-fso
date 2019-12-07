#include <stdio.h>
#include <string.h>
#include <sys/dir.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <stdlib.h>

#define PATH_MAX 4096
char DIRECTORY_NAME[1000000];
char ZIP_NAME[1000000];
char TEMP_NAME[1000000];
FILE *terminal;

char *PATH[1000000];
long long int PATH_POS = 0;

char *FILES[1000000];
long long int FILES_POS = 0;

char *BZIP_FILES[1000000];
long long int BZIP_FILES_POS = 0;

char * get_original_path(const char *file_name){
    char *complete_path = malloc(1000000);
    for(long long int i = 0; i < PATH_POS; i++){
        strcat(complete_path, PATH[i]);
        strcat(complete_path, "/");
    }
    strcat(complete_path, file_name);
    //printf("%s\n", complete_path);
    return complete_path;
}

char * get_bzip_path(const char *file_name){
    char *complete_path = malloc(1000000);
    for(long long int i = 1; i < BZIP_FILES_POS; i++){
        strcat(complete_path, BZIP_FILES[i]);
        strcat(complete_path, "/");
    }
    strcat(complete_path, file_name);
    //printf("%s\n", complete_path);
    return complete_path;
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
            PATH[PATH_POS] = malloc(sizeof(dirp->d_name));
            strcpy(PATH[PATH_POS], dirp->d_name);
            PATH_POS++;
            find_files(dirp->d_name);
        }
        else{
            //printf("%s %s\n", "FILE", dirp->d_name);
            FILES[FILES_POS] = malloc(sizeof(get_original_path(dirp->d_name)));
            strcpy(FILES[FILES_POS], get_original_path(dirp->d_name));
            FILES_POS++;
        }
    }
    chdir("..");
    free(PATH[PATH_POS]);
    PATH_POS--;
    closedir(dir);
}

void remove_dirs(char *dirname){
    DIR *dir;
    struct dirent *dirp;
    dir=opendir(dirname);
    chdir(dirname);
    while((dirp=readdir(dir))!=NULL){
        printf("%s\n", dirp->d_name);
        if(dirp->d_type==4){
            if(strcmp(dirp->d_name, ".")==0 || strcmp(dirp->d_name, "..")==0){
                continue;
            }
            remove_dirs(dirp->d_name);
        }
        else{
            remove(dirp->d_name);
        }
        strcpy(BZIP_FILES[BZIP_FILES_POS], dirp->d_name);
        BZIP_FILES_POS++;
    }
    chdir("..");
    closedir(dir);
    rmdir(dirname);
}
int isdirectory(char *path) {
    struct stat statbuf;
    if (stat(path, &statbuf) == -1){
        return 0;
    }
    else{
        return S_ISDIR(statbuf.st_mode);
    }
}

int main( int argc, char *argv[ ]){
    strcpy(DIRECTORY_NAME, argv[1]); //Caminho atual
    strcpy(ZIP_NAME, argv[2]); //Nome do arquivo final
    int size = strlen(ZIP_NAME);  //Quantidade de caracteres do zip
    strcpy(TEMP_NAME, ZIP_NAME); // TEMP_NAME = ZIP_NAME
    TEMP_NAME[size-4]='\0'; //Termina a string no antes do .tar
    //strcat(TEMP_NAME, ".bz2");
    PATH[PATH_POS] = malloc(sizeof(DIRECTORY_NAME));
    strcpy(PATH[PATH_POS], DIRECTORY_NAME);
    PATH_POS++;
    find_files(DIRECTORY_NAME);

    DIR *dir, *org_dir;
    struct dirent *dirp, *org_dirp;
    mkdir(TEMP_NAME, 0700);
    //rename(TEMP_NAME, ZIP_NAME);
    char tar[255] = "tar cf ";
    strcat(tar, ZIP_NAME);
    strcat(tar, " ");
    strcat(tar, TEMP_NAME);
    //printf("%s\n", ZIP_NAME);
    //printf("%s\n", tar);
    terminal = popen(tar, "r");
    char *path;
    //while(fgets(path, PATH_MAX, terminal) != NULL);
    fgets(path, PATH_MAX, terminal);

    dir=opendir(TEMP_NAME);
    chdir(TEMP_NAME);
    char temp[1000000];
    for(long long int i = 0; i < FILES_POS; i++){
        //printf("%s\n", FILES[i]);
        strcpy(temp, FILES[i]);
        char *ptr = strtok(temp, "/");
        long long int qtd_dirs = 0;
        char file_name[1000000];
        while(ptr != NULL)
        {
            strcpy(file_name, ptr);
            ptr = strtok(NULL, "/");
            qtd_dirs++;
        }
        //long long int j = 0;
        //long long int inside = 0;
        //char *n_ptr = strtok(temp, "/");
        strcpy(temp, FILES[i]);
        char bzip_temp[1000000];
        
        strcpy(bzip_temp, TEMP_NAME);
        printf("%s\n", bzip_temp);
        if(!isdirectory(bzip_temp)){
            int d = mkdir(bzip_temp, 0700);
        }
        /*
        while(n_ptr != NULL)
        {
            if(j >= 1 && j < qtd_dirs-1){
                strcat(bzip_temp, "/");
                strcat(bzip_temp, n_ptr);
                
                
                dir = opendir(n_ptr);
                chdir(n_ptr);
                inside++;
            }
            if(j == qtd_dirs-1){
                strcpy(file_name, n_ptr);
            }
            
            n_ptr = strtok(NULL, "/");
            j++;
        }*/
        /*while(inside--){
            chdir("..");
        }*/

        BZIP_FILES[BZIP_FILES_POS] = malloc(1000000);
        strcat(BZIP_FILES[BZIP_FILES_POS], bzip_temp);
        strcat(BZIP_FILES[BZIP_FILES_POS], "/");
        strcat(BZIP_FILES[BZIP_FILES_POS], file_name);
        BZIP_FILES_POS++;
    }
    closedir(dir);
    for(long long int i = 0; i < FILES_POS; i++){
        char bzip2[1000000] = "bzip2 ";
        strcat(bzip2, FILES[i]);
    }
    chdir("..");
    char bzip[100000];
    char append[100000];
    for(long long int i = 0; i < FILES_POS; i++){
        strcpy(bzip, "bzip2 ");
        strcpy(append, "tar rf ");
        FILE *original, *to_bzip;
        original = fopen(FILES[i], "r");
        //printf("%s\n", FILES[i]);
        to_bzip = fopen(BZIP_FILES[i], "a+");
        //printf("%s\n", BZIP_FILES[i]);

        // char c;
        // while ((c = fgetc(original)) != EOF){
        //     fputc(c, to_bzip);
        // }
        int r;
        while (feof(original)==0){
            char buffer[100000];
            r=fread(buffer,1,100000,original);
            fwrite(buffer,1,r,to_bzip);
        }

        strcat(bzip, BZIP_FILES[i]);
        fclose(original);
        fclose(to_bzip);
        terminal = popen(bzip, "r");
        char *path;
        //while(fgets(path, PATH_MAX, terminal!=NULL);
        fgets(path, PATH_MAX, terminal);
        strcat(append, ZIP_NAME);
        strcat(append, " ");
        strcat(append, BZIP_FILES[i]);
        strcat(append, ".bz2");
        //printf("%s\n", append);
        terminal = popen(append, "r");
        //while(fgets(path, PATH_MAX, terminal) != NULL);
        fgets(path, PATH_MAX, terminal);
        char aux[255];
        strcpy(aux, BZIP_FILES[i]);
        strcat(aux, ".bz2");
        remove(aux);
    }
    printf("%s\n", TEMP_NAME);

    remove(TEMP_NAME);
    
    return 0;
}