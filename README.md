# ulisp-sedit-m5cardputer
Port of [ulisp-lispbox](https://github.com/ErsatzMoco/ulisp-lispbox) text editor for the [M5 Cardputer](https://docs.m5stack.com/en/core/Cardputer). Orignal editor was written by https://github.com/ErsatzMoco

## How to install
Add the lisplibrary.h and extensions.ino files to the folder which contains [ulisp-cardputer.ino](https://github.com/technoblogy/ulisp-cardputer). Then follow the setup instructions for [extensions](http://www.ulisp.com/show?19Q4) and [lisplibrary](http://www.ulisp.com/show?27OV) to enable both features. Also you have to enable the `#define largerfont` option

## Known issues
- [superprint issue](http://forum.ulisp.com/t/packages-and-persistent-storage/1318/16) breaks the editing of existing functions by introducing escape characters into the string being edited
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

- Fn-\[ --- toggle bracket matching on/off

- Fn-\] --- check if bracket under the cursor has a matching bracket in the buffer. If so, they are temporarily highlighted. (Use when continuous bracket matching is off.)

- Fn-5 --- bind contents of the text buffer to a symbol of your choice and quit editor

- Fn-9 --- delete a file on the SD card

- Fn-0 --- save text buffer to SD card

- Fn-1 --- load text from SD card into buffer, discarding the present one

- Fn-2 --- show directory of SD card 
