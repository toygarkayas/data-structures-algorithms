#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MAX 2415
int *kuyruk,front=0,back=0,*visited,*gelinen,gelinenNo=0;
char **yazdir;

int testGraph(char **matrix,char **word,char *w1,char *w2){	//verilen iki kelimenin komþuluk durumunu veren fonksiyon
	int i=0,j=0;
	while(strcmp(word[i],w1)!=0 && i<MAX+1)
		i++;
	while(strcmp(word[j],w2)!=0 && j<MAX+1)
		j++;	
	if(i<MAX && j<MAX){
		if(matrix[i][j]==0)
			return 0;
		else
			return 1;	
	}
	else
		return 0;
}

void kuyrugaEkle(char **matrix,int dummy){		//verilen kelimenin bütün komþularý kuyruða eklenir.(Visited olarak iþaretlenmemiþ komþularý)
	int i;
	for(i=0;i<MAX;i++){	
		if(visited[i]==0 && matrix[dummy][i]==1 && i!=dummy){
			kuyruk[back]=i;
			back++;
			visited[i]=1;
		}
	}
}

void donusumBul(char **matrix,char **word,char *w1,char *w2){
	int i=0,j=0,k,dummy,found=0,yazdirNo=0;
	char tmp[6],exchange;
	while(strcmp(word[i],w1)!=0 && i<MAX)
		i++;
	while(strcmp(word[j],w2)!=0 && j<MAX)
		j++;
	if(i<MAX && j<MAX){
		kuyruk[back]=i;
		back++;
		while(front!=back && !found){
			dummy=kuyruk[front];
			front++;
			visited[dummy]=1;
			for(k=0;k<back;k++){
				if(kuyruk[k]==j){
					dummy=j;												//her seferinde kuyruk kontrol ediliyor aranan kelime bulundu mu ?
					found=1;	
				}
			}
			if(dummy!=j){
				gelinen[gelinenNo]=dummy;									//aþaðýda yol çizmek için geçilen kelimeler bir dizide tutuluyor
				gelinenNo++;
				kuyrugaEkle(matrix,dummy);
			}
		}
	}
	if(found!=1)
		printf("Donusum yok\n");
	else{														
		k=gelinenNo-1;														//burada gelinen yol bulunur yani sondan baþlanarak bütün kuyruk gezilir ve yol bulunur.
		strcpy(tmp,word[j]);												//her seferinde tek harf deðiþimi hangi komþudan geldiði bulunuyor
		while(k>=0){
			if(testGraph(matrix,word,word[gelinen[k]],tmp)){
				strcpy(tmp,word[gelinen[k]]);
				strcpy(yazdir[yazdirNo],tmp);
				yazdirNo++;				
			}
			k--;
		}
	}
	if(found==1){
			printf("\n");
			for(k=yazdirNo-1;k>=0;k--)										//yukarýda tersten yol çizildiði için tersten yazdýrýlýyor
				printf("%s->",yazdir[k]);
			printf("%s\n",word[j]);
			printf("\nIslem %d adimda tamamlandi.\n",yazdirNo);		
	}
}


int main() {
	int i,j,k,fark;
	FILE *fp;
	fp=fopen("kelime.txt","r");
	if(fp==NULL){
		printf("Dosya acilamadi.");
		return 0;
	}
	char dummy[6];
	char **matrix,**word;
	kuyruk=(int *)calloc(MAX,sizeof(int));
	visited=(int *)calloc(MAX,sizeof(int));
	gelinen=(int *)calloc(MAX,sizeof(int));
	matrix=(char **)calloc(MAX,sizeof(char *));
	for(i=0;i<MAX;i++)
		matrix[i]=(char *)calloc(MAX,sizeof(char));						//graf için yer açma
	
	word=(char **)calloc(MAX,sizeof(char *));
	for(i=0;i<MAX;i++)													//kelimeler için yer açma
		word[i]=(char *)calloc(5,sizeof(char));

	yazdir=(char **)calloc(MAX,sizeof(char *));
	for(k=0;k<MAX;k++)													//yazdirma dizisi için yer açma
		yazdir[k]=(char *)calloc(5,sizeof(char));
	i=0;
	while(!feof(fp)){
		fgets(dummy,7,fp);
		for(j=0;j<5;j++)												//kelimeler bir diziye alýnýyor
			word[i][j]=dummy[j];
		i++;
	}
	fclose(fp);	
	for(i=0;i<MAX;i++){
		for(j=i+1;j<MAX;j++){
			fark=0;
			for(k=0;k<5;k++){
				if(word[i][k]!=word[j][k])								//kelimenin komþularý ile arasýnda kaç harf fark var o bulunuyor
					fark++;
			}
			if(fark > 1){
				matrix[i][j]=0;											//ve komþuluk durumu set ediliyor
				matrix[j][i]=0;	
			}
			else{
				matrix[i][j]=1;
				matrix[j][i]=1;				
			}
		}		
	}
	char k1[5],k2[5];
	printf("1. kelimeyi giriniz: ");
	scanf("%s",k1);
	printf("2. kelimeyi giriniz: ");
	scanf("%s",k2);
	donusumBul(matrix,word,k1,k2); 
	return 0;
}
