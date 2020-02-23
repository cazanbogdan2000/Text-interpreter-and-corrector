//CAZAN BOGDAN MARIAN 313CB
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "task_helper.h"

//****Pentru simplitate, variabila s din functiile programului reprezinta poezia 
//****citita de functia din task helper, load_poem, anume bufferul

//Functia de uppercase verifica daca, caracterele de la inceputul versurilor
//sunt majuscule(fiind sigur litere). Daca nu, le face mari. Pentru restul
//literelor, le face mici daca nu sunt, altfel le lasa.
void uppercase(char *s){
	
	int n = strlen(s);
	
	if(s[0] >='a' && s[0] <= 'z'){
		s[0] = s[0] - 32;
	}

	for(int i = 1; i <= n; i++){
		if(s[i-1] == '\n'){
			if(s[i] >= 'a' && s[i] <= 'z'){
				s[i] = s[i] - 32;
			}
		}
		else if(s[i] >= 'A' && s[i] <= 'Z'){
			s[i] = s[i] + 32;
		}
	}
}

//Functie pentru a elimina caracterele nonalfabetice din poezie, dar si pentru
//whitespace-urile in plus
void trimming(char *s){
	
	int i = 1, n = strlen(s);
	//cautam caracterele nonalfabetice, si le eliminam(ca la vectori)
	while(i <= n){
		if(strchr(".,;!?:", s[i]) != NULL){
			for(int j = i; j <= n; j++){
				s[j] = s[j+1];
			}
			n--;
		}
		//verificarea space-urilor
		else if(s[i] == ' ' && s[i+1] == ' '){
			for(int j = i; j <= n; j++){
				s[j] = s[j+1];
			}
			n--;
		}
		else{ 
			i++;
		}
	}
}

//functia specifica cerintei 3
void make_it_silly(char *s){
	
	char *prob = calloc(100 , sizeof(char));;
	int n = strlen(s);
	float probabiliy;

	scanf("%s",prob);
	//transformarea probabilitatii din tip char in tip float # stdlib.h
	probabiliy = atof(prob);
	
	for (int i = 0; i < n; i++){	
		if(s[i] >='a' && s[i]<='z'){
			float sample = rand() % 100 / 99.0;
			if(sample < probabiliy){
				s[i] = s[i] - 32;
			}
		}
		else if(s[i] >= 'A' && s[i] <= 'Z'){
			float sample = rand() % 100 / 99.0;
			if(sample < probabiliy){
				s[i] = s[i] + 32;	
			}
		}
	}

	free(prob);
}

//functie care inlocuieste cuvinte cu diminutivele lor, daca exista, folosind 
//in acelasi timp functia din task_helper, get_friendly_word
void friendly(char *s){
	char *word;
	char *copy_poem = calloc(10000 , sizeof(char));
	char *standard_copy_poem = strdup(s);
	char *friendly_word = calloc(500 , sizeof(char));
	int position_std_copy = 0;
	int position_poem = 0;
	//facem copie poeziei, pe care o spargem cu strtok in cuvinte, pentru
	//a verifica daca exista diminutiv pentru cuvant
	//pentru a inlocui, stergem cuvantul initial si punem diminutivul
	word = strtok(s," \n,;:.!?");
	while(word != NULL){
		
		get_friendly_word(word,&friendly_word);	
		
		if(friendly_word != NULL){
			int lenght_friendly_word = strlen(friendly_word);
			char cuvant[50];
			strcpy(cuvant, friendly_word);
		
			for(int i = 0; i < lenght_friendly_word; i++){		
				copy_poem[position_poem] = cuvant[i];
				position_poem++;
			}

			position_std_copy += strlen(word);
		}
		else {
			for(int i = 0; i < strlen(word); i++){	
				copy_poem[position_poem] = word[i];
				position_poem++;
				position_std_copy++;
			}	
		}

		while(strchr(" \n,;:.!?", standard_copy_poem[position_std_copy])){
			copy_poem[position_poem] = standard_copy_poem[position_std_copy];
			position_poem++;
			position_std_copy++;
		}

		word = strtok(NULL," \n,;:.!?");
	}

	strcpy(s,copy_poem);
	free(standard_copy_poem);
	free(copy_poem);
	free(friendly_word);
}

