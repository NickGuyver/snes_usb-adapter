# snes_usb-adapter
SNES controller to USB adapter

This was my attempt at making the lowest possible latency SNES to USB adapter. It is extremely simple and I did a lot of testing to see how much quicker than spec I could go without breaking communications with the controller IC.

Rough estimate is 25µs for each iteration of loop(), but I have not tested from button to host USB read yet.

If you see any areas that I could improve, please let me know.
