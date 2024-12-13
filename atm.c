#include "atm.h"
#include <stdio.h>
#include <string.h>
#include <windows.h>
#include <conio.h>


void tampilkanMenuAwal() {
    printf("\033[44m\033[37m");
    printf("===========================================\n"); 
    printf("           PILIH TRANSAKSI ANDA            \n");
    printf("       Tekan Cancel untuk membatalkan      \n"); 
    printf("===========================================\n");
    printf("  -->  1. CEKSALDO         5. UBAH PIN    <--\n");
    printf("  -->  2. PENARIKAN TUNAI  6. PEMBAYARAN  <--\n");
    printf("  -->  3. SETOR TUNAI      7. KELUAR      <--\n");
    printf("  -->  4. TRANSFER\n");
    printf("===========================================\n");
}



void tarikTunai(Rekening *rek, int jumlah) {
    if (rek->saldo >= jumlah) {
        int pecahan[3] = {100000, 50000, 20000};
        int jumlahPecahan[3] = {0};
        int sisa = jumlah;
        int pilihan;
        int i = 0;
        
        printf("Masukkan pecahan yang diinginkan (100000, 50000, 20000): ");
        scanf("%d", &pilihan);
        while (i < 3) {
            if (pecahan[i] == pilihan) {
                jumlahPecahan[i] = sisa / pecahan[i];
                sisa %= pecahan[i];
            }
            i++;
        }

        if (sisa == 0) {
            rek->saldo -= jumlah;
            printf("Tarik tunai berhasil! Sisa saldo Anda: %d\n", rek->saldo);
            printf("Rekap uang yang Anda terima:\n");

            i = 0;
            while (i < 3) {
                if (jumlahPecahan[i] > 0) {
                    printf("%d x %d\n", jumlahPecahan[i], pecahan[i]);
                }
                i++;
            }
            updateFileRekening(rek);
        } else {
            printf("Pecahan yang diminta tidak mencukupi jumlah tarik tunai.\n");
        }
    } else {
        printf("Saldo tidak cukup!\n");
    }
}

void deposit(Rekening *rek) {
    int pecahan[3] = {100000, 50000, 20000};
    int jumlahPecahan[3] = {0};
    int i = 0;
    int total = 0;
    char konfirmasi;    

    printf("Masukkan jumlah pecahan uang:\n");
    while (i < 3) {
        printf("Jumlah pecahan %d: ", pecahan[i]);
        scanf("%d", &jumlahPecahan[i]);
        i++;
    }

    i = 0;
    while (i < 3) {
        total += jumlahPecahan[i] * pecahan[i];
        i++;
    }

    printf("Rekap uang yang Anda masukkan:\n");
    i = 0;
    while (i < 3) {
        if (jumlahPecahan[i] > 0) {
            printf("%d x %d\n", jumlahPecahan[i], pecahan[i]);
        }
        i++;
    }

    printf("Konfirmasi deposit sebesar %d? (y/n): ", total);
    scanf(" %c", &konfirmasi);

    if (konfirmasi == 'y' || konfirmasi == 'Y') {
        rek->saldo += total;
        printf("Deposit berhasil! Saldo Anda sekarang: %d\n", rek->saldo);
        updateFileRekening(rek);
    } else {
        printf("Deposit dibatalkan.\n");
    }
}


void transfer(Rekening *rekPengirim, Rekening *rekPenerima, int jumlah) {
    if (rekPengirim->saldo >= jumlah) {
        rekPengirim->saldo -= jumlah;
        rekPenerima->saldo += jumlah;
        printf("Transfer berhasil sebesar %d ke rekening %s.\n", jumlah, rekPenerima->username);
        updateFileRekening(rekPengirim);
        updateFileRekening(rekPenerima);
    } else {
        printf("Saldo tidak mencukupi untuk transfer!\n");
    }
    return;
}

void gantiPIN(Rekening *rek, const char *pinBaru) {
    char pinLama[50];

    printf("Masukkan PIN lama: ");
    inputPassword(pinLama, 50);
    
    if (strcmp(rek->password, pinLama) != 0) {
        printf("PIN lama tidak sesuai! Gagal mengganti PIN.\n");
        return;
    }
    
    printf("Masukkan PIN baru: ");
    scanf("%s", rek->password);
    updateFileRekening(rek);
    printf("PIN berhasil diganti!\n");
}
void cekSaldo(const Rekening *rek) {
    printf("Saldo Anda saat ini: %d\n", rek->saldo);
}

