# v1.9.4 - import original Mesen downloads directly in Mods

Zelda Remastered HD now includes a content-free support .nesmod and a native general-purpose Mesen importer. In Mods, choose the original downloaded ZIP with Select file, enable the feature, and click Play. No Python, manual extraction, generated texture package or prepatched ROM is required.

The .nesmod provides integration rules only. Creator textures, audio, IPS and ROM content are not shipped. The shared runtime discovers the user's pack, preserves its notices in a private cache, validates its original-ROM requirement and applies the user-supplied IPS in memory. The download and original ROM stay unchanged, and the ZIP can be removed after importing.

The importer handles direct ZIPs, pack directories inside ZIPs, nested wrapper downloads and extracted hires.txt files without game-specific archive names. Remastered textures/backgrounds and the existing optional voxel modes are retained. Replacement music and sound effects remain unsupported.
