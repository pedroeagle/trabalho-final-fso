#include <stdio.h>
#include <string.h>
#include <stdlib.h>
typedef struct Path{
    struct Path *prev;
    struct Path *next;
    char *dir;
}Path;

Path *FIRST_PATH;
Path *LAST_PATH;
void Insert(struct Path *path, struct Path *next){
    path->next = next;
    next->prev = path;
}
//utilize apenas para remover o último
void Remove(struct Path *path){
    LAST_PATH = path->prev;
    LAST_PATH->next = NULL;
    free(path);
}

char * GetPath(struct Path *first){
    char *path = malloc(1000000);
    Path *temp = first;
    while(temp){
        strcat(path, temp->dir);
        strcat(path, "/");
        temp = temp->next;
    }
    return path;
}
int main(){
    Path* a = (Path*)malloc(sizeof(Path));
    FIRST_PATH = a;
    a->dir = malloc(sizeof("aaaaa"));
    strcpy(a->dir, "aaaaa");

    Path* b = (Path*)malloc(sizeof(Path));
    b->dir = malloc(sizeof("bbbbb"));
    strcpy(b->dir, "bbbbb");
    Insert(a, b);

    Path* c = (Path*)malloc(sizeof(Path));
    c->dir = malloc(sizeof("ccccc"));
    strcpy(c->dir, "ccccc");
    Insert(b, c);
    Remove(c);
    printf("%s\n", GetPath(FIRST_PATH));
    return 0;
}