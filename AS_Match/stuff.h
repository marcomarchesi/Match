//
//  stuff.h
//  AS_Match
//
//  Created by Marco Marchesi on 11/6/15.
//  Copyright © 2015 Marco Marchesi. All rights reserved.
//

#ifndef stuff_h
#define stuff_h
#include <iostream>
#include <vector>
#include <array>
#include <cmath>
using namespace std;

static const int factor = 2;
static const int kEdgeSize = 10 * factor;
static const int kEmpty = - kEdgeSize;


class KeyPoint3f{
public:
    KeyPoint3f(float _x,float _y,float _z)
    {
        x = _x;
        y = _y;
        z = _z;
    };
    float x;
    float y;
    float z;
};

static KeyPoint3f center_keypoint = KeyPoint3f(0,0,0);

void init(int* arr);
void set(int* arr, int i,int j,int value);

void populate_array(int* arr, const vector<KeyPoint3f>& vec,bool offset);
void populate_vector(const int* arr,vector<KeyPoint3f>& vec);
void print(int* arr);
void printTwo(int* a,int* b);
void Rotate(int* arr,float* angles);
void Center(int* arr);
void Order(int* arr);
void Copy(int* src_arr,int* dst_arr);
int Match(const vector<KeyPoint3f>& a, const vector<KeyPoint3f>& b,const float* rotations,float* translations);

void init(int* arr)
{
    for(int i = 0;i < kEdgeSize; ++i)
    {
        for (int j = 0;j < kEdgeSize; ++j)
        {
            arr[i * kEdgeSize + j] = kEmpty;
        }
    }
}

void set(int* arr, int i,int j,int value)
{
    arr[i * kEdgeSize + j] = value;
}


void populate_array(int* arr, const vector<KeyPoint3f>& vec,bool offset)
{
    int index = 1;
    for (int i = 0; i < vec.size(); ++i)
    {
        if(offset == true)
            set(arr,vec[i].x + center_keypoint.x,vec[i].y + center_keypoint.y ,index++);
        else
            set(arr,vec[i].x,vec[i].y,index++);
    }
}


void populate_vector(const int* arr,vector<KeyPoint3f>& vec)
{
    //for now, just focus on 5 points
    for(int k = 1; k < 6;++k)
    {
        for(int i = 0; i < kEdgeSize * kEdgeSize;++i)
        {
            if(arr[i] == k)
            {
                int x = i / kEdgeSize;
                int y = i % kEdgeSize;
                vec.push_back(KeyPoint3f(x - center_keypoint.x,y - center_keypoint.y,0));
            }
        }
    }
}


void print(int* arr)
{
    for(int i = 0;i < kEdgeSize; ++i)
    {
        for (int j = 0;j < kEdgeSize; ++j)
        {
            if(arr[i * kEdgeSize + j] == kEmpty)
                printf(".");
            else
                printf("%i",arr[i * kEdgeSize + j]);
        }
        printf("\n");
    }
}

void printTwo(int* a,int* b)
{
    for(int i = 0;i < kEdgeSize; ++i)
    {
        for (int j = 0;j < kEdgeSize; ++j)
        {
            if(a[i * kEdgeSize + j] == kEmpty)
                printf(".");
            else
                printf("%i",a[i * kEdgeSize + j]);
        }
        printf("      ");
        for (int j = 0;j < kEdgeSize; ++j)
        {
            if(b[i * kEdgeSize + j] == kEmpty)
                printf(".");
            else
                printf("%i",b[i * kEdgeSize + j]);
        }
        printf("\n");
    }
}

KeyPoint3f x_rotate_vector(KeyPoint3f k_pt, float angle)
{
    KeyPoint3f rot_pt = KeyPoint3f(0,0,0);
    
    // Rotate X
    rot_pt.x = k_pt.x;
    rot_pt.y = k_pt.y * cos(angle); // - k_pt.z * sin(angles[2])
    rot_pt.z = 0;
    
    return rot_pt;
}

KeyPoint3f y_rotate_vector(KeyPoint3f k_pt, float angle)
{
    KeyPoint3f rot_pt = KeyPoint3f(0,0,0);
    // Rotate Y
    rot_pt.x = k_pt.x * cos(angle); // + k_pt.z * sin(angles[1])
    rot_pt.y = k_pt.y;
    rot_pt.z = 0; // + k_pt.z * cos(angles[1])
    
    return rot_pt;
}

