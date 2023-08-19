#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include <string.h>
#include <stdbool.h> 
#define MAX_CAR_NUM 513
#define INVALID -1

typedef struct {
    int distance;
    int* range;
}Station;

typedef struct {
    int distance;
    int max_range;
    int furthest_next;
}Station_list;

FILE *file;
char carattereInFile[2];
char distance_string[10];
char num_car_string[10];
char range_string[10];
char fileInput[2];
char buffer[20];

Station_list* path;
Station* station;

int station_size = 1;
int distance;
int num_car;
int range;
int start;
int end;
int lenght;

/***************************************************************** PRINTERS ***********************************************************************/

void print_path(){
    for(int i = 0; i < lenght; i++){
        printf("%d : ", i);
        printf("DISTANCE = %d || MAX_RANGE = %d || FURTHEST_NEXT = %d\n", path[i].distance, path[i].max_range, path[i].furthest_next); 
        if(path[i].distance == -1){
            break;
        }
    }
    printf("\n\n");
}

void print_station_distance(){
    for(int i = 0; i < station_size; i++){
        printf("%d -> ", station[i].distance);
    }
}

void print_range(){
    for(int i = 0; i < station_size-1; i++){
        printf("%d : ", station[i].distance);
        for(int j = 0; j < MAX_CAR_NUM; j++){
            printf("%d ", station[i].range[j]);
            if(station[i].range[j] == INVALID){
                break;
            }
        }
        printf("\n");
    }
}

/**************************************************************** ALGORITHMS **********************************************************************/

