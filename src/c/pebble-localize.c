#include <pebble.h>
#include <@smallstoneapps/linked-list/linked-list.h>
  
static uint16_t s_limit = 0;
static uint32_t s_previous_locale = 0;

static LinkedRoot* s_root = NULL;

typedef struct hash_string {
  int32_t hashval;
  char*   value;
} hash_string;

static char* prv_load(int32_t hashval) {
  ResHandle  locale_handle = resource_get_handle(s_previous_locale);

  int resource_offset = 0;
  int locale_entries = 0;
  resource_offset += resource_load_byte_range(locale_handle, resource_offset, (uint8_t*)&locale_entries, sizeof(locale_entries));

  struct locale {
    int32_t hashval;
    int32_t strlen;
  } locale_info;

  hash_string *s = (hash_string *)malloc(sizeof(hash_string));
  s->hashval = hashval;
  s->value = NULL;

  linked_list_prepend(s_root, s);

  for (int i = 0; i < locale_entries; i++) {
    resource_offset += resource_load_byte_range(locale_handle, resource_offset, (uint8_t*)&locale_info, sizeof(struct locale));

    if(locale_info.hashval == hashval){
      char *buffer = malloc(locale_info.strlen);
      resource_load_byte_range(locale_handle, resource_offset, (uint8_t*)buffer, locale_info.strlen);
      s->value = buffer;
      break;
    }
    else {
      resource_offset += locale_info.strlen;
    }
  }

  return s->value;
}

static bool prv_destroy_element(void *object, void *context){
  hash_string* s = (hash_string*)object;
  if(s->value){
    free(s->value);
  }
  free(s);
  return true;
}

void localize_init(uint32_t resource_locale) {

  if(s_previous_locale == resource_locale)
    return;

  if(s_root == NULL){
    s_root = linked_list_create_root();
  }
  else {
    linked_list_foreach(s_root, prv_destroy_element, NULL);
    linked_list_clear(s_root);
  }

  s_previous_locale = resource_locale;
}

void localize_deinit(){
  if(s_root){
    linked_list_foreach(s_root, prv_destroy_element, NULL);
    linked_list_clear(s_root);
    free(s_root);
  }
  s_previous_locale = 0;
}

static bool compare_hash_string(void* object1, void* object2){
  return ((hash_string*)object1)->hashval == ((hash_string*)object2)->hashval;
}

const char *localize_str(int hashval) { 
  hash_string s;
  s.hashval = hashval;
  int16_t index = linked_list_find_compare(s_root, &s, compare_hash_string);
  if(index > -1){
    return ((hash_string*)linked_list_get(s_root, index))->value;
  }
  else {
    char *res = prv_load(hashval);
    if(res){
      if(s_limit > 0){
        uint16_t count = linked_list_count(s_root);
        if(count > s_limit){
          hash_string *last = linked_list_get(s_root, count-1);
          prv_destroy_element(last, NULL);
          linked_list_remove(s_root, count-1);
        }
      }
      return res;
    }
  }
  return "\7"; //return blank character
}

void localize_set_cache_size(uint16_t size){
  s_limit = size;
}