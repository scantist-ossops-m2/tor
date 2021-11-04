/* circ_params.c -- generated by Trunnel v1.5.3.
 * https://gitweb.torproject.org/trunnel.git
 * You probably shouldn't edit this file.
 */
#include <stdlib.h>
#include "trunnel-impl.h"

#include "circ_params.h"

#define TRUNNEL_SET_ERROR_CODE(obj) \
  do {                              \
    (obj)->trunnel_error_code_ = 1; \
  } while (0)

#if defined(__COVERITY__) || defined(__clang_analyzer__)
/* If we're running a static analysis tool, we don't want it to complain
 * that some of our remaining-bytes checks are dead-code. */
int circparams_deadcode_dummy__ = 0;
#define OR_DEADCODE_DUMMY || circparams_deadcode_dummy__
#else
#define OR_DEADCODE_DUMMY
#endif

#define CHECK_REMAINING(nbytes, label)                           \
  do {                                                           \
    if (remaining < (nbytes) OR_DEADCODE_DUMMY) {                \
      goto label;                                                \
    }                                                            \
  } while (0)

circ_params_request_t *
circ_params_request_new(void)
{
  circ_params_request_t *val = trunnel_calloc(1, sizeof(circ_params_request_t));
  if (NULL == val)
    return NULL;
  return val;
}

/** Release all storage held inside 'obj', but do not free 'obj'.
 */
static void
circ_params_request_clear(circ_params_request_t *obj)
{
  (void) obj;
}

void
circ_params_request_free(circ_params_request_t *obj)
{
  if (obj == NULL)
    return;
  circ_params_request_clear(obj);
  trunnel_memwipe(obj, sizeof(circ_params_request_t));
  trunnel_free_(obj);
}

uint8_t
circ_params_request_get_version(const circ_params_request_t *inp)
{
  return inp->version;
}
int
circ_params_request_set_version(circ_params_request_t *inp, uint8_t val)
{
  if (! ((val == 0))) {
     TRUNNEL_SET_ERROR_CODE(inp);
     return -1;
  }
  inp->version = val;
  return 0;
}
uint8_t
circ_params_request_get_cc_supported(const circ_params_request_t *inp)
{
  return inp->cc_supported;
}
int
circ_params_request_set_cc_supported(circ_params_request_t *inp, uint8_t val)
{
  if (! ((val == 0 || val == 1))) {
     TRUNNEL_SET_ERROR_CODE(inp);
     return -1;
  }
  inp->cc_supported = val;
  return 0;
}
const char *
circ_params_request_check(const circ_params_request_t *obj)
{
  if (obj == NULL)
    return "Object was NULL";
  if (obj->trunnel_error_code_)
    return "A set function failed on this object";
  if (! (obj->version == 0))
    return "Integer out of bounds";
  if (! (obj->cc_supported == 0 || obj->cc_supported == 1))
    return "Integer out of bounds";
  return NULL;
}

ssize_t
circ_params_request_encoded_len(const circ_params_request_t *obj)
{
  ssize_t result = 0;

  if (NULL != circ_params_request_check(obj))
     return -1;


  /* Length of u8 version IN [0] */
  result += 1;

  /* Length of u8 cc_supported IN [0, 1] */
  result += 1;
  return result;
}
int
circ_params_request_clear_errors(circ_params_request_t *obj)
{
  int r = obj->trunnel_error_code_;
  obj->trunnel_error_code_ = 0;
  return r;
}
ssize_t
circ_params_request_encode(uint8_t *output, const size_t avail, const circ_params_request_t *obj)
{
  ssize_t result = 0;
  size_t written = 0;
  uint8_t *ptr = output;
  const char *msg;
#ifdef TRUNNEL_CHECK_ENCODED_LEN
  const ssize_t encoded_len = circ_params_request_encoded_len(obj);
#endif

  if (NULL != (msg = circ_params_request_check(obj)))
    goto check_failed;

#ifdef TRUNNEL_CHECK_ENCODED_LEN
  trunnel_assert(encoded_len >= 0);
#endif

  /* Encode u8 version IN [0] */
  trunnel_assert(written <= avail);
  if (avail - written < 1)
    goto truncated;
  trunnel_set_uint8(ptr, (obj->version));
  written += 1; ptr += 1;

  /* Encode u8 cc_supported IN [0, 1] */
  trunnel_assert(written <= avail);
  if (avail - written < 1)
    goto truncated;
  trunnel_set_uint8(ptr, (obj->cc_supported));
  written += 1; ptr += 1;


  trunnel_assert(ptr == output + written);
#ifdef TRUNNEL_CHECK_ENCODED_LEN
  {
    trunnel_assert(encoded_len >= 0);
    trunnel_assert((size_t)encoded_len == written);
  }

#endif

  return written;

 truncated:
  result = -2;
  goto fail;
 check_failed:
  (void)msg;
  result = -1;
  goto fail;
 fail:
  trunnel_assert(result < 0);
  return result;
}