int autentikasi(Rekening *rek, const char *username, const char *password) {
    FILE *file = fopen("daftarrekening.txt", "r");
    if (!file) {
        printf("File tidak ditemukan!\n");
        return 0;
    }

    while (fscanf(file, "%s %s %d %d %d", rek->username, rek->password, &rek->saldo, &rek->isBlocked, &rek->pinAttempts) != EOF) {
        if (strcmp(rek->username, username) == 0) {
            fclose(file);

            if (rek->isBlocked) {
                printf("\033[41m\033[37m");
                printf("Akun Anda telah diblokir. Silakan hubungi bank untuk bantuan lebih lanjut.\n");
                printf("\033[0m");
                getchar();
                return 0;
            }

            if (strcmp(rek->password, password) == 0) {
                rek->pinAttempts = 0;
                updateFileRekening(rek);
                return 1;
            } else {
                rek->pinAttempts++;
                printf("Password salah. Percobaan ke-%d dari 3.\n", rek->pinAttempts);
                getchar(); 

                if (rek->pinAttempts >= 3) {
                    rek->isBlocked = 1;
                    printf("\033[41m\033[37m");
                    printf("Akun Anda telah diblokir karena terlalu banyak percobaan login.\n");
                    printf("\033[0m");
                    getchar();
                }

                updateFileRekening(rek);
                return 0;
            }
        }
    }

    fclose(file);
    return 0;
}



int autentikasiRekening(Rekening *rek, const char *username) {
    FILE *file = fopen("daftarrekening.txt", "r");
    if (!file) {
        printf("File tidak ditemukan!\n");
        return 0;
    }
    while (fscanf(file, "%s %s %d", rek->username, rek->password, &rek->saldo) != EOF) {
        if (strcmp(rek->username, username) == 0) {
            fclose(file);
            return 1;
        }
    }
    fclose(file);
    return 0;
}

int updateFileRekening(const Rekening *rek) {
    FILE *file = fopen("daftarrekening.txt", "r");
    FILE *temp = fopen("temp.txt", "w");
    if (!file || !temp) {
        if (file) fclose(file);
        if (temp) fclose(temp);
        return 0;
    }

    Rekening tempRek;
    int updated = 0;

    while (fscanf(file, "%s %s %d %d %d", tempRek.username, tempRek.password, &tempRek.saldo, &tempRek.isBlocked, &tempRek.pinAttempts) != EOF) {
        if (strcmp(tempRek.username, rek->username) == 0) {
            fprintf(temp, "%s %s %d %d %d\n", rek->username, rek->password, rek->saldo, rek->isBlocked, rek->pinAttempts);
            updated = 1;
        } else {
            fprintf(temp, "%s %s %d %d %d\n", tempRek.username, tempRek.password, tempRek.saldo, tempRek.isBlocked, tempRek.pinAttempts);
        }
    }

    fclose(file);
    fclose(temp);

    if (updated) {
        remove("daftarrekening.txt");
        rename("temp.txt", "daftarrekening.txt");
    }

    return updated;
}


void clearScreen() {
    system("cls");
}

void pembayaran(Rekening *rek) {
    int jumlah;
    char kategori[50];
    char informasi[50];

    printf("===== Menu Pembayaran =====\n");
    printf("1. Listrik\n");
    printf("2. Air\n");
    printf("3. Pulsa\n");
    printf("===========================\n");
    printf("Pilih kategori pembayaran: ");
    int pilihan;
    scanf("%d", &pilihan);

    switch (pilihan) {
        case 1:
        	clearScreen();
            strcpy(kategori, "Listrik");
            printf("Masukkan ID pelanggan: ");
            break;
        case 2:
        	clearScreen();
            strcpy(kategori, "Air");
            printf("Masukkan ID pelanggan: ");
            break;
        case 3:
        	clearScreen();
            strcpy(kategori, "Pulsa");
            printf("Masukkan nomor telepon: ");
            break;
        default:
        	clearScreen();
            printf("Pilihan tidak valid!\n");
            return;
    }

    scanf("%s", informasi);

    if (!cekIDPelanggan(informasi)) {
    	clearScreen();
        printf("ID pelanggan tidak terdaftar!\n");
        return;
    }
	clearScreen();
    printf("Masukkan jumlah pembayaran: ");
    scanf("%d", &jumlah);

    if (rek->saldo >= jumlah) {
        rek->saldo -= jumlah;
        clearScreen();
        printf("Pembayaran %s dengan id %s sebesar %d berhasil.\n", kategori, informasi, jumlah);
        updateFileRekening(rek);
    } else {
        printf("Saldo tidak mencukupi untuk pembayaran!\n");
    }
}

int cekIDPelanggan(const char *idPelanggan) {
    FILE *file = fopen("idpelanggan.txt", "r");
    if (!file) {
        printf("Gagal membuka file daftar pelanggan!\n");
        return 0;
    }

    char id[50];
    int ditemukan = 0;
    
    while (fscanf(file, "%s", id) != EOF) {
        if (strcmp(id, idPelanggan) == 0) {
            ditemukan = 1;
            break;
        }
    }

    fclose(file);
    return ditemukan;
}

void inputPassword(char *password, int maxLength) {
    int i = 0;
    char ch;
    
    while ((ch = getch()) != '\r') {
        if (ch == '\b') {
            if (i > 0) {
                i--;
                printf("\b \b");
            }
        } else {
            password[i] = ch;
            i++;
            printf("*"); 
        }

        if (i >= maxLength) {
            break;
        }
    }
    password[i] = '\0';
    printf("\n");
}
