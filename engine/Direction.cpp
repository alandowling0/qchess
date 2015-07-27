#include "stdafx.h"
#include "Direction.h"

void DirectionUtils::Move(Direction aDirection, int aDistance, int& aX, int& aY)
{
	switch (aDirection)
	{
	case Direction::EEast:
	case Direction::ENorthEast:
	case Direction::ESouthEast:
		aX += aDistance;
		break;
	case Direction::EWest:
	case Direction::ENorthWest:
	case Direction::ESouthWest:
		aX -= aDistance;
        break;
    default:
        break;
	}

	switch (aDirection)
    {
	case Direction::ENorth:
	case Direction::ENorthWest:
	case Direction::ENorthEast:
		aY += aDistance;
		break;
	case Direction::ESouth:
	case Direction::ESouthWest:
	case Direction::ESouthEast:
		aY -= aDistance;
        break;
    default:
        break;
	}
}
