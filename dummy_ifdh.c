#include <pcsclite.h>
#include <ifdhandler.h>
#include <stdio.h>
#include <string.h>

#ifdef DEBUG
#include <syslog.h>
#endif /* DEBUG */

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
		syslog(LOG_USER | LOG_NOTICE, "%s%s", __func__, ": request = TAG_IFD_ATR");
#endif /* DEBUG */
		*Length = sizeof(my_forged_atr);
		memcpy((void *)Value, (void *)my_forged_atr, sizeof(my_forged_atr));

		break;
	case TAG_IFD_SIMULTANEOUS_ACCESS:
#ifdef DEBUG
		syslog(LOG_USER | LOG_NOTICE, "%s%s", __func__, ": request = TAG_IFD_SIMULTANEOUS_ACCESS");
#endif /* DEBUG */
		break;

	case TAG_IFD_THREAD_SAFE:
#ifdef DEBUG
		syslog(LOG_USER | LOG_NOTICE, "%s%s", __func__, ": request = TAG_IFD_THREAD_SAFE");
#endif /* DEBUG */
		break;

	case TAG_IFD_SLOTS_NUMBER:
#ifdef DEBUG
		syslog(LOG_USER | LOG_NOTICE, "%s%s", __func__, ": request = TAG_IFD_SLOTS_NUMBER");
#endif /* DEBUG */
		*Length = 1;
		*Value = 1;
		break;
	case TAG_IFD_SLOT_THREAD_SAFE:
#ifdef DEBUG
		syslog(LOG_USER | LOG_NOTICE, "%s%s", __func__, ": request = TAG_IFD_SLOT_THREAD_SAFE");
#endif /* DEBUG */
		break;

	default:
#ifdef DEBUG
		syslog(LOG_USER | LOG_NOTICE, "%s%s ( 0x%lx )", __func__, ": request = UNKNOWN", Tag);
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
		syslog(LOG_USER | LOG_NOTICE, "%s%s", __func__, ": proto = SCARD_PROTOCOL_T1");
#endif /* DEBUG */

		//return IFD_PROTOCOL_NOT_SUPPORTED;
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

	unsigned short reslen;

	(void)Lun;
	(void)SendPci;
	(void)RecvPci;

	unsigned char trigger_apdu[6] = {0x00, 0xCA, 0x01, 0x81, 0x00, 0x00};


	if (TxLength == sizeof(trigger_apdu)) {

		if (!memcmp((void *)trigger_apdu, (void *)TxBuffer, 4)) {

			reslen = (TxBuffer[4] << 8) | TxBuffer[5];

			/* Add up SW1(2) */
			reslen += 2;

#ifdef DEBUG
			syslog(LOG_USER | LOG_NOTICE, "%s: reslen = %d RxLength = %lu", __func__, reslen, *RxLength);
#endif /* DEBUG */

			if (reslen > *RxLength) {
				RxBuffer[0] = 0x67;
				RxBuffer[1] = 0x00;
				*RxLength = 2;

				return IFD_SUCCESS;
			}

			for (i = 0; i < reslen - 2; i++) {
				RxBuffer[i] = 0xAA;
			}

			RxBuffer[reslen - 2] = 0x90;
			RxBuffer[reslen - 1] = 0x00;

			*RxLength = reslen;

			return IFD_SUCCESS;
		}
	}

	/* All other APDUs result in 0x9000 */
	RxBuffer[0] = 0x90;
	RxBuffer[1] = 0x00;
	*RxLength = 2;

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
	//syslog(LOG_USER | LOG_NOTICE, "%s", __func__);
#endif /* DEBUG */

	(void)Lun;

	return IFD_ICC_PRESENT;
}
