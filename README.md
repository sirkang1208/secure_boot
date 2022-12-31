# Secure Boot

※ This program is made for Windows.

## About this project
### Description
- Secure boot for bare metal embedded device
- Bootloader and Firmware Image generator tool

### Development environment
- Hardware
    - digilent genesys 2 FPGA board with ARM cortex M3 and PKCC
- Software
    - ARM Keil MDK (uVision 5 IDE)
- Image gen tool
    - Visual Studio (C language)
    - Python

### Secure boot process
![secure-boot](https://user-images.githubusercontent.com/101001675/209815189-b33aa42e-d056-462a-95ec-b2b465a32931.png)

* chip vendor : Companies that made chips and boards
    - Build the first bootloader and write it to an unmodifiable ROM


* Application vendor : Companies that use boards to make software
    - Build a second bootloader and firmware
### Image generation process
- 2nd bootloader image generation
![image](https://user-images.githubusercontent.com/101001675/209815703-01fa3073-05b9-499f-a5e7-12e277f8d1c7.png)

- firmware image generation
![image](https://user-images.githubusercontent.com/101001675/209815798-21369b26-36ac-4424-ba6c-b9237db351c4.png)


## Directory Structure
- /Keys
    - *_key_1.pem : RSA key of chip vendor
    - *_key_2.pem : RSA key of application vendor
- /sources
    - 1st bootloader (※ must be written to an unmodifiable ROM)
    - 2nd bootloader
    - firmware
    - scatter file
- /img_gen_tool
    - Final image of 1st bootloader, 2nd bootloader and firmware generation tool 
    - Frontend : Python (PyQt5)
    - Backend : C (openssl)
    
## Contributors

<!-- ALL-CONTRIBUTORS-LIST:START - Do not remove or modify this section -->
<!-- prettier-ignore-start -->
<!-- markdownlint-disable -->
<table>
  <tr>
    <td align="center"><a href="https://github.com/sirkang1208"><img src="https://avatars.githubusercontent.com/u/104350527?v=4" width="100px;" alt=""/><br /><sub><b>sirkang1208</b></sub></a></td>
    <td align="center"><a href="https://github.com/bpsswu"><img src="https://avatars.githubusercontent.com/u/101001675?v=4" width="100px;" alt=""/><br /><sub><b>bpsswu</b></sub></a></td>
  </tr>
</table>

<!-- markdownlint-restore -->
<!-- prettier-ignore-end -->

<!-- ALL-CONTRIBUTORS-LIST:END -->
