# Pa2
# Data Deduplication Project - Intermediate Submission

## 1. Project Group Information
- Project Group: 128
- Ahmed Dayib (dayib007):
- Cheston Opsasnick (opsas002)
- Ahmed Kadar (kadar008)

## 2. Tested On
- **CSELabs Machine**: csel-cuda-01.cselabs.umn.edu

## 3. Changes Made
- added a make zip to the MakeFile

## 4. Individual Contributions
- **[Cheston]**: Worked on Readme, leaf_process.c and root_processc
- **[Ahmed Dayib]** : Worked on Readme and root_process and helped with leaf_process program
- **[Ahmed Kadar]**: Worked out plan and helped with root process and debugging

## 5. Plan for Inter-process Communication
- Root process: creates a pipe and spawns first non_leaf process then collects the agregate data from a pipe.
creates the symlinks, handles delteing the files and displaying results to the output file.
- Non-leaf: processes will be responsible for spawning the appropriate child processes based on the directory structure, passing necessary arguments, and aggregating data from their children.
- Child-process: hashes the data and writes it to the pipe
- Changes from inter: added more detail for what the root process handles

## Gradescope note:
- On gradescope it says it is not able to find our output files. Despite when tested locally it works for the   first three test cases. We tried several different changes to try to fix it to no avail.


