#ifndef _DEV_DRIVER_H_
#define	_DEV_DRIVER_H_
/*
 * led-driver.h 
 * desc: for the driver
 */

// ioctl commamds
#define	DEV_MAGIC		'k'
#define	DEV_MAXNR		3
#define	DEV_LED_OFF		_IO(DEV_MAGIC, 0)
#define	DEV_LED_ON		_IO(DEV_MAGIC, 1)
#define	DEV_GETSTATE	_IOR(DEV_MAGIC, 2, long)

// definitions

#endif	/* _DEV_DRIVER_H_ */

