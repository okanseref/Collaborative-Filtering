#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define LEN 8

int UArray[20][8];
int UNArray[5][8];
char movies[8][100];
float kList[100]; //Benzer ki�ilerin benzerlik de�erlerini tutacak dizi
int kIndexList[100]; //Benzer ki�ilerin indekslerini tutacak de�er
int mainPersonIndex=2; //UNArrayde ka��nc� ki�ide oldu�umuzu g�steren indeks
int k=3; //Ka� benzer ki�i kullan�laca��n� tutan de�er
void addToKList(float val,int index){//Benzer kullan�c�lar�n listesine ekleme yapar, val-> eklenenin benzerli�i, index-> eklenenin indeksi
	int i=0;
	int minIndex=0;
	for(i=1;i<k;i++){//Minimum olan de�erin indeksi bulunur
		if(kList[i]<kList[minIndex]){
			minIndex=i;
		}
	}
	if(kList[minIndex]<val){//Minimum olan de�erden b�y�kse listede minimum de�erin yerine ge�er
		kList[minIndex]=val;
		kIndexList[minIndex]=index;//Benzer kullan�c�lar�n indeksi de ayn� indekste indeks dizisinde tutulur
	}
}
float similarity(int *personA,int *personB){//2 ki�inin benzerli�ini bulan fonksiyon. Ki�iler sat�r olarak gelip verdikleri puanlar� bar�nd�r�yorlar.
	int i;
	float rA=0;//ilk ki�inin ortalamas�n� tutacak de�er
	int aCount=0;//ilk ki�inin okudu�u kitap say�s�n� tutacak de�er
	float rB=0;//ikinci ki�inin ortalamas�n� tutacak de�er
	int bCount=0;//ikinci ki�inin okudu�u kitap say�s�n� tutacak de�er
	float topTotal=0;//i�lemimizin �stteki pay de�erini tutacak de�er
	float botLeftTotal=0;//paydan�n sol taraf�ndaki toplama i�leminin sonucunu tutacak de�er
	float botRightTotal=0;//paydan�n sa� taraf�ndaki toplama i�leminin sonucunu tutacak de�er

	for(i=0;i<LEN;i++){//Ortak olmayan kitaplar� 0 l�yoruz
		if((personA[i]!=0&&personB==0)||(personB[i]!=0&&personA==0)){
			personA[i]=0;
			personB[i]=0;
		}
	}
	for(i=0;i<LEN;i++){//�ki ki�inin de ortalama puanlar�n� buluyoruz
		if(personA[i]!=0&&personB[i]!=0){
			aCount++;
			rA+=personA[i];
			bCount++;
			rB+=personB[i];
		}
	}
	rA/=aCount;
	rB/=bCount;
	for(i=0;i<LEN;i++){//Ortak her kitap i�in similarity fonksiyonunun toplama i�lemleri
		if(personA[i]!=0&&personB[i]!=0){
			topTotal+=(personA[i]-rA)*(personB[i]-rB);
			botLeftTotal+=(personA[i]-rA)*(personA[i]-rA);
			botRightTotal+=(personB[i]-rB)*(personB[i]-rB);
		}
	}
	botLeftTotal=sqrt(botLeftTotal);//son olarak paydadaki de�erleri karek�ke al�yoruz
	botRightTotal=sqrt(botRightTotal);
	return topTotal/(botLeftTotal*botRightTotal);//D�n�� de�erimiz �neri say�m�z -1 +1 aral���nda
}
void findKSimilars(int personA){//Al�nan personA ki�isine UArray'den en �ok benzeyen k tane ki�iyi bulan fonksiyon
	int i=0;
	for(i=0;i<k;i++){
		kList[i]=-2;
	}
	for(i=0;i<20;i++){
		addToKList(similarity(UNArray[personA],UArray[i]),i);
	}
}
void predict(){//mainPersonIndex'deki ki�iye benzeyenlere bakarak kitap �neren ve yazd�ran fonksiyon
	int kitapIndex=0; //Ka��nc� kitapta oldu�umuzu tutar
	int i;
	int j;
	float temp;
	int tempIndex;
	float Uaverage=0;//Benzeyenler listesindeki ki�inin puan ortalamas�n� tutar
	int Ucount=0;//Benzeyenler listesindeki ki�inin okudu�u kitap say�s�
	float UNaverage=0;//Kitap �nerece�imiz ki�inin puan ortalamas�
	int UNcount=0;//kitap �nerece�imiz ki�inin okudu�u kitap say�s�
	float predict=-99;//�lk �neri de�erimiz �ok d���k bir de�er veriyoruz ki �zerine yaz�ls�n
	int predictIndex;//�nerilen kitap�n kitap isimleri listesindeki indeksi
	float top;//��lemimizde pay k�sm�n�n toplam�
	float bottom;//��lemde payda k�sm�n�n toplam�
	for(i=0;i<LEN;i++){//�lk olarak ilgili ki�inin ortalama puan�n� buluyoruz
		if(UNArray[mainPersonIndex][i]!=0){
			UNcount++;
			UNaverage+=(float)UNArray[mainPersonIndex][i];
		}
	}
	UNaverage/=UNcount;
	
	printf("\n\nNU%d kullanicisina en yakin kullanicilar (k=%d) ve hesaplanan pearson benzerlikleri sirasiyla: ",mainPersonIndex+1,k);
	for (i=0;i<k-1;i++) {  //Haz�r olan k listesimizi s�rayla yazd�raca��m�z i�in s�ral�yoruz
		for(j=0;j<k-i-1;j++){
			if(kList[j]>kList[j+1]){
				temp=kList[j+1];
				tempIndex=kIndexList[j+1];
				kList[j+1]=kList[j];
				kIndexList[j+1]=kIndexList[j];
				kList[j]=temp;
				kIndexList[j]=tempIndex;
			}
		}
	} 
	for(i=k-1;i>=0;i--){
		printf("\n  U%-3d : %f   ",kIndexList[i]+1,kList[i]);
	}
	printf("\n\nNU%d kullanicisinda okunmamis olan kitaplar icin hesaplanan tahmini begenme degerleri\n",mainPersonIndex+1);

	while(kitapIndex<LEN){//Her bir kitap i�in inceleyece�iz
		if(UNArray[mainPersonIndex][kitapIndex]==0){//Kullan�c�n�n okumad��� kitaplara bak�yoruz
			top=0;
			bottom=0;
			for(i=0;i<k;i++){//��lemin pay k�sm�nda benzedi�i ki�ilerle ilgili i�lemleri yap�yoruz. K benzedi�i ki�i say�s�
				
				for(j=0;j<LEN;j++){// �lgili ki�inin ortalamas�n� buluyoruz
					if(UArray[kIndexList[i]][j]!=0){
						Uaverage+=UArray[kIndexList[i]][j];
						Ucount++;
					}	
				}
				Uaverage/=Ucount;
				//pay ve payda k�sm�ndaki toplama i�lemleri her ad�mda ger�ekle�iyor
				top+=(similarity(UNArray[mainPersonIndex],UArray[kIndexList[i]]))*((float)UArray[kIndexList[i]][kitapIndex]-(float)Uaverage);//sim(a,b)*rbp-rb
				bottom+=similarity(UNArray[mainPersonIndex],UArray[kIndexList[i]]);
				Ucount=0;
				Uaverage=0;

			}
				
			printf("%d. %s , %f\n",kitapIndex,movies[kitapIndex+1],UNaverage+(top/bottom));

			//Bulunan puan de�eri �ncekinden y�ksekse yeni �nerece�imiz kitap mertebesine ula�m�� oluyor 
			if(UNaverage+(top/bottom)>predict){
				predictIndex=kitapIndex;
				predict=UNaverage+(top/bottom);
			}
		}
		kitapIndex++;
	}
	//printf("---------- NU%d --------\n",mainPersonIndex+1);
	printf("\nSonuc olarak onerilen kitap: %s \n",movies[predictIndex+1]);
}
struct my_record {
    char name;
    int value;
};
int fileRead(){
	FILE *fa;//Okulunacak dosya
	char str[100];//Eklenecek kelimeyi tutacak string
	char c;
	int temp;
	int skip=1;
	int index=0;
	int splitter=0;
	int numRead=0;
	int personIndex=0;
	int bookIndex=0;
	fa=fopen ("RecomendationDataSet.csv","r");
	if(fa==NULL){
		return 0;
	}
	while (!feof(fa))
	{
		if(splitter<9){//movie isimleri ald�k
			fscanf(fa,"%c",&c);
			if(c==';'||c=='\n'){
				strcpy(movies[splitter],str);
				index=0;
				str[0]='\0';
				splitter++;
			}else{
				str[index]=c;
				str[index+1]='\0';
				index++;
			}
		}else{
			fscanf(fa,"%c",&c);
			if(c==';'){
				numRead=1;
				if(bookIndex>7){
					bookIndex=0;
					personIndex++;
				}
			}
			if(numRead==1){
				if(personIndex<20){
					fscanf(fa,"%d",&UArray[personIndex][bookIndex]);//U karakterleri U listesine al�yoruz
				}else{
					fscanf(fa,"%d",&UNArray[personIndex%20][bookIndex]);//%20 Olmas�n�n sebebi UNlere ge�ince bozulmamas� i�in
				}
				bookIndex++;
				numRead=0;
			}

		}
	}
	return 1;
}
int main(){
	int i,j;
	if(fileRead()==0){
		printf("Dosya bulunamadi (RecomendationDataSet.csv)");
		return 0;
	}
	char string[100];
	printf("Kitap onerisi yapilacak kullanici adi: ");

	scanf("%s" ,string);
	j=string[2]-48;
	if(j<1||j>5){
		printf("Gecersiz kullanici adi");
		return 0;
	}
	printf("Benzer kullanici sayisi (k): ");
	scanf("%d",&k);
	
	mainPersonIndex=j-1;//�lgili ki�inin indeksi
	findKSimilars(mainPersonIndex);//Benzer kullanc�lar� bulup kListe at�yoruz
	predict();//Kitab� �neriyoruz

	return 0;
}
