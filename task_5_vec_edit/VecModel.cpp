#include "stdafx.h"

//VecPoint class defentition
void VecPoint::move(int x, int y)
{
	//TODO Check for windows bound
	m_x += x;
	m_y += y;
}

//VecLine class defentition
void VecLine::move(int x, int y)
{
	m_p1.move(x, y);
	m_p2.move(x, y);
}

void VecLine::resize(int x, int y)
{
	m_p2.move(x, y);
}

//VecRect class defentition
void VecRect::move(int x, int y)
{
	m_left_top.move(x, y);
	m_rigth_bottom.move(x, y);
}

void VecRect::resize(int x, int y)
{
	m_rigth_bottom.move(x, y);
}