/********************************************************************
 *		mygdi.c
 *		created by JH.LIM		
 *		first revision .2005.05.7
 *		modifyed 	2009.2.12
 *                             2010.09
 *********************************************************************/
 #include <config.h>
 #include "mx100_inc.h"
#include "mx100_bitmap.h"

#include "_gfx.h"

int gfx_get_glyph_utf12(unsigned short usUnicode, unsigned short* ucBuffer);

sGdiObject g_sGdiObjectLCD;


#if 0
static unsigned char g_TempFontSurface[32];
//static unsigned char g_TempFontBuffer[12 * 12 * 200 * 4];
//static unsigned char g_TempFontBuffer[12 * 12 * 200 * 4];

#define SIZEOF_FONT_BUFFER (12 * 12 * 200 * 4)

static unsigned char *g_TempFontBuffer = (unsigned char*) (CFG_LCD_FBUFFER + 0x4B0000);  //must remind later.

static unsigned short g_pusGlyphBuffer[2048];
#else

static unsigned char g_TempFontSurface[32];
#define SIZEOF_FONT_BUFFER (12 * 12 * 200 * 4)
static unsigned char *g_TempFontBuffer;
static unsigned short *g_pusGlyphBuffer;

#endif


void gfx_init_gdi(unsigned char* surface,unsigned char* framebuffer,int WIDTH,int HEIGHT,int bpp)
{
    g_sGdiObjectLCD.iBGColor = GFX_RGB32(0xff,0xff,0xff);
    g_sGdiObjectLCD.iFGColor =  GFX_RGB32(0x0,0x0,0x0);
    g_sGdiObjectLCD.iFGShadeColor =  GFX_RGB32(0x66,0x66,0x66);

   g_TempFontBuffer = ir_malloc(SIZEOF_FONT_BUFFER);
   g_pusGlyphBuffer = ir_malloc(2048 * 2);

   gfx_create_bitmap(surface,framebuffer,WIDTH,HEIGHT,bpp);
   
    gfx_set_cur_surface(surface);
	
}

void gfx_create_bitmap(unsigned char *surface,unsigned char *framebuffer,int width,int height,int bpp)
{
    unsigned int pi_framebuffer = (int)framebuffer;

   if(framebuffer==0) {
 	pi_framebuffer = (int)ir_malloc(width * height * (bpp/8));
   } else {
	pi_framebuffer = (int)framebuffer;
   }
   
    surface[0] = GFX_HIBYTE(width);   
    surface[1] = GFX_LOBYTE(width);   
    surface[2] = GFX_HIBYTE(height);   
    surface[3] = GFX_LOBYTE(height); 
    surface[4] = bpp; 

    surface[7] = 0; 
    surface[8] = (unsigned char)((pi_framebuffer>>24) & 0xff);
    surface[9] = (unsigned char) ((pi_framebuffer>>16) & 0xff);
    surface[10] =(unsigned char) ((pi_framebuffer>>8) & 0xff);
    surface[11] =(unsigned char) (pi_framebuffer & 0xff);
	
}

int GFX_BM_BPP(unsigned char *hSourceBitmap)
{
int bpp;

	switch(hSourceBitmap[4])  {
	   case  GFX_BPP_1:    bpp = 1; break;
	   case  GFX_BPP_2  :  bpp = 2; break;
	   case  GFX_BPP_4  :  bpp = 4; break;
	   case  GFX_BPP_8 :   bpp = 8; break;
	   case  GFX_BPP_8RLE :  bpp = 8; break;
	   case  GFX_BPP_16 :  bpp = 16; break;
	   case  GFX_BPP_32:  bpp = 32; break;
	}
  return bpp;
}

unsigned char*  gfx_get_bmbits(unsigned char *surface)
{
	unsigned char* p_bmbits;
	
	if(surface[7] == 0) {
		p_bmbits =(unsigned char*) (((surface[8])<<24) | ((surface[9])<<16) | ((surface[10])<<8) | (surface[11]));
	} else {
		p_bmbits = (unsigned char*)&surface[(surface[8] << 8) | surface[9] ];
	}
	
	return p_bmbits;
}



inline int read_data_8(char** data_buffer)
{
	int Value;
	Value = **data_buffer;
	(*data_buffer)++;
	return Value;
}
unsigned short read_data_16(char** data_buffer)
{
    unsigned int iValue;
    iValue = read_data_8(data_buffer);
    iValue |= read_data_8(data_buffer) << 8;
    return (unsigned short)iValue;
}

unsigned int read_data_32(char** data_buffer)
{
    unsigned int iValue;
    iValue = read_data_8(data_buffer);
    iValue |= read_data_8(data_buffer) << 8;
    iValue |= read_data_8(data_buffer) << 16;
    iValue |= read_data_8(data_buffer) << 24;
    return iValue;
}

int is_data_eof(char** data_buffer)
{
	return 0;
}

#define EOF -1

// support rle8 bmp image.
static void read_rle8_bmp(unsigned char *surface,unsigned char *buf)
{
   int count;
   unsigned char val;
   unsigned char val0;
   int j, pos, line;
   int eolflag, eopicflag;
   int bm_width,bm_height;
   unsigned char* pbm_bits = gfx_get_bmbits(surface);
   unsigned char **f = &pbm_bits;

  bm_width = GFX_BM_WIDTH(surface);
  bm_height = GFX_BM_HEIGHT(surface);
   
   eopicflag = 0;
   line = bm_height - 1;

   while (eopicflag == 0) {
      pos = 0;                
      eolflag = 0;            

      while ((eolflag == 0) && (eopicflag == 0)) {
         count = read_data_8(f);
         if (count == EOF)
            return;
         val = read_data_8(f);

         if (count > 0) {   
            for (j = 0; j < count; j++) {
               buf[line *  bm_width + pos] = val;
               pos++;
            }
         }
         else {
            switch (val) {

               case 0:         
                  eolflag = 1;
                  break;

               case 1:         
                  eopicflag = 1;
                  break;

               case 2:         
                  count = read_data_8(f);
                  if (count == EOF)
                     return;
                  val = read_data_8(f);
                  pos += count;
                  line -= val;
                  break;

               default:          
                  for (j=0; j<val; j++) {
                     val0 = read_data_8(f);
                     buf[line * bm_width + pos] = val0;
                     pos++;
                  }

                  if (j % 2 == 1)
                     val0 = read_data_8(f); 

                  break;
            }
         }

         if (pos - 1 > (int)bm_width)
            eolflag = 1;
      }

      line--;
      if (line < 0)
         eopicflag = 1;
   }
}