//IDEEA FOLOSITA IN REZOLVAREA CERINTEI 5 A FOST DE A SPARGE POEZIA IN VERSURI,
//DE A LUA CATE 4 VERSURI DINTR-O STROFA, SI A LUA ULTIMELE CUVINTE DIN
//VERSURILE RESPECTIVE, VERFICAND DACA RIMEAZA(ULTIMELE LITERE IDENTICE)
//SE OBSERVA CA ULTIMUL CUVANT POATE FI URMAT DE ANUMITE CARACTERE NONALFABETICE
//DECI, ULTIMA LITERA DIN VERS NU ESTE SI ULTIMUL CARACTER DIN VERS


//functie care verifica daca un caracter este litera. Daca este litera mare,
// o transforma in litera mica, pentru a o compara dupa cu alta litera
//**este folosita in functia formare_rima
char is_letter(char last_letter, char **versuri, int ok, int i){

	last_letter = versuri[i][strlen(versuri[i]) - ok - 1];				
	
	if(last_letter >='A' && last_letter <= 'Z'){
		last_letter += 32;
	}

	return last_letter;
}

//functie care returneaza o variabila ok, cu rolul de a numara cate caractere
//nonalfabetice s-au gasit la sfarsitul unui vers, pana la gasirea ultimei
//litere, care de altfel da rima; folosit in formare_rima
int is_ok(char last_letter, char **versuri, int ok, int i){

	while(!(last_letter >='a' && last_letter <= 'z' 
		|| last_letter >='A' && last_letter <= 'Z')){
			ok++;
			last_letter = versuri[i][strlen(versuri[i]) - ok - 1];				
	}
			
	if(last_letter >='A' && last_letter <= 'Z'){
		last_letter += 32;
	}
	//daca s-a gasit din prima o litera la sfarsitul versului, ok va fi 0
	//daca s-au gasit 3 puncte inainte de ultima litera, ok=3 
	return ok;
}

//functie care sparge o copie a unui vers in cuvinte, luand ultimele doua
//cuvinte din versurile care ar trebui sa rimeze, in functie de rima data
//este folosita in functia rhyme_test; versurile sunt i si j
void last_words(char **versuri, char *last_word_1, char *last_word_2,
	char *new_last_word_1, char *new_last_word_2, int i, int j){

	char copie_vers[1000], *token;
	strcpy(copie_vers, versuri[i]);
	
	token = strtok(copie_vers, " ,!?;.:");
	while(token != NULL){
		strcpy(last_word_1, token);
		token = strtok(NULL, " ,!?;:.");
	}
	
	strcpy(copie_vers,versuri[j]);
	
	token = strtok(copie_vers, " ,!?;.:");
	while(token != NULL){
		strcpy(last_word_2, token);
		token = strtok(NULL, " ,!?;:.");
	}
	
	strcpy(new_last_word_1,last_word_1);
	strcpy(new_last_word_2,last_word_2);
}

//dupa cum spune si numele, functia da sinonimele potrivite
//-->asta inseamna ca, daca un cuvant are o lista de sinonime, atunci va fi ales
//cel care poate face rima potrivita
//-->daca exista mai multe asfel de cuvinte in lista data prin apelul functiei,
//get_synonym, se va lua cel mai mic lexicografic; FOLOSITA IN 	rhyme_test
int good_synonyme(char *last_word_1, char *last_word_2, char **list_synonym,
	char *new_last_word_1, char *new_last_word_2, int *lenght_1, int *lenght_2,
	char last_letter_1, char last_letter_2){
	//acest ok are rolul de a verifica daca s-a gasit un cel putin un sinonim
	//cuvantul s-a gasit: ok=1 in primul vers; ok=2 al doilea vers; altfel ok=0
	int ok = 0;
	get_synonym(last_word_1, lenght_1, &list_synonym);
				
	if(list_synonym != NULL){
		for(int j = 0; j < *lenght_1; j++){
			if(list_synonym[j][strlen(list_synonym[j])-1] == last_letter_2){
				ok = 1;
				strcpy(new_last_word_1,list_synonym[j]);
				break;
			}
		}
		
		for(int j = 0; j < *lenght_1; j++){
			if(list_synonym[j][strlen(list_synonym[j]) - 1] == last_letter_2){
				if(strcmp(new_last_word_1,list_synonym[j]) >= 0){
					strcpy(new_last_word_1,list_synonym[j]);
				}
			}
		}
	}

	if( ok == 0 ){
		get_synonym(last_word_2,lenght_2,&list_synonym);

		if(list_synonym != NULL){
			for(int j = 0; j < *lenght_2; j++){
				if(list_synonym[j][strlen(list_synonym[j])-1] == last_letter_1){
					ok = 2;
					strcpy(new_last_word_2,list_synonym[j]);
					break;
				}
			}

			for(int j = 0; j < *lenght_2; j++){
				if(list_synonym[j][strlen(list_synonym[j]) - 1]
				== last_letter_1){
					if(strcmp(new_last_word_2,list_synonym[j]) >= 0){
						strcpy(new_last_word_2,list_synonym[j]);
					}
				}
			}
		}
	}
	//functia returneaza ok, folosit in functia rhyme_test
	return ok;
}

