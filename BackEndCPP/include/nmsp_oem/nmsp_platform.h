/*---------------------------------------------------------------------------*
 * Copyright 2006-2011, Nuance Communications Inc. All rights reserved.      *
 *                                                                           *
 * Nuance Communications, Inc. provides this document without representation *
 * or warranty of any kind. The information in this document is subject to   *
 * change without notice and does not represent a commitment by Nuance       *
 * Communications, Inc. The software and/or databases described in this      *
 * document are furnished under a license agreement and may be used or       *
 * copied only in accordance with the terms of such license agreement.       *
 * Without limiting the rights under copyright reserved herein, and except   *
 * as permitted by such license agreement, no part of this document may be   *
 * reproduced or transmitted in any form or by any means, including, without *
 * limitation, electronic, mechanical, photocopying, recording, or otherwise,*
 * or transferred to information storage and retrieval systems, without the  *
 * prior written permission of Nuance Communications, Inc.                   *
 *                                                                           *
 * Nuance, the Nuance logo, Nuance Recognizer, and Nuance Vocalizer are      *
 * trademarks or registered trademarks of Nuance Communications, Inc. or its *
 * affiliates in the United States and/or other countries. All other         *
 * trademarks referenced herein are the property of their respective owners. *
 *---------------------------------------------------------------------------*/
#ifndef NMSP_PLATFORM_INCLUDED
#define NMSP_PLATFORM_INCLUDED

#if defined(_BREW_STATIC)
    #include "AEESTDLib.h"
#else
    // Brew is using macros for standard C lib functions.
    // The SDK code must use these macros 
    // so we define them here for other platforms.
    #define MEMCPY           memcpy
    #define MEMMOVE          memmove
    #define MEMSET           memset
    #define MEMCMP           memcmp
    #define MEMCHR           memchr
    #define MEMSTR           memstr
    #define MEMRCHR          memrchr
    #define MEMCHREND        memchrend
    #define MEMRCHRBEGIN     memrchrbegin
    #define MSLEEP           sleep
    #define STRCPY           strcpy
    #define STRNCPY          strncpy
    #define STRNCMP          strncmp
    #define STRICMP          stricmp
    #define STRNICMP         strnicmp
    #define STRCAT           strcat
    #define STRCMP           strcmp
    #define STRLEN           strlen
    #define STRCHR           strchr
    #define STRCHREND        strchrend
    #define STRCHRSEND       strchrsend
    #define STRRCHR          strrchr
    #define STRSTR           strstr
    #define STRISTR          stristr
    #define STRBEGINS        strbegins
    #define STRIBEGINS       stribegins
    #define STRENDS          strends
    #define STRLOWER         strlower
    #define STRUPPER         strupper
    #define SPRINTF          sprintf
    #define SNPRINTF         snprintf
    #define STRTOUL          strtoul
    #define STRTOD           strtod
    #define STRLCPY          strlcpy
    #define STRLCAT          strlcat
    #define WSTRCPY          wstrcpy
    #define WSTRCAT          wstrcat
    #define WSTRCMP          wstrcmp
    #define WSTRNCMP         wstrncmp
    #define WSTRICMP         wstricmp
    #define WSTRNICMP        wstrnicmp
    #define WSTRLEN          wstrlen
    #define WSTRCHR          wstrchr
    #define WSTRRCHR         wstrrchr
    #define WSPRINTF         wsprintf
    #define STRTOWSTR        strtowstr
    #define WSTRTOSTR        wstrtostr
    #define WSTRTOFLOAT      wstrtofloat
    #define FLOATTOWSTR      floattowstr
    #define UTF8TOWSTR       utf8towstr
    #define WSTRTOUTF8       wstrtoutf8
    #define WSTRLOWER        wstrlower
    #define WSTRUPPER        wstrupper
    #define WSTRLCPY         wstrlcpy
    #define WSTRLCAT         wstrlcat
    #define GETCHTYPE        chartype
    #define ATOI             atoi
    #define WSTRCOMPRESS     wstrcompress
    #define STREXPAND        strexpand
    #define LOCALTIMEOFFSET  LocalTimeOffset
    #define GETAPPINSTANCE   GetAppInstance
    #define GETALSCONTEXT    GetALSContext
    #define QSORT            qsort
    #define MALLOC           malloc
    #define REALLOC          realloc
    #define WSTRDUP          wstrdup
    #define STRDUP           strdup
