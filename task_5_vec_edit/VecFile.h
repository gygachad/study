class VecFile
{
public:
	bool SaveModelToFile(const std::string& path, const VecModel& vecModel);
	bool LoadModelFromFile(const std::string path);
};