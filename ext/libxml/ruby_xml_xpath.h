/* Please see the LICENSE file for copyright and distribution information */

#ifndef __RXML_XPATH__
#define __RXML_XPATH__

extern VALUE mXPath;

extern VALUE rxml_xpath_to_value(xmlXPathContextPtr, xmlXPathObjectPtr);
extern xmlXPathObjectPtr rxml_xpath_from_value(VALUE);

void rxml_init_xpath(void);

#endif