#endif

#if defined(_BREW_STATIC)
    #define NMSP_IMPORT_EXPORT_CONVENTION
    #define VST_IMPORT_EXPORT_CONVENTION

	#define NMSP_CORE_EXPORT 
    #define NMSP_RESOURCE_COMMON_EXPORT
    #define NMSP_RESOURCE_COMMON_RECOGNITION_EXPORT
    #define NMSP_RESOURCE_DICTATION_EXPORT
	#define NMSP_RESOURCE_NMAS_EXPORT 
	#define NMSP_RESOURCE_RECOGNITION_EXPORT 
	#define NMSP_RESOURCE_TTS_EXPORT 
	#define NMSP_AUDIOPLAYBACK_EXPORT 
	#define NMSP_AUDIORECORD_EXPORT 
    #define NMSP_DICTATIONRESULT_EXPORT

#elif defined(_BREW)
    #if defined(_AEE_SIMULATOR)
        #undef NMSP_IMPORT_EXPORT_CONVENTION
        #undef VST_IMPORT_EXPORT_CONVENTION
        #undef NMSP_RESOURCE_COMMON_EXPORT
        #undef NMSP_RESOURCE_COMMON_RECOGNITION_EXPORT
        #undef NMSP_RESOURCE_DICTATION_EXPORT
        #if defined (NMSP_MEM_LEAK)
			#define NMSP_IMPORT_EXPORT_CONVENTION
			#define VST_IMPORT_EXPORT_CONVENTION

			#define NMSP_CORE_EXPORT 
			#define NMSP_RESOURCE_COMMON_EXPORT
			#define NMSP_RESOURCE_COMMON_RECOGNITION_EXPORT
			#define NMSP_RESOURCE_DICTATION_EXPORT
			#define NMSP_RESOURCE_NMAS_EXPORT 
			#define NMSP_RESOURCE_RECOGNITION_EXPORT 
			#define NMSP_RESOURCE_TTS_EXPORT 
			#define NMSP_AUDIOPLAYBACK_EXPORT 
			#define NMSP_AUDIORECORD_EXPORT 
			#define NMSP_DICTATIONRESULT_EXPORT
		#else
			#ifdef NMSP_OEM_EXPORTS
			  #define NMSP_IMPORT_EXPORT_CONVENTION __declspec(dllexport)
			  #define VST_IMPORT_EXPORT_CONVENTION __declspec(dllexport)
			#else
			  #define NMSP_IMPORT_EXPORT_CONVENTION __declspec(dllimport)
			  #define VST_IMPORT_EXPORT_CONVENTION __declspec(dllimport)
			#endif

			#ifdef NMSP_CORE_EXPORTS
				#define NMSP_CORE_EXPORT __declspec(dllexport)
			#else 
				#define NMSP_CORE_EXPORT __declspec(dllimport)
			#endif

			#ifdef NMSP_RESOURCE_COMMON_EXPORTS
				#define NMSP_RESOURCE_COMMON_EXPORT __declspec(dllexport)
			#else
				#define NMSP_RESOURCE_COMMON_EXPORT __declspec(dllimport)
			#endif

			#ifdef NMSP_RESOURCE_COMMON_REC_EXPORTS
				#define NMSP_RESOURCE_COMMON_RECOGNITION_EXPORT __declspec(dllexport)
			#else
				#define NMSP_RESOURCE_COMMON_RECOGNITION_EXPORT __declspec(dllimport)
			#endif

			#ifdef NMSP_RESOURCE_DICTATION_EXPORTS
				#define NMSP_RESOURCE_DICTATION_EXPORT __declspec(dllexport)
			#else
				#define NMSP_RESOURCE_DICTATION_EXPORT __declspec(dllimport)
			#endif

			#ifdef NMSP_RESOURCE_NMAS_EXPORTS
				#define NMSP_RESOURCE_NMAS_EXPORT __declspec(dllexport)
			#else
				#define NMSP_RESOURCE_NMAS_EXPORT __declspec(dllimport)
			#endif

			#ifdef NMSP_RESOURCE_RECOGNITION_EXPORTS
				#define NMSP_RESOURCE_RECOGNITION_EXPORT __declspec(dllexport)
			#else
				#define NMSP_RESOURCE_RECOGNITION_EXPORT __declspec(dllimport)
			#endif

			#ifdef NMSP_RESOURCE_TTS_EXPORTS
				#define NMSP_RESOURCE_TTS_EXPORT __declspec(dllexport)
			#else
				#define NMSP_RESOURCE_TTS_EXPORT __declspec(dllimport)
			#endif

			#ifdef NMSP_AUDIOPLAYBACK_EXPORTS
				#define NMSP_AUDIOPLAYBACK_EXPORT __declspec(dllexport)
			#else
				#define NMSP_AUDIOPLAYBACK_EXPORT __declspec(dllimport)
			#endif

			#ifdef NMSP_AUDIORECORD_EXPORTS
				#define NMSP_AUDIORECORD_EXPORT __declspec(dllexport)
			#else
				#define NMSP_AUDIORECORD_EXPORT __declspec(dllimport)
			#endif

			#ifdef NMSP_DICTATION_EXPORTS
				#define NMSP_DICTATIONRESULT_EXPORT __declspec(dllexport)
			#else
				#define NMSP_DICTATIONRESULT_EXPORT __declspec(dllimport)
			#endif

        #endif
    #else
		#define NMSP_IMPORT_EXPORT_CONVENTION
		#define VST_IMPORT_EXPORT_CONVENTION

		#define NMSP_CORE_EXPORT 
		#define NMSP_RESOURCE_COMMON_EXPORT
		#define NMSP_RESOURCE_COMMON_RECOGNITION_EXPORT
		#define NMSP_RESOURCE_DICTATION_EXPORT
		#define NMSP_RESOURCE_NMAS_EXPORT 
		#define NMSP_RESOURCE_RECOGNITION_EXPORT 
		#define NMSP_RESOURCE_TTS_EXPORT 
		#define NMSP_AUDIOPLAYBACK_EXPORT 
		#define NMSP_AUDIORECORD_EXPORT 
		#define NMSP_DICTATIONRESULT_EXPORT
    #endif
