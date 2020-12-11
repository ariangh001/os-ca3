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
typedef std::vector<float> Features;
typedef std::vector<float> Extermums;
typedef std::vector<std::vector<float>> Dataset;

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

Dataset createDataset(Records records)
{
    Dataset dataset;
    for (int i=0; i<records.size(); i++)
        dataset.push_back(changeType(records[i]));
    return dataset;
}

Extermums findMax(Dataset data)
{
    Extermums max_values;
    for (int i=0; i<data[0].size() - 1; i++)
    {
        float max = -1;
        for (int j=0; j<data.size(); j++)
            if(max < data[j][i])
                max = data[j][i];
        max_values.push_back(max);
    }
    return max_values;
}

Extermums findMin(Dataset data)
{
    Extermums min_values;
    for (int i=0; i<data[0].size() -1; i++)
    {
        float min = data[0][i];
        for (int j=0; j<data.size(); j++)
            if(min > data[j][i])
                min = data[j][i];
        min_values.push_back(min);
    }
    return min_values;
}

Dataset normalize(Dataset data)
{
    Extermums min_values = findMin(data);
    Extermums max_values = findMax(data);

    for (int i=0; i<data.size(); i++)
        for (int j=0; j<data[i].size() - 1; j++)
            data[i][j] = (float)(data[i][j] - min_values[j]) / (float)(max_values[j] - min_values[j]);
    return data;
}

int main(int argc , char* argv[])
{
    if(checkInput(argc) < 0)
        return -1;

    string main_dir(argv[1]);
    string dataset_dir = main_dir;
    dataset_dir.append(DATASET_FILE);
    string weights_dir = main_dir;
    weights_dir.append(WEIGHT_FILE);

    Records phones = readCSV(dataset_dir);
    Records weights = readCSV(weights_dir);

    Dataset phones_dataset = createDataset(phones);
    Dataset weights_dataset = createDataset(weights);

    phones_dataset = normalize(phones_dataset);

    return 0;
}