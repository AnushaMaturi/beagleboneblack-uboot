/*
 * Control GPIO pins on the fly
 *
 * Copyright (c) 2008-2011 Analog Devices Inc.
 *
 * Licensed under the GPL-2 or later.
 */

#include <common.h>
#include <command.h>
#include <errno.h>
#include <dm.h>
#include <asm/gpio.h>
#include <console.h>


static int do_blink(cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[])
{

#if 1	
	unsigned int gpio;
	const char *str_gpio = NULL;
	int ret,loop;
	bool i = false;

	if (argc < 1)
 show_usage:
		return CMD_RET_USAGE;

	str_gpio = argv[1];


	if (!str_gpio)
		goto show_usage;


#if defined(CONFIG_DM_GPIO)
	/*
	 * TODO(sjg@chromium.org): For now we must fit into the existing GPIO
	 * framework, so we look up the name here and convert it to a GPIO number.
	 * Once all GPIO drivers are converted to driver model, we can change the
	 * code here to use the GPIO uclass interface instead of the numbered
	 * GPIO compatibility layer.
	 */
	ret = gpio_lookup_name(str_gpio, NULL, NULL, &gpio);
	if (ret) {
		printf("GPIO: '%s' not found\n", str_gpio);
		return cmd_process_error(cmdtp, ret);
	}
#else
	/* turn the gpio name into a gpio number */
	gpio = name_to_gpio(str_gpio);
	if (gpio < 0)
		goto show_usage;
#endif
	/* grab the pin before we tweak it */
	ret = gpio_request(gpio, "cmd_gpio");
	if (ret && ret != -EBUSY) {
		printf("gpio: requesting pin %u failed\n", gpio);
		return -1;
	}

	/* finally, let's do it: set direction and exec command */
	
            /*  for(loop = 0; loop <20; loop++)
	      {  
                   i = !i;
		   gpio_direction_output(gpio,i);
	           mdelay(100);
	      }*/

	while(true)
        {
            i = !i;
            gpio_set_value(gpio,i);
            mdelay(100);

            if(ctrlc()){
                puts("\nAborted\n");
                return -EINTR;
            }
         }
	if (ret != -EBUSY)
                gpio_free(gpio);
	      return 0;
#endif
#if 0
        //int ret, loop;
	bool i=0;
        
	 /* for(loop = 0; loop <20; loop++)
            {  
                   i = !i;
                   gpio_direction_output(10,i);
                   mdelay(100);
            }*/
	while(true)
        {
            i = !i;
            gpio_set_value(10,i);
            mdelay(100);

            if(ctrlc()){
                puts("\nAborted\n");
                return -EINTR;
            }
         }
return 0;
#endif
}

U_BOOT_CMD(blink, 2, 1, do_blink,
	   "blink  gpio pins",
	   "<blink> <pin> \n");
}
