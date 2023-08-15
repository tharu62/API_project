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

typedef struct {
    int start_station;
    int hop_counter;
    int* arr;
}Path;

FILE *file;
char carattereInFile[2];
char distance_string[10];
char num_car_string[10];
char range_string[10];

Station_list* path;
Station* station;
Path temp;
int station_size = 1;
int distance;
int num_car;
int range;
int start;
int end;
int lenght;


/******************************************************************** SHIT ************************************************************************/

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

void mat_search(){

    bool mat[lenght][lenght];
    int final_path[lenght];
    int k = 0;

    /**
     * mat[][i] = start stations
     * mat[j][] = end stations
     * 
     *         0   1   2   3   4  i
     *      0 [T] [T] [T] [T] [T]
     *      1 [F] [T] [T] [T] [T]
     *      2 [F] [F] [T] [T] [T]
     *      3 [F] [F] [F] [T] [T]
     *      4 [F] [F] [F] [F] [T]
     *      j
     * 
    */

    // Inizializzo la matrice.
    for(int i = 0; i < lenght; i++){
        
        for(int j = 0; j < lenght; j++){
            
            if(path[i].furthest_next >= j){
                mat[j][i] = true;
            }else{
                mat[j][i] = false;
            }

        } 
    }
    
    /**
    printf("\n"); 
    for(int i = 0; i < lenght; i++){
        printf("%d :", i);
        printf("%d\n", path[i].distance); 
    }


    printf("\n"); 
    for(int i = 0; i < lenght; i++){
        printf("%d :", i);
        for(int j = 0; j < lenght; j++){
            printf("[%d]", mat[i][j]);
        }
        printf("\n"); 
    }
    */
    
    // Cerco e salvo il path con stazioni più vicine.
    for(int j = lenght-1; j >= 0; j--){   
        for(int i = 0; i < lenght; i++){
            if(mat[j][i]){
                final_path[k] = j;
                k++;
                if(i != 0){
                    j = i+1;
                }
                break; 
            }   
        }
    }
    final_path[k] = INVALID;

    // Stampo il percorso trovato.
    printf("\n");
    for(int i = 0; i < lenght; i++){
        if(final_path[i] == INVALID){
            break;
        }
        printf("%d ->", path[final_path[i]].distance);
    }

}

void mat_search_inverted(){
    bool mat[lenght][lenght];
    int final_path[lenght];
    int k = 0;

    /**
     * mat[j][] = start stations
     * mat[][i] = end stations
     * 
     *         0   1   2   3   4  i
     *      0 [T] [F] [F] [F] [F]
     *      1 [T] [T] [F] [F] [F]
     *      2 [T] [T] [T] [F] [F]
     *      3 [T] [T] [T] [T] [F]
     *      4 [T] [T] [T] [T] [T]
     *      j
     * 
    */

    // Inizializzo la matrice.
    for(int i = 0; i < lenght; i++){
    
        for(int j = 0; j < lenght; j++){
            
            if(path[i].furthest_next <= j || i == 0){
                mat[j][i] = true;
            }else{
                mat[j][i] = false;
            }

        } 
    }

    /**
    printf("\n"); 
    for(int i = 0; i < lenght; i++){
        printf("%d :", i);
        for(int j = 0; j < lenght; j++){
            printf("[%d]", mat[i][j]);
        }
        printf("\n"); 
    }
   
   
    printf("\n"); 
    for(int i = 0; i < lenght; i++){
        printf("%d :", i);
        printf("%d\n", path[i].distance); 
    }
    */

    // Cerco e salvo il path con stazioni più vicine.
    for(int j = 0; j < lenght; j++){   
        for(int i = j+1; i < lenght; i++){
            if(mat[j][i]){
                final_path[k] = j;
                k++;
                j = i-1;
                break; 
            }   
        }
    }
    final_path[k] = INVALID;

    // Stampo il percorso trovato.
    printf("\n");
    for(int i = 0; i < lenght; i++){
        if(final_path[i] == INVALID){
            break;
        }
        printf("%d ->", path[final_path[i]].distance);
    }
   
}