#elif defined(__APPLE_CC__)
    #define NMSP_IMPORT_EXPORT_CONVENTION
    #define VST_IMPORT_EXPORT_CONVENTION

	#define NMSP_CORE_EXPORT 
    #define NMSP_RESOURCE_COMMON_EXPORT
    #define NMSP_RESOURCE_COMMON_RECOGNITION_EXPORT
    #define NMSP_RESOURCE_DICTATION_EXPORT
	#define NMSP_RESOURCE_NMAS_EXPORT 
	#define NMSP_RESOURCE_RECOGNITION_EXPORT 
	#define NMSP_RESOURCE_TTS_EXPORT 
	#define NMSP_AUDIOPLAYBACK_EXPORT 
	#define NMSP_AUDIORECORD_EXPORT 
    #define NMSP_DICTATIONRESULT_EXPORT

    #define NMSP_CPL_ENABLED

#elif defined(QT_CORE_LIB)

    #define NMSP_IMPORT_EXPORT_CONVENTION
    #define VST_IMPORT_EXPORT_CONVENTION

	#define NMSP_CORE_EXPORT 
    #define NMSP_RESOURCE_COMMON_EXPORT
    #define NMSP_RESOURCE_COMMON_RECOGNITION_EXPORT
    #define NMSP_RESOURCE_DICTATION_EXPORT
	#define NMSP_RESOURCE_NMAS_EXPORT 
	#define NMSP_RESOURCE_RECOGNITION_EXPORT 
	#define NMSP_RESOURCE_TTS_EXPORT 
	#define NMSP_AUDIOPLAYBACK_EXPORT 
	#define NMSP_AUDIORECORD_EXPORT 
    #define NMSP_DICTATIONRESULT_EXPORT

    #define NMSP_CPL_ENABLED

