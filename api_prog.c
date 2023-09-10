#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include <string.h>
#include <stdbool.h>
#include <unistd.h>
#define MAX_STATION 27000
#define MAX_CAR_NUM 513
#define INVALID -1
#define IGNORE -2

typedef struct {
    int distance;
    int max_range;
    int* range;
}Station;

typedef struct {
    int distance;
    int max_range;
    short furthest_next;
}Station_list;

Station_list* path;
Station* station;

char command[18];
char* buffer;
int station_size = 1;
int distance;
int num_car;
int lenght;
int range;
int start;
int end;

/**************************************************************** ALGORITHMS **********************************************************************/

void merge(Station_list arr[], int l, int m, int r){
    int i, j, k;
    int n1 = m - l + 1;
    int n2 = r - m;
  
    Station_list L[n1], R[n2];
  
    for (i = 0; i < n1; i++){
        L[i].distance = arr[l + i].distance;
        L[i].max_range = arr[l + i].max_range;
    }
        
    for (j = 0; j < n2; j++){
        R[j].distance = arr[m + 1 + j].distance;
        R[j].max_range = arr[m + 1 + j].max_range;
    }

    i = 0;
    j = 0;
    k = l;

    while (i < n1 && j < n2) {
        if (L[i].distance <= R[j].distance) {
            arr[k].distance = L[i].distance;
            arr[k].max_range = L[i].max_range;
            i++;
        }
        else {
            arr[k].distance = R[j].distance;
            arr[k].max_range = R[j].max_range;
            j++;
        }
        k++;
    }
    while (i < n1) {
        arr[k].distance = L[i].distance;
        arr[k].max_range = L[i].max_range;
        i++;
        k++;
    }
    while (j < n2) {
        arr[k].distance = R[j].distance;
        arr[k].max_range = R[j].max_range;
        j++;
        k++;
    }
}
  
void mergeSort(Station_list arr[], int l, int r){
    if (l < r) {
        int m = l + (r - l) / 2;
        mergeSort(arr, l, m);
        mergeSort(arr, m + 1, r);
        merge(arr, l, m, r);
    }
}

void bfs_search(){
    // Contiene gli indici delle stazioni in path da controllare e controllate.
    int* Queue = malloc((lenght+1)*sizeof(int));
    int current_queue = 0;
    int last_queue = 0;

    // Ci dice se la stazione corrispondente in path è già stata visitata o no.
    // Esempio: path[n]     =>      Queue[current_queue] = n     =>      visited[n] = true;
    bool* visited = malloc((lenght+1)*sizeof(bool));

    // Salva la stazione "parente" di ogni stazione in Queue.
    // Esempio: Queue[current_queue] = n     =>      prev[current_queue] = prev_n.
    int* prev = malloc((lenght+1)*sizeof(int));
    
    int* final_path = malloc(lenght+1*sizeof(int));

    for(int i = 0; i <= lenght; i++){
        visited[i] = false;
    }

    Queue[0] = 0;
    prev[0] = 0;

    while(Queue[last_queue] != lenght){
        
        if(path[Queue[current_queue]].furthest_next != Queue[current_queue]){

            int temp = path[Queue[current_queue]].furthest_next;
            for(int i = Queue[current_queue]+1; i <= temp; i++){

                if(!visited[i]){
                    
                    last_queue++;
                    Queue[last_queue] = i;
                    visited[i] = true;
                    prev[last_queue] = Queue[current_queue];
                    if(Queue[last_queue] == lenght){
                        break;
                    }

                }
                
            }
        }

        if(current_queue == last_queue){
            printf("nessun percorso\n");
            return;
        }

        visited[Queue[current_queue]] = true;
        current_queue++;
        
    }

    int j = 0;
    final_path[0] = lenght;
    for(int i = last_queue; i > 0; i--){
        if(Queue[i] == final_path[j]){
            j++;
            final_path[j] = prev[i];
        }
         
    }
    
    for(int i = j; i > 0 ; i--){
        printf("%d ", path[final_path[i]].distance);
    }           
    printf("%d\n", end);
}

