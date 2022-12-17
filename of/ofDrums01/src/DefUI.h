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

// Layout settings
#define SXsmall (70)
#define SXlarge (100)
#define SYtiny (40)
#define SYsmall (70)
#define SYlarge (100)

#define XVOL (SXsmall)
#define X0 (XVOL + SXsmall)
#define X1 (X0 + SXsmall + SXlarge)
#define X2 (X1 + SXsmall + SXlarge)
#define X3 (X2 + SXsmall + SXlarge)

#define Y0 (180)
#define Y1 (Y0 + SYsmall)
#define Y2 (Y1 + SYsmall)
#define Y3 (Y2 + SYsmall)
#define Y4 (Y3 + SYsmall)

#define Marks (11)
#define FaderR (30)

#define PANELWIDTH (X3 + SXsmall + XVOL)
#define PANELHEIGHT (PANELWIDTH*2/3)
PANEL_SIZE(PANELWIDTH, PANELHEIGHT)
FONT_DEF("fonts/verdana.ttf", Marks, ofColor(128), 4)

// Variables
#define N (4)
VAR(Volume)
VARARR(Vol, N)
VARARR(SR, N)
VARARR(Freq, N)
VARARR(FreqDelta, N)
VARARR(Duration, N)
VARARR(Release, N)
VARARR(Timbre, N)
VARARR(Noise, N)
VARARR(Pad, N)

// UI
FADER(Volume, Volume, "Vol", Marks, XVOL, Y0, FaderR)

#define DRUM_BLOCK(I, X) \
SCREEN(Scr##I, "", X-5, 40, SXsmall+10, 60)\
FADER(Vol##I, Vol[I-1], "Vol", Marks, X, Y0, FaderR) \
FADER(SR##I, SR[I-1], "SR", Marks, X+SXsmall, Y0, FaderR) \
FADER(Freq##I, Freq[I-1], "Freq", Marks, X, Y1, FaderR) \
FADER(FreqDelta##I, FreqDelta[I-1], "Delta", Marks, X + SXsmall, Y1, FaderR) \
FADER(Duration##I, Duration[I-1], "Duration", Marks, X, Y2, FaderR) \
FADER(Release##I, Release[I-1], "Release", Marks, X + SXsmall, Y2, FaderR) \
FADER(Timbre##I, Timbre[I-1], "Timbre", Marks, X, Y3, FaderR) \
FADER(Noise##I, Noise[I-1], "Noise", Marks, X + SXsmall, Y3, FaderR) \
BUTTON(Pad##I, Pad[I-1], "Pad " #I, '1'+I-1, X + SXsmall / 2, Y4, SXsmall, 30)

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
