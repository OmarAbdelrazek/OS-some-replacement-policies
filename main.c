#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <ctype.h>
#define EMPTY -1

typedef struct vm
{
    int numOfPages;
    int *numbers;
    int totalNumbersCount;
    char policy[5];
    int* frame;
} vm;
typedef struct map{
    int* key;
    int *value;
}map;


void fifoPolicy(vm v);
void clockPolicy(vm v);
void lruPolicy(vm v);
void optimalPolicy(vm v);
int getVictim(vm v,map valid,int page,int i);
int getMinimum(vm v, map map);
void iniUsedPos(int usedPosition[]);
void iniMemory(vm v);
int getIndex(vm v,int num);
int changeWithLowestFreq(vm v,int i,map map);
void printMemoryContent(vm v,int input,int foundFlag);
int searchInMemory(vm v,int target);
int getKey(map map,vm v,int page);
void readFromFile(vm v);

void fifoPolicy(vm v){

 printf("Replacement Policy = FIFO\n");
    printf("-------------------------------------\n");
    printf("Page   Content of Frames\n");
    printf("----   -----------------\n");
    int page;
    int faultCounter = 0;
    int found = 0;
    int turn = 0;
    int i=0;
    page = v.numbers[i];
    while(page != -1)
    {
        found = searchInMemory(v,page);
        if(found != -1)
        {
            printMemoryContent(v,page,1);
        }
        else
        {
            if(turn < v.numOfPages && v.frame[v.numOfPages-1] == EMPTY)
            {
                v.frame[turn] = page;
                printMemoryContent(v,page,1);
                turn++;
            }
            else
            {
                faultCounter++;
                v.frame[turn] = page;
                printMemoryContent(v,page,0);
                //printf("%d",faultCounter);
                turn++;
            }
        }

        // printMemoryContent(v);
        i++;
        page = v.numbers[i];
    }
    printf("-------------------------------------\n");
    printf("Number of page faults = %d\n",faultCounter);
}

void clockPolicy(vm v){
printf("Replacement Policy = CLOCK\n");
    printf("-------------------------------------\n");
    printf("Page   Content of Frames\n");
    printf("----   -----------------\n");
    int page;
    map valid;
    valid.key = malloc(sizeof(v.totalNumbersCount));
    valid.value = malloc(sizeof(v.totalNumbersCount));
    int faultCounter = 0;
    int found = 0;
    int turn = 0;
    int i=0;
    int x = 0;
    page = v.numbers[i];
    while(page != -1)
    {
        valid.key[i] = page;
        found = searchInMemory(v,page);
        if(found != -1)
        {
            valid.value[found] = 1;
            printMemoryContent(v,page,1);
                        //    printf("\tbits: %d,Turn= %d,Found= %d\n",valid.value[turn],turn,found);
            //printf("bits: %d\n",valid.value[found]);


        }
        else
        {

             if(turn < v.numOfPages && v.frame[v.numOfPages-1] == EMPTY)
            {
                v.frame[turn] = page;
                valid.value[turn] = 0;
                printMemoryContent(v,page,1);
               // printf("\tbits: %d,Turn= %d\n",valid.value[turn],turn);
                turn++;

            }
            else{
            faultCounter++;
            int check = checkIfAllOnes(v,valid);
            //int turn = getVictim(v,valid,page,turn);
            if(check == 1){
                v.frame[turn] = page;
                turn++;

            }
            while(valid.value[turn % v.numOfPages] != 0){
                if(check = 1 && (turn%v.numOfPages) == turn)
                    break;
                valid.value[turn % v.numOfPages] = 0;
            turn++;
            }
            if(check == 0){
            v.frame[turn % v.numOfPages] = page;
                turn++;
            }
            printMemoryContent(v,page,0);
                          //  printf("\tbits: %d,Turn= %d\n",valid.value[turn],turn);
            valid.value[turn % v.numOfPages] = 1;
            //turn++;



            }



        }
         if(turn >= v.numOfPages)
        {
            turn = 0;
        }

        i++;
        page = v.numbers[i];



}
    printf("-------------------------------------\n");
    printf("Number of page faults = %d\n",faultCounter);
}

int checkIfAllOnes(vm v,map valid){
     for(int i=0 ;i< v.numOfPages ; i++){
        if(valid.value[i] == 0)
            return 0;
     }
     return 1;

}

void lruPolicy(vm v){
printf("Replacement Policy = LRU\n");
    printf("-------------------------------------\n");
    printf("Page   Content of Frames\n");
    printf("----   -----------------\n");
    int page;
    map usedCounter;
    usedCounter.key = malloc(v.numOfPages*sizeof(int));
    usedCounter.value = malloc(v.numOfPages*sizeof(int));
    int faultCounter = 0;
    int found = 0;
    int freq = 0;
    int turn = 0;
     int i=0;
    page = v.numbers[i];
    while(page != -1)
    {
    found = searchInMemory(v,page);
        if(found != -1){
            printMemoryContent(v,page,1);
            //int place = getKey(usedCounter,v,page);
            usedCounter.value[found] = freq;
            freq++;
        }
        else{
            if(turn < v.numOfPages && v.frame[v.numOfPages-1] == EMPTY){
                v.frame[turn] = page;
                usedCounter.value[turn] = freq;
                freq++;
                turn++;
                printMemoryContent(v,page,1);

            }
            else{
                int lru = getMinimum(v,usedCounter);
                v.frame[lru] = page;
                usedCounter.value[lru] = freq;
                freq++;
                faultCounter++;
                printMemoryContent(v,page,0);


            }

        }
         if(turn >= v.numOfPages)
        {
            turn = 0;
        }
        i++;
        page = v.numbers[i];


    }
    printf("-------------------------------------\n");
    printf("Number of page faults = %d\n",faultCounter);


}


