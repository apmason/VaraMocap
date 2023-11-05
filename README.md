# VaraMocap

VaraMocap is an open-source motion capture solution. This project contains code for a UE5 app deployed to Meta Quest 2/3 as well as an Unreal Engine plugin.

This project is built using [Meta's Unreal Engine fork](https://github.com/Oculus-VR/UnrealEngine). Specifically, [this commit](https://github.com/Oculus-VR/UnrealEngine/commit/078a7a84eb44a77b42787063867eee2667740f3a). Note that you'll need to get access to the EpicGames/UnrealEngine repo to view the fork.

## Installation

Clone this repo. Set the Unreal Engine version to the aformentioned commit. Open `VaraMocap.sln` and then Build and Run. Connect your Quest 2/3 headset to your computer and deploy to your headset.

This project is a work in progress. Eventually I will have a pre-packaged .apk, and eventually have it on App Lab, but for right now you'll need to build manually.

## Plugin

When you open this project, the plugin is already installed. The plugin runs a HTTP server that will receive requests from your headset.

Right now, the IP address that the headset uses is hardcoded in the app. You'll need to set that yourself for the time being for the system to work properly.
(I haven't added a keyboard/textfield entry for IP address yet. Soon.)

The plugin receives recordings from your VR headset and applies those to a known `Skeletal Mesh` as a new `Animation Sequence`.

You can then retarget that animation to any other Skeletal Mesh using `IKRetargeting`.

If you want to pull this plugin in to your project you'll need to add 
```
[HTTPServer.Listeners]
DefaultBindAddress=0.0.0.0
```
to your `DefaultEngine.ini` so that the plugin can be communicated to from another device on your network (the VR headset).

To bring the plugin in to another project you'll need to build this project, then copy `Vara` in `Plugins/Vara` over to your new project.

Please contribute if you feel so inclined. This is an evolving project.

