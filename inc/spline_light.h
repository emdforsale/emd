#pragma once

#include <stdint.h>

#define FLY_DATA_BUF_SIZE (3)
#define PARTS_RESULTS_COUNT (50)


typedef struct SplineFlyContextE
{
	int16_t dataIdx;
	int16_t resId;
	uint16_t totalIdx;//id of global buf, contains number of current value
	uint16_t bufIdx; //id of x,y buffer last value in bufIdx-1
	bool extrsProcessed;
	bool processLastExtremum;
	double m_b;
	double m_c;
	double m_d[FLY_DATA_BUF_SIZE];
	double x[FLY_DATA_BUF_SIZE];
	double y[FLY_DATA_BUF_SIZE];
	double lastRes;
	uint16_t curOutDataIdx;
	uint16_t i;
	uint16_t j;
	uint16_t state;
}SplineFlyContextE;


typedef struct MinMaxFlyContextE
{
	int16_t dataIdx;
	double kS0;
	double kS1;
	double kS2;
	double oldxv;
	double oldyv;
	double minVal;
	double maxVal;
	uint16_t dataOutIdx;//number of min or max that exists in buffers after return from function
	double x[FLY_DATA_BUF_SIZE];
	double y[FLY_DATA_BUF_SIZE];
	double splRes[PARTS_RESULTS_COUNT];
	uint16_t splResWrId;
	uint16_t splResReId;
	uint16_t totalIdx;//id of global buf, contains number of current value
	uint16_t bufIdx;
	bool firstElemProcessed;
}MinMaxFlyContextE;

typedef struct FlyContextE
{
	SplineFlyContextE splMin;
	SplineFlyContextE splMax;
	MinMaxFlyContextE mmMin;
	MinMaxFlyContextE mmMax;
	uint16_t MMContextIdx;
	uint16_t bufIdx;
}FlyContextE;

void cleanContext(FlyContextE* fc);
//-1 loop detected, should never happens
//-2 broken by callback, if callback returns false execution stopped
int rParabEmd__LFlyEmb(SOURCE_TYPE* kc, SOURCE_TYPE* tBuf, int dataCnt, int qResol, int qResid, FlyContextE* fce, bool(*imfCallback)(SOURCE_TYPE* data, size_t size, size_t imf));
