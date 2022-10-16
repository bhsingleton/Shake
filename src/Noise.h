#ifndef _Noise
#define _Noise
//
// File: Noise.h
// 
// About: These functions are taken from the 3ds Max SDK inside perlin.cpp!
//
// Author: Benjamin H. Singleton
//

#include <stdlib.h>
#include <stdio.h>
#include <math.h>

#define MAX_OCTAVES		50
#define TRUE			1
#define FALSE			0
#define B				0x100
#define BM				0xff
#define N				0x1000
#define NP				12  // 2^N
#define NM				0xfff

#define s_curve(t)					(t * t * (3.0 - 2.0 * t));
#define lerp(t, a, b)				(a + t * (b - a));
#define at2(rx, ry)					(rx * q[0] + ry * q[1]);
#define at3(rx, ry, rz)				(rx * q[0] + ry * q[1] + rz * q[2]);
#define setup(i, b0, b1, r0, r1)	t = vec[i] + N;\
									b0 = ((int)t) & BM;\
									b1 = (b0+1) & BM;\
									r0 = t - (int)t;\
									r1 = r0 - 1.0;


namespace Noise
{

	static	int		p[B + B + 2];
	static	float	g1[B + B + 2];
	static	float	g2[B + B + 2][2];
	static	float	g3[B + B + 2][3];
	static	int		start = 1;

	void	init();
	int		permutate(int i);
	void	normalize2(float v[2]);
	void	normalize3(float v[3]);

	float	bias(float a, float b);
	float	gain(float a, float b);
	
	float	noise(float v[], int len);
	float	noise1(float v);
	float	noise2(float v[2]);
	float	noise3(float v[3]);

	float	fractionalBrownianMotion(float point, float H, float lacunarity, float octaves);

};
#endif