/** As circ_params_request_parse(), but do not allocate the output
 * object.
 */
static ssize_t
circ_params_request_parse_into(circ_params_request_t *obj, const uint8_t *input, const size_t len_in)
{
  const uint8_t *ptr = input;
  size_t remaining = len_in;
  ssize_t result = 0;
  (void)result;

  /* Parse u8 version IN [0] */
  CHECK_REMAINING(1, truncated);
  obj->version = (trunnel_get_uint8(ptr));
  remaining -= 1; ptr += 1;
  if (! (obj->version == 0))
    goto fail;

  /* Parse u8 cc_supported IN [0, 1] */
  CHECK_REMAINING(1, truncated);
  obj->cc_supported = (trunnel_get_uint8(ptr));
  remaining -= 1; ptr += 1;
  if (! (obj->cc_supported == 0 || obj->cc_supported == 1))
    goto fail;
  trunnel_assert(ptr + remaining == input + len_in);
  return len_in - remaining;

 truncated:
  return -2;
 fail:
  result = -1;
  return result;
}

ssize_t
circ_params_request_parse(circ_params_request_t **output, const uint8_t *input, const size_t len_in)
{
  ssize_t result;
  *output = circ_params_request_new();
  if (NULL == *output)
    return -1;
  result = circ_params_request_parse_into(*output, input, len_in);
  if (result < 0) {
    circ_params_request_free(*output);
    *output = NULL;
  }
  return result;
}
circ_params_response_t *
circ_params_response_new(void)
{
  circ_params_response_t *val = trunnel_calloc(1, sizeof(circ_params_response_t));
  if (NULL == val)
    return NULL;
  return val;
}

/** Release all storage held inside 'obj', but do not free 'obj'.
 */
static void
circ_params_response_clear(circ_params_response_t *obj)
{
  (void) obj;
}

void
circ_params_response_free(circ_params_response_t *obj)
{
  if (obj == NULL)
    return;
  circ_params_response_clear(obj);
  trunnel_memwipe(obj, sizeof(circ_params_response_t));
  trunnel_free_(obj);
}

uint8_t
circ_params_response_get_version(const circ_params_response_t *inp)
{
  return inp->version;
}
int
circ_params_response_set_version(circ_params_response_t *inp, uint8_t val)
{
  if (! ((val == 0))) {
     TRUNNEL_SET_ERROR_CODE(inp);
     return -1;
  }
  inp->version = val;
  return 0;
}
uint8_t
circ_params_response_get_cc_enabled(const circ_params_response_t *inp)
{
  return inp->cc_enabled;
}
int
circ_params_response_set_cc_enabled(circ_params_response_t *inp, uint8_t val)
{
  if (! ((val == 0 || val == 1))) {
     TRUNNEL_SET_ERROR_CODE(inp);
     return -1;
  }
  inp->cc_enabled = val;
  return 0;
}
uint8_t
circ_params_response_get_sendme_inc_cells(const circ_params_response_t *inp)
{
  return inp->sendme_inc_cells;
}
int
circ_params_response_set_sendme_inc_cells(circ_params_response_t *inp, uint8_t val)
{
  inp->sendme_inc_cells = val;
  return 0;
}
const char *
circ_params_response_check(const circ_params_response_t *obj)
{
  if (obj == NULL)
    return "Object was NULL";
  if (obj->trunnel_error_code_)
    return "A set function failed on this object";
  if (! (obj->version == 0))
    return "Integer out of bounds";
  if (! (obj->cc_enabled == 0 || obj->cc_enabled == 1))
    return "Integer out of bounds";
  return NULL;
}

