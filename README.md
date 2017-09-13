# Project125

Collection of code meant to introduce students to data analysis in particle physics

#### Invariant Mass Code
In order to run this code, you will need to have a root file (preferably with only lepton selections), and put the proper path file in the code. Once this is complete, you can run this code with the command:

    root InvMass.c

After running, a file named `invmass_DY.root` will be generated. You can open this file and plot the invariant mass spectrum with the following commands:

    root invmass_DY.root
    invmassmu->Draw()

