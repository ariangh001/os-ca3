#include <stdio.h>
#include <iostream>
#include <unistd.h>
#include <fcntl.h>
#include <string>
#include <cstring>
#include <vector>
#include <dirent.h>
#include <fstream>
#include <iomanip>
#include <pthread.h>
#include <time.h>

#define NUMBER_OF_THREADS 4
#define INPUT_LEN 2
#define COMMA ','
#define DATASET_FILE "/train.csv"
#define WEIGHT_FILE "/weights.csv"

using namespace std;
typedef std::string Record;
typedef std::vector<std::string> Records;
typedef std::vector<std::string> Words;
typedef std::vector<float> Features;
typedef std::vector<float> Extermums;
typedef std::vector<std::vector<float>> Dataset;
typedef std::vector<int> Predictions;

string dataset_dir;
string weights_dir;

int checkInput(int argc)
{
    if(argc != INPUT_LEN)
    {
        cout<< "Bad Input !! Try Again..." <<endl;
        return -1;
    }
    return 0;
}



int main(int argc , char* argv[])
{
    if(checkInput(argc) < 0)
        return -1;

    pthread_t threads[NUMBER_OF_THREADS];

    string main_dir(argv[1]);
    dataset_dir = main_dir;
    weights_dir = main_dir;
    weights_dir.append(WEIGHT_FILE);

    return 0;
}
