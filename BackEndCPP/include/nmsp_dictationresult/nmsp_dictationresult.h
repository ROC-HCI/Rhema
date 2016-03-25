#ifndef NMSP_DICTATION_RESULT
#define NMSP_DICTATION_RESULT
/*-----------------------------------------------------------------
   Note -- this header file is annotated with markup for Doxygen,
   a documentation tool for C and C++. If you are just reading
   the header file, you should ignore such markup as \c, \section
   \param, and so on.
  -----------------------------------------------------------------*/
/** \file nmsp_dictationresult.h
 *			
 *					NMSP Dictation Result Utitlities
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
 *  The NMSP <b>DictationResult utility</b> is available with the NMAS Resource
 *  and Dictation Resource modules when the application uses dictation.
 *  After a dictation, the dictation recognition server returns bytes that
 *  represent the dictation results. The NMSP DictationResult utility
 *  presents the dictation results to the mobile device user
 *  and manages any changes to the dictation results that the user makes.
 *  <br/><br/>
 *  The DictationResultFactory parses the bytes and constructs a DictationResult
 *  object, which contains the result with the highest
 *  confidence score and alternative lower-score choices for each word or phrase.
 *  <br/><br/>
 *  Typically, an application displays the top sentences on
 *  the screen and lets the user select a sentence (nmsp_dr_Sentence).
 *  After a sentence is selected, the user can manipulate the result. The
 *  user can move the cursor, select words, and insert or delete
 *  characters. The application must be able to capture the following actions:
 *  <ul>
 *  <li>
 *  When the user moves the cursor, the application determines the cursor
 *  position.</li>
 *  <li>The application gets alternative choices for the corresponding word
 *  (or phrase if the device supports selection of multiple characters
 *  in a text field) by invoking the function <code>nmsp_dr_sentence_getAlternatives()</code>.
 *  The application can display the alternatives in a drop-down list
 *  and let the user select. The application then captures the user's selection
 *  and updates <code>nmsp_dr_DictationResult</code> by calling the
 *  function <code>nmsp_dr_sentence_chooseAlternative()</code>.
 *  </li>
 *  <li>
 *  If the user inserts or deletes a character, the application captures the
 *  cursor position and updates <code>nmsp_dr_DictationResult</code>
 *  by invoking the function <code>nmsp_dr_sentence_update()</code>.
 *  </li>
 *  </ul>
 *  After the functions that modify the contents of <code>nmsp_dr_DictationResult</code>
 *  are invoked, the function <code>nmsp_dr_sentence_getString()</code> should always be
 *  called to show a string representation of the currently selected sentence.
 *  <br /><br />
 *  The main purpose of updating <code>nmsp_dr_DictationResult</code> is to
 *  continue to generate suitable word alternatives even when the user modifies
 *  the dictation result.
 *  <br /><br /><br />
 *
 *  \section Example
 *  The diagram below shows an example <code>nmsp_dr_DictationResult</code> object
 *  and some standard operations that a user can perform on this object.
 *  <br /><br />
 *  <strong>Get top choice result</strong><br />
 *  You can get the top choice result phrase string by first calling the
 *  <code>nmsp_dr_dictation_result_getSentence()</code> function for index 0,
 *  which is the top choice, and then calling <code>nmsp_dr_sentence_getString()</code>.
 *  <br /><br />
 *  The application can display this phrase in a text field.
 *  In this example, the top choice dictation result is "This is a test." The
 *  digits below the text indicate the cursor positions. The cursor position is between
 *  characters, either at the beginning or at the end of the text. The cursor position
 *  must be greater than or equal to 0 and less than or equal to the length of
 *  the text string buffer.
 *  <br /><br />
 *  \image html DictationResult1.jpg
 *  <br />
 *  The example top choice contains four words and a period: This - is - a - test - .
 *  The cursor is in front of the first character T, and its index is 0.
 *
 *  Sample code:<pre>
 *  \code
 *  for(int i=0;i< nmsp_dr_dictation_result_getSize(dictation_result) && i<3;i++)
 *         presentSentenceChoiceOnScreen(nmsp_dr_dictation_result_getSentence(dictation_result, i))
 *  nmsp_dr_Sentence* selectedSentence = getSelectedSentenceFromUI();
 *
 *  char* str = nmsp_dr_sentence_getString( selectedSentence );
 *
 *  displayStrOnTextField(str);
 *  \endcode
 *  </pre>
 *  <strong>Get alternatives</strong><br />
 *  You can get the list of alternative word-choices by invoking the function
 *  <code>nmsp_dr_sentence_getAlternatives()</code>.
 *  Some mobile platforms let users select multiple characters in a text
 *  field (for example, by using a stylus), whereas other platforms support only
 *  moving the cursor in the text field.</li><br /><br />
 *  <ul>
 *  <li>
 *  On devices that do not support selecting multiple characters,
 *  there is only one cursor index. In this case, both the <code>cursor_pos_begin</code>
 *  and the <code>cursor_pos_end</code> should be equal to the cursor index.
 *  In the example below, the cursor index is 2, and you can call the function
 *  <code>nmsp_dr_alternatives_getAlternative()</code> to get the
 *  alternative word list for the word "This":
 *  <br /><br />
 *  \image html DictationResult2.jpg
 *  Sample code:<pre>
 *  \code
 *  nmsp_dr_Alternatives* alternatives = nmsp_dr_sentence_getAlternatives(selectedSentence, 2, 2);
 *   textField.setSelection(nmsp_dr_alternatives_getBeginIndexSelection(alternatives),
 *         nmsp_dr_alternatives_getEndIndexSelection(alternatives));
 *   for(int i=0;i<nmsp_dr_alternatives_getSize(alternatives) && i<3;i++)
 *   {
 *         dropDownMenu.add(nmsp_dr_alternatives_getAlternative(alternatives,i));
 *   }
 *  \endcode
 *   </pre></li>
 *  <li>
 *  On devices that support selecting multiple characters, the
 *  <code>cursor_pos_begin</code> is the character index that is inside a given word,
 *  and <code>cursor_pos_end</code> is the character index of another given word.
 *  If the user selects the two words "This is," the selection is
 *  from index [0,4] to index [5,7].
 *  <br /><br />
 *  \image html DictationResult2-1.jpg
 *  <br />
 *  The invocation of the method <code>nmsp_dr_sentence_get Alternatives</code> is shown in this sample code:<pre>
 *  \code
 *  nmsp_dr_Alternatives* alternatives = nmsp_dr_sentence_getAlternatives(selectedSentence, 0, 7);
 *   textField.setSelection(nmsp_dr_alternatives_getBeginIndexSelection(alternatives),
 *         nmsp_dr_alternatives_getEndIndexSelection(alternatives));
 *   for(int i=0;i<nmsp_dr_alternatives_getSize(alternatives);i++) {
 *         dropDownMenu.add(nmsp_dr_alternatives_getAlternative(alternatives,i));
 *   }
 *  \endcode
 *  </pre>
 *  Note that selecting part of a word is equal to selecting the whole word. For
 *  example <code>nmsp_dr_sentence_getAlternatives(selectedSentence, 1, 6)</code>
 *  has the same effect as <code>nmsp_dr_sentence_getAlternatives(selectedSentence, 0, 7)</code>.
 *  <br /><br />
 *  \image html DictationResult2-2.jpg
 *  <br />
 *  </li></ul>
 *  <br />
 *  <strong>Replace a word</strong><br />
 *  When the drop-down list of alternative words is available, you can replace a
 *  word in the result with an alternative word from the drop-down list by
 *  calling the function <code>nmsp_dr_sentence_chooseAlternative()</code>.
 *  As shown below, "There's" is the second element in the list. If the user selects
 *  this alternative, the sentence will be updated to use "There's" instead of "This."
 *  After setting the alternative, get the updated sentence from
 *  the dictation result by calling <code>nmsp_dr_sentence_getString()</code>.
 *  <br /><br />
 *  \image html DictationResult3.jpg
 *  Sample code:<pre>
 *  \code
 *  nmsp_dr_Alternative* alter = (nmsp_dr_Alternative*)dropDownMenu.getSelectedObject();
 *
 *  nmsp_dr_sentence_chooseAlternative(selectedSentence, alter);  // choose the selected element from the drop down list
 *  char* str = nmsp_dr_sentence_getString( selectedSentence );  // get the updated dictation result
 *  displayStrOnTextField(str);
 *  \endcode
 *  </pre>
 *  The new dictation result string is:
 *  \image html DictationResult3-1.jpg
 *  <strong>Remove a word</strong>
 *  Now remove "is" from the phrase by removing the three characters: i, s, and space.
 *  Normally, start from the end:  move the cursor to position 10, then call
 *  the <code>nmsp_dr_sentence_update()</code> function. For each change made to the sentence,
 *  the dictation result stays up to date regarding the modifications.
 *  </br></br>
 *  Note that the application <em>must</em> get the new result phrase each time an update is made
 *  because the changes and indexes for the result phrase are updated in
 *  <code>nmsp_dr_DictationResult</code> and <code>nmsp_dr_Sentence</code>.
 *  \image html DictationResult4.jpg<br />
 *  Sample code:<pre>
 *  \code
 *  public void onKeyReleased()
 *  {
 *              char* newText = myTextField.getText();
 *              int curPos = myTextField.getCursorPosition();
 *              nmsp_dr_sentence_update(selectedSentence, newText, curPos);
 *              myTextField.setText(nmsp_dr_sentence_getString( selectedSentence ));
 *  }
 *  \endcode
 *  </pre>
 *  Now the dictation result looks like this:
 *  \image html DictationResult4-1.jpg
 *  The updated dictation result becomes:
 *  \image html DictationResult4-2.jpg
 *  Each time a key is released, <code>nmsp_dr_Sentence</code> is updated. The final result is:
 *  \image html DictationResult4-3.jpg
 *
 *  The same process applies for insertion: each time a key is released,
 *  <code>nmsp_dr_Sentence</code> is updated. In this example, we want to
 *  insert the word "long" before the word "test."
 *  \image html DictationResult5.jpg>
 *  The dictation result now looks like this:
 *  \image html DictationResult5-1.jpg
 *  The dictation result next becomes:
 *  \image html DictationResult5-2.jpg
 *  The final result phrase is:
 *  \image html DictationResult5-3.jpg
 *
 */

