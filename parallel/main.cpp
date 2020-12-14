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

Extermums minimums;
Extermums maximums;
Dataset train_set;
Dataset weights_dataset;
Records weights;

int summation = 0;
pthread_mutex_t mutex_sum;

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

float calcDot(Features features, Features weights)
{
    float result = 0;
    for (int i=0; i<features.size() - 1; i++)
        result += features[i] * weights[i];
    result += weights[weights.size()-1];
    return result;
}

int findPriceRange(Features features, Dataset weights_dataset)
{
    float result = 0;
    int range = 0;
    std::vector<float> prices;
    for (int i=0; i<weights_dataset.size(); i++)
        prices.push_back(calcDot(features,weights_dataset[i]));
    result = prices[0];
    for (int i=0; i<prices.size(); i++)
    {
        if(result < prices[i])
        {
            result = prices[i];
            range = i;
        }
    }
    return range;
}

Predictions classifyPhones(Dataset data,Dataset weights_dataset)
{
    Predictions predicts;
    for (int i=0; i<data.size(); i++)
        predicts.push_back(findPriceRange(data[i],weights_dataset));
    return predicts;
}

float calcAccuracy(Predictions predicts,Dataset data)
{
    int result = 0;
    for (int i=0; i<predicts.size(); i++)
        if (predicts[i] == data[i][data[i].size() - 1])
            result++;
    return ((float)result / (float)predicts.size()) * 100;
}

Extermums minLists(Extermums main, Extermums local)
{
    for (int i=0; i<main.size(); i++)
        if(main[i] > local[i])
            main[i] = local[i];
    return main;
}
Extermums maxLists(Extermums main, Extermums local)
{
    for (int i=0; i<main.size(); i++)
        if (main[i] < local[i])
            main[i] = local[i];
    return main;
}

void* preprocess(void* args)
{
    pthread_mutex_lock(&mutex_sum);
    long offset = (long)args;
    string train_dir = dataset_dir + "/train_" + to_string(offset) + ".csv";
    Records phones = readCSV(train_dir);
    Dataset phones_dataset = createDataset(phones);
    Extermums min_values = findMin(phones_dataset);
    Extermums max_values = findMax(phones_dataset);

    if(minimums.size() == 0)
        minimums = min_values;
    else
        minimums = minLists(minimums,min_values);

    if(maximums.size() == 0)
        maximums = max_values;
    else
        maximums = maxLists(maximums,max_values);

    for (int i=0; i<phones_dataset.size(); i++)
    {
        train_set.push_back(phones_dataset[i]);
    }

    pthread_mutex_unlock(&mutex_sum);
}

int main(int argc , char* argv[])
{
    if(checkInput(argc) < 0)
        return -1;

    pthread_t threads[NUMBER_OF_THREADS];
    //Serial
    string main_dir(argv[1]);
    dataset_dir = main_dir;
    weights_dir = main_dir;
    weights_dir.append(WEIGHT_FILE);
    weights = readCSV(weights_dir);
    weights_dataset = createDataset(weights);
    void* status;
    float result = 0;
    pthread_mutex_init(&mutex_sum, NULL);
    // Parallel
    for (int i=0; i<NUMBER_OF_THREADS; i++)
    {
        int return_code = pthread_create(&threads[i],
				NULL, preprocess, (void*)i);
        if (return_code)
		{
			cout<<"Error in Creating Thread "<<i<<endl;
			exit(-1);
		}
    }
    for(int i = 0; i < NUMBER_OF_THREADS; i++)
		pthread_join(threads[i], &status);
    //Serial
    train_set = normalize(train_set);
    Predictions prices = classifyPhones(train_set,weights_dataset);
    float a = calcAccuracy(prices,train_set);
    cout<<"Accuracy: "<<setprecision(4)<<a<<'%';

    return 0;
}
