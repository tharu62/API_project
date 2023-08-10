#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include <string.h>
#include <stdbool.h> 
#define MAX_CAR_NUM 513
#define MAT_ROW 512
#define INVALID -1

typedef struct {
    int distance;
    int* range;
}Station;

typedef struct {
    int distance;
    int max_range;
}Station_list;

typedef struct {
    int lenght;
    int first;
    int last;
    int* arr;
}Queue;

typedef struct {
    int next_station;
    int hop_counter;
    int* arr;
}Path;
 
FILE *file;
char carattereInFile[2];
char distance_string[10];
char num_car_string[10];
char range_string[10];
Queue* queue;
Station_list* path;
Station* station;
Path temp;
int station_size = 1;
int array_size = 1;
int pila_size = 1; 
int distance;
int num_car;
int range;
int start;
int end;
int lenght;
int current_pila_element = 0;
bool* visited;
int* prev;

/***************************************************************** PRINTERS ***********************************************************************/

void print_path(){
    for(int i = 0; i < lenght; i++){
        printf("%d : %d\n", path[i].distance, path[i].max_range);
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

bool QueueIsEmpty(){
    
    if(queue->first == queue->last){
        return true;
    }else{
        return false;
    }

}

void Enqueue(int value){

    queue->arr[queue->last] = value;
    queue->last++;

}

int Dequeue(){
    
    queue->first++; 
    return queue->arr[queue->first -1];

}

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

void reconstructedPath(int s, int e){
   
    int path_local[lenght];
    int j = 0;
    for(int i = e; i > 0 ; i = prev[i]){
        if(prev[i] == -1){
            break;
        }
        path_local[j] = i;
        printf("%d ->", path[i].distance);
        j++;
    
    }
    //free(queue->arr);

}
 
void solve(int s){

    int node;
    int neighbours[lenght];
    visited[s] = true;
    Enqueue(s);
    
    for(int k = 0; k < lenght; k++){
        neighbours[k] = INVALID;
    }

    while(!QueueIsEmpty()){
        node = Dequeue();
        int j = 0;
        for(int i = 0; i < lenght; i++){

            if((path[s+i].distance - path[s].distance) <= path[s].max_range){
                neighbours[j] = s+i;
                j++;
            }

        }
        neighbours[j] = INVALID;

        for(int k = 0; neighbours[k] != INVALID; k++){
            if(!visited[neighbours[k]]){       
                Enqueue(neighbours[k]);
                visited[neighbours[k]] = true;
                prev[neighbours[k]] = node;
            }
        }
    }    

}

void bfs(int s, int e){

    queue->arr = realloc(queue->arr, lenght*sizeof(int));
    queue->first = 0;
    queue->last = 0;
    queue->lenght = lenght;

    visited = realloc(visited, lenght*(sizeof(bool)));
    for(int i = 0; i < lenght; i++){
        visited[i] = false;
    }

    prev = realloc(prev, lenght*sizeof(int));
    solve(s);
    reconstructedPath(s, e);
    
    for(int i = 0; i < lenght; i++){
        prev[i] = INVALID;
    }
}

Path find(int first){
    Path* local_path = malloc(sizeof(Path));
    local_path[0].arr = malloc(lenght*sizeof(int));
    int j = 0;
    int k = 0;
    local_path[j].hop_counter = 0;

    // Salvo tutti le prossime stazioni che posso raggiungere, dalla stazione di indice "first", nell'array di percorsi "local_path". 
    for (int i = (first + 1); i < lenght; i++){
        if((path[i].distance - path[first].distance) <= path[first].max_range && path[i].distance == end){
            if(j > 0){
                local_path[j].hop_counter = local_path[j-1].hop_counter - 1;
                for(int l = 0; l < lenght; l++){
                    if(l == k){
                        break;
                    }
                    local_path[j].arr[l] = local_path[j-1].arr[l];
                }
            }
            local_path[j].arr[k] = end;
            local_path[j].arr[k+1] = INVALID; 
            local_path[j].hop_counter++;
            return local_path[j];
        }
        if((path[i].distance - path[first].distance) <= path[first].max_range){
            if(j > 0){
                local_path[j].hop_counter = local_path[j-1].hop_counter - 1;
                for(int l = 0; l < lenght; l++){
                    if(l == k){
                        break;
                    }
                    local_path[j].arr[l] = local_path[j-1].arr[l];
                }
            }
            local_path[j].arr[k] = path[i].distance;
            local_path[j].arr[k+1] = INVALID;
            local_path[j].next_station = i;
            local_path[j].hop_counter++;
            j++;
            local_path = realloc(local_path, (j+1)*sizeof(Path));
            local_path[j].arr = malloc(lenght*sizeof(int));
        }
        if(i == (lenght -1) && j == 1 || (path[i].distance - path[first].distance) > path[first].max_range && j == 1){
            j--;
            k++;
            first++;
            i = first;
        }
    }
    local_path[j].hop_counter = INVALID;

    // Per ogni prossima stazione trovata, trovo il percorso a hop e distanza minima per arrivare alla fine (parte di ricorsione).
    for(int i = 0; i < lenght; i++){

        if(local_path[i].hop_counter == INVALID){
            break;
        }

        temp = find(local_path[i].next_station);

        local_path[i].hop_counter += temp.hop_counter;
        
        for(int l = 0; l < lenght; l++){
            
            if(local_path[i].arr[l] == INVALID){
                
                int n = 0;
                for (int m = l; m < lenght; m++){

                    local_path[i].arr[m] = temp.arr[n];
                    if(temp.arr[n] == INVALID){
                        break;
                    }
                    n++;

                }
                break;
            }
        }
        
    }
        
    int min_hop = local_path[0].hop_counter;
    bool zero_extend = false;
    // Elimino tutti i percorsi con hop_counter maggiori del minimo. 
    for(int i = 0; i < lenght; i++){     
        if(zero_extend){
            zero_extend = false;
            i = 0;
        }
        if(min_hop < local_path[i].hop_counter){
            for(int l = i; l < lenght; l++){
                local_path[l].hop_counter = local_path[l+1].hop_counter;
                if(local_path[l].hop_counter == INVALID){
                    break;
                }
                for(int m = 0; m < lenght; m++){
                    local_path[l].arr[m] = local_path[l+1].arr[m];
                    if(local_path[l].arr[m] == INVALID){
                        break;
                    }
                }
            }
            zero_extend = true;
        }
        if(local_path[i].hop_counter == INVALID){
            break;
        }
        if(min_hop > local_path[i].hop_counter){
            min_hop = local_path[i].hop_counter;
            zero_extend = true;
        }
    }

    zero_extend = false;
    // Elimino tutti i percorsi che hanno una stazione a distanza maggiore del necessario.
    for(int i = 1; i < lenght && (local_path[i].hop_counter != INVALID); i++){
        if(zero_extend){
            zero_extend = false;
            i = 1;
        }
        for(int m = 0; m < lenght && (local_path[i].arr[m] != INVALID); m++){

            if(local_path[0].arr[m] != local_path[i].arr[m]){
                
                if(local_path[0].arr[m] > local_path[i].arr[m]){
                    for(int x = 0; x < lenght; x++){
                        if(local_path[x+1].hop_counter == INVALID){
                            local_path[x].hop_counter = INVALID;
                            break;
                        }
                        for(int y = 0; y < lenght; y++){
                            local_path[x].arr[y] = local_path[x+1].arr[y];
                            if(local_path[x].arr[y] == INVALID){
                                break;
                            }
                            
                        }
                    }
                }else{
                    for(int x = i; x < lenght; x++){
                        if(local_path[x+1].hop_counter == INVALID){
                            local_path[x].hop_counter = INVALID;
                            break;
                        } 
                        for(int y = 0; y < lenght; y++){
                            local_path[x].arr[y] = local_path[x+1].arr[y];
                            if(local_path[x].arr[y] == INVALID){
                                break;
                            }
                        }    
                    }
                }
                
                zero_extend = true;
                break;
            }

        }
    
    }

    /**
    printf("hop_counter of %d :\n" , path[first].distance);
    for(int i = 0; i < lenght; i++){

        if(local_path[i].hop_counter == INVALID){
            break;
        }
        printf("%d : ", i);
        for(int m = 0; m < lenght; m++){
            if(local_path[i].arr[m] == INVALID){
                break;
            }
            printf("%d ->", local_path[i].arr[m]);
        }
        printf("\n");
    }
    printf("\n");
    */

    return local_path[0];
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
            return;
        }
    }

    station_size++;
    station = realloc(station, station_size*sizeof(Station));
    station[station_size -1].distance = distance;
    station[station_size -1].range = calloc(513, sizeof(int));

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
            break;
        }
    }

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
                    station[i].range[j] = range;
                    station[i].range[j+1] = INVALID;
                    break;
                }
            }
            break;
        }
    }
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
                    break;
                }
            }
            break;
        }
    }

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
    
    lenght = 0;


    if(start < end){

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

    }else{

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
        
        printf("start > end\n");
        if(path[0].distance != start || path[lenght-1].distance != end){
            printf("nessun percorso\n");
        }

        // Cancello tutti i valori proesenti nell'array path (INVALID = -1).
        for(int i = 0; i < lenght; i++){
            path[i].distance = INVALID;
        }

        return;

    }
    
    
    if(path[0].distance != start || path[lenght-1].distance != end){

        printf("nessun percorso\n");
    
    }else{

        // Trovo il percorso più corto nel segmento di strada appena prodotto "path".
        Path new_path = find(0);

        printf("%d ->", start);
        for(int i = 0; i < lenght; i++){
            if(new_path.arr[i] == INVALID){
                break;
            }
            printf("%d ->", new_path.arr[i]);
        }
    }

    // Cancello tutti i valori proesenti nell'array path (INVALID = -1).
    for(int i = 0; i < lenght; i++){
        path[i].distance = INVALID;
    }

}