//functie care, in functie de valorile lui ok, ok1, ok2, ok3, ok4
//ok-> este acelasi ca cel din functia de mai sus
//ok1,ok2,ok3,ok4->corespund ok-ului returnat din functia is_ok, deci numara in
//cate caractere nonalfabetice se termina versul
void ok_condition(int ok, int ok1, int ok2, int i, int j, char **versuri,
	char *last_word_1, char *last_word_2, char *new_last_word_1,
	char *new_last_word_2){
	//daca pentru primul cuvant s-a gasit un sinonim valid, atunci formam 
	//versurile nou formate, necesare formarii noii poezii 
	if(ok == 1){
		if( ok1 != 0){
			char *point = calloc(ok1,sizeof(char));

			for(int k = ok1 - 1; k >= 0; k--){
				point[k] = versuri[i][strlen(versuri[i]) - 1 - k];
			}

			strcpy(versuri[i] + strlen(versuri[i]) - ok1 - strlen(last_word_1),
			versuri[i] + strlen(versuri[i]));
			
			strcat(versuri[i], new_last_word_1);
			strcat(versuri[i], point);
			strcat(versuri[i], "\n");
			strcat(versuri[j], "\n");
			free(point);
		}
		else{
			strcpy(versuri[i] + strlen(versuri[i]) - strlen(last_word_1),
			versuri[i] + strlen(versuri[i]));
								
			strcat(versuri[i], new_last_word_1);
			strcat(versuri[i], "\n");
			strcat(versuri[j], "\n");

		}
	}//daca s-a gasit sinonim valid in cel de-al doilea vers ---"---"---"---
	else if(ok == 2){
		if( ok2 != 0){		
			char *point = calloc(ok2,sizeof(char));

			for(int k = ok2 - 1; k >= 0; k--){
				point[k] = versuri[j][strlen(versuri[j]) - 1 - k];
			}

			strcpy(versuri[j] + strlen(versuri[j]) - ok2 - strlen(last_word_2),
			versuri[j] + strlen(versuri[j]));
			
			strcat(versuri[i],"\n");
			strcat(versuri[j],new_last_word_2);
			strcat(versuri[j],point);
			strcat(versuri[j],"\n");
			free(point);
		}
		else{
			strcpy(versuri[j] + strlen(versuri[j]) - strlen(last_word_2),
			versuri[j] + strlen(versuri[j]));
			
			strcat(versuri[i],"\n");
			strcat(versuri[j],new_last_word_2);
			strcat(versuri[j],"\n");
		}
	}
	else{
		strcat(versuri[i],"\n");
		strcat(versuri[j],"\n");
	}
}

