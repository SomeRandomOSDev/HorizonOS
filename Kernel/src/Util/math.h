#pragma once

int floor(float nb)
{
	return (int)nb;
}

int ceil(float nb)
{
	if(nb == (float)floor(nb))
		return nb;
	return floor(nb) + 1;
}
int round(float nb)
{
	return floor(nb + 0.5);
}

double sqrt(double x)
{
	double r;
	asm ("fsqrt" : "=t" (r) : "0" (x));
	return r;
}