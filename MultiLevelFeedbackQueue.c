#include "stdio.h"
#include "windows.h"

// TODO: sap xep tien trinh sau khi promote, demote; reset time quantum khi promote, demote

struct hangdoi {
	char tentt[20];
	int tgden;
	int tgchay;
	int tgconlai;
	int hangcho; // Hang cho 1 > 2 > 3.
	int tgcho;
	int tgcho1; // tg cho tai hang cho hien tai
	int thutu;
	int tghoanthanh;
} tientrinh[20];
int timequantumin[3] = {0, 2, 4};
int hang[4] = {0, 0, 0, 0};
int so, tongtg = 0, tongtgcho = 0, timequantum, change = 0, hangtruoc = 0;

void help() {
	printf ("Chuong trinh mo phong hang cho hoi tiep da muc.\n");
	printf ("Chuong trinh nay mo phong hang doi hoi tiep da muc voi 3 hang cho.\n");
	printf ("Hang cho thu 1 theo thuat toan FCFS co time quantum = 2s.\n");
	printf ("Hang cho thu 2 theo thuat toan FCFS co time quantum = 4s.\n");
	printf ("Hang cho thu 3 theo thuat toan FCFS.\n");
	printf ("Tien trinh chay xong 1 lan o hang cho thu 1 hoac thu 2 neu van chua xong thi se bi chuyen xuong hang cho thap hon.\n");
	printf ("Tien trinh co thoi gian cho tai mot hang lon hon 10s se duoc chuyen len hang co do uu tien cao hon.\n");
	printf ("Chuong trinh ho tro toi da 20 tien trinh.\n");
	printf ("Cac tien trinh duoc coi la khong yeu cau vao ra.\n");
	printf ("Chuong trinh se cap nhat du lieu sau moi 1s.\n");
	printf ("Hay nhap cac tien trinh theo thu tu thoi gian den. Nhap phim bat ki de bat dau nhap.\n"); // Viet them function sap xep neu du tg
	getchar();
}

void input (int a) {
	printf ("Ten tien trinh thu %d:", a+1);
	fflush (stdin);
	gets (tientrinh[a].tentt);
	printf ("Thoi gian den:");
	scanf ("%d", &tientrinh[a].tgden);
	printf ("Thoi gian chay:");
	scanf ("%d", &tientrinh[a].tgchay);
	tientrinh[a].tgconlai = tientrinh[a].tgchay;
	tientrinh[a].hangcho = 0;
	tientrinh[a].thutu = 0;
	tientrinh[a].tgcho = 0;
	tientrinh[a].tgcho1 = 0;
}

void table() {
	system ("cls");
	printf ("+--------------------+-------+-------+--------+------+-------+------------------------+--------------------+\n");
	printf ("|Ten tien trinh      |Tg den |Tg chay|Hang cho|Vi tri|Tg cho |Tg cho tai hang hien tai|Thoi diem hoan thanh|\n");
	printf ("+--------------------+-------+-------+--------+------+-------+------------------------+--------------------+\n");
	for (int i = 0; i < so; i += 1)
	{
		printf ("|%-20s|%-7d|%-7d|%-8d|%-6d|%-7d|%-24d|%-20d|\n", tientrinh[i].tentt, tientrinh[i].tgden, tientrinh[i].tgchay, tientrinh[i].hangcho, tientrinh[i].thutu, tientrinh[i].tgcho, tientrinh[i].tgcho1, tientrinh[i].tghoanthanh);
	}
	printf ("+--------------------+-------+-------+--------+------+-------+------------------------+--------------------+\n");
	printf ("Tong thoi gian: %d. Tong thoi gian cho cua tat ca tien trinh: %d.\n", tongtg, tongtgcho);
	Sleep (500);
}

// void swap (int i, int j) {
// 	temp = tientrinh[i];
// 	tientrinh[i] = tientrinh[j];
// 	tientrinh[j] = temp;
// }

void changetimequantum() {
	if (hang[1] != 0) {
		timequantum = timequantumin[1];
	}
	if (hang[2] != 0) {
		timequantum = timequantumin[2];
	}
}

