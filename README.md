# Liquid-dap-duino
Daphile (Squeezebox) control with arduino and LCD

##Important note
I'm arduino newbee, so excuse my errors...

This sketch uses 40x4 LCD so special library is used - check tribute to... It can easily be replaced with standard LiquidCrystal lbrary.

Squeezebox returns text encoded in UTF8.

##Todo
Possibly switch to https://github.com/Swap-File/tron-suit/tree/master/Helmet/Software/Libraries/LiquidCrystalFast

Add buttons for simple control (play/pause/next/previous/vol up|dn)

Improve UTF8 decoding

Add special LCD icons for play/pause/stop and repeat none|song|playlist

##Tribute to
http://www.daphile.com/ Marvelous linux squeezebox player

https://github.com/Logitech for squeezebox stuff

https://forum.arduino.cc/index.php?topic=340250.0 pattdepanick whose base I used for my code (hope it's OK :)

https://code.google.com/archive/p/liquidcrystal440/ for liquidcrystal440 library
