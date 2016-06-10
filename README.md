# pebble-localize

Pebble library to support locale features in apps/faces and select the correct
text to display.

## How to use

### Setting Up Your Project

* Run `pebble package install pebble-localize`.

* Include the library in any C files that will use it:

```
#include <pebble-localize/pebble-localize.h>
```

* Add `locale_english.bin`, `locale_french.bin` and `locale_whatever.bin` 
to your app resources.

* Modify your main function to use `localize_init()` and `localize_deinit()`:

```c
int main(void) {
  // Init localize library
  localize_init(RESOURCE_ID_OF_YOUR_CHOOSEN_LANGUAGE);

  /* Other app setup code */

  // DeInit localize library
  localize_deinit();
}
```

* For all strings that you wish to localize, add `_()` around them.

```c
_("Breakfast Time");
```

### Generating Translation Resources

* Run `python gen_dict.py src/ locale_english.json`. This will generate
  `locale_english.json` and `locale_english.bin`.

* Move `locale_english.bin` to your project's `resources` directory.

* Make a copy of `locale_english.json` for other languages, such as
  `locale_german.json`.

* Modify `locale_german.json` to replace the English strings with German
  strings.

* Run `python dict2bin.py locale_german.json`. This will generate
  `locale_german.bin`.

* Move `locale_german.bin` to your project's `resources` directory.

* Add the new `.bin` resource files to your project's `appinfo.json` file as 
  shown in the 
  [App Resources guide](https://developer.getpebble.com/guides/pebble-apps/display-and-animations/resources/#raw-resources). 
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
  }, 
  {
    "type": "raw",
    "name": "LOCALE_GERMAN",
    "file": "locale_german.bin"
  }
]
```

* Compile your application and install!

### Adding More Languages

If you wish to add more translations in the future, repeat *Generating
Translation Resources* to obtain new translation binary resources. You will also
need to do this in the event that you modify any of your strings.
