

// CONFIG_CLK_800_200_166_133

#define APLL_MDIV       0x64
#define APLL_PDIV       0x3
#define APLL_SDIV       0x1

#define MPLL_MDIV	0x29b
#define MPLL_PDIV	0xc
#define MPLL_SDIV	0x1

#define EPLL_MDIV	0x60
#define EPLL_PDIV	0x6
#define EPLL_SDIV	0x2

#define VPLL_MDIV	0x6c
#define VPLL_PDIV	0x6
#define VPLL_SDIV	0x3

#define APLL_RATIO	0
#define A2M_RATIO	4
#define HCLK_MSYS_RATIO	8
#define PCLK_MSYS_RATIO	12
#define HCLK_DSYS_RATIO	16
#define PCLK_DSYS_RATIO 20
#define HCLK_PSYS_RATIO	24
#define PCLK_PSYS_RATIO 28

#define CLK_DIV0_MASK	0x7fffffff

#define set_pll(mdiv, pdiv, sdiv)	(1<<31 | mdiv<<16 | pdiv<<8 | sdiv)

#define __APLL_VAL	set_pll(APLL_MDIV,APLL_PDIV,APLL_SDIV)
#define __MPLL_VAL	set_pll(MPLL_MDIV,MPLL_PDIV,MPLL_SDIV)
#define __EPLL_VAL	set_pll(EPLL_MDIV,EPLL_PDIV,EPLL_SDIV)
#define __VPLL_VAL	set_pll(VPLL_MDIV,VPLL_PDIV,VPLL_SDIV)

#define __CLK_DIV0_VAL    ((0<<APLL_RATIO)|(3<<A2M_RATIO)|(3<<HCLK_MSYS_RATIO)|(1<<PCLK_MSYS_RATIO)\
			|(3<<HCLK_DSYS_RATIO)|(1<<PCLK_DSYS_RATIO)|(4<<HCLK_PSYS_RATIO)|(1<<PCLK_PSYS_RATIO))


/*

// CONFIG_CLK_400_200_166_133)

#define APLL_MDIV   0x64
#define APLL_PDIV   0x3
#define APLL_SDIV   0x1
#define MPLL_MDIV	0x29b
#define MPLL_PDIV	0xc
#define MPLL_SDIV	0x1
#define EPLL_MDIV	0x60
#define EPLL_PDIV	0x6
#define EPLL_SDIV	0x2
#define VPLL_MDIV	0x6c
#define VPLL_PDIV	0x6
#define VPLL_SDIV	0x3

// CLK_DIV0
#define APLL_RATIO	0
#define A2M_RATIO	4
#define HCLK_MSYS_RATIO	8
#define PCLK_MSYS_RATIO	12
#define HCLK_DSYS_RATIO	16
#define PCLK_DSYS_RATIO 20
#define HCLK_PSYS_RATIO	24
#define PCLK_PSYS_RATIO 28

#define CLK_DIV0_MASK	0x7fffffff

#define set_pll(mdiv, pdiv, sdiv)	(1<<31 | mdiv<<16 | pdiv<<8 | sdiv)

#define __APLL_VAL	set_pll(APLL_MDIV,APLL_PDIV,APLL_SDIV)
#define __MPLL_VAL	set_pll(MPLL_MDIV,MPLL_PDIV,MPLL_SDIV)
#define __EPLL_VAL	set_pll(EPLL_MDIV,EPLL_PDIV,EPLL_SDIV)
#define __VPLL_VAL	set_pll(VPLL_MDIV,VPLL_PDIV,VPLL_SDIV)

#define __CLK_DIV0_VAL    ((1<<APLL_RATIO)|(3<<A2M_RATIO)|(1<<HCLK_MSYS_RATIO)|(1<<PCLK_MSYS_RATIO)\
   		|(3<<HCLK_DSYS_RATIO)|(1<<PCLK_DSYS_RATIO)|(4<<HCLK_PSYS_RATIO)|(1<<PCLK_PSYS_RATIO))
*/