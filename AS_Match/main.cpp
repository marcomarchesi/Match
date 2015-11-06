//
//  main.cpp
//  AS_Match
//
//  Created by Marco Marchesi on 11/5/15.
//  Copyright © 2015 Marco Marchesi. All rights reserved.
//

#include <iostream>
#include <vector>
#include <array>
#include <cmath>

#define ToRadians(x) {x * (M_PI / 180)}

using namespace std;

static const int factor = 2;
static const int edge_size = 10 * factor;


class KeyPoint{
public:
    KeyPoint(float _x,float _y)
    {
        x = _x;
        y = _y;
    };
    float x;
    float y;
};

void init(int* arr)
{
    for(int i = 0;i < edge_size; ++i)
    {
        for (int j = 0;j < edge_size; ++j)
        {
            arr[i * edge_size + j] = -1;
        }
    }
}

void set(int* arr, int i,int j,int value)
{
    arr[i * edge_size + j] = value;
}

void populate_array(int* arr, const vector<KeyPoint>& vec)
{
    int index = 1;
    for (int i = 0; i < vec.size(); ++i)
    {
        set(arr,vec[i].x,vec[i].y,index++);
    }
}

void populate_vector(const int* arr,vector<KeyPoint>& vec)
{
    //for now, just focus on 5 points
    for(int k = 1; k < 6;++k)
    {
        for(int i = 0; i < edge_size * edge_size;++i)
        {
            if(arr[i] == k)
            {
                int x = i / edge_size;
                int y = i % edge_size;
                vec.push_back(KeyPoint(x,y));
            }
        }
    }
}

void print(int* arr)
{
    for(int i = 0;i < edge_size; ++i)
    {
        for (int j = 0;j < edge_size; ++j)
        {
            if(arr[i * edge_size + j] == -1)
                printf(".");
            else
                printf("%i",arr[i * edge_size + j]);
        }
        printf("\n");
    }
}

void printTwo(int* a,int* b)
{
    for(int i = 0;i < edge_size; ++i)
    {
        for (int j = 0;j < edge_size; ++j)
        {
            if(a[i * edge_size + j] == -1)
                printf(".");
            else
                printf("%i",a[i * edge_size + j]);
        }
        printf("      ");
        for (int j = 0;j < edge_size; ++j)
        {
            if(b[i * edge_size + j] == -1)
                printf(".");
            else
                printf("%i",b[i * edge_size + j]);
        }
        printf("\n");
    }
}

void Rotate(int* arr,float angle)
{
    vector<KeyPoint> rotated_vec;
    
    for(int i = 0;i < edge_size; ++i)
    {
        for (int j = 0;j < edge_size; ++j)
        {
            if(arr[i * edge_size + j] > -1){
                rotated_vec.push_back(KeyPoint(i * cos(angle) - j * sin(angle),i * sin(angle) + j * cos(angle)));
            }
        }
    }
    init(arr);
    populate_array(arr,rotated_vec);
}

void Order(int* arr)
{
    int index = 1;
    for(int i = 0;i < edge_size; ++i)
    {
        for (int j = 0;j < edge_size; ++j)
        {
            if(arr[i * edge_size + j] > -1){
                arr[i * edge_size + j] = index++;
            }
        }
    }
}

void Copy(int* src_arr,int* dst_arr)
{
    for(int i = 0; i< edge_size * edge_size;++i)
    {
        dst_arr[i] = src_arr[i];
    }
}


int Match(const vector<KeyPoint>& a, const vector<KeyPoint>& b,const float* rotations,float* translations)
{
    int sa[edge_size * edge_size];
    int ka[edge_size * edge_size];
    
    init(sa);
    populate_array(sa, a);
    Order(sa);
//    print(sa);
    
//    printf("xxxxxxxxxxxxxxx\n");
    init(ka);
    populate_array(ka, b);
    Rotate(ka, -rotations[2]);
    Order(ka);
    
    printTwo(sa,ka);
    
    // TRANSLATION MATCHING
    int max_ab = (int)max(a.size(), b.size());
    for (int i = 0;i < max_ab; ++i)
    {
        translations[0] += b[i].x - a[i].x;
        translations[1] += b[i].y - a[i].y;
        translations[2] += 0;
    }
    translations[0] /= max_ab;
    translations[1] /= max_ab;
    
    printf("tx = %.2f and ty = %.2f\n",translations[0],translations[1]);
        
    return 0;
};

int main(int argc, const char * argv[]) {
    
    vector<KeyPoint> s_vec;
    vector<KeyPoint> k_vec;
    int s_array[edge_size * edge_size];
    int k_array[edge_size * edge_size];
    float rotations[3];
    float translations[3];
    
    // sample features
    s_vec.push_back(KeyPoint(3 * factor,2 * factor));
    s_vec.push_back(KeyPoint(3 * factor,6 * factor));
    s_vec.push_back(KeyPoint(7 * factor,2 * factor));
    s_vec.push_back(KeyPoint(7 * factor,6 * factor));
    s_vec.push_back(KeyPoint(5 * factor,4 * factor));
    init(s_array);
    populate_array(s_array, s_vec);
    Order(s_array);
    // output data
    
    Copy(s_array,k_array);
    //rotate
    float yaw = ToRadians(23.0f);
    rotations[2] = yaw;
    Rotate(k_array,yaw);
    printTwo(s_array,k_array);
    printf("Let's match\n");
    // getting ready for matching algorithm
    populate_vector(k_array, k_vec);
    Match(s_vec, k_vec, rotations, translations);

    return 0;
}
