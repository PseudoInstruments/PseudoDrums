// Defines for UI controls

// Empty definitions
#ifndef FADER
#define FADER(NAME, TITLE, MAX, X, Y, D)
#define BUTTON(NAME, TITLE, KEY, X, Y, W, H)
#define LED(NAME, TITLE, X, Y, D)
#define SCREEN(NAME, TITLE, X, Y, W, H)
#endif
#ifndef PANEL_SIZE
#define PANEL_SIZE(Width, Height)
#endif
#ifndef FONT_DEF
#define FONT_DEF(FontFile, FontSize, FontColor, ShiftY)
#endif

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

#define Marks (9)
#define FaderR (30)

#define PANELWIDTH (X3 + SXsmall + XVOL)
#define PANELHEIGHT (PANELWIDTH*2/3)
PANEL_SIZE(PANELWIDTH, PANELHEIGHT)
FONT_DEF("fonts/verdana.ttf", Marks, ofColor(128), 4)

FADER(Vol, "Vol", Marks, XVOL, Y0, FaderR)

#define DRUM_BLOCK(N, X) \
SCREEN(SCR##N, "", X-5, 40, SXsmall+10, 60)\
FADER(Vol##N, "Vol", Marks, X, Y0, FaderR) \
FADER(SR##N, "SR", Marks, X+SXsmall, Y0, FaderR) \
FADER(Freq##N, "Freq", Marks, X, Y1, FaderR) \
FADER(FreqDelta##N, "Delta", Marks, X + SXsmall, Y1, FaderR) \
FADER(Duration##N, "Duration", Marks, X, Y2, FaderR) \
FADER(Release##N, "Release", Marks, X + SXsmall, Y2, FaderR) \
FADER(Timbre##N, "Timbre", Marks, X, Y3, FaderR) \
FADER(Noise##N, "Noise", Marks, X + SXsmall, Y3, FaderR) \
BUTTON(PAD##N, "Pad " #N, '1'+N-1, X + SXsmall / 2, Y4, SXsmall, 30) \

DRUM_BLOCK(1, X0)
DRUM_BLOCK(2, X1)
DRUM_BLOCK(3, X2)
DRUM_BLOCK(4, X3)

// Clear definitions
#undef FADER
#undef BUTTON
#undef LED
#undef SCREEN
#undef PANEL_SIZE
#undef FONT_DEF
