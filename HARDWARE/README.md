# ESP32-GATEWAY
ESP32 WiFi / BLE development board with Ethernet, microSD card, GPIOs made with KiCAD.

Notice that we use the latest nightly KiCad builds.

Hardware change log:

- Hardware revision E

1. R5 connected after T1's collector and value changed from NA to 1K. 
2. Improved drill distances.

- Hardware revision D

Major Ethernet schematic changes, similar to ESP32-PoE:

1. LAN8710's pin XTAL1 was disconnected from GPIO0 and connected to GPIO17/EMAC_CLK_OUT_180. GPIO17 is now GPIO5 is now used as Ethernet "power pin".
2. CR1(Q50MHz/25ppm/3V/4P/5x3.2mm) and all surrounding elements were deleted.
3. U8(NCP303LSN27T1G(SOT-23-5)) and all surrounding elements were deleted.
4. U10(BAT54C), R44 and R5 were deleted.
5. FET1, R5, R6, C2, T1 and L1 were added to enable Ethernet power on/off feature.
6. GPIO5 is used for Ethernet power on/off.

- Hardware revision C

1. R5(1k) resistor added between OSC_EN and NRST(pin #19) of LAN8710A chip. Later, it was changed to NA!
2. U2 changed from SY8009AAAC(SOT23-5) to SY8089AAAC(SOT23-5).
3. FET2 changed from IRLML6402 to WPM2015-3/TR.
4. ESP32-WROOM module pad improvements 

Major changes to the Ethernet PHY and reset similar to EVB's_Rev_D1:

5. U8 changed from BD5230G-TR(SSOP5/SOT-23-5) to NCP303LSN27T1G(SOT-23-5).
6. C28 changed from 10nF/50V/20%/Y5V/C0603 to 470nF/16V/10%/X7R/C0603.
7. C18 changed from 10uF/6.3V/20%/X5R/C0603 to NA.
8. R38 changed from 100k to 10k/R0603.
9. R5 changed from 1k/R0603 to NA.
