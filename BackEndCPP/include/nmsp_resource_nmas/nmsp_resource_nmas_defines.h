#ifndef NMSP_RESOURCE_NMAS_DEFINES
#define NMSP_RESOURCE_NMAS_DEFINES
/*-----------------------------------------------------------------
   Note -- this header file is annotated with markup for Doxygen,
   a documentation tool for C and C++. If you are just reading
   the header file, you should ignore such markup as \c, \section
   \param, and so on.
  -----------------------------------------------------------------*/

/** \file nmsp_resource_nmas_defines.h
 *
 *           Nuance Mobile Application Server Resource
 *
 *   \section Legal_Notice Legal Notice
 *
 *   Copyright 2006-2011, Nuance Communications Inc. All rights reserved.      
 *                                                                           
 *   Nuance Communications, Inc. provides this document without representation 
 *   or warranty of any kind. The information in this document is subject to   
 *   change without notice and does not represent a commitment by Nuance       
 *   Communications, Inc. The software and/or databases described in this      
 *   document are furnished under a license agreement and may be used or       
 *   copied only in accordance with the terms of such license agreement.       
 *   Without limiting the rights under copyright reserved herein, and except   
 *   as permitted by such license agreement, no part of this document may be   
 *   reproduced or transmitted in any form or by any means, including, without 
 *   limitation, electronic, mechanical, photocopying, recording, or otherwise,
 *   or transferred to information storage and retrieval systems, without the  
 *   prior written permission of Nuance Communications, Inc.                   
 *                                                                           
 *   Nuance, the Nuance logo, Nuance Recognizer, and Nuance Vocalizer are      
 *   trademarks or registered trademarks of Nuance Communications, Inc. or its 
 *   affiliates in the United States and/or other countries. All other         
 *   trademarks referenced herein are the property of their respective owners. 
 *  
 *   \section Overview
 *   This file provides global definitions that are to be used by the application
 *   and by the NMAS resource itself. for example, NMAS class types and command types.
 */
#ifdef __cplusplus
extern "C" {
#endif

/**
 * \enum NMSP_NMAS_CLASS_TYPE
 *
 * \brief Defines the possible NMAS class types, which are used to identify
 *        the NMSP_NMAS_DATA.
 *
 * \note If nmsp_nmas_get_dictionary_class() returns \c INVALID_CLASS_TYPE,
 *       this indicates that the key is missing in the key/value pair.
 *
 * \see nmsp_nmas_Dictionary
 * \see NMSP_NMAS_DATA
 **/
typedef enum {
    INVALID_CLASS_TYPE  = 0x00,     /*!< Used to indicate missing key/value */
    BYTE_STRING         = 0x04,     /*!< Indicates the type is a byte string */
    NMAS_NULL           = 0x05,     /*!< Indicates that the type is NULL entry */
    SEQUENCE            = 0x10,     /*!< Indicates the type is a sequence */
    ASCII_STRING        = 0x16,     /*!< Indicates the type is an ASCII string */
    INTEGER             = 0xC0,     /*!< Indicates the type is an integer */
    UTF8_STRING         = 0xC1,     /*!< Indicates the type is a UTF8 string */
    DICTIONARY          = 0xE0      /*!< Indicates the type is a dictionary */
} NMSP_NMAS_CLASS_TYPE;

/**
 * \enum NMSP_NMAS_COMMAND_TYPE
 *
 * \brief Defines the possible NMAS command types, which are used to
 *        identify the nmsp_nmas_Message objects.
 *
 * \see nmsp_nmas_Message
 **/
typedef enum {
//  GET_SCRIPT_UPDATE          = 0x0101,
    QUERY_BEGIN                = 0x0202,        /*!< Indicates a query begin message */
    QUERY_PARAMETER            = 0x0203,        /*!< Indicates a query parameter message */
    QUERY_END                  = 0x0204,        /*!< Indicates a query end message */
    ENROLLMENT_SEGMENT         = 0x0400,        /*!< Indicates an enrollment message */
    ENROLLMENT_AUDIO           = 0x0402,        /*!< Indicates an enrollment audio message */
//  EnrollementComplete        = 0x0401,
//  DiagnosticLogUpdate        = 0x0500,
    QUERY_RESULT               = 0x7201,        /*!< Indicates a query result message */
    QUERY_ERROR                = 0x7202,        /*!< Indicates a query error message */
//  QueryDisambiguate          = 0x7203,
//  QueryNarrow                = 0x7204,
    QUERY_RETRY                = 0x7205         /*!< Indicates a query retry message */
//  EnrollementText            = 0x7400,
//  SCRIPT_UPDATE_AVAILABLE    = 0x7300,
//  UPDATE_BLOCK               = 0x7301,
//  NO_SCRIPT_UPDATE_AVAILABLE = 0x7302
} NMSP_NMAS_COMMAND_TYPE;

/**
 * \typedef nmsp_nmas_Dictionary
 *
 * \brief An opaque data structure that stores data as key/value pairs.
 *   
 * This structure represents an NMAS dictionary. An entry in a  
 * dictionary consists of a key and a corresponding value.
 *
 * \see nmsp_nmas_create_dictionary
 * \see nmsp_nmas_dictionary_add
 **/
typedef struct nmsp_nmas_Dictionary_ nmsp_nmas_Dictionary;

/**
 * \typedef nmsp_nmas_Sequence
 *
 * \brief An opaque data structure that stores data as an indexed list.
 * 
 * This structure represents an NMAS sequence. A sequence is an indexed
 * list of data elements.
 *
 * \see nmsp_nmas_create_sequence
 * \see nmsp_nmas_sequence_add
 **/
typedef struct nmsp_nmas_Sequence_ nmsp_nmas_Sequence;

/**
 * \struct nmsp_nmas_Message
 *
 * \brief This structure represents an NMAS message.
 * 
 * This structure contains both the command type and an nmsp_nmas_Dictionary
 * object. This structure contains the result of a command creation and is  
 * delivered to te application via a callback.
 *
 * /see nmsp_nmas_command_eventCB
 **/
typedef struct {
    NMSP_NMAS_COMMAND_TYPE cmd;
    nmsp_nmas_Dictionary*  dictionary;
} nmsp_nmas_Message;

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* NMSP_RESOURCE_NMAS_DEFINES */
