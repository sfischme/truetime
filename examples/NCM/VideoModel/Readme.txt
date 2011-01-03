Video Model for True Time:
----------------------------------

Purpose:
This model demonstrates the benfit of using NCM based scheduling via truetime. Surveillance units, generally consists of more cameras than output devices. This example shows how via using NCM and truetime you can make a schedule for 4 cameras and one output device.

Functionality:
This model takes in 4 surveillance videos and checks for motion in front of them. One of the main aspect of surveillance is to be able to catch motion in front of the cameras. Thus, the schedule in this model, is based on transmitting the camera which has motion in front of it. If there is no motion in front of any camera, then a round robin schedule is followed and all cameras are transmitted for a certain time. In case there is motion in front of any of the cameras, then immediately that camera is transmitted.


Running the Model:
In Simulink, you need to open the motion_detection.mdl model. Then you need to the set the correct video paths for the 4 videos. After running of this model, to get the output, you need to open the SaveToWorkspace.mdl model and run it. This will give you a final video of how things will look at the output device. 

Requirements:
4 Input Videos which are placed in InputVideo folder.

Output:
Output video is placed in the OutputVideo folder.