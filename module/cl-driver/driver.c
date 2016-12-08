

/********************峰鸣器*****************************/
static struct resource beepsrc[] = {
        {
                .start          =  EXYNOS4_GPD0(0),
                .end            =  EXYNOS4_GPD0(0),
                .name           =  "changbeep",
                .flags          =  IORESOURCE_MEM,
        },
};

static struct platform_device changbeep = {
                .name                   =  "changbeep",
                .id                             =  -1,
                .num_resources  =  ARRAY_SIZE(beepsrc),
                .resource               =  beepsrc,
};


/***********************土壤湿度**************************/
static struct resource humiditysrc[] = {
        {
                .start          =  EXYNOS4_GPD0(0),
                .end            =  EXYNOS4_GPD0(0),
                .name           =  "changhumidity",
                .flags          =  IORESOURCE_MEM,
        },
};

static struct platform_device changhumidity = {
                .name                   =  "changhumidity",
                .id                             =  -1,
                .num_resources  =  ARRAY_SIZE(humiditysrc),
                .resource               =  humiditysrc,
};


/******************雨滴*********************/

static struct resource rainsrc[] = {
        {
                .start          =  EXYNOS4_GPD0(0),
                .end            =  EXYNOS4_GPD0(0),
                .name           =  "changrain",
                .flags          =  IORESOURCE_MEM,
        },
};

static struct platform_device changrain = {
                .name                   =  "changrain",
                .id                             =  -1,
                .num_resources  =  ARRAY_SIZE(rainsrc),
                .resource               =  rainsrc,
};


/*****************烟雾**************/
static struct resource smogsrc[] = {
        {
                .start          =  EXYNOS4_GPD0(0),
                .end            =  EXYNOS4_GPD0(0),
                .name           =  "changsmog",
                .flags          =  IORESOURCE_MEM,
        },
};

static struct platform_device changsmog = {
                .name                   =  "changsmog",
                .id                             =  -1,
                .num_resources  =  ARRAY_SIZE(smogsrc),
                .resource               =  smogsrc,
};
