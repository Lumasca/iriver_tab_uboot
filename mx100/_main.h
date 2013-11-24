/********************************************************************
* mx100 main
 *
 *********************************************************************/
#ifndef MX100_MAIN_H__
#define MX100_MAIN_H__


extern unsigned char g_surface[32];

enum _TEXT_COLOR
{
COLOR_WHITE = 0,
COLOR_BLUE,
COLOR_RED
};

void mx100_sys_entry(void);
void draw_uboot_version(void);

void draw_bootlogo(void);
void init_mx100_ui(unsigned char* fb_buffer);

void set_text_display_pos(int x,int y);

void display_text(int newline,int ColorType,char *fmt,...);

#ifdef DISPLAY_BOARD_INFO
void add_board_info(int newline,char *fmt, ...);
void display_board_info(int x,int y);

#endif

enum _TEXT_COLOR_LOG
{
LOG_NOR = 0,
LOG_OK,
LOG_FAIL
};


void mx100_display_log(int type,const char *fmt, ...);
void mx100_set_progress(int percent);
void init_boot_ui(void);

// 2011.03.24
// mx100 boot menu settings impl.
//
char* mx100_get_vars(char *name);
void mx100_set_vars(char *name,char *value);
char * mx100_set_vars_toggle(char *name);
void mx100_save_boot_args(void);
void mx100_init_boot_args(void);

#ifdef DETECT_MX100_HW	
extern int g_mx100_hw_type;
char *get_mx100_hw_name(void);
#endif

#define CHECK_ID(a,b,c,d)     (((a)<<24) | ((b)<<16) | ((c)<<8) | (d))
#define CHECK_ID2(a)     (((a[0])<<24) | ((a[1])<<16) | ((a[2])<<8) | (a[3]))

#define MP_ID_PROCESS CHECK_ID('p','r','o','c')
#define MP_ID_NORMAL CHECK_ID('m','p','n','r')
#define MP_ID_DEVELOPER CHECK_ID('d','e','v','e')
#define MP_ID_USER_EMERGENCY CHECK_ID('u','s','e','r')

enum _MX100_BOOTMENU{
	MX100_BOOTMENU_MP_PROCESS =MP_ID_PROCESS,
	MX100_BOOTMENU_MP_NORMAL = MP_ID_NORMAL,
	MX100_BOOTMENU_DEVELOPER = MP_ID_DEVELOPER,
	MX100_BOOTMENU_USER_EMERGENCY = CHECK_ID('u','s','e','r'),
	MX100_BOOTMENU_NONE = CHECK_ID('n','o','n','e')
};
enum _MX100_EMERGENCH_MODE_CHECK
{
_EMERGENCY_NONE = 0,
_EMERGENCY_TRY,
_EMERGENCY_ENTER
};

int check_bootmenu_mode(void);
int get_bootmenu_mode(void);

#endif
