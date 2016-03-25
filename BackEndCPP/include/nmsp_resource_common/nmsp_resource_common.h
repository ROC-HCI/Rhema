#ifndef NMSP_RESOURCE_COMMON
#define NMSP_RESOURCE_COMMON
/*-----------------------------------------------------------------
   Note -- this header file is annotated with markup for Doxygen,
   a documentation tool for C and C++. If you are just reading
   the header file, you should ignore such markup as \c, \section
   \param, and so on.
  -----------------------------------------------------------------*/
/** \file nmsp_resource_common.h
 *  
 *               Common Definitions for NMSP Resources
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
 *  \section Overview
 *
 *  Defines functions that are common to all NMSP SDK resources: Recognition,
 *  Dictation, TTS (text-to-speech), and NMAS. These common functions are
 *  implemented by an SDK resource when that resource is created.
 *
 */

#include <nmsp_oem/nmsp_platform.h>

#ifdef __cplusplus
extern "C" {
#endif

#include <nmsp_oem/nmsp_defines.h>
#include <nmsp_oem/nmsp_vector.h>

/**
 * \enum NMSP_STATUS
 *
 * \brief Defines the possible status of NMSP functions.
 **/
typedef enum {
    NMSP_OK                   = 0x01,               /*!< Indicates success */
    NMSP_ERROR                = 0x02,               /*!< Indicates a generic error */
    //VST_NETWORK_ERROR = 0x03,
    NMSP_OUT_OF_MEMORY_ERROR  = 0x04,               /*!< Indicates an out-of-memory error */
    NMSP_RESOURCE_UNLOADED    = 0x05                /*!< Indicates that the NMSP resource was unloaded */
} NMSP_STATUS;

/**
 *      \typedef nmsp_Manager
 *
 * \brief An opaque data structure.
 *
 *        This structure defines and maintains the connection between the 
 *        NMSP Gateway and the client. It is required to create any NMSP SDK resources.
 * \see nmsp_manager_create
 **/
typedef struct nmsp_Manager_ nmsp_Manager;

/**
 *      \typedef nmsp_Resource
 *
 * \brief An opaque data structure.
 *
 *        This structure defines functionalities that are implemented by all SDK
 *        resources. It provides basic operations such as: getParam, setParam, 
 *        and freeResource. Every NMSP SDK resource will implicitly create its own 
 *        nmsp_Resource structure.
 *
 *        When a resource is created, it is possible to access the corresponding 
 *        nmsp_Resource reference. For example, for an NMAS Resource, after calling
 *        nmsp_nmas_resource_create(), the nmsp_Resource can be retrieved by calling
 *        nmsp_nmas_resource_get_nmsp_resource(). Obtaining a reference to the 
 *        nmsp_Resource allows calling the common operations: getParam, setParam,
 *        and freeResource.        
 *
 * \see nmsp_RecognitionResource
 * \see nmsp_DictationResource
 * \see nmsp_TTSResource
 * \see nmsp_NmasResource
 **/
typedef struct nmsp_Resource_ nmsp_Resource;

/**
 * \enum NMSP_MANAGER_EVENT
 *
 * \brief Defines the possible events between the NMSP manager and the NMSP Gateway.
 * 
 * These events are used to indicate to the nmsp_manager_eventCB callback 
 * that a particular manager event has occurred.
 *
 * \see nmsp_manager_eventCB
 **/
typedef enum {
    NMSP_MANAGER_CONNECTED         = 0x01,          /*!< Indicates that manager successfully connected to gateway */
    NMSP_MANAGER_DISCONNECTED      = 0x02,          /*!< Indicates that manager diconnected from gateway */
    NMSP_MANAGER_CONNECTION_FAILED = 0x03,          /*!< Indicates that manager failed to connect to gateway */
    NMSP_MANAGER_DESTROYED         = 0x04,          /*!< Indicates that manager was destroyed, and corresponding memory freed */
    NMSP_MANAGER_OUT_OF_MEMORY     = 0x05           /*!< Indicates that manager failed due to an out-of-memory error */
} NMSP_MANAGER_EVENT;

/**
 * \brief Callback function. Called to notify the application of manager events.
 *
 * An implementation of this callback function needs to be passed to the 
 * nmsp_manager_create function in order to create a manager. 
 *
 * \param event               Event occuring between the NMSP manager and the NMSP Gateway.
 *                            Possible events are defined in NMSP_MANAGER_EVENT.
 *
 * \param session_id          UUID of the session between client and the NMSP Gateway server.
 *                            Valid only if the event is NMSP_MANAGER_CONNECTED.
 *
 * \param user_data           Pointer to the application data needed to re-create the context 
 *                                     of a function call. Originally passed in nmsp_manager_create. 
 *                            Might be NULL, if application originally passed NULL.
 *
 * \see NMSP_MANAGER_EVENT
 * \see nmsp_manager_create
 **/
typedef void (*nmsp_manager_eventCB)(NMSP_MANAGER_EVENT event, const char* session_id, void* user_data);

/**
 * \brief Creates an nmsp_Manager structure.
 *
 * This function returns a pointer to a newly allocated nmsp_Managerstructure. 
 * If the allocation failed, NULL is returned.  The nmsp_Manager structure defines  
 * and maintains the connection between the NMSP Gateway and the client. It is 
 * required to create any NMSP SDK resources.
 * To delete the nmsp_Manager, the function nmsp_manager_destroy() must be used.
 *
 * \param server              NMSP Gateway IP address or hostname. This address 
 *                            indicates to the manager which NMSP Gateway it should
 *                            attempt to connect to. Specifying an address that 
 *                            does not have a NMSP Gateway running on it causes the 
 *                            connection to fail.
 *                            Must not be NULL.
 *
 * \param port                NMSP Gateway port number. This port number indicates 
 *                            to the manager which NMSP port it should attempt
 *                            to connect to. Specifying an incorrect port number
 *                            causes the connection to fail.
 *                            Must not be NULL.
 *
 * \param applicationId       Application ID provided by Nuance Communications, Inc
 *
 * \param applicationKey      Application key provided by Nuance Communications, Inc.
 *                            Must not be NULL
 *
 * \param applicationKeyLen   Length of the application key, in bytes.
 *                            Must match the length of application key.
 *
 * \param unique_id           Unique identifier. e.g.: phone number (UTF8).
 *                            Must not be NULL.
 *
 * \param input_codec         Input codec as defined in NMSP_DEFINES_CODEC.
 *
 * \param output_codec        Output codec as defined in NMSP_DEFINES_CODEC.
 *
 * \param parameters          Pointer to an array of nmsp_Parameter for this nmsp_Manager.
 *                            <br>Allowed parameter types:
 *                            <br> ::NMSP_PARAM_TYPE_APP
 *                            <br> ::NMSP_PARAM_TYPE_SDK
 *                            <br> ::NMSP_PARAM_TYPE_NMSP
 *                            <br>Sending any other parameter type causes an error.
 *                              
 *
 * \param manager_eventCB     The nmsp_manager_eventCB function. Called when a event
 *                            occurs, for example, if a connection is established or 
 *                            memory allocation fails.
 *                            Must not be NULL.
 *
 * \param user_data           Void pointer to application data. This pointer is returned in the
 *                            nmsp_manager_EventCB function, and allows identifying and/or 
 *                            recreating the original context of the callback.
 *
 * \return      Pointer to an nmsp_Manager. If an error occurs, NULL is returned.
 *
 * \see NMSP_DEFINES_CODEC
 * \see nmsp_manager_eventCB
 **/
NMSP_RESOURCE_COMMON_EXPORT nmsp_Manager* nmsp_manager_create(const char*                server,
                                                              const short                port,
                                                              const char*                applicationId,
                                                              const char*                applicationKey,
                                                              const int                  applicationKeyLen,
                                                              const char*                unique_id,
                                                              const NMSP_DEFINES_CODEC   input_codec,
                                                              const NMSP_DEFINES_CODEC   output_codec,
                                                              const nmsp_Vector*         parameters,
                                                              const nmsp_manager_eventCB manager_eventCB,
                                                              void*                      user_data);

/**
 * \brief Terminates the connection to the NMSP Gateway and deletes an nmsp_Manager structure.
 *
 * This function disconnects from the NMSP Gateway and deletes the memory allocated by the nmsp_Manager.
 * \note This function <em>must</em> be the <em>last</em> SDK function called. The nmsp_Resource used must 
 * be deleted using nmsp_resource_freeResource before deleting this function.
 * \warning The application must wait for the NMSP_MANAGER_DESTROYED event before creating a new nmsp_Manager.
 *
 * \param manager           An nmsp_Manager.
 *                          Must not be NULL.
 *
 * \return      NMSP_OK    if the manager is destroyed and shut down successfully.
 *              NMSP_ERROR if an error occurs.
 **/
NMSP_RESOURCE_COMMON_EXPORT NMSP_STATUS nmsp_manager_destroy(nmsp_Manager* manager);

/**
 * \enum NMSP_RESOURCE_EVENT
 *
 * \brief Defines the possible events delivered by nmsp_Resource.
 * 
 * These events are used to indicate to the nmsp_Resource callback 
 * that the resource event that has occurred.
 *
 * \see nmsp_resource_eventCB
 **/
typedef enum {
    NMSP_RESOURCE_SET_PARAM_COMPLETED = 0x01,          /*!< Indicates that a set parameter request completed successfully */
    NMSP_RESOURCE_SET_PARAM_FAILED    = 0x02,          /*!< Indicates that a set parameter request failed */
    NMSP_RESOURCE_GET_PARAM_COMPLETED = 0x03,          /*!< Indicates that a get parameter request completed successfully */
    NMSP_RESOURCE_GET_PARAM_FAILED    = 0x04,          /*!< Indicates that a get parameter request failed */
    NMSP_RESOURCE_RESOURCE_UNLOADED   = 0x05           /*!< Indicates that the resource was unloaded */
} NMSP_RESOURCE_EVENT;

/**
 * \enum RESOURCE_EVENT_CODE
 *
 * \brief Event code delivered by the NMSP Gateway.
 *
 * These events are used to provide the nmsp_Resource callback 
 * more insight about the resource event that has occurred.
 *
 * \see nmsp_resource_eventCB
 **/
typedef enum {
    CLIENT_DISCONNECTED_NORMALLY           = 0x0000,          /*!< Indicates that the NMSP SDK Client disconnected normally */
    CLIENT_SOCKET_SHUTDOWN_UNEXPECTEDLY    = 0x0001,          /*!< Indicates that the connection socket shutdown unexpectedly */
    CLIENT_PING_TIMEOUT                    = 0x0002,          /*!< Indicates that a ping request timed out */
    CLIENT_IDLE_TIMEOUT                    = 0x0003,          /*!< Indicates that a timeout occurred because Client was idle for to long */
    NSS_TERMINATED_SESSION_TIMEOUT         = 0x0004,          /*!< Indicates that the Nuance Speech Server terminated the session because of a timeout*/
    NSS_TERMINATED_SESSION_ERROR           = 0x0005,          /*!< Indicates that the Nuance Speech Server terminated the session because of an error*/
    GW_TERMINATED_SESSION_ERROR            = 0x0006,          /*!< Indicates that the NMSP Gateway terminated the session because of an error*/
    APP_SERVER_SESSION_TERMINATED_NORMALLY = 0x0007,          
    APP_SERVER_SESSION_TERMINATED_ERROR    = 0x0008,

    RESOURCE_EVENT_CODE_EMPTY              = 0xffff            /*!< Indicates that there is no resource event code to report */
} RESOURCE_EVENT_CODE;

/**
 * \enum RESOURCE_STATUS_CODE
 *
 * \brief Resource status code delivered by the NMSP Gateway.
 *
 * These events are used to provide the nmsp_Resource callback 
 * more insight about the resource event that has occurred.
 *
 * These status codes are used as defined in the Media Resource
 * Control Protocol version 2 (MRCPv2) Standard.
 *
 * \see nmsp_resource_eventCB
 **/
typedef enum {
    STATUS_CODE_SUCCESS                        = 200,               /*!< */
    STATUS_CODE_PARTIAL_SUCCESS                = 201,               /*!< */
    STATUS_CODE_METHOD_NOT_ALLOWED             = 401,               /*!< */
    STATUS_CODE_METHOD_NOT_VALID_IN_THIS_STATE = 402,               /*!< */
    STATUS_CODE_UNSUPPORTED_PARAMETER          = 403,               /*!< */
    STATUS_CODE_ILLEGAL_VALUE_FOR_PARAMETER    = 404,               /*!< */
    STATUS_CODE_NOT_FOUND                      = 405,               /*!< */
    STATUS_CODE_MANDATORY_PARAMETER_MISSING    = 406,               /*!< */
    STATUS_CODE_METHOD_OR_OPERATION_FAILED     = 407,               /*!< */
    STATUS_CODE_UNRECOGNIZED_MESSAGE_ENTITY    = 408,               /*!< */
    STATUS_CODE_UNSUPPORTED_PARAMETER_VALUE    = 409,               /*!< */
    STATUS_CODE_OUT_OF_ORDER_NUMBER_ID         = 410,               /*!< */
    STATUS_CODE_SERVER_INTERNAL_ERROR          = 501,               /*!< */
    STATUS_CODE_PROTOCOL_VERSION_NOT_SUPPORTED = 502,               /*!< */
    STATUS_CODE_PROXY_TIMEOUT                  = 503,               /*!< */
    STATUS_CODE_MESSAGE_TOO_LARGE              = 504,               /*!< */
    RESOURCE_STATUS_CODE_RESOURCE_UNLOADED     = 997,               /*!< */
    RESOURCE_STATUS_CODE_OUT_OF_MEMORY         = 998,               /*!< */
    RESOURCE_STATUS_CODE_EMPTY                 = 999               /*!< */
} RESOURCE_STATUS_CODE;

/**
 * \enum RESOURCE_COMPLETION_CAUSE
 *
 * \brief The completion cause delivered by the NMSP Gateway
 *        specifically for nmsp_Resource callbacks.
 *
 * These events are used to provide the nmsp_Resource callback 
 * more insight about a particular resource event that occurred.
 *
 * \see nmsp_resource_eventCB
 **/
typedef enum {
    RESOURCE_COMPLETION_CAUSE_RESOURCE_UNAVAILABLE = 0x0259,          /*!< Indicates that a resource was unavailable */
    RESOURCE_COMPLETION_CAUSE_3RD_PARTY_APP_ERROR  = 0x0260,          /*!< Indicates that a third pary error occurred */
    RESOURCE_COMPLETION_CAUSE_EMPTY                = 0x7FFF           /*!< Indicates that there is no resource event code to report */
} RESOURCE_COMPLETION_CAUSE;


/**
 * \brief Callback function.
 *
 * This callback delivers nmsp_Resource events to the application. 
 * This callback has to be implemented has to be passed by the application
 * when it creates a specific resource.
 *
 * \param event                 Type of the event. Used to specify the
 *                              kind of request and/or its outcome. The
 *                              events sent are defined in NMSP_RESOURCE_EVENT
 *
 * \param parameters            Array of the parameters delivered by the
 *                              resource. 
 *                              This array conatins valid data only if 
 *                              event is:
 *                              NMSP_RESOURCE_EVENT_SET_PARAM_COMPLETED
 *                              NMSP_RESOURCE_EVENT_GET_PARAM_COMPLETED
 *                              Otherwise, it is NULL.
 *
 * \param status_code           Status code of the submitted request,
 *                              as defined in RESOURCE_STATUS_CODE.
 *                              This code contains a valid value if 
 *                              event is:
 *                              NMSP_RESOURCE_EVENT_SET_PARAM_COMPLETED
 *                              NMSP_RESOURCE_EVENT_GET_PARAM_COMPLETED
 *                              NMSP_RESOURCE_EVENT_SET_PARAM_FAILED
 *                              NMSP_RESOURCE_EVENT_GET_PARAM_FAILED
 *                              Otherwise it is set to:
 *                              RESOURCE_STATUS_CODE_EMPTY
 *
 * \param completion_cause      Reason explaining the outcome of a request,
 *                              as defined in RESOURCE_COMPLETION_CAUSE.
 *                              This code contains a valid value if 
 *                              event is:
 *                              NMSP_RESOURCE_EVENT_SET_PARAM_FAILED
 *                              NMSP_RESOURCE_EVENT_GET_PARAM_FAILED
 *                              Otherwise it is set to:
 *                              RESOURCE_COMPLETION_CAUSE_EMPTY
 *
 * \param event_code            Event code of the submitted request,
 *                              as defined in RESOURCE_EVENT_CODE.
 *                              This code contains a valid value if
 *                              event is:
 *                              NMSP_RESOURCE_EVENT_RESOURCE_UNLOADED
 *                              Otherwise it is set to:
 *                              RESOURCE_EVENT_CODE_EMPTY                                        
 *
 * \param user_data            Pointer to the application data needed to re-create the context 
 *                             of a function call. Originally passed in the function that created
 *                             this resource. Might be NULL, if application originally passed NULL.
 *
 * \see NMSP_RESOURCE_EVENT
 * \see RESOURCE_EVENT_CODE
 * \see RESOURCE_COMPLETION_CAUSE
 * \see RESOURCE_STATUS_CODE
 **/
typedef void (*nmsp_resource_eventCB)(NMSP_RESOURCE_EVENT       event,
                                      nmsp_Vector*              parameters,
                                      RESOURCE_STATUS_CODE      status_code,
                                      RESOURCE_COMPLETION_CAUSE completion_cause,
                                      RESOURCE_EVENT_CODE       event_code,
                                      void*                     user_data);

/**
 * \brief Sets parameters for a given resource.
 *
 * This function sets a certain parameter of a given resource.
 * The corresponding results will be delivered by nmsp_resource_eventCB.
 *
 * \param resource          The nmsp_Resource object.
 *                          Must not be NULL.
 *
 * \param parameters        An nmsp_Vector contains a list of nmsp_Parameters to
 *                          be set on the NMSP Gateway.
 *                          Must not be NULL.
 *
 * \return      NMSP_OK if the set-param request is submitted successfully.
 *              NMSP_ERROR if an error occurs.
 *              NMSP_OUT_OF_MEMORY_ERROR if memory allocation fails.
 **/
NMSP_RESOURCE_COMMON_EXPORT NMSP_STATUS nmsp_resource_setParams(nmsp_Resource* resource,
                                                                nmsp_Vector*   parameters);

/**
 * \brief Gets parameters for a given resource.
 *
 * This function gets a certain parameter of a given resource.
 * The corresponding results will be delivered by nmsp_resource_eventCB.
 *
 * \param resource          The nmsp_Resource object.
 *                          Must not be NULL.
 *
 * \param parameters        An nmsp_Vector contains a list of nmsp_Parameters to
 *                          be retrieved from the NMSP Gateway.
 *                          Must not be NULL.
 *
 * \return      NMSP_OK if the get-param request is submitted successfully.
 *              NMSP_ERROR if an error occurs.
 *              NMSP_OUT_OF_MEMORY_ERROR if memory allocation fails.
 **/
NMSP_RESOURCE_COMMON_EXPORT NMSP_STATUS nmsp_resource_getParams(nmsp_Resource* resource,
                                                                nmsp_Vector*   parameters);

/**
 * \brief Frees the given resource from the NMSP Gateway.
 *
 * This function deletes a previously allocated resource and releases the
 * correponding memeory.
 * The results will be delivered by nmsp_resource_eventCB.
 *
 * \param resource               The nmsp_Resource object.
 *                               Must not be NULL.
 *
 * \param disconnect_timeout     Timeout, in milliseconds, before the resource is released.
 *
 * \return      NMSP_OK if the free-resource request is submitted successfully.
 *              NMSP_ERROR if an error occurs.
 *              NMSP_OUT_OF_MEMORY_ERROR if memory error occurs.
 **/
NMSP_RESOURCE_COMMON_EXPORT NMSP_STATUS nmsp_resource_freeResource(nmsp_Resource* resource,
                                                                   unsigned long  disconnect_timeout);

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* NMSP_RESOURCE_COMMON */
