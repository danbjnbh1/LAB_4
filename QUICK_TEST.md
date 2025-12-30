# Quick Test Script for Task 1

## Compile and Run

```bash
cd /path/to/LAB_4
make clean
make
./hexeditplus
```

## Quick Test Sequence

```
# Enable debug mode
0
Debug flag now on

# Set file name
1
abc

# Set unit size to 2 bytes
2
2

# Load 5 units (10 bytes) from offset 0x12F
3
12F 5

# Display memory in hex
5
0 5

# Toggle to decimal
4

# Display memory in decimal
5
0 5

# Toggle back to hex
4

# Modify first unit
7
0 1234

# Display to see change
5
0 5

# Try with unit size 1
2
1

# Load some data
3
130 10

# Display as individual bytes
5
0 10

# Quit
8
```

## Expected Output Highlights

After loading (step 3):
```
Loaded 5 units into memory
```

After first display (step 5 - hex):
```
Hexadecimal
===========
0x100
0x0
0x2f00
0x696c
0x2f62
```

After toggle (step 4):
```
Decimal display flag now on, decimal representation
```

After second display (step 5 - decimal):
```
Decimal
=======
256
0
12032
26988
12130
```

After modify and display (step 7 & 5):
```
Hexadecimal
===========
0x1234    ← Changed!
0x0
0x2f00
0x696c
0x2f62
```

## Verify with readelf

Find entry point of hexeditplus:
```bash
readelf -h hexeditplus | grep "Entry point"
```

Then in hexeditplus:
```
1
hexeditplus
2
4
3
18 1
5
0 1
```

The displayed value should match the entry point address!

## Test with Patching (Advanced)

```bash
# Make a backup
cp abc abc_test

# In hexeditplus:
1
abc_test
2
1
3
40 5
7
0 90
7
1 90
6
0 40 2

# Verify with hexdump
hexdump -C abc_test | grep "00000040"
```

Should see `90 90` (NOP instructions) written!

---

## All Functions Working? ✅

- [x] Toggle Debug Mode (0)
- [x] Set File Name (1)
- [x] Set Unit Size (2)
- [x] Load Into Memory (3)
- [x] Toggle Display Mode (4)
- [x] Memory Display (5)
- [x] Save Into File (6)
- [x] Memory Modify (7)
- [x] Quit (8)

Task 1 Complete! 🎉


