# Motion Controlled Additive Synthesis
With this we are presenting a additive sound synthesis Pure Data External. Alongside we provide a fun demonstration of controlling it by using smartphone accelerometer and gyroscope data from a smartphone <br />

## Where to start?

The repo consist of the external's source code in addition with other needed c files, the pd external binary (MacOS x64) and two sample patches to use it with [PdParty](http://danomatika.com/code/pdparty/guide)

### Requirements
In order to use the addsi~ external from **External Binary and Help Patch** you either need to have MacOS running on a 64 bit machine, or you build the external yourself using the provided **External's Source**

### Using addsi~ with PdParty
addsi was created to be used with your smartphone! The setup consists of two Pd patches – one on your computer running the addsi~ external embedded in some control logic, and one on your smartphone sending out gyro and accel data to your computer via OSC.

This is how to get it running

1. Download [PdParty](http://danomatika.com/code/pdparty/guide) (iOS) or  [DroidParty](http://droidparty.net) (Android) and see how they work
2. Get the **Composer Pack** provided by the PdParty makers. You can get it [here](http://danomatika.com/code/pdparty/PdParty_composerpack.zip). From the pack you need to add /lib/rj to your Pd search path, because we are using a patch from it in the setup.
3. Now we can start: Grab addsi~smartphone.pd from **Sample Patches** and move it to your device running PdParty and start it. 
4. Tap the buttons to send OSC messages containing accelerometer and gyroscope data. Make sure to set the host IP correctly in the OSC settings, to match the IP adress of you computer!
5. Let's turn back to your computer: Start the addsi~patch.pd from **Sample Patches**, turn on the DSP and click the toggle right on top of the patch.
6. Have fun playing around!
