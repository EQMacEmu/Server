#ifndef FASTMATH_H
#define FASTMATH_H

#ifdef _WINDOWS
#define M_PI	3.141592654
#endif

class FastMath
{
private:
	float lut_cos[512];
	float lut_sin[512];

public:
	FastMath();

	inline float FastSin(float a) { return lut_sin[static_cast<int>(a) & 0x1ff]; }
	inline float FastCos(float a) { return lut_cos[static_cast<int>(a) & 0x1ff]; }

};

#endif /* !FASTMATH_H */
