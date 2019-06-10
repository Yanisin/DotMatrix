#ifndef _CDCACM_H
#define _CDCACM_H

extern void cdcacm_write_char(char c);
extern uint32_t cdcacm_get_char(void);
extern int cdcacm_is_on(void);

#endif
