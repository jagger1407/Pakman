# Pakman
A tool to pack and unpack .pak files from the Sparking! / DBZ Budokai Tenkaichi games.

This tool was created to give users the ability to customize the packing and unpacking process. It does so through the use of a section name file.

## Section Name File

The section name file tells Pakman the order (for packing) and name (for unpacking) of each subsection within the .pak file.

An example section name file for the Evolution Z menu would be 
```
Menu_EvolutionZ.dat
Menu_Collection.dat
Menu_List.dat
Menu_ItemFusion.dat
``` 

## Usage
[] = required
<> = optional

**Packing Syntax:**
`pakman pack [pak path] [input folder path] [section name file] <custom alignment>`

Note: If a custom alignment is used, it must be a power of 2.

**Unpacking Syntax:**
`pakman unpack [pak path] [output folder path] <section name file>`

Note: If no section name file is provided, each section will be called "Section_#" where # is the index.