void find_iterative(){
    
    /**
     * Local_path contiene gli array con i percorsi ottenuti dai vari percorsi di 1 bivio. Oltre ai percorsi sono salvsti anche l'hop_counter di
     * quel percorso.
    */
    Path* Local_path = malloc(lenght*sizeof(Path));
    int last_allocated_index = 0;

    /** 
     * Pile contiene la lista di stazioni da cui è stato attraversato un "ramo" di un "bivio" (scegliendo in maniera non arbitraria il ramo).
     * Per esempio [stazione_corrente-> stazione_raggiungibile_con_1_hop] : 
     * (x-> 2) && (x-> 5) && (x-> 7) => inserisco x dentro Pile e attravero la prima stazione più vicina ossia 2.
    */ 
    int* Pile = malloc(lenght*sizeof(int));
    int current_pile = 0;
    
    /**
     * Queue contine tutte le prossime stazioni raggiungibili con 1 hop dalla stazione corrente.
    */ 
    int* Queue = malloc(lenght*sizeof(int));
    int current_queue = 0;
    int last_queue = 0;

    /**
     * Full_path contiene il path ottimale trovato fino al momento corrente. (NON E' DETTO CHE SIA SEMPRE COMPLETO) 
    */
    int* Full_path = malloc(lenght*sizeof(int));
    int current_hop = 0;

    
    // Inizializzo Pile.
    for(int i = 0; i < lenght; i++){
        
        if((path[i].furthest_next - i) >= 2){
            
            Pile[current_pile] = i;
            current_pile++;

        }

    }

    while(Full_path[current_hop] != start){

        // Inizializzo la Queue.
        for(int j = 0; j < lenght; j++){

            if((path[Pile[current_pile]].furthest_next - j) == Pile[current_pile]){
                break;
            }
            Queue[current_queue] = path[Pile[current_pile]].furthest_next - Pile[current_pile];
            current_queue++;

        }
        current_pile--;

        // Trovo il percorso per ogni stazione in Queue e lo salvo in Local_path[x].arr[y].
        for(int i = Queue[last_queue]; i < lenght; i++){

        }

        // Trovo i percorsi salvati con hop_counter minimo.

        // Trovo i percorsi salvati con stazioni a distanza mininore dallo start.

        // Aggiorno Full_path.

    }
    


}