void gfx_prepare_bitmap(unsigned char *bitmap)
{
    unsigned int pi_bmbits;
   int bm_width,bm_height;
   int bpp;
   
  bm_width = GFX_BM_WIDTH(bitmap);
  bm_height = GFX_BM_HEIGHT(bitmap);
   bpp = GFX_BM_BPP(bitmap);
   
  if(bitmap[4] == GFX_BPP_8RLE) {
    	    pi_bmbits = (int)ir_malloc(bm_width * bm_height * (bpp/8) +32);
   
      read_rle8_bmp(bitmap,pi_bmbits);
    bitmap[4] = GFX_BPP_8; 
    bitmap[7] = 0; 
    bitmap[8] = (unsigned char)((pi_bmbits>>24) & 0xff);
    bitmap[9] = (unsigned char) ((pi_bmbits>>16) & 0xff);
    bitmap[10] =(unsigned char) ((pi_bmbits>>8) & 0xff);
    bitmap[11] =(unsigned char) (pi_bmbits & 0xff);
  } else {

  }
}



#define dALPHA_MASK1 0x03e0
#define dALPHA_MASK2 0x03e0

#define ET_GETR16(c) (((c) >> 11) & 0x1F)
#define ET_GETG16(c)  (((c) >> 5) & 0x3F )
#define ET_GETB16(c)  ((c) & 0x1F)

unsigned short gfx_get_alpha_color(unsigned short ucColor1,unsigned short ucColor2)
{
    unsigned short sR,sG,sB,sRR,sGG,sBB,sBgR,sBgG,sBgB;
    
    sR = ET_GETR16(ucColor1);
    sG = ET_GETG16(ucColor1);
    sB =  ET_GETB16(ucColor1);
    
    sBgR = ET_GETR16(ucColor2);
    sBgG = ET_GETG16(ucColor2);
    sBgB =  ET_GETB16(ucColor2);


    sRR = (sR + sBgR) >> 1;
    sRR &= 0x1F;

    sGG = (sG + sBgG) >> 1;
    sGG &= 0x3F;


    sBB = (sB + sBgB) >>1;
    sBB &= 0x1F;
    
    return (sBB + (sGG << 5) + (sRR <<11));
}

void gfx_draw_fill_rect(  unsigned char *psurface,
                           int iLeft,
                           int iTop,
                           int iWidth,
                           int iHeight,
                           int iColor)
                           
                           
{
    int    x,y;
    unsigned int*   pDstImageBits32;
    unsigned int*   pDstImageBitsVar32;
    unsigned int     ucDstImageWidth;
    
    
    pDstImageBits32   = (unsigned int*)gfx_get_bmbits(psurface);
    ucDstImageWidth   = GFX_BM_WIDTH(psurface);
    
    for (y = iTop; y <iTop + iHeight ; y++) {
        pDstImageBitsVar32 = &pDstImageBits32[y * ucDstImageWidth + iLeft];                   
        for (x = 0; x < iWidth;x++) {
            *pDstImageBitsVar32++ = iColor;
        }
    } 
}

void gfx_draw_pixel( unsigned  char* psurface ,int iX,int iY,int iColor)
{
    unsigned int*   pDstImageBits32;
    unsigned int     ucDstImageWidth;

    pDstImageBits32   = (unsigned int*)gfx_get_bmbits(psurface);
    ucDstImageWidth   = GFX_BM_WIDTH(psurface);
	
   pDstImageBits32[iY * ucDstImageWidth + iX] = iColor;  
}


__inline void _gfx_draw_hline(  unsigned int *pDstImageBits32,int usDstImageWidth,int iX1,int iX2,int iY,int iColor)
{
    int i,temp;
    
    if (iX1 > iX2) {
        temp = iX1;
        iX1 = iX2;
        iX2 = temp;
    }
    
    if (iX1 < 0)
        iX1 = 0;
    
    pDstImageBits32 = &pDstImageBits32[iY * usDstImageWidth + iX1];              
    for(i=0;i<iX2-iX1;i++) {
        *pDstImageBits32++ = iColor;
    }      
}
  
__inline void _gfx_draw_vline( unsigned int *pDstImageBits32,int usDstImageWidth,int iX,int iY1,int iY2,int iColor)
{
    int j,temp;
    
    if (iY1 > iY2) {
        temp = iY1;
        iY1 = iY2;
        iY2 = temp;
    }
    
    if (iY1 < 0)
        iY1 = 0;
    
    pDstImageBits32 = &pDstImageBits32[iY1 * usDstImageWidth];    

    for(j=0;j<iY2-iY1 + 1;j++) {
        pDstImageBits32[j * usDstImageWidth + iX] = iColor;  
    }
}


void gfx_draw_rect( unsigned char *psurface,int iX1,int iY1,int iX2,int iY2,int iColor)
{
    unsigned int* pDstImageBits32;
    int    usDstImageWidth;    
    pDstImageBits32   = (unsigned int*)gfx_get_bmbits(psurface);
    usDstImageWidth   = GFX_BM_WIDTH(psurface);

       
    _gfx_draw_hline(pDstImageBits32,usDstImageWidth, iX1, iX2, iY1,iColor);
    _gfx_draw_hline(pDstImageBits32,usDstImageWidth, iX1, iX2, iY2,iColor);
    _gfx_draw_vline(pDstImageBits32,usDstImageWidth, iX1, iY1, iY2,iColor);
    _gfx_draw_vline(pDstImageBits32,usDstImageWidth, iX2, iY1, iY2,iColor);
}