void bfs_search_inverted(){
   
    // Contiene gli indici delle stazioni in path da controllare e controllate.
    int* Queue = malloc((lenght+1)*sizeof(int));
    int current_queue = 0;
    int last_queue = 0;
    
    // Contine true se la stazione corrispondente in path è già stata visitata, false altrimenti.
    // Esempio: path[n]     =>      Queue[current_queue] = n     =>      visited[n] = true;
    bool* visited = malloc((lenght+1)*sizeof(bool));
    
    // Contiene gli indici di Queue che marcano la fine di un interavallo di hop. (Gli hop nell'algortimo BFS)
    int* marker = malloc((lenght+1)*sizeof(int));
    int current_marker = 0;
    marker[current_marker] = 0;

    // Contiene gli indici delle stazione del percorso finale.
    int* final_path = malloc((lenght+1)*sizeof(int));
    int current_final = 0;

    for(int i = 0; i <= lenght; i++){
        visited[i] = false;
    }

    Queue[0] = lenght;

    int temp = 0;

    while(Queue[last_queue] != 0){

        if(path[Queue[current_queue]].furthest_next != Queue[current_queue]){

            temp = path[Queue[current_queue]].furthest_next;

            for(int i = Queue[current_queue]-1; i >= temp; i--){

                if(!visited[i]){
                    
                    last_queue++;
                    Queue[last_queue] = i;
                    visited[Queue[last_queue]] = true;
                    if(!Queue[last_queue]){
                        break;
                    }

                }
            
            }
        
        }

        if(current_queue == last_queue){
            printf("nessun percorso\n");
            return;
        }

        if(current_queue == marker[current_marker] || Queue[last_queue] == 0){
            current_marker++;
            marker[current_marker] = last_queue;
        }

        visited[Queue[current_queue]] = true;
        current_queue++;
    }

    for(int i = 0; i < current_marker; i++){
        temp = Queue[marker[i+1]];
        for(int j = marker[i]+1; j <= marker[i+1]; j++){
            if(path[Queue[j]].furthest_next > temp){
                Queue[j] = IGNORE;
            }
        }
    }
    
    final_path[current_final] = path[0].distance;
    for(int i = current_marker; i > 1; i--){

        for(int j = marker[i-1]; j > marker[i-2]; j--){

            if(Queue[j] != IGNORE && path[Queue[j]].furthest_next <= Queue[last_queue]){
                current_final++;
                final_path[current_final] = path[Queue[j]].distance;
                last_queue = j;
                break;
            }

        }

    }
    current_final++;
    final_path[current_final] = path[lenght].distance;

    for(int i = current_final; i >= 0; i--){
        printf("%d ", final_path[i]);
    }
    printf("\n");
}

/**************************************************************** FUNCTIONS **********************************************************************/

int parse_num(){

    char temp[10];
    for(int i = 0; i < 11; i++){
        temp[i] = getchar_unlocked();
        switch (temp[i])
        {
        case ' ':
            return atoi(temp);
        case '\n':
            return atoi(temp);
        default:
            break;
        }
    }
    return 0;
}

void aggiungi_stazione(){ 
    
    getchar_unlocked();
    distance = parse_num();
    num_car = parse_num();

    for(int i = 0; i < station_size; i++){
        if(station[i].distance == distance){
            printf("non aggiunta\n");
            return;
        }
    }

    //station = realloc(station, station_size*sizeof(Station));
    station[station_size].distance = distance;
    station[station_size].max_range = 0;
    station[station_size].range = calloc(num_car+1, sizeof(int));
    //station[station_size].range = calloc(MAX_CAR_NUM, sizeof(int));
    
    int temp = 0;
    for(int j = 0; j < num_car && scanf("%d", &station[station_size].range[j]) > 0; j++){
        if(temp < station[station_size].range[j]){
            station[station_size].max_range = station[station_size].range[j];
            temp = station[station_size].range[j];
        }
    }
    station[station_size].range[num_car] = INVALID;
    station_size++;
    printf("aggiunta\n");     
}

void demolisci_stazione(){

    if(scanf("%d", &distance) > 0);

    for(int i=0; i < station_size; i++){
        if(station[i].distance == distance){
            station[i].distance = INVALID;
            printf("demolita\n");
            return;
        }
    }
    printf("non demolita\n");
}

void aggiungi_auto(){

    if(scanf("%d", &distance) > 0 && scanf("%d", &range) > 0);

    for(int i = 0; i < station_size; i++){
        if(station[i].distance == distance){
            for (int j = 0; j < MAX_CAR_NUM; j++){
                if(station[i].range[j] == IGNORE){
                    station[i].range[j] = range;
                    if(station[i].max_range < range){
                        station[i].max_range = range;
                    }
                    printf("aggiunta\n"); 
                    return;
                }
                if(station[i].range[j] == INVALID){
                    station[i].range = realloc(station[i].range, (j+2)*sizeof(int));
                    station[i].range[j] = range;
                    station[i].range[j+1] = INVALID;
                    if(station[i].max_range < range){
                        station[i].max_range = range;
                    }
                    printf("aggiunta\n");
                    return;
                }
            }
            printf("aggiunta\n");
            return;
        }
    }
    printf("non aggiunta\n");
}

