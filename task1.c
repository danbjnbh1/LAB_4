#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* Global state variables */
char debug_mode = 0;
char file_name[128] = "";
int unit_size = 1;
unsigned char mem_buf[10000];
size_t mem_count = 0;

/* Display mode flag: 0 = hex, 1 = decimal */
int display_flag = 0;

/* Format strings for printing different unit sizes */
static char* hex_formats[] = {"%#hhx\n", "%#hx\n", "No such unit", "%#x\n"};
static char* dec_formats[] = {"%#hhd\n", "%#hd\n", "No such unit", "%#d\n"};

/* Function declarations */
void toggle_debug_mode();
void set_file_name();
void set_unit_size();
void load_into_memory();
void toggle_display_mode();
void memory_display();
void save_into_file();
void memory_modify();
void quit_program();

/* Menu structure */
struct fun_desc {
    char *name;
    void (*fun)();
};

/* Menu array - NULL terminated */
struct fun_desc menu[] = {
    {"Toggle Debug Mode", toggle_debug_mode},
    {"Set File Name", set_file_name},
    {"Set Unit Size", set_unit_size},
    {"Load Into Memory", load_into_memory},
    {"Toggle Display Mode", toggle_display_mode},
    {"Memory Display", memory_display},
    {"Save Into File", save_into_file},
    {"Memory Modify", memory_modify},
    {"Quit", quit_program},
    {NULL, NULL}
};

/* Functions */

void toggle_debug_mode() {
    if (debug_mode) {
        debug_mode = 0;
        fprintf(stderr, "Debug flag now off\n");
    } else {
        debug_mode = 1;
        fprintf(stderr, "Debug flag now on\n");
    }
}

void set_file_name() {
    char input[128];
    printf("Enter file name: ");
    if (fgets(input, sizeof(input), stdin)) {
        /* Remove newline if present */
        size_t len = strlen(input);
        if (len > 0 && input[len-1] == '\n') {
            input[len-1] = '\0';
        }
        strcpy(file_name, input);
        
        if (debug_mode) {
            fprintf(stderr, "Debug: file name set to '%s'\n", file_name);
        }
    }
}

void set_unit_size() {
    int size;
    printf("Enter unit size (1, 2, or 4): ");
    if (scanf("%d", &size) == 1) {
        /* Clear input buffer */
        while (getchar() != '\n');
        
        if (size == 1 || size == 2 || size == 4) {
            unit_size = size;
            if (debug_mode) {
                fprintf(stderr, "Debug: set size to %d\n", unit_size);
            }
        } else {
            fprintf(stderr, "Error: Invalid unit size. Must be 1, 2, or 4.\n");
        }
    } else {
        fprintf(stderr, "Error: Invalid input.\n");
        /* Clear input buffer */
        while (getchar() != '\n');
    }
}

/* Task 1a: Load Into Memory */
void load_into_memory() {
    char input[256];
    unsigned int location;
    int length;
    FILE *fp;
    
    /* Check if file_name is empty */
    if (strlen(file_name) == 0) {
        fprintf(stderr, "Error: No file name set. Use option 1 first.\n");
        return;
    }
    
    /* Open file for reading */
    fp = fopen(file_name, "rb");
    if (fp == NULL) {
        fprintf(stderr, "Error: Cannot open file '%s' for reading.\n", file_name);
        return;
    }
    
    /* Prompt for location (hex) and length (decimal) */
    printf("Please enter <location> <length>\n");
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fclose(fp);
        return;
    }
    
    if (sscanf(input, "%x %d", &location, &length) != 2) {
        fprintf(stderr, "Error: Invalid input format.\n");
        fclose(fp);
        return;
    }
    
    /* Debug output */
    if (debug_mode) {
        fprintf(stderr, "Debug: file_name: %s, location: 0x%x, length: %d\n", 
                file_name, location, length);
    }
    
    /* Check buffer size */
    if (length * unit_size > 10000) {
        fprintf(stderr, "Error: Data size exceeds buffer size.\n");
        fclose(fp);
        return;
    }
    
    /* Seek to location and read data */
    if (fseek(fp, location, SEEK_SET) != 0) {
        fprintf(stderr, "Error: Cannot seek to location 0x%x.\n", location);
        fclose(fp);
        return;
    }
    
    mem_count = fread(mem_buf, unit_size, length, fp);
    fclose(fp);
    
    printf("Loaded %zu units into memory\n", mem_count);
}

/* Task 1b: Toggle Display Mode */
void toggle_display_mode() {
    if (display_flag) {
        display_flag = 0;
        printf("Decimal display flag now off, hexadecimal representation\n");
    } else {
        display_flag = 1;
        printf("Decimal display flag now on, decimal representation\n");
    }
}

