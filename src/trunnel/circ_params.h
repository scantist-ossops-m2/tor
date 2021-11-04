/* circ_params.h -- generated by Trunnel v1.5.3.
 * https://gitweb.torproject.org/trunnel.git
 * You probably shouldn't edit this file.
 */
#ifndef TRUNNEL_CIRC_PARAMS_H
#define TRUNNEL_CIRC_PARAMS_H

#include <stdint.h>
#include "trunnel.h"

#if !defined(TRUNNEL_OPAQUE) && !defined(TRUNNEL_OPAQUE_CIRC_PARAMS_REQUEST)
struct circ_params_request_st {
  uint8_t version;
  uint8_t cc_supported;
  uint8_t trunnel_error_code_;
};
#endif
typedef struct circ_params_request_st circ_params_request_t;
#if !defined(TRUNNEL_OPAQUE) && !defined(TRUNNEL_OPAQUE_CIRC_PARAMS_RESPONSE)
struct circ_params_response_st {
  uint8_t version;
  uint8_t cc_enabled;
  uint8_t sendme_inc_cells;
  uint8_t trunnel_error_code_;
};
#endif
typedef struct circ_params_response_st circ_params_response_t;
/** Return a newly allocated circ_params_request with all elements set
 * to zero.
 */
circ_params_request_t *circ_params_request_new(void);
/** Release all storage held by the circ_params_request in 'victim'.
 * (Do nothing if 'victim' is NULL.)
 */
void circ_params_request_free(circ_params_request_t *victim);
/** Try to parse a circ_params_request from the buffer in 'input',
 * using up to 'len_in' bytes from the input buffer. On success,
 * return the number of bytes consumed and set *output to the newly
 * allocated circ_params_request_t. On failure, return -2 if the input
 * appears truncated, and -1 if the input is otherwise invalid.
 */
ssize_t circ_params_request_parse(circ_params_request_t **output, const uint8_t *input, const size_t len_in);
/** Return the number of bytes we expect to need to encode the
 * circ_params_request in 'obj'. On failure, return a negative value.
 * Note that this value may be an overestimate, and can even be an
 * underestimate for certain unencodeable objects.
 */
ssize_t circ_params_request_encoded_len(const circ_params_request_t *obj);
/** Try to encode the circ_params_request from 'input' into the buffer
 * at 'output', using up to 'avail' bytes of the output buffer. On
 * success, return the number of bytes used. On failure, return -2 if
 * the buffer was not long enough, and -1 if the input was invalid.
 */
ssize_t circ_params_request_encode(uint8_t *output, size_t avail, const circ_params_request_t *input);
/** Check whether the internal state of the circ_params_request in
 * 'obj' is consistent. Return NULL if it is, and a short message if
 * it is not.
 */
const char *circ_params_request_check(const circ_params_request_t *obj);
/** Clear any errors that were set on the object 'obj' by its setter
 * functions. Return true iff errors were cleared.
 */
int circ_params_request_clear_errors(circ_params_request_t *obj);
/** Return the value of the version field of the circ_params_request_t
 * in 'inp'
 */
uint8_t circ_params_request_get_version(const circ_params_request_t *inp);
/** Set the value of the version field of the circ_params_request_t in
 * 'inp' to 'val'. Return 0 on success; return -1 and set the error
 * code on 'inp' on failure.
 */
int circ_params_request_set_version(circ_params_request_t *inp, uint8_t val);
/** Return the value of the cc_supported field of the
 * circ_params_request_t in 'inp'
 */
uint8_t circ_params_request_get_cc_supported(const circ_params_request_t *inp);
/** Set the value of the cc_supported field of the
 * circ_params_request_t in 'inp' to 'val'. Return 0 on success;
 * return -1 and set the error code on 'inp' on failure.
 */
int circ_params_request_set_cc_supported(circ_params_request_t *inp, uint8_t val);
/** Return a newly allocated circ_params_response with all elements
 * set to zero.
 */
circ_params_response_t *circ_params_response_new(void);
/** Release all storage held by the circ_params_response in 'victim'.
 * (Do nothing if 'victim' is NULL.)
 */
void circ_params_response_free(circ_params_response_t *victim);
/** Try to parse a circ_params_response from the buffer in 'input',
 * using up to 'len_in' bytes from the input buffer. On success,
 * return the number of bytes consumed and set *output to the newly
 * allocated circ_params_response_t. On failure, return -2 if the
 * input appears truncated, and -1 if the input is otherwise invalid.
 */
ssize_t circ_params_response_parse(circ_params_response_t **output, const uint8_t *input, const size_t len_in);
/** Return the number of bytes we expect to need to encode the
 * circ_params_response in 'obj'. On failure, return a negative value.
 * Note that this value may be an overestimate, and can even be an
 * underestimate for certain unencodeable objects.
 */
ssize_t circ_params_response_encoded_len(const circ_params_response_t *obj);
/** Try to encode the circ_params_response from 'input' into the
 * buffer at 'output', using up to 'avail' bytes of the output buffer.
 * On success, return the number of bytes used. On failure, return -2
 * if the buffer was not long enough, and -1 if the input was invalid.
 */
ssize_t circ_params_response_encode(uint8_t *output, size_t avail, const circ_params_response_t *input);
/** Check whether the internal state of the circ_params_response in
 * 'obj' is consistent. Return NULL if it is, and a short message if
 * it is not.
 */
const char *circ_params_response_check(const circ_params_response_t *obj);
/** Clear any errors that were set on the object 'obj' by its setter
 * functions. Return true iff errors were cleared.
 */
int circ_params_response_clear_errors(circ_params_response_t *obj);
/** Return the value of the version field of the
 * circ_params_response_t in 'inp'
 */
uint8_t circ_params_response_get_version(const circ_params_response_t *inp);
/** Set the value of the version field of the circ_params_response_t
 * in 'inp' to 'val'. Return 0 on success; return -1 and set the error
 * code on 'inp' on failure.
 */
int circ_params_response_set_version(circ_params_response_t *inp, uint8_t val);
/** Return the value of the cc_enabled field of the
 * circ_params_response_t in 'inp'
 */
uint8_t circ_params_response_get_cc_enabled(const circ_params_response_t *inp);
/** Set the value of the cc_enabled field of the
 * circ_params_response_t in 'inp' to 'val'. Return 0 on success;
 * return -1 and set the error code on 'inp' on failure.
 */
int circ_params_response_set_cc_enabled(circ_params_response_t *inp, uint8_t val);
/** Return the value of the sendme_inc_cells field of the
 * circ_params_response_t in 'inp'
 */
uint8_t circ_params_response_get_sendme_inc_cells(const circ_params_response_t *inp);
/** Set the value of the sendme_inc_cells field of the
 * circ_params_response_t in 'inp' to 'val'. Return 0 on success;
 * return -1 and set the error code on 'inp' on failure.
 */
int circ_params_response_set_sendme_inc_cells(circ_params_response_t *inp, uint8_t val);


#endif
