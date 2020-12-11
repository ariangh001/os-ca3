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
#define INPUT_LEN 2
#define COMMA ','
#define DATASET_FILE "/train.csv"
#define WEIGHT_FILE "/weights.csv"

using namespace std;
typedef std::string Record;
typedef std::vector<std::string> Records;
typedef std::vector<std::string> Words;


int checkInput(int argc)
{
    if(argc != INPUT_LEN)
    {
        cout<< "Bad Input !! Try Again..." <<endl;
        return -1;
    }
    return 0;
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

int main(int argc , char* argv[])
{
    if(checkInput(argc) < 0)
        return -1;
    return 0;
}