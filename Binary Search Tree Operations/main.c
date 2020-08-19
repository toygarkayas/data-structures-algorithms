#include <stdio.h>
#include <stdlib.h>
#include <string.h>
struct node *root;				//linkli listelerdeki head gibi agacin en ustunu gosteren bir pointer

struct node{					//agac icin bir structure tanimi
	char name[20];
	char surname[20];
	int kimlikNo;
	int arkadasNo[10];
	struct node* left;
	struct node* right;
};

struct node* max(struct node *tmp){//verilen bir node un altýndaki elemanlarýn maksimumunun yerini döndüren fonksiyon
	while(tmp->right!=NULL)
		tmp=tmp->right;
	return tmp;
}

struct node* min(struct node *tmp){//verilen bir node un altýndaki elemanlarýn minimumunun yerini döndüren fonksiyon
	while(tmp->left!=NULL)
		tmp=tmp->left;
	return tmp;
}

struct node* findParent(struct node *tmp){//verilen node un parent ini bulup donduren fonksiyon
	struct node *current;					//current agacta gezmek icin kullanilan bir degisken
	current=root;
	while(current!=NULL && tmp!=current->left && tmp!=current->right){
		if(current->kimlikNo < tmp->kimlikNo)
			current=current->right;
		else
			current=current->left;
	}
	return current;
}

void insertNewUser(struct node *new,int no){
	struct node* current;				//current agacta gezmek icin kullanilan bir degisken
	int stop=0;
	if(root==NULL)
		root=new;
	else{
		struct node *parent;			//parent bir onceki elemani tutan bir degisken (linkli listedeki previous gibi dusunebiliriz)
		current=root;
		while(!stop){
			parent=current;
			if(no < current->kimlikNo){
				current=current->left;
				if(current==NULL){
					parent->left=new;
					stop=1;
				}
			}
			else{
				current=current->right;
				if(current==NULL){
					parent->right=new;
					stop=1;
				}
			}
		
		}
	}
}

struct node* deleteUser(struct node *tmp,int no){//tmp=root
	struct node *dummy,*parent;
	int i;
	if(tmp==NULL)								//root NULL ise agac yok demektir
		return NULL;
	if(tmp->kimlikNo==no){						//silmek istedigimiz eleman root ise
		if(tmp->left==NULL && tmp->right==NULL){
			parent=findParent(tmp);
			if(parent->left==tmp)
				parent->left=NULL;
			else if(parent->right==tmp)
				parent->right=NULL;
			return NULL;
		}
		if(tmp->right!=NULL){					//root un sagi bos degil ise
			dummy=min(tmp->right);
			parent=findParent(dummy);
			tmp->kimlikNo=dummy->kimlikNo;
			strcpy(tmp->name,dummy->name);
			strcpy(tmp->surname,dummy->surname);
			for(i=0;i<10;i++)									//veri kopyalama			
				tmp->arkadasNo[i]=dummy->arkadasNo[i]; 	
			if(parent->left==dummy)
				parent->left=NULL;
			else if(parent->right==dummy)
				parent->right=NULL;
			free(dummy);
			return tmp;
		}
		dummy=max(tmp->left);					//root un sagi bos ise
		parent=findParent(dummy);	
		tmp->kimlikNo=dummy->kimlikNo;		
		strcpy(tmp->name,dummy->name);
		strcpy(tmp->surname,dummy->surname);
		for(i=0;i<10;i++)
			tmp->arkadasNo[i]=dummy->arkadasNo[i];		//veri kopyalama	
		if(parent->left==dummy)
				parent->left=NULL;
		else if(parent->right==dummy)
				parent->right=NULL;
		free(dummy);
		return tmp;		
	}	
	if(tmp->kimlikNo < no){							//silmek istedigimiz eleman root degil ise
		tmp=deleteUser(tmp->right,no);
		return tmp;
	}
	tmp=deleteUser(tmp->left,no);
	return tmp;
}

int contains(int no){//kimlik numarasi verilmis bir kisinin agacta varsa ismini ve soyismini yazdirip 1 donduren, agacta yoksa 0 donduren fonksiyon
	struct node *current;					//current agacta gezmek icin kullanilan bir degisken
	current=root;
	printf("%d ARANIYOR\n",no);
	while((current!=NULL)&&(current->kimlikNo!=no)){
		printf("Kimlik No:%d \tUYUMSUZ\n",current->kimlikNo);
		if(no < current->kimlikNo)
			current=current->left;
		else
			current=current->right;	
	}
	if(current!=NULL){
		printf("BULUNDU\nIsim:%s, Soyisim:%s",current->name,current->surname);
		return 1;
	}
	printf("Aradiginiz kisi bulunamadi.");
	return 0;

}

