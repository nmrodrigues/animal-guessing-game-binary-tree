// CS1337 - HW6 - Nicole Rodrigues
// The following program is designed to
// guess the animal that the user is thinking of.
// It preforms this task by implementing a
// binary tree of struct referenced by pointers.
// Special Feature:
//      Debugging Code - allowa the user to read with the program
#include <iostream>
#include <cctype> // toupper
#include <string> // strings
#include <cstdlib>
#include <fstream>
//#include <conio.h> // _kbhit

using namespace std;
bool showing = false; // used for debugging
string animalFileName_SAVE = "myAnimalTreeDB.txt";

// build the data structure for the tree
struct AnimalNode {
    string guess; // computer's guess of the animak
    string question; // question to ask the user about animal
    AnimalNode *yesAnswer;
    AnimalNode *noAnswer;
};

// prints what the program is doing
void debuggingCode(string programMessage) {
    if (showing) {
        cout << programMessage << endl;
    }
}

// when the user is asked a question, this function will be called to
// recieve input from the user
void getAnswerFromUser(string& userAnswer) {
    cin >> userAnswer;
    // change the input to uppercase for data validation
    for(unsigned int i = 0; i < userAnswer.length(); i++){
        userAnswer[i] = toupper(userAnswer[i]);
        // cout << userAnswer[i] << endl;
    }
    // invalid entry
    if (!(userAnswer == "YES" || userAnswer == "NO")) {
        cout << "Invalid Entry. Please enter \"YES\" or \"NO\"" << endl;
        getAnswerFromUser(userAnswer);
    }
}

// builds a new node on the tree
AnimalNode *newNode(string initialGuess) {
    AnimalNode *newNode = new AnimalNode();
    newNode->guess = initialGuess;
    newNode->question = "";
    newNode->yesAnswer = nullptr;
    newNode->noAnswer = nullptr;
    return newNode;
}

void printTheTree(ostream &outputStream, AnimalNode *currentNode){
    if((currentNode->yesAnswer) == nullptr){
        outputStream << "G" << endl;
        outputStream << currentNode->guess << endl;
    }
    else{
        outputStream << "Q" << endl;
        outputStream << currentNode->question << endl;
        printTheTree(outputStream, currentNode->yesAnswer);
        printTheTree(outputStream, currentNode->noAnswer);
    }
}
void saveAnimalTree(AnimalNode *root){
    ofstream outfile(animalFileName_SAVE);
    while(!outfile){
        cin >> animalFileName_SAVE;
        ofstream outfile(animalFileName_SAVE);
    }
    printTheTree(outfile, root);
    outfile << flush;
}

void readingSavedFile(ostream &outputStream, AnimalNode *currentNode){
   // if(outputStream == "Q"){
   //     getline(outputStream, currentNode ->question);
   //     getline(outputStream,)
   // }
}

// gets input regarding the animal the user was thinking of
void getDataFromUser(string& tempGuess, string& tempQuestion, AnimalNode *currentNode, string& answerForUserAnimal) {
    debuggingCode("Getting input from the user.");
    cout << "What is the name of the animal you were thinking of?" << endl;
    cin >> tempGuess; // name of the animal the user was thinking of
    cin.ignore(); // clears keyboard buffer
    cout << "What is a question that differentiates a " << tempGuess << " from a " << currentNode->guess << "?" << endl;
    getline(cin, tempQuestion); // question to tell the animals apart
    cout << "What is the answer for a(n) " << tempGuess << "?" << endl;
    cin >> answerForUserAnimal;
    // switching answer to uppercase
    for(unsigned int i = 0; i < answerForUserAnimal.length(); i++){
        answerForUserAnimal[i] = toupper(answerForUserAnimal[i]);
        //  cout << answerForUserAnimal[i] << endl;
    }
    // data validation and reinput
    while (!(answerForUserAnimal == "YES" || answerForUserAnimal == "NO")) {
        cout << "Invalid answer. Please enter \"YES\" or \"NO\"" << endl;
        cin >> answerForUserAnimal;
        for(unsigned int i = 0; i < answerForUserAnimal.length(); i++){
            answerForUserAnimal[i] = toupper(answerForUserAnimal[i]);
            // cout << answerForUserAnimal[i] << endl;
        }
    }
}

// prints a message when the animal is found
void animalFoundDisplay() {
    cout << "Yay! The computer found the animal!" << endl;

}

// asks the user if they would like to continue playing
bool tryAgain() {
    debuggingCode("Asking the user if they want to play again.");
    cout << "Would you like to play again? Please enter \"Y\" or \"N\"" << endl;
    char playAgain;
    cin >> playAgain;
    playAgain = toupper(playAgain);
    if (playAgain == 'Y') {
        return true;
    } else {
        return false;
    }
} // tryAgain

void gamePlay(AnimalNode *currentNode) {
    debuggingCode("Playing the game.");
    //debuggingCode("Inside of gamePlay");
    string userAnswer;
    // Does the current node have a question
    if (!currentNode->question.empty()) {
        cout << currentNode->question << endl; // prints the question in the node
        getAnswerFromUser(userAnswer);
        if (userAnswer == "YES") {
            gamePlay(currentNode->yesAnswer); // points to the yes branch
        } else {
            gamePlay(currentNode->noAnswer); // points to the no branch
        }
    } else {
        cout << "Is it a " << currentNode->guess << "?" << endl; // if there isn't a question, it asks the user if it is an animal
        getAnswerFromUser(userAnswer);
        if (userAnswer == "YES") {
            animalFoundDisplay(); // the animal was found
            //tryAgain(); // asks if want to play again
        } else { // if answer was no
            string tempGuess; // user animal
            string tempQuestion; // user question
            string answerForUserAnimal; // answer for the question
            getDataFromUser(tempGuess, tempQuestion, currentNode, answerForUserAnimal); // gets animal, question, and yes or no answer
            AnimalNode *yesNode;
            AnimalNode *noNode;
            if (answerForUserAnimal == "YES") {
                yesNode = newNode(tempGuess); // puts the temp quess in
                noNode = newNode(currentNode->guess);
            } else if (answerForUserAnimal == "NO") {
                yesNode = newNode(currentNode->guess);
                noNode = newNode(tempGuess); // puts the temp quess in
            }
            currentNode->guess = ""; // clears the guess
            currentNode->question = tempQuestion; // puts the question in the current node
            currentNode->yesAnswer = yesNode; // puts the yes node in the yes answer
            currentNode->noAnswer = noNode; // puts the no node in the no answer
            // This gameplay finishes here
        }
    }
    saveAnimalTree(currentNode);
    // Get input from user

} // gamePlay

//void readTheFile(ostream &outStream, AnimalNode* currentNode) {
//    char guessOrQuestion = "";
//    string guess = "";
//    string question = "";
//    while(!outputStream.eof()){
//        outStream >> guessOrQuestion;
//        if(guessOrQuestion == 'G'){
//            getline(outStream, guess);
//            currentNode -> guess = guess;
//        }
//    }
//}


int main() {
    AnimalNode *root = newNode("Lizard");
    cout << "Welcome to the Animal Guessing Game!" << endl;
    cout << "Press the ENTER key to begin the game" << endl;
  //  while(!_kbhit()){
  //      debuggingCode("Waiting for user to hit the keyboard.");
   // }
    cin.ignore();
    do {
        cout << "Please think of an animal that you would like the computer to guess." << endl;
        gamePlay(root);
    } while (tryAgain()); // while the user still wants to play the game
    printTheTree(cout, root);
    return 0;
}