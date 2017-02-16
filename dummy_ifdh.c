#include <pcsclite.h>
#include <ifdhandler.h>
#include <stdio.h>
#ifdef DEBUG
#include <syslog.h>
#endif /* DEBUG */
#include <string.h>

RESPONSECODE IFDHCreateChannel(DWORD Lun, DWORD Channel)
{
#ifdef DEBUG
	syslog(LOG_USER | LOG_NOTICE, "%s", __func__);
#endif /* DEBUG */

	(void)Lun;
	(void)Channel;

	return IFD_SUCCESS;
}

RESPONSECODE IFDHCloseChannel(DWORD Lun)
{
#ifdef DEBUG
	syslog(LOG_USER | LOG_NOTICE, "%s", __func__);
#endif /* DEBUG */

	(void)Lun;

	return IFD_SUCCESS;
}

RESPONSECODE IFDHGetCapabilities(DWORD Lun, DWORD Tag,
				   PDWORD Length, PUCHAR Value)
{
	unsigned char my_forged_atr[13] = {0x3b, 0xf2, 0x98, 0x00, 0xff, 0xc1, 0x10, 0x31, 0xfe, 0x55, 0xc8, 0x03, 0x15};

#ifdef DEBUG
	syslog(LOG_USER | LOG_NOTICE, "%s", __func__);
#endif /* DEBUG */

	(void)Lun;

	switch(Tag){
	case TAG_IFD_ATR:
#ifdef DEBUG
		syslog(LOG_USER | LOG_NOTICE, "%s%s", __func__, "request = TAG_IFD_ATR");
#endif /* DEBUG */
		*Length = sizeof(my_forged_atr);
		memcpy((void *)Value, (void *)my_forged_atr, sizeof(my_forged_atr));

		break;

	/*case SCARD_ATTR_ATR_STRING:
		syslog(LOG_USER | LOG_NOTICE, "%s%s", "IFDHGetCapabilities(): request = ", "SCARD_ATTR_ATR_STRING");
		break;

	case TAG_IFD_SIMULTANEOUS_ACCESS:
		syslog(LOG_USER | LOG_NOTICE, "%s%s", "IFDHGetCapabilities(): request = ", "TAG_IFD_SIMULTANEOUS_ACCESS");
		break;

	case TAG_IFD_THREAD_SAFE:
		syslog(LOG_USER | LOG_NOTICE, "%s%s", "IFDHGetCapabilities(): request = ", "TAG_IFD_THREAD_SAFE");
		break;

	case TAG_IFD_SLOTS_NUMBER:
		syslog(LOG_USER | LOG_NOTICE, "%s%s", "IFDHGetCapabilities(): request = ", "TAG_IFD_SLOTS_NUMBER");
		break;
	case TAG_IFD_SLOT_THREAD_SAFE:
		syslog(LOG_USER | LOG_NOTICE, "%s%s", "IFDHGetCapabilities(): request = ", "TAG_IFD_SLOT_THREAD_SAFE");
		break;
	case IOCTL_SMARTCARD_VENDOR_VERIFY_PIN:
		syslog(LOG_USER | LOG_NOTICE, "%s%s", "IFDHGetCapabilities(): request = ", "IOCTL_SMARTCARD_VENDOR_VERIFY_PIN");
		break;*/

	default:
#ifdef DEBUG
		syslog(LOG_USER | LOG_NOTICE, "%s%s ( 0x%lx )", __func__, "request = UNKNOWN", Tag);
#endif /* DEBUG */

		return IFD_ERROR_TAG;
	}

	return IFD_SUCCESS;
}

RESPONSECODE IFDHSetCapabilities(DWORD Lun, DWORD Tag,
		DWORD Length, PUCHAR Value)
{
#ifdef DEBUG
	syslog(LOG_USER | LOG_NOTICE, "%s", __func__);
#endif /* DEBUG */

	(void)Lun;
	(void)Tag;
	(void)Length;
	(void)Value;

	return IFD_SUCCESS;
}

RESPONSECODE IFDHSetProtocolParameters(DWORD Lun, DWORD Protocol,
				   UCHAR Flags, UCHAR PTS1,
				   UCHAR PTS2, UCHAR PTS3)
{
#ifdef DEBUG
	syslog(LOG_USER | LOG_NOTICE, "%s", __func__);
#endif /* DEBUG */

	(void)Lun;
	(void)Flags;
	(void)PTS1;
	(void)PTS2;
	(void)PTS3;

	switch(Protocol){
	case SCARD_PROTOCOL_T0:
#ifdef DEBUG
		syslog(LOG_USER | LOG_NOTICE, "%s%s", __func__, ": proto = SCARD_PROTOCOL_T0");
#endif /* DEBUG */
		break;
	case SCARD_PROTOCOL_T1:
#ifdef DEBUG
		syslog(LOG_USER | LOG_NOTICE, "%s%s", __func__, ": proto = SCARD_PROTOCOL_T1 - returning IFD_PROTOCOL_NOT_SUPPORTED");
#endif /* DEBUG */
		return(IFD_PROTOCOL_NOT_SUPPORTED);
		break;
	default:
#ifdef DEBUG
		syslog(LOG_USER | LOG_NOTICE, "%s%s", __func__, ": proto = UNKNOWN");
#endif /* DEBUG */
		break;
	}

	return IFD_SUCCESS;
}


