################################################################################
#                         🟢 MATRIX MANIPULATION PROGRAM 🟢                      #
################################################################################

********************************************************************************
🎯 **Overview**
********************************************************************************
This C program is designed for **various matrix operations**. It allows users to:
✅ **Create, manipulate, and perform calculations on matrices**  
✅ **Execute operations like multiplication, transposition, sorting, resizing,**  
✅ **And raising a matrix to a power.**

********************************************************************************
🚀 **FEATURES**
********************************************************************************
🔹 **Loading Matrices** (Command **'L'**): Load a new matrix with specified dimensions and values.  
🔹 **Displaying Matrix Dimensions** (Command **'D'**): Display the dimensions of a specific matrix.  
🔹 **Printing Matrices** (Command **'P'**): Print the contents of a matrix.  
🔹 **Matrix Multiplication** (Command **'M'**): Multiply two matrices.  
🔹 **Transposing Matrix** (Command **'T'**): Transpose a given matrix.  
🔹 **Deleting Matrix** (Command **'F'**): Free the memory allocated to a matrix.  
🔹 **Sorting Matrices** (Command **'O'**): Sort all matrices based on the sum of their elements.  
🔹 **Resizing Matrix** (Command **'C'**): Resize a given matrix.  
🔹 **Raising Matrix to Power** (Command **'R'**): Raise a matrix to a specified power.  
🔹 **Quit Program** (Command **'Q'**): Exit the program.  

********************************************************************************
🛠 **USAGE GUIDE**
********************************************************************************
### 🏗 Compilation:
To compile the program using **GCC**, run:  

gcc -o matrix_program my_octave.c
▶ Running the Program:

./matrix_program
⌨ Entering Commands:
Once the program is running, enter commands as described above.
For example:

To load a matrix:

L 3 3 
# (Followed by the 3x3 matrix elements)
To print a matrix with index 1:

P 1
🔎 DETAILED CODE STRUCTURE

📌 Matrix Data Structure:

Uses a struct to manage matrices, including their elements and dimensions.
📌 Dynamic Memory Allocation:

Employs malloc() and free() for flexible memory management.
📌 Error Handling:

Includes robust validation for invalid inputs and memory allocation failures.
📌 Command-Driven Interface:

Designed for ease of use, with simple command-line operations.
📌 Optimized Operations:

Implements efficient algorithms for multiplication, transposition, and exponentiation.
📌 Modular Functions:

Code is structured into independent functions, making it easy to read and maintain.
📌 CODE QUALITY CHECK

To ensure the code follows best practices, use the provided checker script:

./check
