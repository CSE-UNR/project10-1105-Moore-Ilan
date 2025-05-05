/*
 * Wordup - Project 10, CS135.1001
 * Author: Ilan Moore
 * Description: A Wordle-style guessing game. This program loads a mystery word
 *              from a file and prompts the user to guess it in up to 6 tries.
 *              Correct letters are capitalized; misplaced letters are marked with '^'.
 */
#include <stdio.h>
#include <stdbool.h>

//preset options for the code, up top for easy access incase i need to change file directories or limits, would optimally have them in a seperate settings.txt
#define WORDLIST "word.txt" //defines the wordlist text file as word.txt
#define MAXGUESSES 6 //defines maximum guesses to be 6
#define WORDLENGTH 5 //defines word length to be 5

#define LIMIT 50 //sets a limit for stack overflow cases

// MAIN FUNCTIONS
void processGuess(char guess[], char mysteryWord[], char allGuesses[][WORDLENGTH + 1], int guessIndex);//processes guess
void processPointers(char guess[], char mysteryWord[], char pointers[][WORDLENGTH + 1], int guessIndex);//processes pointers
void displayGuesses(char allGuesses[][WORDLENGTH + 1], char pointers[][WORDLENGTH + 1], int guessIndex);//displays previous guesses
void displayWinMessage(int guessIndex, char allGuesses[][WORDLENGTH + 1]);//displays the win board

//TRUE/FALSE BOOL FUNCTIONS
bool sanitizeGuess(char *guess);//checks if guess is valid
bool isAlpha(char c);//checks if character is part of alphabet

//UTILITIES
bool stringsEqual(char a[], char b[]);
char toLower(char c);//converts to lowercase
char toCaps(char c);//converts to uppercase
void loadWord(char mysteryWord[]);//load word from .txt file

//main function
int main(){
	int guessIndex = 0;//initializes guess count at zero
	bool won = false;//initialize win state to be false
	
	//declare all arrays and strings
	char allGuesses[MAXGUESSES][WORDLENGTH + 1];//declares 2d array for all guesses
	char pointers[MAXGUESSES][WORDLENGTH + 1]; //declares 2d array for guess pointers ^ for misplaced, ' ' otherwise
	char mysteryWord[WORDLENGTH + 1];//declares mysteryWord string
	
	loadWord(mysteryWord);//loads word from text file into mysteryWord

	//start of game loop, this section will repeat asking user for answers until either they win or guesses run out
	while (guessIndex < MAXGUESSES && !won){//guesses < maxguesses since guesses starts at zero
		//declarations and initialization
		char guess[LIMIT];//declares guess string w max size of LIMIT, prevents overflow
		bool validguess = false; //initializes validguess at false
		
		//start of game, since guesses starts at zero we add 1 to display the correct value to user
		printf("GUESS %d! Enter your guess: ", guessIndex + 1);
		while (!validguess){//loops until guess is valid
			scanf("%s", guess);//checks user's input
			
			validguess = sanitizeGuess(guess);//runs isValidGuess function
			
			if (!validguess){//asks user to input answer again if guess is not valid
				printf("\nPlease try again: ");
			}
		}
		printf("================================\n");//screen seperator before loop restarts
			
		processGuess(guess, mysteryWord, allGuesses, guessIndex);//processes guess so the final guess outputs in all caps
		
		
		if(stringsEqual(guess, mysteryWord)){//checks if guess and mystery word are equal
			won = true;//sets win state to true
			displayWinMessage(guessIndex, allGuesses);//displays the win board
			break;//breaks out of first while loop
		}
		
		//if user doesn't win yet, it will process the pointers, and display all guesses
		processPointers(guess, mysteryWord, pointers, guessIndex);
		displayGuesses(allGuesses, pointers, guessIndex);
		
		
		
		guessIndex++;//increase guess index by 1
	}
	
	return 0;
}

//loads in word from file
void loadWord(char mysteryWord[]){
	FILE *file = fopen(WORDLIST, "r");
	if (file == NULL){//checks if file is not found
		printf("Error: Could not open %s\n", WORDLIST);//prints error message
		return;//ends if file is not found
	}

	int i = 0;//declares index
	char c;//declares c
	while (i < WORDLENGTH && (c = fgetc(file)) != EOF && c != '\n'){//loops the length of the word, and makes sure that c isn't end of file or a newline character
		mysteryWord[i++] = toLower(c);//sets c to lowercase incase a letter is uppercase in the WORDLIST file
	}
	mysteryWord[i] = '\0';//appends null character to end of string
	fclose(file);//closes file
}


//MAIN FUNCTIONS