void find_iterative_inverted(){
    
    /**
     * Local_path contiene gli array con i percorsi ottenuti dai vari percorsi di 1 bivio. Oltre ai percorsi sono salvsti anche l'hop_counter di
     * quel percorso.
    */
    Path* Local_path = malloc(lenght*sizeof(Path));
    int current_local_path = 0;
    int last_allocated_index = 0;

    /** 
     * Pile contiene la lista di stazioni da cui è stato attraversato un "ramo" di un "bivio" (scegliendo in maniera non arbitraria il ramo).
     * Per esempio [stazione_corrente-> stazione_raggiungibile_con_1_hop] : 
     * (x-> 2) && (x-> 5) && (x-> 7) => inserisco x dentro Pile e attravero la prima stazione più vicina ossia 2.
    */ 
    int* Pile = malloc(lenght*sizeof(int));
    int current_pile = 0;
    
    /**
     * Queue contine tutte le prossime stazioni raggiungibili con 1 hop dalla stazione corrente.
    */ 
    int* Queue = malloc(lenght*sizeof(int));
    int current_queue = 0;
    int last_queue = 0;

    /**
     * Full_path contiene il path ottimale trovato fino al momento corrente. (NON E' DETTO CHE SIA SEMPRE COMPLETO) 
    */
    int* Full_path = malloc(lenght*sizeof(int));
    int current_hop = 0;

    
    // Inizializzo Pile.
    for(int i = lenght-1; i > 0; i--){
        
        if((i - path[i].furthest_next) >= 2){
            
            Pile[current_pile] = i;
            current_pile++;

        }

    }
    

    while(current_pile != 0){

        // Inizializzo la Queue.
        for(int j = 0; j < lenght; j++){

            if((path[Pile[current_pile-1]].furthest_next + j) == Pile[current_pile]){
                break;
            }
            Queue[current_queue] = path[Pile[current_pile-1]].furthest_next + j;
            current_queue++;

        }
    
        // Inizializzo Local_path.
        for(int i = 0; i <= current_queue; i++){
            
            Local_path[i].start_station = Queue[i];
            if(last_allocated_index <= i){
                Local_path[i].arr = malloc(sizeof(lenght*sizeof(int)));
                last_allocated_index++;
            }
            current_local_path++;
        }
                        
        /**
         * Inserisco il percorso dalle stazioni Queue[i] appena inserite in Local_path[i].start_station.
         * Controllo prima che ci sia già un percorso da quella stazione in Full_path, se c'è la copio in Local_path[i].arr ,
         * altrimenti inserisco tutte le stazioni da 0 a Queue[i] in Local_path[i].arr .
        */ 
        for(int i = 0; i < current_local_path; i++){
            Local_path[i].hop_counter = 0;
            for(int j = 0; j < lenght; j++){

                if(j < current_hop && Local_path[i].start_station == Full_path[j]){
                    for(int k = 0; k <= j; k++){
                        Local_path[i].arr[k] = Full_path[k];
                        Local_path[i].hop_counter++;
                        if(k == j){
                            Local_path[i].arr[k+1] = INVALID;
                            break;
                        }
                    }
                    break;
                }

                if(j == current_hop){
                    for(int k = 0; k <= Local_path[i].start_station; k++){
                        Local_path[i].arr[k] = k;
                        Local_path[i].hop_counter++;
                        if(k == Local_path[i].start_station){
                            Local_path[i].arr[k+1] = INVALID;
                            break; 
                        }
                    }
                    break;
                }

            }
            
        }

    
        // Trovo i percorsi salvati con hop_counter minimo eliminando tutti i percorsi con hop_counter maggiori del minimo.
        int min_hop = Local_path[0].hop_counter;
        bool zero_extend = false;
        for(int i = 0; i < current_local_path; i++){     
            if(zero_extend){
                zero_extend = false;
                i = 0;
            }
            if(min_hop < Local_path[i].hop_counter){
                for(int j = i; j < current_local_path-1; j++){
                    Local_path[j].hop_counter = Local_path[j+1].hop_counter;
                    for(int k = 0; k < lenght; k++){
                        Local_path[j].arr[k] = Local_path[j+1].arr[k];
                        if(Local_path[j].arr[k] == INVALID){
                            break;
                        }
                    }
                }
                current_local_path--;
                zero_extend = true;
            }
            if(Local_path[i].hop_counter == INVALID){
                break;
            }
            if(min_hop > Local_path[i].hop_counter){
                min_hop = Local_path[i].hop_counter;
                zero_extend = true;
            }
        }
        
        printf("CICLO N:\n");
        for(int i = 0; i < current_local_path; i++){
            printf("%d :", i);
            for(int j = 0; j < lenght; j++){
                if(Local_path[i].arr[j] == INVALID){
                    break;
                }
                printf("%d ->", Local_path[i].arr[j]);
            }
            printf("\n");
        }

        break;

        /**
        // Trovo i percorsi salvati con stazioni a distanza minima da start eliminando tutti i percorsi a distanza maggiore del necessario. VA IN LOOP
        zero_extend = false;
        for(int i = 1; i < current_local_path; i++){

            if(zero_extend){
                zero_extend = false;
                i = 1;
            }

            
            for(int m = 0; m < lenght; m++){

                if(Local_path[i].arr[m] == INVALID){
                    break;
                }
                    
                if(Local_path[0].arr[m] > Local_path[i].arr[m]){
                    
                    for(int k = 0; k < lenght; k++){
                        
                        printf("%d - %d | ", Local_path[0].arr[k],Local_path[m].arr[k]);
                        if(Local_path[0].arr[k] == INVALID){
                            break;
                        }
                    }
                    printf("\n");


                    
                    for(int x = 0; x < current_local_path-1; x++){
                        if(Local_path[x+1].hop_counter == INVALID){
                            Local_path[x].hop_counter = INVALID;
                            break;
                        }
                        for(int y = 0; y < lenght; y++){
                            if(Local_path[x+1].arr[y] == INVALID){
                                Local_path[x].arr[y] = INVALID;
                                break;
                            }
                            Local_path[x].arr[y] = Local_path[x+1].arr[y];
                        }
                    }
                    current_local_path--;
                    zero_extend = true;
                    break;
                    

                }else if(Local_path[0].arr[m] < Local_path[i].arr[m]){
                    
                    
                    for(int x = i; x < current_local_path-1; x++){
                        if(Local_path[x+1].hop_counter == INVALID){
                            Local_path[x].hop_counter = INVALID;
                            break;
                        } 
                        for(int y = 0; y < lenght; y++){
                            Local_path[x].arr[y] = Local_path[x+1].arr[y];
                            if(Local_path[x].arr[y] == INVALID){
                                break;
                            }
                        }    
                    }
                    

                    for(int k = 0; k < lenght; k++){
                        
                        printf("%d - %d | ", Local_path[0].arr[k],Local_path[m].arr[k]);
                        if(Local_path[0].arr[k] == INVALID){
                            break;
                        }
                    }
                    printf("\n");

                    current_local_path--;
                    zero_extend = true;
                    break;
                }
                    
            }
            
        }
        */
        
        /***
        // Aggiorno Full_path.
        for(int i = 0; i < lenght; i++){
            if(Local_path[0].arr[i] == INVALID){
                
                // CONTINUO A RIEMPIRE FINO AL PROSSIMO BIVIO. DA FARE.
                if(Pile[current_pile-1] - Pile[current_pile] >= 1){
                    for(int j = i; j < Pile[current_pile-1]; j++){
                        Full_path[j] = j;
                        if(j == Pile[current_pile-1]-1){
                            Full_path[j+1] = INVALID;
                            current_hop = j+1;
                            break;
                        }
                    }

                }else{
                    Full_path[i] = INVALID;
                    current_hop = i;
                }
                break;
            }
            Full_path[i] = Local_path[0].arr[i];
            
        }
        */

        last_queue = 0;
        current_queue = 0;
        current_pile--;
        current_local_path = 0;

    }

    /**
    for(int i = current_hop; i > 0; i--){
        printf("%d ->", Full_path[i]);
    }
    */
}