void rottama_auto(){

    if(scanf("%d", &distance) > 0 && scanf("%d", &range) > 0);

    for(int i = 0; i < station_size; i++){
        if(station[i].distance == distance){

            if(station[i].max_range < range){
                printf("non rottamata\n");
                return;
            }

            for(int j = 0; j < MAX_CAR_NUM; j++){
                if(station[i].range[j] == INVALID){
                    printf("non rottamata\n");
                    return;
                }
                if(station[i].range[j] == range){
                    station[i].range[j] = IGNORE;
                    int temp = 0;
                    if(station[i].max_range == range){
                        for(int k = 0; k < MAX_CAR_NUM; k++){
                            if(station[i].range[k] == INVALID){
                                station[i].max_range = temp;
                                printf("rottamata\n");
                                return;
                            }
                            if(temp < station[i].range[k]){
                                temp = station[i].range[k];
                            }
                        }
                    }
                    printf("rottamata\n");
                    return;
                }
            }

        }
    }
    printf("non rottamata\n");
}

void pianifica_percorso(){

    if(scanf("%d", &start) > 0 && scanf("%d", &end) > 0);

    if(start == end){
        printf("start\n");
        return;
    }
 
    path = realloc(path ,station_size*sizeof(Station_list));

    lenght = 0;

    if(start < end){

        // Inserisco tutte le stationi con distanze tra "start" ed "end" nell'array "path" che avrà lunghezza "lenght".
        for(int i = 0; i < station_size; i++){
            if(station[i].distance <= end && station[i].distance >= start && station[i].distance != INVALID){
                path[lenght].distance = station[i].distance;
                path[lenght].max_range = station[i].max_range;
                lenght++;
            }
        }

        lenght--;
        
        // Faccio un mergeSort sull'array path per ordinare le stazioni dalla meno distante alla più distante.
        mergeSort(path, 0, lenght);
        
        // Trovo la prossima stazione più lontana raggiungibile da ciascuna stazione
        for(int i = 0; i <= lenght; i++){
            path[i].furthest_next = i;
            for(int j = i+1; j <= lenght; j++){
                if((path[j].distance - path[i].distance) <= path[i].max_range){
                    path[i].furthest_next = j;
                }
                if((path[j].distance - path[i].distance) > path[i].max_range){
                    break;
                }
            }
        }
        path[lenght].furthest_next = INVALID;

        bfs_search();
        return;

    }else{

        int temp = start;
        start = end;
        end = temp;

        // Inserisco tutte le stationi con distanze tra "start" ed "end" nell'array "path" che avrà lunghezza "lenght".
        for(int i = 0; i < station_size; i++){

            if(station[i].distance <= end && station[i].distance >= start && station[i].distance != INVALID){
                path[lenght].distance = station[i].distance;
                path[lenght].max_range = station[i].max_range;
                lenght++;
            }

        }

        lenght--;
        
        // Faccio un mergeSort sull'array path per ordinare le stazioni dalla meno distante alla più distante.
        mergeSort(path, 0, lenght);

        // Trovo la prossima stazione più lontana raggiungibile da ciascuna stazione
        for(int i = lenght; i >= 0; i--){
            path[i].furthest_next = i;
            for(int j = i-1; j >= 0; j--){

                if((path[i].distance - path[j].distance) <= path[i].max_range){
                    path[i].furthest_next = j;
                }
                if((path[i].distance - path[j].distance) > path[i].max_range){
                    break;
                }

            }
        }
        path[0].furthest_next = 0;

        bfs_search_inverted();
        return;
    }
}

/****************************************************************** MAIN **************************************************************************/

int main(int argc, char *argv[]){
    
    station = malloc(MAX_STATION*sizeof(Station));
    station[0].range = malloc(sizeof(int));
    station[0].distance = INVALID;
    station[0].range[0] = INVALID;

    path = malloc(2*sizeof(Station_list));

    command[0] = '0';
    while(command[0] != EOF){

        command[0] = getchar_unlocked();
        switch (command[0])
        {
        case 'a':

            if(fgets(command, 10, stdin) != NULL);
            switch (command[8])
            {
            case 's':
                if(fgets(command, 8, stdin) != NULL);
                aggiungi_stazione();
                break;
            
            default:
                if(fgets(command, 4, stdin) != NULL);
                aggiungi_auto();
                break;
            }
            break;
            
        case 'd':

            if(fgets(command, 18, stdin) != NULL);
            demolisci_stazione();
            break;

        case 'r':

            if(fgets(command, 12, stdin) != NULL);
            rottama_auto();
            break;

        case 'p':
            
            if(fgets(command, 18, stdin) != NULL);
            pianifica_percorso();
            break;        

        default:
            break;
        }

    }

    return 0;
}
