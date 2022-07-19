#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<my_avl_tree.h>
typedef struct __city
{
int code;
char name[51];
}City;
typedef struct __city_header
{
int lastGeneratedCode;
int recordCount;
}CityHeader;
// global data structures
AVLTree *cities;
AVLTree *citiesByName;
CityHeader cityHeader;
int cityCodeComparator(void *left,void *right)
{
City *leftCity=(City *)left;
City *rightCity=(City *)right;
return leftCity->code-rightCity->code;
}

int cityNameComparator(void *left,void *right)
{
City *leftCity=(City *)left;
City *rightCity=(City *)right;
return stricmp(leftCity->name,rightCity->name);
}

void popluateDataStructure(int *success)
{
FILE *cityFile;
City *city;
City c;
int succ;
if(success) *success=false;
printf("Please Wait, loading data..................\n");
cities=createAVLTree(&succ,cityCodeComparator);
if(!succ)
{
printf("Unable to load data, low memory issue\n");
return;
}
citiesByName=createAVLTree(&succ,cityNameComparator);
if(!succ)
{
printf("Unable to load data, low memory issue\n");
destroyAVLTree(cities);
return;
}
cityFile=fopen("city.dat","rb");
if(cityFile!=NULL)
{
fread(&cityHeader,sizeof(CityHeader),1,cityFile);
if(!(feof(cityFile)))
{
while(1)
{
fread(&c,sizeof(City),1,cityFile);
if(feof(cityFile)) break;
city=(City *)malloc(sizeof(City));
// checking if city is null pending
city->code=c.code;
strcpy(city->name,c.name);
insertIntoAVLTree(cities,(void *)city,&succ);
insertIntoAVLTree(citiesByName,(void *)city,&succ);
}
}
fclose(cityFile);
} // loading data from file ends here
if(success) *success=true;
}
void releaseDataStructure()
{
destroyAVLTree(cities);
destroyAVLTree(citiesByName);
}
void addCity()
{

}
int mainMenu()
{
int ch,succ;
if(!succ) return 0;
while(1)
{
printf("1. City Master\n");
printf("2. Get route\n");
printf("3. Exit\n");
printf("Enter your choice: ");
scanf("%d",&ch);
fflush(stdin);
if(ch<1 || ch >3)
{
printf("Invalid input\n");
}
{
return ch;
}
}
}
void cityMenu()
{
int ch;
while(1)
{
printf("City Master\n");
printf("1. Add\n");
printf("2. Edit\n");
printf("3. Delete\n");
printf("4. Search\n");
printf("5. List\n");
printf("6. Add adjacent vertex\n");
printf("7. Exit\n");
printf("Enter your choice: ");
scanf("%d",&ch);
fflush(stdin);
if(ch<1 || ch>7)
{
printf("Invalid input\n");
continue;
}
if(ch==1) addCity();
if(ch==7) return;
}
}
int main()
{
int ch,succ;
popluateDataStructure(&succ);
while(1)
{
ch=mainMenu();
if(ch==1)
{
cityMenu();
}
if(ch==2)
{
}
if(ch==3) break;
}
releaseDataStructure();
return 0;
}