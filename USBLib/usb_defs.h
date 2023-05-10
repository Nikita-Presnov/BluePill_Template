#ifndef __USB_DEFS_H__
#define __USB_DEFS_H__

#define ENDP_DATA_IN 1
#define ENDP_DATA_OUT 2
#define ENDP_DATA_SIZE 64

#define ENDP_CTL_NUM 3
#define ENDP_CTL_SIZE 8

#define STD_DESCR_LANG 0
#define STD_DESCR_VEND 1
#define STD_DESCR_PROD 2
#define STD_DESCR_SN 3

#define USB_VID 0x16C0
#define USB_PID 0x05DF

#define DEVCLASS_CDC 0x02
#define CDCSUBCLASS_ACM 0x02
#define CDCCLASS_DATA 0x0A
#define CDCPROTOCOL_UNDEF 0x00
#define CDCPROTOCOL_VENDOR 0xFF


#endif /* __USB_DEFS_H__ */