#elif defined(SYMBIAN_S60_MR)

    //#define NMSP_IMPORT_EXPORT_CONVENTION IMPORT_C
    //#define VST_IMPORT_EXPORT_CONVENTION IMPORT_C

    #define NMSP_IMPORT_EXPORT_CONVENTION
    #define VST_IMPORT_EXPORT_CONVENTION

	#define NMSP_CORE_EXPORT
    #define NMSP_RESOURCE_COMMON_EXPORT
    #define NMSP_RESOURCE_COMMON_RECOGNITION_EXPORT
    #define NMSP_RESOURCE_DICTATION_EXPORT
	#define NMSP_RESOURCE_NMAS_EXPORT
	#define NMSP_RESOURCE_RECOGNITION_EXPORT
	#define NMSP_RESOURCE_TTS_EXPORT
	#define NMSP_AUDIOPLAYBACK_EXPORT
	#define NMSP_AUDIORECORD_EXPORT
    #define NMSP_DICTATIONRESULT_EXPORT

#elif defined(QNX_650)

    //#define NMSP_IMPORT_EXPORT_CONVENTION IMPORT_C
    //#define VST_IMPORT_EXPORT_CONVENTION IMPORT_C

    #define NMSP_IMPORT_EXPORT_CONVENTION
    #define VST_IMPORT_EXPORT_CONVENTION

	#define NMSP_CORE_EXPORT
    #define NMSP_RESOURCE_COMMON_EXPORT
    #define NMSP_RESOURCE_COMMON_RECOGNITION_EXPORT
    #define NMSP_RESOURCE_DICTATION_EXPORT
	#define NMSP_RESOURCE_NMAS_EXPORT
	#define NMSP_RESOURCE_RECOGNITION_EXPORT
	#define NMSP_RESOURCE_TTS_EXPORT
	#define NMSP_AUDIOPLAYBACK_EXPORT
	#define NMSP_AUDIORECORD_EXPORT
    #define NMSP_DICTATIONRESULT_EXPORT

#elif defined(NMSP_OEM_LINUX)
    #define NMSP_IMPORT_EXPORT_CONVENTION
    #define VST_IMPORT_EXPORT_CONVENTION

	#define NMSP_CORE_EXPORT
    #define NMSP_RESOURCE_COMMON_EXPORT
    #define NMSP_RESOURCE_COMMON_RECOGNITION_EXPORT
    #define NMSP_RESOURCE_DICTATION_EXPORT
	#define NMSP_RESOURCE_NMAS_EXPORT
	#define NMSP_RESOURCE_RECOGNITION_EXPORT
	#define NMSP_RESOURCE_TTS_EXPORT
	#define NMSP_AUDIOPLAYBACK_EXPORT
	#define NMSP_AUDIORECORD_EXPORT
    #define NMSP_DICTATIONRESULT_EXPORT

