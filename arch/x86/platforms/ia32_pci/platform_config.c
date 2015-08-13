/*
 * Copyright (c) 2015 Intel Corporation.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1) Redistributions of source code must retain the above copyright notice,
 * this list of conditions and the following disclaimer.
 *
 * 2) Redistributions in binary form must reproduce the above copyright notice,
 * this list of conditions and the following disclaimer in the documentation
 * and/or other materials provided with the distribution.
 *
 * 3) Neither the name of Intel Corporation nor the names of its contributors
 * may be used to endorse or promote products derived from this software without
 * specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */

/**
 * @file Contains configuration for ia32_pci platforms.
 */

#include <stdio.h>
#include <stdint.h>
#include <device.h>
#include <init.h>

#include "board.h"

#ifdef CONFIG_NS16550
#include <drivers/uart.h>
#include <console/uart_console.h>
#include <serial/ns16550.h>


#if defined(CONFIG_UART_CONSOLE)
#if defined(CONFIG_PRINTK) || defined(CONFIG_STDOUT_CONSOLE)

/**
 * @brief Initialize NS16550 serial port #1
 *
 * UART #1 is being used as console. So initialize it
 * for console I/O.
 *
 * @param dev The UART device struct
 *
 * @return DEV_OK if successful, otherwise failed.
 */
static int ns16550_uart_console_init(struct device *dev)
{
	struct uart_init_info info = {
		.baud_rate = CONFIG_UART_BAUDRATE,
		.sys_clk_freq = UART_XTAL_FREQ,
	};

	uart_init(UART_CONSOLE_DEV, &info);

	return DEV_OK;
}

#else

static int ns16550_uart_console_init(struct device *dev)
{
	ARG_UNUSED(dev);

	return DEV_OK;
}

#endif
#endif /* CONFIG_UART_CONSOLE */

/**
 * @brief UART Device configuration.
 *
 * This contains the device configuration for UART. Note that
 * the first CONFIG_NS16550_PCI_NUM_PORTS ports are reserved
 * for auto detection. So they need to be empty.
 */
struct uart_device_config_t ns16550_uart_dev_cfg[] = {
	/* The first CONFIG_NS16550_PCI_NUM_PORTS ports are reserved. */
	{
		.port = 0,

		.port_init = ns16550_uart_port_init,

		#if (defined(CONFIG_UART_CONSOLE) \
		     && (CONFIG_UART_CONSOLE_INDEX == 0))
			.config_func = ns16550_uart_console_init,
		#endif
	},
	{
		.port = 0,

		.port_init = ns16550_uart_port_init,

		#if (defined(CONFIG_UART_CONSOLE) \
		     && (CONFIG_UART_CONSOLE_INDEX == 1))
			.config_func = ns16550_uart_console_init,
		#endif
	},
	/* Add pre-configured ports after this. */
};

/**< Device data */
static struct uart_ns16550_dev_data_t ns16550_uart_dev_data[2];

/* UART 0 */
DECLARE_DEVICE_INIT_CONFIG(ns16550_uart0,
			   CONFIG_UART_PORT_0_NAME,
			   &uart_platform_init,
			   &ns16550_uart_dev_cfg[0]);

pure_early_init(ns16550_uart0, &ns16550_uart_dev_data[0]);


/* UART 1 */
DECLARE_DEVICE_INIT_CONFIG(ns16550_uart1,
			   CONFIG_UART_PORT_1_NAME,
			   &uart_platform_init,
			   &ns16550_uart_dev_cfg[1]);

pure_early_init(ns16550_uart1, &ns16550_uart_dev_data[1]);


/**< UART Devices */
struct device * const uart_devs[] = {
	&__initconfig_ns16550_uart00,
	&__initconfig_ns16550_uart10,
};

#endif
