ADVERTISEMENT: In the MakeFile is important to update the direction where SystemC is installed in your PC. Open the Makefile file and edit the next line: 

SYSTEMC_HOME = /home/user/systemc-version/install

Make sure that inside this folder exist _include_ and lib-linux64 (if your PC is based on x64) or lib-linux (if it is based on x32 or x86)
