# pebble-localize

Pebble library to support locale features in apps/faces and select the correct
text to display.

> This library is a fork of the excellent [locale_framework](https://github.com/pebble-hacks/locale_framework) under MIT license.
> A lot of code has not been changed or just a little.
> All the credits go to @C-D-Lewis @mhungerford ! Thanks for your work

## How to use

### Install the library

* Run `pebble package install pebble-localize`.

### Generating Translation Resources

* For all strings that you wish to localize, add `_()` around them.

```c
_("Breakfast Time");
```

* Run `python gen_dict.py src/ locale_english.json`. This will generate
  `locale_english.json` and `locale_english.bin`.

  `gen_dict.py` script can be found [here](https://raw.githubusercontent.com/gregoiresage/pebble-localize/master/gen_dict.py)

* Move `locale_english.bin` to your project's `resources` directory.

* Make a copy of `locale_english.json` for other languages, such as
  `locale_german.json`.

* Modify `locale_german.json` to replace the English strings with German
  strings.

* Run `python dict2bin.py locale_german.json`. This will generate
  `locale_german.bin`.

  `dict2bin.py` script can be found [here](https://raw.githubusercontent.com/gregoiresage/pebble-localize/master/dict2bin.py)

* Move `locale_german.bin` to your project's `resources` directory.

* Add the new `.bin` resource files to your project's `appinfo.json` file as 
  shown in the 
  [App Resources guide](https://developer.pebble.com/guides/app-resources/raw-data-files/). 
  For example, for the four language files in this project are added as shown
  below:

```js
"media": [
  {
    "type": "raw",
    "name": "LOCALE_ENGLISH",
    "file": "locale_english.bin"
  }, 
  {
    "type": "raw",
    "name": "LOCALE_FRENCH",
    "file": "locale_french.bin"
  }, 
  {
    "type": "raw",
    "name": "LOCALE_SPANISH",
    "file": "locale_spanish.bin"
  }
]
```

### Call localize api in your code

* Include the library in any C files that will use it:

```
#include <pebble-localize/pebble-localize.h>
```

* Modify your main function to use `localize_init()` and `localize_deinit()`:

```c
int main(void) {
  // Init localize library with your choosen locale
  localize_init(RESOURCE_ID_LOCALE_SPANISH);

  /* Other app code */

  // Change the locale is possible
  localize_init(RESOURCE_ID_LOCALE_FRENCH);

  /* Other app code */

  // DeInit localize library to clean up resources
  localize_deinit();
}
```

* Compile your application and install!

### Memory optimization

In the first version of the package, the whole binary file was loaded during `localize_init`. So it consumed memory even if some strings were not really used in your app.  
Now, strings are loaded on demand and then stored in cache for a fast access. By default the cache size is unlimited (of course the maximum size of your cache is the size of your binary file) but if you know only a certain number of strings are really used at the same time, you can configure the cache size and save memory.

```c
  localize_set_cache_size(5); // 5 is the number of strings in the cache
```

Example :  
Your watchface displays the name of the day. You will have 7 strings in you binary file but only one string is used at the same time.  
With the default configuration, after 2 days the cache will contain 2 strings, after 5 days 5 strings, after 7 days 7 strings, after 21 days ... 7 strings.  
If you call the `localize_set_cache_size(1)`, the cache will contain only 1 string and you will save memory (the 6 other days are not saved in memory)

### Adding More Languages

If you wish to add more translations in the future, repeat *Generating
Translation Resources* to obtain new translation binary resources. You will also
need to do this in the event that you modify any of your strings.