void gfx_draw_bitmap(unsigned char* p_dst_surface,
                       int iLeft,
                       int iTop,
                       unsigned char* hSourceBitmap, 
                       int iSourceLeft,
                       int iSourceTop,
                       int iSourceCopydWidth,
                       int iSourceCopydHeight,
                       int iRop)
{
    int iX,iY;
    unsigned char*   pucSourceImageBits8;
    unsigned char*   pucSourceImageBitsVar8;
	
    unsigned short*   pusSourceImageBits16;
    unsigned short*   pusSourceImageBitsVar16;

    unsigned int*   pusSourceImageBits32;
    unsigned int*   pusSourceImageBitsVar32;

    unsigned int*   pSrcPallete32;
//    unsigned short*   pSrcPallete16;

    int iSourceBitmapWidth,iSourceBitmapHeight;
    int iSourceImageBPP;
    int iRight,iBottom;
    
    unsigned short*   pusFrameBufferBits16;
    unsigned short*   pusFrameBufferBitsVar16;  

    unsigned int*   pusFrameBufferBits32;
    unsigned int*   pusFrameBufferBitsVar32;  

    int iFrameBufferWidth;

    int iDestImageBPP;
	
    int iSourceImageBPL;
    int    ucTransParencyIndex;

    if(!hSourceBitmap) return;
    
    iSourceImageBPP= GFX_BM_BPP(hSourceBitmap);
    iDestImageBPP= GFX_BM_BPP(p_dst_surface);
    

	if(iSourceCopydWidth==0) {
        iSourceCopydWidth = GFX_BM_WIDTH(hSourceBitmap);
    }
    if(iSourceCopydHeight==0) {
        iSourceCopydHeight = GFX_BM_HEIGHT(hSourceBitmap);
    }


    iFrameBufferWidth     = GFX_BM_WIDTH(p_dst_surface);    

 
        iRight = iLeft + iSourceCopydWidth - 1;
        iBottom = iTop + iSourceCopydHeight - 1;
        



        switch(iSourceImageBPP) {
	case 8:
           pucSourceImageBits8 = (unsigned char*)gfx_get_bmbits(hSourceBitmap);
           ucTransParencyIndex = GFX_BM_TRANSPARENCY_INDEX(hSourceBitmap);
	   iSourceImageBPL = GFX_BM_BPL	(hSourceBitmap);
		
	 if(iDestImageBPP == 32) {
		   pSrcPallete32 =  GFX_BM_PAL32(hSourceBitmap);	

		   pusFrameBufferBits32   = (unsigned int*)gfx_get_bmbits(p_dst_surface);
	 	
	           if(iRop & GFX_ROP_TRANSPARENCY) {			   	
	                for (iY = 0; iY < iSourceCopydHeight ; iY++) {
	                    pusFrameBufferBitsVar32 = &pusFrameBufferBits32[(iY + iTop)*iFrameBufferWidth + iLeft];                
	                    pucSourceImageBitsVar8 = &pucSourceImageBits8[(iY + iSourceTop) * iSourceImageBPL + iSourceLeft];

	                    for (iX = iSourceLeft; iX < iSourceLeft + iSourceCopydWidth;iX++) {
	                        if(*pucSourceImageBits8 == ucTransParencyIndex) {
		                        pucSourceImageBits8++;
	        	                pusFrameBufferBitsVar32++;   
	                        } else {
	                            *pusFrameBufferBitsVar32++ = pSrcPallete32 [*pucSourceImageBits8++];
	                        }	                        	   
	                    }
	                }     
	                    
	           } else if(iRop & GFX_ROP_COPY) {
	                for (iY = 0; iY < iSourceCopydHeight ; iY++) {
	                    pusFrameBufferBitsVar32 = &pusFrameBufferBits32[(iY + iTop)*iFrameBufferWidth + iLeft];                
	                    pucSourceImageBitsVar8 = &pucSourceImageBits8[(iY + iSourceTop) * iSourceImageBPL + iSourceLeft];
						
	                    for (iX = iSourceLeft; iX < iSourceLeft + iSourceCopydWidth;iX++) {
	                        *pusFrameBufferBitsVar32++ = pSrcPallete32 [*pucSourceImageBits8++];
	                    }
	                }  
	           }            

            }
        
        break;		
        case 16:
            pusSourceImageBits16 = (unsigned short*)gfx_get_bmbits(hSourceBitmap);
            iSourceBitmapWidth = GFX_BM_WIDTH(hSourceBitmap);
            iSourceBitmapHeight = GFX_BM_HEIGHT(hSourceBitmap);

            if(iDestImageBPP == 16) {
		    pusFrameBufferBits16   =(unsigned short*) gfx_get_bmbits(p_dst_surface);
				
	           if(iRop & GFX_ROP_TRANSPARENCY) {			   	
	                for (iY = 0; iY < iSourceCopydHeight ; iY++) {
	                    pusFrameBufferBitsVar16 = &pusFrameBufferBits16[(iY + iTop)*iFrameBufferWidth + iLeft];                
	                    pusSourceImageBitsVar16 = &pusSourceImageBits16[(iY + iSourceTop) * iSourceBitmapWidth + iSourceLeft];

	                    for (iX = iSourceLeft; iX < iSourceLeft + iSourceCopydWidth;iX++) {
	                        if(*pusSourceImageBitsVar16 == COLOR16_TRANSPARENT) {
		                        pusSourceImageBitsVar16++;
	        	                pusFrameBufferBitsVar16++;   
	                        } else {
	                            *pusFrameBufferBitsVar16++ = *pusSourceImageBitsVar16++;
	                        }
	                        
	   
	                    }
	                }     
	                    
	           } else if(iRop & GFX_ROP_COPY) {
	                for (iY = 0; iY < iSourceCopydHeight ; iY++) {
	                    pusSourceImageBitsVar16 = &pusSourceImageBits16[(iY + iSourceTop) * iSourceBitmapWidth + iSourceLeft];
	                    pusFrameBufferBitsVar16 = &pusFrameBufferBits16[(iY + iTop)*iFrameBufferWidth + iLeft];                
						
	                    for (iX = iSourceLeft; iX < iSourceLeft + iSourceCopydWidth;iX++) {
	                        *pusFrameBufferBitsVar16++ = *pusSourceImageBitsVar16++;
	                    }
	                }  
	           }
            }
	    else if(iDestImageBPP == 32) {
		   pusFrameBufferBits32   = (unsigned int*)gfx_get_bmbits(p_dst_surface);
			
	           if(iRop & GFX_ROP_TRANSPARENCY) {			   	
	                for (iY = 0; iY < iSourceCopydHeight ; iY++) {
	                    pusFrameBufferBitsVar32 = &pusFrameBufferBits32[(iY + iTop)*iFrameBufferWidth + iLeft];                
	                    pusSourceImageBitsVar16 = &pusSourceImageBits16[(iY + iSourceTop) * iSourceBitmapWidth + iSourceLeft];

	                    for (iX = iSourceLeft; iX < iSourceLeft + iSourceCopydWidth;iX++) {
	                        if(*pusSourceImageBitsVar16 == COLOR16_TRANSPARENT) {
		                        pusSourceImageBitsVar16++;
	        	                pusFrameBufferBitsVar32++;   
	                        } else {
	                            *pusFrameBufferBitsVar32++ = *pusSourceImageBitsVar16++;
	                        }	                        	   
	                    }
	                }     
	                    
	           } else if(iRop & GFX_ROP_COPY) {
	                for (iY = 0; iY < iSourceCopydHeight ; iY++) {
	                    pusSourceImageBitsVar16 = &pusSourceImageBits16[(iY + iSourceTop) * iSourceBitmapWidth + iSourceLeft];
	                    pusFrameBufferBitsVar32 = &pusFrameBufferBits32[(iY + iTop)*iFrameBufferWidth + iLeft];                
						
	                    for (iX = iSourceLeft; iX < iSourceLeft + iSourceCopydWidth;iX++) {
	                        *pusFrameBufferBitsVar32++ = *pusSourceImageBitsVar16++;
	                    }
	                }  
	           }            

            }
        case 32:
            pusSourceImageBits32 = (unsigned int*)gfx_get_bmbits(hSourceBitmap);
            iSourceBitmapWidth = GFX_BM_WIDTH(hSourceBitmap);
            iSourceBitmapHeight = GFX_BM_HEIGHT(hSourceBitmap);

		if(iDestImageBPP == 32) {
		   pusFrameBufferBits32   = (unsigned int*)gfx_get_bmbits(p_dst_surface);
			
	           if(iRop & GFX_ROP_TRANSPARENCY) {			   	
	                for (iY = 0; iY < iSourceCopydHeight ; iY++) {
	                    pusFrameBufferBitsVar32 = &pusFrameBufferBits32[(iY + iTop)*iFrameBufferWidth + iLeft];                
	                    pusSourceImageBitsVar32 = &pusSourceImageBits32[(iY + iSourceTop) * iSourceBitmapWidth + iSourceLeft];

	                    for (iX = iSourceLeft; iX < iSourceLeft + iSourceCopydWidth;iX++) {
	                        if(*pusSourceImageBitsVar32 == COLOR16_TRANSPARENT) {
		                        pusSourceImageBitsVar32++;
	        	                pusFrameBufferBitsVar32++;   
	                        } else {
	                            *pusFrameBufferBitsVar32++ = *pusSourceImageBitsVar32++;
	                        }	                        	   
	                    }
	                }     
	                    
	           } else if(iRop & GFX_ROP_COPY) {
	                for (iY = 0; iY < iSourceCopydHeight ; iY++) {
	                    pusSourceImageBitsVar32 = &pusSourceImageBits32[(iY + iSourceTop) * iSourceBitmapWidth + iSourceLeft];
	                    pusFrameBufferBitsVar32 = &pusFrameBufferBits32[(iY + iTop)*iFrameBufferWidth + iLeft];                
						
	                    for (iX = iSourceLeft; iX < iSourceLeft + iSourceCopydWidth;iX++) {
	                        *pusFrameBufferBitsVar32++ = *pusSourceImageBitsVar32++;
	                    }
	                }  
	           }            

            }

            break;
      }
      

    
}

