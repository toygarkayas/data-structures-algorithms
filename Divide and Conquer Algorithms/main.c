#include <stdio.h>
#include <stdlib.h>
#include <math.h>
//TMP[0]=1.nokta x TMP[1]=1.nokta y TMP[2]=UZAKLIK TMP[3]=2.nokta x TMP[4]=2.nokta y
/////////////////////////////////////////////////////
typedef struct{
	int x,y;
}point;

point *merge_tmp;

void siralaY(point *p,int baslangic,int bitis){//y lere göre sýrala
	int orta,i,j,n,merge_i;
	if(baslangic < bitis){
		orta=(baslangic+bitis)/2;
		siralaY(p,baslangic,orta);
		siralaY(p,orta+1,bitis);
		//MERGE
		n=bitis-baslangic+1;
		i=baslangic;
		j=orta+1;
		merge_i=0;
		while(i <= orta && j <= bitis){
			if(p[i].y < p[j].y){
				merge_tmp[merge_i].x=p[i].x;
				merge_tmp[merge_i].y=p[i].y;
				i++;
			}
			else{
				merge_tmp[merge_i].x=p[j].x;
				merge_tmp[merge_i].y=p[j].y;
				j++;
			}
			merge_i++;
		}
		while(i <= orta){
			merge_tmp[merge_i].x=p[i].x;
			merge_tmp[merge_i].y=p[i].y;
			i++;
			merge_i++;
		}
		while(j <= bitis){
			merge_tmp[merge_i].x=p[j].x;
			merge_tmp[merge_i].y=p[j].y;
			j++;
			merge_i++;
		}
		int k;
		for(k=0;k<n;k++){
			p[baslangic].x=merge_tmp[k].x;
			p[baslangic].y=merge_tmp[k].y;
			baslangic++;
		}
		//MERGE		
	}
}

void siralaX(point *p,int baslangic,int bitis){//x lere göre sýrala
	int orta,i,j,n,merge_i;
	if(baslangic < bitis){
		orta=(baslangic+bitis)/2;
		siralaX(p,baslangic,orta);
		siralaX(p,orta+1,bitis);
		//MERGE
		n=bitis-baslangic+1;
		i=baslangic;
		j=orta+1;
		merge_i=0;
		while(i <= orta && j <= bitis){
			if(p[i].x < p[j].x){
				merge_tmp[merge_i].x=p[i].x;
				merge_tmp[merge_i].y=p[i].y;
				i++;
			}
			else{
				merge_tmp[merge_i].x=p[j].x;
				merge_tmp[merge_i].y=p[j].y;
				j++;
			}
			merge_i++;
		}
		while(i <= orta){
			merge_tmp[merge_i].x=p[i].x;
			merge_tmp[merge_i].y=p[i].y;
			i++;
			merge_i++;
		}
		while(j <= bitis){
			merge_tmp[merge_i].x=p[j].x;
			merge_tmp[merge_i].y=p[j].y;
			j++;
			merge_i++;
		}
		int k;
		for(k=0;k<n;k++){
			p[baslangic].x=merge_tmp[k].x;
			p[baslangic].y=merge_tmp[k].y;
			baslangic++;
		}
		//MERGE
	}		
}

float distance(point p1,point p2){//2 nokta arasýndaki uzaklýk
	return sqrt((p1.x - p2.x)*(p1.x - p2.x) + (p1.y - p2.y)*(p1.y - p2.y));
}

float * findMinDistance(point *p,int n){//bir point dizisindeki min uzaklýk O(n2)
	int i,j;
	float min=999;
	float *tmp=(float *)calloc(5,sizeof(float));
	for(i=0;i<n;i++)
		for(j=i+1;j<n;j++)
			if(distance(p[i],p[j]) < min){
				tmp[0]=(float)p[i].x;
				tmp[1]=(float)p[i].y;
				min=distance(p[i],p[j]);	
				tmp[2]=min;	
				tmp[3]=(float)p[j].x;
				tmp[4]=(float)p[j].y;	
			}
	return tmp;
}

float min(float x,float y){//minimum döndür
	if(x<y)
		return x;
	return y;
}

float * minStrip(point *strip,int n,float *d){// y ye göre sýralý noktalar kümesindeki min uzaklýk
	int i,j;
	float min=d[2];
	float *tmp=(float *)calloc(5,sizeof(float));
	tmp[0]=d[0];
	tmp[1]=d[1];
	tmp[2]=d[2];
	tmp[3]=d[3];
	tmp[4]=d[4];
	siralaY(strip,0,n-1);
	
	for(i=0;i<n;i++){
		for(j=i+1;j<n;j++){
			if(distance(strip[i],strip[j]) < min){
				tmp[0]=strip[i].x;
				tmp[1]=strip[i].y;
				min=distance(strip[i],strip[j]);
				tmp[2]=min;
				tmp[3]=strip[j].x;
				tmp[4]=strip[j].y;
			}					
		}
	}

	return tmp;
}

float * closestPair(point *p,int n){
	if(n < 2){
		printf("Gecersiz dizi.");
		return NULL;		
	}

	if(n <= 3)
		return findMinDistance(p,n);
		
	float *dl=(float *)calloc(5,sizeof(float));
	float *dr=(float *)calloc(5,sizeof(float));
	float *d=(float *)calloc(5,sizeof(float));
	float *str=(float *)calloc(5,sizeof(float));		
	
	int mid=n/2;
	point midPoint=p[mid];	
	dl=closestPair(p,mid);
	dr=closestPair(p+mid,n-mid);
	
	if(dl[2] < dr[2]){
		d[4]=dl[4];
		d[3]=dl[3];
		d[2]=dl[2];
		d[1]=dl[1];
		d[0]=dl[0];
	}
	else{
		d[4]=dr[4];
		d[3]=dr[3];
		d[2]=dr[2];
		d[1]=dr[1];
		d[0]=dr[0];
	}

	point strip[n];
	int i,j=0;
	for(i=0;i<n;i++)
		if(abs(p[i].x - midPoint.x) < d[2]){
			strip[j]=p[i];
			j++;			
		}
	str=minStrip(strip,j,d);
	if(d[2] < str[2]){
		return d;
	}
	return str;
}

int main() {
	FILE *fp;
	fp=fopen("Input.txt","r");
	if(fp==NULL){
		printf("Dosya acilamadi.");
		return 0;
	}	
	point *p,*ptr;
	p=(point *)calloc(1,sizeof(point));
	int n=0,tmp,kontrol;
	while(!feof(fp)){
		ptr=(point *)realloc(p,sizeof(point)*(n+1));
		p=ptr;	
		fscanf(fp,"%d",&tmp);
		p[n].x=tmp;
		fscanf(fp,"%d",&tmp);
		p[n].y=tmp;
		n++;
	}
	n--;
	merge_tmp=(point *)calloc(n+1,sizeof(point));
	siralaX(p,0,n-1);
	float *sonuc;
	sonuc=closestPair(p,n);
	if(sonuc!=NULL)
		printf("En yakin iki nokta: [%d,%d] ve [%d,%d].\nAralarindaki mesafe:%f ",(int)sonuc[0],(int)sonuc[1],(int)sonuc[3],(int)sonuc[4],sonuc[2]); 
	return 0;
}
