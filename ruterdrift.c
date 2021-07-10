#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BUF_LEN 1000
#define NEIGHBORE_LIST_SIZE 10


struct ruter {
    int id;
    char flagg;
    char visited;
    unsigned char *modell;
    struct ruter **naboer;

};

struct ruter **r_array; // a pointer to an array of struct ruter pointers, unknown size
int main_struct_arr_size; // global variable that will hold the size of the array of struct pointers

/*the function is used at the initiall stage when reading binary file and its function is
 to find the element with the given id
@ parameters takes in an int id and int size of the main array of pointers
 even though i could have used main_struct_arr_size here i opted to keep this older version
@ return returns a pointer to struct ruter if found, else NULL
 */
struct ruter * findElement(int id, int size){
     for (int i =0; i<size; i++){
        if (r_array[i]->id == id){
            return r_array[i];
        }
    }
    return NULL;

}
/*
is used at later stages in the program as a fix for deleted elements
@parameters int id
@return a pointer to a struct router with a given id or NULL if not found
*/
struct ruter *findElement_deleteFix(int id)
{
    for (int i = 0; i < main_struct_arr_size; i++)
    {
        if (r_array[i] != NULL)
        {

            if (r_array[i]->id == id)
            {
                return r_array[i];
            }
        } else continue;
    }
    return NULL;
}
/*
the function prints individual bites 
used with the permission from group teachers in Cbra 
*/
void printbits(void *n, int size) {
    char *num = (char *)n;
    int i, j;

    for (i = size-1; i >= 0; i--) {
        for (j = 7; j >= 0; j--) {
            printf("%c", (num[i] & (1 << j)) ? '1' : '0');
        }
        printf(" ");
    }
    printf("\n");
}
/*
increases the value of endringsnummer by 1
@param pointer to a struct ruter
*/
void add_one_to_endringsnummer(struct ruter *r){
    
    char old_flagg_value = r->flagg;
    
    r->flagg = (r->flagg >>4)+1;
    if(r->flagg > 15) {
    r->flagg = 0;
    }
     
    r->flagg = r->flagg << 4 | (old_flagg_value & 15); 

}
/*
print out all outgoing edges from a given ruter
@param pointer to a struct ruter
*/
void print_neighborsFrom(struct ruter *p){
     for (size_t y = 0; y < NEIGHBORE_LIST_SIZE; y++)
        {   if(p->naboer[y] != 0)
            printf("%d > ", p->naboer[y]->id);
        }
        printf("\n");
}
/*
print out all edges towards the given ruter
@param pointer to a struct ruter
*/
void print_neighbors_to(int ide, int size)
{
    for (size_t i = 0; i < size; i++)
    {
        if (r_array[i] != NULL)
        {
            if (r_array[i]->id != ide)
            {
                for (size_t y = 0; y < NEIGHBORE_LIST_SIZE; y++)
                {
                    if (r_array[i]->naboer[y] != 0)
                    {

                        if (r_array[i]->naboer[y]->id == ide)
                            printf("%d > ", r_array[i]->id);
                    }
                }
            }
        }
    }
}
/*
prints ruter attributes if found
@param int id
*/
void print(int my_id){
    
    struct ruter *r;
    r = findElement_deleteFix(my_id); 
    if(r!=NULL){

        printf("id: %d\n", r->id);
        printf("flag: ");
        printbits(&r->flagg, sizeof(char));
        printf("modell: %s\n", r->modell);
        print_neighbors_to(r->id, main_struct_arr_size);
        printf(" ** %d ** > ", r->id);
        print_neighborsFrom(r);

        printf("\n");

    }
    else printf("Unable to find the router with the given id\n");
}
/*
prints all routers with respective atributes if found
*/
void print_all(){
    
    for (int i = 0; i<main_struct_arr_size; i++){// pazi ovde je 10 a treba da bude dinamicka velicina
        if(r_array[i] !=NULL){
        
        printf("id: %d\n", r_array[i]->id);
        printf("flagg: ");
        printbits(&r_array[i]->flagg, sizeof(char));
        printf("modell: %s\n", r_array[i]->modell);
        print_neighbors_to(r_array[i]->id, main_struct_arr_size);
        printf(" ** %d ** > ", r_array[i]->id);
       
        print_neighborsFrom(r_array[i]);
        printf("\n");

        }
    }
}
/*
frees the heap memory
free(r_array[i]->modell); -> number of bytes allocated by strdup(?)
free(r_array[i]->naboer); -> 10x8bytes
free(r_array[i]); -> at least 24bytes
*/
void free_all(){
    for (int i = 0; i<main_struct_arr_size; i++){
        if(r_array[i] != NULL){
        free(r_array[i]->modell);
        free(r_array[i]->naboer);
        free(r_array[i]);

        }
    }
    free(r_array); // frees 8x10 bytes
}
/*
@param int id, char flagg, pointer to a string m
@return a pointer to a struct ruter allocated on the heap
*/
struct  ruter * make_ruter  (int id, char f, unsigned char *m){
    struct ruter *r = malloc(sizeof(struct ruter)); // allocated at least 24bytes
    if(r==NULL){
        fprintf(stderr, "malloc failed");
        exit(EXIT_FAILURE);
        
    }
    r->id = id;
    r->flagg = f;
    r->modell = strdup(m); // allocates number of bytes needed, inferred from "m"

