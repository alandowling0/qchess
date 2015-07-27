#pragma once

enum class Direction
{
	ENorth,
	ESouth,
	EEast,
	EWest,
	ENorthEast,
	ESouthEast,
	ENorthWest,
	ESouthWest
};

namespace DirectionUtils
{
	void Move(Direction aDirection, int aDistance, int& aX, int& aY);
}