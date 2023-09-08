#pragma once

enum class PrimitiveType
{
	VecTypePoint = 0,
	VecTypeLine,
	VecTypeRect
};

class IVecPrimitive
{
public:
	virtual ~IVecPrimitive() {};

	virtual PrimitiveType get_type() const = 0;
	virtual void move(int x, int y) = 0;
	virtual void resize(int x, int y) = 0;
};

class VecPoint : public IVecPrimitive
{
	int m_x, m_y;

public:
	VecPoint() : m_x(0), m_y(0) {};
	VecPoint(int x, int y) : m_x(x), m_y(y) {};
	~VecPoint() {};

	PrimitiveType get_type() const { return PrimitiveType::VecTypePoint; };

	int get_x() const { return m_x; };
	int get_y() const { return m_y; };

	void move(int x, int y);
	void resize(int, int) {};
};

class VecLine : public IVecPrimitive
{
	VecPoint m_p1, m_p2;

public:
	VecLine() : m_p1(VecPoint(0, 0)), m_p2(VecPoint(5, 5)) {};

	VecLine(const VecPoint& px, const VecPoint& py) : m_p1(px), m_p2(py) {};
	~VecLine() {};

	PrimitiveType get_type() const { return PrimitiveType::VecTypeLine; };
	VecPoint get_p1() const { return m_p1; };
	VecPoint get_p2() const { return m_p2; };

	void move(int x, int y);
	void resize(int x, int y);
};

class VecRect : public IVecPrimitive
{
	VecPoint m_left_top, m_rigth_bottom;

public:
	VecRect() : m_left_top(VecPoint(0, 0)), m_rigth_bottom(VecPoint(5, 5)) {};
	VecRect(const VecPoint& px, const VecPoint& py) : m_left_top(px), m_rigth_bottom(py) {};
	~VecRect() {};

	PrimitiveType get_type() const { return PrimitiveType::VecTypeRect; };
	VecPoint get_left_top() const { return m_left_top; };
	VecPoint get_rigth_bottom() const { return m_rigth_bottom; };

	void move(int x, int y);
	void resize(int x, int y);
};

class VecModel
{
public:
	std::vector<std::shared_ptr<IVecPrimitive>> m_VecPrimitives;
	std::vector<std::shared_ptr<IVecPrimitive>> m_VecSelectedPrimitives;
};