CIS*4650 A1 - WARMUP ASSIGNMENT README
Name: Daniel Girotto
ID: 0783831
Date: February 4, 2016

---

Problem:

A preprocessing program is needed to filter out irrelevant information from natural
language (possibly human-written) documents. I must build a scanner which takes raw
text as input, and organizes, validates, and structures its contents in such a way that
is useful to the end user.

---

Assumptions & Limitations:

I am assuming that the input files do not have any tags or relevant information stored
outside of the <doc> and </doc> tags. I am also assuming that the <doc> and </doc> tags will be
present for all files (there is no error checking to determine if the pair exists or not). 

---

Building & Testing:

The below steps assume that you have unzipped the .tar.gz file and are currently in
the "dgirotto_warmup" directory.

To build the program simply type "make".
"make clean" will remove all of the object files and executables.

To test the program, first ensure that it is built (compiled), and that an input file
(preferably one of type "txt") is present in the current working directory.

Then type "./scanner < name_of_input_file.txt" to run the scanner against you input file.

---

Test Plan:

Token Exhaustiveness: I tried to make each token as exhaustive as possible (tested the scanner
against plenty of different variations of input to ensure the logic was effective and accurate).
Next to each token is a list of examples which evaluated to that token after sanitization.
 
WORD: 		abc123 123abc thisisaword aNoThErW0rD
NUMBER: 	-123.0 0.01 .23 -.0004 -9.1 -500000 300
APOSTROPHE: 	O'Grady O'RILEY that's o'brian's
HYPHEN: 	son-in-law part-time jack-O-LANTERN
PUNCTUATION: 	! $ , < % / \ ( ' " = ? * . ^ + -
OPEN-TAG: 	<    doc> <DOc align="right"   > <length> <  text  > <  p align="left" color="red">
CLOSE-TAG: 	</doc> <   / doc> < /someTag  >

---

Possible Improvements:

I could store the sanitized input in some sort of a data structure where the user could
access said data if such a need arises. Each of the relevant tags (along with 
their associated attributes) is an example of data which would be relevant to store. 
Perhaps even the ignored tokens (found within irrelevant tags) could be stored in a particular 
structure as well, as it would be informative to know what data was "left out" compared to 
what data made it through to the console.

