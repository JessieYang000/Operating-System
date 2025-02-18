# Module 6

## 📌 Overview
This program generates random strings, processes them using a queue, and encodes them using the **Polybius cipher** in batches. The encoding is performed by a separate process using **pipes** for inter-process communication. The encoded strings are saved into output files.


## ✅ How to Use:
- Automate the compilation and build process of this program by running command ```make``` in terminal
- Run the compiled program main with three command-line arguments.
    - arg1: the number of random strings(at least 10000)
    - arg2: the length of each string
    - arg3: the filename to store the random strings
### Example: 
```./main 10000 5 output.txt```

After running this command, the program prints progress messages like:
```
Writing 10000 random strings with length 5 to output.txt...
Random strings successfully written to output.txt.
Saving strings from file output.txt into queue
10000 strings saved in queue
Successfully generated output_batch_0.txt
Successfully generated output_batch_1.txt
Successfully generated output_batch_2.txt
...
```