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

#include "stuff.h"

#define ToRadians(x) {x * (M_PI / 180)}

using namespace std;


int main(int argc, const char * argv[]) {
    
    vector<KeyPoint3f> s_pts;
    vector<KeyPoint3f> k_pts;
    
    int s_array[kEdgeSize * kEdgeSize];
    int k_array[kEdgeSize * kEdgeSize];
    float rotations[3];
    float translations[3] = {0,0,0};
    
    // sample features
    s_pts.push_back(KeyPoint3f(3 * factor,2 * factor,0));
    s_pts.push_back(KeyPoint3f(3 * factor,6 * factor,0));
    s_pts.push_back(KeyPoint3f(7 * factor,2 * factor,0));
    s_pts.push_back(KeyPoint3f(7 * factor,6 * factor,0));
    s_pts.push_back(KeyPoint3f(5 * factor,4 * factor,0));
    init(s_array);
    populate_array(s_array, s_pts,false);
    Order(s_array);
    Copy(s_array,k_array);
    //rotate
    float roll = ToRadians(0.0f);
    float pitch = ToRadians(0.0f);
    float yaw = ToRadians(45.0f);
    rotations[1] = pitch;
    rotations[0] = roll;
    rotations[2] = yaw;
    Rotate(k_array,rotations);
    printTwo(s_array,k_array);
    
    
    
    printf("Let's match\n");
    // getting ready for matching algorithm
    populate_vector(k_array, k_pts);
    Match(s_pts, k_pts, rotations, translations);

    return 0;
}
