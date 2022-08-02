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
fclose(cityFile);
}
}
if(getSizeOfAVLTree(cities)==0)
{
cityHeader.lastGeneratedCode=1;
cityHeader.recordCount=1;
}
if(success) *success=true;
}
void releaseDataStructure()
{
// pending memory leak issue.
destroyAVLTree(cities);
destroyAVLTree(citiesByName);
}
void printLine(char c,int numberOfTimes)
{
int i;
for(i=0;i<numberOfTimes;i++) printf("%c",c);
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
printf("%s is already Exist\n",c.name);
return;
}
printf("Press (Y/y) to save %s: ",name);
m=getchar();
fflush(stdin);
if(m!='Y' && m!='y')
{
printf("%s is not Added\n",name);
return;
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
printf("%s is Added\n",name);
printf("Press any key to continue..................\n");
getchar();
fflush(stdin);
}
void editCity()
{
City *city,*city2;
FILE *cityFile;
CityHeader vHeader;
City c;
char m;
int succ,position;
char name[52],newName[52];
printf("City (Edit Module)\n");
printf("Enter name of city to edit: ");
fgets(name,52,stdin);
fflush(stdin);
name[strlen(name)-1]='\0';
strcpy(c.name,name);
city=(City *)getFromAVLTree(citiesByName,(void *)&c,&succ);
if(!succ)
{
printf("City %s, does not exist, press enter to continue....",name);
getchar();
fflush(stdin);
return;
}
printf("City: %s\n",city->name);
printf("Edit (Y/N): ");
m=getchar();
fflush(stdin);
if(m!='y' && m!='Y')
{
printf("City %s, not updated, press enter to continue.....",name);
getchar();
fflush(stdin);
return;
}
printf("Enter new name of city: ");
fgets(newName,52,stdin);
newName[strlen(newName)-1]='\0';
if(strcmp(name,newName)!=0)
{
strcpy(c.name,newName);
city2=(City *)getFromAVLTree(citiesByName,(void *)&c,&succ);
if(succ && city->code!=city2->code)
{
printf("City %s exists, press enter to continue.....",newName);
getchar();
fflush(stdin);
return;
}
}
printf("Update (Y/N): ");
m=getchar();
fflush(stdin);
if(m!='y' && m!='Y')
{
printf("City %s not updated, press enter to continue......",name);
getchar();
fflush(stdin);
return;
}
cityFile=fopen("city.dat","rb+");
fread(&vHeader,sizeof(CityHeader),1,cityFile);
while(1)
{
position=ftell(cityFile);
fread(&c,sizeof(City),1,cityFile);
if(feof(cityFile)) break;
if(strcmp(city->name,c.name)==0)
{
fseek(cityFile,position,SEEK_SET);
c.code=city->code;
strcpy(c.name,newName);
fwrite(&c,sizeof(City),1,cityFile);
break;
}
}
fclose(cityFile);
strcpy(c.name,city->name);
c.code=city->code;
removeFromAVLTree(citiesByName,(void *)&c,&succ);
removeFromAVLTree(cities,(void *)&c,&succ);
strcpy(city->name,newName);
insertIntoAVLTree(cities,(void *)city,&succ);
insertIntoAVLTree(citiesByName,(void *)city,&succ);
printf("City %s updated to city %s\n",name,newName);
printf("Press enter to continue.................");
getchar();
fflush(stdin);
} // function end
void removeCity()
{
City *city;
FILE *cityFile,*tmpFile;
CityHeader vHeader;
City c;
char m;
int succ;
char name[52];
printf("City (Delete Module)\n");
printf("Enter name of city to delete: ");
fgets(name,52,stdin);
fflush(stdin);
name[strlen(name)-1]='\0';
strcpy(c.name,name);
city=(City *)getFromAVLTree(citiesByName,(void *)&c,&succ);
if(!succ)
{
printf("City %s, does not exist, press enter to continue.....",name);
getchar();
fflush(stdin);
return;
}
printf("City: %s\n",city->name);
printf("Delete (Y/N): ");
m=getchar();
fflush(stdin);
if(m!='y' && m!='Y')
{
printf("City %s, not removed, press enter to continue.....",name);
getchar();
fflush(stdin);
}
cityFile=fopen("city.dat","rb");
tmpFile=fopen("tmp.tmp","wb");
fread(&vHeader,sizeof(CityHeader),1,cityFile);
cityHeader.recordCount--;
fwrite(&cityHeader,sizeof(CityHeader),1,tmpFile);
while(1)
{
fread(&c,sizeof(City),1,cityFile);
if(feof(cityFile)) break;
if(strcmp(city->name,c.name)!=0)
{
fwrite(&c,sizeof(City),1,tmpFile);
}
}
fclose(tmpFile);
fclose(cityFile);
cityFile=fopen("city.dat","wb");
tmpFile=fopen("tmp.tmp","rb");
fread(&vHeader,sizeof(CityHeader),1,tmpFile);
fwrite(&vHeader,sizeof(CityHeader),1,cityFile);
while(1)
{
fread(&c,sizeof(City),1,tmpFile);
if(feof(tmpFile)) break;
fwrite(&c,sizeof(City),1,cityFile);
}
fclose(cityFile);
fclose(tmpFile);
tmpFile=fopen("tmp.tmp","w");
fclose(tmpFile);
c.code=city->code;
strcpy(c.name,city->name);
removeFromAVLTree(citiesByName,(void *)&c,&succ);
city=(City *)removeFromAVLTree(cities,(void *)&c,&succ);
free(city);
printf("City %s removed, press enter to continue",name);
getchar();
fflush(stdin);
}
void searchCity()
{
City c;
City *city;
char name[52];
int succ;
printf("Enter city name: ");
fgets(name,52,stdin);
fflush(stdin);
name[strlen(name)-1]='\0';
strcpy(c.name,name);
city=(City *)getFromAVLTree(citiesByName,(void *)&c,&succ);
if(!succ)
{
printf("%s not found\n",name);
return;
}
printf("%s is found\n",name);
printf("Press any key to continue...................\n");
getchar();
fflush(stdin);
}
void displayListOfCities()
{
int succ,serialNumber;
City *city;
AVLTreeInOrderIterator it;
printf("City (Display Module)\n");
if(getSizeOfAVLTree(cities)==0)
{
printf("No Cities added\n");
return;
}
it=getAVLTreeInOrderIterator(cities,&succ);
serialNumber=0;
while(hasNextInOrderElementInAVLTree(&it))
{
printLine('-',86);
printf("Serial Number | City Name\n");
printLine('-',86);
printf("\n");
while(hasNextInOrderElementInAVLTree(&it))
{
city=(City *)getNextInOrderElementFromAVLTree(&it,&succ);
printf("%6d",serialNumber+1);
printf("%9c ",'|');
printf("%s\n",city->name);
serialNumber++;
if(serialNumber%5==0) break;
}
printf("Press any key to Continue................\n");
getchar();
fflush(stdin);
}
/*
	Note : Use the logic applied in displayListOfStudents
	after every 15 record, press enter to continue message
	pause with press enter to continue message after evry page
	as well as in the end
*/
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
else if(ch==2) editCity();
else if(ch==3) removeCity();
else if(ch==4) searchCity();
else if(ch==5) displayListOfCities();
else if(ch==7) return;
}
}
int main()
{
int ch,succ;
populateDataStructure(&succ);
while(1)
{
ch=mainMenu();
if(ch==1) cityMenu();
if(ch==3) break;
}
releaseDataStructure();
return 0;
}