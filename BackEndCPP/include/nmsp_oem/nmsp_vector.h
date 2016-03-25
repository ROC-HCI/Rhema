#ifndef NMSP_VECTOR
#define NMSP_VECTOR
/*-----------------------------------------------------------------
   Note -- this header file is annotated with markup for Doxygen,
   a documentation tool for C and C++. If you are just reading
   the header file, you should ignore such markup as \c, \section
   \param, and so on.
  -----------------------------------------------------------------*/
/** \file nmsp_vector.h
 *   
 *			NMSP Vector Definitions	
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
 *  The NMSP Vector API provides a variable-length vector data container and the 
 *  associated methods that allow the storage and retrieval of specific NMSP
 *  data objects. The NMSP Vector object grows incrementally as new items are 
 *  added. Items can be retrieved from their corresponding index. Items can be
 *  inserted into the end of the vector or at any specified index. Similarly, 
 *  items can be removed from any specified index. The indexing updates required
 *  after insertions and removals are done automatically.
 */

#include <nmsp_oem/nmsp_platform.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * \enum NMSP_VECTOR_OBJECT_TYPE
 *
 * \brief Type of the objects contained by the nmsp_Vector object.
 *
 * This enumeration defines the types of objects that can be placed within
 * an NMSP vector
 **/
typedef enum {
    NMSP_OBJECT_TYPE_PARAMETER   = 0x01,	/*!< Used to create a vector of nmsp_Parameter objects */	
    NMSP_OBJECT_TYPE_GRAMMAR     = 0x02,	/*!< Used to create a vector of nmsp_Grammar objects */	
    NMSP_OBJECT_TYPE_UTTERANCE   = 0x03,	/*!< Used to create a vector of nmsp_Utterance objects */
    NMSP_OBJECT_TYPE_SENTENCE    = 0x04,	/*!< Used to create a vector of nmsp_dr_Sentence objects */
    NMSP_OBJECT_TYPE_TOKEN       = 0x05,	/*!< Used to create a vector of nmsp_dr_Token objects */
    NMSP_OBJECT_TYPE_ALTERNATIVE = 0x06,	/*!< Used to create a vector of nmsp_dr_Alternative objects */
    NMSP_OBJECT_TYPE_AUDIO_STATS = 0x07,	/*!< Used to create a vector of ExtraInformationEntry objects */
    NMSP_OBJECT_TYPE_KEY         = 0x08		/*!< Used to create a vector of keys (character pointer strings) */
} NMSP_VECTOR_OBJECT_TYPE;

/**
 * \typedef nmsp_Vector
 *
 * \brief An opaque data structure.
 *
 *        A vector to hold certain types of nmsp objects:
 *        nmsp_Parameter, nmsp_Grammar, or nmsp_Utterance.
 **/
typedef struct nmsp_Vector_ nmsp_Vector;

/**
 * \brief Creates an array object.
 *
 * Returns an opaque pointer to a newly allocated vector object.
 * If the allocation fails, NULL will be returned.
 * The nmsp_vector_destroy function must be called in order to 
 * free the memory allocated by this function
 *
 * \param type	The type of the vector to create, as enumerated in NMSP_VECTOR_OBJECT_TYPE.
 *				Must be a valid enumeration from NMSP_VECTOR_OBJECT_TYPE.
 *
 * \return      Pointer to the new array object.
 *				Returns NULL if an error occurred.
 *
 * \see NMSP_VECTOR_OBJECT_TYPE
 * \see nmsp_vector_destroy
 **/
NMSP_IMPORT_EXPORT_CONVENTION nmsp_Vector* nmsp_vector_create(NMSP_VECTOR_OBJECT_TYPE type);

/**
 * \brief Destroys a previously allocated array object.
 *
 *  Frees the memory of the array object.
 *  
 * \note    The items stored in the array by the user application are <em>not</em> deallocated
 *			by this function. The user application is responsible for iterating over the
 *			items and freeing them prior to destroying the array object.
 *
 * \param nmsp_vector       Pointer to the array object. Must not be NULL.
 *
 * \return      void
 **/
NMSP_IMPORT_EXPORT_CONVENTION void nmsp_vector_destroy(nmsp_Vector* nmsp_vector);

/**
 * \brief Gets the number of items in the array object.
 *
 *        Returns the number of data items currently stored in the
 *        array object.
 *
 * \param vector        Pointer to the array object.
 *                      Must not be NULL.
 *
 * \return      Number of items in the array.
 **/
