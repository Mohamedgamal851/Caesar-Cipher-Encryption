# Caesar-Cipher-Encryption
This is a parallel c program using MPI to implement Caesar Cipher Encryption algorithm. This is the simplest of all, where every character of the message is replaced by its next 3rd character. Additionally, prompt the user from the console to choose to encode or decode the text.
You need to implement this program in two modes.
First mode reads the input from the console, and the second one reads the input
from a file.

### Example:
### For encryption:
#####     Input: tutorial
#####     Output: wxwruldo
### For decryption:
#####     Input: wxwruldo
#####     Output: tutorial

### Parallelization Scenario:
- Master Process Reads a String from user.
- The String gets scattered among the processes.
- Every process converts the characters of its String portion to be encrypted/decrypted.
- The encrypted/decrypted String portions to be gathered at the master process.
- The master process prints the String.
