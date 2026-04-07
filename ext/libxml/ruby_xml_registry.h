/* Please see the LICENSE file for copyright and distribution information */

#ifndef __RXML_REGISTRY__
#define __RXML_REGISTRY__

/* Pointer-to-VALUE registry for mapping C structs (xmlDocPtr, xmlNodePtr,
   etc.) back to their Ruby wrappers.

   The registry is a global st_table keyed by C pointer address.  It is NOT a
   GC root -- entries do not prevent garbage collection.  Objects stay alive
   through the normal mark chains (mark functions look up the registry instead
   of reading _private).

   Registered pointers MUST be unregistered before the underlying C struct is
   freed, typically in the wrapper's dfree function. */

void  rxml_init_registry(void);
void  rxml_registry_register(void *ptr, VALUE obj);
void  rxml_registry_unregister(void *ptr);
VALUE rxml_registry_lookup(void *ptr);   /* Qnil on miss */

#endif
