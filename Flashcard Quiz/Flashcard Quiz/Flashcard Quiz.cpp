#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <cstdlib>
#include <ctime>
#include <algorithm>

using namespace std;

struct Flashcard {
    string question;
    string answer;
};

// Recursive function that gets the questions and answers from the user.
void inputFlashcards(vector<Flashcard>& flashcards, int count) {
    if (count <= 0) return;

    Flashcard fc;

    cout << "\nEnter question: ";
    getline(cin, fc.question);

    cout << "Enter answer: ";
    getline(cin, fc.answer);

    flashcards.push_back(fc);

    inputFlashcards(flashcards, count - 1); 
}

// Stores the questions and answers entered by the user in the "questions.txt" and "answers.txt" files
void saveFlashcards(const vector<Flashcard>& flashcards, bool append = false) {
    ofstream qfile("questions.txt", append ? ios::app : ios::out);
    ofstream afile("answers.txt", append ? ios::app : ios::out);

    if (!qfile || !afile) {
        cerr << "Failed to open one or both files!\n";
        return;
    }

    for (const auto& fc : flashcards) {
        qfile << fc.question << "\n";
        afile << fc.answer << "\n";
    }

    qfile.close();
    afile.close();
}

// Accesses the questions and their matching answers from each file
void loadFlashcards(vector<Flashcard>& flashcards) {
    ifstream qfile("questions.txt");
    ifstream afile("answers.txt");

    if (!qfile || !afile) return;

    string q, a;
    while (getline(qfile, q) && getline(afile, a)) {
        flashcards.push_back({ q, a });
    }

    qfile.close();
    afile.close();
}

// Shuffles the questions so when the user is testing themselves it is outputted in random order
void shuffleFlashcards(vector<Flashcard>& flashcards) {
    srand((unsigned)time(0));
    random_shuffle(flashcards.begin(), flashcards.end());
}

// Outputs the quesitons and after the user hits ENTER the answer is revealed
// Continues until user enters 'n' when asked to continue or all the flashcards are used
void runSession(vector<Flashcard>& flashcards) {
    if (flashcards.empty()) {
        cout << "No flashcards available.\n";
        return;
    }

    shuffleFlashcards(flashcards);

    cin.ignore();

    for (int i = 0; i < flashcards.size(); ++i) {
        cout << "\nQuestion: " << flashcards[i].question << endl;
        cout << "Press Enter to see the answer...";
        cin.get();

        cout << "Answer: " << flashcards[i].answer << endl;

        if (i < flashcards.size() - 1) {
            cout << "Continue? (y/n): ";
            char choice;
            cin >> choice;
            cin.ignore();
            if (tolower(choice) == 'n') break;
        }
    }

    cout << "\nSession ended.\n";
}

// Empties the memory of the questions and answer files
void clearFlashcards() {
    ofstream qfile("questions.txt", ios::trunc);
    ofstream afile("answers.txt", ios::trunc);
    qfile.close();
    afile.close();
    cout << "All flashcards have been deleted.\n";
}

// Main Function
int main() {
    vector<Flashcard> flashcards;
    loadFlashcards(flashcards);

    cout << "Welcome to the Flashcard App!\n";

    while (true) {
        cout << "\nMenu:\n"
            << "1. Add new flashcards\n"
            << "2. Start flashcard session\n"
            << "3. Clear all flashcards\n"
            << "4. Exit\n"
            << "\nChoose an option: ";

        int choice;
        cin >> choice;
        cin.ignore();

        switch (choice) {
        case 1: {
            int count;
            cout << "\nHow many flashcards would you like to add?: ";

            while (!(cin >> count)) {
                cin.clear(); 
                cin.ignore(1000, '\n'); 
                cout << "Invalid input. Please enter a number: ";
            }

            cin.ignore();
            cout << "\n";

            vector<Flashcard> newCards;
            inputFlashcards(newCards, count);
            saveFlashcards(newCards, true);
            flashcards.insert(flashcards.end(), newCards.begin(), newCards.end());
            cout << "------------------------------------------------------------------------------------------------------------------------";
            break;
        }
        case 2:
            runSession(flashcards);
            cout << "------------------------------------------------------------------------------------------------------------------------";
            break;
        case 3:
            clearFlashcards();
            flashcards.clear();
            cout << "------------------------------------------------------------------------------------------------------------------------";
            break;
        case 4:
            cout << "Goodbye!\n";
            return 0;
        default:
            cout << "Invalid choice. Please try again.\n";
        }
    }

    return 0;
}