    if (r->modell == NULL){
        perror("strdup failed");
        exit(EXIT_FAILURE);
    }
    r->naboer = calloc(sizeof(struct ruter *), 10);// 10x8 bytes
    if (r->naboer == NULL){
        perror("calloc naboer failed");
        exit(EXIT_FAILURE);
    }
    return r;

}
/*
places egdes in the respectable lists
@param pointer to an int array, int n. Due to testing and time constraints, i decided to keep n.
I an aware of its redundancy
@return 1 on success, 0 on failure
*/
int enter_neighbours(int arr[], int n)
{
    struct ruter *r1 = findElement(arr[0], n);
    struct ruter *r2 = findElement(arr[1], n);
    if (r1 != NULL && r2 != NULL)
    {
        //    printf("success!");
        //    printf("%d\n", r1->id);
        //     printf("%d\n", r2->id);
        for (size_t i = 0; i < NEIGHBORE_LIST_SIZE; i++)
        {
            if (r1->naboer[i] == 0)
            { // promenio !

                r1->naboer[i] = r2;
                break;
            }
        }
        return 1;
    }
    else
    {
        printf("Failed to set neighbors, full array?");
        return 0;
    }
}
/*
@param char pointer to filename, int pointer 
*/
void read_file(const char *filnavn, int *s){
    FILE *fil;
    int n, rc;
    char name_modell[250];

    fil = fopen(filnavn, "rb");
    if (fil == NULL){
        perror("fopen");
        exit(EXIT_FAILURE);
    }
    rc = fread(&n, sizeof(int), 1, fil);
    if (!rc) perror("fread error on n.");
    
    *s = n; // assigning the value to a global size variable
   

    r_array = malloc(n * sizeof(struct ruter *)); // Nx8 bytes
    int id;
    char flagg, temp_null;
    unsigned char lengde_modell;

    for (int i = 0; i<n; i++){
        rc = fread(&id, sizeof(int), 1, fil);
        if (!rc)
            perror("fread error on id.");

        rc = fread(&flagg, sizeof(char), 1, fil);
        if (!rc)
            perror("fread error on flagg.");

        rc = fread(&lengde_modell, sizeof(char), 1, fil);
        if (!rc)
            perror("fread error on lengde_modell.");
            
        rc = fread(&name_modell, sizeof(char), lengde_modell, fil);
        name_modell[lengde_modell] = 0;

        fread(&temp_null, sizeof(char), 1, fil);

        r_array[i] = make_ruter(id, flagg, name_modell);
    
    }

    int buf[2];
    while(1==1){
    fread(&buf, sizeof(int), 2, fil);
    
    if(feof(fil))
        break;

    enter_neighbours(buf, n);    
    }
    fclose(fil);
}
/*
uses switch to determine which bite is to be changed 
@param int id,unsigned char flagg, unsigned char bit
*/
void sett_flagg(int r_id, unsigned char flagg, unsigned char bit){
    struct ruter *r;
    r = findElement_deleteFix(r_id); // size!
    if(r!=NULL){
        switch(flagg){

         case 0:
         r->flagg = (r->flagg & (~1)) ^ bit;
           break;
         case 1:
         r->flagg = (r->flagg & (~2)) ^ bit<<flagg;
           break;
          case 2:
          r->flagg = (r->flagg & (~4)) ^ bit<<flagg;
           break;
          case 4:
          if (bit>=0 && bit <16){
          r->flagg = (bit<<4) | (r->flagg & 15);

          }printf("To big number on sett flag position 4!");
           break;  
        
        default:
           printf("Input feil on sett flagg!");
           
        } 
        } else printf("Error, found no ellement with that id!");
}
/*
gets the pointer to a router with the given id, frees the heap memory allocated by the strdup at read_file
and allocates a new portion of memory determened by strdup function
*/
void sett_modell(int r_id, char *name){
    struct ruter *r;
    r = findElement_deleteFix(r_id);
    if (r != NULL)
    {
        free(r->modell);
        r->modell = strdup(name);
        if (r->modell == NULL)
        {
            perror("strdup failed");
            exit(EXIT_FAILURE);
        }
    }
    else
        printf("Sett_modell: couldn't find the element with the given id");
}
/*
add a new edge, if the router are found and the enter_neighbours was successful, one is added to endringsnummer
@param int r1 and int r2
*/
void legg_til_kobling (int r1, int r2){
   
   struct ruter *r = findElement_deleteFix(r1);
   struct ruter *r_two = findElement_deleteFix(r2);
   if(r != NULL && r_two != NULL){
               
       for (size_t i = 0; i < NEIGHBORE_LIST_SIZE; i++)
       {
           if(r->naboer[i] != 0){
               
                if(r->naboer[i]->id == r2){
                    
                    printf("Alredy on the list!\n");
                    return;
                }
           }
       }
   int arr [2];
   arr[0] = r1;
   arr[1] = r2;    
   
   if(enter_neighbours(arr, main_struct_arr_size))
    
    add_one_to_endringsnummer(r);
    
    } else printf("Unable to find the element!");

}

