#include <stdio.h>
#include <stdlib.h>
#include <time.h>
int n;

int partition(int *a,int start,int end){ //verilen diziyi baslangic elemanindan buyuk olanlar ve kucuk olanlar seklinde ayýran fonksiyon
	int i,j,tmp,pivot;
	i=start;
	j=end;
	pivot=a[start];
	while(i < j){
		if(a[i] <= pivot)
			i++;
		else{
			if(a[j] > pivot)
				j--;
			else{
				tmp=a[j];
				a[j]=a[i];
				a[i]=tmp;
				j--;
			}
		}
	}
	if(pivot>a[j]){
		a[start]=a[j];
		a[j]=pivot;
		return j;		
	}
	a[start]=a[j-1];
	a[j-1]=pivot;
	return j-1;
}


int indisBul(int *a,int x,int start,int end){ //verilen dizi eger x degerini iceriyorsa o degerin indisini donduren fonksiyon
	int i;
	for(i=start;i<end+1;i++){
		if(a[i]==x)
			return i;
	}
}

void yazdir(int *a){//verilen diziyi yazdýran fonksiyon (n elemanli olmasý gerekli)
	int i;
	for(i=0;i<n;i++){
		if(i==n-1)
			printf("%d",a[i]);
		else
			printf("%d-",a[i]);
	}
	printf("\n");
}

void sorting(int *lock,int *key,int start,int end){
	int i,x,a,tmp;
	srand(time(NULL));
	if(start < end){	
		int rastgele=(rand() % (end-start+1))+start;								//rastgele indis uretimi
		a=indisBul(lock,key[rastgele],start,end);									//rastgele indisdeki degerin lock dizisinde nerede oldugu		
		printf("\nRastgele pivot %d icin Lock ve Key dizisi:\n",key[rastgele]);	
		tmp=lock[a];
		lock[a]=lock[start];														//rastgele indisdeki degerin basa alinmasi
		lock[start]=tmp;
		
		x=partition(lock,start,end);
		printf("Lock:");
		yazdir(lock);
	
		tmp=key[rastgele];
		key[rastgele]=key[start];													//rastgele indisdeki degerin basa alinmasi
		key[start]=tmp;	
	
		i=partition(key,start,end);
		printf("Key :");
		yazdir(key);
		
		sorting(lock,key,start,x-1);
		sorting(lock,key,x+1,end);		
	}	
}

int main(int argc, char *argv[]) {
	int *lock,*key,i,sayi,randi,randj;
	int menu;
	printf("Yapilacak islemi seciniz.\n1)Dizinin elemanlarini tek tek girme\n2)Hazir Lock={6,3,2,5,1,4} ve Key={2,4,1,3,6,5} dizilerini kullanma\n3)Dizinin elemanlarini random uretme\n");
	scanf("%d",&menu);
	
	switch(menu){
		case 1:
			printf("Eleman sayisini giriniz:");
			scanf("%d",&n);
			lock=(int*)calloc(n,sizeof(int));	
			key=(int*)calloc(n,sizeof(int));			
			for(i=0;i<n;i++){
				printf("\nLock[%d]:",i);
				scanf("%d",&lock[i]);								
			}
			for(i=0;i<n;i++){
				printf("\nKey[%d]:",i);
				scanf("%d",&key[i]);
			}
			sorting(lock,key,0,n-1);
		break;
		
		case 2:
			n=6;
			lock=(int*)calloc(6,sizeof(int));	
			key=(int*)calloc(6,sizeof(int));
			lock[0]=6; lock[1]=3; lock[2]=2; lock[3]=5; lock[4]=1; lock[5]=4;
			key[0]=2; 	key[1]=4; 	key[2]=1; 	key[3]=3; 	key[4]=6; 	key[5]=5;
			sorting(lock,key,0,5);
		break;
		
		case 3:
		srand(time(NULL));
		printf("Elemanlar rastgele olusturulacak eleman sayisini giriniz:");
		scanf("%d",&n);	
		lock=(int*)calloc(n,sizeof(int));	
		key=(int*)calloc(n,sizeof(int));
		for(i=0;i<n;i++){													
			sayi=rand() % 101;
			randi=rand() % n;
			randj=rand() % n;
			while(key[randi]!=0)
				randi=rand() % n;
			while(lock[randj]!=0)
				randj=rand() % n;
			key[randi]=sayi;
			lock[randj]=sayi;
		}
		sorting(lock,key,0,n-1);
		break;
		
		default:
			printf("Gecersiz numara girdiniz.");
	}

	return 0;
}
