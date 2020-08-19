#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MAXCHAR 4000
#define HASH_SIZE 500111

 struct NODE{
	char name[250];
	int isVisited;
	struct NODE * next;
};

typedef struct{
	struct NODE *nodes;
	int front;
	int rear;
	int size;
	char **nereden;
}Queue;

void addQueue(Queue * kuyruk,struct NODE * tmp,char * beginning){
	kuyruk->nodes[kuyruk->rear]=*tmp;
	strcpy(kuyruk->nereden[kuyruk->rear],beginning);
	kuyruk->rear=kuyruk->rear+1;
}

struct NODE * removeQueue(Queue * kuyruk){
	struct NODE *tmp=&kuyruk->nodes[kuyruk->front];
	kuyruk->front=kuyruk->front+1;
	return tmp;
}

int isEmpty(Queue * kuyruk){
	if(kuyruk->front == kuyruk->rear)
		return 1;
	return 0;
}

int findValue(char * name){
	int i=0,value=0,prime=7;
	while(name[i]!=NULL){
		if(name[i]!='\n' && name[i]!=',' && name[i]!='/' && name[i]!=' '){
			value=value+prime*(int)name[i];
			if(value < 0)
				value=value*-1;
			prime=prime*7;
		} 
		else{
			prime=7;	
		}
		i++;
	}
	return value;
}

int findKey(char * name, struct NODE * hashTable,int *toplam){
	int i=0,finalKey;
	int key=findValue(name) % HASH_SIZE;
	finalKey=key;
	while(hashTable[finalKey].name[0] != '\0' && i < HASH_SIZE && strcmp(hashTable[finalKey].name,name)){
		i++;
		finalKey=(key + i*i) % HASH_SIZE;
	}
	if(!strcmp(hashTable[finalKey].name,name))
		return finalKey;
	if(hashTable[finalKey].name[0] == '\0'){
		*toplam=*toplam+1;
		strcpy(hashTable[finalKey].name,name);
		hashTable[finalKey].next=NULL;
		return finalKey;
	}	
	return -1;
}

void addNode(char * tmp,struct NODE * hashTable,int number){
	struct NODE * current=&hashTable[number];
	while(current->next != NULL){
		current=current->next;			
	}
	struct NODE *tmpNode=(struct NODE *)calloc(1,sizeof(struct NODE));
	current->next = tmpNode;
	current=current->next;
	strcpy(current->name,tmp);
	current->next=NULL;
}

int findNo(Queue * kuyruk,char *name){
	int i,j;
	j=kuyruk->size;
	for(i=0;i<j;i++){
		if(!strcmp(kuyruk->nodes[i].name,name))
			return i;
	}
	return 0;
}

int tablodaMi(char *actor,struct NODE *actors,int actorCount){
	int i;
	for(i=0;i<HASH_SIZE;i++){
		if(!strcmp(actors[i].name,actor) && actors[i].name[0] != '\0'){
			return 1;
		}
	}
	return 0;
}

