#include <stdio.h>
#include <math.h>
int is_zero(double a)
{
	const double eps = 0.00000001;
	return (fabs(a) < eps);
}

int SolveLinear(double b, double c, double *x) // returns 2, if number of radicals is infinity
{
	int quantity;
	if (is_zero (b))
	{
		if (is_zero (c))
		{
			quantity = 2;
		}
		else
		{
			quantity = 0;
		}
	}
	else
	{
		*x = - c / b;
		quantity = 1;
	}
	return quantity;
}

int SolveSquare(double a, double b, double c, double *x1, double *x2) // returns 3, if number of radicals is infinity
{
	int quantity;
	if (is_zero (a))
	{
		int Linear_Quantity;
		Linear_Quantity = SolveLinear (b, c, x1);
		if (Linear_Quantity > 1)
		{
			quantity = 3;
		}
		else
		{
			quantity = Linear_Quantity;
		}
	}
	else
	{
		double d;
		d = b * b - 4 * a * c;
		if (is_zero (d))
		{
			quantity = 1;
			*x1 = - b / 2 / a;
		}
		else
		{
			if (d < 0)
			{
				quantity = 0;
			}
			else
			{
				quantity = 2;
				*x1 = (- b - sqrt(d)) / 2 / a;
				*x2 = (- b + sqrt(d)) / 2 / a;
			}
		}
	}
	if (is_zero (*x1))
	{
		*x1 = 0;
	}
	if (is_zero (*x2))
	{
		*x2 = 0;
	}
	return quantity;
}

int main()
{
	double a, b, c, x1, x2;
	int quantity;
	scanf ("%lg %lg %lg", &a, &b, &c);
	quantity = SolveSquare (a, b, c, &x1, &x2);
	switch (quantity)
	{
	case 0:
		printf ("NO");
		break;
	case 1:
		printf ("%lg", x1);
		break;
	case 2:
		if (x1 > x2)
		{
			a = x2;
			x2 = x1;
			x1 = a;
		}
		printf ("%lg %lg", x1, x2);
		break;
	case 3:
		printf ("R");
		break;
	}
	return 0;
}