//unifica functiile de mai sus, verifcand daca doua versuri rimeaza, in functie
//de rima data, lasand versurile intacte daca rimeaza, altfel cauta sinonime
//si formeaza noile versuri; daca nu se poate realiza rima, atunci versurile
//raman la fel
void rhyme_test(char last_letter_1, char last_letter_2,
	int i, int j, int ok1, int ok2, char **versuri, char *last_word_1,
	char *last_word_2, char *new_last_word_1, char *new_last_word_2,
	char **list_synonym, int *lenght_1, int *lenght_2){

	if (last_letter_1 != last_letter_2){
		int ok = 0;		
				
		last_words(versuri, last_word_1, last_word_2, new_last_word_1,
		new_last_word_2, i, j);

		ok = good_synonyme(last_word_1, last_word_2, list_synonym,
		new_last_word_1, new_last_word_2, lenght_1, lenght_2, last_letter_1,
		last_letter_2);

		ok_condition(ok, ok1, ok2, i, j, versuri, last_word_1, last_word_2,
		new_last_word_1, new_last_word_2);

	}
	else{
		strcat(versuri[i],"\n");
		strcat(versuri[j],"\n");
	}
}

//aceasta este functia care, in functie de rima ceruta, apeleaza functiile
//necesare pentru formarea NOILOR VERSURI
void rhyme_checker(char last_letter_1, char last_letter_2, char last_letter_3,
	char last_letter_4, int i, int ok1, int ok2, int ok3, int ok4,
	char **versuri, char *last_word_1, char *last_word_2, char *new_last_word_1,
	char *new_last_word_2, char **list_synonym,
	int *lenght_1, int *lenght_2, char *rhyme_type){

	if(strcmp(rhyme_type,"imperecheata") == 0){
		rhyme_test(last_letter_1, last_letter_2, i, i+1, ok1, ok2,
		versuri, last_word_1, last_word_2, new_last_word_1, new_last_word_2,
		list_synonym, lenght_1, lenght_2);

		rhyme_test(last_letter_3, last_letter_4, i+2, i+3, ok3, ok4,
		versuri, last_word_1, last_word_2, new_last_word_1, new_last_word_2,
		list_synonym, lenght_1, lenght_2);		
	}

	else if(strcmp(rhyme_type,"incrucisata") == 0){
		rhyme_test(last_letter_1, last_letter_3, i, i+2, ok1, ok3,
		versuri, last_word_1, last_word_2, new_last_word_1, new_last_word_2,
		list_synonym, lenght_1, lenght_2);

		rhyme_test(last_letter_2, last_letter_4, i+1, i+3, ok2, ok4,
		versuri, last_word_1, last_word_2, new_last_word_1, new_last_word_2,
		list_synonym, lenght_1, lenght_2);
	}

	else if(strcmp(rhyme_type,"imbratisata") == 0){
		rhyme_test(last_letter_1, last_letter_4, i, i+3, ok1, ok4,
		versuri, last_word_1, last_word_2, new_last_word_1, new_last_word_2,
		list_synonym, lenght_1, lenght_2);

		rhyme_test(last_letter_2, last_letter_3, i+1, i+2, ok2, ok3,
		versuri, last_word_1, last_word_2, new_last_word_1, new_last_word_2,
		list_synonym, lenght_1, lenght_2);
	}
}

//Functie de eliberare a memoriei alocate dinamic
void eliberare_memorie(char *last_word_1, char *last_word_2,
	char *new_last_word_1, char *new_last_word_2, char **list_synonym){
	
	for(int i = 0; i < 100; i++){
		if(list_synonym[i] != NULL){
			free(list_synonym[i]);
		}
	}
	free(list_synonym);
	free(last_word_1);
	free(last_word_2);
	free(new_last_word_1);
	free(new_last_word_2);
}