#include <nmsp_oem/nmsp_platform.h>
#include <nmsp_resource_common/nmsp_resource_common.h>

#ifdef __cplusplus
extern "C" {
#endif


enum nmsp_dr_TokenType_ {
        VOICE_TOKEN,
        TEXT_TOKEN
    };

/** 
     \typedef nmsp_dr_TokenType
     
     This type represents the different kinds of tokens that can be present in a dictation result.
	 It enumerates the following tokens: 
     <ul>
     <li>VOICE_TOKEN is a token that is created according to the dictation result 
     returned by the recognizer. This type is based on voice input from the user.</li>
     <li>TEXT_TOKEN is a token that is created directly when a user enters text. 
     For example, a user might add one or more new words at the end of a sentence 
     that was created from a dictation result.</li>
     </ul>    
    */
typedef enum nmsp_dr_TokenType_ nmsp_dr_TokenType;


/**
    \typedef nmsp_dr_DictationResult

    Represents a dictation result.
*/
typedef struct nmsp_dr_DictationResult_ nmsp_dr_DictationResult;

/**
    \typedef nmsp_dr_Sentence

    Represents a sentence in a dictation result.
    One dictation result can contain one or more than one candidate sentences against one
    utterance.
*/
typedef struct nmsp_dr_Sentence_ nmsp_dr_Sentence;

/**
    \typedef nmsp_dr_Token

    Represents a voice token (word) that is contained by an
    nmsp_dr_Sentence object. The confidence score and the content of this
    token can be retrieved with these functions:
    <ul>
    <li>nmsp_dr_token_getConfidenceScore(nmsp_dr_Token* token)</li>
    <li>nmsp_dr_token_getString(nmsp_dr_Token* token)</li>
   </ul>
*/
typedef struct nmsp_dr_Token_ nmsp_dr_Token;

/**
    \typedef nmsp_dr_Alternatives

    Represents the alternate choices for a specified
    range of the top-rated sentence in the dictation result.

    One nmsp_dr_Alternatives object can contain one or more options that are represented by
    the nmsp_dr_Alternative structure.
*/
typedef struct nmsp_dr_Alternatives_ nmsp_dr_Alternatives;

/**
    \typedef nmsp_dr_Alternative

    Represents one alternative
    in an nmsp_dr_Alternatives structure.
*/
typedef struct nmsp_dr_Alternative_ nmsp_dr_Alternative;

/**
    \brief Creates a nmsp_dr_DictationResult
	
	This function returns a pointer to a newly allocated nmsp_dr_DictationResult structure. If the
	allocation fails, NULL is returned. The function nmsp_dr_dictation_result_delete must be used
	in order to delete this structure.

    \param data             Pointer to the raw data of dictation result.
							Must not be NULL.
	
    \param data_length      Dictation result data length. Specifies te length in bytes of the data string 
							parameter. Must be strictly greater than zero.

    \return                 Pointer to an nmsp_dr_DictationResult. If an error occurs, NULL is returned.                          
	
	\see nmsp_dr_dictation_result_delete
*/
NMSP_DICTATIONRESULT_EXPORT
nmsp_dr_DictationResult* nmsp_dr_dictation_result_create(const unsigned char* data,
                                                         const unsigned long  data_length);
/**
    \brief Deletes an nmsp_dr_DictationResult
	
	This function deletes a previously allocated nmsp_dr_DictationResult and sets the corresponding
	pointer to NULL.

    \param dictation_result Pointer to an nmsp_dr_DictationResult object.
							Must not be NULL.

    \return                 NMSP_OK if the dictation result is deleted correctly.
                            If an error occurs, then NMSP_ERROR is returned.
*/
NMSP_DICTATIONRESULT_EXPORT
NMSP_STATUS nmsp_dr_dictation_result_delete(nmsp_dr_DictationResult* dictation_result);

/**
    \brief Retrieves the number of sentences in an nmsp_dr_DictationResult object
	
	This function returns the number of sentences contained in a populated 
	nmsp_dr_DictaionResult object.

    \param dictation_result Pointer to an nmsp_dr_DictationResult object.
							Must not be NULL.

    \return                 Number of sentences.
                            If an error occurs, then zero is returned.
*/
NMSP_DICTATIONRESULT_EXPORT
unsigned long nmsp_dr_dictation_result_getSize(nmsp_dr_DictationResult* dictation_result);

/**
	\brief Retrieves sentence based on index

    Retrieves the nmsp_dr_Sentence object specified by the index from the nmsp_dr_DictationResult object.
	
    \param dictation_result Pointer to an nmsp_dr_DictationResult object.
							Must not be NULL.
	
    \param index            The index (zero based) of the sentence to be retrieved.
							If index exceeds the bound, NULL will be returned.

    \return                 An nmsp_dr_Sentence object.
                            If an error occurs, then NULL is returned.
*/
NMSP_DICTATIONRESULT_EXPORT
nmsp_dr_Sentence* nmsp_dr_dictation_result_getSentence(nmsp_dr_DictationResult* result,
                                                       unsigned short index);

/**
	\brief Get characeter encoding used
	
    This function returns the name of the character encoding being used by the specified
	nmsp_dr_DictationResult object.

    \param result			Pointer to an nmsp_dr_DictationResult object.
                            Must not be NULL

    \return					Returns a string containing the name of the encoding used.
                            If an error occurs, the default value ISO-8859-1 is returned
*/
NMSP_DICTATIONRESULT_EXPORT
char* nmsp_dr_dictation_result_getEncoding(nmsp_dr_DictationResult* result);


/**
 * \struct ExtraInformationEntry
 *
 * \brief Represents one entry of extra information attached to the dictation result data.
 */
typedef struct
{
    const char* const key;
    const char* const value;
} ExtraInformationEntry;

/**
    \brief Retrieve all extra information
    
    This function retrieves the extra information attached to the dictation result data.

    \param result           Pointer to an nmsp_dr_DictationResult object.
                            Must not be NULL.

    \return                 A vector containing all the ExtraInformationEntry structures belonging to
                            this nmsp_dr_DictationResult.

    \note                   The information may vary from one type of dictation result representation
                            to another. Some information may appear in one type and some may not. It
                            is data type dependent.
*/
NMSP_DICTATIONRESULT_EXPORT
nmsp_Vector* nmsp_dr_dictation_result_getExtraInformation(nmsp_dr_DictationResult* result);

/**
    \brief Retrieve extra information base on specified key
        
    This function retrieves the ExtraInformationEntry associated with the provided key.

    \param result           Pointer to an nmsp_dr_DictationResult object.
                            Must not be NULL.
                            
    \param key              The key that the caller is looking for.

    \return                 A pointer to the associated ExtraInformationEntry.
                            Returns NULL if key was not found.
*/
NMSP_DICTATIONRESULT_EXPORT
ExtraInformationEntry* nmsp_dr_dictation_result_findExtraInformation(nmsp_dr_DictationResult* result, const char *key);

/**
    \brief Update the dictation result with another binary buffer.
	
    \param result           Pointer to an nmsp_dr_DictationResult object.
                            Must not be NULL.

    \param data             Pointer to the raw data of dictation result.
							Must not be NULL.
	
    \param data_length      Dictation result data length. Specifies te length in bytes of the data string 
							parameter. Must be strictly greater than zero.

    \return                 If successful, NMSP_OK is returned.
                            If an error occurs, then NMSP_ERROR is returned.
*/
NMSP_DICTATIONRESULT_EXPORT
NMSP_STATUS nmsp_dr_dictation_result_update(nmsp_dr_DictationResult* result,
                                            const unsigned char* data,
                                            const unsigned long  data_length);


/**
    \brief Obtain confidence scores
    
    This function retrieves the confidence score for a specific nmsp_dr_Sentence object (for future use).

    \param sentence         An nmsp_dr_Sentence object.
                            Must not be NULL.

    \return                 The confidence score of the given sentence.
                            Zero is returned if the given sentence does not have any confidence score
                            or if any error occurs.

    \note                   This is not always supported, and the scope may vary according to
                            different server-side configurations. To get the range of the confidence
                            score, please check the dictation server's configuration.
*/
NMSP_IMPORT_EXPORT_CONVENTION
double nmsp_dr_sentence_getConfidenceScore(nmsp_dr_Sentence* sentence);

/**
	\brief Obtain the number of tokens in a sentence

    This function returns the number of tokens in a specified sentence.

    \param sentence         An nmsp_dr_Sentence object.
							Must not be NULL.

    \return                 Number of tokens within the given sentence.
                            If an error occurs, then zero is returned.
*/
NMSP_DICTATIONRESULT_EXPORT
unsigned long nmsp_dr_sentence_getSize(nmsp_dr_Sentence* sentence);

/**
    \brief Obtain sentence length
    
    This function returns the length of the specified sentence.

    \param sentence         An nmsp_dr_Sentence object.
                            Must not be NULL.

    \return                 Number of characters within the given sentence.
*/
NMSP_DICTATIONRESULT_EXPORT
unsigned long nmsp_dr_sentence_getStringLength(nmsp_dr_Sentence* sentence);

/**
	\brief Obtain token based on index
    
	This function retrieves the token that is at the specified index within a given sentence.

    \param sentence         An nmsp_dr_Sentence object.
							Must not be NULL. 
    
    \param index            Token index (zero based).

    \return                 Pointer to an nmsp_dr_Token.
                            If an error occurs, then NULL is returned.
*/
NMSP_DICTATIONRESULT_EXPORT
nmsp_dr_Token* nmsp_dr_sentence_getToken(nmsp_dr_Sentence* sentence,
                                         unsigned short index);


/**
	\brief Obtain token based on cursor position
    
	This function retrieves the token that is at the specified cursor position within a given sentence.

    \param sentence         An nmsp_dr_Sentence object.
							Must not be NULL. 
    
    \param cursor_pos       Cursor position within the sentence.

    \return                 Pointer to an nmsp_dr_Token.
                            If an error occurs, then NULL is returned.
*/
NMSP_DICTATIONRESULT_EXPORT
nmsp_dr_Token* nmsp_dr_sentence_getTokenAtCursorPosition(nmsp_dr_Sentence* sentence,
                                         unsigned short cursor_pos);

/**
    \brief Retrieves alternatives to a specified range in a sentence

	This function returns possible alternatives to the transcription used in
	part of he specified sentence. The part of the sentence to reinterpret is 
	specified by the begin and end cursor positions.

    \param sentence         An nmsp_dr_Sentence object.
							Must not be NULL.
    
	\param cursor_pos_begin The begin cursor position (zero based) of the selection.
    
	\param cursor_pos_end   The end cursor position (zero based) of the selection.

    \return                 Pointer to an nmsp_dr_Alternatives object.
                            If an error occurs, then NULL is returned.

    \note If this function is reused, any previously created
          nmsp_dr_Alternatives object will be destroyed.

    \see nmsp_dr_Alternatives
*/
NMSP_DICTATIONRESULT_EXPORT
nmsp_dr_Alternatives* nmsp_dr_sentence_getAlternatives(nmsp_dr_Sentence* sentence,
                                                       unsigned short cursor_pos_begin,
                                                       unsigned short cursor_pos_end);

/**
	\brief Replace current with alternative

    This function replaces word(s) in a sentence with word(s) from an alternative.
	Thi function can be used when the user wishes to replace a certain part of a 
	transcribed sentence with an alternative suggested through nmsp_dr_sentence_getAlternatives.

    \param sentence          An nmsp_dr_Sentence object.
							 Must not be NULL.
    
	\param alternative       Pointer to an nmsp_dr_Alternative object in the related
                             nmsp_dr_Alternatives object.
							 Must not be NULL

    \return                  If successful, NMSP_OK is returned.
                             If an error occurs, then NMSP_ERROR is returned.

	\see nmsp_dr_Alternative
	\see nmsp_dr_Alternatives
	\see nmsp_dr_sentence_getAlternatives
*/
NMSP_DICTATIONRESULT_EXPORT
NMSP_STATUS nmsp_dr_sentence_chooseAlternative(nmsp_dr_Sentence* sentence,
                                               nmsp_dr_Alternative* alternative);

/**
    \brief Updates dictation result sentence with changes

	This function must be called everytime a modification is made to a transcribed
	sentence.

    \param sentence          The nmsp_dr_Sentence object to be updated.
							 Must not be NULL.
    
	\param sentence_modified The modified sentence placed in a character pointer string.
							 Must not be NULL.
    
	\param cursor_pos        The position of the cursor after the modification.

    \return                  NMSP_OK if the original sentence is successfully updated.
                             If an error occurs, then NMSP_ERROR is returned.

    \note This function has to be called <em>each</em> time a single modification is made, for example,
          if a character is added or removed or if a word is added or removed. If there is more
          than one change in the modified sentence, it fails to update the original
          sentence. In this case, the modified sentence is considered to be
          unsynchronized with original sentence.
*/
NMSP_DICTATIONRESULT_EXPORT
NMSP_STATUS nmsp_dr_sentence_update(nmsp_dr_Sentence* sentence,
                                    char* sentence_modified,
                                    unsigned short cursor_pos);

/**
	\brief Appends two sentences

    Updates a dictation result sentence by adding a new sentence (from a subsequent dictation)
    to the current dictation result sentence at the cursor position. The cursor can be positioned
    at the beginning, the end, or inside the dictation result sentence. If the cursor is positioned
    inside a token, then the token is overwritten.

    \param sentence                     The nmsp_dr_Sentence object to be updated.
										Must not be NULL

    \param sentenceToInsert             The new selected sentence to add to (or insert into) the sentence
                                        object to be updated.
										Must not be NULL

    \param cursor_pos                   The cursor position that indicates where to insert the new sentence.

    \return                  NMSP_OK if the original sentence is successfully updated.
                             If an error occurs, then NMSP_ERROR is returned.
*/
NMSP_DICTATIONRESULT_EXPORT
NMSP_STATUS nmsp_dr_sentence_insert(nmsp_dr_Sentence* sentence,
                                    nmsp_dr_Sentence* sentenceToInsert,
                                    unsigned short cursor_pos);

/**
	\brief Replaces part of a sentence by another sentence

    Updates a dictation result sentence by inserting a new selected token (word) or sentence (from a subsequent
    dictation) into the sentence between two cursor positions: begin and end. The new selected token replaces
    (overwrites) the text between the begin and end cursor positions. If a cursor is positioned inside a token,
    then the entire token is overwritten.

    \param sentence                     The nmsp_dr_Sentence object to be updated.
										Must not be NULL.

    \param sentenceToInsert             The selected sentence to insert into the sentence object to be updated.
										Must not be NULL.

    \param cursor_pos_begin_selection   The cursor position that indicates where to start the insertion.
										Causes function to return error if out-of-bound, or if larger than
										cursor_pos_end_selection

    \param cursor_pos_end_selection     The cursor position that indicates where to end the insertion.
										Causes function to return error if out-of-bound.

    \return                  NMSP_OK if the original sentence is successfully updated.
                             If an error occurs, then NMSP_ERROR is returned.
*/
NMSP_DICTATIONRESULT_EXPORT
NMSP_STATUS nmsp_dr_sentence_insertOverRegion(nmsp_dr_Sentence* sentence,
                                              nmsp_dr_Sentence* sentenceToInsert,
                                              unsigned short cursor_pos_begin_selection,
                                              unsigned short cursor_pos_end_selection);


/**
	\brief Get string representation of sentence

    This function returns a character pointer string representation of a specfied
	nmsp_dr_Sentence object.

    \param sentence          An nmsp_dr_Sentence object.
							 Must not be NULL.

    \return                  A char* array that contains the sentence.
							 If error occurs, returns NULL.

    \note The char* string <em>must</em> be copied before reusing this method or modifying the sentence,
    otherwise its contents may be compromised.

*/
NMSP_DICTATIONRESULT_EXPORT
char* nmsp_dr_sentence_getString(nmsp_dr_Sentence* sentence);

/**
	\brief Retrieve confidence score of a token
	
	This function retrieves the confidence score of a specified nmsp_dr_Token object (for future use).

    \param token            An nmsp_dr_Token object.
							Must not be NULL.

    \return                 The confidence score of the given token.
                            If the given token does not have a confidence score
                            or if any error occurs, then zero is returned.

    \note                   This is not always supported, and the scope may vary according to
                            different server-side configurations. To get the range of the confidence
                            score, please check the dictation server configuration.
*/
NMSP_DICTATIONRESULT_EXPORT
double nmsp_dr_token_getConfidenceScore(nmsp_dr_Token* token);

/**
	\brief Get string representation of a token

    This function retrieves the string representation of the given nmsp_dr_Token object.

    \param token            An nmsp_dr_Token object.
							Must not be NULL.

    \return                 A NULL-terminated character string.
                            If an error occurs, then NULL is returned.

    \note A copy of the char* string must be made before reusing this method, otherwise
	its contents will be compromised.
*/
NMSP_DICTATIONRESULT_EXPORT
char* nmsp_dr_token_getString(nmsp_dr_Token* token);

/**
	\brief Get token type

    This function returns the type of the token that is present in the dictation result.
  
    \param token            An nmsp_dr_Token object.
							Must not be NULL.

    \return                 An nmsp_dr_TokenType enum.
*/
NMSP_DICTATIONRESULT_EXPORT
nmsp_dr_TokenType nmsp_dr_token_getTokenType(nmsp_dr_Token* token);

/**
	\brief check for \*no-space-before directive
    
	This function returns TRUE if the specified Token has a \*no-space-before directive.
  
    \param token            An nmsp_dr_Token object.
							Must not be NULL.

    \return                 TRUE if there is a \*no-space-before directive for this Token, otherwise returns FALSE
*/
NMSP_DICTATIONRESULT_EXPORT
int nmsp_dr_token_hasNoSpaceBeforeDirective(nmsp_dr_Token* token);

/**
	\brief check for \*no-space-after directive

    This function returns TRUE if the specified Token has a \*no-space-after directive.
  
    \param token            An nmsp_dr_Token object..
							Must not be NULL.

    \return                 TRUE if there is a \*no-space-after directive for this Token, otherwise returns FALSE
*/
NMSP_DICTATIONRESULT_EXPORT
int nmsp_dr_token_hasNoSpaceAfterDirective(nmsp_dr_Token* token);

/**
	\brief get the start time stamp of the given token

    This function returns start time stamp of the given token.
  
    \param token            An nmsp_dr_Token object..
							Must not be NULL.

    \return                 start time stamp in milliseconds
*/
NMSP_DICTATIONRESULT_EXPORT
long nmsp_dr_token_getStartTime(nmsp_dr_Token* token);

/**
	\brief get the end time stamp of the given token

    This function returns end time stamp of the given token.
  
    \param token            An nmsp_dr_Token object..
							Must not be NULL.

    \return                 end time stamp in milliseconds
*/
NMSP_DICTATIONRESULT_EXPORT
long nmsp_dr_token_getEndTime(nmsp_dr_Token* token);

/**
	\brief Get the begin index for an nmsp_dr_Alternatives

    For an nmsp_dr_Alternatives object, this function returns the begin index
	that was used to construct the alternatives object.

    \param alternatives      Pointer to an nmsp_dr_Alternatives object.
							 Must not be NULL.

    \return                  Begin index (zero based) of the selection area.

	\see nmsp_dr_sentence_getAlternatives
 */
NMSP_DICTATIONRESULT_EXPORT
int nmsp_dr_alternatives_getBeginIndexSelection(nmsp_dr_Alternatives* alternatives);

/**
	\brief Get the end index for an nmsp_dr_Alternatives

    For an nmsp_dr_Alternatives object, this function returns the end index
	that was used to construct the alternatives object.

    \param alternatives      Pointer to an nmsp_dr_Alternatives object.
							 Must not be NULL.

    \return                  End index (zero based) of the selection area.

	\see nmsp_dr_sentence_getAlternatives
 */
NMSP_DICTATIONRESULT_EXPORT
int nmsp_dr_alternatives_getEndIndexSelection(nmsp_dr_Alternatives* alternatives);

/**
	\brief Get number of alternatives

    This function retrieves the number of alternatives in an nmsp_dr_Alternatives object.
    It is advisable to call this function before calling nmsp_dr_alternatives_getAlternative(),
	and to make sure an in-bound index is used with nmsp_dr_alternatives_getAlternative().

    \param alternatives     A pointer to an nmsp_dr_Alternatives object.
							Must not be NULL.

    \return                 Number of alternatives.
                            If an error occurs, then zero is returned.

	\see nmsp_dr_sentence_getAlternatives
	\see nmsp_dr_alternatives_getAlternative
*/
NMSP_DICTATIONRESULT_EXPORT
unsigned long nmsp_dr_alternatives_getSize(nmsp_dr_Alternatives* alternatives);

/**
	\brief Get specific alternative

    This function returns a pointer to the nmsp_dr_Alternative object that is at the
	specified index within an nmsp_dr_Alternatives object.
    It is advisable to call this function after obtaining the total number of alternatives 
	from nmsp_dr_alternatives_getSize().

    \param alternatives     A pointer to an nmsp_dr_Alternatives object.
							Must not be NULL.

    \param index            An identifier (zero-based) of an nmsp_dr_Alternative object. Must not exceed
                            the number of alternatives returned by nmsp_dr_alternatives_getSize().
							
    \return                 An Alternative object.
                            If an error occurs, then NULL is returned.
*/
NMSP_DICTATIONRESULT_EXPORT
nmsp_dr_Alternative* nmsp_dr_alternatives_getAlternative(nmsp_dr_Alternatives* alternatives,
                                                        unsigned short index);

/**
	\brief Get string representation of an alternative

    This function returns a character pointer string representation of a given nmsp_dr_Alternative object.

    \param alternative      An nmsp_dr_Alternative object.
							Must not be NULL.

    \return                 A NULL terminated character string.
                            If an error occurs, then NULL is returned.

    \note The char* string <em>must</em> be copied before reusing this method, otherwise its contents
	will be compromised.

 */
NMSP_DICTATIONRESULT_EXPORT
char* nmsp_dr_alternative_getString(nmsp_dr_Alternative* alternative);


/**
	\brief Obtain the number of tokens in an alternative

    This function returns the number of tokens in a specified alternative.

    \param alternative      An nmsp_dr_Alternative object.
							Must not be NULL.

    \return                 Number of tokens within the given alternative.
                            If an error occurs, then zero is returned.

 */
NMSP_DICTATIONRESULT_EXPORT
unsigned long nmsp_dr_alternative_getSize(nmsp_dr_Alternative* alternative);

/**
    \brief Obtain token based on index
    
	This function retrieves the token that is at the specified index within a given alternative.

    \param alternative      An nmsp_dr_Alternative object.
							Must not be NULL. 
    
    \param index            Alternative index (zero based).

    \return                 Pointer to an nmsp_dr_Token.
                            If an error occurs, then NULL is returned.
 */
NMSP_DICTATIONRESULT_EXPORT
nmsp_dr_Token* nmsp_dr_alternative_getToken(nmsp_dr_Alternative* alternative,
                                            unsigned short index);

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* NMSP_DICTATION_RESULT */
