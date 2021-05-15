# ulp_wire1

This is an [ESP-IDF][1] module providing [1-Wire][2] support for the
[ULP-RISC-V coprocessor on the ESP32-S2][3], and maybe other Espressif
targets in the future.

## Usage

TODO

## Installation / Dependencies

This module depends on [ulp_gpio][4], so both need to [compiled for
the ULP-RISC-V][5].

	$ git submodule add https://github.com/bitmandu/ulp_gpio.git
	$ git submodule add https://github.com/bitmandu/ulp_wire1.git

Modify `CMakeLists.txt` as follows.

	idf_component_register(SRCS ...
	  INCLUDE_DIRS "ulp_gpio/include" "ulp_wire1/include" ...
	  PRIV_REQUIRES ulp riscv driver)

	set(ulp_app_name ulp_${COMPONENT_NAME})
	set(ulp_srcs "ulp_gpio/ulp_gpio.c" "ulp_wire1/wait.S" "ulp_wire1/wire1.c" ...)
	set(ulp_dep_srcs ...)

	ulp_embed_binary(${ulp_app_name} "${ulp_srcs}" "${ulp_dep_srcs}")

## Configuration

    $ idf.py set-target esp32s2
    $ idf.py menuconfig

In the `Component config` ⇨ `ESP32S2-specific` menu check:

- [x] Enable Ultra Low Power (ULP) Coprocessor
- [x] Enable RISC-V as ULP coprocessor

## Contributing

[Pull requests][pulls] and [issue/bug reports][issues] are very much encouraged.

## License

[MIT](LICENSE)


[issues]: https://github.com/bitmandu/ulp_wire1/issues
[pulls]: https://github.com/bitmandu/ulp_wire1/pulls
[1]: https://docs.espressif.com/projects/esp-idf/en/latest/esp32s2/
[2]: https://en.wikipedia.org/wiki/1-Wire
[3]: https://docs.espressif.com/projects/esp-idf/en/latest/esp32s2/api-guides/ulp-risc-v.html
[4]: https://github.com/bitmandu/ulp_gpio
[5]: https://docs.espressif.com/projects/esp-idf/en/latest/esp32s2/api-guides/ulp-risc-v.html#compiling-the-ulp-risc-v-code