const unsigned short
CP1252_UNICODE[]= { 0x20AC, 0x0020, 0x201A, 0x0192, 0x201E, 0x2026, 0x2020, 0x2021,
                    0x02C6, 0x2030, 0x0160, 0x2039, 0x0152, 0x0020, 0x017D, 0x0020,
                    0x0020, 0x2018, 0x2019, 0x201C, 0x201D, 0x2022, 0x2013, 0x2014,
                    0x02DC, 0x2122, 0x0161, 0x203A, 0x0153, 0x0020, 0x017E, 0x0178,
                    0x00A0, 0x00A1, 0x00A2, 0x00A3, 0x00A4, 0x00A5, 0x00A6, 0x00A7,
                    0x00A8, 0x00A9, 0x00AA, 0x00AB, 0x00AC, 0x00AD, 0x00AE, 0x00AF,
                    0x00B0, 0x00B1, 0x00B2, 0x00B3, 0x00B4, 0x00B5, 0x00B6, 0x00B7,
                    0x00B8, 0x00B9, 0x00BA, 0x00BB, 0x00BC, 0x00BD, 0x00BE, 0x00BF,
                    0x00C0, 0x00C1, 0x00C2, 0x00C3, 0x00C4, 0x00C5, 0x00C6, 0x00C7,
                    0x00C8, 0x00C9, 0x00CA, 0x00CB, 0x00CC, 0x00CD, 0x00CE, 0x00CF,
                    0x00D0, 0x00D1, 0x00D2, 0x00D3, 0x00D4, 0x00D5, 0x00D6, 0x00D7,
                    0x00D8, 0x00D9, 0x00DA, 0x00DB, 0x00DC, 0x00DD, 0x00DE, 0x00DF,
                    0x00E0, 0x00E1, 0x00E2, 0x00E3, 0x00E4, 0x00E5, 0x00E6, 0x00E7,
                    0x00E8, 0x00E9, 0x00EA, 0x00EB, 0x00EC, 0x00ED, 0x00EE, 0x00EF,
                    0x00F0, 0x00F1, 0x00F2, 0x00F3, 0x00F4, 0x00F5, 0x00F6, 0x00F7,
                    0x00F8, 0x00F9, 0x00FA, 0x00FB, 0x00FC, 0x00FD, 0x00FE, 0x00FF };

const unsigned short
ASCII_ETC[] = {     0x20AC, 0x201A, 0x0192, 0x201E, 0x2026, 0x2020, 0x2021, 0x02C6,
                    0x2030, 0x2039, 0x2018, 0x2019, 0x201C, 0x201D, 0x2022, 0x2013,
                    0x2014, 0x02DC, 0x2122, 0x203A, 0x02C7, 0x02D8, 0x02DB, 0x2116,
                    0x02D9 };


 
static int
CP1252code2Unicode(char *cCP1252Code, unsigned short *usString, int StrLength)
{
    int i, j;

    for(i = 0, j = 0; (i < StrLength);i++)
    {
        if(!cCP1252Code[i]) break;

        if(cCP1252Code[i] < 0x80)
        {
            usString[j++] = (unsigned short)cCP1252Code[i];
        }
        else
        {
            usString[j++] = CP1252_UNICODE[cCP1252Code[i] - 0x80];
        }
    }
    return(j);
}