void demote (int b) {
	if (tientrinh[b].hangcho < 3) {
		for (int i = 0; i < so; i += 1) {
			if (tientrinh[i].hangcho == tientrinh[b].hangcho) {
				tientrinh[i].thutu -= 1;
			}
		}
		int temp = tientrinh[b].hangcho;
		hang[temp] -= 1;
		tientrinh[b].hangcho += 1;
		tientrinh[b].tgcho1 = 0;
		temp = tientrinh[b].hangcho;
		tientrinh[b].thutu = hang[temp];
		hang[temp] += 1;
		changetimequantum();
		// printf ("Tien trinh %s duoc chuyen xuong hang cho thu %d.\n", tientrinh[b].tentt, tientrinh[b].hangcho);
	} else {
		// printf ("Tien trinh %s da o hang cho thu 3.\n");
	}
}

void promote (int c) {
	if (tientrinh[c].hangcho > 1) {
		for (int i = 0; i < so; i += 1) {
			if (tientrinh[i].hangcho == tientrinh[c].hangcho) {
				tientrinh[i].thutu -= 1;
			}
		}
		int temp = tientrinh[c].hangcho;
		hang[temp] -= 1;
		tientrinh[c].hangcho -= 1;
		tientrinh[c].tgcho1 = 0;
		temp = tientrinh[c].hangcho;
		tientrinh[c].thutu = hang[temp];
		hang[temp] += 1;
		changetimequantum();
		// printf ("Tien trinh %s duoc chuyen len hang cho thu %d.\n", tientrinh[c].tentt, tientrinh[c].hangcho);
	} else {
		// printf ("Tien trinh %s da o hang cho thu 1.\n");
	}
}

void markasdone (int d) {
	int temp = tientrinh[d].hangcho;
	hang[temp] -= 1;
	for (int j = 0; j < so; j += 1) {
		if (tientrinh[j].hangcho == tientrinh[d].hangcho) {
			tientrinh[j].thutu -= 1;
		}
	}
	tientrinh[d].hangcho = 0;
	tientrinh[d].thutu = 0;
	tientrinh[d].tgcho1 = 0;
	tientrinh[d].tghoanthanh = tongtg;
}

void addprocess () {
	for (int i = 0; i < so; i += 1) {
		if (tientrinh[i].tgden == tongtg) {
			tientrinh[i].hangcho = 1;
			tientrinh[i].thutu = hang[1];
			if (hang[1] == 0) {
				timequantum = timequantumin[1];
			}
			hang[1] += 1;
		}
	}
}

void run_1 (int e) {
	tongtg += 1;
	if (change == 1) {
		timequantum = timequantumin[e];
		change = 0;
	}
	int lan = 0;
	for (int j = 0; j < so; j += 1) {
		if ((tientrinh[j].hangcho == e) && (tientrinh[j].thutu == 0) && (lan != 1)) {
			tientrinh[j].tgconlai -= 1;
			timequantum -= 1;
			if (tientrinh[j].tgconlai == 0) {
				change = 1;
				markasdone (j);
			}
			if ((tientrinh[j].tgconlai != 0) && (timequantum == 0) && (e != 3)) {
				change = 1;
				demote (j);
			}
			lan = 1;
			continue;
		}
		if (tientrinh[j].hangcho != 0) {
			tientrinh[j].tgcho += 1;
			tientrinh[j].tgcho1 += 1;
			if (tientrinh[j].tgcho1 == 10) {
				promote (j);
				change = 1;
			}
			tongtgcho += 1;
		}
	}
}

void run() {
	if (hang[1] > 0) {
		run_1(1);
		return;
	}
	if (hang[2] > 0) {
		run_1(2);
		return;
	}
	run_1(3);
}

int main() {
	help();
	do {
		printf ("So tien trinh: ");
		scanf ("%d", &so);
	} while ((so < 1) || (so > 20));
	for (int i = 0; i < so; i += 1) {
		input (i);
	}
	while (1) {
		addprocess();
		table ();
		for (int i = 0; i < so; i += 1) {	
			if (tientrinh[i].tgconlai != 0) {
				break;
			}
			if (i == so - 1) {
				table();
				printf ("Chuong trinh chay xong trong thoi gian la: %d va tong thoi gian cho cua tat ca tien trinh la: %d", tongtg, tongtgcho);
				getchar();
				return 0;
			}
		}
		run();
	}
	return 0;
}