struct node *varMi(int no){ //kimlik numarasi verilmis bir kisinin agacta yerini bulan fonksiyon
	struct node *current;				//current agacta gezmek icin kullanilan bir degisken
	current=root;
	while((current!=NULL)&&(current->kimlikNo!=no)){
		if(no < current->kimlikNo)
			current=current->left;
		else
			current=current->right;	
	}
	return current;
}

void friends(int no){//kimlik numarasi verilen birinin arkadaslarini yazdiran fonksiyon
	int i=0;
	struct node *current,*tmp;				//current agacta gezmek icin kullanilan bir degisken , tmp ise ilgili kisinin arkadasinin olup olmadigini (NULL olup olmadigini) kontrol etmek icin kullanilen degisken
	current=varMi(no);
	if(current!=NULL){
		printf("%d numarali kisinin arkadaslari:\n",no);
		while((current->arkadasNo[i]>=0) && i<10){ //Herhangi birinin kimlik numarasi 0 olamaz dolayisiyla 0 degilse arkadasi yazdýrabiliriz.
			tmp=varMi(current->arkadasNo[i]);
			if(tmp!=NULL)
				printf("\t%d.arkadasi:%s %s\n",i+1,tmp->name,tmp->surname);
			i++;
		}			
	}
	else{
		printf("\n%d numarali kisi agacta yok.Arkadaslari yazdirilamadi.\n",no);
		i=1;														//eger kisi agacta yok ise asagidaki if e girmesin
	}
	if(i==0)
		printf("\n%d numarali kisinin arkadasi bulunmuyor.\n",no);
}

int size(struct node *tmp){//tmp yi root olarak kabul ederek agacin size ini bulur
	if(tmp!=NULL)
		return(size(tmp->left)+size(tmp->right)+1);
	else
		return 0;
}

void printTree(struct node *tmp){//tmp yi root olarak kabul ederek agaci yazdirir
	if(tmp!=NULL){
		printf("Kimlik No:%d \tIsim:%s\tSoyisim:%s\n",tmp->kimlikNo,tmp->name,tmp->surname);
		printTree(tmp->left);
		printTree(tmp->right);
	}
}

void printGreater(int no){//verilen numaradan buyuk olan numarali elemanlari yazdirir
	struct node *tmp,*parent; //tmp ye verilen numaranin agactaki node unu aliyoruz parent ise tmp nin parent i
	tmp=varMi(no);
	parent=findParent(tmp);
	if(root->kimlikNo > no && tmp!=root){			//eleman agacin solunda ise
		printTree(tmp->right);
		while(parent->kimlikNo < tmp->kimlikNo)		//parent aradigimiz numaradan kucuk olabilir , aradigimiz numaradan buyuk bir parent arama
			parent=findParent(parent);				
		printf("Kimlik No:%d \tIsim:%s\tSoyisim:%s\n",parent->kimlikNo,parent->name,parent->surname);
		printGreater(parent->kimlikNo);
	}
	else if(tmp!=root){								//eleman agacin saginda ise
		printTree(tmp->right);	
		if(parent!=root){
			while(parent->kimlikNo < tmp->kimlikNo)	//parent aradigimiz numaradan kucuk olabilir , aradigimiz numaradan buyuk bir parent arama
				parent=findParent(parent);			
			printf("Kimlik No:%d \tIsim:%s\tSoyisim:%s\n",parent->kimlikNo,parent->name,parent->surname);
			printGreater(parent->kimlikNo);
		}			
	}
	else
		printTree(tmp->right);
}
void printInOrder(struct node *tmp){//tmp yi root olarak kabul ederek agaci inOrder ile yazdirir
	if(tmp!=NULL){
		printInOrder(tmp->left);
		printf("Kimlik No:%d \tIsim:%s\tSoyisim:%s\n",tmp->kimlikNo,tmp->name,tmp->surname);
		printInOrder(tmp->right);
	}
		
}

void printNext(int no){//kimlik nosu verilen birinin altindaki agaci yazdiran fonksiyon
	struct node *tmp=varMi(no); //kimlik nosu verilen kisinin agactaki yeri
	if(tmp!=NULL){
		printInOrder(tmp->left);
		printInOrder(tmp->right);
	}
}