int
gfx_draw_string(
		unsigned char*    p_dst_surface,
		void* pString,
              int iLeft,
              int iTop,
              int iFormat,
              int iCodePage,
              unsigned int* pusColorSet
            )
{
    int i,j;

    char *szString = (char*) pString;
	
    unsigned short* wszString;
    unsigned short wUnicode;
    
    unsigned char iGlyphWidth,iGlyphHeight;
    

    unsigned short usString[256];
    unsigned int usBackColor,usFontColor,usFontShadeColor;
    int iCount,iGlyphWidthCount=0;
    
    unsigned int*   pusFrameBufferBits;
    int iFrameBufferWidth;
    
    unsigned int* pusGlyphDoubleBuffer;
    unsigned int* pusGlyphDoubleBufferVar;
    
    
    iGlyphHeight = 12;
    
    if(!pString) {
//	printf("%d err\n",__LINE__);
		
        return 0;   // NULL String
    }
	#if 0    
   	if(iCodePage == CP_UNICODE)  { 
        wszString = (unsigned short*) pString;
   	}
   	else 
	#endif
	{
	       	wszString = usString;
	       	memset((unsigned char *)wszString, 0, sizeof(usString));
		//       (pfmbtowc[iCodePage])((char*)pString, wszString, 64);
		CP1252code2Unicode((char*)szString, wszString, 256);
   	}
    
    iCount = 0;

	
    while(1)
    {
        wUnicode = wszString[iCount];
//	printf("%c %d %d\n",szString[iCount],szString[iCount],wUnicode);

        if(!(wUnicode))  {
	//	printf("%d err %d %d \n",__LINE__,iCount,iGlyphWidthCount );
		
		break;
        }
	if(iFormat & TF_GET_STRING_WIDTH) {
	        iGlyphWidth = gfx_get_glyph_utf12(wUnicode,0);
	} else {
	        iGlyphWidth = gfx_get_glyph_utf12(wUnicode,(unsigned short*)&g_pusGlyphBuffer[iGlyphWidthCount]);
	}
        iGlyphWidthCount += iGlyphWidth + 1;
	iCount ++;
    }

    if(iFormat & TF_GET_STRING_WIDTH) {
	return iGlyphWidthCount;
    }
   
    pusFrameBufferBits   =  (unsigned int*)gfx_get_bmbits(p_dst_surface);
    iFrameBufferWidth     = GFX_BM_WIDTH(p_dst_surface); 

   gfx_create_bitmap(g_TempFontSurface,g_TempFontBuffer,iGlyphWidthCount,iGlyphHeight,GFX_BPP_32);

   memset(g_TempFontBuffer	,0x0,iGlyphWidthCount * iGlyphHeight * 4);  // fix me later.

    pusGlyphDoubleBuffer =  (unsigned int *)gfx_get_bmbits(g_TempFontSurface);
    
    usFontColor = pusColorSet[0];
    usFontShadeColor = pusColorSet[1];
    usBackColor= pusColorSet[2];
    
    pusGlyphDoubleBufferVar = pusGlyphDoubleBuffer;
    
    if(iFormat & TF_TRANSPARENT) {

       
    } else {        
        
        for(j = 0; j < iGlyphHeight; j ++)
        {
            for(i = 0; i < iGlyphWidthCount; i ++)
            {
                *pusGlyphDoubleBufferVar++ = usBackColor;
            }
        }
    }
    
    pusGlyphDoubleBufferVar = pusGlyphDoubleBuffer;
    
    if(iFormat & TF_SHADE) {
        
        for(j = 0; j < iGlyphHeight; j ++)    {
            for(i = 0; i < iGlyphWidthCount; i++)    {
                if((g_pusGlyphBuffer[i]>>j)&0x1) {
                    if(i < iGlyphWidthCount-1) {
                        *(pusGlyphDoubleBufferVar+1) = usFontShadeColor;
                    }
                    *pusGlyphDoubleBufferVar = usFontColor;
                }
                pusGlyphDoubleBufferVar++;
            }
        }
    }
    else {
        
        for(j = 0; j < iGlyphHeight; j ++)    {
            for(i = 0; i < iGlyphWidthCount; i++)   {
                if((g_pusGlyphBuffer[i]>>j)&0x1) {
                    *pusGlyphDoubleBufferVar = usFontColor;
                }
                pusGlyphDoubleBufferVar++;
            }
        }
    }
    gfx_draw_bitmap( p_dst_surface,
        iLeft,
        iTop,
	g_TempFontSurface,        
        0,
        0,
        iGlyphWidthCount,
        iGlyphHeight,
        GFX_ROP_COPY);
    
    
    
    
    return iGlyphWidthCount;
}

#include "ascii.h"


int gfx_get_glyph_utf12(unsigned short usUnicode, unsigned short* ucBuffer)
{
    unsigned char w;
    unsigned short* pusGlyph; 
    if((usUnicode < 0x180) || ((usUnicode >= 0xff00) && (usUnicode <=0xff60)))
    {
        if(usUnicode >= 0xff00) {
            pusGlyph =(unsigned short *) &ascii_font2[usUnicode - 0xff00];   
        } else {
            pusGlyph =(unsigned short *) &ascii_font1[usUnicode];
        }
        
        w = (unsigned char)pusGlyph[0];
        pusGlyph++;
	if(ucBuffer)  memcpy(ucBuffer,pusGlyph,24);  
        
        return (w);
    }

   return 0;
}


//
// simple control
//
int _gfx_init_window(struct ts_gfx_window *ps_win,va_list list)
{
    int i;

    int ArgvType = -1;
     	
    if(!ps_win) return 0;
	
		
    for(i=0;i < MAX_GFX_ARGV_FIELDS;i++) {
        ArgvType= va_arg(list,int);
		
        if(ArgvType == GFX_ARGV_END)
        {                
                goto End;           
        }
	        switch(ArgvType)
	        {
	           	case WIN_X1: 	ps_win->x1 = (int)va_arg(list,int); break;
		    	case WIN_Y1:  ps_win->y1 = (int)va_arg(list,int); break;
		    	case WIN_WIDTH:  ps_win->width = (int)va_arg(list,int); break;
		    	case WIN_HEIGHT:  ps_win->height = (int)va_arg(list,int); break;
		    	case WIN_TITLE:  
			{
				char *title = (char*)va_arg(list,char *);
				if(strlen(title) < MAX_TITLE_LEN) {
					strcpy(ps_win->title ,title);
				}
		    	}
			break;
		        default:   break;
	        }
	}  
	
End:

    return 0;
}


int gfx_init_window(struct ts_gfx_window *ps_win,...)
{
    int i;

    int ArgvType = -1;
     	
    va_list list;	    

    if(!ps_win) return 0;
	
    va_start(list,ps_win); 

   _gfx_init_window(ps_win,list);	

    va_end(list);

   ps_win->_id = GFX_WINDOW_ID;

    gfx_create_bitmap(ps_win->m_surface,0,ps_win->width,ps_win->height,GFX_BPP_32);

    return 0;
}


