#ifndef UTILS_H
#define UTILS_H

/* For somewhat reason, tcc assumes that linux equals 1 */
#undef linux

/* Abusing the preprocessor violently */
#define __cat_internal(a, ...) a ## __VA_ARGS__
#define __cat(a, ...) __cat_internal(a, __VA_ARGS__)
#define __cat3(a, b, ...) __cat(a, __cat(b, __VA_ARGS__))
#define __cat4(a, b, c, ...) __cat(a, __cat3(b, c, __VA_ARGS__))
#define __pass(...) __VA_ARGS__

/* Append '0x' prefix to hex numbers */
#define HEX_PREFIX(x) __cat(0x, x)

/* Pattern-matching accessors for pins declarations */
#define __pad(a, b, c) a
#define __gpio_port(a, b, c) b
#define __gpio_pin(a, b, c) c
#define __pin_attr(x, attr) __pass(__##attr __cat3(MOD, _, x))

/* Helper macros that substitutes actual pins values depending on MOD value
 * e.g. if MOD == MOD1 it will expand to pins regarding to MOD1 connector
 * Supposed to be used to avoid DTS duplication for modules fitting more than
 * one slot.
 *
 * MOD_PINMUX(FOO, SOME_MUX) -> MX28_PAD_<pad name>__SOME_MUX
 *	use for fine-grained pinmux control, usually not needed
 * MOD_PINMUX_GPIO(FOO) -> MX28_PAD_<pad name>__GPIO_<port>_<pin>
 *	use to configure pin as GPIO
 * MOD_GPIO(FOO) -> &gpio<port> <pin>
 *	use in periherial device nodes, e.g. gpios = <MOD_GPIO(FOO)>
 * MOD_DT_ALIAS(foo) -> mod<n>_foo
 *	use to reference on per-slot peripherials (i2c, spi, uart)
 */
#define MOD_PINMUX(x, func) __cat4(MX28_PAD_, __pin_attr(x, pad), __, func)
#define MOD_PINMUX_GPIO(x) MOD_PINMUX(x, __cat4(GPIO_, __pin_attr(x, gpio_port), _, __pin_attr(x, gpio_pin)))
#define MOD_GPIO_PORT(x) __pin_attr(x, gpio_port)
#define MOD_GPIO_PIN(x) __pin_attr(x, gpio_pin)
#define MOD_GPIO(x) __cat(&gpio, MOD_GPIO_PORT(x)) MOD_GPIO_PIN(x) 0
#define MOD_DT_ALIAS(x) __cat3(MOD_ALIAS, _, x)

#ifndef MOD_ALL_PINS
#define MOD_ALL_PINS
#endif

#ifdef FROM_SHELL
#define MOD_FOR_PIN(x) local GPIO_##x=$[MOD_GPIO_PORT(x) * 32 + MOD_GPIO_PIN(x)];
MOD_ALL_PINS
#undef MOD_FOR_PIN
#endif

#endif /* UTILS_H */