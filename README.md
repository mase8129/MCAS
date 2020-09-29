# Motion Controlled Additive Synthesis
With this we are presenting a additive sound synthesis Pure Data External. Alongside we provide a fun demonstration of controlling it by using smartphone accelerometer and gyroscope data from a smartphone <br />

## Where to start?

The repo consist of the external's source code in addition with other needed c files, the pd external binary (MacOS x64) and two sample patches to use it with [PdParty](http://danomatika.com/code/pdparty/guide)

### Requirements
In order to use the addsi~ external from [External Binary](/External Binary and Help Patch) you either need to have a MacOs running on a 64 bit machine, or you build the external yourself using the provided [External's Source](/External's Source)

### Using addsi~ with PdParty
addsi was created to be used with your smartphone! How to?
1. Download [PdParty](http://danomatika.com/code/pdparty/guide) (iOS) or  [DroidParty](http://droidparty.net) (Android) and 
2. Grab the smartphone patch from [Sample Patches](Sample Patches) and move it to your device and start it. Tap the buttons to send OSC messages containing accelerometer and gyroscope data. Make sure to set the host IP correctly in the OSC settings, to match the IP adress of you computer!
3. Start the addsi~patch.pd from [Sample Patches](/Sample Patches) on your computer
4. Have fun!
