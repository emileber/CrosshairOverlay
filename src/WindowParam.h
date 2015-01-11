/*
 * @file	CrosshairParam.h
 * @date	2014-12-31
 * @author	Emile
 * @brief	
 */

#ifndef WINDOWPARAM_H_
#define WINDOWPARAM_H_

//#include <Windef.h>
#include <windows.h>

#include <string>

namespace xover
{

class WindowParam
{
	public:
		WindowParam();
		virtual ~WindowParam();

		BYTE getOpacity() const;
		void setOpacity(BYTE opacity);

		const RECT& getRect() const;
		void setRect(const RECT& rect);
		void setSize(LONG width = 40, LONG height = 40);
		LONG getWidth() const;
		LONG getHeight() const;
		void setPosition(POINT pos);
		void setPosition(LONG x, LONG y);
		POINT getPosition() const;
		LONG getX() const;
		LONG getY() const;

		COLORREF getTransparencyMaskColor() const;
		void setTransparencyMaskColor(COLORREF transparencyColorKey);
		void setTransparencyMaskColor(BYTE red, BYTE green, BYTE blue);
		const std::wstring& getImageFilename() const;
		void setImageFilename(const std::wstring& filename);

	private:
		RECT mRect;
		COLORREF mTransparencyMaskColor; /** uses RGB(r,g,b) macro */
		BYTE mOpacity; /** 0 to 255 */
		std::wstring mImageFilename;

};

} /* namespace xover */

#endif /* WINDOWPARAM_H_ */