int ara(char *actor1,char *actor2,Queue *actorsQueue,Queue *filmsQueue,struct NODE *actors,struct NODE *films,int actorCount,int filmCount){
	int indis,indis2,indis3,found=0,i;
	struct NODE * current,*current2;
	struct NODE *dummy,*dummy2;
	char * gecici;
	char * film=(char*)calloc(250,sizeof(char));
	found=0;
	if(!tablodaMi(actor2,actors,actorCount)){
		if(!tablodaMi(actor1,actors,actorCount)){
			printf("\n%s ve %s isimli aktorler tabloda yok.",actor1,actor2);
			return 1;
		}
		else{
			printf("\n%s isimli aktor tabloda yok.",actor2);
			return 1;
		}
	}else if(!tablodaMi(actor1,actors,actorCount)){
		printf("\n%s isimli aktor tabloda yok.",actor1);
		return 1;
	}
	indis=findKey(actor1,actors,&actorCount);
	addQueue(actorsQueue,&actors[indis],"beginning");
	while(!isEmpty(actorsQueue) && !found && actorsQueue->rear < actorsQueue->size){
		dummy=removeQueue(actorsQueue);
		dummy->isVisited=1;
		indis=findKey(dummy->name,actors,&actorCount);
		actors[indis].isVisited=1;
		current=&actors[indis];
		while(current->next != NULL && filmsQueue->rear < filmsQueue->size){
			current=current->next;
			indis3=findKey(current->name,films,&filmCount);
			if(!films[indis3].isVisited){
				addQueue(filmsQueue,current,actors[indis].name);			
			}	
		}
		while(!isEmpty(filmsQueue) && filmsQueue->rear < filmsQueue->size){
			dummy2=removeQueue(filmsQueue);
			dummy2->isVisited=1;
			indis2=findKey(dummy2->name,films,&filmCount);
			films[indis2].isVisited=1;
			current2=&films[indis2];
			while(current2->next != NULL && !found && actorsQueue->rear < actorsQueue->size){
				current2=current2->next;
				indis3=findKey(current2->name,actors,&actorCount);		
				if(!actors[indis3].isVisited){
					addQueue(actorsQueue,current2,films[indis2].name);
				}
				if(!strcmp(current2->name,actor2)){
					found=1;
					strcpy(film,films[indis2].name);
					printf("\n\nBacon number between  %s and %s is found.\n\n",actor1,actor2);
					break;						
				}
			}
		}
	}
	int actorSon,filmSon,j;
	for(i=0;i<actorCount;i++){
		if(!strcmp(film,filmsQueue->nodes[i].name)){
			filmSon=i;
			break;
		}		
	}
	actorSon=actorsQueue->rear-1;
	if(!found || filmsQueue->rear == filmsQueue->size || actorsQueue->rear == actorsQueue){
		printf("\nBulunamadi.\n");
		return 0;
	}
	found=0;i=0;
	int result=0;
	while(!found){
		result++;
		printf("%s ve %s -> %s\n",actorsQueue->nodes[actorSon].name,filmsQueue->nereden[filmSon],actorsQueue->nereden[actorSon]);
		if(!strcmp(actor1,filmsQueue->nereden[filmSon]))
			found=1;
		actorSon=findNo(actorsQueue,filmsQueue->nereden[filmSon]);
		filmSon=findNo(filmsQueue,actorsQueue->nereden[actorSon]);
		i++;
	}
	if(result >= 6){
		printf("\nSayi 6 'dan buyuk.");
		return 1;	
	}
	printf("\nBacon number %s and %s is %d",actor1,actor2,result);
	for(i=0;i<HASH_SIZE;i++){
		actors[i].isVisited=0;
	}
	for(i=0;i<HASH_SIZE;i++){
		films[i].isVisited=0;
	}
	actorsQueue->front=0;
	actorsQueue->rear=0;
	filmsQueue->front=0;
	filmsQueue->rear=0;
	for(i=0;i<actorsQueue->size;i++){
		actorsQueue->nereden[i][0]='\0';
		actorsQueue->nodes[i].name[0]='\0';
		actorsQueue->nodes[i].next=NULL;
		actorsQueue->nodes[i].isVisited=0;
	}
	for(i=0;i<filmsQueue->size;i++){
		filmsQueue->nereden[i][0]='\0';
		filmsQueue->nodes[i].name[0]='\0';
		filmsQueue->nodes[i].next=NULL;
		filmsQueue->nodes[i].isVisited=0;
	}
	found=0;
	return 1;
}





