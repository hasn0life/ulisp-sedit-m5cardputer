/*
  LispBox uLisp Extension - Version 1.0 - June 2024
  Hartmut Grawe - github.com/ersatzmoco - June 2024

  edited by hasn0life for M5 Cardputer - Nov 2024

  Licensed under the MIT license: https://opensource.org/licenses/MIT
*/


/*
  (keyboard-get-key [pressed])
  Get key last recognized or null if no key is pressed.
*/
object *fn_KeyboardGetKey (object *args, object *env) {
  (void) env, (void) args;
  M5Cardputer.update();
  if (M5Cardputer.Keyboard.isChange()) {
    if (M5Cardputer.Keyboard.isPressed()) {
      Keyboard_Class::KeysState status = M5Cardputer.Keyboard.keysState();
      if (status.enter) return number('\n');
      if (status.del) return number(127);
      if (status.tab) return number('\t');
      if (status.ctrl){
        //ctrl capitalizes everything so we use those letters
        for (auto i : status.word){
          //exit
          if((i == 'Q') || (i == 'C')){  
            return number(17);
          }
          //flush buffer
          else if((i == 'X') || (i == 'B')|| (i == 'N')){
            return number(24);
          } 
          // delete line
          else if((i == 'K') || (i == 'I')){
            return number(12);
          }
          //move to start of line
          else if((i == 'A') || (i == '<')) {
            return number(210);
          }
          // move to end of line
          else if((i == 'E') || (i == '?')) {
            return number(213);
          } 
          //move to beginning
          else if(i == '^')  {
            return number(94);
          }   
          // pg up
          else if(i == ':')  {
            return number(211);
          }  
          //pg down  
          else if(i == '>')  {
            return number(214);
          } 
        }
      }
      if (status.fn){
        for (auto i : status.word){
          if(i == ',') {
            return number(216);
          }
          else if(i == '/') {
            return number(215);
          }
          else if(i == ';') {
            return number(218);
          }
          else if(i == '.') {
            return number(217);
          }
          //bind to symbol
          else if(i == '5') {
            return number(198);
          }
          //delete SD card file
          else if(i == '9') {
            return number(202);
          }
          //save text
          else if(i == '0') {
            return number(203);
          }
          //load text
          else if(i == '1') {
            return number(204);
          }
          //show directory
          else if(i == '2') {
            return number(205);
          }
          //help menu
          else if(i == 'h') {
            return number(16);
          }
          //toggle bracket matching
          else if(i == '[') {
            return number(194);
          }
          //chekck if matching bracket
          else if(i == ']') {
            return number(195);
          }
        }
      }

      for (auto i : status.word) return number(i);
    }
  }
  return nil;
}

/*
  (keyboard-flush)
  Discard missing key up/down events, doesnt do anything here but I'm leaving it in for future stuff
*/
object *fn_KeyboardFlush (object *args, object *env) {
  (void) args, (void) env;
  // kb_pressed_k = 0;
  // kb_pressed_m = 0;
  // kb_released_k = 0;
  // kb_released_m = 0;

  return nil;
}

object *fn_searchstr (object *args, object *env) {
  (void) env;
  
  int startpos = 0;
  object *pattern = first(args);
  object *target = second(args);
  args = cddr(args);
  if (pattern == NULL) return number(0);
  else if (target == NULL) return nil;
  if (args != NULL) startpos = checkinteger(car(args));
  
if (stringp(pattern) && stringp(target)) {
    int l = stringlength(target);
    int m = stringlength(pattern);
    if (startpos > l) error2(indexrange);
    for (int i = startpos; i <= l-m; i++) {
      int j = 0;
      while (j < m && nthchar(target, i+j) == nthchar(pattern, j)) j++;
      if (j == m) return number(i);
    }
    return nil;
  } else error2("arguments are not both lists or strings");
  return nil;
}

#if defined sdcardsupport
/*
  (sd-file-exists filename)
  Returns t if filename exists on SD card, otherwise nil.
*/
object *fn_SDFileExists (object *args, object *env) {
  (void) args, (void) env;

  SD.begin(SD_SPI_CS_PIN);

  int slength = stringlength(checkstring(first(args)))+1;
  char *fnbuf = (char*)malloc(slength);
  cstring(first(args), fnbuf, slength);

  if (SD.exists(fnbuf)) {
    //while (*fnbuf) ProcessKey(*fnbuf++);
    return tee;
  }
  else {
    //while (*fnbuf) ProcessKey(*fnbuf++);
    return nil;
  }
}

/*
  (sd-file-remove filename)
  Returns t if filename exists on SD card, otherwise nil.
*/
object *fn_SDFileRemove (object *args, object *env) {
  (void) args, (void) env;

  SD.begin(SD_SPI_CS_PIN);
  int slength = stringlength(checkstring(first(args)))+1;
  char *fnbuf = (char*)malloc(slength);
  cstring(first(args), fnbuf, slength);

  if (SD.exists(fnbuf)) {
    SD.remove(fnbuf);
    return tee;
  }
  else {
    return nil;
  }
}

