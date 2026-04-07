/* Please see the LICENSE file for copyright and distribution information */

#include "ruby_libxml.h"
#include "ruby_xml_registry.h"

#include <ruby/st.h>

static st_table *rxml_registry;

void rxml_init_registry(void)
{
  rxml_registry = st_init_numtable();
}

void rxml_registry_register(void *ptr, VALUE obj)
{
  st_insert(rxml_registry, (st_data_t)ptr, (st_data_t)obj);
}

void rxml_registry_unregister(void *ptr)
{
  st_delete(rxml_registry, (st_data_t *)&ptr, NULL);
}

VALUE rxml_registry_lookup(void *ptr)
{
  st_data_t val;
  if (st_lookup(rxml_registry, (st_data_t)ptr, &val))
    return (VALUE)val;
  return Qnil;
}