ssize_t
circ_params_response_encoded_len(const circ_params_response_t *obj)
{
  ssize_t result = 0;

  if (NULL != circ_params_response_check(obj))
     return -1;


  /* Length of u8 version IN [0] */
  result += 1;

  /* Length of u8 cc_enabled IN [0, 1] */
  result += 1;

  /* Length of u8 sendme_inc_cells */
  result += 1;
  return result;
}
int
circ_params_response_clear_errors(circ_params_response_t *obj)
{
  int r = obj->trunnel_error_code_;
  obj->trunnel_error_code_ = 0;
  return r;
}
ssize_t
circ_params_response_encode(uint8_t *output, const size_t avail, const circ_params_response_t *obj)
{
  ssize_t result = 0;
  size_t written = 0;
  uint8_t *ptr = output;
  const char *msg;
#ifdef TRUNNEL_CHECK_ENCODED_LEN
  const ssize_t encoded_len = circ_params_response_encoded_len(obj);
#endif

  if (NULL != (msg = circ_params_response_check(obj)))
    goto check_failed;

#ifdef TRUNNEL_CHECK_ENCODED_LEN
  trunnel_assert(encoded_len >= 0);
#endif

  /* Encode u8 version IN [0] */
  trunnel_assert(written <= avail);
  if (avail - written < 1)
    goto truncated;
  trunnel_set_uint8(ptr, (obj->version));
  written += 1; ptr += 1;

  /* Encode u8 cc_enabled IN [0, 1] */
  trunnel_assert(written <= avail);
  if (avail - written < 1)
    goto truncated;
  trunnel_set_uint8(ptr, (obj->cc_enabled));
  written += 1; ptr += 1;

  /* Encode u8 sendme_inc_cells */
  trunnel_assert(written <= avail);
  if (avail - written < 1)
    goto truncated;
  trunnel_set_uint8(ptr, (obj->sendme_inc_cells));
  written += 1; ptr += 1;


  trunnel_assert(ptr == output + written);
#ifdef TRUNNEL_CHECK_ENCODED_LEN
  {
    trunnel_assert(encoded_len >= 0);
    trunnel_assert((size_t)encoded_len == written);
  }

#endif

  return written;

 truncated:
  result = -2;
  goto fail;
 check_failed:
  (void)msg;
  result = -1;
  goto fail;
 fail:
  trunnel_assert(result < 0);
  return result;
}

/** As circ_params_response_parse(), but do not allocate the output
 * object.
 */
static ssize_t
circ_params_response_parse_into(circ_params_response_t *obj, const uint8_t *input, const size_t len_in)
{
  const uint8_t *ptr = input;
  size_t remaining = len_in;
  ssize_t result = 0;
  (void)result;

  /* Parse u8 version IN [0] */
  CHECK_REMAINING(1, truncated);
  obj->version = (trunnel_get_uint8(ptr));
  remaining -= 1; ptr += 1;
  if (! (obj->version == 0))
    goto fail;

  /* Parse u8 cc_enabled IN [0, 1] */
  CHECK_REMAINING(1, truncated);
  obj->cc_enabled = (trunnel_get_uint8(ptr));
  remaining -= 1; ptr += 1;
  if (! (obj->cc_enabled == 0 || obj->cc_enabled == 1))
    goto fail;

  /* Parse u8 sendme_inc_cells */
  CHECK_REMAINING(1, truncated);
  obj->sendme_inc_cells = (trunnel_get_uint8(ptr));
  remaining -= 1; ptr += 1;
  trunnel_assert(ptr + remaining == input + len_in);
  return len_in - remaining;

 truncated:
  return -2;
 fail:
  result = -1;
  return result;
}

ssize_t
circ_params_response_parse(circ_params_response_t **output, const uint8_t *input, const size_t len_in)
{
  ssize_t result;
  *output = circ_params_response_new();
  if (NULL == *output)
    return -1;
  result = circ_params_response_parse_into(*output, input, len_in);
  if (result < 0) {
    circ_params_response_free(*output);
    *output = NULL;
  }
  return result;
}
