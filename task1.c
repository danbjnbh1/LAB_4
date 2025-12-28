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

void load_into_memory() {
    printf("Not implemented yet\n");
}

void toggle_display_mode() {
    printf("Not implemented yet\n");
}

void memory_display() {
    printf("Not implemented yet\n");
}

void save_into_file() {
    printf("Not implemented yet\n");
}

void memory_modify() {
    printf("Not implemented yet\n");
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

