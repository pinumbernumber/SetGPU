
# SetGPU

## Force a game to use your preferred GPU

You might be aware of a workaround- first discovered [by reddit user /u/bryf50](https://www.reddit.com/r/hardware/comments/998zkw/freesync_on_an_nvidia_gpu_through_an_amd_gpu/) and later covered by [various youtubers](https://www.youtube.com/watch?v=qUYRZHFCkMw)- to get Freesync working while playing on an Nvidia card: Use an AMD GPU as an intermediary. Right now, this only works with APUs thanks to a Win10 power saving hack, and with dGPUs on a very small handful of games that let you select a GPU.

This tool works in any game (or, as of right now, any DX11 x64 game) and supports dedicated AMD cards.

## WARNING WARNING WARNING

Very work in progress. Not user friendly at all. DX11 64-bit only. **Use in multiplayer games at your own risk** (it's not a cheat, but may well trip battleye etc due to the way it works. Not tested in multiplayer games.) Intended for use only by hardware enthusiasts who are happy to jump through hoops and who have patience for inevitable issues.

This work has a restrictive proprietary license for now. See LICENSE.md for details. You can use it freely for personal, non-commercial purposes, but must not redistribute it, release forks of it (except that you may of course create a fork for the purposes of sending a pull request etc), etc. It may be re-released under a more permissive license eventually.

## Instructions


### Hardware requirements
* An Nvidia card installed in your PC
* An AMD card [that supports freesync](https://www.amd.com/en/technologies/free-sync) also installed in your PC
* *(In the typical/intended use case the NV card would be much more powerful than the AMD one, but that isn't actually necessary for testing.)*
* A freesync monitor
* Some single-player 64-bit DX11 games to test with. See above warning re. multiplayer.

### Getting started

* Connect your freesync monitor (and other monitors if you wish, it shouldn't matter) to your AMD card
* Disconnect all monitors from your Nvidia card
* Fire up a 64-bit DirectX 11 game and verify (using some OSD tool, or just by observing which fans are spinning) that the game is using your AMD card- which is the problem we're trying to solve
* Close the game, download [the latest version of SetGPU](https://github.com/pinumbernumber/SetGPU/releases), extract it, and place those three DLLs beside the game's EXE. Try again and hopefully the game should be using your NV card, but with video output still going through your AMD card and freesync still working.

If you run into problems, please open an issue. Let me know which game(s) you were trying and post a screenshot of the output from [this tool](https://github.com/pinumbernumber/misctemp105/raw/master/listgpu.exe).