void add_one_to_my_connections(struct ruter *r){
    for (size_t i = 0; i < NEIGHBORE_LIST_SIZE; i++)
    {
        if (r->naboer[i] !=0)
        {
            add_one_to_endringsnummer(r->naboer[i]);
        }
        
    }
    
}
/*
Deletes the router with the given id if found.
It first goes over all the routers with the id that doesn't match the id specified and deletes
a pointer to a given id if found (replaces with 0). Then it increases the value of endringsnummer.
Then it searches after the router with the given id and increases endringsnummer to all of its connections (edges)
After that the memory allocated is freed up and the struct pointer towards this router is set to point to NULL
*/
void slett_ruter(int rid)
{
    struct ruter *r;
    
    r = findElement_deleteFix(rid); 
    if (r != NULL)
    {
        for (size_t i = 0; i < main_struct_arr_size; i++) 
        {   
            if(r_array[i] !=NULL){

            if (r_array[i]->id != rid)
            {
                
                for (size_t y = 0; y < NEIGHBORE_LIST_SIZE; y++)// size
                {
                    if (r_array[i]->naboer[y] != 0)
                    {
                        
                        if (r_array[i]->naboer[y]->id == rid){
                            r_array[i]->naboer[y] = 0;
                            
                            add_one_to_endringsnummer(r_array[i]);

                        }
                    }
                }
            }

            } else continue;
            
        }
        
         for (int z = 0; z<main_struct_arr_size; z++){
            if(r_array[z] != NULL){

             if (r_array[z]->id == rid){
                add_one_to_my_connections(r_array[z]);
                 free(r_array[z]->naboer);
                 free(r_array[z]->modell);
                 free(r_array[z]);
                 r_array[z] = NULL;
             } 

            } else continue;
         }

    }
    else printf("Slett_rute error!");
}
void sett_visited_to_false(){
    for (size_t i = 0; i < main_struct_arr_size; i++)
    {   if(r_array[i] != NULL){

        r_array[i]->visited = 0;

    }
    }
    
}
/*
dfs search
@param pointer to a start router, int start and int end.
it prints appropriate message if the connection is found as well as path
*/
int dfs (struct ruter *r1, int r_start, int r2){
    r1->visited=1;
    int r=0;
    if(r1->id == r2){
        printf("SUCCESS FOUND CONNECTION!\n");
        
        printf(" %d < ", r1->id);
        r =1;
        return r;

    }
    for(int i =0; i<NEIGHBORE_LIST_SIZE; i++){
        if(r1->naboer[i]!= 0){
        if(r1->naboer[i]->visited == 0){
            if(!r){

              r = dfs(r1->naboer[i], r_start, r2);

            }
            else dfs(r1->naboer[i], r_start, r2);

        }
        }
    }
    if(r==1) printf(" %d < ", r1->id);
    return r;
    
}
/*
function used to call dfs
*/
void finnes_rute(int r1, int r2){
    sett_visited_to_false();
    
    struct ruter *r = findElement_deleteFix(r1 );
    struct ruter *r_two = findElement_deleteFix(r2) ;
   if(r != NULL && r_two != NULL){
       printf("\n");
    if(!dfs(r, r1,r2)) printf("Found no connection between %d %d\n", r1, r2);
       printf("\n");
   }
   else printf("No connection found, possibly due to a nonexistent id!\n");


}

