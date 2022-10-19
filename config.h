/* See LICENSE file for copyright and license details. */

#include "patches/selfrestart.c"
#include "patches/fibonacci.c"

/* appearance */
static const unsigned int borderpx  = 1;        /* border pixel of windows */
static const unsigned int snap      = 32;       /* snap pixel */
static const int showbar            = 1;        /* 0 means no bar */
static const int topbar             = 1;        /* 0 means bottom bar */
static const char *fonts[]          = { "SourceCodePro:size=12" };
static const char dmenufont[]       = "SourceCodePro:size=12";
static const char col_gray1[]       = "#1d1f21";
static const char col_gray2[]       = "#323233";  
static const char col_gray3[]       = "#929593";
static const char col_gray4[]       = "#ecebec";
static const char col_cyan[]        = "#81a2be";
static const char *colors[][3]      = {
	/*               fg         bg         border   */
	[SchemeNorm] = { col_gray3, col_gray1, col_gray2 },
	[SchemeSel]  = { col_gray4, col_gray1,  col_cyan  },
};

/* tagging */
static const char *tags[] = { "1", "2", "3", "4", "5", "6", "7", "8", "9" };

static const Rule rules[] = {
	/* xprop(1):
	 *	WM_CLASS(STRING) = instance, class
	 *	WM_NAME(STRING) = title
	 */
	/* class      instance    title       tags mask     isfloating   monitor */
	{ "Gimp",     NULL,       NULL,       0,            1,           -1 },
	{ "Firefox",  NULL,       NULL,       1 << 8,       0,           -1 },
};

/* layout(s) */
static const float mfact     = 0.55; /* factor of master area size [0.05..0.95] */
static const int nmaster     = 1;    /* number of clients in master area */
static const int resizehints = 0;    /* 1 means respect size hints in tiled resizals */

static const Layout layouts[] = {
	/* symbol     arrange function */

	{ "[\\]",     dwindle },  /* first entry is default */
	{ "[]=",      tile },
	{ "><>",      NULL },    /* no layout function means floating behavior */
	{ "[M]",      monocle },
    { "[@]",      spiral },
};

/* key definitions */
#define MODKEY Mod4Mask
#define TAGKEYS(KEY,TAG) \
	{ MODKEY,                       KEY,      view,           {.ui = 1 << TAG} }, \
	{ MODKEY|ControlMask,           KEY,      toggleview,     {.ui = 1 << TAG} }, \
	{ MODKEY|ShiftMask,             KEY,      tag,            {.ui = 1 << TAG} }, \
	{ MODKEY|ControlMask|ShiftMask, KEY,      toggletag,      {.ui = 1 << TAG} },

/* helper for spawning shell commands in the pre dwm-5.0 fashion */
#define SHCMD(cmd) { .v = (const char*[]){ "/bin/sh", "-c", cmd, NULL } }

#define TERM "alacritty"
#define TERMCMD(cmd) { .v = (const char*[]){ TERM, "-e", cmd, NULL } }

#define BROWSER "brave"

/* commands */
static char dmenumon[2] = "0"; /* component of dmenucmd, manipulated in spawn() */
static const char *dmenucmd[] = { "dmenu_run", "-m", dmenumon, "-fn", dmenufont, "-nb", col_gray1, "-nf", col_gray3, "-sb", col_cyan, "-sf", col_gray4, NULL };
static const char *termcmd[]  = { TERM, NULL };
static const char *browsercmd[]  = { BROWSER, NULL };
static const char *cwdtermcmd[]  = { "alacritty", "--working-directory", "`xcwd`", NULL };

#define METACOMBO MODKEY|ShiftMask


