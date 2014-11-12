#ifndef __fonts_h__
#define __fonts_h__
#include <arch\arch.h>

typedef struct _Font
{
	CHAR	Signature[4];		/* Font file signature.							*/
	UCHAR	CharWidth;			/* Character Width								*/
	UCHAR	CharHeight;			/* Charecter Height								*/
	UCHAR	BytesPerChar;		/* Bytes Per Charecter							*/
	UCHAR	SpacingX;			/* X Character Spacing							*/
	UCHAR   SpacingY;			/* Y Character Spacing							*/
	UCHAR	Reserved[3];		/* Reserved										*/
	UINT32	CharCount;			/* Total Characters in font						*/
	UINT32	DataLength;			/* Length of Data Area							*/
	UINT32	OffsetName;			/* Offset of Font Name (in data area)			*/
	UINT32	OffsetCopyright;	/* Offset of Copyright String (in data area)	*/
	UINT32  OffsetFont;			/* Offset of Font Bitmaps (in data area)		*/
	BYTE	FontData[];			/* Data Area									*/
}Font;

#define FONT_SIGNATURE		"FONT"

#endif //__fonts_h__