RESPONSECODE IFDHPowerICC(DWORD Lun, DWORD Action,
			    PUCHAR Atr, PDWORD AtrLength)
{
	unsigned char my_forged_atr[13] = {0x3b, 0xf2, 0x98, 0x00, 0xff, 0xc1, 0x10, 0x31, 0xfe, 0x55, 0xc8, 0x03, 0x15};

#ifdef DEBUG
	syslog(LOG_USER | LOG_NOTICE, "%s", __func__);
#endif /* DEBUG */

	(void)Lun;

	switch(Action){
	case IFD_POWER_UP:
#ifdef DEBUG
		syslog(LOG_USER | LOG_NOTICE, "%s%s", __func__, ": ACTION = IFD_POWER_UP");
#endif /* DEBUG */

		*AtrLength = sizeof(my_forged_atr);
		memcpy((void *)Atr, (void *)my_forged_atr, sizeof(my_forged_atr));

		break;
	case IFD_POWER_DOWN:
#ifdef DEBUG
		syslog(LOG_USER | LOG_NOTICE, "%s%s", __func__, ": ACTION = IFD_RESET");
#endif /* DEBUG */

		memset((void *)Atr, (int)0, *AtrLength);
		*AtrLength = 0;

		break;
	case IFD_RESET:
#ifdef DEBUG
		syslog(LOG_USER | LOG_NOTICE, "%s%s", __func__, ": ACTION = IFD_RESET");
#endif /* DEBUG */

		*AtrLength = sizeof(my_forged_atr);
		memcpy((void *)Atr, (void *)my_forged_atr, sizeof(my_forged_atr));

		break;
	default:
#ifdef DEBUG
		syslog(LOG_USER | LOG_NOTICE, "%s%s", __func__, ": ACTION = UNKNOWN");
#endif /* DEBUG */
		break;
	}

	return IFD_SUCCESS;
}

RESPONSECODE IFDHTransmitToICC( DWORD Lun, SCARD_IO_HEADER SendPci,
				 PUCHAR TxBuffer, DWORD TxLength,
				 PUCHAR RxBuffer, PDWORD RxLength,
				 PSCARD_IO_HEADER RecvPci)
{
	int i;

	struct dummy_comm {
		unsigned char command[64];
		int commlen;
		unsigned char reply[64];
		int replylen;

	} my_dummy_comm[4] = {
		{{0x00, 0xCA, 0x01, 0x81, 0x00}, 5, {0x33, 0x66, 0x00, 0x45, 0xFF, 0xFF, 0xFF, 0xFF, 0x60, 0xFF, 0x24, 0x7E, 0x4F, 0x0B, 0x10, 0x16, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x90, 0x00}, 34},
		{{0x00, 0xA4, 0x08, 0x00, 0x06, 0x66, 0x66, 0x10, 0x00, 0x00, 0x01, 0x00}, 12, {0x6F, 0x12, 0x81, 0x02, 0x00, 0x0B, 0x82, 0x02, 0x01, 0x21, 0x83, 0x02, 0x00, 0x01, 0x85, 0x01, 0x01, 0x86, 0x01, 0x00, 0x90, 0x00}, 22},
		{{0x00, 0xA4, 0x08, 0x0C, 0x06, 0x66, 0x66, 0x10, 0x00, 0x00, 0x01}, 11, {0x90, 0x00}, 2},
		{{0x00, 0xB0, 0x00, 0x00, 0x0B}, 5, {0xFF, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x90, 0x00}, 13}
	};

	(void)Lun;
	(void)SendPci;
	(void)RecvPci;

#ifdef DEBUG
	char syslog_buf[1024];
	memset((void *)syslog_buf, (int)0, sizeof(syslog_buf));
	syslog(LOG_USER | LOG_NOTICE, "%s", __func__);
	syslog(LOG_USER | LOG_NOTICE, "%s%lu", "Proto:", SendPci.Protocol);

	for( i = 0; i < TxLength; i++) sprintf(&syslog_buf[i * 5], "0x%02x ", TxBuffer[i]);

	syslog(LOG_USER | LOG_NOTICE, "Data: %s", syslog_buf);
#endif /* DEBUG */


	for (i = 0; i < 4; i++){

		if (!memcmp((void *)my_dummy_comm[i].command, (void *)TxBuffer, TxLength)) {
			memcpy((void *)RxBuffer, (void *)my_dummy_comm[i].reply,  my_dummy_comm[i].replylen);
			*RxLength = my_dummy_comm[i].replylen;
			return IFD_SUCCESS;
		}
	}

	return IFD_SUCCESS;
}


RESPONSECODE IFDHControl(DWORD Lun,
		DWORD dwControlCode,
		PUCHAR TxBuffer,
		DWORD TxLength,
		PUCHAR RxBuffer,
		DWORD RxLength,
		PDWORD pdwBytesReturned)
{
#ifdef DEBUG
	syslog(LOG_USER | LOG_NOTICE, "%s", __func__);
#endif /* DEBUG */

	(void)Lun;
	(void)dwControlCode;
	(void)TxBuffer;
	(void)TxLength;
	(void)RxBuffer;
	(void)RxLength;
	(void)pdwBytesReturned;

	return IFD_ICC_PRESENT;
}

RESPONSECODE IFDHICCPresence(DWORD Lun)
{
#ifdef DEBUG
	syslog(LOG_USER | LOG_NOTICE, "%s", __func__);
#endif /* DEBUG */

	(void)Lun;

	return IFD_ICC_PRESENT;
}
