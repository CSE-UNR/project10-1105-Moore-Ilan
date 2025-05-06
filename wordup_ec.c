/*
 * Wordup extra credit - Project 10, CS135.1001
 * Author: Ilan Moore
 * For the extra credit section I have decided to make the code compatible with words that have multiple of the same letters.
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
		if (guessIndex == MAXGUESSES - 1){//if user is at last guess it will display this message
		printf("FINAL GUESS : ");
		} else{//normally displays next message
		printf("GUESS %d! Enter your guess: ", guessIndex + 1);
		}
		
		
		while (!validguess){//loops until guess is valid
			scanf("%s", &guess);//checks user's input
			
			validguess = sanitizeGuess(guess);//runs isValidGuess function
			
			if (!validguess){//asks user to input answer again if guess is not valid
				printf("\nPlease try again: ");
			}
		}
		printf("================================\n");
			
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
	if (!won){//checks if user has not one once loop is over
		printf("You lost, better luck next time!\n");//losing message
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
	while (i < WORDLENGTH && (c = fgetc(file)) != '\0' && c != '\n'){//loops the length of the word, and makes sure that c isn't end of file or a newline character
		mysteryWord[i++] = toLower(c);//sets c to lowercase incase a letter is uppercase in the WORDLIST file
	}
	mysteryWord[i] = '\0';//appends null character to end of string
	fclose(file);//closes file
}


//MAIN FUNCTIONS


/*processes user's guess, if a letter is correct it will capitalize and put it into the allGuesses 2d array depending on guess index, this time i've decided to make sure words with matching letters work*/
void processGuess(char guess[], char mysteryWord[], char allGuesses[][WORDLENGTH + 1], int guessIndex) {
	bool matched[WORDLENGTH] = {false}; // track which letters have been matched with a boolean array, initialized at false

	for (int i = 0; i < WORDLENGTH; i++) {//loop for all letters
		if (guess[i] == mysteryWord[i]) {//if guess letter i and mystery letter i are =
			allGuesses[guessIndex][i] = toCaps(guess[i]);//put a capitalized version of it into allGuesses at i
			matched[i] = true; // mark this letter as matched
		} else {
			allGuesses[guessIndex][i] = guess[i]; // lowercase by default
		}
	}
	allGuesses[guessIndex][WORDLENGTH] = '\0';//appends null to end
}

void processPointers(char guess[], char mysteryWord[], char pointers[][WORDLENGTH + 1], int guessIndex) {
	bool matched[WORDLENGTH] = {false}; // track used letters in mysteryWord

	// first pass: match correct positions
	for (int i = 0; i < WORDLENGTH; i++) {//loop for all letters
		if (guess[i] == mysteryWord[i]) {//checks if letters match places
			matched[i] = true; //sets letter to matched
			pointers[guessIndex][i] = ' '; // correct letters are capitalized, no ^
		} else {
			pointers[guessIndex][i] = ' '; // initialize pointer
	}
}

	// Second pass: match misplaced letters
	for (int i = 0; i < WORDLENGTH; i++) {
		if (guess[i] == mysteryWord[i]) continue; // Already matched

		for (int j = 0; j < WORDLENGTH; j++) {//loop for all letters
			if (!matched[j] && toLower(guess[i]) == toLower(mysteryWord[j])) {//if the letter is flagged as not already matching and the letter is found in the mystery word
				pointers[guessIndex][i] = '^';
				matched[j] = true; // Mark this mystery letter as used, prevents letter from being flagged too many times
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