/**************************************************************** FUNCTIONS **********************************************************************/

void free_all(){
    free(path);
    for(int i = 0; i < station_size; i++){
        free(station[i].range);
    }
    free(station);
}

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

        
        // Elimino tutte le stazioni di inframezzo, ossia che stanno in mezzo a due stazioni collegate che sono sicuramente il path ottimale per quel segmento.
        for(int i = 0; i < lenght; i++){
            for(int j = i+1; j < lenght - 1; j++){
                if(path[i].distance == -2){
                    break;
                }
                if(path[j].furthest_next <= path[i].furthest_next ){
                    path[j].distance = -2;
                }
                if(j >= path[i].furthest_next){
                    break;
                }
            }
        }
        
        for(int i = 0; i < lenght; i++){

            if(path[i].distance == -2){

                lenght--;
                for(int j = i; j < lenght; j++){
                    path[j] = path[j+1];
                    if(path[i].distance == INVALID){
                        break;
                    }
                }
                i--;
            }
            if(path[i].distance == INVALID){
                break;
            }

        }

        // Ritrovo la prossima stazione più lontana raggiungibile da ciascuna stazione
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


    }else{

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
        
        // Elimino tutte le stazioni di inframezzo, ossia che stanno in mezzo a due stazioni collegate che sono sicuramente il path ottimale per quel segmento.
        bool reach = true;
        for(int i = 1; i < lenght; i++){
            for(int j = i-1; j < lenght; j++){

                // DA CAPIRE.
                if(path[i].distance == -2){
                    break;
                }
                if(j >= i){
                    break;
                }
                if(path[j].furthest_next >= path[i].furthest_next && path[j].distance != -2 && (i - path[i].furthest_next) >= 3){
                    
                    for(int k = j+1; k < i; k++){
                        if(path[k].furthest_next <= path[i].furthest_next && path[k].distance != -2){
                            reach = false;
                            break;
                        }
                    }

                    if(reach){
                        for(int k = j; k < i; k++){
                            path[k].distance = -2;
                        }
                        reach = true;
                    }
                    

                }   
            }
        }
        
        for(int i = 0; i < lenght; i++){

            if(path[i].distance == -2){

                lenght--;
                for(int j = i; j < lenght; j++){
                    path[j] = path[j+1];
                    if(path[i].distance == INVALID){
                        break;
                    }
                }
                i--;
            }
            if(path[i].distance == INVALID){
                break;
            }

        }

        // Ritrovo la prossima stazione più lontana raggiungibile da ciascuna stazione
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

    

    if(path[0].distance != start || path[lenght-1].distance != end){

        printf("nessun percorso\n");
    
    }else{

        //printf("%d\n", lenght);
        //print_path();
        
        if(inverted){

            find_iterative_inverted();
            
        }else{

        }
    
        //print_path();
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
    station[0].range = malloc(sizeof(int));
    station[0].range[0] = INVALID;
    path = malloc(2*sizeof(Station_list));
    //Optimal_path = malloc(sizeof(Path));
    //Optimal_path[0].next_station = INVALID;
    //path_ref = malloc(sizeof(Path_ref));
    //path_ref[0].index = INVALID;
    char fileInput[2];
    char buffer[20];

    file = fopen("open_3.txt", "r");
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

    //print_range();

    fclose(file);
    return 0;
}
