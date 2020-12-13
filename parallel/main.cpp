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

Words parseString(string str, char delimiter)
{
    Words words;
    string tmp = "";
    for (auto x : str)
    {
        if(x == delimiter)
        {
            words.push_back(tmp);
            tmp = "";
        }
        else
        {
            tmp += x;
        }
    }
    words.push_back(tmp);
    return words;
}

Records readCSV(string filename)
{
    string line;
    Records records;
    ifstream dataframe(filename);
    if(dataframe.is_open())
    {
        if(dataframe.peek() == ifstream::traits_type::eof())
            return records;
        while(getline(dataframe,line))
            if(line != "")
                records.push_back(line);
        if(records.size() > 0)
            records.erase(records.begin());
        dataframe.close();
    }
    return records;
}

Features changeType(Record record)
{
    Features features;
    Words words = parseString(record,COMMA);
    for (int j=0; j<words.size(); j++)
        features.push_back(stof(words[j]));
    return features;
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
