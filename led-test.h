#ifndef _LED_TEST_H_
#define _LED_TEST_H_
/*
 * led-test.h 
 * Desc: for the library and test
 */

// ioctl commands
#define DEV_MAGIC   'k'
#define DEV_MAXNR   3
#define DEV_LED_OFF     _IO(DEV_MAGIC, 0)
#define DEV_LED_ON      _IO(DEV_MAGIC, 1)
#define DEV_GETSTATE    _IOR(DEV_MAGIC, 2, long)

// definitions

// function prototypes
#ifdef __cplusplus
extern "C"{
#endif 

int device_init(void);
void led_on();
void led_off();
void device_close();
int getLed3State();

#ifdef __cplusplus
}
#endif

#endif  /* _LED_TEST_H_ */

