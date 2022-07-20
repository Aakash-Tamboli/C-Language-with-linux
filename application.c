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

void populateDataStructure(int *success)
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
cityFile=fopen("city.dat","rb+");
if(cityFile!=NULL)
{
printf("files is opened\n");
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
fclose(cityFile);
}
}
printf("%d",1);
if(getSizeOfAVLTree(cities)==0)
{
cityHeader.lastGeneratedCode=0;
cityHeader.recordCount=0;
}
printf("%d",2);
if(success) *success=true;
printf("%d",3);
printf("Populate Data Structure is Complete\n");
}
void releaseDataStructure()
{
destroyAVLTree(cities);
destroyAVLTree(citiesByName);
}
void addCity()
{
City c;
City *city;
FILE *cityFile;
char m;
char name[52];
int succ;
printf("City (Add Module)\n");
printf("Enter City Name: ");
fgets(name,52,stdin);
fflush(stdin);
name[strlen(name)-1]='\0';
strcpy(c.name,name);
city=(City *)getFromAVLTree(citiesByName,(void *)&c,&succ);
if(city!=NULL)
{
printf("%s is already Exist\n");
return;
}
printf("Press (Y/y) to save %s: ",name);
m=getchar();
fflush(stdin);
if(m!='Y' || m!='y')
{
printf("%s is not Added\n",name);
}
cityFile=fopen("city.dat","rb+");
if(cityFile==NULL)
{
cityFile=fopen("city.dat","wb+");
fwrite(&cityHeader,sizeof(CityHeader),1,cityFile);
}
else
{
fseek(cityFile,0,SEEK_END);
}
c.code=cityHeader.lastGeneratedCode+1;
fwrite(&c,sizeof(City),1,cityFile);
cityHeader.lastGeneratedCode++;
cityHeader.recordCount++;
fseek(cityFile,0,SEEK_SET);
fwrite(&cityHeader,sizeof(CityHeader),1,cityFile);
fclose(cityFile);
city=(City *)malloc(sizeof(City));
// if city is NULL then code is pending
city->code=c.code;
strcpy(city->name,c.name);
insertIntoAVLTree(cities,(void *)city,&succ);
insertIntoAVLTree(citiesByName,(void *)city,&succ);
// if insertion is avl tree is failed then implementation is pending
printf("%s is Added\n");
getchar();
fflush(stdin);
}
void displayListOfCities()
{
City *city;
printf("City (Display Module)\n");
if(getSizeOfAVLTree(cities)==0)
{
printf("No Cities added\n");
return;
}
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
if(ch==5) displayListOfCities();
if(ch==7) return;
}
}
int main()
{
int ch,succ;
populateDataStructure(&succ);
if(succ==true) printf("Data Structure is loaded\n");
while(1)
{
ch=mainMenu();
if(ch==1) cityMenu();
// if(ch==2) 
if(ch==3) break;
}
releaseDataStructure();
return 0;
}