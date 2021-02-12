#include <stdio.h>
#include "cs1300bmp.h"
#include <iostream>
#include <fstream>
#include <stdlib.h>
#include "Filter.h"

using namespace std;

#include "rdtsc.h"

//
// Forward declare the functions
//
Filter * readFilter(string filename);
double applyFilter(Filter *filter, cs1300bmp *input, cs1300bmp *output);

int
main(int argc, char **argv)
{

  if ( argc < 2) {
    fprintf(stderr,"Usage: %s filter inputfile1 inputfile2 .... \n", argv[0]);
  }

  //
  // Convert to C++ strings to simplify manipulation
  //
  string filtername = argv[1];

  //
  // remove any ".filter" in the filtername
  //
  string filterOutputName = filtername;
  string::size_type loc = filterOutputName.find(".filter");
  if (loc != string::npos) {
    //
    // Remove the ".filter" name, which should occur on all the provided filters
    //
    filterOutputName = filtername.substr(0, loc);
  }

  Filter *filter = readFilter(filtername);

  double sum = 0.0;
  int samples = 0;

  for (int inNum = 2; inNum < argc; inNum++) {
    string inputFilename = argv[inNum];
    string outputFilename = "filtered-" + filterOutputName + "-" + inputFilename;
    struct cs1300bmp *input = new struct cs1300bmp;
    struct cs1300bmp *output = new struct cs1300bmp;
    int ok = cs1300bmp_readfile( (char *) inputFilename.c_str(), input);

    if ( ok ) {
      double sample = applyFilter(filter, input, output);
      sum += sample;
      samples++;
      cs1300bmp_writefile((char *) outputFilename.c_str(), output);
    }
    delete input;
    delete output;
  }
  fprintf(stdout, "Average cycles per sample is %f\n", sum / samples);

}

class Filter *
readFilter(string filename)
{
  ifstream input(filename.c_str());

  if ( ! input.bad() ) {
    int size = 0;
    input >> size;
    Filter *filter = new Filter(size);
    int div;
    input >> div;
    filter -> setDivisor(div);
    for (int i=0; i < size; i++) {
      for (int j=0; j < size; j++) {
	int value;
	input >> value;
	filter -> set(i,j,value);
      }
    }
    return filter;
  } else {
    cerr << "Bad input in readFilter:" << filename << endl;
    exit(-1);
  }
}


double
applyFilter(class Filter *filter, cs1300bmp *input, cs1300bmp *output)
{
    long long cycStart, cycStop;
    cycStart = rdtscll();

    output -> width = input -> width;
    output -> height = input -> height;

    int sumColor, plane, row, col; //removing calculations within loop and uneeded memory references
    int filterDiv = filter -> getDivisor();
    int inputW = (input -> width) - 1;
    int inputH = (input -> height) - 1; 
    int filterArr[8];  //localizing filter and removing function calls within loop as well as inner two loops
    filterArr[0] = filter -> get(0, 0);
    filterArr[1] = filter -> get(1, 0);
    filterArr[2] = filter -> get(2, 0);
    filterArr[3] = filter -> get(0, 1);
    filterArr[4] = filter -> get(1, 1);
    filterArr[5] = filter -> get(2, 1);
    filterArr[6] = filter -> get(0, 2);
    filterArr[7] = filter -> get(1, 2);
    filterArr[8] = filter -> get(2, 2);
    for(plane = 0; plane < 3; plane++)//to help locality switch the order of the outer 3 loops to follow C's row major ordering
    {
        for(row = 1; row < inputH; row++) 
        {
            for(col = 1; col < inputW; col++) 
            {
                sumColor = 0;
                //accumulation in temporary
                sumColor += ((input -> color[plane][row - 1][col - 1]) * filterArr[0]);
                sumColor += ((input -> color[plane][row][col - 1]) * filterArr[1]);
                sumColor += ((input -> color[plane][row + 1][col - 1]) * filterArr[2]);
                sumColor += ((input -> color[plane][row - 1][col]) * filterArr[3]);
                sumColor += ((input -> color[plane][row][col]) * filterArr[4]);
                sumColor += ((input -> color[plane][row + 1][col]) * filterArr[5]);
                sumColor += ((input -> color[plane][row - 1][col + 1]) * filterArr[6]);
                sumColor += ((input -> color[plane][row][col + 1]) * filterArr[7]);
                sumColor += ((input -> color[plane][row + 1][col + 1]) * filterArr[8]);
                
                sumColor = sumColor / filterDiv;
                if ( sumColor  < 0 ) 
                {
                  sumColor = 0;
                }
                if ( sumColor  > 255 ) 
                {
                  sumColor = 255;
                }
                output -> color[plane][row][col] = sumColor;
            }
        }
    }
    
    cycStop = rdtscll();
    double diff = cycStop - cycStart;
    double diffPerPixel = diff / (output -> width * output -> height);
    fprintf(stderr, "Took %f cycles to process, or %f cycles per pixel\n",
      diff, diff / (output -> width * output -> height));
    return diffPerPixel;
}
