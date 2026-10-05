# Unpublished cycle migration preview

Zelda now builds with the cycle CPU by default. The legacy backend remains
available through an explicit CMake selection. Stock preserves the existing
optional voxel diorama and first-person modes, camera controls and package
settings. Remastered preserves the existing HD graphics sampler using actual
PPU fetch metadata; original NES audio continues without replacement music.

The cartridge uses its actual 8 KiB battery geometry, accepting older raw
progress through the launcher. Cycle states save presentation caches and
validate matching modes before changing gameplay. Legacy binary states
remain specific to their old backend.

Local archives contain executables, SDL/UI dependencies and stock packages
or the existing IPS patch helper. They contain no ROM, HD art, player settings
or progress. Owner playtests and branch integration remain pending; these
notes do not announce a published release.
