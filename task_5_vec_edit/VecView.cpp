#include "stdafx.h"

//Base method for view
void IVecView::outPrimitive(const IVecPrimitive& primitive)
{
	switch (primitive.get_type())
	{
		case PrimitiveType::VecTypePoint:
		{
			outPrimitiveWithType(dynamic_cast<const VecPoint&>(primitive));
			break;
		}
		case PrimitiveType::VecTypeLine:
		{
			outPrimitiveWithType(dynamic_cast<const VecLine&>(primitive));
			break;
		}
		case PrimitiveType::VecTypeRect:
		{
			outPrimitiveWithType(dynamic_cast<const VecRect&>(primitive));
			break;
		}
		default:
		{
			std::cout << "View does not support this primitive" << std::endl;
			assert(0);
			break;
		}
	}
}

//Screen view for GUI output
void VecScreenView::outPrimitiveWithType(const VecPoint& primitive)
{
	std::cout << "VecPoint at x=" << primitive.get_x() << " y=" << primitive.get_y() << std::endl;
}

void VecScreenView::outPrimitiveWithType(const VecLine& primitive)
{
	std::cout << "VecLine at p1=" << primitive.get_p1().get_x() << "," << primitive.get_p1().get_y() <<
						   " p2=" << primitive.get_p2().get_x() << "," << primitive.get_p2().get_y() << std::endl;
}

void VecScreenView::outPrimitiveWithType(const VecRect& primitive)
{
	std::cout << "VecRect at p1=" << primitive.get_left_top().get_x() << "," << primitive.get_left_top().get_y() <<
		                   " p2=" << primitive.get_rigth_bottom().get_x() << "," << primitive.get_rigth_bottom().get_y() << std::endl;
}

void VecScreenView::cls()
{
	//Time for magic here
	//CSI[2J clears screen, CSI[H moves the cursor to top-left corner
	std::cout << "\x1B[2J\x1B[H";
}

//File view for file save
void VecFileView::outPrimitiveWithType(const VecPoint& primitive)
{
	fVecStream << "VecPoint:" << primitive.get_x() << ':' << primitive.get_y();
}

void VecFileView::outPrimitiveWithType(const VecLine& primitive)
{
	fVecStream << "VecLine:";
	outPrimitive(primitive.get_p1());
	fVecStream << ':';
	outPrimitive(primitive.get_p2());
}

void VecFileView::outPrimitiveWithType(const VecRect& primitive)
{
	fVecStream << "VecRect:";
	outPrimitive(primitive.get_left_top());
	fVecStream << ':';
	outPrimitive(primitive.get_rigth_bottom());
}

bool VecFileView::OpenStream(const std::string& path, std::ios_base::openmode mode)
{
	fVecStream.open(path, mode);

	if (!fVecStream.is_open())
	{
		return false;
	}
	return true;
}

bool VecFileView::ReadNextPrimitive(std::string& pr)
{
	std::getline(fVecStream, pr);

	return pr != "" ? true : false;
}

void VecFileView::CloseStream()
{
	fVecStream.close();
}