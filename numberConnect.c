#include<stdio.h>
#include <time.h>
#include <stdlib.h>
#define MAX 15   // kullanýlacak max sayi

void readFromFile(int matris[][MAX], char *fileName);
void reset(int matris[][MAX], int N);
void fillMtr(int matris[][MAX], int *N);
void drawBoard(int matris[][MAX], int N);
int name(char user[][20], int *k);
int mainMenu();
int gameMenu();
int option();
int getPoint(int matris[][MAX], int N, int *k, int *x1, int *y1, int *x2, int *y2, int top, int move[][7]);
void push(int x1, int y1, int x2, int y2, int *top, int move[][7], int dogrultu, int k);
void undo(int *top, int matris[][MAX], int move[][7]);
int control(int matris[][MAX], int N);
void manuel(int matris[][MAX], int move[][7], int N, int *numUndo, int label[][MAX], int c[][MAX], int index[][4]);
void findIndex(int matris[][MAX], int index[][4], int N);
void copyForLabeling(int matris[][MAX], int label[][MAX], int N, int a);
void dfs(int a, int x, int y, int current_label,int c[][MAX], int N,int label[][MAX]);
int chokepoint(int N, int label[][MAX], int matris[][MAX], int c[][MAX], int index[][4]);
void automatic(int a, int i, int j, int N, int matris[][MAX], int index[][4], int move[][7], int *top, int label[][MAX], int c[][MAX]);
void scoreFunc(int choice, int choice2, int N, int k, int score[], int numUndo);
void printScore(int score[], char user[][20], int k);



int main(){
	srand(time(NULL));
    int i,j,matris[MAX][MAX]={{0}}, label[MAX][MAX], c[MAX][MAX], m, choice, choice2, k=-1, move[300][7], top=-1, dogrultu, index[MAX][4], a=1, man, oto, score[100]={0}, numUndo; 
    int N; 
    char fileName[20], user[100][20];
	
	printf("\nSAYI ESLESTIRME\n");
	printf("\nManuel Oyun Kurallari:");
	printf("\n1)Baslangic noktanizda ilerletmek istediginiz sayý bulunmalidir!!");
	printf("\n2)Noktayi her adimda saga, sola, asagiya ya da yukariya bir dogru boyunca ilerletmelisiniz!!");
	printf("\n3)Ilerleme boyunca baska bir sayi bulunmamali!!");
		
	  	do{
	  		choice = mainMenu();
	  		
	  		if(choice != 4 && choice != 3){
	  			k = name(user, &k); //kullanýcý adý
	  		}
	  		if(choice == 1){
			  	
			  	do{
				  	choice2 = gameMenu();
				  	
				  	if(choice2 != 3){
				  		fillMtr(matris, &N);	
				  		drawBoard(matris, N);
				  	}
				  	
					if(choice2 == 1){ //manuel mod	
				  		manuel(matris, move, N, &numUndo, label, c, index); 
				  	}
				  			
				  	if(choice2 == 2){//otomatik mod
				  		findIndex(matris, index, N);
				  		i = index[0][0];
				  		j = index[0][1];
				  		top = -1;
				  		a = 1;
				  		automatic(a, i, j, N, matris, index, move, &top, label, c);
				  		if(control(matris, N) && !chokepoint(N, label, matris, c, index)){
				  			printf("\n");
				  			drawBoard(matris, N);	
						}
						else{
							printf("\n!!Bu matrisin cozumu yok!!\n");
						}
					  }
		  			scoreFunc(choice, choice2, N, k, score, numUndo);
	  			}while(choice2 != 3);
	  		}
	  			
	  		if(choice == 2){ //dosyadan oku
			  	
			  	do{
				  	choice2 = gameMenu();
				  	
				  	if(choice2 != 3){
					    printf("\nMatrisin boyutu: ");
						scanf("%d", &N);
						reset(matris, N);
						printf("\nDosya Adini Giriniz: ");
	  					scanf("%s",fileName);
	  					readFromFile(matris, fileName);
	    				drawBoard(matris,N);
    				}
    				
					if(choice2 == 1){ //manuel mod				  		  					
				  		manuel(matris, move, N, &numUndo, label, c, index);
				  	}
				  			
				  	if(choice2 == 2){//otomatik mod
				    	findIndex(matris, index, N);
				  		i = index[0][0];
				  		j = index[0][1];
				  		top = -1;
				  		a = 1;
				  		automatic(a, i, j, N, matris, index, move, &top, label, c);
				  		if(control(matris, N) && !chokepoint(N, label, matris, c, index)){
				  			printf("\n");
				  			drawBoard(matris, N);	
						}
						else{
							printf("\n!!Bu matrisin cozumu yok!!\n");
						}		
		  			}
		  			scoreFunc(choice, choice2, N, k, score, numUndo);
	  			}while(choice2 != 3);
			}
			
			if(choice == 3){ //skorlarý yazdýrma
	  			printScore(score, user, k);
	  		}	
		}while(choice != 4);
    return 0;
}