void gfx_fill_window(struct ts_gfx_window *ps_win,int x1,int y1,int width,int height,int fillcolor)
{
	if(ps_win->_id!=GFX_WINDOW_ID) 	return;

	gfx_draw_fill_rect(  ps_win->m_surface,
                           x1,
                           y1,
                           width,
                           height,
                           fillcolor
                           );
}

int gfx_draw_window(struct ts_gfx_window *ps_win)
{
	if(ps_win->_id!=GFX_WINDOW_ID) 	return;
	#if 0
	gfx_draw_fill_rect(ps_win->m_surface,0,0,ps_win->width,ps_win->height,GFX_RGB32(0x66,0x66,0x66));
	#else
	gfx_draw_fill_rect(ps_win->m_surface,0,0,ps_win->width,ps_win->height,GFX_RGB32(0x0,0x0,0x0));
	gfx_draw_rect(ps_win->m_surface,0,0,ps_win->width-1,ps_win->height-1,GFX_RGB32(0xFF,0xFF,0xFF));
	#endif
	
	return 0;	
}

int gfx_flush_window(struct ts_gfx_window *ps_win,int x_offset,int y_offset,int width,int height)
{
	int src_width,src_height;
	unsigned char *p_screen_surface =  gfx_get_screen_surface();
	if(ps_win->_id!=GFX_WINDOW_ID) 	return 0;

	gfx_draw_bitmap(p_screen_surface,
                     ps_win->x1, //int iLeft,
                      ps_win->y1, //int iTop,
                      ps_win->m_surface,
                      x_offset, //int iSourceLeft,
                      y_offset, //int iSourceTop,
                       width,//int iSourceCopydWidth,
                      height,// int iSourceCopydHeight,
                      GFX_ROP_COPY //int iRop
                       );	

   if(width==0) {
        src_width = GFX_BM_WIDTH(ps_win->m_surface);
    }
    if(height==0) {
        src_height= GFX_BM_HEIGHT(ps_win->m_surface);
    }

  // gfx_draw_rect(ps_win->m_surface,x_offset,y_offset,src_width -1,src_height-1,GFX_RGB32(0xFF,0x0,0x0));

  return 0;
}

//
// simple popup message box.
//


int gfx_init_msgbox(struct ts_gfx_msg_box *ps_msgbox,...)
{
   struct ts_gfx_window *ps_win = &ps_msgbox->win;

    int i;
	
    int ArgvType = -1;
     	
    va_list list;	    

    if(!ps_msgbox) return 0;

    va_start(list,ps_msgbox); 

    gfx_init_window(ps_win,list);
   	
    va_start(list,ps_msgbox); 
		
    for(i=0;i < MAX_GFX_ARGV_FIELDS;i++) {
        ArgvType= va_arg(list,int);
        if(ArgvType == GFX_ARGV_END)   goto End;  
		
	        switch(ArgvType)
	        {
		    	case MSGBOX_TYPE: ps_msgbox->type  = (int)va_arg(list,int); break;
		    	case MSGBOX_STYLE: ps_msgbox->style  = (int)va_arg(list,int); break;
		    	case MSGBOX_DELAY: ps_msgbox->delay  = (int)va_arg(list,int); break;
		    	case MSGBOX_FLIP_NUM: ps_msgbox->flip_num = (int)va_arg(list,int); break;
		        default:    break;
	        }
        }  
	
End:
    va_end(list);


	if(ps_msgbox->style & GFX_MSGBOX_CENTER) {
		ps_win->x1 = 	(MX100_LCD_WIDTH- ps_win->width) / 2;
		ps_win->y1 = 	(MX100_LCD_HEIGHT - ps_win->height) / 2;
	}

	
    return 0;
}

int gfx_msgbox(struct ts_gfx_msg_box *ps_msgbox,char *msg)
{
	int i,j;
   	struct ts_gfx_window *ps_win = &ps_msgbox->win;
	
	unsigned int ColorSet[] = { GFX_RGB32(0xff,0xff,0xff),GFX_RGB32(0xdd,0xdd,0xdd),GFX_RGB32(0x0,0x0,0x0) ,
						GFX_RGB32(0x0,0x0,0xff),GFX_RGB32(0x0,0x0,0xdd),GFX_RGB32(0x0,0x0,0x0) ,
						  GFX_RGB32(0xff,0x0,0x0),GFX_RGB32(0xdd,0x0,0x0),GFX_RGB32(0x0,0x0,0x0)};

	if(ps_msgbox->type ==0) ps_msgbox->type = GFX_MSGBOX_POPUP;
	
	if(ps_msgbox->type ==GFX_MSGBOX_POPUP) {
		strcpy(ps_msgbox->msg_buffer,msg);
			
		ps_msgbox->max_msg_line = 0;

		  ps_msgbox->p_msg_token = strtok( ps_msgbox->msg_buffer, "\n");

	   	  if(ps_msgbox->p_msg_token)  {
			   ps_msgbox->pmsg_line_array[ps_msgbox->max_msg_line] = ps_msgbox->p_msg_token;
		   
			   ps_msgbox->max_msg_line++;

			   while( ps_msgbox->p_msg_token = strtok( NULL, "\n"))
			   {
			         ps_msgbox->pmsg_line_array[ps_msgbox->max_msg_line] = ps_msgbox->p_msg_token;
				   ps_msgbox->max_msg_line++;
			   }
	   	  }
		
		for(i=0;i<ps_msgbox->flip_num*2;i++) {
			
			gfx_draw_fill_rect(  ps_win->m_surface,
		                   0,
		                   0,
		                   ps_win->width,
		                   ps_win->height,
		                   GFX_RGB32(0x0,0x0,0x0)
		                   );

			if(i%2==1) {

				for(j=0;j<ps_msgbox->max_msg_line;j++) {
					gfx_draw_string(
						ps_win->m_surface,
						ps_msgbox->pmsg_line_array[j],
				              20,
				              j*16 + 40,
				              TF_SHADE | TF_TRANSPARENT,
				              0,
				              &ColorSet[0]	);		
				}
			}

			gfx_flush_window(ps_win,0,0,0,0);

			udelay(1000 * ps_msgbox->delay);
			
		}

		gfx_fill_window(  ps_win,
	                      ps_win->x1,
	                      ps_win->y1,
	                      ps_win->width,
	                      ps_win->height,
	                       GFX_RGB32(0x0,0x0,0x0)
	                       );
	}
	
	gfx_flush_window(ps_win,0,0,0,0);
	

}


