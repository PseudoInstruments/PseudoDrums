// Defines for UI controls

// Empty definitions
#ifndef VAR
#define VAR(V)
#define VARARR(V,COUNT)
#endif

#ifndef FADER
#define FADER(NAME, V, TITLE, MAX, X, Y, D)
#define BUTTON(NAME, V, TITLE, KEY, X, Y, W, H)
#define LED(NAME, V, TITLE, X, Y, D)
#define SCREEN(NAME, TITLE, X, Y, W, H)
#endif
#ifndef PANEL_SIZE
#define PANEL_SIZE(Width, Height)
#endif
#ifndef FONT_DEF
#define FONT_DEF(FontFile, FontSize, FontColor, ShiftY)
#endif

//------------------------------------------------------
// UI size, font, and other main values
//------------------------------------------------------
#define PANELWIDTH (2000)
#define PANELHEIGHT (3000)
PANEL_SIZE(PANELWIDTH, PANELHEIGHT)
FONT_DEF("fonts/verdana.ttf", 50, ofColor(128), 4)

// Screen size in pixels
#define SCREEN_PX_W 128
#define SCREEN_PX_H 64

// Number of Fader values
#define FaderMax (11)


//------------------------------------------------------
// Layout coordinates
//------------------------------------------------------
#define SYsmall (250)
#define SYlarge (350)

#define XVOL (200)
#define X0 (400)
#define X1 (800)
#define X2 (1200)
#define X3 (1600)

#define Y0 (700)
#define Y1 (Y0 + SYsmall)
#define Y2 (Y1 + SYsmall)
#define Y3 (Y2 + SYsmall)
#define Y4 (Y3 + SYsmall)
#define Y5 (Y4 + SYsmall)
#define Y6 (Y5 + SYsmall)
#define Y7 (Y6 + SYsmall)
#define Y8 (Y7 + SYsmall)

#define FaderR (100)

//------------------------------------------------------
// Variables
//------------------------------------------------------
#define N (4)
VAR(Volume)
VAR(Compress)
VARARR(Vol, N)
VARARR(SR, N)
VARARR(Freq, N)
VARARR(FreqDelta, N)
VARARR(Duration, N)
VARARR(Release, N)
VARARR(Timbre, N)
VARARR(Noise, N)
VARARR(Pad, N)

//--------------------------------------------
// UI
//--------------------------------------------
FADER(Volume, Volume, "Vol", FaderMax, XVOL, Y0, FaderR)
FADER(Compress, Compress, "Compress", FaderMax, XVOL, Y1, FaderR)

SCREEN(Scr1, "", 200, 200, 700, 350)
SCREEN(Scr2, "", 1100, 200, 700, 350)

#define DRUM_BLOCK(I, X) \
FADER(Vol##I, Vol[I-1], "Vol", FaderMax, X, Y0, FaderR) \
FADER(SR##I, SR[I-1], "SR", FaderMax, X, Y1, FaderR) \
FADER(Freq##I, Freq[I-1], "Freq", FaderMax, X, Y2, FaderR) \
FADER(FreqDelta##I, FreqDelta[I-1], "Delta", FaderMax, X, Y3, FaderR) \
FADER(Duration##I, Duration[I-1], "Duration", FaderMax, X, Y4, FaderR) \
FADER(Release##I, Release[I-1], "Release", FaderMax, X, Y5, FaderR) \
FADER(Timbre##I, Timbre[I-1], "Timbre", FaderMax, X, Y6, FaderR) \
FADER(Noise##I, Noise[I-1], "Noise", FaderMax, X, Y7, FaderR) \
BUTTON(Pad##I, Pad[I-1], "Pad " #I, '1'+I-1, X, Y8, FaderR*2, FaderR*1.5)

DRUM_BLOCK(1, X0)
DRUM_BLOCK(2, X1)
DRUM_BLOCK(3, X2)
DRUM_BLOCK(4, X3)

// Clear definitions
#undef VAR
#undef VARARR

#undef FADER
#undef BUTTON
#undef LED
#undef SCREEN
#undef PANEL_SIZE
#undef FONT_DEF