/*
  (sd-card-dir [mode])
  Print SD card directory and return nothing [mode 0, optional] or a uLisp string object [mode 1] or a List [mode 2].
*/
object *fn_SDCardDir (object *args, object *env) {
  (void) env;
  int mode = 0;
  if (args != NULL) {
    mode = checkinteger(first(args));
    if (mode > 2) mode = 2;
    if (mode < 0) mode = 0;
  }

  SD.begin(SD_SPI_CS_PIN);
  File root = SD.open("/");
  int numTabs = 0;
  String dirstr;
  object* dirlist = NULL;

  if (mode == 0) {
    printDirectory(root, numTabs);
    return nil;
  }
  else if (mode == 1) {
    dirstr = printDirectoryStr(root, numTabs, dirstr);
    return lispstring((char*)dirstr.c_str());
  }
  else if (mode == 2) {
    dirlist = printDirectoryList(root, dirlist);
    return dirlist;
  }
  else return nil;
}

//Directory functions (modified Arduino PD code)
String printDirectory(File dir, int numTabs) {
  pfun_t pf = pserial;

  while (true) {
    File entry =  dir.openNextFile();
    if (! entry) {
      // no more files
      break;
    }
    for (uint8_t i = 0; i < numTabs; i++) {
      pfstring("  ", pf);
    }
    pfstring(entry.name(), pf);
    if (entry.isDirectory()) {
      pfstring("/\n", pf);
      printDirectory(entry, numTabs + 1);
    } else {
      // files have sizes, directories do not
      pfstring(" ", pf);
      pint(entry.size(), pf);
      pfstring("\n", pf);
    }
    entry.close();
  }
  // return NULL;
  return String("");
}

String printDirectoryStr(File dir, int numTabs, String dirstr) {
  while (true) {
    File entry =  dir.openNextFile();
    if (! entry) {
      // no more files
      break;
    }
    dirstr = dirstr + entry.name();
    if (entry.isDirectory()) {
      dirstr = dirstr + "/~%";
      dirstr = printDirectoryStr(entry, numTabs + 1, dirstr);
    } else {
      // files have sizes, directories do not
      dirstr = dirstr + " " + String(entry.size()) + "~%";
    }
    entry.close();
  }
  return dirstr;
}

object* printDirectoryList(File dir, object* dirlist) {
  String dirstr = "";

  while (true) {
    File entry =  dir.openNextFile();
    if (! entry) {
      // no more files
      break;
    }
    dirstr = entry.name();
    if (entry.isDirectory()) {
      dirstr = dirstr + "/";
      dirlist = cons(printDirectoryList(entry, dirlist), dirlist);
    }
    dirlist = cons(lispstring((char*)dirstr.c_str()), dirlist);
    entry.close();
  }
  return dirlist;
}
#endif


// Symbol names

//USB host keyboard supported anytime
const char stringKeyboardGetKey[] PROGMEM = "keyboard-get-key";
const char stringKeyboardFlush[] PROGMEM = "keyboard-flush";
const char stringSearchStr[] PROGMEM = "search-str";

#if defined sdcardsupport
const char stringSDFileExists[] PROGMEM = "sd-file-exists";
const char stringSDFileRemove[] PROGMEM = "sd-file-remove";
const char stringSDCardDir[] PROGMEM = "sd-card-dir";
#endif


// Documentation strings
//USB host keyboard supported anytime
const char docKeyboardGetKey[] PROGMEM = "(keyboard-get-key [pressed])\n"
"Get key last recognized - default: when released, if [pressed] is t: when pressed).";
const char docKeyboardFlush[] PROGMEM = "(keyboard-flush)\n"
"Discard missing key up/down events.";
const char docSearchStr[] PROGMEM = "(search pattern target [startpos])\n"
"Returns the index of the first occurrence of pattern in target, or nil if it's not found\n"
"starting from startpos";

#if defined sdcardsupport
const char docSDFileExists[] PROGMEM = "(sd-file-exists filename)\n"
"Returns t if filename exists on SD card, otherwise nil.";
const char docSDFileRemove[] PROGMEM = "(sd-file-remove filename)\n"
"Delete file with filename. Returns t if successful, otherwise nil.";
const char docSDCardDir[] PROGMEM = "(sd-card-dir)\n"
"Print SD card directory and return nothing [mode 0, optional] or a uLisp string object [mode 1] or a List [mode 2].";
#endif



// Symbol lookup table
const tbl_entry_t lookup_table2[] PROGMEM = {

{ stringKeyboardGetKey, fn_KeyboardGetKey, 0201, docKeyboardGetKey },
{ stringKeyboardFlush, fn_KeyboardFlush, 0200, docKeyboardFlush },
{ stringSearchStr, fn_searchstr, 0224, docSearchStr },
#if defined sdcardsupport
  { stringSDFileExists, fn_SDFileExists, 0211, docSDFileExists },
  { stringSDFileRemove, fn_SDFileRemove, 0211, docSDFileRemove },
  { stringSDCardDir, fn_SDCardDir, 0201, docSDCardDir },
#endif



};

// Table cross-reference functions - do not edit below this line

tbl_entry_t *tables[] = {lookup_table, lookup_table2};
const unsigned int tablesizes[] = { arraysize(lookup_table), arraysize(lookup_table2) };

const tbl_entry_t *table (int n) {
  return tables[n];
}

unsigned int tablesize (int n) {
  return tablesizes[n];
}