void readFromFile(int matris[][MAX], char *fileName){
	int i,j, temp;
	FILE *data = fopen(fileName,"r");
	if(!data){
        printf("Dosya Acilamadi!");
		return;
    }
    while(!feof(data)){
        fscanf(data,"%d %d %d\n",&i,&j,&temp);  
		matris[i][j]=temp; 
    }  
  	fclose(data); 

}


void reset(int matris[][MAX], int N){
	int i, j;
	
	for(i=0 ; i<N ; i++){
		for(j=0 ; j<N ; j++){
			matris[i][j] = 0;
		}
	}
} 

void fillMtr(int matris[][MAX], int *N){
	int i, j, k, m, tempx, tempy, flag=0;
	
	printf("\nMatris boyutu: ");
	scanf(" %d", N);
	
	k = *N;
	reset(matris, k); //matrisi sýfýrlama
	
	for(k=1 ; k<=*N ; k++){//matrisi rastgele sayilarla doldurma
		tempx = -2; //-2 vermemin nedeni randomda 0 çýkýnca -1 ile karþýlaþtýracak olmasý
		tempy = -2;
		for(m=0 ; m<2 ; m++){ //ayný sayýdan 2 kere eklmememiz lazým
			do{
				i = rand()% *N;
				j = rand()% *N;
				if(i-1 == tempx || i+1 == tempx || j-1 == tempy || j+1 == tempy){ //diðer sayýnýn çevresinde olup olmadýðýnýn kontrolü
					flag = 1;
				}
				else{
					flag = 0;
				}
			}while(matris[i][j] != 0 || flag == 1);
			matris[i][j] = k;
			tempx = i;
			tempy = j;
		}
	}	
}

void drawBoard(int matris[][MAX], int N){
    int i,j,k;
    for(i=0;i<N;i++){
        printf("\n");
        for (k=0;k<N;k++)
            printf("-------");
        printf("\n");
        for(j=0;j<N;j++)
            if (matris[i][j]!=0)
                printf("  %d   |",matris[i][j]);
            else
                 printf("      |",matris[i][j]);
    }
}


int name(char user[][20], int *k){
	int i, c;
	if(*k == -1){
		printf("\n1. oyuncu");
		printf("\nAdinizi giriniz: ");
		scanf("%s", user[0]);
		*k += 1;
	}
	else{
		for(i=0;i<=*k;i++){
			printf("%d) %s", i+1, user[i]);
		}
		printf("\nEger daha once oynadiysaniz numaranizi girin. Oynamadýysanýz 0'a basin: ");
		scanf("%d", &c);
		if(c == 0){
			*k += 1;
			printf("\n%d. oyuncu", *k+1);
			printf("\nAdinizi giriniz: ");
			scanf("%s", &user[*k][0]);
		}
		else{
			return c; //skor için oyuncunun numarasý
		}
	}
	return *k;
}

int mainMenu(){
	int choice;
	do{
		printf("\n\nYapmak istediginiz islemi seciniz:");
		printf("\n1)Rastgele Matris Olustur");
		printf("\n2)Dosyadan Matris Olustur");
		printf("\n3)Kullanicilarin Skorlarini Goster");
		printf("\n4)Cikis");
		printf("\nSecim: ");
		scanf("%d", &choice);
	}while(choice != 1 && choice != 2 && choice != 3 && choice != 4);
	
	return choice;
}

int gameMenu(){
	int choice;
	
	do{
		printf("\n\nYapilacak islemi seciniz:");
		printf("\n1)Manuel Modda Oyna");
		printf("\n2)Otomatik Modda Oyna");
		printf("\n3)Ana Menuye Don");
		printf("\nSecim: ");
		scanf("%d", &choice);	
	}while(choice != 1 && choice != 2 && choice != 3);
	
	return choice;
}

