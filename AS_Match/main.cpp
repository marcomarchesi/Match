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

static const int factor = 1;
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
    int index = 1;
    //for now, just focus on 5 points
    while(index < 6)
    {
        for(int i = 0; i < edge_size * edge_size;++i)
        {
            if(arr[i] == index)
            {
                int x = i / edge_size;
                int y = i % edge_size;
                vec.push_back(KeyPoint(x,y));
                index++;
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
    init(ka);
    
    populate_array(sa, a);
    populate_array(ka, b);
    
    
    return 0;
};

int main(int argc, const char * argv[]) {
    
    vector<KeyPoint> s_vec;
    vector<KeyPoint> k_vec;
    int s_array[edge_size * edge_size];
    int k_array[edge_size * edge_size];
    int t_array[edge_size * edge_size];
    
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
//    printf("S points\n");
//    print(s_array);
    
    Copy(s_array,k_array);
    //rotate
    float yaw = ToRadians(30.0f);
    printf("K points - We know are rotated by %.2f\n",yaw);
    Rotate(k_array,yaw);
    print(k_array);
    printf("Let's verify\n");
    // getting ready for matching algorithm
    populate_vector(k_array, k_vec);
    init(t_array);
    populate_array(t_array, k_vec);
    print(t_array);

    return 0;
}