#else
    #undef NMSP_IMPORT_EXPORT_CONVENTION
    #undef VST_IMPORT_EXPORT_CONVENTION
    #if defined (NMSP_MEM_LEAK)
		#define NMSP_IMPORT_EXPORT_CONVENTION
		#define VST_IMPORT_EXPORT_CONVENTION

		#define NMSP_CORE_EXPORT 
		#define NMSP_RESOURCE_COMMON_EXPORT
		#define NMSP_RESOURCE_COMMON_RECOGNITION_EXPORT
		#define NMSP_RESOURCE_DICTATION_EXPORT
		#define NMSP_RESOURCE_NMAS_EXPORT 
		#define NMSP_RESOURCE_RECOGNITION_EXPORT 
		#define NMSP_RESOURCE_TTS_EXPORT 
		#define NMSP_AUDIOPLAYBACK_EXPORT 
		#define NMSP_AUDIORECORD_EXPORT 
		#define NMSP_DICTATIONRESULT_EXPORT
    #else

	#ifdef NMSP_OEM_EXPORTS
      #define NMSP_IMPORT_EXPORT_CONVENTION __declspec(dllexport)
      #define VST_IMPORT_EXPORT_CONVENTION __declspec(dllexport)
    #else
      #define NMSP_IMPORT_EXPORT_CONVENTION __declspec(dllimport)
      #define VST_IMPORT_EXPORT_CONVENTION __declspec(dllimport)
    #endif

	#ifdef NMSP_CORE_EXPORTS
		#define NMSP_CORE_EXPORT __declspec(dllexport)
	#else 
		#define NMSP_CORE_EXPORT __declspec(dllimport)
	#endif

    #ifdef NMSP_RESOURCE_COMMON_EXPORTS
        #define NMSP_RESOURCE_COMMON_EXPORT __declspec(dllexport)
    #else
        #define NMSP_RESOURCE_COMMON_EXPORT __declspec(dllimport)
    #endif

    #ifdef NMSP_RESOURCE_COMMON_REC_EXPORTS
        #define NMSP_RESOURCE_COMMON_RECOGNITION_EXPORT __declspec(dllexport)
    #else
        #define NMSP_RESOURCE_COMMON_RECOGNITION_EXPORT __declspec(dllimport)
    #endif

    #ifdef NMSP_RESOURCE_DICTATION_EXPORTS
        #define NMSP_RESOURCE_DICTATION_EXPORT __declspec(dllexport)
    #else
        #define NMSP_RESOURCE_DICTATION_EXPORT __declspec(dllimport)
    #endif

	#ifdef NMSP_RESOURCE_NMAS_EXPORTS
        #define NMSP_RESOURCE_NMAS_EXPORT __declspec(dllexport)
    #else
        #define NMSP_RESOURCE_NMAS_EXPORT __declspec(dllimport)
	#endif

	#ifdef NMSP_RESOURCE_RECOGNITION_EXPORTS
        #define NMSP_RESOURCE_RECOGNITION_EXPORT __declspec(dllexport)
    #else
        #define NMSP_RESOURCE_RECOGNITION_EXPORT __declspec(dllimport)
	#endif

	#ifdef NMSP_RESOURCE_TTS_EXPORTS
        #define NMSP_RESOURCE_TTS_EXPORT __declspec(dllexport)
    #else
        #define NMSP_RESOURCE_TTS_EXPORT __declspec(dllimport)
	#endif

	#ifdef NMSP_AUDIOPLAYBACK_EXPORTS
        #define NMSP_AUDIOPLAYBACK_EXPORT __declspec(dllexport)
    #else
        #define NMSP_AUDIOPLAYBACK_EXPORT __declspec(dllimport)
	#endif

	#ifdef NMSP_AUDIORECORD_EXPORTS
        #define NMSP_AUDIORECORD_EXPORT __declspec(dllexport)
    #else
        #define NMSP_AUDIORECORD_EXPORT __declspec(dllimport)
	#endif

	#ifdef NMSP_DICTATION_EXPORTS
        #define NMSP_DICTATIONRESULT_EXPORT __declspec(dllexport)
    #else
        #define NMSP_DICTATIONRESULT_EXPORT __declspec(dllimport)
	#endif

    #endif


//  #ifndef NMSP_CPL_ENABLED
//  #define NMSP_CPL_ENABLED
//  #endif


#endif

#endif /* NMSP_PLATFORM_INCLUDED */
