#ifndef NMSP_AUDIO_PLAYBACK
#define NMSP_AUDIO_PLAYBACK
/*-----------------------------------------------------------------
 * Note -- this header file is annotated with markup for Doxygen,
 * a documentation tool for C and C++. If you are just reading
 * the header file, you should ignore such markup as \c, \section
 * \param, and so on.
 *---------------------------------------------------------------*/

/** \file nmsp_audioplayback.h
 *
 *             NMSP C SDK Audio Play Back Utilities
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
 *  Plays back files that are saved on disk or TTS audio,
 *  for example, for voice confirmation. Obtains system-dependent resources
 *  for playing audio (audio players), plays and stops playing audio, and
 *  delivers an event to a registered listener when a player event is delivered.
 **/

#ifdef __cplusplus
extern "C" {
#endif

/*
 *  Include headers for external data types required by this interface.
 */
#include <nmsp_oem/nmsp_platform.h>
#include <nmsp_oem/nmsp_general.h>
#include <nmsp_oem/nmsp_defines.h>
#include <nmsp_oem/nmsp_vector.h>
#include <nmsp_resource_common/nmsp_resource_common.h>

/**
 *      \typedef NMSP_AUDIO_PLAYBACK_EVENT
 *
 * This data structure represents the audio player event.
 *
 * \see nmsp_audio_playback_eventCallback
 **/
typedef enum {
    NMSP_AUDIO_PLAYBACK_STARTED,            /*!< Indicates that playback started */
    NMSP_AUDIO_PLAYBACK_STOPPED,            /*!< Indicates that playback stopped */
    NMSP_AUDIO_PLAYBACK_BUFFERING,          /*!< Indicates that playback is buffering */
    NMSP_AUDIO_PLAYBACK_BUFFER_RECEIVED,    /*!< Indicates that a buffer was received */
    NMSP_AUDIO_PLAYBACK_BUFFER_PLAYED,      /*!< Indicates that the buffer was played */
    NMSP_AUDIO_PLAYBACK_OUT_OF_MEMORY,      /*!< Indicates an out-of-memory error */
    NMSP_AUDIO_PLAYBACK_INVALID_STATE,      /*!< Indicates that player is in an invalid state */
    NMSP_AUDIO_PLAYBACK_ERROR,              /*!< Indicates that a playback error */
    NMSP_AUDIO_PLAYBACK_PLAYER_DESTROYED    /*!< Indicates that the player was destroyed */
} NMSP_AUDIO_PLAYBACK_EVENT;

/**
 *      \typedef nmsp_audio_Player
 *
 * An opaque data structure describing an audio player.
 *
 * \see nmsp_audio_player_create
 **/
typedef struct nmsp_audio_Player_ nmsp_audio_Player;

/**
 *      \typedef nmsp_audio_playback_eventCallback
 *
 * Called to deliver an event
 * when a player event is observed.
 *
 * \param event       Event generated as defined by NMSP_AUDIO_PLAYBACK_EVENT.
 * \param user_data   Pointer to the application data needed to re-create the context of a function call.
 *
 * \see NMSP_AUDIO_PLAYBACK_EVENT
 **/
typedef void (*nmsp_audio_playback_eventCallback)(const NMSP_AUDIO_PLAYBACK_EVENT event, void* user_data);

/**
 * \brief Creates an audio player.
 *
 * Returns a pointer to a newly allocated player or, if the creation failed, returns NULL.
 * To delete the nmsp_audio_Player, use the function nmsp_audio_player_delete().
 *
 * \param manager        Pointer to the nmsp_Manager object.
 *                       Must not be NULL.
 *
 * \param eventCallback  Event callback function, implementing nmsp_audio_playback_eventCallback.
 *                       Must not be NULL.
 *
 * \param parameters     Vector of audio parameters (for future use).
 *                       May be NULL.
 *
 * \param user_data      Pointer to the application data needed to recreate the context of a function call.
 *                       May be NULL.
 *
 * \return               Pointer to the newly created player. Returns NULL if error occurs.
 *
 * \see nmsp_audio_playback_eventCallback
 **/
NMSP_AUDIOPLAYBACK_EXPORT nmsp_audio_Player* nmsp_audio_player_create(nmsp_Manager* manager, const nmsp_audio_playback_eventCallback eventCallback, const nmsp_Vector* parameters, void* user_data);

/**
 * \brief Gets the nmsp_general_AudioSink object from nmsp_audio_Player.
 *
 * This function returns the nmsp_general_AudioSink object from nmsp_audio_Player.
 *
 * \param player         Pointer to an nmsp_audio_Player.
 *                       Must not be NULL.
 *
 * \return               Pointer to an nmsp_general_AudioSink object. Returns NULL if error occurs.
 **/
NMSP_AUDIOPLAYBACK_EXPORT nmsp_general_AudioSink* nmsp_audio_player_getAudioSink(const nmsp_audio_Player* player);

/**
 * Starts playback.
 *
 * This function starts the audio playback.
 * Note that calling start player on an already started player will cause the second call to send
 * an INVALID_STATE event in the callback, and will ignore the second operation
 *
 * \param player      Pointer to an nmsp_audio_Player.
 *                    Must not be NULL.
 *
 * \return            NMSP_OK if succeeded, NMSP_ERROR if player is NULL
 **/
NMSP_AUDIOPLAYBACK_EXPORT NMSP_STATUS nmsp_audio_player_start(nmsp_audio_Player* player);

/**
 * Stops playback.
 *
 * This function stops the audio playback.
 * Note that calling stop player on a player that has not been started, will cause this call to send
 * an INVALID_STATE event in the callback, and it will ignore the stop operation.
 *
 * \param player      Pointer to an nmsp_audio_Player.
 *                    Must not be NULL.
 *
 * \return            NMSP_OK if succeeded, NMSP_ERROR if player is NULL
 **/
NMSP_AUDIOPLAYBACK_EXPORT NMSP_STATUS nmsp_audio_player_stop(nmsp_audio_Player* player);


/**
 * Destroys an audio player.
 * This functions deletes a previously allocated nmsp_audio_Player, and releases the corresponding memory.
 * \warning The nmsp_audio_Player must not be deleted while its nmsp_general_AudioSink is still receiving audio.
 * e.g.: nmsp_nmas_tts_param_create() wait until the NMAS transaction is over.
 *       nmsp_tts_resource_generateAudio() wait until NMSP_TTS_RESOURCE_GENERATE_AUDIO_COMPLETED is received.
 *
 * \param player      Pointer to the player.
 *                    Must not be NULL.
 *
 * \return            NMSP_OK if succeeded, NMSP_ERROR if player is NULL
 **/
NMSP_AUDIOPLAYBACK_EXPORT NMSP_STATUS nmsp_audio_player_delete(nmsp_audio_Player* player);

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* NMSP_AUDIO_PLAYBACK */
