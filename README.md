# Pa2
# Data Deduplication Project - Intermediate Submission

## 1. Project Group Information
- Project Group: 128
- Ahmed Dayib (dayib007):
- Cheston Opsasnick (opsas002)
- Ahmed Kadar (kadar008)

## 2. Tested On
- **CSELabs Machine**: csel-kh1250-01.cselabs.umn.edu

## 3. Changes Made
- Implemented the `leaf_process.c` functionality as per the project requirements. This involves:
  - Extracting file paths and pipe write ends from command-line arguments.
  - Computing hash values of the file's content.
  - Constructing and writing strings to pipes or output files.
  - Extracting filenames, determining output file locations, and freeing allocated memory.
  - Made changes to MakeFile for it to work on intermediate submission.

## 4. Individual Contributions
- **[Cheston]**: Worked on leaf_process.c program
- **[Ahmed Dayib]**:Worked on Readme and helped with leaf_process program
- **[Ahmed Kadar]**: Worked out plan and debug program.

## 5. Plan for Inter-process Communication
- The parent processes will aggregate information received from the child processes through the read end of the pipes.
- Non-leaf processes will be responsible for spawning the appropriate child processes based on the directory structure, passing necessary arguments, and aggregating data from their children.
- Our goal is to ensure smooth data transfer between processes and handle potential errors efficiently.


