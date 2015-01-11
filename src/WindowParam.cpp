/*
 * @file	CrosshairParam.cpp
 * @date	2014-12-31
 * @author	Emile
 * @brief	
 */

#include "WindowParam.h"

#include <Wingdi.h>

namespace xover
{

WindowParam::WindowParam() :
				mTransparencyMaskColor(RGB(255, 255, 255)),
				mOpacity(255)
{
	mRect.left = mRect.top = 0;

	this->setSize();
}

WindowParam::~WindowParam()
{
	// TODO Auto-generated destructor stub
}

BYTE WindowParam::getOpacity() const
{
	return mOpacity;
}

void WindowParam::setOpacity(BYTE opacity)
{
	mOpacity = opacity;
}

const RECT& WindowParam::getRect() const
{
	return mRect;
}

void WindowParam::setRect(const RECT& rect)
{
	mRect = rect;
}

COLORREF WindowParam::getTransparencyMaskColor() const
{
	return mTransparencyMaskColor;
}

void WindowParam::setSize(LONG width, LONG height)
{
	mRect.right = mRect.left + width;
	mRect.bottom = mRect.top + height;
}

LONG WindowParam::getWidth() const
{
	return mRect.right - mRect.left;
}

LONG WindowParam::getHeight() const
{
	return mRect.bottom - mRect.top;
}

void WindowParam::setPosition(POINT pos)
{
	this->setPosition(pos.x, pos.y);
}

void WindowParam::setPosition(LONG x, LONG y)
{
	LONG width = this->getWidth();
	LONG height = this->getHeight();
	mRect.left = x;
	mRect.top = y;
	this->setSize(width, height);
}

POINT WindowParam::getPosition() const
{
	POINT p;
	p.x = mRect.left;
	p.y = mRect.top;
	return p;
}

LONG WindowParam::getX() const
{
	return mRect.left;
}

LONG WindowParam::getY() const
{
	return mRect.top;
}

void WindowParam::setTransparencyMaskColor(COLORREF transparencyColorKey)
{
	mTransparencyMaskColor = transparencyColorKey;
}

void WindowParam::setTransparencyMaskColor(BYTE red, BYTE green, BYTE blue)
{
	mTransparencyMaskColor = RGB(red, green, blue);
}

const std::wstring& WindowParam::getImageFilename() const
{
	return mImageFilename;
}

void WindowParam::setImageFilename(const std::wstring& filename)
{
	mImageFilename = filename;
}

} /* namespace xover */

