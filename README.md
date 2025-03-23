Overview
--------
Reads in a file and prints an alphabetical list of words in that file, with each word
followed by a list of lines that word appears on.

Usage
-----
```
./concordance <filename>
```

Assumptions
-----------
* That hyphens delimit words, so 'mother-in-law' is treated as three distinct words
* That ' is not a delimiter (as it can be used in words with contractions)
* That groups of digits are words
* Apart from ', the program removes all punctuation and unprintable characters.
* That all input files are ASCII (or will be interpreted as ASCII)
* That lines are limited to 1024 characters in length
* That words that appear multiple times on the same line will have that line listed multiple times for that word

Files
-----
* main.c - handles command line parameters and driving the rest of the program
* display.c - handles displaying the information to the user
* process.c - handles reading a file in to our data structure
* state.c - handles basic management of the data structure

Notes
-----
This repo is now being used to test https://github.com/slsa-framework/slsa-source-poc.