KeyPoint3f z_rotate_vector(KeyPoint3f k_pt, float angle)
{
    KeyPoint3f rot_pt = KeyPoint3f(0,0,0);
    
    // Rotate Z
    rot_pt.x = k_pt.x * cos(angle) - k_pt.y * sin(angle);
    rot_pt.y = k_pt.x * sin(angle) + k_pt.y * cos(angle);
    rot_pt.z = 0;
    
    return rot_pt;
}

void Rotate(int* arr,float* angles)
{
    
    //get center
    Center(arr);
    
    vector<KeyPoint3f> rotated_vec;
    
    for(int i = 0;i < kEdgeSize; ++i)
    {
        for (int j = 0;j < kEdgeSize; ++j)
        {
            if(arr[i * kEdgeSize + j] != kEmpty){
                KeyPoint3f pt = z_rotate_vector(KeyPoint3f(i - center_keypoint.x,j -center_keypoint.y,0), angles[2]);
                pt = y_rotate_vector(pt, angles[1]);
                pt = x_rotate_vector(pt, angles[0]);
                rotated_vec.push_back(pt);
            }
        }
    }
    init(arr);
    populate_array(arr,rotated_vec,true);
}

void RotateBack(int* arr,float* angles)
{
    
    //get center
    Center(arr);
    
    vector<KeyPoint3f> rotated_vec;
    
    for(int i = 0;i < kEdgeSize; ++i)
    {
        for (int j = 0;j < kEdgeSize; ++j)
        {
            if(arr[i * kEdgeSize + j] != kEmpty){
                KeyPoint3f pt = x_rotate_vector(KeyPoint3f(i - center_keypoint.x,j -center_keypoint.y,0), angles[0]);
                pt = y_rotate_vector(pt, angles[1]);
                pt = z_rotate_vector(pt, angles[2]);
                rotated_vec.push_back(pt);
            }
        }
    }
    init(arr);
    populate_array(arr,rotated_vec,true);
}


void Center(int* arr)
{
    for (int i = 0; i < kEdgeSize * kEdgeSize;++i)
    {
        if(arr[i] == 3)
        {
            int x = i / kEdgeSize;
            int y = i % kEdgeSize;
            center_keypoint = KeyPoint3f(x,y,0);
        }
    }
}

void Order(int* arr)
{
    int index = 1;
    for(int i = 0;i < kEdgeSize; ++i)
    {
        for (int j = 0;j < kEdgeSize; ++j)
        {
            if(arr[i * kEdgeSize + j] != kEmpty){
                arr[i * kEdgeSize + j] = index++;
            }
        }
    }
}

void Copy(int* src_arr,int* dst_arr)
{
    for(int i = 0; i< kEdgeSize * kEdgeSize;++i)
    {
        dst_arr[i] = src_arr[i];
    }
}


int Match(const vector<KeyPoint3f>& a, const vector<KeyPoint3f>& b,const float* rotations,float* translations)
{
    int sa[kEdgeSize * kEdgeSize];
    int ka[kEdgeSize * kEdgeSize];
    
    // SAMPLE
    init(sa);
    populate_array(sa, a,false);
    Order(sa);
    
    
    // ROTATE BACK
    init(ka);
    populate_array(ka, b,true);
    float inverse_angles[3];
    inverse_angles[0] = - rotations[0];
    inverse_angles[1] = - rotations[1];
    inverse_angles[2] = - rotations[2];
    RotateBack(ka, inverse_angles);
    Order(ka);
    
    printTwo(sa,ka);
    
    // TRANSLATION MATCHING
    int max_ab = (int)max(a.size(), b.size());
    for (int i = 0;i < max_ab; ++i)
    {
        translations[0] += b[i].x - a[i].x + center_keypoint.x;
        translations[1] += b[i].y - a[i].y + center_keypoint.y;
        translations[2] += b[i].z - a[i].z;
    }
    translations[0] /= max_ab;
    translations[1] /= max_ab;
    translations[2] /= max_ab;
    
    printf("tx = %.2f and ty = %.2f\n",translations[0],translations[1]);
    
    return 0;
};


#endif /* stuff_h */