//functia in care sunt declarate variabilele necesare realizarii cerintei 5,
//prin apelul functiei care reda versurile noii poezii, care vor fi unite
//intr-o singura poezie, cea finala
void creare_poezie(char **versuri, int number_lines, char *s, char *rhyme_type){
	
	char *final_poem = calloc(5000, sizeof(char));

	for(int i = 0; i < number_lines-1; i++){
		if((i%4 == 0 && (i+1)%4 == 1 && (i+2)%4 == 2 && (i+3)%4 == 3)){
			if(i%4 == 0 && i > 0){
				strcat(final_poem,"\n");
			}
			
			int ok1 = 0, ok2 = 0, ok3 = 0, ok4 = 0;
			int *lenght_1 = calloc(1, sizeof(int));
			int *lenght_2 = calloc(1, sizeof(int));

			char last_letter_1 = versuri[i][strlen(versuri[i]) - 1];
			char last_letter_2 = versuri[i+1][strlen(versuri[i+1]) - 1];
			char last_letter_3 = versuri[i+2][strlen(versuri[i+2]) - 1];
			char last_letter_4 = versuri[i+3][strlen(versuri[i+3]) - 1];
			char *last_word_1 = calloc(100, sizeof(char));
			char *last_word_2 = calloc(100, sizeof(char));
			char *new_last_word_1 = calloc(100, sizeof(char));
			char *new_last_word_2 = calloc(100, sizeof(char));
			char **list_synonym = calloc(100, sizeof(char *));

			for(int j = 0; j < 100; j++){
				list_synonym[j] = calloc(100, sizeof(char));
			}

			ok1 = is_ok(last_letter_1, versuri, ok1, i);
			ok2 = is_ok(last_letter_2, versuri, ok2, i+1);
			ok3 = is_ok(last_letter_3, versuri, ok3, i+2);
			ok4 = is_ok(last_letter_4, versuri, ok4, i+3);

			last_letter_1 = is_letter(last_letter_1, versuri, ok1, i);
			last_letter_2 = is_letter(last_letter_2, versuri, ok2, i+1);
			last_letter_3 = is_letter(last_letter_3, versuri, ok3, i+2);
			last_letter_4 = is_letter(last_letter_4, versuri, ok4, i+3);
			
			rhyme_checker(last_letter_1, last_letter_2, last_letter_3,
			last_letter_4, i, ok1, ok2, ok3, ok4, versuri, last_word_1,
			last_word_2, new_last_word_1, new_last_word_2,
			list_synonym, lenght_1, lenght_2, rhyme_type);

			strcat(final_poem, versuri[i]);
			strcat(final_poem, versuri[i+1]);
			strcat(final_poem, versuri[i+2]);
			strcat(final_poem, versuri[i+3]);

			eliberare_memorie(last_word_1, last_word_2, new_last_word_1,
			new_last_word_2, list_synonym);
		}

	}

	strcpy(s, final_poem);
	s[strlen(s) - 1] = '\0';
	free(final_poem);
}

//functia centrala a cerintei, unde se sparge poezia in versuri si se citeste
//tipul de rima
void rhimy(char *s){

	char *rhyme_type = calloc(30, sizeof(char));
	char **versuri = calloc(100, sizeof(char *));
	char *token;
	int number_lines = 0;
	
	for(int i = 0; i < 100; i++){
		versuri[i] = calloc(1000,sizeof(char));
	}

	token = strtok(s,"\n");	
	while(token != NULL){
		strcpy(versuri[number_lines], token);
		number_lines++;
		token = strtok(NULL,"\n");
	}

	scanf("%s",rhyme_type);
	creare_poezie(versuri, number_lines, s, rhyme_type);
	
	for(int i = 0; i < number_lines; i++){
		free(versuri[i]);
	}

	free(rhyme_type);
	free(versuri);
}

//Consola intercatciva a temei, cerinta 0, care, in functie de selectie,
//va rezolva unul dintre cerintele date(cat timp nu se introduce "quit")
void interactive_console(){

	int close = 1;
	char command[100], *poem = calloc(10000 , sizeof(char));

	while(close != 0){
		scanf("%s", command);

		if(strcmp(command,"load") == 0){
			char *path = malloc(100*sizeof(char));
			scanf("%s",path);

			load_file(path,poem);
				
			free(path);
		}
		else if(strcmp(command,"uppercase") == 0){
			uppercase(poem);
		}
		else if(strcmp(command,"trimming") == 0){
			trimming(poem);
		}
		else if (strcmp(command,"make_it_silly") == 0){
			make_it_silly(poem);
		}
		else if(strcmp(command,"make_it_friendlier") == 0){
			friendly(poem);
		}
		else if(strcmp(command,"make_it_rhyme") == 0){
			rhimy(poem);			
		}
		else if(strcmp(command,"print") == 0){
			printf("%s\n\n",poem);
		}
		else if(strcmp(command,"quit") == 0){
		close = 0;
		}
	}
	free(poem);
}

int main(void){

	srand(42);
	interactive_console();
	return 0;
}