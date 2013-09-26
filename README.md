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
The program assumes that hyphens delimit words, so 'mother-in-law' is treated as three
distinct words.

It also assumes that ' is not a delimiter (as it can be used in words with contractions).

The program does consider numbers to be words.

Apart from ', the program removes all punctuation and unprintable characters.

The program does not currently support Unicode.