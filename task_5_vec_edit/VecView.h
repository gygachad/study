#pragma once

class IVecView
{
public:
	void outPrimitive(const IVecPrimitive& primitive);
	virtual void outPrimitiveWithType(const VecPoint& primitive) = 0;
	virtual void outPrimitiveWithType(const VecLine& primitive) = 0;
	virtual void outPrimitiveWithType(const VecRect& primitive) = 0;
};

class VecScreenView : public IVecView
{
public:
	void outPrimitiveWithType(const VecPoint& primitive) override;
	void outPrimitiveWithType(const VecLine& primitive) override;
	void outPrimitiveWithType(const VecRect& primitive) override;

	void cls();
};

class VecFileView : public IVecView
{
	std::fstream fVecStream;

public:

	void outPrimitiveWithType(const VecPoint& primitive) override;
	void outPrimitiveWithType(const VecLine& primitive) override;
	void outPrimitiveWithType(const VecRect& primitive) override;

	void nextPrDelimiter() { fVecStream << std::endl; };
	bool ReadNextPrimitive(std::string& pr);

	bool OpenStream(const std::string& path, std::ios_base::openmode mode);
	void CloseStream();
};