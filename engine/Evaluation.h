#pragma once

enum class EvaluationType
{
	EAbsolute,
	EAlphaCutoff,
	EBetaCutoff
};

class Evaluation
{
public:
	Evaluation(int aScore, int aDepth, EvaluationType aType);
	Evaluation(int32_t aCompressed);

	int Score() const;
	int Depth() const;
	EvaluationType Type() const;
	int32_t Compressed() const;

private:
	static const int KMaxEvaluation = 0x0000FFFF;

	int iScore;
	int iDepth;
	EvaluationType iType;
};