#include "stdafx.h"

bool VecFile::SaveModelToFile(const std::string& path, const VecModel& vecModel)
{
	std::fstream fout;
	fout.open(path, std::ios_base::out);

	if (!fout.is_open())
	{
		return false;
	}

	fout << "test" << std::endl;

	fout.close();

	return true;
}