int option(){ //oynarken yapacaðý iþlem
	int choice;
	do{
		printf("\n\nYapilacak islemi seciniz:");
		printf("\n1)Hamle yap");
		printf("\n2)Hamleyi geri al");
		printf("\n3)Pes et ve oyun menusune don"); //oyun çözümsüz çýkarsa ya da devam etmek istemezse
		printf("\nSecim: ");
		scanf("%d", &choice);
	}while(choice != 1 && choice != 2 && choice != 3);
	return choice;
}


int getPoint(int matris[][MAX], int N, int *k, int *x1, int *y1, int *x2, int *y2, int top, int move[][7]){ 
	int i, j, flag=0, flag2=0, dogrultu;
	
	do{
	
		printf("\n\nBaslangic noktasi (x,y): ");
		scanf("%d %d", x1, y1);
		printf("\nBitis noktasi (x,y): ");
		scanf("%d %d", x2, y2);
		
		if(*x1 != *x2 && *y1 != *y2){ // doðru boyunca hareket ettirebilmek adýna x veya y'lerden biri ayný olmalý
			printf("\n!!Noktayi sadece bir dogru boyunca hareket ettirebilirsiniz!!\n");
			flag = 1;
		}
		else if(*x1 == *x2 && *y1 == *y2){
			printf("\nNoktalar birbirinden farkli olmali");
			flag = 1;
		}
		else if(*x1>=N || *x2>=N || *y1>=N || *y2>=N || *x1<0 || *x2<0 || *y1<0 || *y2<0){ //matrisin dýþýna çýkma
			printf("\n!!Matrisin sinirlari disina cikmamalisiniz!!\n");
			flag = 1;
		}
				
		else{ // ilerleme yolunda baþka bir sayýnýn olup olmadýðýnýn kontrolü
			flag = 0;
			if(*x1 == *x2){ //x'in mi yoksa y'nin mi doðrultusunda hareket var? i'ler aynýysa x ekseninde hareket 
				dogrultu = 1;
				if(*y1 < *y2){
					*k = 1; //saða ilerleme 
				}
				
				else{
					*k = -1; //sola ilerleme 
				}
				
				i = *x1;
				j = *y1 + *k;
			}
			else{ //i'ler ayný deðilse y ekseninde
				dogrultu = 0;
				if(*x1 < *x2){
					*k = 1; //aþaðý ilerleme
				}
				
				else{
					*k = -1; //yukarý ilerleme
				}
				
				i = *x1 + *k;
				j = *y1;	
			}
			
			while(matris[i][j] == 0 && flag2 == 0){
				if(dogrultu){ //dogrultu=1 ise x üzerinde hareket
					if(j == *y2){
						flag2 = 1; //y2'yi geçmesini engellemek için
					}
					
					else{
						j += *k;
					}
				}
				
				else{ // dogrultu=0'dýr. x üzerinde hareket
					if(i == *x2){
						flag2 = 1; //x2'yi geçmesini engellemek için
					}
					
					else{
						i += *k;
					}
				}	
			}
			if(flag2 == 1 && (matris[*x2][*y2] == matris[*x1][*y1] || matris[*x2][*y2] == 0)){ //bu adýmda sayýlar eþleþmiþ ya da son adýmdaki sayý 0 olmuþ olacak ve hamle yapýlabilecek
				flag2 = 0;
			}
			
			if(flag2 == 1){ //doðrultu üzerinde baþka bir sayý var
				printf("\n!!Gidecegi yol uzerinde baska bir sayi bulunmamali!!\n");
			}		
		}
	}while(flag == 1 || flag2 == 1);
	
	return dogrultu; //manuel fonksiyonunda kullanabilmek için	
}

void push(int x1, int y1, int x2, int y2, int *top, int move[][7], int dogrultu, int k){
	int i;
	
	*top = *top + 1;
	move[*top][0] = x1;
	move[*top][1] = y1;
	move[*top][2] = x2;
	move[*top][3] = y2;	
	move[*top][4] = dogrultu;
	move[*top][5] = k;
	move[*top][6] = 0; //eþleþtirilirse sonrasýnda deðiþtireceðim

}

