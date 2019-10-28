#pragma once
#if !defined (SOURCE_TYPE)
	#define SOURCE_TYPE float
#endif

typedef struct MinMaxContextE
{
	SOURCE_TYPE* x;
	SOURCE_TYPE* y;
	size_t dataIdx;
	size_t idx;
	float oldxv;
	float oldyv;
	size_t maxSize;//we recommend to use twice less value than size of ContextE buffers, this variable should be inited by number of cells into ContextE buffer
}MinMaxContextE;

//all buffers should have the same size ie
/*
	ce->kImf= (float*)malloc(sz*sizeof(float));
	ce->bBias = (float*)malloc(sz*sizeof(float));
	ce->m_b = (double*)malloc(sz*sizeof(double));
	ce->m_c = (double*)malloc(sz*sizeof(double));
	ce->m_d = (double*)malloc(sz*sizeof(double));

//MinMax twice less
	ce->minfc.x = (float*)malloc(sz/2*sizeof(float));
	ce->minfc.y = (float*)malloc(sz/2*sizeof(float));
	ce->maxfc.x = (float*)malloc(sz/2*sizeof(float));
	ce->maxfc.y = (float*)malloc(sz/2*sizeof(float));
*/

struct ContextE
{
	SOURCE_TYPE* kImf;
	MinMaxContextE minfc;
	MinMaxContextE maxfc;
	SOURCE_TYPE* bBias;
	double* m_b;
	double* m_c;
	double* m_d;
};


//-1 loop detected, should never happens
//-2 broken by callback, if callback returns false execution stopped
int rParabEmd__LEmb (SOURCE_TYPE* kc, int quntN, int qResol, int qResid, ContextE* c, double* prev, bool(*imfCallback)(SOURCE_TYPE* data, size_t size, size_t imf) = NULL);//minP ticks 60 PPS=666 120 PPS=333
void clearFlyC(MinMaxContextE* c);

//you can init pointers by static buffers if you need, or write your own allocator for it
void alloc(ContextE* ce, size_t size);
void release(ContextE* ce);
