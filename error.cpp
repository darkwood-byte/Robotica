#define Push_error()\
    do{\
        gpio_put(18, 0);\
        gpio_put(19, 0);\
        gpio_put(20, 1);\
        gpio_put(21, 0);\
        sleep_ms(1000);\
        gpio_put(20, 0);\
    }while(0)
