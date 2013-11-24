/********************************************************************
 *		mygdi.h
 *		created by JH.LIM		
 *		first revision .2005.05.7
 *		modifyed 	2009.2.12
 *             2010.10
 *********************************************************************/
#ifndef _GFX_H__
#define _GFX_H__

#define COLOR_16BIT 0
#define COLOR_32BIT 1

#define CUR_COLOR_BIT COLOR_32BIT


typedef unsigned int 	GFX_RGB32COLOR;
typedef unsigned short 	GFX_RGB16COLOR;


typedef struct _sGdiObject
{	
	int             iBkMode;
	unsigned char*  p_cur_gfx_surface;
    	int             iAlpha;    
	GFX_RGB32COLOR             iFGColor;
    	GFX_RGB32COLOR             iFGShadeColor;
    	GFX_RGB32COLOR            iBGColor;
	unsigned int             iFontType;
}sGdiObject;

enum _ROPMODE
{
    GFX_ROP_NONE = 0,
    GFX_ROP_COPY = 1,
    GFX_ROP_TRANSPARENCY = 2,
    GFX_ROP_ALPHABLEND = 4
};

enum _BMP_TYPE
{
    GFX_BPP_1 = 0,
    GFX_BPP_2 = 10,
    GFX_BPP_4 = 20,
    GFX_BPP_8 = 30,
    GFX_BPP_8RLE = 40,
    GFX_BPP_16 = 50,
    GFX_BPP_32 = 60
};
enum _TEXTFORMAT
{
    TF_NONE = 0,
    TF_SHADE = 1,
    TF_HCENTER = 2,
    TF_TRANSPARENT = 4,
    TF_ALPHABLEND = 8,
    TF_GET_STRING_WIDTH = 16
};
enum _TEXTBKMODE
{
    TM_OPAQUE = 0,
    TM_TRANSPARENT = 1
};

#define GFX_ID(a,b,c,d)     (((a)<<24) | ((b)<<16) | ((c)<<8) | (d))

#define GFX_LOWORD(l)           ((unsigned short)(l))
#define GFX_HIWORD(l)           ((unsigned short)(((unsigned long)(l) >> 16) & 0xFFFF))
#define GFX_LOBYTE(w)           ((unsigned char)(w))
#define GFX_HIBYTE(w)           ((unsigned char)(((unsigned short)(w) >> 8) & 0xFF))


#define GFX_RGB32(r,g,b)   ((GFX_RGB32COLOR)(((unsigned char)(b)|((unsigned int)((unsigned char)(g))<<8))|(((unsigned int)(unsigned char)(r))<<16)))
#define GFX_RGB16(r,g,b) (GFX_RGB16COLOR)((b >> 3) |((g >> 2) << 5) |((r >> 3) << 11))


#define GFX_BM_HEADERSIZE 16

#define GFX_BM_WIDTH(hBm) ((unsigned int)(((int) hBm[0] << 8) | (int)hBm[1]))
#define GFX_BM_HEIGHT(hBm) ((unsigned int)(( (int)hBm[2] << 8) |(int) hBm[3]))
//#define GFX_BM_BPP(hBm) (hBm[4])

#define GFX_BM_PALNUM(hBm) (hBm[5])

#define GFX_BM_TRANSPARENCY_INDEX(hBm) (hBm[6])


#define GFX_BM_IS_RCS(hBm) (hBm[7]))


#define GFX_BM_BPL(hBm) ((int)((hBm[14] << 8) | hBm[15]))

//#define GFX_BM_SIZE(hBm)  (((hBm[16])<<24) | ((hBm[17])<<16) | ((hBm[18])<<8) | (hBm[19]))
#define GFX_BM_PAL(hBm) ((unsigned short*)&hBm[16])
#define GFX_BM_PAL32(hBm) ((unsigned int*)&hBm[16])

#define GFX_SIZEOFBM(iBmWidth,iBmHeight,iBmBpp,iPalNum) (((iBmWidth) * (iBmHeight) * (iBmBpp) >> 3) + ((iPalNum) << 1) + 16)

#define gfx_set_cur_surface(pucBg)  g_sGdiObjectLCD.p_cur_gfx_surface = (unsigned char*)(pucBg)
#define gfx_get_screen_surface(x)  g_sGdiObjectLCD.p_cur_gfx_surface



void gfx_init_gdi(unsigned char* surface,unsigned char* framebuffer,int WIDTH,int HEIGHT,int bpp);

void gfx_create_bitmap(unsigned char *hbitmap,unsigned char *framebuffer,int width,int height,int bpp);
void gfx_prepare_bitmap(unsigned char *bitmap);

unsigned char* gfx_get_bmbits(unsigned char *surface);
 int GFX_BM_BPP(unsigned char *hSourceBitmap);
                           
void gfx_draw_fill_rect(  unsigned char *psurface,
                           int iLeft,
                           int iTop,
                           int iWidth,
                           int iHeight,
                           int iColor);
                           
void gfx_draw_pixel( unsigned  char* psurface ,int iX,int iY,int iColor);

