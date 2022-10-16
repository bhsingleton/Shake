#include "Noise.h"


namespace Noise
{

	void init()
	{

		int i, j, k;
		srand(0);

		for (i = 0; i < B; i++)
		{

			p[i] = i;
			g1[i] = (float)((rand() % (B + B)) - B) / B;

			for (j = 0; j < 2; j++)
			{

				g2[i][j] = (float)((rand() % (B + B)) - B) / B;

			}
				
			normalize2(g2[i]);

			for (j = 0; j < 3; j++)
			{

				g3[i][j] = (float)((rand() % (B + B)) - B) / B;

			}
				
			normalize3(g3[i]);

		}

		while (--i) 
		{

			k = p[i];
			p[i] = p[j = rand() % B];
			p[j] = k;

		}

		for (i = 0; i < B + 2; i++)
		{

			p[B + i] = p[i];
			g1[B + i] = g1[i];

			for (j = 0; j < 2; j++)
			{

				g2[B + i][j] = g2[i][j];

			}

			for (j = 0; j < 3; j++)
			{

				g3[B + i][j] = g3[i][j];

			}

		}

	};

	int permutate(int v)
	{

		return p[v & BM];

	};

	void normalize2(float v[2])
	{

		float s;

		s = sqrt(v[0] * v[0] + v[1] * v[1]);
		v[0] = v[0] / s;
		v[1] = v[1] / s;

	}

	void normalize3(float v[3])
	{

		float s;

		s = sqrt(v[0] * v[0] + v[1] * v[1] + v[2] * v[2]);
		v[0] = v[0] / s;
		v[1] = v[1] / s;
		v[2] = v[2] / s;

	}

	float bias(float a, float b)
	{

		return pow(a, log(b) / log(0.5));

	};

	float gain(float a, float b)
	{

		float p = log(1.0 - b) / log(0.5);

		if (a < 0.001)
		{

			return 0.0;

		}
		else if (a > 0.999)
		{

			return 1.0;

		}
		else if (a < 0.5)
		{

			return pow(2 * a, p) / 2.0;

		}
		else
		{

			return 1.0 - pow(2.0 * (1.0 - a), p) / 2.0;

		}

	};

	float noise(float vec[], int len)
	{

		switch (len)
		{

		case 1:
			return noise1(vec[0]);

		case 2:
			return noise2(vec);

		case 3:
			return noise3(vec);

		default:
			return 0.0;

		}

	};

	float noise1(float arg)
	{

		int bx0, bx1;
		float rx0, rx1, sx, t, u, v, vec[1];

		vec[0] = arg;

		if (start)
		{

			start = 0;
			init();

		}

		setup(0, bx0, bx1, rx0, rx1);

		sx = s_curve(rx0);

		u = rx0 * g1[p[bx0]];
		v = rx1 * g1[p[bx1]];

		return lerp(sx, u, v);

	};

	float noise2(float vec[2])
	{

		int bx0, bx1, by0, by1, b00, b10, b01, b11;
		float rx0, rx1, ry0, ry1, * q, sx, sy, a, b, t, u, v;
		int i, j;

		if (start)
		{

			start = 0;
			init();

		}

		setup(0, bx0, bx1, rx0, rx1);
		setup(1, by0, by1, ry0, ry1);

		i = p[bx0];
		j = p[bx1];

		b00 = p[i + by0];
		b10 = p[j + by0];
		b01 = p[i + by1];
		b11 = p[j + by1];

		sx = s_curve(rx0);
		sy = s_curve(ry0);

		q = g2[b00]; u = at2(rx0, ry0);
		q = g2[b10]; v = at2(rx1, ry0);
		a = lerp(sx, u, v);

		q = g2[b01]; u = at2(rx0, ry1);
		q = g2[b11]; v = at2(rx1, ry1);
		b = lerp(sx, u, v);

		return lerp(sy, a, b);

	}

	float noise3(float vec[3])
	{

		int bx0, bx1, by0, by1, bz0, bz1, b00, b10, b01, b11;
		float rx0, rx1, ry0, ry1, rz0, rz1, * q, sy, sz, a, b, c, d, t, u, v;
		int i, j;

		if (start)
		{

			start = 0;
			init();

		}

		setup(0, bx0, bx1, rx0, rx1);
		setup(1, by0, by1, ry0, ry1);
		setup(2, bz0, bz1, rz0, rz1);

		i = p[bx0];
		j = p[bx1];

		b00 = p[i + by0];
		b10 = p[j + by0];
		b01 = p[i + by1];
		b11 = p[j + by1];

		t = s_curve(rx0);
		sy = s_curve(ry0);
		sz = s_curve(rz0);

		q = g3[b00 + bz0]; u = at3(rx0, ry0, rz0);
		q = g3[b10 + bz0]; v = at3(rx1, ry0, rz0);
		a = lerp(t, u, v);

		q = g3[b01 + bz0]; u = at3(rx0, ry1, rz0);
		q = g3[b11 + bz0]; v = at3(rx1, ry1, rz0);
		b = lerp(t, u, v);

		c = lerp(sy, a, b);

		q = g3[b00 + bz1]; u = at3(rx0, ry0, rz1);
		q = g3[b10 + bz1]; v = at3(rx1, ry0, rz1);
		a = lerp(t, u, v);

		q = g3[b01 + bz1]; u = at3(rx0, ry1, rz1);
		q = g3[b11 + bz1]; v = at3(rx1, ry1, rz1);
		b = lerp(t, u, v);

		d = lerp(sy, a, b);

		return lerp(sz, c, d);

	}

	float fractionalBrownianMotion(float point, float H, float lacunarity, float octaves)
	{

		static float exponent_array[MAX_OCTAVES + 1];
		static float lastH;
		float value, frequency, remainder;
		int i;
		static int first = TRUE;

		// Precompute and store spectral weights
		//
		if (first || H != lastH)
		{

			lastH = H;
			frequency = 1.0;

			for (i = 0; i <= octaves; i++)  // compute weight for each frequency
			{

				exponent_array[i] = pow(frequency, -H);
				frequency *= lacunarity;

			}

			first = FALSE;

		}

		// Initialize variables to proper values
		//
		value = 0.0;
		frequency = 1.0;

		/// Inner loop of spectral construction
		//
		for (i = 0; i < octaves; i++) 
		{

			value += noise1(point) * exponent_array[i];
			point *= lacunarity;

		}

		remainder = octaves - (int)octaves;

		if (remainder)  // add in octaves remainder
		{

			value += remainder * noise1(point) * exponent_array[i];  // i and spatial frequency are preset in loop above!

		}

		return value;

	};

};