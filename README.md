This was my attempt at making the lowest possible latency SNES to USB adapter. It is extremely simple and I did a lot of testing to see how much quicker than spec I could go without breaking communications to the controller IC.

Testing, with an input latency testing setup I created (https://github.com/NickGuyver/usb_input_latency), shows the following stats:

	Minimum - 0.016033 ms
	Maximum - 0.155016 ms
	Average - 0.08671401102204408 ms
	Sample Standard Deviation - 0.03677839974308105 ms

Raw results are here - https://github.com/NickGuyver/usb_input_latency/tree/main/total_phase/results/12095757/20221005/154122

According to the controller IC spec it should be is 16.67ms, and this is significantly faster. The MiSTer testing sheet* shows the fastest of any retro controller or adapter, not limited to SNES, as 116µs. The lowest average currently recorded is 662µs. This setup proves that there is possibility to have a much lower latency retro controller to USB adapter and I was able to accomplish my goal.

All my testing was done with ~5 different controllers from eBay in various conditions. Some were actually able to go even faster, but I made this sketch to the slowest of my test group.

The latency testing setup leverages a USB protocol analyzer that recieves a hardware trigger in at the same moment the controller button does (using port registers). Then I subtract the time between the trigger and seeing the data packet pass through the analyzer. The analyzer timestamps have an accuracy of 16.7ns. Building the testing setup has been a much longer, more expensive, and more difficult project than the adapter itself. I will hopefully release this more accurate testing methodology for USB input devices in the near future.


If you see any areas that I could improve, please let me know.

*MiSTer testing sheet - https://docs.google.com/spreadsheets/d/1KlRObr3Be4zLch7Zyqg6qCJzGuhyGmXaOIUrpfncXIM/edit#gid=836207500
