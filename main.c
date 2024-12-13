#include "atm.h"
#include <stdlib.h>

int main() {
    Rekening akunAktif, penerima;
    char username[50], password[50];
    int pilihan, jumlah;
    char pinBaru[50];
    char rekeningTujuan[50];
	
	system("color 1F");
	clearScreen();	
	printf("\033[44m\033[37m");
	printf("=============================================\n");
	printf(" SSSSS   EEEEE   DDDD    EEEEE   PPPPP\n");
    printf("S        E       D   D   E       P   P\n");
    printf(" SSSSS   EEEE    D   D   EEEE    PPPPP\n");
    printf("      S  E       D   D   E       P\n");
    printf(" SSSSS   EEEEE   DDDD    EEEEE   P\n");
    printf("=============================================\n");
    printf("Selamat datang di ATM!\n");
    printf("Masukkan username: ");
    scanf("%s", username);
    printf("Masukkan password: ");
    inputPassword(password, 50); 
    
    if (autentikasi(&akunAktif, username, password)) {
        printf("Login berhasil!\n");
        do {
        	clearScreen();
            tampilkanMenuAwal();
            printf("Pilih menu: ");
            scanf("%d", &pilihan);

            switch (pilihan) {
                case 1:
                	clearScreen();
                    cekSaldo(&akunAktif);
                    system("pause");
                    break;
                case 2:
                	clearScreen();
                	printf("Masukkan jumlah tarik tunai: ");
        			scanf("%d", &jumlah);
                    tarikTunai(&akunAktif, jumlah);
                    system("pause");
                    break;
                case 3:
                	clearScreen();
                    deposit(&akunAktif);
                    system("pause");
                    break;
                case 4:
                	clearScreen();
                    printf("Masukkan nomor rekening tujuan: ");
                    scanf("%s", rekeningTujuan);
                    printf("Masukkan jumlah transfer: ");
                    scanf("%d", &jumlah);
                    if (autentikasiRekening(&penerima, rekeningTujuan)) {
                        transfer(&akunAktif, &penerima, jumlah);
                    } else {
                        printf("Rekening tujuan tidak ditemukan! atau saldo anda tidak cukup!\n");
                    }
                    system("pause");
                    break;
                case 5:
                	clearScreen();
                    gantiPIN(&akunAktif, pinBaru);
                    system("pause");
                    break;
                case 6:
    				clearScreen();
  					pembayaran(&akunAktif);
    				system("pause");
    				break;
				 case 7:
                	clearScreen();
                    printf("Terima kasih telah menggunakan layanan kami!\n");
                    system("pause");
                    break;
                default:
                	clearScreen();
                    printf("Pilihan tidak valid!\n");
                    system("pause");
            }

        } while (pilihan != 6);
    } else {
    	clearScreen();
        printf("\033[41m\033[37m");
    	printf("Login gagal! Username atau password salah, atau akun diblokir.\n");
    	printf("\033[0m");
    	getchar();
    }
    printf("\033[0m");
    return 0;
}
