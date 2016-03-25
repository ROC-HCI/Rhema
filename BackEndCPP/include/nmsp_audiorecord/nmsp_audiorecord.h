#ifndef NMSP_AUDIO_RECORD
#define NMSP_AUDIO_RECORD
/*------------------------------------------------------------------------
   Note -- This header file is annotated with markup for Doxygen,
   a documentation tool for C and C++. If you are just reading
   the header file, you should ignore such markup as \c, \section
   \param, and so on.
  ------------------------------------------------------------------------*/

/** \file nmsp_audiorecord.h
 *
 *                  NMSP C SDK Audio Record Utilities
 *
 *  \section Legal_Notice Legal Notice
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
 *
 *  \section Overview
 *
 *  Records audio (subscriber's speech), which is sent to the
 *  NMSP for speech recognition. Obtains system-dependent resources
 *  for recording audio (audio recorders), records and stops recording audio, and
 *  delivers an event to a registered listener when a recorder event is delivered.
 */


/*
 *  Include headers for external data types required by this interface.
 */

#include <nmsp_oem/nmsp_platform.h>

#ifdef __cplusplus
extern "C" {
#endif

#include <nmsp_oem/nmsp_general.h>
#include <nmsp_oem/nmsp_defines.h>
#include <nmsp_oem/nmsp_vector.h>
#include <nmsp_resource_common/nmsp_resource_common.h>

/**
 * \typedef NMSP_AUDIO_RECORD_EVENT
 *
 * Represents an audio recorder event. These are delivered
 * to the callback to signal it of a certain record event.
 *
 * \see nmsp_audio_record_eventCallback
 **/
typedef enum {
    NMSP_AUDIO_RECORD_BUFFER_RECORDED,          /*!< Indicates that the buffer was recorded */
    NMSP_AUDIO_RECORD_ERROR,                    /*!< Indicates that a recorder error was detected*/
    NMSP_AUDIO_RECORD_STARTED,                  /*!< Indicates that recording started */
    NMSP_AUDIO_RECORD_STOPPED,                  /*!< Indicates that recording stopped */
    NMSP_AUDIO_RECORD_STOPPED_TIMEOUT,          /*!< Indicates that recording stopped after timeout was reached */
    NMSP_AUDIO_RECORD_STOPPED_END_OF_SPEECH,    /*!< Indicates that recording stopped because end of speech was detected */
    NMSP_AUDIO_RECORD_START_OF_SPEECH,          /*!< Indicates that start of speech was detected */
    NMSP_AUDIO_RECORD_OUT_OF_MEMORY,            /*!< Indicates that the recorder is out of memory */
    NMSP_AUDIO_RECORD_INVALID_STATE,            /*!< Indicates that the recorder is in an invalid state */
    NMSP_AUDIO_RECORD_RECORDER_DESTROYED        /*!< Indicates that the recorder was destroyed */
} NMSP_AUDIO_RECORD_EVENT;

/**
 * \typedef nmsp_audio_Recorder
 *
 * An opaque data structure representing a recorder.
 *
 * \see nmsp_audio_recorder_create
 **/
typedef struct nmsp_audio_Recorder_ nmsp_audio_Recorder;

/**
 * \typedef nmsp_audio_record_eventCallback
 *
 * Called to deliver an event when a recorder event is observed.
 *
 * \param event       Event generated as defined by NMSP_AUDIO_RECORD_EVENT.
 *
 * \param eventData   Indicates energy level of the audio (when the endpointer is enabled)
 *                    sent back by BUFFER_RECORDED event.
 *                    Can be used, for example, to implement an equalizer.
 *                    Value of energy level equals *((long *)eventData).
 *                    Range of energy level equals 0–100, where 0 indicates silence.
 *                    Might be NULL. NULL when the event is not BUFFER_RECORDED.
 *
 * \param user_data   Pointer to the application data needed to re-create the context of a function call.
 *                    Might be NULL if originally assigned to NULL by user.
 *
 * \see NMSP_AUDIO_RECORD_EVENT
 **/
typedef void (*nmsp_audio_record_eventCallback)(const NMSP_AUDIO_RECORD_EVENT event, void* eventData, void* user_data);

/**
 * Starts recording. Note that this function is asynchronous.
 * To be sure that the recorder is started, it is necessary to wait for the 
 * NMSP_AUDIO_RECORD_STARTED event before the user starts speaking (for example,
 * the recording started screen should only display after receiving the 
 * NMSP_AUDIO_RECORD_STARTED event).
 *
 * Note that calling start recorder on an already started recorder will cause the second call to send
 * an INVALID_STATE event in the callback, and will ignore the second operation.
 *
 * \param recorder      Pointer to an nmsp_audio_Recorder.
 *                      Must not be NULL
 *
 * \return              NMSP_OK if succeeded, NMSP_ERROR if recorder is NULL
 **/
NMSP_AUDIORECORD_EXPORT NMSP_STATUS nmsp_audio_recorder_start(nmsp_audio_Recorder* recorder);

/**
 * Stops recording.
 * This function stops recording if it is started.
 *
 * Note that calling stop recorder on a recorder that has not been started, will cause this call to send
 * an INVALID_STATE event in the callback, and it will ignore the stop operation.
 *
 * \param recorder      Pointer to an nmsp_audio_Recorder.
 *                      Must not be NULL.
 *
 * \return              NMSP_OK if succeeded, NMSP_ERROR if recorder is NULL
 **/
NMSP_AUDIORECORD_EXPORT NMSP_STATUS nmsp_audio_recorder_stop(nmsp_audio_Recorder* recorder);

/**
 * Creates a recorder.
 *
 * Returns a pointer to a newly allocated recorder or, if the creation failed, returns NULL.
 * To delete the nmsp_audio_Recorder, the function nmsp_audio_recorder_delete() must be used.
 * \note If you are running on the Macintosh iPhone emulator, make sure that the Audio MIDI Setup
 *       is set to 16 kHz 1 channel 16 bits.
 *
 * \param audiosink       pointer to an nmsp_general_AudioSink object to sync the audio that is
 *                        recorded by the recorder object.
 *                        Must not be NULL.
 *
 * \param manager         The NMSP manager of the session this recorder will be used in.
 *                        Must not be NULL.
 *
 * \param eventCallback   Recorder's event callback function.
 *                        Must not be NULL.
 *
 * \param audioRecordTime Maximum audio record time, in milliseconds. When the maximum time is reached,
 *                        an NMSP_AUDIO_RECORD_STOPPED_TIMEOUT is received.
 *
 * \param parameters      A vector of audio parameters. Used to pass parameters to enable
 *                        and fine-tune endpointing to indicate to the application (via an event)
 *                        when the user starts and stops speaking. Possible parameters are:
 *                        ep.StopOnEndOfSpeech and ep.VadLongUtterance. Both are FALSE by default.
 *                        To enable the endpointer, you must pass the parameter ep.StopOnEndOfSpeech=TRUE.
 *                        If ep.VadLongUtterance=TRUE, it will allow longer pauses between words and sentences.
 *                        Once endpointing is enabled, two new NMSP_AUDIO_RECORD_EVENT events are sent with
 *                        the nmsp_audio_record_eventCallback: NMSP_AUDIO_RECORD_START_OF_SPEECH
 *                        (when start of speech is detected by the endpointer)
 *                        and NMSP_AUDIO_RECORD_STOPPED_END_OF_SPEECH (when the recorder stops because end of
 *                        speech is detected). See the Release Notes for platforms that support endpointing.
 *
 * \param user_data       Pointer to the application data that is needed to
 *                        re-create the context of a function call.
 *                        May be NULL.
 *
 * \return                Pointer to the newly created recorder, or returns NULL if error occurred.
 *
 * \see nmsp_audio_record_eventCallback
 **/
NMSP_AUDIORECORD_EXPORT nmsp_audio_Recorder* nmsp_audio_recorder_create(nmsp_general_AudioSink* audiosink,
                                                                        nmsp_Manager* manager,
                                                                        const nmsp_audio_record_eventCallback eventCallback,
                                                                        const long audioRecordTime,
                                                                        const nmsp_Vector* parameters,
                                                                        void* user_data);


/**
 * Reinitializes a fresh created recorder.
 * 
 * This function should be jointly used with nmsp_audio_recorder_create() method, on the platforms which suffer
 * a slow recorder initialization, we can firstly call nmsp_audio_recorder_create() to prime the system recorder
 * and then call this function to pass the realy audio sink object, audio paramters and other parameters.
 *
 * This function should only be called once the recorder has been created, calling this function in other
 * state has no effect.
 *
 * \param recorder         Pointer to the recorder that needs to be reinitialized.
 *                         Must not be NULL.
 * 
 * \param audioSink        Pointer to an nmsp_general_AudioSink object that will be used to 
 *                         handle the audio that is recorded by the recorder object.
 *                         Must not be NULL.
 *
 * \param eventCallback   Recorder's event callback function.
 *                        Must not be NULL.
 *
 * \param audioRecordTime Maximum audio record time, in milliseconds. When the maximum time is reached,
 *                        an NMSP_AUDIO_RECORD_STOPPED_TIMEOUT is received.
 *
 * \param parameters      A vector of audio parameters. Used to pass parameters to enable
 *                        and fine-tune endpointing to indicate to the application (via an event)
 *                        when the user starts and stops speaking. Possible parameters are:
 *                        ep.StopOnEndOfSpeech and ep.VadLongUtterance. Both are FALSE by default.
 *                        To enable the endpointer, you must pass the parameter ep.StopOnEndOfSpeech=TRUE.
 *                        If ep.VadLongUtterance=TRUE, it will allow longer pauses between words and sentences.
 *                        Once endpointing is enabled, two new NMSP_AUDIO_RECORD_EVENT events are sent with
 *                        the nmsp_audio_record_eventCallback: NMSP_AUDIO_RECORD_START_OF_SPEECH
 *                        (when start of speech is detected by the endpointer)
 *                        and NMSP_AUDIO_RECORD_STOPPED_END_OF_SPEECH (when the recorder stops because end of
 *                        speech is detected). See the Release Notes for platforms that support endpointing.
 *
 * \param user_data       Pointer to the application data that is needed to
 *                        re-create the context of a function call.
 *                        May be NULL.
 *
 * \return                NMSP_OK if the recorder has been reinitialized successfully
 *                        NMSP_ERROR if either the recorder is NULL or it is in a bad state for reinitialization
 **/
NMSP_AUDIORECORD_EXPORT NMSP_STATUS nmsp_audio_recorder_reinitialize(nmsp_audio_Recorder *recorder,
                                                                     nmsp_general_AudioSink* audioSink,
                                                                     const nmsp_audio_record_eventCallback eventCallback,
                                                                     const nmsp_Vector* parameters,
                                                                     void* user_data);

/**
 * Destroys a recorder and sets its pointer to NULL.
 *
 * This function deallocates a previously allocated recorder object. It also sets the pointer
 * to NULL to avoid having a dangling pointer.
 *
 * \param recorder         Pointer to the recorder.
 *                         Must not be NULL.
 *
 * \return                 NMSP_OK if succeeded, NMSP_ERROR if recorder is NULL
 **/
NMSP_AUDIORECORD_EXPORT NMSP_STATUS nmsp_audio_recorder_delete(nmsp_audio_Recorder* recorder);


/**
 * \brief Gets parameters for a given recorder.
 *
 * This function can be used to retrieve the parameters that the recorder is set to. The results
 * will be delivered by nmsp_resource_eventCB.
 *
 * \param recorder          Pointer to the recorder.
 *                          Must not be NULL.
 *
 * \param parameters        An nmsp_Vector contains a list of nmsp_Parameters
 *
 * \return                  NMSP_STATUS
 *
 * For example, to get the input source the audio system is currently
 * using to record audio.  This could be the internal
 * microphone, bluetooth headset or other types of
 * devices.
 * "HeadsetBT"
 * "HeadsetInOut"
 * "SpeakerAndMicrophone"
 *
 * If the phone's audio system does not provide
 * insight to the audio source, this function should return
 * "UnknownInputSource"
 *
 **/
NMSP_AUDIORECORD_EXPORT NMSP_STATUS nmsp_audio_recorder_getParams(nmsp_audio_Recorder* recorder, nmsp_Vector* parameters);

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* NMSP_AUDIO_RECORD */
