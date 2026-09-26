# LilyCam

<img width="588" height="419" alt="image" src="https://github.com/user-attachments/assets/42481707-9da6-4d1a-8819-80bec02c9e68" />

LilyCam is a custom DIY digital camera built from scratch around the Seeed Studio XIAO ESP32-S3 Sense, a tiny, low-cost microcontroller board with an integrated camera sensor and microSD card slot inspired by the Kodak Charmera Keychain

The idea behind the name

"Lily" evokes something small, elegant, and blooming. A compact camera that fits in your palm but still delivers personality through its built-in creative filters making photography playful and personal.


## What it does
Core Features :

Button	Function

SW1 — Power	Toggles the camera between active mode and deep sleep (ultra-low battery drain)

SW2 — Shutter	Captures a photo and saves it to the microSD card

SW3 — Style	Cycles creative color filters: Normal → Grayscale → Negative → Sepia → Night Vision Green


## BOM

| Item | Qty | Part Name | Description | Price (\$) | Product Page Link |
| :---: | :---: | :--- | :--- | :---: | :--- |
| **1** | 1 | Seeed Studio XIAO ESP32-S3 Sense | Dual-core ESP32-S3 MCU with detachable OV2640 camera board and digital mic | \$13.90 | [Seeed Studio Product Page](https://www.seeedstudio.com/XIAO-ESP32S3-Sense-p-5639.html) |
| **2** | 3 | 6mm Tactile Switch (6x6x5mm) | 4-pin momentary push button for hardware input/reset | \$1.00 | Search target: `6x6x5mm tactile switch pack` |
| **3** | 1 | 1.3 Inch TFT IPS Display Module | 240x240 Full-Color Screen with SPI interface (ST7789 Driver) | \$7.99 | [Amazon Product Page](https://www.amazon.com/Display-Module-240x240-Interface-Arduino/dp/B0DN9NMBFW/ref=sr_1_10) |



### Note
* **PCB and 3D printing**: PCB and 3D printind doesnt included in the Bill of material. 
* **Buttons (Tactile Switches)**: These are universally sold in multi-packs (usually 10 to 50 pieces) rather than single units, which accounts for the \$1.00 baseline package price. 
* **Display Interface**: The 1.3-inch screen utilizes a **4-wire SPI communication interface** driven by the **ST7789** controller chip. Ensure you match the pinout labels (GND, VCC, SCL, SDA, RES, DC, BLK) to the corresponding digital pins on your XIAO module during assembly.

## Schematics

<img width="634" height="432" alt="image" src="https://github.com/user-attachments/assets/a953e7cb-c78e-442f-9f63-0e2608953c49" />


## PCB
<img width="393" height="517" alt="image" src="https://github.com/user-attachments/assets/b0666fdc-c80c-4036-8b13-11597e734d18" />


## CAD

<img width="465" height="323" alt="image" src="https://github.com/user-attachments/assets/44d58ec3-0aad-4e53-8a0d-28444c9898e7" />

<img width="304" height="175" alt="image" src="https://github.com/user-attachments/assets/e124fd30-4904-415c-93f7-88a089871ff1" />

<img width="439" height="284" alt="image" src="https://github.com/user-attachments/assets/c9504641-0410-47cc-b20f-6dc2c4b57f53" />


## Extra
This project is made for Hack Club YSWS program : Stardance
Links : https://stardance.hackclub.com/projects/59421




