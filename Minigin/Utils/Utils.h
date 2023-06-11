#pragma once

namespace utils
{
	inline int Get2DArrayIndex(int rowIndex, int nrCols, int collIndex)
	{
		int result{ rowIndex * nrCols + collIndex };
		return result;
	}

	inline int GetColIndex(int index, int nrCols)
	{
		int result{ index % nrCols };
		return result;
	}

	inline int GetRowIndex(int index, int nrCols)
	{
		int result{ index / nrCols };
		return result;
	}

	inline bool CompareFloats(float value1, float value2, float epsilon = 0.01f)
	{
		if(fabs(value1 - value2) < epsilon)
		{
			return true;
		}
		return false;
	}
}
