/*
=================
cD3DXFont.cpp
- Header file for class definition - IMPLEMENTATION
=================
*/
#include "cD3DXFont.h"
/*
=================
- Data constructor initializes the cBalloon to the data passed to 
- the constructor from the paramater sPosition.
- Is always called, thus ensures all sprite objects are in a consistent state.
=================
*/
cD3DXFont::cD3DXFont()
{
	mFont = NULL;
}
cD3DXFont::cD3DXFont(LPDIRECT3DDEVICE9 theDevice, HINSTANCE hInstance, LPCSTR theFontname) // Constructor
{
	if(FAILED(createCustomFont( hInstance, theFontname)))
	{
		OutputDebugString("Error: Cannot create Custom Font!");
	}
	else 
		if(FAILED(createDXFont( theDevice )))
		{
			OutputDebugString("Error: Cannot create DX Font!");
		}

}
/*
=================
- Destructor.
=================
*/
cD3DXFont::~cD3DXFont()					// Destructor
{
	if(mFont != NULL)
		mFont->Release();
	RemoveFontMemResourceEx(mHFontHdle);
}
/*
=================
- Use windows to find the custom font.
=================
*/
HRSRC cD3DXFont::locateFontResource(HINSTANCE hInstance, LPCSTR fontName)	// Use windows to find the custom font
{
	return mCustFont = FindResource(hInstance, "Fonts\\8-Bit.TTF", RT_FONT);
}
/*
=================
- Use windows to create a custom font in memory.
=================
*/
HRESULT cD3DXFont::createCustomFont(HINSTANCE hInstance, LPCSTR fontName)	// Use windows to create a custom font in memory;
{
	 if (locateFontResource(hInstance, fontName)) 
     {
          HGLOBAL mem = LoadResource(hInstance, mCustFont);
          void *data = LockResource(mem);
          size_t len = SizeofResource(hInstance, mCustFont);

          DWORD nFonts;
          mHFontHdle = AddFontMemResourceEx(data, len, NULL, &nFonts);

          if(mHFontHdle == 0)
		  {
			  OutputDebugString("Error: Font add fails");
			  return S_FALSE;
		  }
     }
	 return S_OK;
}
/*
=================
- Create the DX Font based on the custom font.
=================
*/
HRESULT cD3DXFont::createDXFont(LPDIRECT3DDEVICE9 theDevice)	// Create the DX Font based on the custom font
{
	mFontDesc.Height = 30;
	mFontDesc.Width = 0;
	mFontDesc.Weight = FW_NORMAL;
	mFontDesc.MipLevels = 0;
	mFontDesc.Italic = false;
	mFontDesc.CharSet = DEFAULT_CHARSET;
	mFontDesc.OutputPrecision = OUT_TT_PRECIS;
	mFontDesc.Quality = CLIP_DEFAULT_PRECIS;
	mFontDesc.PitchAndFamily = DEFAULT_PITCH;
	strcpy_s(mFontDesc.FaceName, 32, "8BIT WONDER");

	if (FAILED(D3DXCreateFontIndirect(theDevice, &mFontDesc, &mFont)))
	{
		 OutputDebugString("Error: D3DXFont failed");
		 return 0;
	}
	return S_OK;
}
/*
=================
- Draw the text onto the screen.
=================
*/
void cD3DXFont::printText(LPCSTR string, RECT pos)				// Draw the text onto the screen.
{
	mFont->DrawText(NULL, string,  -1, &pos, DT_LEFT, 0xffff0000);
}
