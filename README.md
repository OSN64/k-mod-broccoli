# Simple Kernel RootKit

***Module Initialise Tasks***

1. Locates syscall table
2. Disables page protection
3. Store reference to the syscall
4. Set new syscall
5. Enable page protection

***Module Exit Tasks***

1. Disables page protection
2. Return original syscall
3. Enable page protection

***New Read Syscall***

1. Call original read syscall
2. Print read buffer
3. return syscall output

***New Unlink Syscall***

1. Call original unlink syscall
2. Print pathname buffer
3. return syscall output

***New UnlinkAt Syscall***

1. Check if pathname is not equal to our file path then return success
1. If not call original unlinkat syscall
2. Print pathname buffer
3. return syscall output

NOTES
==

1. Make sure kernel headers are install before running
2. Unlink pathname can be an absolute or relative path
