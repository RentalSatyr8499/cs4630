# Tricky jump exploit
In this exercise, I create C scripts that infected three different target executables: `target1.exe`, `target2.exe`, and `target3a.exe`. Originally, these executables all output the following in terminal: 
```
Initialize application.
Begin application execution.
Terminate application.
```

But after infection, they print:

```
Initialize application.
You have been infected with a virus!
Begin application execution.
Terminate application. 
```


## The vulnerability
When executables leave a lot of empty space in the executable sections of their binary, virus authors can inject their own code into that space.
* For `target1.exe`, I found a no-op island by looking around in `objdump -d target1.exe`.
* For `target2.exe` and `target3a.exe`, I identified a cavity with executable permissions using `readelf -l`. (Cavities appeared after the end of the R-X segment, because of how coarse-grained hardware-enforced perms are - the entire page is executable, not just the segment itself). 
Additionally, the tricky jump exploit is enabled by the fact that these executables are non-PIE. Virtual addresses that I found via Ghidra and GDB are hard coded into all of my C scripts here. I found the file offset of each symbol via `objdump -d --file-offsets`, then added that to the instruction offset from the symbol (found by subtracting virtual addresses). 

## The exploit
After finding the cavity, I needed to inject a payload there that would print "You have been infected with a virus!\n". I used a syscall to do this. I wrote my desired payload in x86 (AT&T) in `exploit/payload.s`, `objdump`ed it, and wrote it to the cavity in each of my C scripts.

The only thing left to do now was redirect program flow to my payload. Finally! This is where I got to execute the tricky jump:
1. Find a `ret` command
2. Overwrite the previous command with `pushq [cavity location]`
3. Insert overwritten commands at the end of the payload to preserve original program functionality

## To run
1. In a Linux environment with `gcc` installed, open a terminal in `TRICKY/`. 
2. To run the original uninfected executables (you may need to `chmod +x` them first):
    * `./targets/target1.exe`
    * `./targets/target2.exe`
    * `./targets/target3a.exe`
3. To run the exploit:
    * Run `make infect`
    * Now infected versions of the files are in `./infected-targets/`. To run each (you may need to `chmod +x` them first as well): 
        * `./infected-targets/target1.exe`
        * `./infected-targets/target2.exe`
        * `./infected-targets/target3a.exe`
4. `make clean` if you want

## Extension
These C scripts only work on executables with the very specific layout they were designed for. If I were to write an exploit that automatically handled executables with slightly different layouts, my exploit would:
1. Parse the ELF header to find where the executable segment ends. 
2. If it finds that there's slack space between the executable segment and the next segment, it chooses that "cavity" as the location for the payload. 
3. Then it would find the first "pushq $value, ret" it can. It would save those bytes, then overwrite them. 
4. Finally (as `infect3a.c` does) it injects the saved bytes back into where the payload is to restore original program functionality.