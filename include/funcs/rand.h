#ifndef RAND_H
#define RAND_H

#include <random>
#include <cstring>

#include <cmath>
#include <vector>
#include <algorithm>
#include <climits>

#include "world/dnloop.h"

float randomf(const char* ngtype) {
    static std::random_device rd;
    
    // default generator
    static std::mt19937 mtGen32(rd());
    static std::mt19937_64 mtGen64(rd());
	static std::minstd_rand0 msGen0(rd());
	static std::minstd_rand msGen(rd());
	static std::ranlux24_base r24b(rd());
	static std::ranlux48_base r48b(rd());
	static std::ranlux24 r24(rd());
	static std::ranlux48 r48(rd());
	static std::knuth_b		kb(rd());
	static std::default_random_engine	dre(rd());

    std::uniform_real_distribution<float> dist(0.0f, 1.0f);

    if (strcmp(ngtype, "mt32") == 0){
        return dist(mtGen32);
    }
    else if(strcmp(ngtype, "mt64") == 0){
    	return dist(mtGen64);
    }
    else if(strcmp(ngtype, "ms0") == 0) {
        return dist(msGen0);
    }
    else if(strcmp(ngtype, "ms") == 0) {
        return dist(msGen);
    }
    else if(strcmp(ngtype, "r24b") == 0) {
        return dist(r24b);
    }
    else if(strcmp(ngtype, "r48b") == 0) {
        return dist(r48b);
    }
    else if(strcmp(ngtype, "r24") == 0) {
        return dist(r24);
    }
    else if(strcmp(ngtype, "r48") == 0) {
        return dist(r48);
    }
    else if(strcmp(ngtype, "kb") == 0) {
        return dist(kb);
    }
    else if(strcmp(ngtype, "dre") == 0) {
        return dist(dre);
    }
    else if(strcmp(ngtype, "rn") == 0) {
        return dist(rd);
    }
    else{
        return dist(mtGen32); // fallback
    }
}

float fade(float t) {
    return t * t * t * (t * (t * 6 - 15) + 10);
}

float grad(int hash, float x, float y) {
    switch (hash & 3) {
        case 0: return  x + y;
        case 1: return -x + y;
        case 2: return  x - y;
        case 3: return -x - y;
        default: return 0; // will never happen
    }
}

std::vector<int> p(512);

void initPerlin(unsigned int seed = 0) {
    static std::vector<int> permutation(256);
    for (int i = 0; i < 256; i++) permutation[i] = i;

    // shuffle using LCG, deterministic
    for (int i = 255; i > 0; i--) {
        unsigned int j = (seed + i) % (i + 1);
        std::swap(permutation[i], permutation[j]);
        seed = (1664525 * seed + 1013904223);
    }

    for (int i = 0; i < 256; i++) {
        p[i] = permutation[i];
        p[256 + i] = permutation[i];
    }
}

int randomi(const char* ngtype) {
    static std::random_device rd;
    
    // default generator
    static std::mt19937 mtGen32(rd());
    static std::mt19937_64 mtGen64(rd());
	static std::minstd_rand0 msGen0(rd());
	static std::minstd_rand msGen(rd());
	static std::ranlux24_base r24b(rd());
	static std::ranlux48_base r48b(rd());
	static std::ranlux24 r24(rd());
	static std::ranlux48 r48(rd());
	static std::knuth_b		kb(rd());
	static std::default_random_engine	dre(rd());

    std::uniform_int_distribution<int> dist(0, INT_MAX);

    if (strcmp(ngtype, "mt32") == 0){
        return dist(mtGen32);
    }
    else if(strcmp(ngtype, "mt64") == 0){
    	return dist(mtGen64);
    }
    else if(strcmp(ngtype, "ms0") == 0) {
        return dist(msGen0);
    }
    else if(strcmp(ngtype, "ms") == 0) {
        return dist(msGen);
    }
    else if(strcmp(ngtype, "r24b") == 0) {
        return dist(r24b);
    }
    else if(strcmp(ngtype, "r48b") == 0) {
        return dist(r48b);
    }
    else if(strcmp(ngtype, "r24") == 0) {
        return dist(r24);
    }
    else if(strcmp(ngtype, "r48") == 0) {
        return dist(r48);
    }
    else if(strcmp(ngtype, "kb") == 0) {
        return dist(kb);
    }
    else if(strcmp(ngtype, "dre") == 0) {
        return dist(dre);
    }
    else if(strcmp(ngtype, "rn") == 0) {
        return dist(rd);
    }
    else{
        return dist(mtGen32); // fallback
    }
}

float perlin(float x, float y) {
    int X = (int)floor(x) & 255;
    int Y = (int)floor(y) & 255;

    x -= floor(x);
    y -= floor(y);

    float u = fade(x);
    float v = fade(y);

    int aa = p[p[X] + Y];
    int ab = p[p[X] + Y + 1];
    int ba = p[p[X + 1] + Y];
    int bb = p[p[X + 1] + Y + 1];

    float res = lerp(
        lerp(grad(aa, x, y), grad(ba, x - 1, y), u),
        lerp(grad(ab, x, y - 1), grad(bb, x - 1, y - 1), u),
        v
    );

    return (res + 1.0f) / 2.0f; // normalize to 0..1
}

#endif
