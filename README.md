# STM32F1 Template

Project template for STM32F103XB with USART, easy to adaptation to another board, makefile is used.
It ~~was tested~~ for BK3432 UART.
CSIMS only, not libs, but part of LL was added.

## Dependencies

### System
1. `arm-none-eabi`
2. `arm-none-eabi-gdb` or `gdb-multiarch`(by default, see .vscode/settings.json)
3. `gcc-multilib`
4. `openocd`
5. `make`

### VS Code
1. [C/C++ for Visual Studio Code](https://marketplace.visualstudio.com/items?itemName=ms-vscode.cpptools).
2. [Cortex-Debug](https://marketplace.visualstudio.com/items?itemName=marus25.cortex-debug).
3. [GNU Assembler Language Support](https://marketplace.visualstudio.com/items?itemName=basdp.language-gas-x86).

### Libs
1. [STM32CubeF1](https://github.com/STMicroelectronics/STM32CubeF1), not necessary to run this template. In common, the project can be adapted to any controller, so look for the libraries yourself)

## Build Firmware
Run in a terminal.
```bash
make
```
For debug mode 
```bash
DEBUG=1 make
```
or 
```bash
make debug
```

Not tested on Windows, but should work.

Firmware `firmware.elf` will be in the `Build/Release` directory.

To build .hex
```bash
make intel-hex
```
To build .bin
```bash
make binary
```
## Controller flasing

```bash
make flash
```
## Debugging

The above `openocd` uses VS Code with the [Cortex-Debug](https://marketplace.visualstudio.com/items?itemName=marus25.cortex-debug) extension as its IDE. Configs in the commit, all the charms of the register type are available.