/* Task 1c: Memory Display */
void memory_display() {
    char input[256];
    unsigned int addr;
    int u;
    unsigned char *ptr;
    
    /* Prompt for address (hex) and length (decimal) */
    printf("Enter address and length\n");
    printf("> ");
    if (fgets(input, sizeof(input), stdin) == NULL) {
        return;
    }
    
    if (sscanf(input, "%x %d", &addr, &u) != 2) {
        fprintf(stderr, "Error: Invalid input format.\n");
        return;
    }
    
    /* If addr is 0, use mem_buf, otherwise use the virtual address */
    if (addr == 0) {
        ptr = mem_buf;
    } else {
        ptr = (unsigned char *)addr;
    }
    
    /* Print header */
    if (display_flag) {
        printf("Decimal\n");
        printf("=======\n");
    } else {
        printf("Hexadecimal\n");
        printf("===========\n");
    }
    
    /* Print units */
    for (int i = 0; i < u; i++) {
        unsigned int val = 0;
        
        /* Read unit_size bytes and construct value (little endian) */
        for (int j = 0; j < unit_size; j++) {
            val |= (ptr[i * unit_size + j] << (j * 8));
        }
        
        /* Print based on display mode and unit size */
        if (display_flag) {
            /* Decimal display */
            printf(dec_formats[unit_size - 1], val);
        } else {
            /* Hexadecimal display */
            printf(hex_formats[unit_size - 1], val);
        }
    }
    printf("\n");
}

/* Task 1d: Save Into File */
void save_into_file() {
    char input[256];
    unsigned int source_addr;
    unsigned int target_location;
    int length;
    FILE *fp;
    unsigned char *ptr;
    
    /* Check if file_name is empty */
    if (strlen(file_name) == 0) {
        fprintf(stderr, "Error: No file name set. Use option 1 first.\n");
        return;
    }
    
    /* Prompt for source-address, target-location, and length */
    printf("Please enter <source-address> <target-location> <length>\n");
    if (fgets(input, sizeof(input), stdin) == NULL) {
        return;
    }
    
    if (sscanf(input, "%x %x %d", &source_addr, &target_location, &length) != 3) {
        fprintf(stderr, "Error: Invalid input format.\n");
        return;
    }
    
    /* Debug output */
    if (debug_mode) {
        fprintf(stderr, "Debug: source_addr: 0x%x, target_location: 0x%x, length: %d\n",
                source_addr, target_location, length);
    }
    
    /* Determine source pointer */
    if (source_addr == 0) {
        ptr = mem_buf;
    } else {
        ptr = (unsigned char *)source_addr;
    }
    
    /* Open file for writing (not truncating) */
    fp = fopen(file_name, "r+b");
    if (fp == NULL) {
        fprintf(stderr, "Error: Cannot open file '%s' for writing.\n", file_name);
        return;
    }
    
    /* Check if target_location is within file size */
    fseek(fp, 0, SEEK_END);
    long file_size = ftell(fp);
    if (target_location > file_size) {
        fprintf(stderr, "Error: Target location 0x%x exceeds file size.\n", target_location);
        fclose(fp);
        return;
    }
    
    /* Seek to target location and write data */
    if (fseek(fp, target_location, SEEK_SET) != 0) {
        fprintf(stderr, "Error: Cannot seek to location 0x%x.\n", target_location);
        fclose(fp);
        return;
    }
    
    size_t written = fwrite(ptr, unit_size, length, fp);
    fclose(fp);
    
    if (written != length) {
        fprintf(stderr, "Warning: Only wrote %zu units out of %d.\n", written, length);
    }
}

/* Task 1e: Memory Modify */
void memory_modify() {
    char input[256];
    unsigned int location;
    unsigned int val;
    
    /* Prompt for location and val (both in hex) */
    printf("Please enter <location> <val>\n");
    if (fgets(input, sizeof(input), stdin) == NULL) {
        return;
    }
    
    if (sscanf(input, "%x %x", &location, &val) != 2) {
        fprintf(stderr, "Error: Invalid input format.\n");
        return;
    }
    
    /* Debug output */
    if (debug_mode) {
        fprintf(stderr, "Debug: location: 0x%x, val: 0x%x\n", location, val);
    }
    
    /* Check if location is valid */
    if (location + unit_size > sizeof(mem_buf)) {
        fprintf(stderr, "Error: Location 0x%x exceeds buffer size.\n", location);
        return;
    }
    
    /* Write value to memory in little endian */
    for (int i = 0; i < unit_size; i++) {
        mem_buf[location + i] = (val >> (i * 8)) & 0xFF;
    }
}

void quit_program() {
    if (debug_mode) {
        fprintf(stderr, "quitting\n");
    }
    exit(0);
}

/* Main Program */

void print_menu() {
    printf("\nChoose action:\n");
    for (int i = 0; menu[i].name != NULL; i++) {
        printf("%d-%s\n", i, menu[i].name);
    }
    printf("\n");
}

int main(int argc, char **argv) {
    int choice;
    char input[256];
    
    while (1) {
        /* Print debug info if debug mode is on */
        if (debug_mode) {
            fprintf(stderr, "unit_size: %d\n", unit_size);
            fprintf(stderr, "file_name: %s\n", file_name);
            fprintf(stderr, "mem_count: %zu\n", mem_count);
        }
        
        /* Print menu */
        print_menu();
        
        /* Get user choice */
        printf("> ");
        if (fgets(input, sizeof(input), stdin) == NULL) {
            break;
        }
        
        if (sscanf(input, "%d", &choice) != 1) {
            fprintf(stderr, "Error: Invalid input.\n");
            continue;
        }
        
        /* Count menu items */
        int menu_size = 0;
        while (menu[menu_size].name != NULL) {
            menu_size++;
        }
        
        /* Execute chosen function */
        if (choice >= 0 && choice < menu_size) {
            menu[choice].fun();
        } else {
            fprintf(stderr, "Error: Invalid choice. Please select 0-%d.\n", menu_size - 1);
        }
    }
    
    return 0;
}

