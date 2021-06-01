#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define LEN 8

int UArray[20][8];
int UNArray[5][8];
char movies[8][100];
float kList[100]; //Benzer kiþilerin benzerlik deðerlerini tutacak dizi
int kIndexList[100]; //Benzer kiþilerin indekslerini tutacak deðer
int mainPersonIndex=2; //UNArrayde kaçýncý kiþide olduðumuzu gösteren indeks
int k=3; //Kaç benzer kiþi kullanýlacaðýný tutan deðer
void addToKList(float val,int index){//Benzer kullanýcýlarýn listesine ekleme yapar, val-> eklenenin benzerliði, index-> eklenenin indeksi
	int i=0;
	int minIndex=0;
	for(i=1;i<k;i++){//Minimum olan deðerin indeksi bulunur
		if(kList[i]<kList[minIndex]){
			minIndex=i;
		}
	}
	if(kList[minIndex]<val){//Minimum olan deðerden büyükse listede minimum deðerin yerine geçer
		kList[minIndex]=val;
		kIndexList[minIndex]=index;//Benzer kullanýcýlarýn indeksi de ayný indekste indeks dizisinde tutulur
	}
}
float similarity(int *personA,int *personB){//2 kiþinin benzerliðini bulan fonksiyon. Kiþiler satýr olarak gelip verdikleri puanlarý barýndýrýyorlar.
	int i;
	float rA=0;//ilk kiþinin ortalamasýný tutacak deðer
	int aCount=0;//ilk kiþinin okuduðu kitap sayýsýný tutacak deðer
	float rB=0;//ikinci kiþinin ortalamasýný tutacak deðer
	int bCount=0;//ikinci kiþinin okuduðu kitap sayýsýný tutacak deðer
	float topTotal=0;//iþlemimizin üstteki pay deðerini tutacak deðer
	float botLeftTotal=0;//paydanýn sol tarafýndaki toplama iþleminin sonucunu tutacak deðer
	float botRightTotal=0;//paydanýn sað tarafýndaki toplama iþleminin sonucunu tutacak deðer

	for(i=0;i<LEN;i++){//Ortak olmayan kitaplarý 0 lýyoruz
		if((personA[i]!=0&&personB==0)||(personB[i]!=0&&personA==0)){
			personA[i]=0;
			personB[i]=0;
		}
	}
	for(i=0;i<LEN;i++){//Ýki kiþinin de ortalama puanlarýný buluyoruz
		if(personA[i]!=0&&personB[i]!=0){
			aCount++;
			rA+=personA[i];
			bCount++;
			rB+=personB[i];
		}
	}
	rA/=aCount;
	rB/=bCount;
	for(i=0;i<LEN;i++){//Ortak her kitap için similarity fonksiyonunun toplama iþlemleri
		if(personA[i]!=0&&personB[i]!=0){
			topTotal+=(personA[i]-rA)*(personB[i]-rB);
			botLeftTotal+=(personA[i]-rA)*(personA[i]-rA);
			botRightTotal+=(personB[i]-rB)*(personB[i]-rB);
		}
	}
	botLeftTotal=sqrt(botLeftTotal);//son olarak paydadaki deðerleri kareköke alýyoruz
	botRightTotal=sqrt(botRightTotal);
	return topTotal/(botLeftTotal*botRightTotal);//Dönüþ deðerimiz öneri sayýmýz -1 +1 aralýðýnda
}
void findKSimilars(int personA){//Alýnan personA kiþisine UArray'den en çok benzeyen k tane kiþiyi bulan fonksiyon
	int i=0;
	for(i=0;i<k;i++){
		kList[i]=-2;
	}
	for(i=0;i<20;i++){
		addToKList(similarity(UNArray[personA],UArray[i]),i);
	}
}
void predict(){//mainPersonIndex'deki kiþiye benzeyenlere bakarak kitap öneren ve yazdýran fonksiyon
	int kitapIndex=0; //Kaçýncý kitapta olduðumuzu tutar
	int i;
	int j;
	float temp;
	int tempIndex;
	float Uaverage=0;//Benzeyenler listesindeki kiþinin puan ortalamasýný tutar
	int Ucount=0;//Benzeyenler listesindeki kiþinin okuduðu kitap sayýsý
	float UNaverage=0;//Kitap önereceðimiz kiþinin puan ortalamasý
	int UNcount=0;//kitap önereceðimiz kiþinin okuduðu kitap sayýsý
	float predict=-99;//Ýlk öneri deðerimiz çok düþük bir deðer veriyoruz ki üzerine yazýlsýn
	int predictIndex;//Önerilen kitapýn kitap isimleri listesindeki indeksi
	float top;//Ýþlemimizde pay kýsmýnýn toplamý
	float bottom;//Ýþlemde payda kýsmýnýn toplamý
	for(i=0;i<LEN;i++){//Ýlk olarak ilgili kiþinin ortalama puanýný buluyoruz
		if(UNArray[mainPersonIndex][i]!=0){
			UNcount++;
			UNaverage+=(float)UNArray[mainPersonIndex][i];
		}
	}
	UNaverage/=UNcount;
	
	printf("\n\nNU%d kullanicisina en yakin kullanicilar (k=%d) ve hesaplanan pearson benzerlikleri sirasiyla: ",mainPersonIndex+1,k);
	for (i=0;i<k-1;i++) {  //Hazýr olan k listesimizi sýrayla yazdýracaðýmýz için sýralýyoruz
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

	while(kitapIndex<LEN){//Her bir kitap için inceleyeceðiz
		if(UNArray[mainPersonIndex][kitapIndex]==0){//Kullanýcýnýn okumadýðý kitaplara bakýyoruz
			top=0;
			bottom=0;
			for(i=0;i<k;i++){//Ýþlemin pay kýsmýnda benzediði kiþilerle ilgili iþlemleri yapýyoruz. K benzediði kiþi sayýsý
				
				for(j=0;j<LEN;j++){// Ýlgili kiþinin ortalamasýný buluyoruz
					if(UArray[kIndexList[i]][j]!=0){
						Uaverage+=UArray[kIndexList[i]][j];
						Ucount++;
					}	
				}
				Uaverage/=Ucount;
				//pay ve payda kýsmýndaki toplama iþlemleri her adýmda gerçekleþiyor
				top+=(similarity(UNArray[mainPersonIndex],UArray[kIndexList[i]]))*((float)UArray[kIndexList[i]][kitapIndex]-(float)Uaverage);//sim(a,b)*rbp-rb
				bottom+=similarity(UNArray[mainPersonIndex],UArray[kIndexList[i]]);
				Ucount=0;
				Uaverage=0;

			}
				
			printf("%d. %s , %f\n",kitapIndex,movies[kitapIndex+1],UNaverage+(top/bottom));

			//Bulunan puan deðeri öncekinden yüksekse yeni önereceðimiz kitap mertebesine ulaþmýþ oluyor 
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
		if(splitter<9){//movie isimleri aldýk
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
					fscanf(fa,"%d",&UArray[personIndex][bookIndex]);//U karakterleri U listesine alýyoruz
				}else{
					fscanf(fa,"%d",&UNArray[personIndex%20][bookIndex]);//%20 Olmasýnýn sebebi UNlere geçince bozulmamasý için
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
	
	mainPersonIndex=j-1;//Ýlgili kiþinin indeksi
	findKSimilars(mainPersonIndex);//Benzer kullancýlarý bulup kListe atýyoruz
	predict();//Kitabý öneriyoruz

	return 0;
}
