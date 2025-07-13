/* Please see the LICENSE file for copyright and distribution information */

#ifndef __RXML_ERROR__
#define __RXML_ERROR__

#include <libxml/xmlerror.h>

extern VALUE eXMLError;

void rxml_init_error(void);
VALUE rxml_error_wrap(const xmlError *xerror);
void rxml_raise(const xmlError *xerror);

#endif