//
// simple progress bar.
// 

int gfx_init_progressbar(struct ts_gfx_progressbar *ps_pgs,...)
{
   struct ts_gfx_window *ps_win = &ps_pgs->win;

    int i;
	
    int ArgvType = -1;
     	
    va_list list;	    

    if(!ps_pgs) return 0;

    va_start(list,ps_pgs); 

    gfx_init_window(ps_win,list);
   	
    va_start(list,ps_pgs); 

   for(i=0;i < MAX_GFX_ARGV_FIELDS;i++) {
        ArgvType= va_arg(list,int);
        if(!ArgvType || ArgvType == GFX_ARGV_END)   goto End;  
		
	        switch(ArgvType)
	        {
		    	case PGS_START: 	ps_pgs->start  = (int)va_arg(list,int); break;
		    	case PGS_END: 	ps_pgs->end  = (int)va_arg(list,int); break;
		    	case PGS_STEP: 	ps_pgs->step  = (int)va_arg(list,int); break;
		    	case PGS_STYLE: 	ps_pgs->pgs_style  = (int)va_arg(list,int); break;
				
		        default:    break;
	        }
   }  
  

End:
    va_end(list);

	ps_pgs->_id =GFX_PGS_ID;	
	ps_pgs->max = ps_pgs->end-ps_pgs->start;
	ps_pgs->cur =ps_pgs->start;	

	if(ps_pgs->pgs_style == PGS_CHARGE){	
		ps_pgs->bar_x1 = 0;
		ps_pgs->bar_y1= 0;
		ps_pgs->bar_width = ps_win->width;
		ps_pgs->bar_height= ps_win->height;	
	} else {
		ps_pgs->bar_x1 = 10;
		ps_pgs->bar_y1= 10;
		ps_pgs->bar_width = ps_win->width - 20;
		ps_pgs->bar_height= ps_win->height - 20;	
	}
   return 0;
}

#define BATT_PLUS_POLL_LEFT  /* 2011.05.18 JHLIM  */

int gfx_draw_progressbar(struct ts_gfx_progressbar *ps_pgs,int cur)
{
	struct ts_gfx_window *ps_win = &ps_pgs->win;

	int cur_bar_width;
	int cur_step;
	
	if(ps_pgs->_id!=GFX_PGS_ID) 	return 0;

	if(cur == 0 ||cur == -1) {
		ps_pgs->prev_step = -1;
		if(ps_pgs->pgs_style == PGS_CHARGE){	
			gfx_draw_bitmap(ps_win->m_surface,
                      0,
                     0,
                     (unsigned char*) pImg_BATTERY_BG,
                      0, //int iSourceLeft,
                      0, //int iSourceTop,
                       0,//int iSourceCopydWidth,
                      0,// int iSourceCopydHeight,
                      GFX_ROP_COPY //int iRop
                       );	
//			gfx_draw_rect(ps_win->m_surface,0,0,ps_win->width-1,ps_win->height-1,GFX_RGB32(0xFF,0xFF,0xFF));

		} else {
			gfx_draw_window(ps_win);
		}
		
		gfx_flush_window(ps_win,0,0,0,0);
		
	} else {

		if(cur<ps_pgs->start) cur=ps_pgs->start;
		if(cur>ps_pgs->end) cur=ps_pgs->end;

		cur_step = (cur-ps_pgs->start) /ps_pgs->step;
		

		if(cur_step != ps_pgs->prev_step) {
			ps_pgs->cur = cur;
			ps_pgs->prev_step = cur_step;
			cur_bar_width = ((ps_pgs->bar_width * (cur-ps_pgs->start)) / ps_pgs->max);
//			printf("cur_step : %d\n",cur_step);
			if(ps_pgs->pgs_style == PGS_CHARGE){	
				int i,bar_ypos;

				gfx_draw_bitmap(ps_win->m_surface,
	                      0,
	                     0,
	                     (unsigned char*) pImg_BATTERY_BG,
	                      0, //int iSourceLeft,
	                      0, //int iSourceTop,
	                       0,//int iSourceCopydWidth,
	                      0,// int iSourceCopydHeight,
	                      GFX_ROP_COPY //int iRop
	                       );	

				if(cur_step==1) {
					#ifdef BATT_PLUS_POLL_LEFT		
				        bar_ypos =  GFX_BM_HEIGHT(pImg_CHARGE_0)-12;
					#else
				        bar_ypos =  ps_win->height - 1 * (GFX_BM_HEIGHT(pImg_CHARGE_0))-12;
					#endif
					
					gfx_draw_bitmap(ps_win->m_surface,
		                      13,
		                     bar_ypos ,
		                     (unsigned char*) pImg_CHARGE_0,
		                      0, //int iSourceLeft,
		                      0, //int iSourceTop,
		                       0,//int iSourceCopydWidth,
		                      0,// int iSourceCopydHeight,
		                      GFX_ROP_COPY //int iRop
		                       );	

				} else {
 		                         bar_ypos =   1 * (GFX_BM_HEIGHT(pImg_CHARGE_2));
			
					gfx_draw_bitmap(ps_win->m_surface,
			                      13,
			                    bar_ypos,
			                     (unsigned char*) pImg_CHARGE_1,
			                      0, //int iSourceLeft,
			                      0, //int iSourceTop,
			                       0,//int iSourceCopydWidth,
			                      0,// int iSourceCopydHeight,
			                      GFX_ROP_COPY //int iRop
			                       );					
					for(i=2 ;i <=cur_step;i++) {
					    #ifdef BATT_PLUS_POLL_LEFT		
			                    bar_ypos =   i* (GFX_BM_HEIGHT(pImg_CHARGE_2))-6;
					    #else
			                    bar_ypos =  ps_win->height - i* (GFX_BM_HEIGHT(pImg_CHARGE_2))-6;
  					    #endif
						if(i==20) {
						gfx_draw_bitmap(ps_win->m_surface,
				                      13,
				                    bar_ypos,
				                     (unsigned char*) pImg_CHARGE_3,
				                      0, //int iSourceLeft,
				                      0, //int iSourceTop,
				                       0,//int iSourceCopydWidth,
				                      0,// int iSourceCopydHeight,
				                      GFX_ROP_COPY //int iRop
				                       );	
						} else {
							gfx_draw_bitmap(ps_win->m_surface,
				                      13,
				                    bar_ypos,
				                     (unsigned char*) pImg_CHARGE_2,
				                      0, //int iSourceLeft,
				                      0, //int iSourceTop,
				                       0,//int iSourceCopydWidth,
				                      0,// int iSourceCopydHeight,
				                      GFX_ROP_COPY //int iRop
				                       );	

						}
					}
				}

				printf("batt step %d \n",cur_step);
			} else {
				gfx_draw_fill_rect(ps_win->m_surface,ps_pgs->bar_x1,ps_pgs->bar_y1,ps_pgs->bar_width,ps_pgs->bar_height,GFX_RGB32(0x0,0x0,0x0));		
				gfx_draw_fill_rect(ps_win->m_surface,ps_pgs->bar_x1,ps_pgs->bar_y1,cur_bar_width,ps_pgs->bar_height,GFX_RGB32(0xFF,0xFF,0xff));
			}

			gfx_flush_window(ps_win,0,0,0,0);

		}
	}
	return 0;	
}