/*processes user's guess, if a letter is correct it will capitalize and put it into the allGuesses 2d array depending on guess index*/
void processGuess(char guess[], char mysteryWord[], char allGuesses[][WORDLENGTH + 1], int guessIndex){
	for(int i = 0; i < WORDLENGTH; i++){//loop for all letters
		if (guess[i] == mysteryWord[i]) {//if guess letter i and mystery letter i are =
			allGuesses[guessIndex][i] = toCaps(guess[i]);//put a capitalized version of it into allGuesses at i
			}
		else {//if not
			allGuesses[guessIndex][i] = guess[i];//put the regular uncapitalized version in
		}
	}
	allGuesses[guessIndex][WORDLENGTH] = '\0';//appends null character to end
}

void processPointers(char guess[], char mysteryWord[], char pointers[][WORDLENGTH + 1], int guessIndex){
	for (int i = 0; i < WORDLENGTH; i++){//for each letter
		pointers[guessIndex][i] = ' ';//initializes with
		if (guess[i] == mysteryWord[i]){//checks if letters match places
			continue;//skips loop to next letter
		}
		for(int j = 0; j < WORDLENGTH; j++){//for each letter of the mystery word
			if (guess[i] == mysteryWord[j] && guess[j] != mysteryWord[j]){//checks if any one of the letters matches and ensures letter is not already in the correct place
				pointers[guessIndex][i] = '^';//places pointer under letter
				break;//breaks out of for j loop
			}
		}
	}
	pointers[guessIndex][WORDLENGTH] = '\0';//appends null to end
}

void displayGuesses(char allGuesses[][WORDLENGTH + 1], char pointers[][WORDLENGTH + 1], int guessIndex){//displays all previous guesses
	for (int i = 0; i < guessIndex + 1; i++){//loop from first guess to the latest guess user put in
		printf("%s\n", allGuesses[i]);//prints guess line i
		printf("%s\n", pointers[i]);//prints pointers for line i
	}
}

void displayWinMessage(int guessIndex, char allGuesses[][WORDLENGTH + 1]){//displays the win message with different responses depending on how many answers it takes
	printf("		%s\n", allGuesses[guessIndex]);
	if (guessIndex == 0){//case if user wins in first guess
		printf("	You won in 1 guess!\n", guessIndex + 1);
		printf("		BAZINGA!\n");//couldn't bring myself to write out the word from the example
	} else {
		printf("	You won in %d guesses!\n", guessIndex + 1);//seperate from first to preserve grammar
		if (guessIndex <= 2){//case if user wins in 2 or 3 tries
			printf("		Amazing!\n");
		} else if (guessIndex <= 4){//case if user wins in 4 or 5 tries
			printf("		Nice!\n");
		}
	}

}

//passthrough function. takes guess converts guess to all lowercase letters, and returns true or false if its valid

bool sanitizeGuess(char *guess){
	int length = 0; //initialize length at zero
	bool onlyalpha = true; //initialize alpha checker bool to true so it doesn't return false
	bool correctlength = true; //initialize length checker bool to true so it doesn't return false
	
	while(guess[length] != '\0'){//loop that stops until null character
		if(!isAlpha(guess[length])){//checks if character is valid
			onlyalpha = false;//sets only alpha to false
		}
		guess[length] = toLower(guess[length]);
		length++;//increases length by 1
	}
		if (length != WORDLENGTH){//checks if length isn't equal to WORDLENGTH
			printf("Your guess must be %d letters long.", WORDLENGTH);//i have the printf functions in here since that's what made the most sense when compared to the provided executable
			correctlength = false;
		}
	if (!onlyalpha){//checks if onlyalpha is false
		printf("Your guess must contain only letters.");//same as above, left it outside the loop so it won't print this statement multiple times for each wrong letter(I think this structuring resembles what i infer the original code to be)
	}
	return onlyalpha && correctlength;
}

//UTILITIES

//checks if character is part of alphabet
bool isAlpha(char c){
	return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z');//returns true if c is lowercase or uppercase
}

//checks if strings are equal
bool stringsEqual(char a[], char b[]){
	for (int i = 0; i < WORDLENGTH; i++){//loop for each letter in word
		if (a[i] != b[i]) return false;//returns function as false if a doesn't match b
	}
	return true;
}

//converts character to lowercase if it's uppercase
char toLower(char c){
	if (c >= 'A' && c <= 'Z')//checks if c is uppercase
		return c + ('a' - 'A');//returns c as lowercase
	return c;
}

//converts character to uppercase if it's lowercase
char toCaps(char c){
	if (c >= 'a' && c <= 'z')//checks if c is lowercase
		return c - ('a' - 'A');//returns c as uppercase
	return c;
}


