#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define SIZE 100
#define R 31

int dosyaUzunlugu(char *buff){
	FILE *fp=fopen(buff,"r");
	if(fp==NULL)
		return -1;
	char c;
	int i=0;
	while(!feof(fp)){
		fscanf(fp,"%c",&c);
		i++;
	}
	fclose(fp);
	return i-1;	
}

int asalmi(int n){
	if(n==0 || n==1)
		return 0;
	int flag=0,bolen=2;
	while(!flag && bolen < n){
		if(n % bolen == 0)
			flag=1;
		bolen++;
	}
	if(flag)
		return 0;
	return 1;
}

int kacsatir(char* name){
	FILE *fp=fopen(name,"r");
	if(fp==NULL)
		return -1;
	int n=0;
	char buff[100];
	while(!feof(fp)){
		fgets(buff,100,fp);
		n++;
	}
	fclose(fp);
	return n;
}

int us(int value,int n){
	if(n==0)
		return 1;
	int i,tmp;	
	tmp=value;
	for(i=1;i<n;i++)
		value=tmp*value;
	if(value < 0)
		value=value*-1;
	return value;	
}

int hash(int key,int M,char **HT){
	int h1,h,h2,i,MM;
	h1=key % M;
	h=h1;
	
	if(HT[h]==NULL)
		return h;
		
	i=0;	
	MM=M-1;
	h2=1+(key % MM);
		
	while((i<M) && HT[h]!=NULL ){
		h=(h1+i*h2) % M;
		i++;
	}
	
	if(HT[h]==NULL)
		return h;
	return -1;	
}

int hashSearch(char *eklenen,int M,FILE *fp,char **HT){
	int n,tmp;
	if(fp==NULL)
		return -1;
	n=dosyaUzunlugu(eklenen);
	FILE *fp2=fopen(eklenen,"r");
	if(fp2==NULL)
		return -1;
	int value=0,sub_value=0;
	char c;
	n=0;
	while(!feof(fp2)){
		fscanf(fp2,"%c",&c);
		while((c !=' ')&& (c!= '\t')&& (c!='\n') && !feof(fp2)){
			tmp=us(R,n);
			sub_value=sub_value+(int)c*tmp;	
			fscanf(fp2,"%c",&c);
			n++;
		}
		n=0;
		value=value+sub_value;	
		sub_value=0;
	}
	printf("Eklemek istenilen dosyanin value degeri:%d\n",value);
	fclose(fp2);
	int h1,h,h2,i,MM;
	h1=value % M;
	h=h1;
	if(HT[h]==NULL){
		HT[h]=eklenen;
		fprintf(fp,"%s\n",eklenen);
		printf("Eklemek istenilen dosya eklendi,konumu:%d.",h);	
		return 1;
	}
	else{
		i=0;	
		MM=M-1;
		h2=1+(value % MM);		
		int flag=0;
		while(!flag && (i < M) && HT[h]!=NULL){
			FILE *fp2=fopen(eklenen,"r");
			FILE *fp3=fopen(HT[h],"r");
			char c1,c2;
			int flag2=0;
			while(!feof(fp2) && !feof(fp3) && !flag2){
				fscanf(fp2,"%c",&c1);
				fscanf(fp3,"%c",&c2);
				if(c1!=c2)
					flag2=1;			
			}
			if(flag2==0 && (!feof(fp2)==!feof(fp3)))
				flag=1;
			else{
				h=(h1+i*h2) % M;
				i++;				
			}
			fclose(fp2);
			fclose(fp3);
		}
		if(i==M){
			printf("Deger tabloda mevcut degil fakat tabloya eklenemiyor.\n");
			return -2;
		}
		if(flag==1){
			printf("Eklemek istenilen dosya mevcut,konumu %d.",h);
			return 1;			
		}
		else if(HT[h]==NULL){
			HT[h]=eklenen;
			fprintf(fp,"%s\n",eklenen);
			printf("Eklemek istenilen dosya eklendi,konumu:%d.",h);	
			return 1;		
		}					
	}
	return 0;
}

void yazdir(char **HT,int M){
	int i;
	printf("\n");
	for(i=0;i<M;i++)
		printf("%d:%s\n",i,HT[i]);
}

int main(){
	char **HT=(char**)calloc(SIZE,sizeof(char*));
	int i,M,n,tmp;                                         //HASH TABLE A YER AÇ
	for(i=0;i<SIZE;i++)
		HT[i]=(char*)calloc(SIZE,sizeof(char));
	for(i=0;i<SIZE;i++)
		HT[i]=NULL;									//HASH TABLE I NULL DOLDUR
	M=kacsatir("Index.txt");
	M=M*10/6;
	while(!asalmi(M)){
		M++;
	}
	printf("Table size belirlendi: %d (load factor=0.6)\n",M);
	FILE *fp,*fp2;
	fp=fopen("Index.txt","r+");
	if(fp==NULL){
		printf("samples.doc acilamadi.");
		return 99;		
	}
	int value=0,while_girme,sub_value=0;
	char c;		
	while(!feof(fp)){
		char *buff=(char*)calloc(SIZE,sizeof(char));
		while_girme=0;
		value=0;
		fscanf(fp,"%s",buff);
		n=dosyaUzunlugu(buff);
		if(strlen(buff) > 2){// dosya ismi 2 karakterden büyük olmak zorunda)		
			fp2=fopen(buff,"r");	
			if(fp2==NULL){
				printf("%s adli dosya acilamadi.\n",buff);
				while_girme=1;					
			}			
			if(!while_girme){
				n=0;
				while(!feof(fp2)){
					fscanf(fp2,"%c",&c);
					while((c !=' ')&& (c!= '\t')&& (c!='\n') && !feof(fp2)){
						tmp=us(R,n);
						sub_value=sub_value+(int)c*tmp;	
						fscanf(fp2,"%c",&c);
						n++;
					}
					n=0;
					value=value+sub_value;	
					sub_value=0;
				}
				printf("%s\t",buff);
				i=hash(value,M,HT);
				printf("value = %d\tkey = %d\n",value,i);
				if(i!=-1)
					HT[i]=buff;
				else
					printf("Tablo dolu.");
				fclose(fp2);					
			}			
		}
	}

	char *eklenen=(char*)calloc(SIZE,sizeof(char));
	printf("Eklenecek olan dosya ismini giriniz.\n");
	scanf("%s",eklenen);
	i=hashSearch(eklenen,M,fp,HT);
	if(i == -1)
		printf("Girilen dosyada hata olustu.");		
	else if(i==-2){
		printf("Tablo dolu oldugundan veya double hashing uygun key bulamadigindan dosya eklenemedi.");
	}
	fclose(fp);
	yazdir(HT,M);
	return 0;
}
