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
}
