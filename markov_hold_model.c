#include<stdio.h>
#include<stdlib.h>
#include "timing.c"
#include "OptimizationSplay.c"
#include <locale.h>
#include <math.h>

double number[1000000];
void getRandomNumber(){
    FILE *fp = fopen("resource/random_number/uniform.txt", "r");
    int index = 0;
    double buff;
    while (!feof (fp)) {
        fscanf(fp, "%lf", &buff);
        number[index] = buff;
        index++;
    }
    fclose(fp);
}

double variate[1000000];
void getRandomVariate(){
    FILE *fp = fopen("resource/random_variate/variate.txt", "r");
    int index = 0;
    double buff;
    while (!feof (fp)) {
        fscanf(fp, "%lf", &buff);
        variate[index] = buff;
        index++;
    }
    fclose(fp);
}

void printFlie(FILE *f, double wc1, double wc2, long count, ProcStatm proc_statm, long page_size){
    fprintf(f,"%f  %ld\n",(wc2 - wc1)*1000, count);

    double time = wc2 - wc1;
    ProcStat_init(&proc_statm);
    double total_vm = ((double)proc_statm.size * page_size) / (1024 * 1024);
    fprintf(f,"/proc/self/statm size resident %f MiB, page_size %ld\n",
                total_vm, page_size
            );
    double minutes = time / 60;
    if(time < 1)
	time = time * 1000;
    double _badness = total_vm / (int_sqrt((long)time) * //pow(minutes, 1.0/4)
                        sqrt(sqrt(minutes))
                        );
    fprintf(f,"Badness bd = %f\n", _badness);
}

/*MAIN*/
int main(int argc, char** argv){
    /*init priority queue*/
    //initqueue();
    getRandomNumber();
    getRandomVariate();
    FILE *f = fopen("result/markov_hold_model/result.txt","a");
    /*end init priority queue*/

    /*init variable*/
    double wc1 = 0, wc2 = 0, cpuT = 0;
    int defaultBias = 1;
    ProcStatm proc_statm;
    long page_size = sysconf(_SC_PAGESIZE);

    if(argc >= 2)
    {
        if(argc >= 3)
            defaultBias = atoi(argv[2]);
    }

    int first = -1;
    unsigned long arr[20250][7];//20250 = 3*(k*k*k/4) as k = 30
    int root = -1;

    long count = 0, n = 1;
    long index = 0;
    double current = 0;
    setlocale(LC_NUMERIC, "");

    unsigned long mem = mem_avail();
    printf("Free memory Available = %'ld\n", mem / (1024*1024));
    printf("Start......\n");

    int state = 0; // delete
    int done = 0; // run program
    int variateIndex = 0;
    double anpha = 0.5;
    double beta = 0.5;

    /*START*/
    timing(&wc1, &cpuT);

    // begin insert 1000 event
    for(int i=0; i<10000; i++){
        //enqueue(new_node(A,0,0,0));
        enqueue(A, 0, 0, 0, &root, arr);
    }
    //node* del = dequeue(); current = del->endTime;
    dequeue(&first, &root, arr); current = first;
    while(!done){
        if(state == 0){
            if(/*qsize == 0*/root == -1){ // calendar queue is empty
                printf("error!");
                return 0;
            }

            double randomU = variate[variateIndex]; // get U(0,1) random variate u
            variateIndex++; if(variateIndex == 1000000) variateIndex = 0;

            if(randomU < anpha){
                //del = dequeue(); current = del->endTime;
                dequeue(&first, &root, arr); current = first;
                state = 0;
                count++;
            } else {
                enqueue(A, 0, 0, current + number[index], &root, arr);
                //node* new_n = new_node(A, 0, 0, current + number[index]);
                index++; if(index == 1000000) index = 0;
                //enqueue(new_n);
                state = 1;
                count++;
            }
        } else {
            double randomU = variate[variateIndex]; // get U(0,1) random variate u
            variateIndex++; if(variateIndex == 1000000) variateIndex = 0;

            if(randomU < 1 - beta){
                //del = dequeue(); current = del->endTime;
                dequeue(&first, &root, arr); current = first;
                state = 0;
                count++;
            } else {
                enqueue(A, 0, 0, current + number[index], &root, arr);
                //node* new_n = new_node(A, 0, 0, current + number[index]);
                index++; if(index == 1000000) index = 0;
                //enqueue(new_n);
                state = 1;
                count++;
            }
        }

        if(count == n) done = 1;
    }


    timing(&wc2, &cpuT);
    /*END*/

    printf("Time: %'f ms with count = %'ld\n", (wc2 - wc1)*1000, count);
    printf("================================\n");
    badness(wc2 - wc1, page_size, proc_statm);
    printFlie(f,wc1,wc2,count, proc_statm,page_size);

    return 0;
}