/****************************************************************** MAIN **************************************************************************/

int main(int argc, char *argv[]){

    station = malloc(sizeof(Station));
    station[0].distance = INVALID;
    station[0].range = calloc(1,sizeof(int));
    station[0].range[0] = INVALID;
    path = malloc(2*sizeof(Station_list));
    queue = malloc(sizeof(Queue));
    queue->arr = malloc(sizeof(int));
    visited = malloc(sizeof(bool));
    prev = malloc(sizeof(int));
    char fileInput[2];

    file = fopen("open_101.txt", "r");
    if(NULL == file){
        perror("error when opening file!");
        return INVALID;
    }	

	while(fgets(fileInput, 2, file) != NULL) {	
		
		if(strcmp(fileInput , "a") == 0){

            do{
                if(fgets(fileInput, 2, file) == NULL){
                    break;
                }
            }while(strcmp(fileInput, "-") != 0);
			if(fgets(fileInput, 2, file) != NULL);
			
            if(strcmp(fileInput, "s") == 0){
            	
                do{
                    if(fgets(fileInput, 2, file) == NULL){
                    break;
                    }
                }while(strcmp(fileInput, " ") != 0);

                aggiungi_stazione();
                
            }else{
            	
            	do{
                    if(fgets(fileInput, 2, file) == NULL){
                    break;
                    }
                }while(strcmp(fileInput, " ") != 0);
                aggiungi_auto();

            }
            
        }

        if(strcmp(fileInput , "d") == 0){
            do{
                if(fgets(fileInput, 2, file) == NULL){
                    break;
                }
            }while(strcmp(fileInput, " ") != 0);
            demolisci_stazione();
        
        }

        if(strcmp(fileInput , "r") == 0){
            do{
                if(fgets(fileInput, 2, file) == NULL){
                    break;
                }
            }while(strcmp(fileInput, " ") != 0);
            rottama_auto();
            
        }

        if(strcmp(fileInput , "p") == 0){
            do{
                if(fgets(fileInput, 2, file) == NULL){
                    break;
                }
            }while(strcmp(fileInput, " ") != 0);
            pianifica_percorso();
		}

    }

    //print_range();

    fclose(file);
    return 0;
}
