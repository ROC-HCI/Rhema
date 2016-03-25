#ifndef NMSP_DEFINES_INCLUDED
#define NMSP_DEFINES_INCLUDED
/*-----------------------------------------------------------------
   Note -- this header file is annotated with markup for Doxygen,
   a documentation tool for C and C++. If you are just reading
   the header file, you should ignore such markup as \c, \section
   \param, and so on.
  -----------------------------------------------------------------*/

/** \file nmsp_defines.h
	
				NMSP Common Defines
   
   \section Legal_Notice Legal Notice
 
   Copyright 2006-2011, Nuance Communications Inc. All rights reserved.
 
   Nuance Communications, Inc. provides this document without representation
   or warranty of any kind. The information in this document is subject to
   change without notice and does not represent a commitment by Nuance
   Communications, Inc. The software and/or databases described in this
   document are furnished under a license agreement and may be used or
   copied only in accordance with the terms of such license agreement.
   Without limiting the rights under copyright reserved herein, and except
   as permitted by such license agreement, no part of this document may be
   reproduced or transmitted in any form or by any means, including, without
   limitation, electronic, mechanical, photocopying, recording, or otherwise,
   or transferred to information storage and retrieval systems, without the
   prior written permission of Nuance Communications, Inc.
 
   Nuance, the Nuance logo, Nuance Recognizer, and Nuance Vocalizer are
   trademarks or registered trademarks of Nuance Communications, Inc. or its
   affiliates in the United States and/or other countries. All other
   trademarks referenced herein are the property of their respective owners.
   
 
    \section def-sec1 Overview

    Defines constants for configuration parameters and for codec types.

*/
#ifdef __cplusplus
extern "C" {
#endif


    /**
    \def NMSP_DEFINES_CONNECTION_TIMEOUT_PARAM
         Defines the ConnectionTimeout parameter, which specifies the maximum time, in seconds, from
         the creation of the Command until the connection to the dictation server is established and a
         COP CONNECTION is received. Can be used as a key for the properties in the property list.
         Must be an integer value greater than 0. Default = 30.
    \def NMSP_DEFINES_CONNECTION_TIMEOUT_DEFAULT
          Defines the default value of the ConnectionTimeout parameter.
    \def NMSP_DEFINES_IDLE_SESSION_TIMEOUT_PARAM
         Defines the IdleSessionTimeout parameter, which specifies the maximum time, in seconds,
         that a session can remain idle. A session is considered to be idle when no messages are
         sent to or received by the NMSP Gateway from the mobile device.
         Can be used as a key for the properties in the property list. Must be an integer value
         greater than 0. Default = 45.
    \def NMSP_DEFINES_IDLE_SESSION_TIMEOUT_DEFAULT
         Defines the default value of the IdleSessionTimeout parameter.
    \def NMSP_DEFINES_PING_INTERVAL_PARAM
         Defines the PingInterval parameter, which specifies the maximum time interval, in seconds, between
         pings from the NMSP Gateway. The ping is sent in the COP CONNECTED. Can be used as a key for the properties
         in the property list. Must be an integer value equal to or greater than 0. Default = 0.
    \def NMSP_DEFINES_PING_INTERVAL_DEFAULT
         Defines the default value of the PingInterval parameter.
    \def NMSP_DEFINES_MISSED_PINGS_BEFORE_DISCONNECT_PARAM
         Defines the MissedPingsBeforeDisconnect parameter, which specifies the maximum number of missed ping
         responses from the client before the NMSP Gateway disconnects. The ping is sent in the COP CONNECTED. Can
         be used as a key for the properties in the property list. Must be an integer value greater than 0. Default = 3.
    \def NMSP_DEFINES_MISSED_PINGS_BEFORE_DISCONNECT_DEFAULT
         Defines the default value of the MissedPingsBeforeDisconnect parameter.
	\def NMSP_DEFINES_ENDPOINTER
		 Define that allows enabling the endpointer which automatically detects the start of speech and
		 end of speech when recording.
    \def NMSP_DEFINES_USE_ENERGY_LEVEL
         Defines the energy level parameter, which specifies if a notification must be sent using the
         NMSP_AUDIO_RECORD_BUFFER_RECORDED event when a change in the audio energy level occurs.
	\def NMSP_DEFINES_AUDIO_INPUTSOURCE
		 Allows users to specify the input audio source.
	\def NMSP_DEFINES_AUDIO_SAMPLERATE
		 Allows users to specify the input audio sample rate.
	\def NMSP_DEFINES_SSL_SOCKET
		 Enables the SSL soceket connection.
	\def NMSP_DEFINES_SSL_CERT_SUMMARY
		 Presets the SSL certificate summary, which will be compared with the one replied by the server for verification. 
	\def NMSP_DEFINES_SSL_CERT_DATA
		 Presets the SSL certificate data, which will be compared with the data replied by the server for verification. 
	\def NMSP_DEFINES_SSL_SELFSIGNED_CERT
		 Lets the server use its self-signed certificate for development purposes. 
	\def NMSP_DEFINES_VERSION
		 The NMSP SDK version will be updated only when a new feature set is 
		 released by product management. The build should be updated each time 
		 the main R&D branch is certified by QA and integrated into the release 
		 branch. In addition to the NMSP SDK version and build information defined here,
		 the API function nmsp_getVersion() also returns version information.
	
*/

#define NMSP_DEFINES_CONNECTION_TIMEOUT_PARAM                "ConnectionTimeout"
#define NMSP_DEFINES_CONNECTION_TIMEOUT_DEFAULT              30
#define NMSP_DEFINES_IDLE_SESSION_TIMEOUT_PARAM              "IdleSessionTimeout"
#define NMSP_DEFINES_IDLE_SESSION_TIMEOUT_DEFAULT            45
#define NMSP_DEFINES_PING_INTERVAL_PARAM                     "Ping_IntervalSecs"
#define NMSP_DEFINES_PING_INTERVAL_DEFAULT                   0
#define NMSP_DEFINES_MISSED_PINGS_BEFORE_DISCONNECT_PARAM    "Ping_MissedResponsesBeforeDisconnect"
#define NMSP_DEFINES_MISSED_PINGS_BEFORE_DISCONNECT_DEFAULT  3
#define NMSP_DEFINES_ENDPOINTER                              "ep.StopOnEndOfSpeech"
#define NMSP_DEFINES_USE_ENERGY_LEVEL                        "Energy_Level"
#define NMSP_DEFINES_AUDIO_INPUTSOURCE                       "Audio_Source"
#define NMSP_DEFINES_AUDIO_SAMPLERATE                        "Audio_HWSampleRate"
#define NMSP_DEFINES_SSL_SOCKET								 "SSL_Socket_Enable"
#define NMSP_DEFINES_SSL_CERT_SUMMARY						 "SSL_Cert_Summary"
#define NMSP_DEFINES_SSL_CERT_DATA							 "SSL_Cert_Data"
#define NMSP_DEFINES_SSL_SELFSIGNED_CERT					 "SSL_SelfSigned_Cert"
#define NMSP_DEFINES_VERSION								 "NMSP 3.8 client SDK - build 16"

/** \typedef NMSP_DEFINES_CODEC

    Defines constants for codec types.
 **/
typedef enum {
    /* invalid codec or codec unavailable*/
    CODEC_NONE = -1,					/*!< Used to indicate an invalid or unspecified codec */

    /* PCM 16k */
    CODEC_PCM_16_8K  = 0x0002,			/*!< Pulse code modulation with 16-bit quantization and  8KHz sampling rate */
    CODEC_PCM_16_11K = 0x0004,			/*!< Pulse code modulation with 16-bit quantization and 11KHz sampling rate */
    CODEC_PCM_16_16K = 0x0005,			/*!< Pulse code modulation with 16-bit quantization and 16KHz sampling rate */

    /* u-law */
    CODEC_ULAW       = 0x0008,			/*!< u-law(mu-law) codec */

    /* DSR */
    //CODEC_DSR_8K     = 0x0030,
    //CODEC_DSR_11K    = 0x0031,

    /* G.729 */
    //CODEC_G729_64    = 0x0041, /* 6.4 Kbps */
    //CODEC_G729_80    = 0x0042, /* 8.0 Kbps */
    //CODEC_G729_118   = 0x0043, /* 11.6 Kbps */

    /* AMR */
    CODEC_AMR_00     = 0x0050,			/*!< Adaptive Multi-Rate codec 00 */
    CODEC_AMR_01     = 0x0051,			/*!< Adaptive Multi-Rate codec 01 */
    CODEC_AMR_02     = 0x0052,			/*!< Adaptive Multi-Rate codec 02 */
    CODEC_AMR_03     = 0x0053,			/*!< Adaptive Multi-Rate codec 03 */
    CODEC_AMR_04     = 0x0054,			/*!< Adaptive Multi-Rate codec 04 */
    CODEC_AMR_05     = 0x0055,			/*!< Adaptive Multi-Rate codec 05 */
    CODEC_AMR_06     = 0x0056,			/*!< Adaptive Multi-Rate codec 06 */	
    CODEC_AMR_07     = 0x0057,			/*!< Adaptive Multi-Rate codec 07 */

    /* AMR Non bit-ordering */
    CODEC_AMR_NBO_R0 = 0x0080,			/*!< Non Bit-Ordering Adaptive Multi-Rate codec 00 */
    CODEC_AMR_NBO_R1 = 0x0081,			/*!< Non Bit-Ordering Adaptive Multi-Rate codec 01 */
    CODEC_AMR_NBO_R2 = 0x0082,			/*!< Non Bit-Ordering Adaptive Multi-Rate codec 02 */
    CODEC_AMR_NBO_R3 = 0x0083,			/*!< Non Bit-Ordering Adaptive Multi-Rate codec 03 */
    CODEC_AMR_NBO_R4 = 0x0084,			/*!< Non Bit-Ordering Adaptive Multi-Rate codec 04 */
    CODEC_AMR_NBO_R5 = 0x0085,			/*!< Non Bit-Ordering Adaptive Multi-Rate codec 05 */
    CODEC_AMR_NBO_R6 = 0x0086,			/*!< Non Bit-Ordering Adaptive Multi-Rate codec 06 */
    CODEC_AMR_NBO_R7 = 0x0087,			/*!< Non Bit-Ordering Adaptive Multi-Rate codec 07 */

    /* QCP */
    CODEC_QCELP_13   = 0x0062,			/*!< Qualcomm Code Excited Linear Prediction (QCELP) codec 13Kbits/s */
    CODEC_QCELP_VAR  = 0x0063,			/*!< Qualcomm Code Excited Linear Prediction (QCELP) codec variable rate */

    /* EVRC */
    CODEC_EVRC       = 0x0070,			/*!< Enhanced Variable Rate CODEC (EVRC) */

    /* iLBC */
    CODEC_ILBC       = 0x0090,			/*!< Internet Low Bit Rate Codec (iLBC) */

    /* Speex */
	CODEC_SPEEX_8K		= 0x0106,		/*!< Speex wideband (8 kHz) */
	CODEC_SPEEX_16K		= 0x0116,		/*!< Speex wideband (16 kHz) */
	CODEC_SPEEX_11K		= 0x0168		/*!< Speex wideband (11 kHz) */
} NMSP_DEFINES_CODEC;

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* NMSP_DEFINES_INCLUDED */
