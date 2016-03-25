#ifndef NMSP_GENERAL
#define NMSP_GENERAL

/*-----------------------------------------------------------------
   Note -- this header file is annotated with markup for Doxygen,
   a documentation tool for C and C++. If you are just reading
   the header file, you should ignore such markup as \c, \section
   \param, and so on.
  -----------------------------------------------------------------*/
/** \file nmsp_general.h
 *
 *				NMSP AudioSink Definitions
 *  
 * \section Legal_Notice Legal Notice
 *
 *  Copyright 2006-2011, Nuance Communications Inc. All rights reserved.
 *
 *  Nuance Communications, Inc. provides this document without representation
 *  or warranty of any kind. The information in this document is subject to
 *  change without notice and does not represent a commitment by Nuance
 *  Communications, Inc. The software and/or databases described in this
 *  document are furnished under a license agreement and may be used or
 *  copied only in accordance with the terms of such license agreement.
 *  Without limiting the rights under copyright reserved herein, and except
 *  as permitted by such license agreement, no part of this document may be
 *  reproduced or transmitted in any form or by any means, including, without
 *  limitation, electronic, mechanical, photocopying, recording, or otherwise,
 *  or transferred to information storage and retrieval systems, without the
 *  prior written permission of Nuance Communications, Inc.
 *
 *  Nuance, the Nuance logo, Nuance Recognizer, and Nuance Vocalizer are
 *  trademarks or registered trademarks of Nuance Communications, Inc. or its
 *  affiliates in the United States and/or other countries. All other
 *  trademarks referenced herein are the property of their respective owners.
 *  
 *  \section Overview
 *
 *  This file defines the AudioSink structure and related functionalities which are
 *  used to either capture audio buffers received from the text-to-speech (TTS) 
 *  commands or to send a voice stream over the network to the server for recogniton
 *  or other services requiring the straming of a recorded utterance.
 */
#ifdef __cplusplus
extern "C" {
#endif

#include <nmsp_oem/nmsp_defines.h>

/**
 *  \typedef nmsp_general_AudioSink
 *
 *  Defines the nmsp_general_AudioSink structure.
 *
 **/
typedef struct nmsp_general_AudioSink nmsp_general_AudioSink;


#ifdef NMSP_CPL_ENABLED
/**
 * \typedef nmsp_general_audiosink_addAudioBuf
 *
 * Defines a function pointer to an nmsp_general_audiosink_addAudioBuf function.
 * This function is expected to receive and handle audio buffers. Depending on
 * the context, the implementation of this function may be different. For example,
 * if the corresponding AudioSink is receiving TTS results, the implementation of
 * this function may collect and store the results to be played. If on the other hand
 * the corresponding AudioSink is used for recognition, the implementation of this function
 * would stream the buffers to the server.
 *
 * <b><em>Note: </em></b> For better performance, addAudioBuf() should be called multiple times 
 * with small buffers, not one time with a very big buffer. The addAudioBuf() function works 
 * when the audio buffer contains 200 to 500 milliseconds of audio. This amount of audio
 * best represents a different number of bytes depending on the codec you use. If you have to use
 * one big buffer, your application must ensure that the buffer size does not exceed the
 * server maximum buffer size. To change the default value of the server maximum buffer size,
 * edit the value assigned to the XmodeInitialBufferSize() parameter in the NMSPGW.cfg
 * configuration file on the NMSP Gateway host.
 *
 * \param audioSink			Pointer to the audioSink struct.
 *
 * \param buffer			Pointer to the audio buffer.
 *
 * \param len				Indicates the length of the buffer passed
 *
 * \param isLastBuf			Indicates that this buffer is the last buffer of the audio stream.
 *
 * \param startTime			Indicates when recording started
 *
 * \param stopTime			Indicates when recording stopped
 *
 * \param streamStartTime	Indicates when audio streaming started
 *
 * \param streamStopTime	Indicates when audio streaming stopped
 *
 * \return            0 if successful.
 *
 * \see nmsp_general_AudioSink
 **/
typedef int (*nmsp_general_audiosink_addAudioBuf)(nmsp_general_AudioSink* audioSink,
                                                  const char* buffer,
                                                  unsigned long len,
                                                  unsigned short isLastBuf,
                                                  unsigned long startTime,
                                                  unsigned long stopTime,
                                                  unsigned long streamStartTime,
                                                  unsigned long streamStopTime);
#else
/**
 * \typedef nmsp_general_audiosink_addAudioBuf
 *
 * Defines a function pointer to an nmsp_general_audiosink_addAudioBuf function.
 * This function is expected to receive and handle audio buffers. Depending on
 * the context, the implementation of this function may be different. For example,
 * if the corresponding AudioSink is receiving TTS results, the implementation of
 * this function may collect and store the results to be played. If on the other hand
 * the corresponding AudioSink is used for recognition, the implementation of this function
 * would stream the buffers to the server.
 *
 * <b><em>Note: </em></b> For better performance, addAudioBuf() should be called multiple times 
 * with small buffers, not one time with a very big buffer. The addAudioBuf() function works 
 * when the audio buffer contains 200 to 500 milliseconds of audio. This amount of audio
 * best represents a different number of bytes depending on the codec you use. If you have to use
 * one big buffer, your application must ensure that the buffer size does not exceed the
 * server maximum buffer size. To change the default value of the server maximum buffer size,
 * edit the value assigned to the XmodeInitialBufferSize() parameter in the NMSPGW.cfg
 * configuration file on the NMSP Gateway host.
 *
 * \param audioSink   Pointer to the audioSink struct.
 *
 * \param buffer      Pointer to the audio buffer.
 *
 * \param len		  Indicates the length of the buffer passed
 *
 * \param isLastBuf   Indicates that this buffer is the last buffer of the audio stream.
 *
 * \return            0 if successful.
 *
 * \see nmsp_general_AudioSink
 */

typedef int (*nmsp_general_audiosink_addAudioBuf)(nmsp_general_AudioSink* audioSink,
                                                  const char* buffer,
                                                  unsigned long len,
                                                  unsigned short isLastBuf);
#endif

/**
 * \struct nmsp_general_AudioSink
 *
 * \brief The nmsp_general_AudioSink structure.
 *
 * This structure constitutes the audio sink that can be used to handle
 * audio buffers to be sent, or audio buffers received.
 * The object that implements this structure must be properly initialized. 
 * It is crucial to define the nmsp_general_audiosink_addAudioBuf()
 * function and assign it to the function pointer of the AudioSink
 * before using the AudioSink.
 **/
struct nmsp_general_AudioSink {
    nmsp_general_audiosink_addAudioBuf addAudioBuf;
    short                              audioId;
    void*                              user_data;
};

/**
 \enum nmsp_general_ReturnCode
	
  Defines general purpose return codes.
*/
typedef enum {
    NMSP_GENERAL_SUCCESS,	/*!< Indicates a success */
    NMSP_GENERAL_FAILURE	/*!< Indicates a failure */
} nmsp_general_ReturnCode;

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* NMSP_GENERAL */