void merge(Station_list arr[], int l, int m, int r){
    int i, j, k;
    int n1 = m - l + 1;
    int n2 = r - m;
  
    Station_list L[n1], R[n2];
  
    for (i = 0; i < n1; i++){
        L[i].distance = arr[l + i].distance;
    }
        
    for (j = 0; j < n2; j++){
        R[j].distance = arr[m + 1 + j].distance;
    }

    i = 0;
    j = 0;
    k = l;

    while (i < n1 && j < n2) {
        if (L[i].distance <= R[j].distance) {
            arr[k].distance = L[i].distance;
            i++;
        }
        else {
            arr[k].distance = R[j].distance;
            j++;
        }
        k++;
    }
    while (i < n1) {
        arr[k].distance = L[i].distance;
        i++;
        k++;
    }
    while (j < n2) {
        arr[k].distance = R[j].distance;
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
    int* Queue = malloc(lenght*sizeof(int));
    
    // Ci dice se la stazione corrispondente in path è già stata visitata o no.
    // Esempio: path[n]     =>      Queue[current_queue] = n     =>      visited[n] = true;
    bool* visited = malloc(lenght*sizeof(bool));

    // Salva la stazione "parente" di ogni stazione in Queue.
    // Esempio: Queue[current_queue] = n     =>      prev[current_queue] = prev_n.
    int* prev = malloc(lenght*sizeof(int));
    
    int* final_path = malloc(lenght*sizeof(int));

    int current_queue = 0;
    int last_queue = 0;
    bool skip = false;

    for(int i = 0; i < lenght; i++){
        visited[i] = false;
    }

    Queue[0] = 0;
    prev[0] = 0;

    while(Queue[last_queue] != lenght-1){
        for(int i = Queue[current_queue]+1; i <= path[Queue[current_queue]].furthest_next ; i++){

            if(!visited[i]){
                
                last_queue++;
                Queue[last_queue] = i;
                visited[i] = true;
                prev[last_queue] = Queue[current_queue];
                if(Queue[last_queue] == lenght-1){
                    skip = true;
                    break;
                }

            }
            
        }

        visited[Queue[current_queue]] = true;
        current_queue++;
        if(skip){
            break;
        }
    }

    /**
    for(int i = 0; i <= current_queue; i++){
        printf("%d ->", Queue[i]);
    }
    printf("\n");
    for(int i = 0; i <= current_queue; i++){
        printf("%d ->", prev[i]);
    }
    printf("\n");
    */

    int j = 0;
    final_path[0] = lenght-1;
    for(int i = last_queue; i > 0; i--){
        if(Queue[i] == final_path[j]){
            j++;
            final_path[j] = prev[i];
        }
         
    }
    
    /**
    for(int i = j; i > 0 ; i--){
        printf("%d ->", final_path[i]);
    }
    */
    
    for(int i = j; i > 0 ; i--){
        printf("%d ->", path[final_path[i]].distance);
    }           
    printf("%d\n", end);
    
}

void bfs_search_inverted(){

    // Contiene gli indici delle stazioni in path da controllare e controllate.
    int* Queue = malloc(lenght*sizeof(int));
    
    // Ci dice se la stazione corrispondente in path è già stata visitata o no.
    // Esempio: path[n]     =>      Queue[current_queue] = n     =>      visited[n] = true;
    bool* visited = malloc(lenght*sizeof(bool));

    // Salva la stazione "parente" di ogni stazione in Queue.
    // Esempio: Queue[current_queue] = n     =>      prev[current_queue] = prev_n.
    int* prev = malloc(lenght*sizeof(int));
    
    int* final_path = malloc(lenght*sizeof(int));

    int current_queue = 0;
    int last_queue = 0;

    for(int i = 0; i < lenght; i++){
        visited[i] = false;
    }

    Queue[0] = lenght-1;
    prev[0] = 0;

    while(Queue[last_queue] != 0){
        for(int i = 0; path[Queue[current_queue]].furthest_next + i < Queue[current_queue]; i++){

            if(!visited[path[Queue[current_queue]].furthest_next + i]){
                
                last_queue++;
                Queue[last_queue] = path[Queue[current_queue]].furthest_next + i;
                visited[Queue[last_queue]] = true;
                prev[last_queue] = Queue[current_queue];
                if(Queue[last_queue] == 0){
                    break;
                }

            }
            
        }

        visited[Queue[current_queue]] = true;
        current_queue++;
    }

    /**
    for(int i = 0; i <= current_queue; i++){
        printf("%d ->", Queue[i]);
    }
    printf("\n");
    for(int i = 0; i <= current_queue; i++){
        printf("%d ->", prev[i]);
    }
    printf("\n");


    */

    int j = 0;
    final_path[0] = prev[1];
    for(int i = 1; i <= current_queue; i++){
        if(final_path[j] != prev[i]){
            j++;
            final_path[j] = prev[i];
            final_path[j+1] = INVALID;
        }   
    }
    
    /**
    for(int i = 0; final_path[i] > 0 ; i++){
        printf("%d ->", final_path[i]);
    }
    */
    
    for(int i = 0; final_path[i] > 0 ; i++){
        printf("%d ->", path[final_path[i]].distance);
    }    
    printf("%d \n", start);
}

/**************************************************************** FUNCTIONS **********************************************************************/

void aggiungi_stazione(){

	memset(distance_string, 0, sizeof(char[10]));
    while(fgets(carattereInFile, 2, file) != NULL){
        if(strcmp(carattereInFile, " ") == 0 || strcmp(carattereInFile, "\n") == 0){
            break;
        }else{
            strcat(distance_string, carattereInFile);
        }
    }
    distance = atoi(distance_string);
	
	memset(num_car_string, 0, sizeof(char[10]));
	while(fgets(carattereInFile, 2, file) != NULL){
        if(strcmp(carattereInFile, " ") == 0 || strcmp(carattereInFile, "\n") == 0){
            break;
        }else{
            strcat(num_car_string, carattereInFile);
        }
    }
    num_car = atoi(num_car_string);

    for(int i = 0; i <  station_size; i++){
        if(station[i].distance == distance){
            printf("non aggiunta\n");
            return;
        }
    }

    station_size++;
    station = realloc(station, station_size*sizeof(Station));
    station[station_size -1].distance = distance;
    //station[station_size -1].range = calloc(num_car+1, sizeof(int));
    station[station_size -1].range = calloc(MAX_CAR_NUM, sizeof(int));

    for(int j = 0; j < num_car; j++){
        memset(range_string, 0, sizeof(char[10]));
        while(fgets(carattereInFile, 2, file) != NULL){
            if(strcmp(carattereInFile, " ") == 0 || strcmp(carattereInFile, "\n") == 0){
                break;
            }else{
                strcat(range_string, carattereInFile);
            }
        }
        range = atoi(range_string);
        station[station_size -1].range[j] = range;
    }
    station[station_size -1].range[num_car] = -1;
    printf("aggiunta\n");        

}

void demolisci_stazione(){
    
    memset(distance_string, 0, sizeof(char[10]));
    while(fgets(carattereInFile, 2, file) != NULL){
        if(strcmp(carattereInFile, " ") == 0 || strcmp(carattereInFile, "\n") == 0){
            break;
        }else{
            strcat(distance_string, carattereInFile);
        }
    }
    distance = atoi(distance_string);

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
    memset(distance_string, 0, sizeof(char[10]));
    while(fgets(carattereInFile, 2, file) != NULL){
        if(strcmp(carattereInFile, " ") == 0 || strcmp(carattereInFile, "\n") == 0){
            break;
        }else{
            strcat(distance_string, carattereInFile);
        }
    }
    distance = atoi(distance_string);
	
	memset(range_string, 0, sizeof(char[10]));
	while(fgets(carattereInFile, 2, file) != NULL){
        if(strcmp(carattereInFile, " ") == 0 || strcmp(carattereInFile, "\n") == 0){
            break;
        }else{
            strcat(range_string, carattereInFile);
        }
    }
    range = atoi(range_string);

    for(int i = 0; i < station_size; i++){
        if(station[i].distance == distance){
            for (int j = 0; j < MAX_CAR_NUM; j++){
                if(station[i].range[j] == INVALID){
                    //station[i].range = realloc(station[i].range, (j+2)*sizeof(int));
                    station[i].range[j] = range;
                    station[i].range[j+1] = INVALID;
                    break;
                }
            }
            printf("aggiunta\n");
            return;
        }
    }
    printf("non aggiunta\n");
}

void rottama_auto(){
    memset(distance_string, 0, sizeof(char[10]));
    while(fgets(carattereInFile, 2, file) != NULL){
        if(strcmp(carattereInFile, " ") == 0 || strcmp(carattereInFile, "\n") == 0){
            break;
        }else{
            strcat(distance_string, carattereInFile);
        }
    }
    distance = atoi(distance_string);
	
	memset(range_string, 0, sizeof(char[10]));
	while(fgets(carattereInFile, 2, file) != NULL){
        if(strcmp(carattereInFile, " ") == 0 || strcmp(carattereInFile, "\n") == 0){
            break;
        }else{
            strcat(range_string, carattereInFile);
        }
    }
    range = atoi(range_string);

    for(int i = 0; i < station_size; i++){
        if(station[i].distance ==  distance){
            for(int j = 0; j < MAX_CAR_NUM; j++){
                if(station[i].range[j] == range){
                    for(int k = j; k < MAX_CAR_NUM; k++){
                        station[i].range[k] = station[i].range[k+1];
                        if(station[i].range[k] == INVALID){
                            break;
                        }
                    }
                    printf("rottamata\n");
                    return;
                }
            }
            break;
        }
    }
    printf("non rottamata\n");
}

void pianifica_percorso(){
    memset(distance_string, 0, sizeof(char[10]));
    while(fgets(carattereInFile, 2, file) != NULL){
        if(strcmp(carattereInFile, " ") == 0 || strcmp(carattereInFile, "\n") == 0){
            break;
        }else{
            strcat(distance_string, carattereInFile);
        }
    }
    start = atoi(distance_string);

    memset(distance_string, 0, sizeof(char[10]));
    while(fgets(carattereInFile, 2, file) != NULL){
        if(strcmp(carattereInFile, " ") == 0 || strcmp(carattereInFile, "\n") == 0){
            break;
        }else{
            strcat(distance_string, carattereInFile);
        }
    }
    end = atoi(distance_string);
    
    if(lenght == 0){
        path = malloc(station_size*sizeof(Station_list));
    }else{
        path = realloc(path ,station_size*sizeof(Station_list));
    }
    
    bool inverted = false;
    lenght = 0;

    if(start < end){

        // Inserisco tutte le stationi con distanze tra "start" ed "end" nell'array "path" che avrà lunghezza "lenght".
        for(int i = 0; i < station_size; i++){

            if(station[i].distance <= end && station[i].distance >= start && station[i].distance != INVALID){
                
                path[lenght].distance = station[i].distance;
                lenght++;
                //path = realloc(path, (lenght + 1)*sizeof(Station_list));

            }

        }
        
        // Faccio un mergeSort sull'array path per ordinare le stazioni dalla meno distante alla più distante.
        mergeSort(path, 0, lenght-1);

        // Inserisco il max range delle auto nelle stationi della lista di stazioni "path".
        for(int i = 0; i < lenght; i++){
            for(int j = 0; j < station_size; j++){
                if(path[i].distance == station[j].distance){
                    path[i].max_range = 0;
                    for(int k = 0; k < MAX_CAR_NUM; k++){
                        if(path[i].max_range < station[j].range[k]){
                            path[i].max_range = station[j].range[k];
                        }
                        if(station[j].range[k] == INVALID){
                            break;
                        }
                    }
                    break;
                }
            }
        }

        // Cancello le stazioni che non possono raggiungere almeno una stazione successiva (da start a end). 
        for(int i = 0; i < lenght - 1; i++){
            if(path[i].distance == INVALID){
                break;
            }
            if((path[i+1].distance - path[i].distance) > path[i].max_range){
                
                for(int j = i; j < lenght; j++){
                    if(j == lenght - 1){
                        path[lenght -1].distance = INVALID;
                        lenght--;
                        break;
                    }
                    path[j].distance = path[j+1].distance;
                    path[j].max_range = path[j+1].max_range;
                }

                i--;
            } 
        }

        // Trovo la prossima stazione più lontana raggiungibile da ciascuna stazione
        for(int i = 0; i < lenght; i++){
            for(int j = i+1; j < lenght; j++){
                if((path[j].distance - path[i].distance) <= path[i].max_range){
                    path[i].furthest_next = j;
                }
                if((path[j].distance - path[i].distance) > path[i].max_range){
                    break;
                }
            }
        }

    }else if(start > end){

        inverted = true;
        int temp = start;
        start = end;
        end = temp;

        // Inserisco tutte le stationi con distanze tra "start" ed "end" nell'array "path" che avrà lunghezza "lenght".
        for(int i = 0; i < station_size; i++){

            if(station[i].distance <= end && station[i].distance >= start && station[i].distance != INVALID){
                
                path[lenght].distance = station[i].distance;
                lenght++;
                path = realloc(path, (lenght + 1)*sizeof(Station_list));

            }

        }
        
        // Faccio un mergeSort sull'array path per ordinare le stazioni dalla meno distante alla più distante.
        mergeSort(path, 0, lenght-1);

        // Inserisco il max range delle auto nelle stationi della lista di stazioni "path".
        for(int i = 0; i < lenght; i++){
            for(int j = 0; j < station_size; j++){
                if(path[i].distance == station[j].distance){
                    path[i].max_range = 0;
                    for(int k = 0; k < MAX_CAR_NUM; k++){
                        if(path[i].max_range < station[j].range[k]){
                            path[i].max_range = station[j].range[k];
                        }
                        if(station[j].range[k] == INVALID){
                            break;
                        }
                    }
                    break;
                }
            }
        }

        // Cancello le stazioni che non possono raggiungere almeno una stazione successiva (da end a start). 
        for(int i = (lenght-1); i > 0; i--){
            
            if(path[i].distance == INVALID){
                break;
            }
            if((path[i].distance - path[i-1].distance) > path[i].max_range){
                for(int j = i; j < lenght; j++){
                    if(j == lenght - 1){
                        path[lenght -1].distance = INVALID;
                        lenght--;
                        break;
                    }
                    path[j].distance = path[j+1].distance;
                    path[j].max_range = path[j+1].max_range;
                }
                i++;
            }
             
        }
        
        // Trovo la prossima stazione più lontana raggiungibile da ciascuna stazione
        for(int i = lenght -1; i >= 0; i--){
            for(int j = i-1; j >= 0; j--){
                if((path[i].distance - path[j].distance) <= path[i].max_range){
                    path[i].furthest_next = j;
                }
                if((path[i].distance - path[j].distance) > path[i].max_range){
                    break;
                }
            }
        }

    }

    if(start == end){
        printf("start\n");
        return;
    }

    if(path[0].distance != start || path[lenght-1].distance != end){
        printf("nessun percorso\n");
    }else{
        if(inverted){
            bfs_search_inverted();
        }else{ 
            bfs_search();
        }
    }

    // Cancello tutti i valori proesenti nell'array path (INVALID = -1).
    for(int i = 0; i < lenght; i++){
        path[i].distance = INVALID;
    }

    // CHECK PRINTERS
    /*************************
     * print_path();     
     * 
    */  

}

/****************************************************************** MAIN **************************************************************************/

int main(int argc, char *argv[]){ 

    station = malloc(sizeof(Station));
    station[0].range = malloc(sizeof(int));
    station[0].distance = INVALID;
    station[0].range[0] = INVALID;

    path = malloc(2*sizeof(Station_list)); 

    file = fopen("open_104.txt", "r");
    if(NULL == file){
        perror("error when opening file!");
        return INVALID;
    }	

	while(fgets(fileInput, 2, file) != NULL) {	

        switch (fileInput[0]){

        case 'a':
    
            if(fgets(buffer, 9, file) != NULL){
                if(fgets(fileInput, 2, file) != NULL){
                    if(strcmp(fileInput, "s") == 0 ){
                
                        if(fgets(buffer, 9, file) != NULL){
                            aggiungi_stazione();
                        }
                                                    
                    }else{

                        if(fgets(buffer, 5, file) != NULL){
                            aggiungi_auto();
                        }
                        

                    }
                }
            }
            break;

        case 'd':

            if(fgets(buffer, 19, file) != NULL){
                demolisci_stazione();
            }
            break;

        case 'r':
            
            if(fgets(buffer, 13, file) != NULL){
                rottama_auto();
            }
            break;

        case 'p':
            
            if(fgets(buffer, 19, file) != NULL){
                pianifica_percorso();
            }
            break;        

        default:
            break;
        }

    }

    // CHECK PRINTERS
    /*************************
     * print_range();    
     * 
    */ 

    fclose(file);
    return 0;
}
