# Task 0b Testing Guide

## Compilation

```bash
make
# or
gcc -m32 -Wall -g -o hexeditplus task1.c
```

## Testing Checklist

### 1. Test Toggle Debug Mode (Option 0)

```
./hexeditplus
> 0
```

**Expected output:**
```
Debug flag now on
```

Then run option 0 again:
```
> 0
```

**Expected output:**
```
Debug flag now off
```

### 2. Test Debug Info Display

With debug mode ON, you should see these variables before each menu:
```
unit_size: 1
file_name: 
mem_count: 0
```

### 3. Test Set File Name (Option 1)

```
> 1
Enter file name: abc
```

**Expected (if debug is on):**
```
Debug: file name set to 'abc'
```

### 4. Test Set Unit Size (Option 2)

**Valid input:**
```
> 2
Enter unit size (1, 2, or 4): 2
```

**Expected (if debug is on):**
```
Debug: set size to 2
```

**Invalid input:**
```
> 2
Enter unit size (1, 2, or 4): 3
```

**Expected:**
```
Error: Invalid unit size. Must be 1, 2, or 4.
```

### 5. Test Stub Functions (Options 3-7)

Each should print:
```
Not implemented yet
```

### 6. Test Quit (Option 8)

```
> 8
```

**Expected (if debug is on):**
```
quitting
```

Then program exits.

## Full Test Session Example

```
./hexeditplus

Choose action:
0-Toggle Debug Mode
1-Set File Name
2-Set Unit Size
3-Load Into Memory
4-Toggle Display Mode
5-Memory Display
6-Save Into File
7-Memory Modify
8-Quit 

> 0
Debug flag now on

unit_size: 1
file_name: 
mem_count: 0

Choose action:
0-Toggle Debug Mode
1-Set File Name
2-Set Unit Size
3-Load Into Memory
4-Toggle Display Mode
5-Memory Display
6-Save Into File
7-Memory Modify
8-Quit 

> 1
Enter file name: abc
Debug: file name set to 'abc'

unit_size: 1
file_name: abc
mem_count: 0

Choose action:
0-Toggle Debug Mode
1-Set File Name
2-Set Unit Size
3-Load Into Memory
4-Toggle Display Mode
5-Memory Display
6-Save Into File
7-Memory Modify
8-Quit 

> 2
Enter unit size (1, 2, or 4): 4
Debug: set size to 4

unit_size: 4
file_name: abc
mem_count: 0

Choose action:
0-Toggle Debug Mode
1-Set File Name
2-Set Unit Size
3-Load Into Memory
4-Toggle Display Mode
5-Memory Display
6-Save Into File
7-Memory Modify
8-Quit 

> 3
Not implemented yet

unit_size: 4
file_name: abc
mem_count: 0

Choose action:
0-Toggle Debug Mode
1-Set File Name
2-Set Unit Size
3-Load Into Memory
4-Toggle Display Mode
5-Memory Display
6-Save Into File
7-Memory Modify
8-Quit 

> 8
quitting
```

## Key Implementation Features ✅

- ✅ Menu system using function pointer array
- ✅ NULL-terminated menu array
- ✅ Global state variables (debug_mode, file_name, unit_size, mem_buf, mem_count)
- ✅ Toggle Debug Mode implementation
- ✅ Set File Name implementation
- ✅ Set Unit Size implementation (with validation)
- ✅ Quit implementation
- ✅ Stub functions for Task 1
- ✅ Debug info printed before menu when debug mode is on
- ✅ All debug messages go to stderr