void gfx_draw_rect( unsigned char *psurface,int iX1,int iY1,int iX2,int iY2,int iColor);

void gfx_draw_bitmap(unsigned char* p_dst_surface,
                       int iLeft,
                       int iTop,
                       unsigned char* hSourceBitmap, 
                       int iSourceLeft,
                       int iSourceTop,
                       int iSourceCopydWidth,
                       int iSourceCopydHeight,
                       int iRop);

int
gfx_draw_string(
		unsigned char*    p_dst_surface,
		void* pString,
              int iLeft,
              int iTop,
              int iFormat,
              int iCodePage,
              unsigned int* pusColorSet
            );

#define COLOR16_TRANSPARENT	0xF81F




//
// simple widget control
//
#define MAX_GFX_ARGV_FIELDS  (100)


#define GFX_ARGV_START 	GFX_ID('g','a','s',0)
#define GFX_ARGV_END 	GFX_ID('g','a','e',0)

enum WIN_ARGV{
    	WIN_X1 = GFX_ID('w','i','n',0),
    	WIN_Y1,
    	WIN_WIDTH,
    	WIN_HEIGHT,
    	WIN_TITLE
};

#define MAX_TITLE_LEN (32)

struct ts_gfx_window
{
	int _id;
	unsigned char m_surface[MAX_TITLE_LEN];
	
	int x1;
	int y1;
	int width;
	int height;
	char title[32];
};

#define GFX_WINDOW_ID GFX_ID('w','i','n',0)

int gfx_init_window(struct ts_gfx_window *ps_win,...);

void gfx_fill_window(struct ts_gfx_window *ps_win,int x1,int y1,int width,int height,int fillcolor);
int gfx_draw_window(struct ts_gfx_window *ps_win);
int gfx_flush_window(struct ts_gfx_window *ps_win,int x_offset,int y_offset,int width,int height);


//
// simple popup message 
//

#define GFX_MSGBOX_ID GFX_ID('m','s','g','b')

enum _MSGBOX_TYPE
{
    GFX_MSGBOX_POPUP = 1,
    GFX_MSGBOX_YESNO = 2,
    GFX_MSGBOX_CENTER = 4
};

enum MSG_BOX_ARGV{
    	MSGBOX_TYPE = GFX_ID('m','s','g','b'),
    	MSGBOX_STYLE,
    	MSGBOX_DELAY,
    	MSGBOX_FLIP_NUM
};

#define MAX_LINE_BUFFER 128

struct ts_gfx_msg_box
{
	struct ts_gfx_window win;
	int _id;	
	int type;
	int style;
	int delay;
	int flip_num;
	int max_msg_line;
	char msg_buffer[MAX_LINE_BUFFER];
	char *pmsg_line_array[10];
	char *p_msg_token;		
};

int gfx_init_msgbox(struct ts_gfx_msg_box *ps_msgbox,...);
int gfx_msgbox(struct ts_gfx_msg_box *ps_msgbox,char *msg);

//
// simple log viewer .
// 

#define MAX_LOG_VIEW_LINE (20)
#define MAX_LOG_LINE_STR_SIZE (100)

#define GFX_LOG_VIEW_ID GFX_ID('l','v','i',0)

struct ts_log_text_line
{
	char log_text[MAX_LOG_LINE_STR_SIZE];
	int 	text_color[3];
	int 	text_attr;
};

struct ts_gfx_log_view
{
	struct ts_gfx_window win;
	int _id;
	
	struct ts_log_text_line as_log_text_line[MAX_LOG_VIEW_LINE];
	struct ts_log_text_line *ps_log_text_line[MAX_LOG_VIEW_LINE];

	int cur_log_line;
	int max_log_line;

	int lv_x1;
	int lv_y1;
	int lv_y_space;
	int lv_width;
	int lv_height;
};

int gfx_init_logview(struct ts_gfx_log_view *ps_lv,...);
int gfx_reset_logview(struct ts_gfx_log_view *ps_lv); 
int gfx_logview_settext(struct ts_gfx_log_view *ps_lv,char *log_msg,int *textcolor) ;



//
// simple progress bar.
// 

#define GFX_PGS_ID GFX_ID('p','g','s',0)
enum _PGS_STYLE  
{
PGS_NORMAL,
PGS_CHARGE
};

enum PGS_ARGV{
    	PGS_START =GFX_PGS_ID,
    	PGS_END,
    	PGS_STEP,
	PGS_MAX,
    	PGS_CUR,
    	PGS_BAR_X1,
    	PGS_BAR_Y1,
    	PGS_BAR_WIDTH,
    	PGS_BAR_HEIGHT,
    	PGS_STYLE
};

struct ts_gfx_progressbar
{
	struct ts_gfx_window win;
	int _id;
	
	int start;
	int end;
	int prev_step;
	int step;
	int max;
	int cur;
	int bar_x1;
	int bar_y1;
	int bar_width;
	int bar_height;
	int pgs_style;
};

int gfx_init_progressbar(struct ts_gfx_progressbar *ps_pgs,...);
int gfx_draw_progressbar(struct ts_gfx_progressbar *ps_pgs,int cur);


#endif

