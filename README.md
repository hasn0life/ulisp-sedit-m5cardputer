# ulisp-sedit-m5cardputer
Port of [ulisp-lispbox](https://github.com/ErsatzMoco/ulisp-lispbox) text editor for the [M5 Cardputer](https://docs.m5stack.com/en/core/Cardputer). Orignal editor was written by https://github.com/ErsatzMoco

## How to install
Add the lisplibrary.h and extensions.ino files to the folder which contains [ulisp-cardputer.ino](https://github.com/technoblogy/ulisp-cardputer). Then follow the setup instructions for [extensions](http://www.ulisp.com/show?19Q4) and [lisplibrary](http://www.ulisp.com/show?27OV) to enable both features. Note: You will also need to change the `lispstring` function in the ulisp-cardputer.ino file to take a `const char*`or you'll get a compiler error. Also you have to enable the `#define largerfont` option

## Known issues
- the `lispstring` function needs to be changed to take a `const char*`
- [superprint issue](http://forum.ulisp.com/t/packages-and-persistent-storage/1318/16) breaks the editing of existing functions by introducing escape characters into the string being edited
- `split-string-to-list` can take up all the RAM if the function is too long
- only works with the larger font option

## Useage
To invoke it type `(se:sedit)` or `(se:sedit 'symbol)` where "symbol" can be any symbol name already present in uLisp
The commands from the orignal lispbox editor work with slight modifications

- CTRL-q / CTRL-c --- quit editor and return to REPL

- CTRL-x / CTRL-b / CTRL-n --- discard current text buffer (i.e. new file)

- CTRL-k / CTRL-l --- delete line starting at cursor position

- CTRL-a / CTRL-, (left arrow) --- move cursor to start of line

- CTRL-e / CTRL-/ (right arrow) --- move cursor to end of line

- ^ --- move cursor to beginning of buffer

- CTRL-; (up arrow) / CTRL-. (down arrow) --- move one page up or down

- Fn-h --- help menu

- Fn-5 --- bind contents of the text buffer to a symbol of your choice and quit editor

- Fn-9 --- delete a file on the SD card

- Fn-0 --- save text buffer to SD card

- Fn-1 --- load text from SD card into buffer, discarding the present one

- Fn-2 --- show directory of SD card Note: file names for SD card follow the 8.3 standard and thus must be given in capital letters.
