# Unpublished cycle migration preview

Zelda now builds with the cycle CPU by default. The legacy backend remains
available through an explicit CMake selection. Stock preserves the existing
optional voxel diorama and first-person modes, camera controls and package
settings. Remastered is now a modern HD-pack Mod in the same stock executable,
using the shared framework importer, installed assets and verified in-memory
IPS patch. Select it through Mods before Play. Original files stay untouched.
The existing graphics sampler uses actual PPU fetch metadata; original NES
audio continues without replacement music.

The cartridge uses its actual 8 KiB battery geometry, accepting older raw
progress through the launcher. Cycle states save presentation caches and
validate matching modes before changing gameplay. Legacy binary states
remain specific to their old backend.

Local archives contain the executable, SDL/UI dependencies, default-off voxel
packages, the shared HD importer and original pack creator notice. They contain
no ROM, HD art, player settings or progress. The owner accepted the earlier
four cycle variants; the updated HD Mod and branch integration remain pending.
These notes do not announce a published release.