int main(){
	FILE *fp=fopen("input-mpaa.txt","r+");
	char str[MAXCHAR];
	char *token,*tmp,*tmp2;	
	struct NODE * actors,*films;
	int number,number2,flag=1,counter=1,i;
	if(fp==NULL){
		printf("input-mpaa.txt dosyasi acilamadi.");
		return -1;
	}
	actors=(struct NODE *)calloc(HASH_SIZE,sizeof(struct NODE));
	for(i=0;i<HASH_SIZE;i++){
		actors[i].name[0]='\0';
		actors[i].next=NULL;
		actors[i].isVisited=0;
	}
	films=(struct NODE *)calloc(HASH_SIZE,sizeof(struct NODE));
	for(i=0;i<HASH_SIZE;i++){
		films[i].name[0]='\0';
		films[i].next=NULL;
		films[i].isVisited=0;
	}
	int actorCount=0,filmCount=0;
	while(fgets(str,MAXCHAR,fp)!=NULL){		
		str[strlen(str+1)]=0;
		tmp=strtok(str,"/");
		counter++;
		tmp2=tmp;
		number2=findKey(tmp,films,&filmCount);
		while(flag){
			tmp2=strtok(NULL,"/");
			if(tmp2 == NULL){
				flag=0;
				continue;
			}
			number=findKey(tmp2,actors,&actorCount);
			addNode(tmp,actors,number);
			addNode(tmp2,films,number2);
		}
		flag=1;
	}
	printf("%d sayida aktor , %d sayida film",actorCount,filmCount);
	Queue *actorsQueue=(Queue *)calloc(1,sizeof(Queue));
	actorsQueue->nodes=(struct NODE *)calloc(10*actorCount,sizeof(struct NODE));
	actorsQueue->size=10*actorCount;
	actorsQueue->front=0;
	actorsQueue->rear=0;
	actorsQueue->nereden=(char**)calloc(10*actorCount,sizeof(char*));
	for(i=0;i<10*actorCount;i++)
		actorsQueue->nereden[i]=(char*)calloc(250,sizeof(char));
	Queue *filmsQueue=(Queue *)calloc(1,sizeof(Queue));
	filmsQueue->nodes=(struct NODE *)calloc(10*filmCount,sizeof(struct NODE));
	filmsQueue->size=10*filmCount;
	filmsQueue->front=0;
	filmsQueue->rear=0;
	filmsQueue->nereden=(char**)calloc(10*filmCount,sizeof(char*));
	for(i=0;i<10*filmCount;i++)
		filmsQueue->nereden[i]=(char*)calloc(250,sizeof(char));
	char *actor1,*actor2;
	actor1=(char*)calloc(250,sizeof(char));
	actor2=(char*)calloc(250,sizeof(char));
	int secimVarMi=1;
	char *secim=(char*)calloc(25,sizeof(char));
	while(secimVarMi){
		printf("\n***************************************************\n");
		printf("\n1)Yeni arama(Girilecek 2 aktor arasindaki mesafe).\n2)Kevin bacon sayisi bulma(Girilecek aktor ile Kevin Bacon arasindaki mesafe).\n3)Cikis.\n");	
		fgets(secim,25,stdin);
		secim[strlen(secim)-1]=0;
		switch(secim[0]){
			case '1':
				printf("\n1.aktorun ismini giriniz:");
				fgets(actor1,250,stdin);
				printf("\n2.aktorun ismini giriniz:");
				fgets(actor2,250,stdin);
				actor1[strlen(actor1)-1]=0;
				actor2[strlen(actor2)-1]=0;
				ara(actor1,actor2,actorsQueue,filmsQueue,actors,films,actorCount,filmCount);
				break;
			case '2':
				printf("\nKevin Bacon sayisi alinacak aktorun ismini giriniz:");
				fgets(actor1,250,stdin);
				actor1[strlen(actor1)-1]=0;
				strcpy(actor2,"Bacon, Kevin");
				ara(actor2,actor1,actorsQueue,filmsQueue,actors,films,actorCount,filmCount);
				break;
			case '3':
				secimVarMi=0;
				break;
			default:
				printf("\n1,2 veya 3 giriniz.");
				break;
		}
	}
	fclose(fp);
	free(actors);
	free(films);
	free(filmsQueue);
	free(actorsQueue);
	return 0;
}
