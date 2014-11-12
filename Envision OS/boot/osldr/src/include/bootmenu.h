#ifndef __bootmenu_h__
#define __bootmenu_h__
#include <arch\arch.h>

typedef enum _ostype
{
	ostype_system = 0,
	ostype_bootsect,
	ostype_custom
}ostype;

typedef struct _osentry
{
	ostype type;
	UINT32 off_name;
	UINT32 off_path;
	UINT32 off_options;
}osentry;

typedef struct _bootdata
{
	UINT32   size;
	struct
	{
		UINT32 width;
		UINT32 height;
		UINT32 depth;
	}mode;
	UINT32	 timeout;
	UINT32   oscount;
	osentry  oslist[];
}bootdata;

#endif //__bootmenu_h__