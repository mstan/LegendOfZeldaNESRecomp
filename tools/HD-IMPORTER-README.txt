Zelda Remastered HD importer for LegendOfZeldaNESRecomp v1.9.2 and later

Requires Python 3.11 or later. Extract this entire ZIP before running.
Supply your own original Zelda USA PRG0 ROM and Zelda Remastered 1.3 pack.
No ROM or third-party graphics/audio are included in this tool download.
ZIP discovery and conversion use the shared general-purpose NESRecomp importer.
The Zelda wrapper only supplies the game identity and creator attribution.
See GENERIC-IMPORTER-README.txt for use with other games and Mesen packs.

In a terminal opened in this extracted folder, run:

python tools/import_hdpack.py --pack "E:\Downloads\UnZipMeFirstZeldaRemasteredV1_3.zip" --rom "C:\path\to\Legend of Zelda.NES" --out "Zelda-Remastered.nesmod"

Replace the ROM path with your own file. The pack can be the original outer
download, its inner ZeldaRemastered.zip, or a folder containing hires.txt.
The original download's default artwork is used; alternate art/audio folders
are optional variants and are not installed by this importer.

Open LegendOfZeldaNESRecomp, select your original ROM, and open Mods.
Install Zelda-Remastered.nesmod and enable Zelda Remastered HD before Play.
HD and Voxel display modes are mutually exclusive. To return to stock graphics,
disable HD and restart. Your ROM file is never modified.

Only textures and layered backgrounds are supported. Replacement music and
sound effects are not implemented; the game uses its original NES audio.
The existing renderer reports a few unsupported conditions and one missing
background PNG in the original pack. This importer does not invent those assets.

The original ROM must have SHA-1 dab79c84934f9aa5db4e7dad390e5d0c12443fa2
when the pack's IPS patch is used. PRG1 and already-patched ROMs do not match.
Use a different output filename if Zelda-Remastered.nesmod already exists.

The pack creators' notice is in hdpatch/ZeldaRemasteredReadme.txt and is also
included in the generated local .nesmod. The importer license is in LICENSE.
