# SpatialAudio
Implemetations of three different methods that generate spatial audio effects for stereo headphones on the OMAP L138 DSP board with TI's Code Composer Studio (written in C). <\br>
It first downmixes the two channels (stereo) into one channel (mono) and coverts it to a two-channel (stereo) signal to be played back as the output. <\br>
amplitude_IIDITD.c contains two methods: amplitude panning and IID/ITD methods.<\br>
lambda.gel is a GUI slider that controls pan parameter rannging between 0 and 1 for the amplitude panning method. <\br>
azimuth.gel is a GUI slider that controls azimuth ranging from -pi/2 (left) to pi/2 (right) for the IID/ITD method.<\br>
HRTFcontrol.gel is a GUI slider that controls azimuth ranging from 0 to 360 degrees for the HRTF2.c (head related transfer function method). <\br>
Note that HRTF.c places audio singl to the 120 degrees azimuth location only.<\br>
HRTF.txt HRTF2.txt contain HRTF data.
