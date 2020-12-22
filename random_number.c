#include <math.h>
#include <stdio.h>
#include <time.h>
#include <stdlib.h>

// return random number between 0 and 1
double randomDouble(){
    //srand((double)time(0));
    double scale = (double) rand() / (double) RAND_MAX;
    return scale;
}

// exponential distribution
double exponential(){
    double rnd = randomDouble();
    if(rnd == 0) return 1000000;
    else {
        return -log(rnd);
    }
}

// uniform real number distribution between 0 and x
double uniform(double x){
    return 2 * randomDouble();
}

// biased real number distribution between x and y
double biased(double x, double y){
    return x + (y-x) * randomDouble();
}

// bimodal distribution
double bimodal(){
    double rnd = randomDouble();
    return 0.95238 * rnd + rnd<0.1? 9.5238 : 0;
}

// triangular distribution between 0 to x
double triangular(double x){
    double rnd = randomDouble();
    return x * sqrt(rnd);
}

int main(){
    FILE *f;
    srand(time(NULL));
    f = fopen("resource/random_number/triangular.txt", "w");
    for(int i=0; i<1000000; i++){
        double r = triangular(1.5);
        printf("%lf\n", r);
        fprintf(f, "%lf\n", r);
    }

    f = fopen("resource/random_number/bimodal.txt", "w");
    for(int i=0; i<1000000; i++){
        double r = bimodal();
        printf("%lf\n", r);
        fprintf(f, "%lf\n", r);
    }

    f = fopen("resource/random_number/uniform.txt", "w");
    for(int i=0; i<1000000; i++){
        double r = uniform(2.0);
        printf("%lf\n", r);
        fprintf(f, "%lf\n", r);
    }

    f = fopen("resource/random_number/exponential.txt", "w");
    for(int i=0; i<1000000; i++){
        double r = exponential();
        printf("%lf\n", r);
        fprintf(f, "%lf\n", r);
    }

    f = fopen("resource/random_number/biased.txt", "w");
    for(int i=0; i<1000000; i++){
        double r = biased(0.9, 1.1);
        printf("%lf\n", r);
        fprintf(f, "%lf\n", r);
    }

    f = fopen("resource/random_variate/variate.txt", "w");
    for(int i=0; i<1000000; i++){
        double r = randomDouble();
        printf("%lf\n", r);
        fprintf(f, "%lf\n", r);
    }
    fclose(f);
}
