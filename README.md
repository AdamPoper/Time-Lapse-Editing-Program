# TimeLapseEditingProgram

The Problem:
When doing a timelapse of the sun setting, the photographer must change the exposure settings of the camera every so often to account for a decrease in the amount of light.
In post production, the developer would have to manually change the exposure offset for each image so that each image looks like it is of the same exposure.
Solution:
This program automates the process of adjusting each individual image exposure offset so that the timelapse in the end doesn't have flickering that denotes the frame
where there was a change in the camera exposure settings. I believe there are already programs out there that do this but I decided to make my own.
This program only works with adobe xmp files for editing photos in Lightroom or Photoshop. It woks by comparing each image in the sequence with the one after it 
for an exposure change in either ISO, shutter speed, or f stop. From there is calculates how much it must offset each image in the sub sequence to make the timelapse look like
each frame is just one exposure. It updates each exposure change to each image's subsequent XMP file to then be read by Lightroom or Photoshop to make the changes take effect.