static Key keys[] = {
	/* modifier                     key        function        argument */
	
  /* Apps */
	{ MODKEY,                       XK_d,      			spawn,          {.v = dmenucmd } },       /* dmenu */
	{ MODKEY,                       XK_Return, 			spawn,          {.v = termcmd } },        /* terminal */ 
	{ MODKEY,                       XK_b,      			spawn,          {.v = browsercmd } },     /* browser */ 
	{ MODKEY,                       XK_v,      			spawn,          TERMCMD("nvim") },        /* nvim */ 
	{ MODKEY|ControlMask,           XK_v,      			spawn,          TERMCMD("nvim ~/dev/tools/dwm/config.h") }, 
	{ MODKEY|ControlMask,           XK_Return,  		spawn,          {.v = cwdtermcmd }}, 
	
	/* META ============================================================================= */
	// General
	{ METACOMBO,             		XK_b,      			togglebar,      {0} },                    /* toggle bar*/
	{ MODKEY|ControlMask,           XK_q,      			quit,         	{0} },                    /* quit dwm */
	{ MODKEY|ControlMask,           XK_r,      			self_restart,   {0} },            				/* restart dwm */
	// Window
	{ METACOMBO,                    XK_h,      			setmfact,       {.f = -0.05} },           /* master size increase */
	{ METACOMBO,                    XK_l,      			setmfact,       {.f = +0.05} },           /* master size decrease */
	{ METACOMBO,                    XK_bracketleft,     incnmaster,     {.i = +1 } },           	/* master number increase */
	{ METACOMBO,                    XK_bracketright,    incnmaster,     {.i = -1 } },           	/* master number decrease */
	// Focus/view
	{ MODKEY,                       XK_j,      			focusstack,     {.i = +1 } },             /* stack focus heigher */
	{ MODKEY,                       XK_k,      			focusstack,     {.i = -1 } },             /* stack focus lower */
	{ MODKEY,                       XK_space, 			zoom,           {0} },                    /* bring focused to top of the stack */
	{ MODKEY,                       XK_Tab,    			view,           {0} },                    /* toggle view to the last viewed tag */
	{ METACOMBO,                    XK_q,      			killclient,     {0} },                    /* kill client */
	{ MODKEY,                       XK_0,      			view,           {.ui = ~0 } },            /* view all tags */
	{ METACOMBO,                    XK_0,      			tag,            {.ui = ~0 } },            /* tag focused to 0, making it sticky */
	/* Layouts */
	{ MODKEY,                       XK_t,      			setlayout,      {.v = &layouts[1]} },     /* layout to tile */
	{ MODKEY,                       XK_f,      			setlayout,      {.v = &layouts[2]} },     /* layout to floating */
	{ MODKEY,                       XK_m,      			setlayout,      {.v = &layouts[3]} },     /* layout to monocle */
    { MODKEY,                       XK_r,               setlayout,      {.v = &layouts[4]} },     /* layout to fibonacci spiral */
    { MODKEY|ShiftMask,             XK_r,               setlayout,      {.v = &layouts[0]} },     /* layout to fibonacci dwindle */

	{ METACOMBO,                    XK_f,  				setlayout,      {0} },                    /* toggle between floating and previous layout  */
	{ MODKEY|ControlMask,           XK_f,  				togglefloating, {0} },                    /* toggle floating specifically for active client */
	// Multiple monitors
	{ MODKEY,                       XK_comma,  			focusmon,       {.i = -1 } },
	{ MODKEY,                       XK_period, 			focusmon,       {.i = +1 } },
	{ MODKEY|ShiftMask,             XK_comma,  			tagmon,         {.i = -1 } },
	{ MODKEY|ShiftMask,             XK_period, 			tagmon,         {.i = +1 } },
  
	/* Media keys */
	// TODO

	/* TAGS                         key                        tag */
	TAGKEYS(                        XK_1,                      0)
	TAGKEYS(                        XK_2,                      1)
	TAGKEYS(                        XK_3,                      2)
	TAGKEYS(                        XK_4,                      3)
	TAGKEYS(                        XK_5,                      4)
	TAGKEYS(                        XK_6,                      5)
	TAGKEYS(                        XK_7,                      6)
	TAGKEYS(                        XK_8,                      7)
	TAGKEYS(                        XK_9,                      8)
	
};

/* button definitions */
/* click can be ClkTagBar, ClkLtSymbol, ClkStatusText, ClkWinTitle, ClkClientWin, or ClkRootWin */
static Button buttons[] = {
	/* click                event mask      button          function        argument */
	{ ClkLtSymbol,          0,              Button1,        setlayout,      {0} },
	{ ClkLtSymbol,          0,              Button3,        setlayout,      {.v = &layouts[2]} },
	{ ClkWinTitle,          0,              Button2,        zoom,           {0} },
	{ ClkStatusText,        0,              Button2,        spawn,          {.v = termcmd } },
	{ ClkClientWin,         MODKEY,         Button1,        movemouse,      {0} },
	{ ClkClientWin,         MODKEY,         Button2,        togglefloating, {0} },
	{ ClkClientWin,         MODKEY,         Button3,        resizemouse,    {0} },
	{ ClkTagBar,            0,              Button1,        view,           {0} },
	{ ClkTagBar,            0,              Button3,        toggleview,     {0} },
	{ ClkTagBar,            MODKEY,         Button1,        tag,            {0} },
	{ ClkTagBar,            MODKEY,         Button3,        toggletag,      {0} },
};