int main(int argc, char *argv[]) {
	root=(struct node*)calloc(1,sizeof(struct node));				//agacin root una yer acma islemi
	FILE *fp;
	fp=fopen("Input.txt","r");										
	if(fp!=NULL)													//dosya acma islemleri
		printf("Input.txt adli dosya acildi.\n");
	else{
		printf("dosya acilamadi cikiliyor.");
		return 0;
	}
	int i,decimal,j=0,isFirst=0,printCounter=0;						//i degiskeni dosyadan okudugum stringin üzerinde gezinmek için  , j degiskeni okudugum kisilerin ismini , soyismini ve arkadaslarini kopyalarken tek tek kopyalamak icin bir indis
	char tmp[50];													//decimal degiskeni kisinin ve arkadaslarinin kimlik numarasini alirken kullandigim bir dummy degisken , isFirst degiskeni ilk gelen elemani root olarak set etmeme yarayan degisken
																	//printCounter ise 10 insert isleminde bir agaci inOrder ile yazdirmak icin kullandigim degisken	
	while(!feof(fp)){
		struct node *dummy;
		dummy=(struct node*)calloc(1,sizeof(struct node));          
		fgets(tmp,50,fp);
		decimal=0;
		i=0;
		if(tmp[0]!=10){         //eðer bir satýrdaki ilk karakter diðer satýra geçmemizi gerektiren karakter ise(10) yani o satýr boþ ise girmemize gerek yok.
			while(tmp[i]!=','){
			decimal=decimal*10;
			decimal=decimal+(tmp[i]-'0');					//kisinin kimlik numarasinin alindigi alan
			i++;
			}i++;
			dummy->kimlikNo=decimal;
			j=0;
			while(tmp[i]!=' '){
				dummy->name[j]=tmp[i];
				i++;										//kisinin isminin alindigi alan
				j++;
			}i++;
			j=0;
			while(tmp[i]!=',' && tmp[i]!=10){			//10 numarali eleman bir sonraki satira gecmemizi soyleyen karakterdir
				dummy->surname[j]=tmp[i];
				i++;										//kisinin soyisminin alindigi alan
				j++;
			}i++;
			j=0;
			decimal=0;
			while(tmp[i]!=10){
				while(tmp[i]!='-' && tmp[i]!=10){		//10 numarali eleman bir sonraki satira gecmemizi soyleyen karakterdir
					decimal=decimal*10;						//kisinin arkadaslarinin kimlik numaralarinin alindigi alan
					decimal=decimal+(tmp[i]-'0');
					i++;
				}
				dummy->arkadasNo[j]=decimal;
				j++;
				decimal=0;
				if(tmp[i]!=10)
					i++;
			}
			insertNewUser(dummy,dummy->kimlikNo);
			printCounter++;
			if(printCounter==10){												//10 kayitta bir yazdirma islemi
				printCounter=0;
				printInOrder(root);
				printf("**********************************************************\n");
			}
			if(!isFirst)
				root=dummy;									//eger ilk insert ise root set etme
			isFirst=1;	
		}		
	}
	
	printInOrder(root);                                									
	printf("**********************************************************\n");

	
	int menu=1,menuNo,sayi;    //sayi bir dummy degisken fonksiyonlara kullanicidan aldigim degeri yollarken kullaniyorum.menu 0 oldugu durumda menu kapaniyor. menuNo ise hangi islemin yapilacagini sectiriyor.
	
	while(menu){
		printf("\n\nYapmak istediginiz islemin numarasini giriniz.");
		printf("\n1)insertNewUser\n2)deleteUser\n3)contains\n4)friends\n5)size\n6)printNext\n7)printGreater\n8)printInOrder\n9)EXIT\n");
		scanf("%d",&menuNo);
		struct node *new=(struct node*)calloc(1,sizeof(struct node));
		switch(menuNo){
			case 1:		
				printf("\nEklenecek kisinin kimlik nosunu giriniz.\t");
				scanf("%d",&new->kimlikNo);
				printf("\nEklenecek kisinin ismini ve soyismini giriniz.");
				scanf("%s %s",new->name,new->surname);
				insertNewUser(new,new->kimlikNo);
				break;
			case 2:
				printf("\nSilinecek kisinin kimlik nosunu giriniz.\t");
				scanf("%d",&sayi);
				deleteUser(root,sayi);
				printf("\n%d nolu kisi silindi.",sayi);
				break;
			case 3:
				printf("\nAranacak kisinin kimlik nosunu giriniz.\t");
				scanf("%d",&sayi);
				contains(sayi);
				break;
			case 4:
				printf("\nArkadaslarini goruntelemek istediginiz kisinin kimlik nosunu giriniz.\t");
				scanf("%d",&sayi);
				friends(sayi);
				break;
			case 5:
				printf("\nAgacin size i:%d",size(root));
				break;
			case 6:
				printf("\nKimlik no giriniz.(girdiginiz kisinin altindaki kisileri goruntulemek icin)\t");
				scanf("%d",&sayi);
				printNext(sayi);
				break;
			case 7:
				printf("\nGireceginiz kimlik nolu insandan daha buyuk kimlik noya sahip olan insanlari yazdirmak icin kimlik no giriniz.\t");		
				scanf("%d",&sayi);
				printf("\nNumarasi %d den buyuk olanlar:\n",sayi);
				printGreater(sayi);
				break;
			case 8:
				printInOrder(root);
				break;
			case 9:
				menu=0;
				break;
			default:
				printf("Hatali giris.");
		}
	}
	
	
	fclose(fp);	
	return 0;
}
