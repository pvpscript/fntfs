# Introduction
Friendly NTFS is a tool for detecting files and folder with names that would create problems when read under the Windows operational system and rename them in a friendly way.

## Why?
The Linux kernel is more flexible when it comes to naming files, which means that a user operating a Linux machine is able to name a file or folder pretty much anything they want, even when in an NTFS file system. This hability to give wathever name to a file or folder is good, but it might create some problems when trying to open those said entries inside a Windows machine because of reserved names and characters.

Therefore, this program was created based on the [Windows naming conventions](https://docs.microsoft.com/en-us/windows/win32/fileio/naming-a-file) so that it searches for entries that doesn't follow those conventions, said to be potentially problematic names, and rename them.

## Configuration
All configurations and naming rules are set under the `config.h` file.

# Usage
`./fntfs [OPTION...] path1 [path2 ...]`

## Options
`-h` Show help and exit;

`-i` Prompt before rename, in case an entry with the new name already exists;

`-v` Explain what is begin done.


# Installation
To perform a manual installation, follow these steps:
```
git clone https://github.com/pvpscript/friendly-ntfs
cd friendly-ntfs
make
sudo make install
```

### TODO
* Display a warning if the entry has depth greater than the default maximum allowed.
