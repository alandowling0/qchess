#include "stdafx.h"
#include "Evaluation.h"

Evaluation::Evaluation(int aScore, int aDepth, EvaluationType aType):
iScore(aScore), iDepth(aDepth), iType(aType)
{

}

Evaluation::Evaluation(int32_t aCompressed)
{
	iScore = (aCompressed & 0xFFFF0000) >> 16;
	if (aCompressed & 0x00000001)
		iScore *= -1;

	iDepth = (aCompressed & 0x0000FF00) >> 8;

	iType = static_cast<EvaluationType>((aCompressed & 0x000000F0) >> 4);
}


int Evaluation::Score() const
{
	return iScore;
}

int Evaluation::Depth() const
{
	return iDepth;
}

EvaluationType Evaluation::Type() const
{
	return iType;
}

int32_t Evaluation::Compressed() const
{
	int32_t compressed = 0;

	if (iScore < 0)
		compressed += (-1 * iScore) << 16;
	else
		compressed += iScore << 16;

	compressed += iDepth << 8;
	compressed += static_cast<int>(iType) << 4;
	
	if (iScore < 0)
		compressed += 1;

	return compressed;
}