#pragma once

#ifndef MAINHEADER

#define _CRT_SECURE_NO_WARNINGS
#define MAINHEADER
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <string.h>

typedef struct Object Object;
struct Object
{
	int cost;
	int weight;
	float efficiency;
	int index;
};
#include "MinHeapHeader.h"

#endif 