//
// simple log view.
// 

int gfx_init_logview(struct ts_gfx_log_view *ps_lv,...)
{
  struct ts_gfx_window *ps_win = &ps_lv->win;

    int i;
	
    int ArgvType = -1;
     	
    va_list list;	    

    if(!ps_lv) return 0;

    va_start(list,ps_lv); 

    gfx_init_window(ps_win,list);
   	
    va_start(list,ps_lv); 
		
    for(i=0;i < MAX_GFX_ARGV_FIELDS;i++) {
        ArgvType= va_arg(list,int);
        if(ArgvType == GFX_ARGV_END)   goto End;  
		
	        switch(ArgvType)
	        {
//		    	case PGS_START: 	ps_pgs->start  = (int)va_arg(list,int); break;

		        default:    break;
	        }
   }  
  

End:
    va_end(list);

	ps_lv->_id =GFX_LOG_VIEW_ID;
	
	ps_lv->lv_x1 = 10;
	ps_lv->lv_y1= 10;
	ps_lv->lv_width = ps_win->width - 20;
	ps_lv->lv_height= ps_win->height - 20;	

	memset(ps_lv->as_log_text_line,0x0,sizeof(ps_lv->as_log_text_line));

	ps_lv->lv_y_space = (12 +3);
	ps_lv->max_log_line = ps_lv->lv_height /ps_lv->lv_y_space;
	
		
	for(i=0;i<MAX_LOG_VIEW_LINE;i++) {
		ps_lv->ps_log_text_line[i] =  &ps_lv->as_log_text_line[i];
	}

	ps_lv->cur_log_line = 0;	
	
return 0;

}
int gfx_draw_logview(struct ts_gfx_log_view *ps_lv)
{
	struct ts_gfx_window *ps_win = &ps_lv->win;

	gfx_draw_window(ps_win);
			
	gfx_flush_window(ps_win,0,0,0,0);

	return 0;	
}


int gfx_reset_logview(struct ts_gfx_log_view *ps_lv)
{

	return 0;
}

int gfx_logview_settext(struct ts_gfx_log_view *ps_lv,char *log_msg,int *textcolor) 
{
	int size;
	int i,k;
	
	struct ts_log_text_line* temp;
	struct ts_log_text_line* ps_text_log_line;

	unsigned int ColorSet[3] = { GFX_RGB32(0xff,0xff,0xff),GFX_RGB32(0xdd,0xdd,0xdd),GFX_RGB32(0x0,0x0,0x0) };

	struct ts_gfx_window *ps_win = &ps_lv->win;

	if(ps_lv->_id!=GFX_LOG_VIEW_ID) 	return 0;

	{
		int max_msg_line;
		char msg_buffer[MAX_LINE_BUFFER];
		char *pmsg_line_array[20];
		char *p_msg_token;	

		strcpy(msg_buffer,log_msg);
				
		max_msg_line = 0;

		  p_msg_token = strtok( msg_buffer, "\n");

	   	  if(p_msg_token)  {
			   pmsg_line_array[max_msg_line] = p_msg_token;
		   
			   max_msg_line++;

			   while( p_msg_token = strtok( NULL, "\n"))
			   {
			         pmsg_line_array[max_msg_line] = p_msg_token;
				   max_msg_line++;
			   }
	   	  }

		 for(k=0;k<max_msg_line;k++) {
			//
			// Below is hard to implement....
			//
		
			if(ps_lv->cur_log_line < ps_lv->max_log_line) {	
				ps_text_log_line = ps_lv->ps_log_text_line[ps_lv->cur_log_line++];
				memcpy(ps_text_log_line->text_color,textcolor, 4 * 3);
				strcpy(ps_text_log_line->log_text,pmsg_line_array[k]);
				
			} else {
				for(i=0;i<ps_lv->max_log_line;i++) 
				{
					if(i==0) {
						temp  = ps_lv->ps_log_text_line[0] ;
					}	
					ps_lv->ps_log_text_line[i] = ps_lv->ps_log_text_line[i+1] ;
				}
			
				ps_lv->ps_log_text_line[ps_lv->max_log_line] = temp;

				ps_text_log_line = ps_lv->ps_log_text_line[ps_lv->max_log_line-1];
				memcpy(ps_text_log_line->text_color,textcolor, 4 * 3);
				strcpy(ps_text_log_line->log_text,pmsg_line_array[k]);
			}

		 }
		  
	}

	
	#if 0
	for(i=0;i<ps_lv->cur_log_line;i++) {
		printf("%s\n",ps_lv->ps_log_text_line[i]->log_text);
	}
	printf("\n");
	#endif
	

	gfx_draw_fill_rect(  ps_win->m_surface,
                      ps_lv->lv_x1,
                      ps_lv->lv_y1,
                      ps_lv->lv_width,
                      ps_lv->lv_height,
                       GFX_RGB32(0x0,0x0,0x0)
                       );
	for(i=0;i<ps_lv->cur_log_line;i++) {
		ps_text_log_line = ps_lv->ps_log_text_line[i];

		if(ps_text_log_line->log_text[0]!=0) {
		gfx_draw_string(
				ps_win->m_surface,
				ps_text_log_line->log_text,
		             ps_lv->lv_x1,
		             ps_lv->lv_y1 +  ps_lv->lv_y_space * i,
		              TF_SHADE | TF_TRANSPARENT,
		              0,
		              ps_text_log_line->text_color);
		}
	}

	gfx_flush_window(ps_win,0,0,0,0);

	return 0;
}


