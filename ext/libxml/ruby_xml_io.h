/* Please see the LICENSE file for copyright and distribution information */

#ifndef __RXML_IO__
#define __RXML_IO__

/* Context that libxml2 receives for reads from a Ruby IO object.

   libxml2 keeps the context pointer and calls rxml_read_callback much later.
   A raw VALUE is not a valid context, because GC compaction moves the IO
   object and the VALUE then holds a dead address.  So give libxml2 the
   address of this struct instead.

   The owner of the struct MUST call rxml_io_context_mark from its dmark
   function.  rb_gc_mark pins the IO object, so the VALUE in the struct stays
   correct.  The owner MUST also call rxml_io_context_free from its dfree
   function. */
typedef struct
{
  VALUE io;
} rxml_io_context;

rxml_io_context* rxml_io_context_new(VALUE io);
void rxml_io_context_init(rxml_io_context* context, VALUE io);
void rxml_io_context_free(rxml_io_context* context);
void rxml_io_context_mark(rxml_io_context* context);

int rxml_read_callback(void *context, char *buffer, int len);
int rxml_write_callback(VALUE io, const char *buffer, int len);
void rxml_init_io(void);

#endif
