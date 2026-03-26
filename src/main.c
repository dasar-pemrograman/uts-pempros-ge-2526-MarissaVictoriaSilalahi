#include <stdio.h>
#include <string.h>
#include <stdlib.h>

// Definisi konstanta
#define MAX_ITEMS 100
#define MAX_LOGS 500

// Struktur untuk menyimpan data barang
typedef struct {
    char id[10];
    char name[50];
    int quantity;
    double price;
} Item;

// Struktur untuk menyimpan catatan audit
typedef struct {
    int log_id;
    char item_id[10];
    char type[10];
    int quantity;
} AuditLog;

// Variabel Global
Item inventory[MAX_ITEMS];
AuditLog logs[MAX_LOGS];
int item_count = 0;
int log_count = 0;
int global_log_id = 1;

// Fungsi untuk mencari indeks barang berdasarkan ID
int find_item(char *id) {
    for (int i = 0; i < item_count; i++) {
        if (strcmp(inventory[i].id, id) == 0) {
            return i;
        }
    }
    return -1;
}

// Fungsi untuk mencatat aktivitas ke audit log
void add_log(char *id, char *type, int qty) {
    logs[log_count].log_id = global_log_id++;
    strcpy(logs[log_count].item_id, id);
    strcpy(logs[log_count].type, type);
    logs[log_count].quantity = qty;
    log_count++;
}

int main() {
    char *input[256];
    char line[200];

    while (fgets(line, sizeof(line), stdin)) {
        // Hilangkan newline
        line[strcspn(line, "\n")] = 0;

        // Stop jika ---
        if (strcmp(line, "---") == 0) break;

        char *cmd = strtok(line, "#");
        if (cmd == NULL) continue;

        // ================= RECEIVE =================
        if (strcmp(cmd, "receive") == 0) {
            char *id = strtok(NULL, "#");
            char *name = strtok(NULL, "#");
            int qty = atoi(strtok(NULL, "#"));
            double price = atof(strtok(NULL, "#"));

            strcpy(inventory[item_count].id, id);
            strcpy(inventory[item_count].name, name);
            inventory[item_count].quantity = qty;
            inventory[item_count].price = price;

            add_log(id, "receive", qty);
            item_count++;
        }

        // ================= SHIP =================
        else if (strcmp(cmd, "ship") == 0) {
            char *id = strtok(NULL, "#");
            int qty = atoi(strtok(NULL, "#"));
            int idx = find_item(id);

            if (idx != -1 && qty > 0 && inventory[idx].quantity >= qty) {
                inventory[idx].quantity -= qty;
                add_log(id, "ship", qty);
            }
        }

        // ================= RESTOCK =================
        else if (strcmp(cmd, "restock") == 0) {
            char *id = strtok(NULL, "#");
            int qty = atoi(strtok(NULL, "#"));
            int idx = find_item(id);

            if (idx != -1 && qty > 0) {
                inventory[idx].quantity += qty;
                add_log(id, "restock", qty);
            }
        }

        // ================= REPORT =================
        else if (strcmp(cmd, "report") == 0) {
            for (int i = 0; i < item_count; i++) {
                printf("%s|%s|%d|%.1f\n",
                    inventory[i].id,
                    inventory[i].name,
                    inventory[i].quantity,
                    inventory[i].price);
            }
        }

        // ================= AUDIT =================
        else if (strcmp(cmd, "audit") == 0) {
            char *id = strtok(NULL, "#");
            int idx = find_item(id);

            if (idx != -1) {
                printf("%s|%s|%d|%.1f\n",
                    inventory[idx].id,
                    inventory[idx].name,
                    inventory[idx].quantity,
                    inventory[idx].price);

                for (int i = 0; i < log_count; i++) {
                    if (strcmp(logs[i].item_id, id) == 0) {
                        printf("%d|%s|%d\n",
                            logs[i].log_id,
                            logs[i].type,
                            logs[i].quantity);
                    }
                }
            }
        }
    }

    return 0;
}