/*
function reads the commands from a file
@param pointer to a file descriptor
*/
char *read_commands(FILE *fd) {
    int index = 0;
    char *buffer = (char *)malloc(50);//50xsizeof(char)

    if (buffer == NULL) {
        perror("malloc failed");
        return NULL;
    }
    
    while (fread(buffer + index, 1, 1, fd) && index < 50 && buffer[index++] != '\n');// this line is taken from group 4 exercises, available on the git page for group 4 IN2140

    if (index > 50) {
        perror("buffer is full");
        buffer[index - 2] = '\0';
    } else {
        buffer[index] = '\0';
    }
    
    if (feof(fd)) {
        clearerr(fd);
        free(buffer);// frees 50xsizeof(char)
        return NULL;
    } 

    return buffer;
}
/*
function uses fopen to get a file descriptor to a new file in write binary modus
it then goes over the elements in the struct array of pointers and writes every element attributes as it is placed in memory
it then goes to write all the respectable edges of every router before closing the file.
*/
void write_to_file (){
char *filename = "new-topology.dat";
	
	FILE *fd = fopen(filename , "wb");
	if (!fd) {
		fprintf(stderr, "Error in opening file for writing: %s\n", filename);
		perror("Error:");
		exit(EXIT_FAILURE);
	}
	
	
	int wc;
    wc = fwrite(&main_struct_arr_size, sizeof(int), 1, fd);
    if (!wc) perror("fread error on n.");

    char name_modell[250];
    int id;
    char flagg, temp_null;
    unsigned char lengde_modell;

    for (int i = 0; i<main_struct_arr_size; i++){
        if(r_array[i] != NULL){

        id = r_array[i]->id;
        flagg = r_array[i]->flagg;
        lengde_modell = strlen(r_array[i]->modell);
        temp_null = 0;
        strcpy(name_modell, r_array[i]->modell);
        
        wc = fwrite(&id, sizeof(int), 1, fd);
        if (!wc)
            perror("fread error on id.");

        wc = fwrite(&flagg, sizeof(char), 1, fd);
        if (!wc)
            perror("fread error on id."); 

        wc = fwrite(&lengde_modell, sizeof(char),1,fd);     
            if (!wc)
            perror("fread error on id.");

        wc = fwrite(&name_modell, sizeof(char), lengde_modell, fd);
        if (!wc)
            perror("fread error on id."); 

        wc = fwrite(&temp_null, sizeof(char), 1, fd);
        if (!wc)
            perror("fread error on id.");    

        }
    
    }
    int r1;
    int r2;
    for (size_t i = 0; i < main_struct_arr_size; i++)
    {
        for (size_t y = 0; y < NEIGHBORE_LIST_SIZE; y++)
        {
            if (r_array[i] != NULL && r_array[i]->naboer[y] !=0){
                r1 = r_array[i]->id;
                r2 = r_array[i]->naboer[y]->id;
                wc = fwrite(&r1, sizeof(int), 1, fd);
                if (!wc)
                    perror("fread error on id.");   
                wc = fwrite(&r2, sizeof(int), 1, fd);
                if (!wc)
                    perror("fread error on id."); 
                    
            }

        }
        
    }
	fclose(fd); /* Done writing to file */
    
}
/*
the function reads line by line from command file and determines what function is to be called.
@param pointer to a file descriptor
*/
void do_the_commands (FILE *fd){
     char *str;
     while ((str = read_commands(fd)) != NULL) {
        /*
        As the new line of text is read, fgets reads the 
        whole line inluding '\n', later when i try to concatinate i get the new line
        as well and it prints one extra new line after a model name has been changed
        thats why i replace the '\n' with '\0' on the next line
        https://stackoverflow.com/questions/58223895/concatenating-strings-in-c-newline-in-output
        */
        str[strlen(str)-1] = '\0';
         char *ret = strtok(str, " ");
	 	
          if (strcmp(ret, "print") == 0)
         {
             int r_id;
             int index = 0;
             do
             {
                 if (index == 1)
                     r_id = atoi(ret);
                 index++;

             } while ((ret = strtok(NULL, " ")));

             if(r_id != 0){
                 print(r_id);
             }
         }
         else if(strcmp(ret, "sett_modell")==0){
             int r_id;
             int index = 0;
             char modell[BUF_LEN];
             strcpy(modell, "");
             

             do
             {
                 if (index == 1)
                     r_id = atoi(ret);
                 if(index >1){
                     strcat(modell, ret);
                     strcat(modell,  " ");
                 }    
                 index++;

             } while ((ret = strtok(NULL, " ")));
             
             
             modell[strlen(modell)-1] = '\0';// deleting the white space at the end.
             sett_modell(r_id, modell);
         }
         else if (strcmp(ret, "sett_flagg")==0){

             int r_id;
             char flagg;
             char bit;
             int index = 0;
             do
             {
                 if (index == 1)
                     r_id = atoi(ret);
                 if (index == 2)
                     flagg = atoi(ret); 
                 if (index == 3)
                     bit = atoi(ret);         

                 index++;

             } while ((ret = strtok(NULL, " ")));
             if(r_id != 0){

             sett_flagg(r_id,flagg,bit);
                
             }
            }

         else if (strcmp(ret, "finnes_rute")==0){
             int r_id;
             int r2_id;
            
             int index = 0;
             do
             {
                 if (index == 1)
                     r_id = atoi(ret);
                 if (index == 2)
                     r2_id = atoi(ret); 
                        

                 index++;

             } while ((ret = strtok(NULL, " ")));
             if(r_id != 0 && r2_id !=0){

                 finnes_rute(r_id, r2_id);
                
             }
            }

          else if (strcmp(ret, "legg_til_kobling")==0){
             int r_id;
             int r2_id;
            
             int index = 0;
             do
             {
                 if (index == 1)
                     r_id = atoi(ret);
                 if (index == 2)
                     r2_id = atoi(ret); 
                        

                 index++;

             } while ((ret = strtok(NULL, " ")));
             if(r_id != 0 && r2_id !=0){

                 legg_til_kobling(r_id, r2_id);
                
             }

         }

          else if (strcmp(ret, "slett_ruter")==0){
             int r_id;
             int index = 0;
            
             do
             {
                 if (index == 1)
                     r_id = atoi(ret);
                
                     index++;

             } while ((ret = strtok(NULL, " ")));
             if(r_id != 0){

                 slett_ruter(r_id);
                
             }

         }
         free(str);
        }

}

int main(int argc, char const *argv[]){
    
       if (argc < 3) {
	   	fprintf(stderr, "USAGE: %s <topology.dat> <commands.txt>\n", argv[0]);
	   	return 1;
	   }
    
    const char *load_file_name = argv[1];
    read_file(load_file_name, &main_struct_arr_size);
    

      FILE *fd = fopen(argv[2], "r");

        if (fd == NULL) {
            perror("fopen failed");
            return 1;
        }

    do_the_commands(fd); 

    write_to_file();

    free_all();
    fclose(fd); 
    
    return EXIT_SUCCESS;
}
