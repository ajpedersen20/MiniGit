MiniGit
Alex Pedersen
CSCI 2270

This version of Git includes four main features: adding files, removing files, committing changes, and checking out previous commits.

To add a file, the user selects option one and enters a valid filename. The file is then tracked by the program.

To remove a file, the user selects option two and enters a valid filename. The program will then remove the file from the list of
tracked files.

To commit the files that are being tracked, the user should select option 3. Then, any files that are either new or have had
changes made to them will be copied into the .minigit directory.

To checkout a previous commit, the user should select option 4. The program will then issue a warning that the user must enter
'Y' for yes. Lastly, the user will be prompted to enter the commit number that they want to check out. Then, the main directory files
that were committed with that commit number will be restored to the versions stored in the commit.

The program ends when the user selects option 5 from the main menu.

I developed this project using C++ within a WSL2 environment.