void optimalPolicy(vm v){
  printf("\nReplacement Policy = OPTIMAL\n");
    printf("------------------------\n");
    printf("Page\tContent of Frames\n");
    printf("------------------------\n");
    int page;
    map longestRef;
    longestRef.key = malloc(v.numOfPages*sizeof(int));
    longestRef.value = malloc(v.numOfPages*sizeof(int));
    int faultCounter = 0;
    int found = 0;
    int freq = 0;
    int turn = 0;
     int i=0;
    page = v.numbers[i];
    while(page != -1)
    {
    found = searchInMemory(v,page);
        if(found != -1){
            printMemoryContent(v,page,1);
        }
        else{
            if(turn < v.numOfPages && v.frame[v.numOfPages-1] == EMPTY){
                v.frame[turn] = page;
                turn++;
                printMemoryContent(v,page,1);

            }
            else{
                int minimum = changeWithLowestFreq(v,i,longestRef);
                v.frame[minimum] = page;
                faultCounter++;
                printMemoryContent(v,page,0);


            }

        }
         if(turn >= v.numOfPages)
        {
            turn = 0;
        }
        i++;
        page = v.numbers[i];
    }
    printf("-------------------------------------\n");
    printf("Number of page faults = %d\n",faultCounter);

}


int changeWithLowestFreq(vm v,int i,map map){
    //int lowest = v.numbers[i];
    int freq[v.numOfPages];
    int j = 0;
    for(int e = 0 ; e<v.numOfPages ; e++){
        map.key[e] = v.frame[e];
        map.value[e] = i ;
        for(int k = i ; k< v.totalNumbersCount ; k++){
            if(v.frame[e] == v.numbers[k]){
            map.value[e] = k;

            break;

            }

            else if(v.frame[e] != v.numbers[k]){
                map.value[e] = v.totalNumbersCount+1;


            }

        }


    }
        int longest = -10000000, maxIndex;
    for(int i = 0; i< v.numOfPages; i++)
    {
        if(map.value[i] > longest)
        {
            longest = map.value[i];
            maxIndex = i;
        }
    }


    return maxIndex;
}



int getMinimum(vm v, map map){
              int min = 10000000, minIndex;
    for(int i = 0; i< v.numOfPages; i++)
    {
        if(map.value[i] < min)
        {
            min = map.value[i];
            minIndex = i;
        }
    }
    return minIndex;
}

int getKey(map map,vm v,int page){

    for(int i=0 ; i< v.numOfPages ; i++){
        if(map.key[i] == page){
            return i;
        }
    }
    return -1;

}



int searchInMemory(vm v,int target){

    for(int i=0 ; i< v.numOfPages ; i++)
    {
        if(v.frame[i] == target)
            return i;
    }
    return -1;
}

void iniMemory(vm v){

    for(int i=0; i< v.numOfPages ; i++)
    {
        v.frame[i] = EMPTY;
        ///printf("%d",v.frame[i]);
    }
}

void printMemoryContent(vm v,int input,int foundFlag){


    if(foundFlag ==1)
    {
        printf("%02d     ",input);
    }
    else
    {
        printf("%02d F   ",input);
    }
    for(int i=0 ; i< v.numOfPages ; i++)
    {
        if(v.frame[i] == -1)
        {
            continue;
        }
        else
        {
            printf("%02d ",v.frame[i]);
        }

    }
    printf("\n");

}

/*void readFromFile(vm v){
        char fileName[256];
        int numOfPlaces;
        printf("Enter file name: ");
        scanf("%s", fileName);
        //printf("%s", fileName);
        FILE* file = fopen (fileName, "r");
          int i = 0;
          int j = 0;


          fscanf (file, "%d\n", &v.numOfPages);
                  printf("%d\n", v.numOfPages);

          fscanf (file, "%s", v.policy);
                  printf("%s", v.policy);

          v.frame = malloc(v.numOfPages * sizeof(int));
          fscanf (file, "%d\n",&i);
          while (!feof (file))
        {
                v.numbers[j] = i;
                printf ("%d\n ", v.numbers[j]);

                j++;
              fscanf (file, "%d\n",&i);

            }

          fclose (file);
}
*/

int main()
{

    vm v;
    int in;
    int i= 0;

    /**printf("1.Read input from file: \n2.Enter input\n");
    scanf("%d",&in);
    if(in == 1){

        readFromFile(v);
    }**/
    scanf("%d",&v.numOfPages);
    scanf("%s", v.policy);
    int size = 1;
    v.frame = malloc(v.numOfPages * sizeof(int));
    v.numbers = malloc(size*sizeof(int));
    v.totalNumbersCount = 0;
     scanf("%d",&v.numbers[i]);
    while(v.numbers[i] != -1){
        i++;
        size++;
        v.totalNumbersCount++;
        v.numbers =  realloc(v.numbers,size*sizeof(int));
        scanf("%d",&v.numbers[i]);
    }
    iniMemory(v);
    if(strcmp(v.policy,"fifo") == 0 || strcmp(v.policy,"FIFO") == 0)
    {
        fifoPolicy(v);
    }
    else if(strcmp(v.policy,"lru") == 0 || strcmp(v.policy,"LRU") == 0)
    {
        lruPolicy(v);
    }
    else if(strcmp(v.policy,"optimal") == 0 || strcmp(v.policy,"OPTIMAL") == 0)
    {
        optimalPolicy(v);
    }
    else if(strcmp(v.policy,"clock") == 0 || strcmp(v.policy,"CLOCK") == 0)
    {
        clockPolicy(v);
    }
    return 0;
}
