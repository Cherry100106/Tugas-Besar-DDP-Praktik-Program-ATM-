#ifndef ATM_H
#define ATM_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    char username[50];
    char password[50];
    int saldo;
    int isBlocked;
    int pinAttempts; 
} Rekening;

void tampilkanMenuAwal();
void tarikTunai(Rekening *rek, int jumlah);
void deposit(Rekening *rek);
void transfer(Rekening *rekPengirim, Rekening *rekPenerima, int jumlah);
void gantiPIN(Rekening *rek, const char *pinBaru);
void cekSaldo(const Rekening *rek);
void pembayaran(Rekening *rek);
int cekIDPelanggan(const char *idPelanggan);
int autentikasi(Rekening *rek, const char *username, const char *password);
int autentikasiRekening(Rekening *rek, const char *username);
int updateFileRekening(const Rekening *rek);
void inputPassword(char *password, int maxLength);
void clearScreen();

#endif

