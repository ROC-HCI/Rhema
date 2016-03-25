#ifndef NMSP_PARAMETERS
#define NMSP_PARAMETERS
/*-----------------------------------------------------------------
   Note -- this header file is annotated with markup for Doxygen,
   a documentation tool for C and C++. If you are just reading
   the header file, you should ignore such markup as \c, \section
   \param, and so on.
  -----------------------------------------------------------------*/
/** \file nmsp_parameter.h
 *
 *			NMSP Parameter-Related Definitions
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
 *  \Overview
 *
 *  Contains session-related parameters that an NMSP device application can
 *  retrieve and set in the NMSP Gateway configuration file, the Nuance Speech
 *  Server (NSS) configuration file, and in the SDK to modify the operation of
 *  the NMSP Gateway or the application while it is executing. See the
 *  SDK <em>Programmer's Guide</em> for more information.
 *
 *  Note that modifications apply on a NMSP session basis only.
 *
 */

#ifdef __cplusplus
extern "C" {
#endif

/**
 * \enum NMSP_PARAM_TYPE
 *
 * \brief Defines the possible types of NMSP parameters.
 **/
typedef enum {
    /**
     * Application Server parameter.
     * Used to post recognition result to external application server.
     * Possible names are: "ASOR", "ASORUri." See the SDK <em>Programmer's Guide</em> for more details.
     * Suitable for nmsp_Resource setParam and nmsp_Resource getParam.
     *
     **/
    NMSP_PARAM_TYPE_APP    = 1,

    /**
     * NSS (Nuance Speech Server) parameter.
     * Used to set or get NSS configurations.
     * Refer to MRCPv2 specification (section
     * "Recognizer Header Fields") for parameter names.
     * Also see the SDK <em>Programmer's Guide</em>.
     * Suitable for nmsp_Resource.setParam and nmsp_Resource.getParam.
     *
     **/
    NMSP_PARAM_TYPE_NSS    = 2,

    /**
     * NMSP Gateway and NSS log parameter.
     * Adds logging in NMSP Gateway and NSS call logs.
     * See the SDK <em>Programmer's Guide</em>. Suitable
     * for nmsp_Resource.setParam only.
     *
     **/
    NMSP_PARAM_TYPE_SLOG   = 3,

    /**
     * NSS log parameter.
     * Adds logging in NSS call logs.
     * See the SDK <em>Programmer's Guide</em>.
     * Suitable for nmsp_Resource.setParam only.
     *
     **/
    NMSP_PARAM_TYPE_NSSLOG = 4,

    /**
     * NMSP Gateway log parameter.
     * Adds logging in NMSP Gateway.
     * See the SDK <em>Programmer's Guide</em>.
     * Suitable for nmsp_Resource.setParam only.
     *
     **/
    NMSP_PARAM_TYPE_GWLOG  = 5,

    /**
     * SDK parameter.
     * Used to configure NMSP SDK behaviors,
     * for example, timeouts.
     * See the SDK <em>Programmer's Guide</em>.
     * Used only to create nmsp_Manager or certain resources.
     *
    **/
    NMSP_PARAM_TYPE_SDK    = 6,

    /*
     * For future use; behavior undefined.
     *
     */
    NMSP_PARAM_TYPE_NMSP   = 7,     /*!< */
    NMSP_PARAM_TYPE_SVSP   = 8,     /*!< */
    NMSP_PARAM_TYPE_SIP    = 9,     /*!< */
    NMSP_PARAM_TYPE_SDP    = 10     /*!< */
} NMSP_PARAM_TYPE;


/**
 * \struct nmsp_Parameter
 *
 * \brief Represents an NMSP parameter that can be sent
 *        to or retrieved from the NMSP Gateway.
 *
 * A parameter is used for
 * configuration, logging, or other purposes. For details, see the
 * topic "Common functionality: Modifying an NMSP session through
 * parameters" in the SDK <em>Programmer's Guide</em>.
 * \note  IMPORTANT! If the payload is a string, the length must equal the
 *        number of characters and should not include the terminating NULL character.
 **/
typedef struct {
    NMSP_PARAM_TYPE type;
    char* name;
    char* payload;
    unsigned long length;
} nmsp_Parameter;

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* NMSP_PARAMETERS */
