# CrosshairOverlay
Put down your dry erase marker, your ruler and/or your transparent tape. Win API Crosshair application, click-through, always on top, per-pixel alpha, loading custom image.

## Why another cheating app like millions of others?
*Disclaimer: some game consider using a crosshair overlay as cheating, I personally don't consider it cheating as anyone could use dry erase marker directly on the screen and it would be undetectable.*

I wanted to play around a little with a fixed crosshair on weapon that sometimes don't have one, like most unzoomed sniper rifles. Also, I sometime lose track of the default crosshair in intense firefights. I tried drawing a point on a transparent tape directly on my screen like others might have tried but I found it was obstructing to much of screen real estate.

I searched the web for an application that could simply overlay a game with a crosshair but I didn't trust those shady application. It's then I decided to try making my own.

## What's different here?
Mostly, it only does what it claim it does. No malware/spyware/adware/etc. And the proof is that the source are open.


It's also an experiment, offering sources to multiple simple use cases (amateur) developpers may face with the Win API and GDI+, such as:

- Transparent/click-through window;
- Per-pixel alpha using no other lib than the one offered by Microsoft;
- Simple file opening dialog box;
- Rendering BMP, PNG and more;
- Forcing your own app on top; *don't do that with a normal application please*

# Documentation
See source comment, doc directory and below.

## How to use
See the [menu.txt](data/menu.txt) file that is shown as a MessageBox on app startup.