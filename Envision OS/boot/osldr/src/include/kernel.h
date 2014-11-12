#ifndef __kernel_h__
#define __kernel_h__

#pragma pack(push,4)
typedef struct _kmem_map
{
    unsigned int start_low;
    unsigned int start_high;
    unsigned int size_low;
    unsigned int size_high;
    unsigned int type;
	unsigned int p;
}kmem_map;

typedef struct _kimage_map
{
    unsigned __int64 start;
    unsigned int size;
}kimage_map;

typedef struct _kvideo_mode
{
    unsigned __int64 FrameBuffer;
    unsigned int FrameBufferSize;
    unsigned int ScanLineSize;
    unsigned int ModeWidth;
    unsigned int ModeHeight;
    unsigned int BitsPerPixel;
	unsigned int RedMaskSize;
	unsigned int GreenMaskSize;
	unsigned int BlueMaskSize;
	unsigned int RedFieldPosition;
	unsigned int GreenFieldPosition;
	unsigned int BlueFieldPosition;
}kvideo_mode;

typedef struct _kboot_info
{
    unsigned __int64 kernel_paddr;
    unsigned int kernel_size;
    unsigned __int64 stack_paddr;
    unsigned int stack_size;
    void *stack_top;
    unsigned __int64 page_data;
    unsigned int page_data_size;
    unsigned int mmap_cnt;
    unsigned int imap_cnt;
    kmem_map *mmap;
    kimage_map *imap;
	kvideo_mode video_mode;
}kboot_info;
#pragma pack(pop)

typedef void (*kernel_entry)(kboot_info *bi);

#endif // __kernel_h__
