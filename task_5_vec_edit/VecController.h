#pragma once

class VecController
{
	VecModel		m_VecModel;
	VecScreenView	m_VecScreenView;
	VecFileView		m_VecFileView;

public:
	VecController() {};

	template<typename T, typename... Args>
	auto CreatePrimitive(Args... arguments)
	{
		auto primitive = std::make_shared<T>(T(std::forward<Args>(arguments)...));
		
		m_VecModel.m_VecPrimitives.push_back(primitive);

		return primitive;
	}

	bool DeletePrimitive(size_t prNum)
	{
		if (m_VecModel.m_VecPrimitives.size() < prNum)
			return false;

		m_VecModel.m_VecPrimitives.erase(m_VecModel.m_VecPrimitives.begin() + prNum);

		return true;
	}

	void UpdateScreenView()
	{
		std::cout << "========================" << std::endl;
		for (auto& pr : m_VecModel.m_VecPrimitives)
		{
			m_VecScreenView.outPrimitive(*pr);
		}
		std::cout << "========================" << std::endl;
	}

	void ClearModel()
	{
		m_VecModel.m_VecPrimitives.clear();
		m_VecModel.m_VecSelectedPrimitives.clear();
	}

	bool SaveModel(const std::string& path)
	{
		if (!m_VecFileView.OpenStream(path, std::ios_base::out))
			return false;
		
		for (auto& pr : m_VecModel.m_VecPrimitives)
		{
			m_VecFileView.outPrimitive(*pr);
			m_VecFileView.nextPrDelimiter();
		}

		m_VecFileView.CloseStream();

		return true;
	}

	bool LoadModel(const std::string& path)
	{
		if (!m_VecFileView.OpenStream(path, std::ios_base::in))
			return false;

		ClearModel();

		std::string pr;
		
		while (m_VecFileView.ReadNextPrimitive(pr))
		{
			if (!ParsePrimitive(pr))
			{
				ClearModel();
				std::cout << "File parse error" << std::endl;
				break;
			}
		}

		m_VecFileView.CloseStream();

		return true;
	}

	//TODO - make another class for parse file
	bool ParsePrimitive(const std::string& pr)
	{
		std::vector<std::string> prVec = StrTool::split(pr, ':');

		//Magic number comes here
		if (prVec.size() < 3)
			return false;

		if (prVec[0] == "VecPoint")
		{
			CreatePrimitive<VecPoint>(std::stoi(prVec[1]), std::stoi(prVec[1]));
			return true;
		}

		if (prVec[0] == "VecLine")
		{
			//Magic number comes here
			if (prVec.size() < 7)
				return false;

			VecPoint pt1 = VecPoint(std::stoi(prVec[2]), std::stoi(prVec[3]));
			VecPoint pt2 = VecPoint(std::stoi(prVec[5]), std::stoi(prVec[6]));

			CreatePrimitive<VecLine>(pt1, pt2);
			return true;
		}

		if (prVec[0] == "VecRect")
		{
			//Magic number comes here
			if (prVec.size() < 7)
				return false;

			VecPoint pt1 = VecPoint(std::stoi(prVec[2]), std::stoi(prVec[3]));
			VecPoint pt2 = VecPoint(std::stoi(prVec[5]), std::stoi(prVec[6]));

			CreatePrimitive<VecRect>(pt1, pt2);
			return true;
		}

		return false;
	}

	bool Select(size_t prNum)
	{
		if (m_VecModel.m_VecPrimitives.size() < prNum)
			return false;

		auto primitive = m_VecModel.m_VecPrimitives[prNum];

		m_VecModel.m_VecSelectedPrimitives.push_back(primitive);

		return true;
	}

	bool Unselect(size_t prNum)
	{
		if (m_VecModel.m_VecSelectedPrimitives.size() < prNum)
			return false;

		m_VecModel.m_VecSelectedPrimitives.erase(m_VecModel.m_VecSelectedPrimitives.begin() + prNum);

		return true;
	}

	void UnselectAll()
	{
		m_VecModel.m_VecSelectedPrimitives.clear();
	}

	void MoveSelected(int x, int y)
	{
		for (auto& pr : m_VecModel.m_VecSelectedPrimitives)
		{
			pr->move(x, y);
		}
	}

	void ResizeSelected(int x, int y)
	{
		for (auto& pr : m_VecModel.m_VecSelectedPrimitives)
		{
			pr->resize(x, y);
		}
	}
};