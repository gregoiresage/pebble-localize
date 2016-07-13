#include <pebble.h>
  
static DictionaryIterator s_locale_dict;
static uint8_t *s_dict_buffer = 0;
static uint32_t s_previous_locale = 0;

void localize_init(uint32_t resource_locale) {

  if(s_previous_locale == resource_locale)
    return;

  s_previous_locale = resource_locale;

  ResHandle  locale_handle = resource_get_handle(resource_locale);
  int locale_size = resource_size(locale_handle);

  int resource_offset = 0;
  int locale_entries = 0;
  resource_offset += resource_load_byte_range(locale_handle, resource_offset, 
      (uint8_t*)&locale_entries, sizeof(locale_entries));

  struct locale {
    int32_t hashval;
    int32_t strlen;
  } locale_info;

  int dict_buffer_size = locale_size;
  dict_buffer_size -= sizeof(locale_entries); // remove size of the first bytes containing the number of entries
  dict_buffer_size -= sizeof(locale_info) * locale_entries; // remove the size of the local_info for each entry
  dict_buffer_size += 1 + 7 * locale_entries; //7 byte header per item
  
  if(s_dict_buffer)
    free(s_dict_buffer);
  s_dict_buffer = malloc(dict_buffer_size);
  dict_write_begin(&s_locale_dict, (uint8_t*)s_dict_buffer, dict_buffer_size);

  for (int i = 0; i < locale_entries; i++) {
    resource_offset += resource_load_byte_range(locale_handle,
                                                resource_offset,
                                                (uint8_t*)&locale_info,
                                                sizeof(struct locale));
    char *buffer = malloc(locale_info.strlen);
    resource_offset += resource_load_byte_range(locale_handle,
                                                resource_offset,
                                                (uint8_t*)buffer,
                                                locale_info.strlen);
    dict_write_cstring(&s_locale_dict, locale_info.hashval, buffer);
    free(buffer);
  }

  dict_write_end(&s_locale_dict);
}

void localize_deinit(){
  if(s_dict_buffer) {
    free(s_dict_buffer);
    s_dict_buffer = NULL;
  }
  s_previous_locale = 0;
}

const char *localize_str(int hashval) { 
  Tuple *tupl = dict_find(&s_locale_dict, hashval);

  if (tupl && tupl->value->cstring) {
    return tupl->value->cstring;
  }
  return "\7"; //return blank character
}
