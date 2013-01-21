# ScreenLab

[ScreenLab 0x02](screenlabseries.tumblr.com) residency with Kyle McDonald, Joanie Lemercier and Joel Gethin Lewis.

This repository contains openFrameworks code, originally developed for the Octave virtual reality space at The University of Salford.

For further details, check the [Tumblr](screenlabseries.tumblr.com).

## Blurb

For 10 days, 3 internationally renowned digital media artists will be based at the University of Salford, working with the advanced technologies at the Centre for Virtual Environments, and interactive spaces at MediaCityUK Campus.

ScreenLab Residency is supported by the public funding through the Arts Council of England, the University of Salford and FIRM. tes in the Quays programme of events, encouraging interest in cultural activities within the Salford Quays area.

ScreenLab is curated by Elliot Woods [(Kimchi and Chips)](http://www.kimchiandchips.com) and Kit Turner [(University of Salford)](http://www.salford.ac.uk).

## Apps

### AetherBridge

This Processing sketch is the backbone of the audio control. It glues together Ableton Live, vvvv and the wave field synthesis system using a combination of OSC, Midi and UDP, allowing all three applications to talk to each other.

### DepthAO

Inspired by Joanie's use of ambient occlusion and other high level shader effects in vvvv, I tried imitating some of Joanie's look with OF. This example was a quick test of a technique for creating ambient occlusion on a landscape by simply taking a high pass filter on the depth map. While combining the ambient occlusion effect with directional lighting, I accidentally created a bug that caused the landscape to look like internally illuminated clouds.

### InfiniteTunnel

This was the initial version of a transition effect that structurally imitates some of the classic "outer space", "wormhole" transitions from sci-fi films. I left this version in a really simplified state in order to explain the principle of how the tunnel is generated. The camera movement is really rigid in this one.

### Meteorite

This quick sketch was meant to be used as an element within one of the scenes where pyramidal meteorites rain down from above. It was very directly based on a sketch in Joanie's sketchbook that we both liked.

### OctaveAudioControl

This app was used for interfacing to a wave field synthesis app that was only used during testing, and not used in the final piece.

### OctaveMesh

During the residency I took a few hours to make a detailed model of the space in sketchup that we could use both for checking our calibration and as an asset allowing us to create the illusion that the physical space is "melting away". I tested out the mesh in OF just to make sure the Collada file was well structured.

### PlyLoader

This sketch was initially meant to be used as a generic vvvv node for loading simple 3d .ply files, but it turned into an app for drawing Platonic solids as efficiently as possible.

### RandomMesh

This was one of the main effects we were planning on using in multiple scenes. We built custom slippers that were motion captured using the Vicon system (with [ofxVicon](https://github.com/kylemcdonald/ofxVicon)), and every time the slippers would pass a horizontally oriented "ground plane" it would trigger a "drop" in the floor, causing ripples to spread across the surface. We were planning on physically augmenting the floor with electrical tape in order to get clear projection-mapped lines augmented by simple shaded surfaces, but we weren't able to test this in time.

### RandomSteps

Before we had Vicon data for RandomMesh, we needed fake "step" data to test the scene. RandomSteps created this data.

### RippleEffect

The foundation of the RandomMesh look can be found in the height map that is generated using this simple old-school ripple effect.

### TestPattern

While calibrating the space, we needed a simple test pattern to project that would help us align the projectors. This app was meant to accomplish that, but we used vvvv instead.

### TunnelTransition

This is the evolution of InfiniteTunnel, and contains a number of interesting parameters that were tweaked by hand to develop some unique "looks" for transitions from the main space into each of the scenes. One of the more interesting pieces of this code is how the camera movement is created to create a sensation of flying along the general path of the tunnel without being stuck to it as rigidly as in InfiniteTunnel.