NMSP_IMPORT_EXPORT_CONVENTION unsigned long nmsp_vector_getCount(const nmsp_Vector* vector);

/**
 * \brief Gets an item from the array object.
 *
 * Returns the item at the specified index from the array object.
 * Array indices are zero-based. Returns NULL if the array object
 * is empty or if the index is beyond the end of the array.
 *
 * \param vector        Pointer to the array object.
 *                      Must not be NULL.
 *
 * \param index         Index of the item.
 *                      Must be less than the total number of items in the vector.
 *
 * \return      Pointer to the item.
 *              Returns NULL if an error occurred.
 **/
NMSP_IMPORT_EXPORT_CONVENTION void* nmsp_vector_getItem(const nmsp_Vector*  vector,
                                                        const unsigned long index);

/**
 * \brief Sets an item into the array object.
 *
 * Stores the item at the specified index in the array object.
 * Array indices are zero-based. Returns NULL if the array object
 * is empty or if the index is beyond the end of the array.
 *
 * \param vector        Pointer to the array object.
 *                      Must not be NULL.
 *
 * \param index         Index of the item.
 *                      Must be less than the total number of items in the vector.
 *
 * \param item          Pointer to the new item.
 *                      Must not be NULL.
 *
 * \return      Pointer to the item.
 *				Returns NULL if an error occurred.
 **/
NMSP_IMPORT_EXPORT_CONVENTION void* nmsp_vector_setItem(nmsp_Vector*        vector,
                                                        const unsigned long index,
                                                        const void*         item);

/**
 * \brief Adds a new item to the end of the array object.
 *
 * Adds a new item to the end of the array object and updates the array
 * item-count. Returns a pointer to the new item, or returns NULL if the item
 * cannot be added because memory allocation failed.
 *
 * \param vector        Pointer to the array object.
 *                      Must not be NULL.
 *
 * \param item          Pointer to the new item.
 *                      Must not be NULL.
 *
 * \return      Pointer to the newly added item. Returns NULL if an error occurred.
 **/
NMSP_IMPORT_EXPORT_CONVENTION void* nmsp_vector_addItem(nmsp_Vector* vector,
                                                        const void*  item);

/**
 * \brief Inserts an item into the array object.
 *
 * Inserts a new item at the specified index in the array object and updates
 * the array item-count. If there are any items currently in the array
 * at or beyond the specified index, they are moved up by one position.
 * Returns a pointer to the new item or returns NULL if the item cannot be
 * added because memory allocation failed.
 *
 * \param vector        Pointer to the array object.
 *                      Must not be NULL.
 *
 * \param index         Index of the item.
 *                      Must be non-negative. Must be less than the total number of items in the vector.
 *
 * \param item          Pointer to the new item.
 *                      Must not be NULL.
 *
 * \return      Pointer to the newly inserted item. Returns NULL if an error occurred.
 **/
NMSP_IMPORT_EXPORT_CONVENTION void* nmsp_vector_insertItem(nmsp_Vector* vector,
                                                           const long   index,
                                                           const void*  item);

/**
 * \brief Removes an item from the array object.
 *
 * Removes the item at the specified index in the array object, updates
 * the array item-count, and returns the pointer to the item so that
 * the user can deallocate it, if necessary. If there are any items in
 * the array beyond the specified index, they are moved down by one
 * position. Returns NULL if the list object is empty.
 *
 * \param vector        Pointer to the array object.
 *                      Must not be NULL.
 *
 * \param index         Index of the item.
 *                      Must be less than the total number of items in the vector.
 *
 * \return      Pointer to the removed item. Returns NULL if an error occurred.
 **/
NMSP_IMPORT_EXPORT_CONVENTION void* nmsp_vector_removeItem(nmsp_Vector* vector,
                                                           const unsigned long   index);

/**
 * \brief Returns the type of the object contained by the nmsp_Vector.
 *
 * \param vector        Pointer to the array object.
 *                      Must not be NULL.
 *
 * \return      NMSP_VECTOR_OBJECT_TYPE
 **/
NMSP_IMPORT_EXPORT_CONVENTION NMSP_VECTOR_OBJECT_TYPE nmsp_vector_getType(const nmsp_Vector* vector);

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* NMSP_VECTOR */
