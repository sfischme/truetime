
Usage:
ttNCM_Wrapper.mdl gives the flexibility of adding the network number to the NCM network. Earlier, we could only run NCM network on network number 1. This wrapper, works similar to NCM.mdl. Instead of placing NCM.mdl in the model, you place ttNCM_Wrapper.mdl with the same inputs. One additional thing that needs to be done is to specify the network number by double clicking on ttNCM_Wrapper block, similar to other truetime blocks.

Structure:
Under the ttNCMWrapper.mdl, the NCM.mdl is present. It takes both the inputs, the next node and network number from ttNCMWrapper.mdl and calls the ttNCM.cpp with those values to update the next node on the current network.

Other Changes:
ttNCM.cpp has been modified to use 2 inputs now and initializing network number during simulation runs.

Bugs Encountered:
In truetime, there is a parameter, nextHit. This parameter speicifies when the next data transfer should happen. It seems in the truetime block, this parameter is not initialized. Hence, sometimes it takes some garbage values and the simulation does not run as expected. To fix this temporaily, within the ttNCM.cpp file, this parameter is updated at every run time. 