void undo(int *top, int matris[][MAX], int move[][7]){
	
	if(*top != -1){
		if(move[*top][4]){ //doðrultu x 
			if(move[*top][6] == 1){ //o hamlede sayý eþleþtirilmiþtir yani o yerde bulunan sayý baþta rastgele daðýtýlan sayýdýr. silinemez
				move[*top][3] -= move[*top][5]; //bir önce bulunduðu yer
			}
			while(move[*top][3] != move[*top][1]){
				matris[move[*top][2]][move[*top][3]] = 0;
				move[*top][3] -= move[*top][5];
			}
		}
		
		else{ //doðrultu y
			if(move[*top][6] == 1){ //o hamlede sayý eþleþtirilmiþtir yani o yerde bulunan sayý baþta rastgele daðýtýlan sayýdýr. silinemez
				move[*top][2] -= move[*top][5]; //bir önce bulunduðu yer
			}
			while(move[*top][2] != move[*top][0]){  
				matris[move[*top][2]][move[*top][3]] = 0;
				move[*top][2] -= move[*top][5];
			}
		}
		
		*top = *top - 1; //move'u deðiþtirmeye gerek duymadým zaten bir sonraki hamlede üstüne yazýlacak
	}
	else{
		printf("\n!!Geri alinacak hamle yok!!\n");
	}
}

int control(int matris[][MAX], int N){ //matrisin dolup dolmadýðýnýn kontrolü. 1 ise dolmuþtur
	int i=0, j=0;
	
	while (i < N) {
        j = 0;
        while (j < N) {
            if (matris[i][j] == 0){
                return 0; 
            }
            j++;
        }
        i++;
    }
    return 1;
}

void manuel(int matris[][MAX], int move[][7], int N, int *numUndo, int label[][MAX], int c[][MAX], int index[][4]){ 
	int top=-1, i, j, x1, y1, x2, y2, k, dogrultu, selection;
	
	*numUndo = 0; //skor için gerekli
	do{
		selection = option(); //hamle yap, geri al, pes et
		if(selection == 1){ //hamle yap
			dogrultu = getPoint(matris, N, &k, &x1, &y1, &x2, &y2, top, move);
			push(x1, y1, x2, y2, &top, move, dogrultu, k);
			
			i = x1;
			j = y1;
			
			if(matris[i][j] == matris[x2][y2]){
				move[top][6] = 1;
				printf("\nSayilar eslestirildi!");
			}
			
			if(dogrultu){ //y üzerinde hareket
				do{
					matris[x1][j] = matris[x1][y1];
					j += move[top][5];
				}while(j != y2);
				matris[x1][j] = matris[x1][y1];	
			}
			
			else{
				do{
					matris[i][y1] = matris[x1][y1];
					i += move[top][5];
				}while(i != x2);
				matris[i][y1] = matris[x1][y1];
			}
			drawBoard(matris, N);
			
		}
		
		else if(selection == 2){ //geri al
			*numUndo += 1;
			undo(&top, matris, move);
			drawBoard(matris, N);
		}
		
		if(control(matris, N) && !chokepoint(N, label, matris, c, index)){
			printf("\n\nTEBRIKLER KAZANDINIZ.");
			selection = 3;	
		}
	
	}while(selection != 3);	
}

void findIndex(int matris[][MAX], int index[][4], int N){
	int i=0, j=0, k=0;
	
	for(i=0 ; i<N ; i++){ //index'in elemanlarýný -1' eþitliyorum böylelikle elemanlarýn indislerini kopyalayýp kopyalamadýðým belli olacak
		for(j=0 ; j<4 ; j++){
			index[i][j] = -1;
		}
	}
	
	i = 0;
	while (i < N && k != 2*N) { //k == 2*N olursa bütün sayýlar bulunmuþ demektir
        j = 0;
        while (j < N && k != 2*N) {
            if (matris[i][j] != 0){
            	if(index[matris[i][j]-1][0] == -1){ //o sayýnýn ilki bulundu //-1 
                	index[matris[i][j]-1][0] = i; //x deðeri 
					index[matris[i][j]-1][1] = j; //y deðeri
            	}
            	
            	else{ //o sayýnýn ikincisi 
            		index[matris[i][j]-1][2] = i; //x deðeri 
					index[matris[i][j]-1][3] = j; //y deðeri
				}
				k++;
			}
            j++;
        }
        i++;
    }
    
}


void copyForLabeling(int matris[][MAX], int c[][MAX], int N, int a){
	int i, j;
	reset(c, N);
	for(i=0 ; i<N ; i++){
		for(j=0 ; j<N ; j++){
			if(matris[i][j] == 0 || matris[i][j] == a){ //0 olan kýsýmlarý baðlý olup olmadýðýna bakmak istediðim sayýya eþitliyorum
				c[i][j] = a;
			}
		}
	}
}


