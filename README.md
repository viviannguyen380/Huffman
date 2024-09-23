# Huffman
A text file compression program to compress (encode) text files into binary and decompress (decode) binary back into text using Huffman Tree coding algorithm. Class project, Storage class created by Paul Raupach.

# Description:
My huffman works using encapsulation to encode text files into binary and decode them back
to their original text. The binary encryption reduces the amount of memory a file has
so it can be sent to another person and decoded.

The user interface asks if you want to compress (encode) or decompress (decode) a file. The
encoding option will ask for the text file name, then will call compress. Compress reads in the file,
 populating the array tracking how many of each char (denoted by its ascii) are in the text file (readInFile),
 creating nodes and enqueing them into a queue for
 the corresponding values and chars and pairing them together with nullchar \0 nodes (createNodes),
 then builds the tree (buildTree), creating codes for each char into the table as it builds. Then
 the file is written in binary, using the tree and given text to build
 a 01 encoded message, then reading it into a Storage object. The header is formatted to contain
 the chars and their equivalent 01 values, as well as the actual length of the 01 (which is
 padded with 0s, aren't part of the message)

The decode option will prompt the user to enter a binary file. The header is obtained, and the tree
is rebuilt based on the codes to move left/right and the characters to place. Then the 01 encoded
message is decoded by traversing the tree. Then the user can save the actual text into a file.

My version implements a special datatype (wchar/wifstream) to allow the conversion of unicode characters as
well as ascii.

