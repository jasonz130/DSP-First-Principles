#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

#include "FIR.h"

int main(){   
    
    int n = 1000;
    float x[n];
    float y[n];

    const float pi = 3.14159;
    const float f = 2.5;
    srand(time(0));
    for(int i = 0; i < n; i++){   
        float error = 0.3f;
        float noise = ((float)rand() / (float)RAND_MAX) * (error);
        x[i] = (float) sin(2*pi * f * i/n) + noise;
    }

    FIRFilter FIR_lowpass;

    FIRFilter_init(&FIR_lowpass);

    for(int i = 0; i < n; i++){
        FIRFilter_calc(&FIR_lowpass, x[i]);
        y[i] = FIR_lowpass.out;
    }
    
    FILE *gnuplot;
    char gnuPlotCommandString[500] = "";
    char title[200] = "";
    char xLabel[200] = "";
    char yLabel[200] = "";

    const int NUM_TERMS = 5;

    int termNo = 0;
    for(termNo = 0; termNo < NUM_TERMS; termNo++){
        sprintf(gnuPlotCommandString, "gnuplot -e \"set term %s\" 2>1", "qt");
        gnuplot = popen(gnuPlotCommandString, "r");
        char tempc;
        tempc = fgetc(gnuplot);
        if (feof(gnuplot)){
            break;
        }
        if(tempc)
        fclose(gnuplot);
    }

    sprintf(gnuPlotCommandString, "gnuplot -persistent 2> /dev/null");
    if ((gnuplot = popen(gnuPlotCommandString, "w")) == NULL){
        char strError[100];
        sprintf(strError,"Failed to open gnuplot executable");
        printf("[ERROR]: %s\n", strError);
        exit(-1);
    }
    int orders = FIR_FILTER_LENGTH;
    sprintf(title, "FIR Low-pass Time-Domain Output of %d Orders", orders);

    fprintf(gnuplot, "set terminal %s size 500,400\n", "qt");
    fprintf(gnuplot, "set title '%s'\n", title);
    fprintf(gnuplot, "set xlabel '%s'\n", "Time [ms]");
    fprintf(gnuplot, "set ylabel '%s'\n", "Amplitude");

    //Plot the data
    fprintf(gnuplot,"plot '-' w lines lc rgb 'blue' title \"Input\", '-' w lines lc rgb 'red' title \"Filtered Output\" ");
    for(int i = 0; i < n; i++){
        fprintf(gnuplot, "%d %lf\n", i, x[i]);
    }
    fprintf(gnuplot,"e\n");
    for(int i = 0; i < n; i++){
        fprintf(gnuplot, "%d %lf\n", i, y[i]);
    }

    fflush(gnuplot);
    fclose(gnuplot);

}