void dfs(int a, int x, int y, int current_label,int c[][MAX], int N,int label[][MAX]){
	int direction;
	// direction vectors
	int dx[] = {+1, 0, -1, 0}; 
	int dy[] = {0, +1,  0, -1}; 
	//  	down, right, up, left

  	if (x < 0 || x == N) 
		return; // out of bounds
  	if (y < 0 || y == N) 
		return; // out of bounds
  	if (label[x][y] || c[x][y] != a) 
		return; // already labeled or not marked with a in m

	// mark the current cell
	label[x][y] = current_label;
	// recursively mark the neighbors
	dfs(a, x+1, y, current_label,c,N,label);
	dfs(a, x, y+1, current_label,c,N,label);
	dfs(a, x-1, y, current_label,c,N,label);
	dfs(a, x, y-1, current_label,c,N,label);
}

int chokepoint(int N, int label[][MAX], int matris[][MAX], int c[][MAX], int index[][4]){ //o anki matrisin durumunda diðer sayýlarýn birbirine ulaþýp ulaþamayacaðýný kontrol eder 
	int i, j, k=0, flag=0, a=1, component=1;
	
	while(flag == 0 && a <= N){
		component = 1;
		copyForLabeling(matris, c, N, a);
		reset(label, N);
		for (i = 0; i < N; i++){
    		for (j = 0; j < N; j++){
      			// if not labelled and c(i,j)=a
				if (!label[i][j] && c[i][j] == a) 
	  				dfs(a, i, j, component++, c, N, label);
			}
		}
		
		if(label[index[a-1][0]][index[a-1][1]] != label[index[a-1][2]][index[a-1][3]]){ // a sayýlarý iþaretlenmiþtir ve a sayýlarý matrisin o anki haliyle birleþtirilemez
			flag = 1;

		}
			
		else{
			a++;
		}
	}
	
	if(flag == 0)
		return 0;
	else
		return 1;
}


void automatic(int a, int i, int j, int N, int matris[][MAX], int index[][4], int move[][7], int *top, int label[][MAX], int c[][MAX]){
	
	if(i < 0 || j < 0){
		*top -= 1;
		return;
	}
		
	if(i >= N || j >= N){
		*top -= 1;
		return;
	}
	
	if(control(matris, N)){
		return;
	}
	
	if(matris[i][j] != 0 && !((i == index[a-1][0] && j == index[a-1][1] && (*top == -1 || move[*top-1][6] == 1)) || (i == index[a-1][2] && j == index[a-1][3]))){ 
		*top -= 1;
		return;
	}
	
	matris[i][j] = a;

	*top = *top + 1;

	move[*top][6] = 0;
	
//	drawBoard(matris,N);
//	printf("\n");
	
	if(i == index[a-1][2] && j == index[a-1][3] && a <= N){
		move[*top][6] = 1; //eþleþtirildi
		a++;
		i = index[a-1][0];
		j = index[a-1][1];
		automatic(a, i, j, N, matris, index, move, top, label, c);
	}
	
	if(chokepoint(N, label, matris, c, index)){
		*top -= 1;
		undo(top, matris, move);
		return;
	}

	
	push(i, j, i-1, j, top, move, 0, -1);
	automatic(a, i-1, j, N, matris, index, move, top, label, c); //yukarý

	
	push(i, j, i+1, j, top, move, 0, 1);
	automatic(a, i+1, j, N, matris, index, move, top, label, c); //aþaðý

	
	push(i, j, i, j+1, top, move, 1, 1);
	automatic(a, i, j+1, N, matris, index, move, top, label, c); //sað

	
	push(i, j, i, j-1, top, move, 1, -1);
	automatic(a, i, j-1, N, matris, index, move, top, label, c); //sol
	
	if(control(matris, N) && !chokepoint(N, label, matris, c, index)){
		return;//çözüm
	}
	
	else{
		*top -= 1;
		undo(top, matris, move);
		return;	
	}
}


void scoreFunc(int choice, int choice2, int N, int k, int score[], int numUndo){
	int s=0;
	
	if(choice == 1){
		s += 10;	
	}
	
	else if(choice == 2){
		s += 5;
	}
	
	if(choice2 == 1){
		s += 15 - numUndo;
	}
	
	else if(choice2 == 2){
		s += 5;
	}
	
	s += 3 * N;

	score[k] = s;
	
	return ;
}


void printScore(int score[], char user[][20], int k){
	int i;
	
	if(k == -1){
		printf("\nListelenecek kullanýcý ve skoru yok!");
	}
	
	for(i=0 ; i<k ; i++){
		printf("\n%d) %s  --->  %d", i+1, user[i], score[i]);
	}
		
	return;
}


