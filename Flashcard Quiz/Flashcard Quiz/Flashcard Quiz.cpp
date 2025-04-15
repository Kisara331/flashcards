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

void inputFlashcards(vector<Flashcard>& flashcards, int count) {
    if (count <= 0) return;

    Flashcard fc;

    if (cin.peek() == '\n') cin.ignore();

    cout << "\nEnter question: ";
    getline(cin, fc.question);

    cout << "Enter answer: ";
    getline(cin, fc.answer);

    flashcards.push_back(fc);

    inputFlashcards(flashcards, count - 1); // Recursive call
}

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

void shuffleFlashcards(vector<Flashcard>& flashcards) {
    srand((unsigned)time(0));
    random_shuffle(flashcards.begin(), flashcards.end());
}

void runSession(vector<Flashcard>& flashcards) {
    if (flashcards.empty()) {
        cout << "No flashcards available.\n";
        return;
    }

    shuffleFlashcards(flashcards);

    cin.ignore(); // Clear leftover newline before first Enter

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

void clearFlashcards() {
    ofstream qfile("questions.txt", ios::trunc);
    ofstream afile("answers.txt", ios::trunc);
    qfile.close();
    afile.close();
    cout << "All flashcards have been deleted.\n";
}

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
            << "Choose an option: ";

        int choice;
        cin >> choice;

        switch (choice) {
        case 1: {
            int count;
            cout << "\nHow many flashcards would you like to add? ";
            cin >> count;
            cout << "\n";

            vector<Flashcard> newCards;
            inputFlashcards(newCards, count); // recursive input
            saveFlashcards(newCards, true);
            flashcards.insert(flashcards.end(), newCards.begin(), newCards.end());
            break;
        }
        case 2:
            runSession(flashcards);
            break;
        case 3:
            clearFlashcards();
            flashcards.clear();
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
