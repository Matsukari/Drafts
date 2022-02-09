#include <iostream>
#include <cstdlib>
#include <string>
#include <math.h>
#include <cmath>

typedef struct Vector {
	double x;
	double y;
}Vector;

// Creations
Vector *STR_CreateVector(double x, double y);
void STR_DestroyVector(Vector *v);

// Extra
void STR_SetLength(double length, Vector *vec);
void STR_SetAngle(double angle, Vector *vec);
double STR_GetAngle(Vector *vec);
double STR_GetLength(Vector *vec);

// Operations 
Vector *STR_AddVector(Vector *v1, Vector *v2);
Vector *STR_SubtractVector(Vector *v1, Vector *v2);
Vector *STR_MultiplyVector(Vector *v1, double val);
Vector *STR_DivideVector(Vector *v1, double val);

void STR_AddVectorToo(Vector *src, Vector *val);
void STR_SubtractVectorToo(Vector *src, Vector *val);
void STR_MultiplyVectorToo(Vector *src, Vector *val);
void STR_DivideVectorToo(Vector *src, Vector *val);

double STR_RadianToDegree(double rad);
double STR_DegreeToRadian(double deg);

#define			PI					3.1415926353890

Vector *STR_CreateVector(double x, double y)
{
	Vector *newVector = (Vector*)malloc(sizeof(Vector));

	newVector->x = (double)x;
	newVector->y = (double)y;

	return (newVector);
}
void STR_DestroyVector(Vector *v)
{
	if (v != NULL){
		free(v);
	}
	v = NULL;
}

void STR_SetAngle(double angle, Vector *vec)
{
	double length = STR_GetLength(vec);
	vec->x = cos(angle) * length;
	vec->y = sin(angle) * length;
}
double STR_GetAngle(Vector *vec)
{
	return (atan2(vec->x, vec->y));
}
void STR_SetLength(double length, Vector *vec)
{
	double angle = STR_GetAngle(vec);
	vec->x = cos(angle) * length;
	vec->y = sin(angle) * length;
}
double STR_GetLength(Vector *vec)
{
	return (sqrt(vec->x * vec->x + vec->y * vec->y));
}

Vector *STR_AddVector(Vector *v1, Vector *v2)
{
	Vector *newVector = (Vector*)malloc(sizeof(Vector));

	newVector->x = v1->x + v2->x;
	newVector->y = v1->y + v2->y;

	return (newVector);
}
Vector *STR_SubtractVector(Vector *v1, Vector *v2)
{
	Vector *newVector = (Vector*)malloc(sizeof(Vector));

	newVector->x = v1->x - v2->x;
	newVector->y = v1->y - v2->y;

	return (newVector);
}
Vector *STR_MultiplyVector(Vector *v1, double val)
{
	Vector *newVector = (Vector*)malloc(sizeof(Vector));

	newVector->x = v1->x * val;
	newVector->y = v1->y * val;

	return (newVector);
}
Vector *STR_DivideVector(Vector *v1, double val)
{
	Vector *newVector = (Vector*)malloc(sizeof(Vector));

	newVector->x = v1->x / val;
	newVector->y = v1->y / val;

	return (newVector);
}
void STR_AddVectorToo(Vector *src, Vector *val)
{
	src->x += val->x;
	src->y += val->y;
}
void STR_SubtractVectorToo(Vector *src, Vector *val)
{
	src->x -= val->x;
	src->y -= val->y;
}
void STR_MultiplyVectorToo(Vector *src, Vector *val)
{
	src->x *= val->x;
	src->y *= val->y;
}
void STR_DivideVectorToo(Vector *src, Vector *val)
{
	src->x /= val->x;
	src->y /= val->y;
}

double STR_RadianToDegree(double rad)
{
	return ((rad * 2) / PI);
}
double STR_DegreeToRadian(double deg)
{
	return ((deg * PI) / 2);
}


int main(int argc, char** argv)
{
	Vector* vec = STR_CreateVector(10, 10);

	STR_SetLength(100, vec);
	std::cout << "\nSet Len: " << vec->x << " " << vec->y;
	std::cout << "\nGet Len: " << STR_GetLength(vec);
	STR_SetAngle(90, vec);
	std::cout << "\nSet Angle: " << vec->x << " " << vec->y;
	std::cout << "\nGet Angle: " << STR_RadianToDegree(STR_GetAngle(vec));


	STR_DestroyVector(vec);

	getchar();
	return 0;
}