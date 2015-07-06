PanamaKit
=========

iOS SDK for pulse and tempo synchronization and shared quantization across multiple applications running on multiple devices. Apps that integrate the provided [ABLSync](include/ABLSync.h) library find each other automatically when connected to the same network and are immediately able to play together in time without any configuration.

###Pulse Synchronization
Playing "in time" might have different meanings for different musical use cases or different combinations of apps. At the most basic level, ABLSync provides a shared pulse between instances and exposes this pulse to client apps. By aligning their musical beats with this pulse, apps can be assured that their beats will align with those of other participating apps. Apps can join and leave the session without stopping the music, the library aligns to the pulse stream of an existing session when joining.

###Tempo Synchronization
The rate at which these shared pulses occur is the tempo, which is also synchronized between apps. Any participant in a session may propose changes to the shared tempo, which is propagated to all other participants. Importantly, this may happen while the music is playing - there is no need to stop, change tempo, and then restart.

###Shared Quantization
But simply playing beat-aligned may not be enough in many musical contexts. ABLSync also provides a shared reference grid for quantization, allowing apps to synchronize phase over durations longer or shorter than a single beat.

As an example, consider a case where two users both have a 4 beat loop in their respective apps. In this case playing "in time" may require more than simply aligning pulses. It may require synchronizing the phase of the loops, meaning that the first beats in each loop play together. In other cases, whole beat alignment may be too restrictive and prevent interesting musical interactions, such as playing upbeats or polyrhythms.

The ABLSync library facilitates these use cases by maintaining a beat quantization value that can be controlled by the client app. If this value is set to 1, then the library will implement simple beat synchronization - beats will be aligned between apps with no consideration for any larger musical structures (such as bars or loops). Values greater than 1 lead to phase synchronization across the number of beats specified. Values less than 1 result in sub-beat quantizaton. A value of 0 will result in no quantization, but tempo and pulse synchronization will remain in effect.

##Technical notes for integrators
###Integration concept###
Since the library must negotiate tempo, pulse, and quantization with other participants on the network, the app must defer control over these aspects of playback to the library. For each audio buffer, the integrating app must ask the library where it's supposed to be on the beat timeline by the end of that buffer. The app then figures out how it can render its buffer so as to get to that beat time. This could mean speeding up or slowing down or doing a beat time jump to get to the right position. The library does not specify *how* the app should get there, it just reports where it should be at a given time.

When there are no other participants on the network, or if syncing is disabled, it's guaranteed that no quantization is applied and tempo proposals are handled immediately. This means that client code in the audio callback should call the same ABLSync functions in all cases. There is no need (and it will almost certainly introduce bugs) to try to only use the library functions in the audio callback when syncing is enabled or there are other participants in a session.

###Host and beat times###
The ABLSync API deals with two time coordinate systems: host time and beat time. Establishing a bidirectional mapping between these coordinate systems is one of the primary functions of the library.

Host time as used in the API always refers to the system host time and is the same coordinate system as values returned by [`mach_absolute_time`](https://developer.apple.com/library/mac/qa/qa1398/_index.html) and the `mHostTime` field of the `AudioTimeStamp` structure.

Beat time as used in the API refers to a coordinate system in which integral values represent beats. The library maintains a beat timeline, which starts at zero when the library is initialized and runs continuously at a rate defined by the session tempo. Clients may sample this beat timeline at a given host time via the `ABLSyncBeatTimeAtHostTime` function. Clients may reset this beat time to a chosen value via the `ABLSyncResetBeatTime` function, which is useful for aligning the values on the library's beat timeline to the values on a client app's transport timeline.

###Host time at speaker output###
All host time values used in the ABLSync API refer to host times at speaker output. This is the important value for the library to know, since it must coordinate the timelines of multiple devices such that the same beat times are hitting the speakers of those devices at the same moment. This is made more complicated by the fact that different devices (and even the same device in different configurations) can have different output latencies.

In the audio callback, the system provides an `AudioTimeStamp` value for the audio buffer. The `mHostTime` field of this structure represents the host time at which the audio buffer will be passed to the hardware for output. Adding the output latency (see `AVAudioSession.outputLatency`) to this value will generally result in the correct host time at speaker output for the *beginning* of that buffer. To get the host time at speaker output for the end of the buffer, you would just add the buffer duration. For an example of this caluclation, see the [SyncHut example project](examples/SyncHut/SyncHut/AudioEngine.m).

Note that if your app adds additional software latency, you will need to add this as well in the calculation of the host time at speaker output.
